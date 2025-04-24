@echo off
cd C:\Users\Yayam\Music\loader-autoupdate
echo Compilation du Auto-Update Loader (Version 0.9.0)...

echo Compilation des ressources...
windres loader_version.rc -O coff -o loader_version.res

echo Compilation du programme...
g++ simple_loader.cpp loader_version.res -o loader.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation réussie!
    echo Création d'une copie nommée loader_0.9.0.exe...
    copy loader.exe loader_0.9.0.exe
    echo L'exécutable loader.exe (et loader_0.9.0.exe) a été créé.
    echo Pour tester le programme, lancez loader_0.9.0.exe
) else (
    echo Erreur de compilation!
    echo Vérifiez que TDM-GCC est correctement installé et que tous les chemins sont configurés.
)

pause 