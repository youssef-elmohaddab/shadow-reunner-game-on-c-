# Jeu de Plateforme - Runner

## Description
Un jeu de course infinie développé en C++ avec SFML. Le joueur contrôle un personnage qui doit éviter les obstacles, collecter des récompenses et atteindre le bunker de fin de niveau.

## Fonctionnalités
- **Personnage joueur** : Course, saut, accroupissement
- **Obstacles variés** : Obstacles au sol et aériens avec mouvements dynamiques
- **Système de récompenses** : Collectez des objets pour augmenter votre score
- **Gestion des niveaux** : Sélection de niveaux multiples avec configurations différentes
- **Effets sonores** : Musique de fond et effets sonores pour une immersion complète
- **Interface utilisateur** : Menu principal, HUD avec score et vies, écran de sélection de niveaux

## Prérequis
- Compilateur C++ (g++ ou clang++)
- Bibliothèque SFML 2.5+
- Make ou CMake (optionnel)

## Installation

### 1. Installer SFML
Sur Ubuntu/Debian :
```bash
sudo apt-get install libsfml-dev
```

Sur macOS avec Homebrew :
```bash
brew install sfml
```

Sur Windows :
Téléchargez SFML depuis https://www.sfml-dev.org/download.php et configurez votre IDE.

### 2. Compiler le projet

**Option A : Avec Make**
```bash
make
```

**Option B : Avec CMake**
```bash
mkdir build
cd build
cmake ..
make
```

**Option C : Compilation manuelle**
```bash
g++ -std=c++17 -I./include -c src/*.cpp
g++ -std=c++17 -o JeuPlateforme *.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

## Exécution
```bash
./JeuPlateforme
```

## Structure du Projet
```
├── include/              # Fichiers d'en-tête (.hpp)
│   ├── game.hpp
│   ├── Player.hpp
│   ├── Obstacle.hpp
│   ├── AirObstacle.hpp
│   ├── GroundObstacle.hpp
│   ├── GameWorld.hpp
│   ├── Reward.hpp
│   ├── Bunker.hpp
│   ├── Menu.hpp
│   ├── LevelSelect.hpp
│   └── LevelConfig.hpp
├── src/                  # Fichiers source (.cpp)
│   ├── main.cpp
│   ├── game.cpp
│   ├── Player.cpp
│   ├── Obstacle.cpp
│   ├── AirObstacle.cpp
│   ├── GroundObstacle.cpp
│   ├── GameWorld.cpp
│   ├── Reward.cpp
│   ├── Bunker.cpp
│   ├── Menu.cpp
│   └── LevelSelect.cpp
├── assets/               # Ressources du jeu
│   ├── sprites/
│   ├── fonts/
│   ├── music/
│   └── sounds/
├── CMakeLists.txt        # Configuration CMake
├── Makefile              # Configuration Make
└── README.md             # Ce fichier
```

## Contrôles
- **Flèche droite / D** : Courir vers la droite
- **Flèche gauche / A** : Courir vers la gauche
- **Espace / Flèche haut / W** : Sauter
- **Flèche bas / S** : S'accroupir
- **Échap** : Retour au menu / Quitter

## Ressources
Assurez-vous que les dossiers suivants sont présents avec les ressources nécessaires :
- `assets/sprites/` : Images des personnages, obstacles et décorations
- `assets/fonts/` : Polices de caractères pour l'interface
- `assets/music/` : Musiques de fond
- `assets/sounds/` : Effets sonores

## Développement
Le code est commenté en français pour faciliter la compréhension et la maintenance. Chaque classe a une responsabilité claire selon les principes de la programmation orientée objet.

## Licence
Ce projet est fourni à titre éducatif. Vous êtes libre de le modifier et de le distribuer selon vos besoins.

## Auteur
Développé comme projet d'apprentissage du C++ et de la bibliothèque SFML.