#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#include <thread>
#include <filesystem>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shell32.lib")

// Include your existing matrix loader headers here
// #include "protection.h"
// #include "dye.h"
// #include "skCrypt.h"
// #include "KeyAuth.hpp"
// ... etc

// Current version of this loader
#define VERSION "0.9.0"
#define SOFTWARE_TITLE "Matrix Auto-Update Loader"

// GitHub repository details
const std::string GITHUB_USER = "YourGitHubUsername";
const std::string GITHUB_REPO = "matrix-loader";
// API URL to get latest release info
const std::string GITHUB_API_URL = "https://api.github.com/repos/" + GITHUB_USER + "/" + GITHUB_REPO + "/releases/latest";

// Global variables from your existing loader
struct {
    std::string brandName;
    // Add other global variables from your original code here
} GameVars;

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
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Fetching data from: " << url << std::flush;
    
    HINTERNET hInternet = InternetOpenA("MatrixUpdater", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Failed to open internet connection." << std::endl;
        return "";
    }

    // Set up the request headers for GitHub API
    HINTERNET hFile;
    if (url.find("api.github.com") != std::string::npos) {
        // Add User-Agent header for GitHub API
        const char* headers = "User-Agent: MatrixLoader\r\n";
        hFile = InternetOpenUrlA(hInternet, url.c_str(), headers, strlen(headers), INTERNET_FLAG_RELOAD, 0);
    } else {
        hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    }
    
    if (!hFile) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Failed to open URL" << std::endl;
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
    
    return result;
}

// Function to download a file from a URL to a local path
bool downloadFile(const std::string& url, const std::string& path) {
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Downloading update..." << std::flush;
    
    HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL);
    if (SUCCEEDED(hr)) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Download completed successfully!" << std::endl;
        return true;
    } else {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Download failed with error code: " << hr << std::endl;
        return false;
    }
}

// Function to get local app data directory
std::string getAppDataDir() {
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        std::string appDir = std::string(path) + "\\MatrixLoader";
        return appDir;
    }
    // Fallback to current directory if app data can't be accessed
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
    return "matrix_loader_new.exe"; // Default fallback name
}

// Function to check for and apply updates
bool checkAndUpdate() {
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Checking for updates..." << std::endl;
    
    // Get remote version from GitHub API
    std::string apiResponse = fetchFromUrl(GITHUB_API_URL);
    if (apiResponse.empty()) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Failed to get API response from GitHub." << std::endl;
        return false;
    }
    
    std::string remoteVersion = extractVersionFromAPI(apiResponse);
    if (remoteVersion.empty()) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Failed to extract version from GitHub API response." << std::endl;
        return false;
    }
    
    std::string downloadURL = extractDownloadURLFromAPI(apiResponse);
    if (downloadURL.empty()) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Failed to extract download URL from GitHub API response." << std::endl;
        return false;
    }
    
    std::string remoteFilename = extractFilenameFromURL(downloadURL);
    
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Current version: " << VERSION << std::endl;
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Latest version: " << remoteVersion << std::endl;
    
    // Compare versions
    if (remoteVersion == VERSION) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " You are already running the latest version." << std::endl;
        return false;
    }
    
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Update available! Downloading..." << std::endl;
    
    // Create application directory
    std::string appDir = getAppDataDir();
    createDirIfNotExists(appDir);
    
    // Download new version
    std::string downloadPath = appDir + "\\matrix_loader_new.exe";
    if (!downloadFile(downloadURL, downloadPath)) {
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Failed to download update." << std::endl;
        return false;
    }
    
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Download complete. Creating update script..." << std::endl;
    
    // Create updater script
    std::string updaterPath = appDir + "\\updater.bat";
    std::string exePath = appDir + "\\matrix_loader.exe";
    
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
    std::string newVersionedFilename = currentDir + "matrix_loader_" + remoteVersion + ".exe";
    
    std::ofstream updater(updaterPath);
    updater << "@echo off" << std::endl;
    updater << "echo Updating Matrix Loader to version " << remoteVersion << "..." << std::endl;
    
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
    updater << "  ren \"" << currentExePath << "\" old_matrix_loader_to_delete.exe" << std::endl;
    updater << "  del /F /Q old_matrix_loader_to_delete.exe" << std::endl;
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
    
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Update ready. The application will restart after update." << std::endl;
    
    // Launch updater and exit
    ShellExecuteA(NULL, "open", updaterPath.c_str(), NULL, NULL, SW_SHOW);
    return true;
}

