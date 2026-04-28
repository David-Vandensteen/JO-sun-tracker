# ESP32 HTTP Server Demo

Ce dossier contient un exemple de serveur HTTP pour ESP32 qui sert une page HTML depuis le système de fichiers SPIFFS.

## Prérequis
- Carte ESP32
- Python 3
- Script esptool.py du dépôt officiel : https://github.com/espressif/esptool
- Bibliothèque ArduinoLog (à installer via le gestionnaire de bibliothèques de l’IDE Arduino)

### Installer la bibliothèque ArduinoLog

Dans l’IDE Arduino, va dans « Outils » > « Gérer les bibliothèques… », recherche « ArduinoLog » et clique sur « Installer ».

## Installation de esptool.py

Installez esptool avec pip :

```sh
pip3 install esptool
```

Pour plus d’informations et d’options, consultez la documentation officielle :
https://docs.espressif.com/projects/esptool/en/latest/esp32/

esptool permet de lire, écrire, vérifier et manipuler la mémoire flash de l’ESP32. Pour l’upload SPIFFS, voir la section ci-dessus.

## Lister les ports série disponibles

Avant d’uploader, il peut être utile de connaître le port série de votre ESP32. Voici comment lister les ports disponibles :

- **macOS / Linux** :

```sh
ls /dev/tty.*
ls /dev/cu.*
```

- **Windows** :

Ouvrez le « Gestionnaire de périphériques » et regardez sous « Ports (COM & LPT) ».

Ou utilisez la commande PowerShell :

```powershell
[System.IO.Ports.SerialPort]::getportnames()
```

Note : Le port change selon la machine et l’USB utilisé.

## Trouver l’adresse de départ SPIFFS (offset) dans l’IDE Arduino

1. Ouvre l’IDE Arduino.
2. Va dans le menu **Outils > Partition Scheme** (ou « Schéma de partition »).
3. Note le nom du schéma sélectionné (ex : « Default 4MB with spiffs »).
4. Pour connaître l’offset exact :
   - Va dans le dossier d’installation de l’ESP32, sous :  
     `hardware/espressif/esp32/tools/partitions/`
   - Ouvre le fichier `.csv` correspondant au schéma choisi.
   - Cherche la ligne commençant par `spiffs` : la colonne « Offset » donne l’adresse à utiliser (ex : `0x290000`).

**Remarque** : L’offset par défaut est souvent `0x290000`, mais il peut varier selon le schéma choisi.

## Upload SPIFFS avec le script Python

Pour téléverser des fichiers SPIFFS sur l’ESP32, utilisez le script python esptool.py du dépôt officiel Espressif :

1. Ouvrez un terminal dans le dossier de votre projet (là où se trouve le dossier `data`).
2. Exécutez la commande suivante (adaptez le port série, la version et l'adresse SPIFFS si besoin) :

```sh
python3 /Library/Frameworks/Python.framework/Versions/3.11/bin/esptool.py \
   --chip esp32 --port /dev/ttyUSB0 --baud 115200 write_flash 0x290000 data/
```

- Remplacez `/dev/ttyUSB0` par le port série de votre ESP32.
- Remplacez `<version>` par la version installée (ex : 3.1.0).
- Vérifiez l’adresse de la partition SPIFFS (0x290000 est un exemple, voir votre partition.csv).

Pour plus d’informations sur esptool.py, consultez le dépôt officiel :
https://github.com/espressif/esptool


## Formater/effacer complètement SPIFFS depuis le code ESP32

Pour effacer tout le contenu SPIFFS, ajoute temporairement ce code dans ton setup() :

```cpp
#include <SPIFFS.h>

void setup() {
  Serial.begin(115200);
  if (SPIFFS.begin(true)) {
    SPIFFS.format(); // Formate et efface tout SPIFFS
    Serial.println("SPIFFS formaté !");
  } else {
    Serial.println("Erreur SPIFFS");
  }
  // ... le reste de ton setup
}
```

Après le formatage, retire la ligne `SPIFFS.format();` pour éviter d’effacer à chaque démarrage.

## Étapes pour servir index.html depuis SPIFFS

1. **Téléverser le fichier sur l’ESP32**
   - Ouvrez un terminal dans le dossier du projet.
   - Exécutez la commande d’upload SPIFFS décrite plus haut.
   - Attendez la fin du téléversement.

2. **Téléverser le sketch**
   - Téléversez ensuite le sketch `http-server.ino` normalement sur l’ESP32.

3. **Accéder à la page web**
   - Ouvrez le moniteur série pour récupérer l’adresse IP affichée.
   - Dans un navigateur, allez à l’adresse IP indiquée (ex : http://192.168.0.96).
   - La page `index.html` doit s’afficher.

## Remarques
- Le fichier HTML doit être dans le dossier `data` au moment de l’upload SPIFFS.
