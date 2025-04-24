# Système d'Auto-Update pour Loaders

Ce dépôt contient un système d'auto-update en C++ conçu pour être facilement intégré à des loaders existants.

## Fonctionnement du système

Le système d'auto-update permet à votre application de :

1. Vérifier automatiquement si une nouvelle version est disponible sur GitHub
2. Télécharger la nouvelle version depuis les releases GitHub
3. Installer la mise à jour et redémarrer l'application
4. Afficher correctement les informations de version dans les propriétés du fichier EXE

## Configuration et utilisation

### Prérequis

- TDM-GCC (MinGW) installé sur votre système
- Git pour gérer les versions

### Installation et compilation

1. Clonez ce dépôt :
   ```
   git clone https://github.com/AW203/loader-autoupdater.git
   ```

2. **Important** : Modifiez les fichiers batch pour qu'ils contiennent le chemin complet vers votre dossier :
   - Ouvrez `compile.bat` et `compile_release.bat` 
   - Assurez-vous que la ligne `cd C:\Users\Yayam\Music\loader-autoupdate` pointe vers votre dossier

3. Compilation :
   - Pour la version de base (0.9.0) : exécutez `compile.bat`
   - Pour la version release (1.0.0) : exécutez `compile_release.bat`

### Configuration de GitHub

1. Assurez-vous que le fichier `version.txt` à la racine de votre dépôt contient la version actuelle (ex: "1.0.0")
2. Créez une release sur GitHub :
   - Allez sur votre dépôt → Releases → Create a new release
   - Tag version: v1.0.0 (correspondant à version.txt)
   - Téléversez le fichier `loader.exe` compilé avec `compile_release.bat`
   - Publiez la release

### Test du système d'auto-update

1. Compilez la version de base (0.9.0) avec `compile.bat`
2. Exécutez le loader généré
3. Le loader devrait détecter la version 1.0.0 sur GitHub et télécharger la mise à jour

## Fichiers principaux

- `simple_loader.cpp` - Version de base du loader (0.9.0)
- `release_loader.cpp` - Version release du loader (1.0.0)
- `loader_version.rc` - Informations de version pour la version de base
- `release_version.rc` - Informations de version pour la version release
- `compile.bat` - Script pour compiler la version de base
- `compile_release.bat` - Script pour compiler la version release
- `version.txt` - Fichier de version utilisé par GitHub

## Comment mettre à jour votre application

Quand vous voulez publier une nouvelle version :

1. Modifiez `release_loader.cpp` et `release_version.rc` pour mettre à jour le numéro de version
2. Compilez la nouvelle version avec `compile_release.bat`
3. Mettez à jour `version.txt` sur GitHub avec le nouveau numéro de version
4. Créez une nouvelle release sur GitHub et téléversez le nouveau `loader.exe`

## Informations de version dans les propriétés du fichier

Le système utilise les fichiers `.rc` pour ajouter des informations de version qui sont visibles dans les propriétés du fichier .exe sous Windows. Ces informations permettent de distinguer facilement les différentes versions de votre loader.

## Licence

Ce projet est sous licence MIT. 