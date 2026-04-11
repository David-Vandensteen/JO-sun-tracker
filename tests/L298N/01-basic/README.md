# Test basique du module L298N avec Arduino UNO

Ce test permet de vérifier le fonctionnement du module L298N en le pilotant avec une carte Arduino UNO.

## Objectif
- Allumer différentes LEDs pour simuler les états de commande du module L298N
- Préparer le code de gestion pour le contrôle de moteurs ou vérins via le L298N.

## Schéma de câblage
- L'image ci-dessous montre le câblage utilisé pour ce test.


<img src="./images/wiring.png" alt="Schéma de câblage" width="350"/>

## Fonctionnement du code de test

Le code de ce dossier permet de tester le pilotage de LEDs via les broches de l'Arduino UNO, en simulant les commandes qui seraient envoyées au module L298N pour contrôler des moteurs ou vérins.

- Chaque LED représente un état ou une commande (activation, déploiement, rétraction, etc.).
- Le code allume ou éteint les LEDs selon une séquence, permettant de valider le câblage et la logique de commande avant de connecter de vrais moteurs.

### Ce qu'il va se passer lors du test

- Lorsque le code est téléversé et exécuté sur l'Arduino UNO, les LEDs vont s'allumer et s'éteindre selon la logique programmée.
- Chaque LED correspond à une commande qui serait envoyée au L298N : activation, direction, etc.
- Le comportement attendu est que les LEDs changent d'état pour simuler l'activation des moteurs ou vérins, permettant de visualiser le fonctionnement.
- Ce test permet de vérifier que chaque sortie de l'Arduino contrôle bien la LED (et donc la commande associée) avant de passer à un montage avec moteurs réels.

> **Remarque :**
> Ce code de test est conçu pour fonctionner aussi bien avec des LEDs de simulation qu'avec un vrai module L298N connecté à l'Arduino UNO. Une fois le câblage validé avec les LEDs, il suffit de remplacer celles-ci par les entrées correspondantes du L298N pour piloter de vrais moteurs ou vérins.

> **Info supplémentaire :**
> Pendant l'exécution, le code envoie également des messages sur la communication série (Serial Monitor) pour indiquer l'état des commandes et faciliter le débogage. Pensez à ouvrir le moniteur série dans l'IDE Arduino pour suivre ces traces en temps réel.

## Vidéo de démonstration

[![Demo vidéo](https://img.youtube.com/vi/gziRdgw5Lqo/0.jpg)](https://youtu.be/gziRdgw5Lqo)



