#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#include <ctime>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")

class AutoUpdater {
private:
    std::string currentVersion;
    std::string remoteVersion;
    std::string remoteVersionUrl;
    std::string remoteFileUrl;
    std::string localFilePath;
    std::string versionFilePath;
    std::string tempDownloadPath;
    std::string appName;

public:
    AutoUpdater(const std::string& appName, 
                const std::string& remoteVersionUrl, 
                const std::string& remoteFileUrl) 
        : appName(appName),
          remoteVersionUrl(remoteVersionUrl),
          remoteFileUrl(remoteFileUrl) {
        
        // Create folder to store application files
        char appDataPath[MAX_PATH];
        SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath);
        
        std::string basePath = std::string(appDataPath) + "\\" + appName;
        CreateDirectoryA(basePath.c_str(), NULL);
        
        // Define file paths
        localFilePath = basePath + "\\loader.exe";
        versionFilePath = basePath + "\\version.txt";
        tempDownloadPath = basePath + "\\update_temp.exe";
        
        // Read local version
        loadLocalVersion();
    }

    std::string fetchRemoteContent(const std::string& url) {
        HINTERNET hInternet = InternetOpenA("AutoUpdater", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return "";

        HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hFile) {
            InternetCloseHandle(hInternet);
            return "";
        }

        char buffer[1024];
        DWORD bytesRead;
        std::string result;

        while (InternetReadFile(hFile, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            result += buffer;
        }

        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        
        // Clean the string (remove whitespace and newlines)
        while (!result.empty() && (result.back() == '\n' || result.back() == '\r' || result.back() == ' ')) {
            result.pop_back();
        }
        
        return result;
    }

    bool downloadFile(const std::string& url, const std::string& path) {
        HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL);
        return SUCCEEDED(hr);
    }

    void loadLocalVersion() {
        std::ifstream infile(versionFilePath);
        if (infile.is_open()) {
            std::getline(infile, currentVersion);
            infile.close();
        } else {
            // If version file doesn't exist, we consider this is the first installation
            currentVersion = "0.0.0";
            std::ofstream outfile(versionFilePath);
            outfile << currentVersion;
            outfile.close();
        }
    }

    void saveLocalVersion(const std::string& version) {
        std::ofstream outfile(versionFilePath);
        outfile << version;
        outfile.close();
        currentVersion = version;
    }

    bool checkForUpdate() {
        std::cout << "[*] Checking for updates..." << std::endl;
        
        remoteVersion = fetchRemoteContent(remoteVersionUrl);
        if (remoteVersion.empty()) {
            std::cout << "[!] Unable to check for updates. Check your internet connection." << std::endl;
            return false;
        }

        std::cout << "[*] Local version: " << currentVersion << std::endl;
        std::cout << "[*] Available version: " << remoteVersion << std::endl;

        return remoteVersion != currentVersion;
    }

    bool update() {
        if (!checkForUpdate()) {
            std::cout << "[=] The loader is up to date." << std::endl;
            return false;
        }

        std::cout << "[+] Update available. Downloading..." << std::endl;
        
        if (!downloadFile(remoteFileUrl, tempDownloadPath)) {
            std::cout << "[!] Failed to download the update." << std::endl;
            return false;
        }

        // Replace the old file with the new one
        // We need to first rename the old file and then copy the new one to avoid issues
        std::string backupPath = localFilePath + ".bak";
        
        // Delete old backup if it exists
        DeleteFileA(backupPath.c_str());
        
        // Rename old file to backup
        MoveFileA(localFilePath.c_str(), backupPath.c_str());
        
        // Move the new file to the target location
        if (!MoveFileA(tempDownloadPath.c_str(), localFilePath.c_str())) {
            // In case of failure, restore the old file
            MoveFileA(backupPath.c_str(), localFilePath.c_str());
            std::cout << "[!] Failed to install the update." << std::endl;
            return false;
        }

        // Update version file
        saveLocalVersion(remoteVersion);
        std::cout << "[+] Successfully updated to version " << remoteVersion << "." << std::endl;
        
        return true;
    }

    bool selfUpdate() {
        // For auto-updating a running application
        if (!checkForUpdate()) {
            return false;
        }

        std::string updaterBatchPath = localFilePath + "_updater.bat";
        std::ofstream batchFile(updaterBatchPath);
        
        // Create a batch script to update the application after it closes
        batchFile << "@echo off" << std::endl;
        batchFile << "timeout /t 1 /nobreak > nul" << std::endl;
        batchFile << "echo Updating..." << std::endl;
        batchFile << "if exist \"" << tempDownloadPath << "\" (" << std::endl;
        batchFile << "    if exist \"" << localFilePath << "\" del \"" << localFilePath << "\"" << std::endl;
        batchFile << "    move \"" << tempDownloadPath << "\" \"" << localFilePath << "\"" << std::endl;
        batchFile << ")" << std::endl;
        batchFile << "echo Update completed." << std::endl;
        batchFile << "start \"\" \"" << localFilePath << "\"" << std::endl;
        batchFile << "del \"" << updaterBatchPath << "\"" << std::endl;
        batchFile.close();

        // Download the update
        if (!downloadFile(remoteFileUrl, tempDownloadPath)) {
            DeleteFileA(updaterBatchPath.c_str());
            return false;
        }

        // Save the new version
        saveLocalVersion(remoteVersion);

        // Run the batch script and close the application
        ShellExecuteA(NULL, "open", updaterBatchPath.c_str(), NULL, NULL, SW_HIDE);
        return true;
    }
};

// Example function that simulates license verification
bool checkLicense(const std::string& licenseKey) {
    // Here, you would implement your own license verification logic
    return licenseKey == "DEMO-KEY-123";
}

int main() {
    // Auto-updater configuration
    // Replace these URLs with your own pointing to your GitHub repo
    AutoUpdater updater(
        "AutoUpdateLoader", 
        "https://raw.githubusercontent.com/AW203/loader-autoupdater/main/version.txt",
        "https://github.com/AW203/loader-autoupdater/releases/latest/download/loader.exe"
    );
    
    // Check and install updates
    updater.update();
    
    // After updating, continue with license verification
    std::string licenseKey;
    std::cout << "\n=== LOADER ===\n" << std::endl;
    std::cout << "Please enter your license key: ";
    std::cin >> licenseKey;
    
    if (checkLicense(licenseKey)) {
        std::cout << "\n[+] Valid license! Starting loader..." << std::endl;
        // Here you can start the main functionality of the loader
    } else {
        std::cout << "\n[!] Invalid license!" << std::endl;
    }
    
    std::cout << "\nPress any key to exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();
    
    return 0;
} 