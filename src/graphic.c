#include "graphic.h"
/* #################################################################################
	 									UTILS
   ################################################################################# */

/*
* Retourne le chemin absolue du programme (Cette fonction est utilisee pour trouver le chemin absolu pour charger le fichier .ttf)
*/
void recuperer_chemin_programme() {
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	   printf("Dossier de travail : %s\n", cwd);
	} else {
	   perror("getcwd() error");
	}
}

/* #################################################################################
	 						AFFICHAGE GRAPHIQUE
   ################################################################################# */
/*

* Initialisation de la fenetre et du renderer
*/
void initInterfaceGraphique() {
	window = SDL_CreateWindow("A STAR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LARGEUR, HAUTEUR,
        SDL_WINDOW_RESIZABLE);
	if (window == 0)
	{
	    fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
	    /* on peut aussi utiliser SLD_Log() */
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED ); /*  SDL_RENDERER_SOFTWARE */
	if (renderer == 0) {
	     fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
	     /* faire ce qu'il faut pour quitter proprement */
	}
}
/*
 * Initialisation de la font
 */
void initFont() {
	Sans = TTF_OpenFont(strcat(cwd, "\\Roboto-Black.ttf"), 120); /* this opens a font style and sets a size */
	if(!Sans) {
	    printf("TTF_OpenFont: %s\n", TTF_GetError());
	    return EXIT_FAILURE;
	}
}

/*
* Destruction des objects graphiques qui ne servent plus
*/
void desctructionGraphique() {
	TTF_CloseFont(Sans);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
