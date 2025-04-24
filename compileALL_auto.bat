@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compiling both versions of the loader with automatic version detection...
echo =====================================================================
echo.

echo 1. Compiling base version...
echo ---------------------------
call compile_auto.bat

echo.
echo 2. Compiling release version...
echo -----------------------------
call compile_release_auto.bat

echo.
echo Compilation complete.
echo.
echo All executables have been created with their respective version numbers.
echo.
echo To test the auto-update system:
echo 1. Upload the compiled executable to a GitHub release with tag v[VERSION]
echo 2. Run the base version executable to see if it detects and downloads the update
echo.
pause 