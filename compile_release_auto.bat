@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compiling Release Loader with automatic version detection...

echo Extracting version from source code...
FOR /F "tokens=2 delims=^" %%G IN ('findstr /C:"#define VERSION" release_loader.cpp') DO (
    SET VERSION=%%G
)

SET VERSION=%VERSION:"=%
echo Detected version: %VERSION%

echo Compiling resources...
windres release_version.rc -O coff -o release_version.res

echo Compiling program...
g++ release_loader.cpp release_version.res -o loader_%VERSION%.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation successful!
    echo The executable loader_%VERSION%.exe has been created.
    
    echo Creating a copy named loader.exe for GitHub release...
    copy loader_%VERSION%.exe loader.exe
    echo Remember to upload loader.exe to your GitHub release!
) else (
    echo Compilation error!
    echo Make sure your compiler is properly installed and all paths are configured.
)

pause 