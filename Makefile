program: astar.o graphic.o main.o
	   gcc -L ./lib -I ./include -o ./src/a_star ./astar.o ./graphic.o ./main.o -w -Wall -ansi -pedantic -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

graphic.o: ./src/graphic.c ./src/graphic.h
	gcc -L ./lib -I ./include -c ./src/graphic.c -w -Wall -ansi -pedantic -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

astar.o: ./src/astar.c ./src/astar.h
		gcc -L ./lib -I ./include -c ./src/astar.c -w -Wall -ansi -pedantic -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

main.o: ./src/main.c
		gcc -L ./lib -I ./include -c ./src/main.c -w -Wall -ansi -pedantic -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

clean:
		rm -rf ./*.o
