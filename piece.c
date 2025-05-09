#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "piece.h"
#include "tetris.h"


void affiche_piece(int piece[TAILLE2][TAILLE2]) {
    for (int i = 0; i < TAILLE2; i++) {
        printf("|");
        for (int j = 0; j < TAILLE2; j++) {
            printf("%d", piece[i][j] ? 1 : 0); // Affiche 1 si la case est occupée, sinon 0
        }
        printf("|\n");
    }
}

// Fonction pour faire tourner la pièce de 90 degrés dans le sens des aiguilles d'une montre
void rotation_piece(int piece[TAILLE2][TAILLE2], int degre) {
    int fois = (degre / 90) % 4;
    int temp[TAILLE2][TAILLE2];
    for (int t = 0; t < fois; t++) {
        for (int i = 0; i < TAILLE2; i++) {
            for (int j = 0; j < TAILLE2; j++) {
                temp[j][TAILLE2 - 1 - i] = piece[i][j];
            }
        }
        memcpy(piece, temp, sizeof(temp));
    }
}

// Fonction pour charger une pièce à partir d'un fichier
int telecharge_piece(const char *filename, int piece[TAILLE2][TAILLE2]) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char line[TAILLE2+3];
    for (int i = 0; i < TAILLE2; i++) {
        if (fgets(line, sizeof(line), f)) {
            for (int j = 0; j < TAILLE2; j++) {
                piece[i][j] = (line[j] == '1') ? 1 : 0;
            }
        } 
        else {
            for (int j = 0; j < TAILLE2; j++){
              piece[i][j] = 0;
            }
        }
    }
    fclose(f);
    return 1;
}

int telecharge_piece_joueur(int piece[TAILLE2][TAILLE2], char **piece_files, int nb_pieces) {
    int piece1[TAILLE2][TAILLE2];
    printf("Choisissez une pièce parmi les suivantes :\n");
    for (int i = 0; i < nb_pieces; i++) {
        char path[256];
        snprintf(path, sizeof(path), "pieces/%s", piece_files[i]);
        printf("%d.\n", i + 1);
        telecharge_piece(path,piece1);
        affiche_piece(piece1);
        printf("\n");
    }
    int choice;
    printf("Votre choix (1-%d) : ", nb_pieces);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > nb_pieces) return 0;

    char path[256];
    snprintf(path, sizeof(path), "pieces/%s", piece_files[choice - 1]);
    return telecharge_piece(path, piece);
}
