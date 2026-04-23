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
```bash
g++ -std=c++17 -c *.cpp
g++ -std=c++17 -o jeu *.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

Ou utilisez le Makefile s'il est présent :
```bash
make
```

## Exécution
```bash
./jeu
```

## Contrôles
- **Flèche droite / D** : Courir vers la droite
- **Flèche gauche / A** : Courir vers la gauche
- **Espace / Flèche haut / W** : Sauter
- **Flèche bas / S** : S'accroupir
- **Échap** : Retour au menu / Quitter

## Structure du Projet
- `main.cpp` : Point d'entrée du programme
- `Game.hpp/cpp` : Gestion principale du jeu et des états
- `Player.hpp/cpp` : Logique du joueur et animations
- `Obstacle.hpp/cpp` : Classe de base pour les obstacles
- `AirObstacle.hpp/cpp` : Obstacles aériens
- `GroundObstacle.hpp/cpp` : Obstacles au sol
- `GameWorld.hpp/cpp` : Monde de jeu, collisions et rendu
- `Reward.hpp/cpp` : Système de récompenses
- `Bunker.hpp/cpp` : Objectif de fin de niveau
- `Menu.hpp/cpp` : Menu principal
- `LevelSelect.hpp/cpp` : Écran de sélection des niveaux
- `LevelConfig.hpp` : Configuration des paramètres de niveaux

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