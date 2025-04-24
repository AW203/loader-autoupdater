@echo off
echo Compilation du Auto-Update Loader (Version Release 1.0.0)...

echo Compilation des ressources...
windres release_version.rc -O coff -o release_version.res

echo Compilation du programme...
g++ release_loader.cpp release_version.res -o loader.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation réussie!
    echo L'exécutable loader.exe (version 1.0.0) a été créé.
    echo Ce fichier doit être téléversé dans la release GitHub.
) else (
    echo Erreur de compilation!
    echo Vérifiez que TDM-GCC est correctement installé et que tous les chemins sont configurés.
)

pause 