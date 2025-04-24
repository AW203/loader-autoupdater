@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compiling Release Loader with automatic version detection...

echo Extracting version from source code...
for /f "tokens=3" %%i in ('findstr /C:"#define VERSION" release_loader.cpp') do set VERSION=%%i
set VERSION=%VERSION:"=%

echo Detected version: %VERSION%

echo Compiling resources...
windres release_version.rc -O coff -o release_version.res

echo Compiling program...
g++ release_loader.cpp release_version.res -o loader_%VERSION%.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation successful!
    echo The executable loader_%VERSION%.exe has been created.
    echo Upload this file to your GitHub release with tag v%VERSION%
) else (
    echo Compilation error!
    echo Make sure your compiler is properly installed and all paths are configured.
)

pause 