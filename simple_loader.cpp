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
#define VERSION "0.9.0"

// GitHub repository details
const std::string GITHUB_USER = "AW203";
const std::string GITHUB_REPO = "loader-autoupdater";
// API URL to get latest release info
const std::string GITHUB_API_URL = "https://api.github.com/repos/" + GITHUB_USER + "/" + GITHUB_REPO + "/releases/latest";

// Function to extract version from GitHub API response
std::string extractVersionFromAPI(const std::string& apiResponse) {
    // Look for "tag_name":" pattern in the JSON response
    std::string searchPattern = "\"tag_name\":\"";
    size_t tagPos = apiResponse.find(searchPattern);
    if (tagPos == std::string::npos) {
        return "";
    }
    
    // Move position to start of the actual version
    tagPos += searchPattern.length();
    
    // Find the closing quote
    size_t endPos = apiResponse.find("\"", tagPos);
    if (endPos == std::string::npos) {
        return "";
    }
    
    // Extract the version string (remove 'v' prefix if present)
    std::string version = apiResponse.substr(tagPos, endPos - tagPos);
    if (!version.empty() && version[0] == 'v') {
        version = version.substr(1);
    }
    
    return version;
}

// Function to extract download URL from GitHub API response
std::string extractDownloadURLFromAPI(const std::string& apiResponse) {
    // Look for "browser_download_url":" pattern in the JSON response
    std::string searchPattern = "\"browser_download_url\":\"";
    size_t urlPos = apiResponse.find(searchPattern);
    if (urlPos == std::string::npos) {
        return "";
    }
    
    // Move position to start of the actual URL
    urlPos += searchPattern.length();
    
    // Find the closing quote
    size_t endPos = apiResponse.find("\"", urlPos);
    if (endPos == std::string::npos) {
        return "";
    }
    
    // Extract the download URL
    return apiResponse.substr(urlPos, endPos - urlPos);
}

// Function to fetch content from a URL
std::string fetchFromUrl(const std::string& url) {
    std::cout << "[Debug] Attempting to fetch from URL: " << url << std::endl;
    
    HINTERNET hInternet = InternetOpenA("AutoUpdater", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cout << "[Debug] Failed to open internet connection. Error: " << GetLastError() << std::endl;
        return "";
    }

    // Set up the request headers for GitHub API
    HINTERNET hFile;
    if (url.find("api.github.com") != std::string::npos) {
        // Add User-Agent header for GitHub API
        const char* headers = "User-Agent: AutoUpdateLoader\r\n";
        hFile = InternetOpenUrlA(hInternet, url.c_str(), headers, strlen(headers), INTERNET_FLAG_RELOAD, 0);
    } else {
        hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    }
    
    if (!hFile) {
        std::cout << "[Debug] Failed to open URL. Error: " << GetLastError() << std::endl;
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
    
    std::cout << "[Debug] API response length: " << result.length() << " bytes" << std::endl;
    // Don't print the entire response as it can be very long
    if (result.length() > 0) {
        std::cout << "[Debug] API response starts with: " << result.substr(0, 100) << "..." << std::endl;
    }
    
    return result;
}

// Function to download a file from a URL to a local path
bool downloadFile(const std::string& url, const std::string& path) {
    std::cout << "[Debug] Attempting to download file from: " << url << std::endl;
    std::cout << "[Debug] Saving to: " << path << std::endl;
    
    HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL);
    if (SUCCEEDED(hr)) {
        std::cout << "[Debug] Download succeeded!" << std::endl;
        return true;
    } else {
        std::cout << "[Debug] Download failed with error code: " << hr << std::endl;
        return false;
    }
}

// Function to get local app data directory
std::string getAppDataDir() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        std::string appDir = std::string(path) + "\\AutoUpdateLoader";
        std::cout << "[Debug] App data directory: " << appDir << std::endl;
        return appDir;
    }
    // Fallback to current directory if app data can't be accessed
    std::cout << "[Debug] Failed to get AppData path, using current directory." << std::endl;
    return ".";
}

// Function to create directory if it doesn't exist
void createDirIfNotExists(const std::string& dir) {
    CreateDirectoryA(dir.c_str(), NULL);
}

// Function to extract filename from URL
std::string extractFilenameFromURL(const std::string& url) {
    size_t lastSlash = url.find_last_of('/');
    if (lastSlash != std::string::npos && lastSlash < url.length() - 1) {
        return url.substr(lastSlash + 1);
    }
    return "loader_new.exe"; // Default fallback name
}

