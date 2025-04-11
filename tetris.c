#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE 10
#define TAILLE2 5

void pieces(FILE *f){
    int choix=(rand()%7)+1;
    
    
}

void affichage(int tab[TAILLE][TAILLE], int taille){
    for(int i=0;i<taille;i++){
       for(int j=0;j<taille;j++){
         if(tab[i][j]==0){
           printf("| ");
         }
         if(tab[i][j]==1){
            printf("|@");
        }
        
    }
      printf("|\n");
}
}

int main(){
  srand(time(NULL));
  FILE *pieces = fopen("pieces.xlsx", "r");
  if (pieces == NULL) {
      printf("Le fichier n'a pas pu être ouvert\n");
  }

  int tab[TAILLE][TAILLE]={0};
  tab[5][2]=1;
  affichage(tab, TAILLE);
  
  return 0;
}
