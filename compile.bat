@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compiling Auto-Update Loader (Version 0.9.0)...

echo Compiling resources...
windres loader_version.rc -O coff -o loader_version.res

echo Compiling program...
g++ simple_loader.cpp loader_version.res -o loader_0.9.0.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation successful!
    echo The executable loader_0.9.0.exe has been created.
    echo To test the program, run loader_0.9.0.exe
) else (
    echo Compilation error!
    echo Make sure TDM-GCC is properly installed and all paths are configured.
)

pause 