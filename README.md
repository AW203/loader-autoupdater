# Auto-Update System for Loaders

This repository contains a C++ auto-update system designed to be easily integrated into existing loaders or applications.

## How the System Works

The auto-update system allows your application to:

1. Automatically check if a new version is available on GitHub
2. Download the new version from GitHub releases
3. Install the update and restart the application
4. Display version information in the EXE file properties

## Configuration and Usage

### Prerequisites

- TDM-GCC (MinGW) installed on your system
- Git for version management

### Installation and Compilation

1. Clone this repository:
   ```
   git clone https://github.com/AW203/loader-autoupdater.git
   ```

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

1. Make sure the `version.txt` file at the root of your repository contains the current version (e.g., "1.0.0")
2. Create a release on GitHub:
   - Go to your repository → Releases → Create a new release
   - Tag version: v1.0.0 (matching version.txt)
   - Upload the `loader_1.0.0.exe` file compiled with `compile_release.bat`
   - Publish the release

### Testing the Auto-Update System

1. Compile the base version (0.9.0) with `compile.bat`
2. Run the generated loader (`loader_0.9.0.exe`)
3. The loader should detect version 1.0.0 on GitHub and download the update

## Main Files

- `simple_loader.cpp` - Base version of the loader (0.9.0)
- `release_loader.cpp` - Release version of the loader (1.0.0)
- `loader_version.rc` - Version information for the base version
- `release_version.rc` - Version information for the release version
- `compile.bat` - Script to compile the base version
- `compile_release.bat` - Script to compile the release version
- `version.txt` - Version file used by GitHub
- `compileALL.bat` - Script to compile both versions

## How to Update Your Application

When you want to release a new version:

1. Modify `release_loader.cpp` and `release_version.rc` to update the version number
2. Compile the new version with `compile_release.bat`
3. Update `version.txt` on GitHub with the new version number
4. Create a new release on GitHub and upload the new `loader_1.0.0.exe`

## Version Information in File Properties

The system uses `.rc` files to add version information that is visible in the .exe file properties under Windows. This information makes it easy to distinguish between different versions of your loader.

## License

This project is under the MIT License. 