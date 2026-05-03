# Test de moyenne glissante (Moving Average)

Ce dossier contient un exemple de sketch Arduino pour tester le calcul d'une moyenne glissante (moving average) sur des valeurs analogiques.

## Fichier principal
- `average.ino` : Sketch Arduino de test.

## Fonctionnalité
- Lit une valeur analogique (ex : capteur sur A0).
- Calcule la moyenne glissante sur N dernières valeurs (ex : avec FIR-Filter).

## Prérequis
- Arduino IDE
- Bibliothèque [FIR-Filter](https://github.com/Moarbue/FIR-Filter) installée via le Library Manager.

## Utilisation
- Ouvrir "Serial Plotter" pour observer l'évolution des valeurs.