// Function to check for and apply updates
bool checkAndUpdate() {
    std::cout << "[*] Checking for updates..." << std::endl;
    
    // Get remote version from GitHub API
    std::string apiResponse = fetchFromUrl(GITHUB_API_URL);
    if (apiResponse.empty()) {
        std::cout << "[!] Failed to get API response from GitHub." << std::endl;
        return false;
    }
    
    std::string remoteVersion = extractVersionFromAPI(apiResponse);
    if (remoteVersion.empty()) {
        std::cout << "[!] Failed to extract version from GitHub API response." << std::endl;
        return false;
    }
    
    std::string downloadURL = extractDownloadURLFromAPI(apiResponse);
    if (downloadURL.empty()) {
        std::cout << "[!] Failed to extract download URL from GitHub API response." << std::endl;
        return false;
    }
    
    std::string remoteFilename = extractFilenameFromURL(downloadURL);
    
    std::cout << "[*] Current version: " << VERSION << std::endl;
    std::cout << "[*] Latest version: " << remoteVersion << std::endl;
    std::cout << "[*] Download URL: " << downloadURL << std::endl;
    std::cout << "[*] Remote filename: " << remoteFilename << std::endl;
    
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
    if (!downloadFile(downloadURL, downloadPath)) {
        std::cout << "[!] Failed to download update." << std::endl;
        return false;
    }
    
    std::cout << "[+] Download complete. Creating update script..." << std::endl;
    
    // Create updater script
    std::string updaterPath = appDir + "\\updater.bat";
    std::string exePath = appDir + "\\loader.exe";
    
    // Get current executable path
    char currentExePath[MAX_PATH];
    GetModuleFileNameA(NULL, currentExePath, MAX_PATH);
    
    // Extract filename only for taskkill
    std::string currentExeFile = strrchr(currentExePath, '\\') ? strrchr(currentExePath, '\\') + 1 : currentExePath;
    
    // Get directory of current executable
    std::string currentDir = std::string(currentExePath);
    size_t lastBackslash = currentDir.find_last_of('\\');
    if (lastBackslash != std::string::npos) {
        currentDir = currentDir.substr(0, lastBackslash + 1);
    } else {
        currentDir = ".\\";
    }
    
    // Create new filename with version
    std::string newVersionedFilename = currentDir + "loader_" + remoteVersion + ".exe";
    
    std::ofstream updater(updaterPath);
    updater << "@echo off" << std::endl;
    updater << "echo Updating to version " << remoteVersion << "..." << std::endl;
    
    // Try to kill the original process if it's still running
    updater << "echo Attempting to close any running instances..." << std::endl;
    updater << "taskkill /F /IM " << currentExeFile << " 2>nul" << std::endl;
    updater << "timeout /t 2 /nobreak > nul" << std::endl;
    
    // Wait to make sure the original process is closed
    updater << "echo Waiting for processes to close..." << std::endl;
    updater << "timeout /t 3 /nobreak > nul" << std::endl;
    
    // Forcefully delete old executable (using permanent deletion)
    updater << "echo Removing old version..." << std::endl;
    updater << "if exist \"" << exePath << "\" del /F /Q \"" << exePath << "\"" << std::endl;
    updater << "if exist \"" << currentExePath << "\" del /F /Q \"" << currentExePath << "\"" << std::endl;
    
    // If deletion failed, try to move the file instead
    updater << "if exist \"" << currentExePath << "\" (" << std::endl;
    updater << "  echo Direct deletion failed, trying alternative method..." << std::endl;
    updater << "  ren \"" << currentExePath << "\" old_loader_to_delete.exe" << std::endl;
    updater << "  del /F /Q old_loader_to_delete.exe" << std::endl;
    updater << ")" << std::endl;
    
    // Copy new version
    updater << "echo Installing new version..." << std::endl;
    updater << "copy \"" << downloadPath << "\" \"" << exePath << "\"" << std::endl;
    
    // Also copy to original location with versioned name
    updater << "echo Copying to original location with versioned name..." << std::endl;
    updater << "copy \"" << downloadPath << "\" \"" << newVersionedFilename << "\" 2>nul" << std::endl;
    
    // Clean up downloaded file
    updater << "del /F /Q \"" << downloadPath << "\"" << std::endl;
    
    updater << "echo Update completed!" << std::endl;
    
    // Launch new version (prefer original location if successful, otherwise AppData)
    updater << "if exist \"" << newVersionedFilename << "\" (" << std::endl;
    updater << "  start \"\" \"" << newVersionedFilename << "\"" << std::endl;
    updater << ") else (" << std::endl;
    updater << "  start \"\" \"" << exePath << "\"" << std::endl;
    updater << ")" << std::endl;
    
    // Self-destruct the updater script
    updater << "echo Cleaning up..." << std::endl;
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
        // If update is available and downloaded, exit immediately to let the updater work
        std::cout << "[*] Exiting for update. Application will restart automatically." << std::endl;
        // No need to wait for user input here - exit immediately
        return 0;
    }
    
    // Get remote version to verify if an update was detected but failed
    std::string apiResponse = fetchFromUrl(GITHUB_API_URL);
    std::string remoteVersion = extractVersionFromAPI(apiResponse);
    if (!remoteVersion.empty() && remoteVersion != VERSION) {
        // If remote version is different but we got here, update failed
        std::cout << "[!] Update was detected but failed to apply." << std::endl;
        std::cout << "[!] Please check your internet connection or download manually." << std::endl;
        std::cout << "\nPress Enter to continue anyway..." << std::endl;
        std::cin.ignore();
        std::cin.get();
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