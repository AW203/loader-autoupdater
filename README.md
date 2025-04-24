# Auto-Update System for Loaders

This repository contains a C++ auto-update system designed to be easily integrated into existing loaders or applications.

## How the System Works

The auto-update system allows your application to:

1. Automatically check if a new version is available using the GitHub API
2. Download the new version from GitHub releases (supports any filename)
3. Install the update and restart the application
4. Display version information in the EXE file properties

## Configuration and Usage

### Prerequisites

- Any C++ compiler (GCC, MinGW, MSVC, etc.)
- The compiled system works on Windows without any additional dependencies

### Installation and Compilation

1. Clone or download this repository:
   ```
   git clone https://github.com/AW203/loader-autoupdater.git
   ```
   Or simply download as ZIP from GitHub

2. **Important**: Modify the batch files to contain the path to your folder:
   - Open the batch files (`compile_auto.bat`, `compile_release_auto.bat`, etc.)
   - Change the line `cd C:\Users\Yayam\Music\loader-autoupdate` to point to your folder

3. Compilation Options with Automatic Version Detection:
   - For both versions at once: run `compileALL_auto.bat`
   - For base version only: run `compile_auto.bat`
   - For release version only: run `compile_release_auto.bat`

### Adapting to Your Project

To customize this auto-update system for your own project, you'll need to modify:

1. **GitHub Repository Information** (in both `simple_loader.cpp` and `release_loader.cpp`):
   ```cpp
   // Change these lines to your GitHub username and repository
   const std::string GITHUB_USER = "YourGitHubUsername";
   const std::string GITHUB_REPO = "YourRepoName";
   ```

2. **Version Numbers**:
   - In `simple_loader.cpp`: Change `#define VERSION "0.9.0"` to your base version
   - In `release_loader.cpp`: Change `#define VERSION "1.0.0"` to your release version
   - In `loader_version.rc` and `release_version.rc`: Update the version information
   - The scripts will automatically extract and use these versions

3. **Application Name** (in both `.rc` files):
   ```
   #define VER_PRODUCTNAME_STR "Your App Name\0"
   #define VER_FILEDESCRIPTION_STR "Your App Description\0"
   ```

4. **Storage Location**:
   - In both loader files, modify `getAppDataDir()` to change the folder name from "AutoUpdateLoader" to your application name
   ```cpp
   // Change "AutoUpdateLoader" to your app name
   return std::string(path) + "\\YourAppName";
   ```

5. **License Check Logic** (optional):
   - In both files, modify the license checking code to match your requirements
   - The current code has a simple demo key "DEMO-KEY-123"

### GitHub Configuration

1. Create a release on GitHub:
   - Go to your repository → Releases → Create a new release
   - Tag version: v1.0.0 (the "v" prefix is important as the system will strip it automatically)
   - Upload your compiled executable (can have any filename)
   - Publish the release

### Testing the Auto-Update System

1. Run your base version loader (e.g. `loader_0.9.0.exe`)
2. The loader will:
   - Use the GitHub API to check for updates
   - Extract the download URL for the first asset in the latest release
   - Download the file automatically (regardless of its name)
3. After downloading, it will automatically:
   - Close the current loader
   - Install the new version with the correct version number in the filename
   - Launch the updated version

## Main Files

- `simple_loader.cpp` - Base version of the loader
- `release_loader.cpp` - Release version of the loader
- `loader_version.rc` - Version information for the base version
- `release_version.rc` - Version information for the release version
- `compile_auto.bat` - Script to compile the base version with auto-detection
- `compile_release_auto.bat` - Script to compile the release version with auto-detection
- `compileALL_auto.bat` - Script to compile both versions with auto-detection

## How to Update Your Application

When you want to release a new version:

1. Modify `release_loader.cpp` to update the version number in the `#define VERSION` line
2. Update the version information in `release_version.rc`
3. Run `compile_release_auto.bat` to compile the new version
   - This will automatically create a versioned executable (e.g., `loader_1.1.0.exe`)
4. Create a new release on GitHub with tag version v[VERSION]
5. Upload the compiled executable to the release (can use any filename)

The system will automatically:
- Extract the correct version from the release tag
- Find and download the executable regardless of its filename
- Install it with the proper version in the filename

## Version Information in File Properties

The system uses `.rc` files to add version information that is visible in the .exe file properties under Windows. This information makes it easy to distinguish between different versions of your loader.

## Advanced: Using GitHub API

This system uses the GitHub API to check for updates, which provides several advantages:
- Detects and downloads the latest release executable regardless of filename
- No need to maintain a separate version file
- Updates can be deployed by simply creating a new release
- The system automatically extracts version information from release tags
- Works within GitHub's API rate limits (5000 requests/hour for authenticated requests, 60/hour for anonymous)

## License

This project is under the MIT License. 