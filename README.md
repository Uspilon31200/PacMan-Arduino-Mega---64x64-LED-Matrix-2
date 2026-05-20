# PacMan-Arduino-Mega---64x64-LED-Matrix-2
🎮 Description

Projet complet de réimplémentation du jeu Pac-Man sur Arduino Mega 2560, affiché sur deux matrices LED RGB 64×64 :

    Matrice 1 : affichage du jeu (labyrinthe, Pac-Man, fantômes, gommes…)

    Matrice 2 : affichage du score, des vies, et des informations de jeu

Les deux Arduino communiquent via une liaison série TX/RX, permettant une synchronisation fluide entre l’affichage du jeu et le tableau de score.

Ce projet reproduit fidèlement le comportement du Pac-Man original : déplacements, IA des fantômes, collisions, gestion des gommes, super-gommes, et conditions de victoire/défaite.
⭐ Fonctionnalités principales

    🟡 Jeu Pac-Man complet sur matrice LED 64×64

    👻 IA des fantômes réimplémentée (Blinky, Pinky, Inky, Clyde)

    🧱 Labyrinthe 64×64 optimisé pour l’affichage LED

    🔄 Rafraîchissement fluide malgré les contraintes matérielles

    📡 Communication série TX/RX entre deux Arduino

    📊 Affichage du score sur une seconde matrice LED

    💾 Optimisation mémoire (PROGMEM, tableaux compressés)

    🧩 Architecture modulaire (mouvements, collisions, affichage, IA…)

🔧 Matériel utilisé

    2× Arduino Mega 2560

    2× Matrices LED RGB 64×64 (HUB75)

    2× Alimentations 5V adaptées

    Câbles Dupont / HUB75

    Liaison série TX/RX entre les deux Arduino

🛠️ Stack technique

    C / C++ (Arduino)

    Bibliothèques LED HUB75 (Adafruit / RGBMatrix)

    Communication série (Serial TX/RX)

    PROGMEM pour stocker le labyrinthe et les sprites

    Timers & interruptions pour un rendu fluide

🧩 Architecture du projet

    game_logic/ — gestion des déplacements, collisions, états du jeu

    ghost_ai/ — comportement des fantômes

    display/ — rendu graphique sur la matrice LED

    score_display/ — affichage du score sur la seconde matrice

    serial_comm/ — protocole TX/RX entre les deux Arduino

    sprites/ — Pac-Man, fantômes, gommes, labyrinthe

🚀 Points techniques intéressants

    Optimisation du double-buffering pour éviter le scintillement

    Compression du labyrinthe 64×64 pour tenir dans la RAM limitée

    Synchronisation temps réel entre deux microcontrôleurs

    Gestion des couleurs et animations sur matrice HUB75

    Implémentation d’une IA multi‑comportements pour les fantômes

📸 Captures d’écran / Démo

(À ajouter : photos des matrices, vidéos du gameplay)
📦 Installation

    Installer les bibliothèques HUB75 compatibles

    Flasher le code sur les deux Arduino

    Connecter les matrices LED

    Relier TX → RX entre les deux cartes

    Alimenter les matrices (5V, intensité suffisante)