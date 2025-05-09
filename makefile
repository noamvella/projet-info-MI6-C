all: tetris

main.o: main.c
	gcc -c main.c -o main.o

piece.o: piece.c piece.h
	gcc -c piece.c -o piece.o

score.o: score.c score.h
	gcc -c score.c -o score.o

tetris.o: tetris.c tetris.h
	gcc -c tetris.c -o tetris.o

tetris: main.o piece.o score.o tetris.o
	gcc -o tetris main.o piece.o score.o tetris.o


# Supprime tous les fichiers objets
clean:
	rm -f *.o
	rm tetris