// Main function that integrates auto-update with your existing Matrix loader
int main()
{
    // Set up your brand name for use in both auto-update and matrix loader
    GameVars.brandName = SOFTWARE_TITLE;
    
    // Auto-update check first
    std::cout << "\r" << "/* MATRIX UPDATE */ " << " Initializing " << GameVars.brandName << " version " << VERSION << std::endl;
    
    // Check for updates first
    if (checkAndUpdate()) {
        // If update is available and downloaded, exit immediately to let the updater work
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Exiting for update. Application will restart automatically." << std::endl;
        // No need to wait for user input here - exit immediately
        return 0;
    }
    
    // Get remote version to verify if an update was detected but failed
    std::string apiResponse = fetchFromUrl(GITHUB_API_URL);
    std::string remoteVersion = extractVersionFromAPI(apiResponse);
    if (!remoteVersion.empty() && remoteVersion != VERSION) {
        // If remote version is different but we got here, update failed
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Update was detected but failed to apply." << std::endl;
        std::cout << "\r" << "/* MATRIX UPDATE */ " << " Please check your internet connection or download manually." << std::endl;
    }
    
    // ------------------------------------------------------------
    // BEGIN ORIGINAL MATRIX LOADER CODE
    // ------------------------------------------------------------
    
    // Include your existing code here, with minor adjustments to work with the auto-update system
    
    PROTECTION::startProtectionMeasures();

    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

    SetConsoleTitleA(GameVars.brandName.c_str());
    std::cout << dye::yellow("(x)") << skCrypt(" Connecting to authentication servers") << std::flush;
    KeyAuthApp.init();
    //system("CLS");
    if (!KeyAuthApp.response.success)
    {
        std::cout << "\r" << dye::red("(-)") << skCrypt(" Error connecting to authentication servers. Exiting...") << std::endl;
        Sleep(4000);
        exit(1);
    }

    std::cout << "\r" << dye::green("(+)") << skCrypt(" Connected to authentication servers         ") << std::endl;
    if (std::filesystem::exists(skCrypt("license.json").decrypt()))
    {
        if (CheckIfJsonKeyExists(skCrypt("license.json").decrypt(), skCrypt("license").decrypt()))
        {
            std::string key = ReadFromJson(skCrypt("license.json").decrypt(), skCrypt("license").decrypt());
            KeyAuthApp.license(key);
            if (!KeyAuthApp.response.success)
            {
                std::remove(skCrypt("license.json"));
                std::cout << "\r" << dye::red("(-)") << skCrypt(" Saved license key is invalid. Exiting... ") << std::endl;
                Sleep(4000);
                exit(1);
            }
            std::cout << "\r" << dye::green("(+)") << skCrypt(" Automatically logged in") << std::endl;
        }
    } else {
        std::cout << dye::yellow("(x)") << skCrypt(" Enter your license key: ") << std::flush;

        int option;
        std::string key;
        std::cin >> key;
        KeyAuthApp.license(key);

        if (KeyAuthApp.response.message.empty()) exit(11);
        if (!KeyAuthApp.response.success)
        {
            std::cout << "\r" << dye::red("(-)") << skCrypt(" License invalid. Exiting...                                                                          ") << std::endl;
            Sleep(4000);
            exit(1);
        }
        else
        {
            WriteToJson(skCrypt("license.json").decrypt(), skCrypt("license").decrypt(), key, false, "", "");
        }
        std::cout << "\r" << dye::green("(+)") << skCrypt(" Successfully logged in                                                                       ") << std::endl;
    }

    /*
    * Do NOT remove this checkAuthenticated() function.
    * It protects you from cracking, it would be NOT be a good idea to remove it
    */

    std::thread run(checkAuthenticated, ownerid);
    std::thread check(sessionStatus);

    std::cout << "\r" << dye::green("(+)") << skCrypt(" License expires on ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.user_data.subscriptions[0].expiry))) << std::endl;

    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL)) {
        std::cout << dye::yellow("(!)") << skCrypt(" Failed to set main thread priority to critical") << std::endl;
    }

    std::cout << dye::yellow("(x)") << skCrypt(" Initializing DMA") << std::flush;

    bool initialized = mem.Init(skCrypt("FortniteClient-Win64-Shipping.exe").decrypt(), true, false);
    while (!initialized)
    {
        std::cout << "\r" << dye::red("(-)") << skCrypt(" Failed to initialize DMA. Is Fortnite open? Retrying...") << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        initialized = mem.Init(skCrypt("FortniteClient-Win64-Shipping.exe").decrypt(), true, false);
    }

    std::cout << "\r" << dye::green("(+)") << skCrypt(" DMA Initialized                                        ") << std::endl;

    std::cout << dye::yellow("(x)") << skCrypt(" Initializing keyboard") << std::flush;

    if (!mem.GetKeyboard()->InitKeyboard())
    {
        std::cout << "\r" << dye::red("(-)") << skCrypt(" Failed to initialize keyboard. Exiting...") << std::endl;
        Sleep(4000);
        exit(1);
    }

    std::cout << "\r" << dye::green("(+)") << skCrypt(" Keyboard initialized    ") << std::endl;

    std::cout << dye::yellow("(x)") << skCrypt(" Initializing aim device") << std::flush;

    if (!AIMBOT::kmBoxInit()) {
        std::cout << "\r" << dye::yellow("(!)") << skCrypt(" Failed to initialize aim device. Continuing without aimbot features") << std::endl;
    }
    else {
        std::cout << "\r" << dye::green("(+)") << skCrypt(" Aim Device initialized                                                      ") << std::endl;
    }

    std::cout << dye::yellow("(x)") << skCrypt(" Analysing game changes") << std::flush;

    if (!updateOffsets()) {
        std::cout << "\r" << dye::red("(-)") << skCrypt(" Failed to fetch game changes. Exiting...") << std::endl;
        Sleep(4000);
        exit(1);
    }

    std::cout << "\r" << dye::green("(+)") << skCrypt(" Game changes successfully accounted for") << std::endl;

    std::cout << dye::yellow("(x)") << skCrypt(" Loading config") << std::flush;

    GameSetting.loadFromConfig(GameVars.config);

    std::cout << "\r" << dye::green("(+)") << skCrypt(" Config loaded   ") << std::endl;

    std::cout << dye::green("(+)") << skCrypt(" Starting ") << GameVars.brandName << std::endl;

    Render::CreateOverlay();
    Render::DirectXInit();

    mem.FullRefresh();

    std::thread refreshThread(refresh_memory);
    refreshThread.detach();

    std::thread gameLoop(GAME::loop);
    gameLoop.detach();

    Render::MainLoop();
    
    // ------------------------------------------------------------
    // END ORIGINAL MATRIX LOADER CODE
    // ------------------------------------------------------------

    return 0;
} 