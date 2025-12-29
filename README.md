# 🗺️ C++ A* Pathfinder & Visualizer

![C++](https://img.shields.io/badge/Language-C++17%2F20-blue)
![Qt](https://img.shields.io/badge/Framework-Qt6-green)
![Status](https://img.shields.io/badge/Status-Functional-brightgreen)

Une application haute performance de calcul d'itinéraire sur des données cartographiques réelles (OpenStreetMap / Graphes), développée en C++ avec l'interface graphique Qt.

Le projet implémente l'algorithme **A* (A-Star)** dans sa version standard ainsi qu'une version **Bidirectionnelle** optimisée. Il intègre un moteur de rendu interactif capable d'afficher des milliers de nœuds et d'exporter les résultats au format GeoJSON.

---
## 📸 Aperçu

![Interface Principale](https://i.ibb.co/RkGtM86b/image.png)

##  Fonctionnalités Clés

###  Algorithmes
* **A* Standard :** Recherche de chemin classique avec heuristique de distance.
* **A* Bidirectionnel :** Version optimisée lançant deux recherches simultanées (depuis le départ et l'arrivée) qui se rencontrent au milieu. Réduit drastiquement l'espace de recherche.
* **Affichage des voisins :** Visualisation des connexions entre les nœuds pour le débogage.

###  Interface & Visualisation
* **Carte Interactive :** Vue 2D basée sur `QGraphicsView`.
* **Contrôles Intuitifs :** * 🖱️ **Zoom :** Molette de la souris (centré sur le curseur).
    * ✋ **Pan :** Déplacement de la carte par "Drag & Drop" (Clic gauche maintenu).
* **Rendu Optimisé :** Utilisation de `QPainterPath` et de l'accélération matérielle (OpenGL) pour une fluidité maximale même avec des milliers de points.
* **Code Couleur :** Distinction visuelle claire (Noeuds en Jaune, Routes en Vertes, Chemin choisi en Rouge).

###  Export & Données
* **Export GeoJSON :** Sauvegarde du chemin calculé au format `.geojson` pour une visualisation externe (sur [geojson.io](https://geojson.io) ou Google Earth).
* **Données Réelles :** Conçu pour parser des fichiers de graphes géographiques (Latitude/Longitude).

---

##  Architecture Technique

Ce projet met en œuvre des pratiques de **C++ Moderne** :

* **Pimpl Idiom (Pointer to Implementation) :** Séparation stricte de l'interface (`.h`) et de l'implémentation (`.cpp`) pour réduire les temps de compilation et masquer la complexité (`std::unique_ptr`).
* **Gestion Mémoire :** Utilisation exclusive de **Smart Pointers** (`std::unique_ptr`) pour une gestion automatique et sans fuite de mémoire (RAII).
* **Qt Graphics View Framework :** Architecture Scène/Vue découplée pour la gestion graphique.
