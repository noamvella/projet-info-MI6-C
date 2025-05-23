#include "tetris.h"
#include "piece.h"
#include "score.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


void Partie() {
    int tab[TAILLE][TAILLE];
    int piece[TAILLE2][TAILLE2];
    int score = 0;
    int game_over = 0;
    char name[50];

    srand(time(NULL));
    init_tab(tab);

    //Page d'accueil
    printf("===========================================\n");
    printf("=== Bienvenue dans le jeu Tetris en C ! ===\n");
    printf("===========================================\n\n");
    printf("Regles du jeu :\n");
    printf("- Placez les pieces dans une grille de %d x %d.\n", TAILLE, TAILLE);
    printf("- Les pieces font %d x %d cases.\n", TAILLE2, TAILLE2);
    printf("- Remplissez les lignes pour marquer des points.\n");
    printf("- Le jeu se termine quand vous ne pouvez plus placer de piece.\n\n");
    printf("Pour commencer, entrez votre nom : ");
    scanf("%49s", name);
    printf("Bonjour %s ! Bonne chance !\n", name);


    DIR *d; // Pointeur pour le dossier
    struct dirent *dir; // Pointeur pour les fichiers
    char *piece_files[100]; // Tableau pour stocker les noms des fichiers de pièces
    int nb_pieces = 0;

    d = opendir("pieces"); // Ouvre le dossier "pieces"

    // Vérifie si le dossier a été ouvert avec succès
    if (!d) {
        perror("Erreur d'ouverture du dossier 'pieces'");
        return;
    }

    // Parcours les fichiers du dossier
    // et compte le nombre de fichiers .txt
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".txt")) {
            piece_files[nb_pieces] = strdup(dir->d_name);
            nb_pieces++;
        }
    }
    closedir(d);

    if (nb_pieces == 0) {
        printf("Aucune piece trouvee dans le dossier 'pieces'.\n");
        return;
    }

    printf("%d pieces trouvees.\n", nb_pieces);

    while (!game_over) {
        if (!telecharge_piece_joueur(piece, piece_files, nb_pieces)) {
            printf("Erreur lors du chargement de la piece.\n");
            break;
        }

        affichage(tab, TAILLE);
        printf("\nVoici la piece a jouer :\n");
        affiche_piece(piece);

        //On va mettre un temps afin que si le joueur ne joue pas, la colonne et laa rotation soient choisies aléatoirement
        time_t debut, fin;
        int temps_ecoule = 0;

        srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

        printf("Tu as 20 secondes pour choisir une colonne et une rotation.\n");

        time(&debut); // Enregistre le temps de début

        // On demande à l'utilisateur de choisir une colonne
        int col, rotation;

        printf("Colonne (0 a %d) : ", TAILLE - 1); 
        if (scanf("%d", &col) != 1) break;

        int piece_temp[TAILLE2][TAILLE2];
        
        // Affichage des rotations possibles
        for (int angle = 0; angle < 360; angle += 90) {
            memcpy(piece_temp, piece, sizeof(piece_temp)); // Copie la pièce d'origine dans une pièce temporaire
            rotation_piece(piece_temp, angle); // Applique la rotation

            printf("\nRotation %d°:\n", angle); // Affiche l'angle de rotation
            affiche_piece(piece_temp); // Affiche la pièce après rotation
        }

        printf("Choisissez l'orientation (0, 90, 180, 270) : ");
        if (scanf("%d", &rotation) != 1 || (rotation % 90 != 0)) {
            printf("Orientation invalide.\n");
            break;
        }


        time(&fin); // Enregistre le temps de fin

        temps_ecoule = difftime(fin, debut); // Calcule le temps écoulé
        if (temps_ecoule > 20) {
            printf("Temps ecoule !\n");
            col = rand() % TAILLE; // Choix aléatoire de la colonne
            rotation = (rand() % 4) * 90; // Choix aléatoire de la rotation
            printf("Colonne choisie aleatoirement : %d\n", col);
            printf("Rotation choisie aleatoirement : %d°\n", rotation);

            rotation_piece(piece, rotation); // Applique la rotation choisie
        }
        else{
          rotation_piece(piece, rotation); // Applique la rotation choisie
        }


        // Si la pièce ne peut pas être placée, on arrête le jeu
        if (!placer(tab, piece, col)) {
            printf("Game Over !\n");
            game_over = 1;
            break;
        }
        affichage(tab, TAILLE);


        int lignes_effacees = vider_ligne_remplies(tab); // On vide les lignes remplies
        score += lignes_effacees; // On ajoute le score

        printf("Score actuel : %d\n", score);
        sauvegarde_score(name, score);
        score_max();

    }

    for (int i = 0; i < nb_pieces; i++) free(piece_files[i]); // Libère la mémoire allouée pour les noms de fichiers
}



int main(){
  Partie();
  return 0;
}
