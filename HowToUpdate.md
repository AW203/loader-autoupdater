# How to Update the Auto-Update Loader

This guide explains the step-by-step process for updating your loader to a new version.

## 1. Modify Version Numbers

### For a Minor Update (Release Version)

When releasing a new version, you need to update:

1. **In `release_loader.cpp`**:
   - Find the line: `#define VERSION "1.0.0"`
   - Change it to your new version number: `#define VERSION "1.1.0"`

2. **In `release_version.rc`**:
   - Update all version references:
   ```
   #define VER_FILEVERSION             1,1,0,0
   #define VER_FILEVERSION_STR         "1.1.0.0\0"
   #define VER_PRODUCTVERSION          1,1,0,0
   #define VER_PRODUCTVERSION_STR      "1.1.0\0"
   ```

### For a Base Version Update (Optional)

If you also need to update the base version:

1. **In `simple_loader.cpp`**:
   - Find the line: `#define VERSION "0.9.0"`
   - Change it to your new base version: `#define VERSION "0.9.1"`

2. **In `loader_version.rc`**:
   - Update all version references:
   ```
   #define VER_FILEVERSION             0,9,1,0
   #define VER_FILEVERSION_STR         "0.9.1.0\0"
   #define VER_PRODUCTVERSION          0,9,1,0
   #define VER_PRODUCTVERSION_STR      "0.9.1\0"
   ```

## 2. Compile the New Version

1. **Compile the release version**:
   - Run `compile_release_auto.bat`
   - This script will automatically:
     - Extract the version from your code
     - Compile the loader with the correct filename (e.g., `loader_1.1.0.exe`)

2. **Compile both versions (optional)**:
   - If you updated both versions, run `compileALL_auto.bat`

## 3. Create a GitHub Release

1. **Go to your GitHub repository**
2. **Click on "Releases" in the right sidebar**
3. **Click "Create a new release" or "Draft a new release"**
4. **Enter the tag version**:
   - Follow the format: `v1.1.0` (the 'v' prefix is important)
   - This MUST match the version in your code (except for the 'v' prefix)
5. **Enter a release title** (e.g., "Version 1.1.0")
6. **Add release notes** describing the changes in this version
7. **Upload the compiled executable**:
   - Drag and drop the `loader_1.1.0.exe` file
   - The filename doesn't matter - the system will detect it automatically
8. **Click "Publish release"**

## 4. Testing the Update

1. **Run the base version of your loader** (e.g., `loader_0.9.0.exe`)
2. **The loader will automatically**:
   - Check GitHub for updates using the API
   - Detect the new version 1.1.0
   - Download the updated executable
   - Install it and restart

## Troubleshooting

- **Version not detected**: Make sure the GitHub release tag matches your code version (with 'v' prefix)
- **Compilation errors**: Check if all required libraries are properly linked
- **Update not working**: Verify the GitHub release is published and contains a valid executable

## Advanced: Versioning System

The automatic versioning system uses:
- The `#define VERSION` in your source files to name compiled executables
- The GitHub release tag (e.g., "v1.1.0") to identify the latest version
- The GitHub API to extract download URLs automatically

This eliminates the need for manual version tracking files and simplifies the update process. 