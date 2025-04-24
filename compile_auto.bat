@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compiling Auto-Update Loader with automatic version detection...

echo Extracting version from source code...
FOR /F "tokens=2 delims=^" %%G IN ('findstr /C:"#define VERSION" simple_loader.cpp') DO (
    SET VERSION=%%G
)

SET VERSION=%VERSION:"=%
echo Detected version: %VERSION%

echo Compiling resources...
windres loader_version.rc -O coff -o loader_version.res

echo Compiling program...
g++ simple_loader.cpp loader_version.res -o loader_%VERSION%.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation successful!
    echo The executable loader_%VERSION%.exe has been created.
    echo To test the program, run loader_%VERSION%.exe
) else (
    echo Compilation error!
    echo Make sure your compiler is properly installed and all paths are configured.
)

pause 