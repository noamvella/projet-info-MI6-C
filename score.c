#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"
#include "score.h"

// Fonction pour sauvegarder le meilleur score d'un joueur
void sauvegarde_score(const char *name, int score) {
    FILE *f = fopen("scores.txt", "r+"); // Ouvre le fichier en mode lecture/écriture
    if (!f) {
        // Si le fichier n'existe pas, on le crée
        f = fopen("scores.txt", "a");
        if (!f) return; // Si erreur d'ouverture on retourne
        fprintf(f, "%s %d\n", name, score);
        fclose(f);
        return;
    }

    char existing_name[50];
    int existing_score;
    int player_found = 0;
    long pos = 0; // Position dans le fichier pour pouvoir mettre à jour le score

    // On vérifie si le joueur existe déjà
    while (fscanf(f, "%s %d", existing_name, &existing_score) == 2) {
        if (strcmp(existing_name, name) == 0) {
            // Si le joueur existe, on met à jour son score si nécessaire
            if (score > existing_score) {
                // On revient à la position du score du joueur
                fseek(f, pos, SEEK_SET); //revient a la position dans le fichier ou se trouvait le score du joueur
                fprintf(f, "%s %d\n", name, score); // On écrit le meilleur score
            }
            player_found = 1;
            break;
        }
        pos = ftell(f); // Enregistre la position actuelle pour pouvoir revenir plus tard
    }

    if (!player_found) {
        // Si le joueur n'existe pas, on ajoute son score à la fin du fichier
        fprintf(f, "%s %d\n", name, score);
    }

    fclose(f);
}

// Fonction de comparaison utilisée pour trier les scores
int compare_scores(const void *a, const void *b) {
    return ((Score *)b)->score - ((Score *)a)->score;
}

// Fonction pour afficher les scores enregistrés
void score_max() {
    Score scores[MAX_SCORES];
    int count = 0;
    FILE *f = fopen("scores.txt", "r"); // Ouverture du fichier en mode lecture
    if (!f) {
        printf("Aucun score enregistré.\n");
        return;
    }

    // Enregistre les noms et les scores dans le tableau scores
    while (fscanf(f, "%s %d", scores[count].name, &scores[count].score) == 2 && count < MAX_SCORES) {
        count++;
    }
    fclose(f);

    // Trie les scores par ordre décroissant 
    qsort(scores, count, sizeof(Score), compare_scores);

    // Affiche les scores enregistrés
    printf("\n   Scores enregistrés :\n");
    for (int i = 0; i < count; i++) {
        printf("%s : %d\n", scores[i].name, scores[i].score);
    }
}
