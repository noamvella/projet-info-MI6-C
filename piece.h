#ifndef PIECE_H
#define PIECE_H

#include "tetris.h"


void affiche_piece(int piece[TAILLE2][TAILLE2]);
void rotation_piece(int piece[TAILLE2][TAILLE2], int degre);
int telecharge_piece(const char *filename, int piece[TAILLE2][TAILLE2]);
int telecharge_piece_joueur(int piece[TAILLE2][TAILLE2], char **piece_files, int nb_pieces);

#endif
