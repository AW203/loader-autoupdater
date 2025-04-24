@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compiling Auto-Update Loader (Release Version 1.0.0)...

echo Compiling resources...
windres release_version.rc -O coff -o release_version.res

echo Compiling program...
g++ release_loader.cpp release_version.res -o loader_1.0.0.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation successful!
    echo The executable loader_1.0.0.exe has been created.
    echo This file should be uploaded to GitHub releases.
) else (
    echo Compilation error!
    echo Make sure TDM-GCC is properly installed and all paths are configured.
)

pause 