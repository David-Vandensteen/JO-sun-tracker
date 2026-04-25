# ESP32 HTTP Server Demo

Ce dossier contient un exemple de serveur HTTP pour ESP32 qui sert une page HTML depuis le système de fichiers SPIFFS.

## Prérequis
- Carte ESP32
- Python 3
- Script esptool.py du dépôt officiel : https://github.com/espressif/esptool


## Installation de esptool.py

Installez esptool avec pip :

```sh
pip install esptool
```

Pour plus d’informations et d’options, consultez la documentation officielle :
https://docs.espressif.com/projects/esptool/en/latest/esp32/

esptool permet de lire, écrire, vérifier et manipuler la mémoire flash de l’ESP32. Pour l’upload SPIFFS, voir la section ci-dessus.


## Upload SPIFFS avec le script Python

Pour téléverser des fichiers SPIFFS sur l’ESP32, utilisez le script python esptool.py du dépôt officiel Espressif :

1. Ouvrez un terminal dans le dossier de votre projet (là où se trouve le dossier `data`).
2. Exécutez la commande suivante (adaptez le port série, la version et l'adresse SPIFFS si besoin) :


```sh
python3 ./esptool.py \
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
