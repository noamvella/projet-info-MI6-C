#include <stdio.h>
#include "tetris.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#include "piece.h"
#include "score.h"
#include "tetris.h"
#include "piece.h"


//Initialisation de la matrice
void init_tab(int tab[TAILLE][TAILLE]) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            tab[i][j] = 0;
        }
    }
}

// Affichage de la matrice
void affichage(int tab[TAILLE][TAILLE], int taille) {
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            if (tab[i][j] == 0){
            printf("| ");
            }
            if (tab[i][j] == 1){
                printf("|%d", tab[i][j]);
            }
        }
        printf("|\n");
    }
}

// Placement de la pièce dans le tableau
int placer(int tab[TAILLE][TAILLE], int piece[TAILLE2][TAILLE2], int col) {
    int ligne_possible = -1; // Ligne où la pièce peut être placée

    // Recherche des bords reels de la piece (On va prendre min tres grand et max tres petit pour minimiser et maximiser par la suite)
    int min_col = TAILLE2, max_col = -1; 
    int last_row = -1;
    for (int i = 0; i < TAILLE2; i++) {
        for (int j = 0; j < TAILLE2; j++) {
            if (piece[i][j] == 1) {
                if (j < min_col) min_col = j; 
                if (j > max_col) max_col = j;
                if (i > last_row) last_row = i;  // mémorise la dernière ligne utile
            }
        }
    }
    int hauteur_piece = last_row + 1;
    int largeur_piece = max_col - min_col + 1;

    // Décalage auto dans les bornes
    if (col + largeur_piece > TAILLE) {
        col = TAILLE - largeur_piece;
    }

    // Décalage auto dans les bornes pour empecher les bugs de placement
    if (col < 0) {
        col = 0;
    }

    // Placement vertical on regarde ligne par ligne
    for (int i = 0; i <= TAILLE - hauteur_piece; i++) {
        int peut_placer = 1;
        for (int j = 0; j < hauteur_piece && peut_placer; j++) {
            for (int k = 0; k < TAILLE2 && peut_placer; k++) {
                if (piece[j][k] == 1) {
                    int x = i + j; // Ligne actuelle
                    int y = col + (k - min_col); // Colonne actuelle
                    if (x >= TAILLE || y < 0 || y >= TAILLE || tab[x][y]) {
                        peut_placer = 0;
                    }
                    // Si la pièce dépasse le bas (x >= TAILLE) ou les côtés (y < 0 ou y >= TAILLE) ou rencontre un bloc existant (tab[x][y]), on ne peut pas la placer.
                }
            }
        }
        if (peut_placer) {
            ligne_possible = i; // On mémorise la ligne où la pièce peut être placée
        } else {
            break; // On arrête la recherche si on ne peut pas placer la pièce à cette ligne
        }
    }

    // Si on a trouvé une ligne où la pièce peut être placée, on la place
    if (ligne_possible != -1) {
        for (int j = 0; j < hauteur_piece; j++) {
            for (int k = 0; k < TAILLE2; k++) {
                if (piece[j][k] == 1) {
                    int x = ligne_possible + j; // Ligne actuelle
                    int y = col + (k - min_col); // Colonne actuelle
                    tab[x][y] = 1; // On place la pièce dans le tableau
                }
            }
        }
        return 1;
    }

    return 0;
}

// Fonction pour vider les lignes remplies
int vider_ligne_remplies(int tab[TAILLE][TAILLE]) {
    int efface = 0;
    for (int i = 0; i < TAILLE; i++) {
        int plein = 1;
        for (int j = 0; j < TAILLE; j++) {
            if (tab[i][j] == 0) {
                plein = 0;
                break;
            }
        }
        if (plein) {
            efface++;
            for (int k = i; k > 0; k--) {
                memcpy(tab[k], tab[k - 1], sizeof(int) * TAILLE); // Décale la ligne vers le bas... memcpy permet de copier la ligne d'en haut dans celle d'en bas
            }
            for (int j = 0; j < TAILLE; j++) {
                tab[0][j] = 0;
            }
        }
    }
    return efface; // Retourne le nombre de lignes effacées
}
