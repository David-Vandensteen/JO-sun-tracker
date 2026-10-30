# Schéma de câblage

```
                     ESP32 DevKit (GPIO)
                 ┌───────────────────────────┐
  Bouton DEPLOY  │ GPIO18 ●──────────┐       │
  Bouton RETRACT │ GPIO19 ●──────┐   │       │
  Commutateur    │ GPIO33 ●──┐   │   │       │
  MODE           │           │   │   │       │
  (pull-up int.) │           │   │   │       │
                 │                           │
  LDR UP         │ GPIO34 ● (entrée seule)   │
  LDR DOWN       │ GPIO35 ● (entrée seule)   │
                 │                           │
  Pilote moteur  │ GPIO25 ● ENA              │
  (type L298N)   │ GPIO26 ● IN1              │
                 │ GPIO27 ● IN2              │
                 │ GPIO14 ● ENB              │
                 │ GPIO22 ● IN3              │
                 │ GPIO13 ● IN4              │
                 │                           │
  Écran OLED     │ GPIO21 ● SDA              │
  SSD1306 0x3C   │ GPIO23 ● SCL              │
                 │                           │
  LED tracker    │ GPIO4  ● ──[R]──▶|── GND  │
  LED d'état     │ GPIO2  ● ──[R]──▶|── GND  │  (strapping)
                 │                           │
                 │ GND ───────────────────── GND
                 │ 3V3 ───────────────────── VCC (écran, LDR)
                 └───────────────────────────┘
```

## Boutons et commutateur

- Une borne sur la broche, l'autre sur GND.
- Lecture avec `INPUT_PULLUP` : appuyé = LOW, relâché = HIGH.
- Commutateur MODE fermé vers GND = manuel (LOW), ouvert = auto (HIGH).

## Pilote moteur

Sorties vers les moteurs 1 et 2 :

- `ENA` / `ENB` : PWM (vitesse)
- `IN1` / `IN2` : sens moteur 1 (deploy/retract)
- `IN3` / `IN4` : sens moteur 2 (même commande que moteur 1)
- Alimentation moteur séparée (VM). GND commun avec l'ESP32 obligatoire.
