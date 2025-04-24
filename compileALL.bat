@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compilation des deux versions du loader...
echo ==========================================
echo.

echo 1. Compilation de la version de base (0.9.0)...
echo ----------------------------------------------
call compile.bat

echo.
echo 2. Compilation de la version release (1.0.0)...
echo ------------------------------------------------
call compile_release.bat

echo.
echo Compilation terminée.
echo.
echo Pour tester le système d'auto-update :
echo 1. Téléversez le fichier loader.exe (version 1.0.0) dans une release GitHub
echo 2. Exécutez le fichier loader_0.9.0.exe pour voir s'il détecte et télécharge la mise à jour
echo.
pause 