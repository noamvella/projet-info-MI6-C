Jeu tetris en C

-------------------------------------------------------------------------------------------------------------

Presentation :

Le jeu se joue sur une grille de 10x10 cases.

Le joueur choisit une pièce à placer, parmi celles disponibles dans le dossier pièces, puis sélectionne :

1) Une colonne où la placer

2) Une orientation (rotation 0°, 90°, 180°, 270°)

Si il prend trop de temps, celles-ci seront choisit aleatoirement.

L’objectif est de compléter des lignes entières, qui seront alors supprimées et rapporteront des points.

Le jeu se termine lorsque la pièce ne peut plus être placée.


-------------------------------------------------------------------------------------------------------------

Format des pièces :

Les pièces sont représentées dans des fichiers .txt de 5x5 caractères contenant uniquement des 1 et 0, par exemple :

00000

00100

01110

00100

00000

Chaque ligne représente une ligne de la pièce. Les 1 indiquent les blocs occupés.


-------------------------------------------------------------------------------------------------------------

Compilation :

Dans un premier temps, le projet qui est sur notre github doit etre copier dans l'ordinateur.

Ensuite, dans le terminal nous allons appeller notre fichier à l'aide de la commande cd "chemin/a/parcourir".

Ensuite, grace a notre makefile nous allons pouvoir ecrire "make" afin de compiler notre projet.

Enfin, à l'aide de la commande "./tetris" nous allons lancer notre jeu.

-------------------------------------------------------------------------------------------------------------
