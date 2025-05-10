#ifndef TETRIS_H
#define TETRIS_H

#define TAILLE 10
#define TAILLE2 5
#define MAX_SCORES 10

// Structure pour stocker les scores et les noms des joueurs
typedef struct {
    char name[50];
    int score;
} Score;


void init_tab(int tab[TAILLE][TAILLE]);
void affichage(int tab[TAILLE][TAILLE], int taille);
int placer(int tab[TAILLE][TAILLE], int piece[TAILLE2][TAILLE2], int col);
int vider_ligne_remplies(int tab[TAILLE][TAILLE]);


#endif
