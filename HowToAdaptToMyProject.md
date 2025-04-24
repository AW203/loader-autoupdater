# How to Adapt the Auto-Update System to Your Project

This guide explains how to implement the auto-update system into your own loader project (such as a Matrix-themed loader). Follow these steps to integrate auto-update capabilities with your existing GitHub repository.

## 1. Set Up Your GitHub Repository

1. **Create or use an existing GitHub repository**
   - Make sure your repository is public (or you have GitHub Enterprise for private repos)
   - This will host your loader files and releases

2. **Add release permissions**
   - Go to repository Settings → Manage Access
   - Ensure you have permissions to create releases

## 2. Copy the Auto-Update Files

1. **Copy these essential files from this project**:
   - `simple_loader.cpp` → Rename to something like `matrix_loader_base.cpp`
   - `release_loader.cpp` → Rename to something like `matrix_loader_release.cpp`
   - `loader_version.rc` → Rename to something like `matrix_base_version.rc`
   - `release_version.rc` → Rename to something like `matrix_release_version.rc`
   - `compile_auto.bat`, `compile_release_auto.bat`, and `compileALL_auto.bat`

2. **Modify the batch files** to reflect your new filenames:
   - Edit paths and filenames in each .bat file
   - Update the path to your project folder

## 3. Integrate Auto-Update Code into Your Loader

1. **Replace your loader code**:
   
   If your loader code is in a file like `matrix-loader.txt`, you need to merge it with the auto-update system.

2. **Modify the `matrix_loader_base.cpp`** (renamed from `simple_loader.cpp`):
   
   a. Change the version definition (around line 13):
   ```cpp
   // Change to your base version
   #define VERSION "0.9.0"  
   ```

   b. Update GitHub repository details (around line 15-18):
   ```cpp
   // Change to YOUR GitHub username and repository
   const std::string GITHUB_USER = "YourGitHubUsername";
   const std::string GITHUB_REPO = "your-matrix-loader";
   // API URL to get latest release info
   const std::string GITHUB_API_URL = "https://api.github.com/repos/" + GITHUB_USER + "/" + GITHUB_REPO + "/releases/latest";
   ```

   c. Update the application directory name (around line 140):
   ```cpp
   // Change "AutoUpdateLoader" to your app name
   std::string appDir = std::string(path) + "\\MatrixLoader";
   ```

   d. **MOST IMPORTANT**: Copy your existing matrix loader functionality into the main function, after the update check:
   ```cpp
   int main() {
       // Update check code (KEEP THIS)
       // ...
       
       // --- INSERT YOUR MATRIX LOADER CODE HERE ---
       // Example:
       // displayMatrixAnimation();
       // promptForPassword();
       // loadMatrixEnvironment();
       // ... etc
       
       return 0;
   }
   ```

3. **Modify the `matrix_loader_release.cpp`** (renamed from `release_loader.cpp`):
   - Follow the same steps as above, but set:
   ```cpp
   #define VERSION "1.0.0"  // Your release version
   ```

4. **Update the RC files**:
   - In both RC files, modify:
   ```
   #define VER_PRODUCTNAME_STR "Matrix Loader\0"
   #define VER_FILEDESCRIPTION_STR "Matrix-styled Loader with Auto-Updates\0"
   // And update version numbers as needed
   ```

## 4. Compile Your New Auto-Update Matrix Loader

1. **Update the batch files**:
   - Edit `compile_auto.bat` to use your new filenames:
   ```batch
   findstr /C:"#define VERSION" matrix_loader_base.cpp
   g++ matrix_loader_base.cpp matrix_base_version.res -o matrix_loader_%VERSION%.exe -lwininet -lurlmon -lshell32
   ```
   - Do the same for `compile_release_auto.bat` and `compileALL_auto.bat`

2. **Run the compilation**:
   - Execute `compileALL_auto.bat` to compile both versions
   - You should get executable files named according to their versions

## 5. Create Your First GitHub Release

1. **Go to your GitHub repository**
2. **Click "Releases" in the right sidebar**
3. **Click "Draft a new release"**
4. **Enter the tag version**: `v1.0.0`
5. **Upload your compiled release file**
   - The file produced by `compile_release_auto.bat`
   - The name doesn't matter - the system detects it automatically
6. **Publish the release**

## 6. Test the Auto-Update System

1. **Run the base version** (`matrix_loader_0.9.0.exe`)
2. **Watch it detect and download the update**
   - It should fetch the release version information
   - Download the new version
   - Install and launch it automatically

## Troubleshooting

- **Connection issues**: Check your firewall settings and internet connection
- **Compilation errors**: Make sure all libraries are properly linked
- **GitHub API issues**: Verify your repository is public or accessible
- **Update not working**: Check that your release tag follows the correct format (v1.0.0)

## Customization Options

- **UI Elements**: Modify the console output messages to match your matrix theme
- **Update Behavior**: Adjust when updates are checked (e.g., at startup, on demand)
- **Installation Location**: Change where updates are installed in the `getAppDataDir()` function
- **Post-Update Actions**: Modify what happens after an update in the updater batch script

## Advanced Integration

For a more seamless experience, consider:

1. **Adding a matrix-style animation during update download**
2. **Customizing the update messages with matrix theme**
3. **Creating a special update visualization with ASCII art**
4. **Adding sound effects during the update process**

This system provides a solid foundation that you can customize to match your matrix-themed loader perfectly. 