#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"
#include "score.h"

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
