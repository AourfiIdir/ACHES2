all:
	gcc -o bin/proge src/main.c src/arbre.c src/minimax.c  -I include -I include -I include -L lib -L lib2 -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	
