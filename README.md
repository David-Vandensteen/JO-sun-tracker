# JO-sun-tracker

## Matériel pour un groupe de panneaux solaire
- 1 carte Arduino UNO (5v) ou 1 carte Arduino NANO (3,3v) ?
- 1 module L298N (peut piloter 2 moteurs 12v)
- 2 modules LDR LM393 (photocellule)
- 1 anémomètre (ref ?)
- 2 boutons poussoir
- 1 bouton switch
- 2 vérins 650mm 12v
- Connecteurs JST

## Asservissement, monitoring et pilotage à distance
Dans un second temps, il est prévu d'ajouter une carte de type Raspberry Pi ou ESP32, équipée d'un écran tactile, afin de superviser et piloter le groupe de cartes Arduino via un bus série et un protocole de communication adapté.
Cette évolution permettra également le pilotage et la supervision à distance, nécessitant donc une capacité Wi-Fi sur la carte ajoutée.
Les cartes Arduino devront conserver une logique de repli autonome en cas de dysfonctionnement de cette couche supérieure.

```mermaid
graph TD
  A[Utilisateurs à distance Wi-Fi] -->|Supervision/Pilotage| B(Raspberry Pi / ESP32)
  B -->|Bus série & protocole| C1[Carte Arduino 1]
  B -->|Bus série & protocole| C2[Carte Arduino 2]
  C1 -->|Contrôle| D1[Panneaux solaires 1]
  C2 -->|Contrôle| D2[Panneaux solaires 2]
  B -->|Écran tactile| E[Interface locale]
  C1 -->|Fallback autonome| F1[Logique de repli 1]
  C2 -->|Fallback autonome| F2[Logique de repli 2]
```


## Programme
Le code source pour la carte Arduino est en cours de développement ici :

[Voir le code source principal](src/driver/main.cpp)

