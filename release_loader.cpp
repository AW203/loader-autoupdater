#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shell32.lib")

// Current version of this loader
#define VERSION "1.0.0"

// GitHub repository details
const std::string GITHUB_USER = "AW203";
const std::string GITHUB_REPO = "loader-autoupdater";
const std::string VERSION_FILE_URL = "https://raw.githubusercontent.com/" + GITHUB_USER + "/" + GITHUB_REPO + "/main/version.txt";
const std::string DOWNLOAD_URL = "https://github.com/" + GITHUB_USER + "/" + GITHUB_REPO + "/releases/latest/download/loader.exe";

// Function to fetch content from a URL
std::string fetchFromUrl(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("AutoUpdater", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cout << "Failed to open internet connection" << std::endl;
        return "";
    }

    HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        std::cout << "Failed to open URL: " << url << std::endl;
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

// Function to download a file from a URL to a local path
bool downloadFile(const std::string& url, const std::string& path) {
    HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL);
    return SUCCEEDED(hr);
}

// Function to get local app data directory
std::string getAppDataDir() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        return std::string(path) + "\\AutoUpdateLoader";
    }
    // Fallback to current directory if app data can't be accessed
    return ".";
}

// Function to create directory if it doesn't exist
void createDirIfNotExists(const std::string& dir) {
    CreateDirectoryA(dir.c_str(), NULL);
}

// Function to check for and apply updates
bool checkAndUpdate() {
    std::cout << "[*] Checking for updates..." << std::endl;
    
    // Get remote version
    std::string remoteVersion = fetchFromUrl(VERSION_FILE_URL);
    if (remoteVersion.empty()) {
        std::cout << "[!] Failed to get remote version." << std::endl;
        return false;
    }
    
    std::cout << "[*] Current version: " << VERSION << std::endl;
    std::cout << "[*] Latest version: " << remoteVersion << std::endl;
    
    // Compare versions
    if (remoteVersion == VERSION) {
        std::cout << "[=] You are already running the latest version." << std::endl;
        return false;
    }
    
    std::cout << "[+] Update available! Downloading..." << std::endl;
    
    // Create application directory
    std::string appDir = getAppDataDir();
    createDirIfNotExists(appDir);
    
    // Download new version
    std::string downloadPath = appDir + "\\loader_new.exe";
    if (!downloadFile(DOWNLOAD_URL, downloadPath)) {
        std::cout << "[!] Failed to download update." << std::endl;
        return false;
    }
    
    std::cout << "[+] Download complete. Creating update script..." << std::endl;
    
    // Create updater script
    std::string updaterPath = appDir + "\\updater.bat";
    std::string exePath = appDir + "\\loader.exe";
    
    std::ofstream updater(updaterPath);
    updater << "@echo off" << std::endl;
    updater << "echo Updating..." << std::endl;
    updater << "timeout /t 1 /nobreak > nul" << std::endl;
    updater << "if exist \"" << exePath << "\" del \"" << exePath << "\"" << std::endl;
    updater << "copy \"" << downloadPath << "\" \"" << exePath << "\"" << std::endl;
    updater << "del \"" << downloadPath << "\"" << std::endl;
    updater << "echo Update completed!" << std::endl;
    updater << "start \"\" \"" << exePath << "\"" << std::endl;
    updater << "del \"%~f0\"" << std::endl;
    updater.close();
    
    std::cout << "[+] Update ready. The application will restart after update." << std::endl;
    
    // Launch updater and exit
    ShellExecuteA(NULL, "open", updaterPath.c_str(), NULL, NULL, SW_SHOW);
    return true;
}

int main() {
    std::cout << "======================================" << std::endl;
    std::cout << "            AUTO-UPDATE LOADER        " << std::endl;
    std::cout << "              Version " << VERSION << "            " << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;
    
    // Check for updates first
    if (checkAndUpdate()) {
        // If update is available and downloaded, exit to let the updater work
        std::cout << "[*] Exiting for update. Application will restart automatically." << std::endl;
        return 0;
    }
    
    // No update or update failed, continue with normal operation
    std::string licenseKey;
    std::cout << "\nPlease enter your license key: ";
    std::cin >> licenseKey;
    
    // Simple license check (replace with your real check)
    if (licenseKey == "DEMO-KEY-123") {
        std::cout << "\n[+] License valid! Welcome to the application." << std::endl;
        
        // Main application functionality would go here
        std::cout << "\nApplication is running..." << std::endl;
    } else {
        std::cout << "\n[!] Invalid license key!" << std::endl;
    }
    
    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();
    
    return 0;
} 