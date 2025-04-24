@echo off
echo Compilation du Auto-Update Loader...

g++ simple_loader.cpp -o loader.exe -lwininet -lurlmon -lshell32

if %ERRORLEVEL% == 0 (
    echo Compilation réussie!
    echo L'exécutable loader.exe a été créé.
    echo Pour tester le programme, lancez loader.exe
) else (
    echo Erreur de compilation!
    echo Vérifiez que TDM-GCC est correctement installé et que tous les chemins sont configurés.
)

pause 