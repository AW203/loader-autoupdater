# C++ Auto-Update System for Loaders

This repository contains a C++ auto-update system designed to be easily integrated into existing loaders or applications.

## Configuration Instructions for GitHub Auto-Update

### 1. Setup your GitHub Repository

1. Make sure your repository is public (or your credentials are properly configured for private access)
2. Place the `version.txt` file in the root of your repository with the current version number (e.g., "1.0.0")
3. Create a new release on GitHub:
   - Go to your repository → Releases → Create a new release
   - Tag version: v1.0.0 (match with version.txt)
   - Upload the compiled loader.exe as a binary file to the release
   - Publish the release

### 2. Compile the Loader

1. Install TDM-GCC if not already installed
2. Run the `compile.bat` script to compile the simple_loader.cpp
3. This will create `loader.exe` which automatically checks for updates from your GitHub repository

### 3. Testing the Auto-Update

1. Run `loader.exe` - it will check the version.txt on GitHub against its own version
2. If versions match, it will proceed to the loader functionality
3. If GitHub has a newer version, it will download and install the update

### 4. How to Update Your Application

When you want to release a new version:

1. Update the code as needed and recompile to create a new `loader.exe`
2. Update `version.txt` in your repository with the new version number (e.g., change from "1.0.0" to "1.0.1")
3. Create a new release on GitHub and upload the new `loader.exe`
4. When users run their old version, it will detect the update and automatically download the new version

## Features

- Automatic update checks from a GitHub repository
- Download and installation of updates
- Simple versioning system
- Self-updating capability with automatic restart
- Windows compatible

## Project Structure

- `simple_loader.cpp` - A simplified loader with built-in auto-update functionality
- `main.cpp` - The main auto-update system as a reusable class 
- `version.txt` - Version file used to determine if an update is available
- `compile.bat` - Simple script to compile the loader with TDM-GCC

## License

This project is under the MIT License. 