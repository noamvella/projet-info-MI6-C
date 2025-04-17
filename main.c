#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


#define MAX_SCORES 10

#define TAILLE 10
#define TAILLE2 5


// src/grid.c


typedef struct {
    char name[50];
    int score;
} Score;

void init_tab(int tab[TAILLE][TAILLE]) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            tab[i][j] = 0;
        }
    }
}

void affichage(int tab[TAILLE][TAILLE], int taille) {
    for (int i = 0; i < taille; i++) {
        for (int j = 0; j < taille; j++) {
            printf("|%d", tab[i][j] ? 1 : 0);
        }
        printf("|\n");
    }
}

int placer(int tab[TAILLE][TAILLE], int piece[TAILLE2][TAILLE2], int col) {
    for (int i = 0; i <= TAILLE - TAILLE2; i++) {
        int can_place = 1;
        for (int j = 0; j < TAILLE2 && can_place; j++) {
            for (int k = 0; k < TAILLE2 && can_place; k++) {
                if (piece[j][k]) {
                    if (col + k >= TAILLE || tab[i + j][col + k]) {
                        can_place = 0;
                    }
                }
            }
        }
        if (can_place) {
            for (int j = 0; j < TAILLE2; j++) {
                for (int k = 0; k < TAILLE2; k++) {
                    if (piece[j][k]) {
                        tab[i + j][col + k] = 1;
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}

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
                memcpy(tab[k], tab[k - 1], sizeof(int) * TAILLE);
            }
            for (int j = 0; j < TAILLE; j++) {
                tab[0][j] = 0;
            }
        }
    }
    return efface;
}

// src/pieces.c


void affiche_piece(int piece[TAILLE2][TAILLE2]) {
    for (int i = 0; i < TAILLE2; i++) {
        printf("|");
        for (int j = 0; j < TAILLE2; j++) {
            printf("%d", piece[i][j] ? 1 : 0);
        }
        printf("|\n");
    }
}

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

int load_piece_from_file(const char *filename, int piece[TAILLE2][TAILLE2]) {
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

int load_user_piece(int piece[TAILLE2][TAILLE2], char **piece_files, int nb_pieces) {
    printf("Choisissez une pièce parmi les suivantes :\n");
    for (int i = 0; i < nb_pieces; i++) {
        printf("%d. %s\n", i + 1, piece_files[i]);
    }
    int choice;
    printf("Votre choix (1-%d) : ", nb_pieces);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > nb_pieces) return 0;

    char path[256];
    snprintf(path, sizeof(path), "pieces/%s", piece_files[choice - 1]);
    return load_piece_from_file(path, piece);
}






void save_score(const char *name, int score) {
    FILE *f = fopen("scores.txt", "a");
    if (!f) return;
    fprintf(f, "%s %d\n", name, score);
    fclose(f);
}

void show_high_scores() {
    Score scores[MAX_SCORES];
    int count = 0;
    FILE *f = fopen("scores.txt", "r");
    if (!f) return;

    while (fscanf(f, "%s %d", scores[count].name, &scores[count].score) == 2 && count < MAX_SCORES) {
        count++;
    }
    fclose(f);

    printf("\n--- High Scores ---\n");
    for (int i = 0; i < count; i++) {
        printf("%s : %d\n", scores[i].name, scores[i].score);
    }
}

void start_game() {
    int tab[TAILLE][TAILLE];
    int piece[TAILLE2][TAILLE2];
    int score = 0;
    int game_over = 0;

    srand(time(NULL));
    init_tab(tab);

    DIR *d;
    struct dirent *dir;
    char *piece_files[100];
    int nb_pieces = 0;

    d = opendir("pieces");
    if (!d) {
        perror("Erreur d'ouverture du dossier 'pieces'");
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".txt")) {
            piece_files[nb_pieces] = strdup(dir->d_name);
            nb_pieces++;
        }
    }
    closedir(d);

    if (nb_pieces == 0) {
        printf("Aucune pièce trouvée dans le dossier 'pieces'.\n");
        return;
    }

    printf("%d pièces trouvées.\n", nb_pieces);

    while (!game_over) {
        if (!load_user_piece(piece, piece_files, nb_pieces)) {
            printf("Erreur lors du chargement de la pièce.\n");
            break;
        }

        affichage(tab, TAILLE);
        printf("\nVoici la pièce à jouer :\n");
        affiche_piece(piece);

        int col, rotation;
        printf("Colonne (0 à %d) : ", TAILLE - 1);
        if (scanf("%d", &col) != 1) break;

        printf("Orientation (0, 90, 180, 270) : ");
        if (scanf("%d", &rotation) != 1) break;

        rotation_piece(piece, rotation);

        if (!placer(tab, piece, col)) {
            printf("Game Over !\n");
            game_over = 1;
            break;
        }

        int lignes_effacees = vider_ligne_remplies(tab);
        score += lignes_effacees;

        printf("Score actuel : %d\n", score);
    }

    char name[50];
    printf("Entrez votre nom : ");
    scanf("%s", name);
    save_score(name, score);
    show_high_scores();

    for (int i = 0; i < nb_pieces; i++) free(piece_files[i]);
}

int main(){
  start_game();
  return 0;
}
