@echo off
echo Installation de TDM-GCC pour compiler le loader
echo ==========================================
echo.

:: Vérifier si TDM-GCC est déjà installé
where g++.exe >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo TDM-GCC est déjà installé sur votre système.
    echo.
    g++ --version
    echo.
    goto :CHECK_WINDRES
)

echo TDM-GCC n'est pas installé ou n'est pas dans le PATH.
echo Le téléchargement va commencer...
echo.

:: Créer un dossier temporaire pour le téléchargement
if not exist "temp" mkdir temp
cd temp

:: Télécharger TDM-GCC
curl -L -o tdm-gcc.exe https://github.com/jmeubank/tdm-gcc/releases/download/v10.3.0-tdm64-2/tdm64-gcc-10.3.0-2.exe

if not exist "tdm-gcc.exe" (
    echo Le téléchargement a échoué. Veuillez vérifier votre connexion internet.
    echo Ou téléchargez manuellement TDM-GCC depuis : https://jmeubank.github.io/tdm-gcc/
    pause
    exit /b 1
)

echo Téléchargement terminé. L'installateur va maintenant s'ouvrir.
echo.
echo Instructions :
echo 1. Suivez les étapes de l'installateur
echo 2. Nous recommandons d'installer dans C:\TDM-GCC-64
echo 3. Sélectionnez au minimum les composants "gcc-core" et "binutils"
echo.
echo IMPORTANT : Après l'installation, vous devrez ajouter le dossier bin au PATH
echo en utilisant cette commande dans une invite de commande administrateur :
echo.
echo     setx PATH "%%PATH%%;C:\TDM-GCC-64\bin" /M
echo.
echo Appuyez sur une touche pour lancer l'installateur...
pause

:: Lancer l'installateur
start "" "tdm-gcc.exe"

cd ..
echo.
echo Une fois l'installation terminée, relancez ce script pour vérifier l'installation.

goto :EOF

:CHECK_WINDRES
:: Vérifier si windres est disponible
where windres.exe >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo windres est correctement installé.
    echo.
    windres --version
    echo.
    echo Votre système est correctement configuré pour compiler le loader.
) else (
    echo AVERTISSEMENT : windres.exe n'a pas été trouvé dans le PATH.
    echo Il est possible que certaines fonctionnalités du compilateur ne soient pas disponibles.
    echo Assurez-vous que le dossier bin de TDM-GCC est bien dans votre PATH.
)

echo.
echo Installation terminée. Vous pouvez maintenant compiler le loader avec compile.bat
echo.
pause 