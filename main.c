#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


#define MAX_SCORES 10

#define TAILLE 10
#define TAILLE2 5



// Structure pour stocker les scores
// et les noms des joueurs
typedef struct {
    char name[50];
    int score;
} Score;


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


//
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






void sauvegarde_score(const char *name, int score) {
    FILE *f = fopen("scores.txt", "w");
    if (!f) return;

    char existing_name[50];
    int existing_score;
    int duplicate_found = 0;

    while (fscanf(f, "%s %d", existing_name, &existing_score) == 2) {
        if (strcmp(existing_name, name) == 0) {
            duplicate_found = 1;
            break;
        }
    }
    fclose(f);

    if (duplicate_found) {
        printf("Le nom \"%s\" existe déjà. Score non sauvegardé.\n", name);
        return;
    }

    f = fopen("scores.txt", "a");
    if (!f) return;
    fprintf(f, "%s %d\n", name, score);
    fclose(f);
}


int compare_scores(const void *a, const void *b) {
    return ((Score *)b)->score - ((Score *)a)->score;
}

void score_max() {
    Score scores[MAX_SCORES];
    int count = 0;
    FILE *f = fopen("scores.txt", "r");
    if (!f) {
        exit(1);
    }

    while (fscanf(f, "%s %d", scores[count].name, &scores[count].score) == 2 && count < MAX_SCORES) {
        count++;
    }
    fclose(f);

    qsort(scores, count, sizeof(Score), compare_scores);

    printf("\n   Scores enregistrés :\n");
    for (int i = 0; i < count; i++) {
        printf("%s : %d\n", scores[i].name, scores[i].score);
    }
}


void Partie() {
    int tab[TAILLE][TAILLE];
    int piece[TAILLE2][TAILLE2];
    int score = 0;
    int game_over = 0;
    char name[50];

    srand(time(NULL));
    init_tab(tab);

    printf("Entrez votre nom : ");
    scanf("%s", name);

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
        printf("Aucune pièce trouvée dans le dossier 'pieces'.\n");
        return;
    }

    printf("%d pièces trouvées.\n", nb_pieces);

    while (!game_over) {
        if (!telecharge_piece_joueur(piece, piece_files, nb_pieces)) {
            printf("Erreur lors du chargement de la pièce.\n");
            break;
        }

        affichage(tab, TAILLE);
        printf("\nVoici la pièce à jouer :\n");
        affiche_piece(piece);
        
        // On demande à l'utilisateur de choisir une colonne
        int col, rotation;
        printf("Colonne (0 à %d) : ", TAILLE - 1);
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

        rotation_piece(piece, rotation);


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
