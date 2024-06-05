#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* Definition de la taille de la fenetre graphique */
#define LARGEUR 		1600
#define HAUTEUR			1000

/*
* Utilise pour l'interface graphique
*/
SDL_Window  * window;
SDL_Renderer *renderer;
TTF_Font* Sans;


/*
* Utilise pour trouver le chemin absolu du projet
*/
char cwd[PATH_MAX];
#ifndef GRAPHIC_H
#define GRAPHIC_H


void initInterfaceGraphique();
void initFont();
void desctructionGraphique();
/*
* Utilise pour trouver le chemin absolu de la font du projet
*/
void recuperer_chemin_programme();

#endif
