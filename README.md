# Auto-Update System for Loaders

This repository contains a C++ auto-update system designed to be easily integrated into existing loaders or applications.

## How the System Works

The auto-update system allows your application to:

1. Automatically check if a new version is available using the GitHub API
2. Download the new version from GitHub releases
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
   - Open `compile.bat`, `compile_release.bat`, and `compileALL.bat`
   - Change the line `cd C:\Users\Yayam\Music\loader-autoupdate` to point to your folder

3. Compilation:
   - For both versions at once: run `compileALL.bat`
   - For base version (0.9.0) only: run `compile.bat`
   - For release version (1.0.0) only: run `compile_release.bat`

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
   - Upload the compiled executable and rename it to `loader.exe` (important: this exact name is required)
   - Publish the release

### Testing the Auto-Update System

1. Run your version 0.9.0 loader (e.g. `loader_0.9.0.exe`)
2. The loader will use the GitHub API to check for updates and detect version 1.0.0
3. After downloading, it will automatically:
   - Close the current loader
   - Install the new version as `loader_1.0.0.exe`
   - Launch the updated version

## Main Files

- `simple_loader.cpp` - Base version of the loader (0.9.0)
- `release_loader.cpp` - Release version of the loader (1.0.0)
- `loader_version.rc` - Version information for the base version
- `release_version.rc` - Version information for the release version
- `compile.bat` - Script to compile the base version
- `compile_release.bat` - Script to compile the release version
- `compileALL.bat` - Script to compile both versions

## How to Update Your Application

When you want to release a new version:

1. Modify `release_loader.cpp` and `release_version.rc` to update the version number
2. Compile the new version with `compile_release.bat`
3. Create a new release on GitHub with tag version v1.1.0 (or update existing release)
4. Upload the new executable renamed to `loader.exe`

## Version Information in File Properties

The system uses `.rc` files to add version information that is visible in the .exe file properties under Windows. This information makes it easy to distinguish between different versions of your loader.

## Advanced: Using GitHub API

This system uses the GitHub API to check for updates, which provides several advantages:
- No need to maintain a separate version.txt file
- Updates can be deployed by simply creating a new release
- The system automatically extracts version information from release tags
- Works within GitHub's API rate limits (5000 requests/hour for authenticated requests, 60/hour for anonymous)

## License

This project is under the MIT License. 