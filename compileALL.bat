@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compiling both versions of the loader...
echo ========================================
echo.

echo 1. Compiling base version (0.9.0)...
echo ------------------------------------
call compile.bat

echo.
echo 2. Compiling release version (1.0.0)...
echo --------------------------------------
call compile_release.bat

echo.
echo Compilation complete.
echo.
echo To test the auto-update system:
echo 1. Upload the loader_1.0.0.exe file to a GitHub release
echo 2. Run the loader_0.9.0.exe file to see if it detects and downloads the update
echo.
pause 