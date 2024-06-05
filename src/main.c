#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "astar.h"
#include "graphic.h"

int carte_donnee[X_CARTE][Y_CARTE] = {
		{ 0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,1,1,1,1,1 },
		{ 0,0,0,4,4,4,0,0,0,0,0,0,4,4,4,1,0,0,0,2,1 },
		{ 0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,2,2 },
		{ 0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,1,0,2,1 },
		{ 0,0,0,0,0,1,1,0,2,2,0,0,0,1,1,1,4,0,1,1,1 },
		{ 0,0,3,3,0,1,1,0,2,2,0,0,0,0,4,4,4,0,2,2,2 },
		{ 0,0,0,1,3,3,0,0,2,2,0,0,0,0,4,4,4,4,4,4,4 },
		{ 0,1,1,3,1,0,0,0,2,2,0,0,0,0,4,4,4,4,4,4,4 },
		{ 0,0,0,0,0,1,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4 },
		{ 0,0,1,3,3,3,1,1,1,1,1,0,2,2,4,4,1,1,1,1,1 },
		{ 1,0,1,0,0,1,0,0,1,1,1,1,2,2,4,4,1,1,1,1,1 },
		{ 0,0,0,0,4,4,0,0,0,0,0,1,1,0,2,2,4,4,1,1,0 },
		{ 1,0,1,0,4,4,0,1,1,0,0,0,4,4,4,4,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,1,1,0,0,0,4,4,4,4,1,1,1,1,1 },
		{ 1,0,4,4,1,1,0,0,0,0,0,0,4,4,4,4,1,1,1,1,1 },
		{ 1,0,4,4,4,4,0,1,1,1,0,0,4,4,4,4,1,1,1,1,1 },
		{ 1,0,0,0,0,0,0,0,1,1,0,0,4,4,4,4,1,1,1,1,1 },
		{ 0,0,2,2,2,2,0,0,1,1,0,0,4,4,4,4,1,1,1,1,1 },
		{ 2,2,2,2,2,2,0,0,0,1,0,0,4,4,4,4,1,1,1,1,1 },
		{ 2,2,2,2,0,1,1,0,0,0,0,0,4,4,4,4,1,1,1,1,1 },
		{ 2,2,0,0,0,0,0,0,0,0,0,0,4,4,4,4,1,1,1,1,1 },
		{ 2,2,0,0,1,1,1,1,1,0,0,0,4,4,4,4,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,0,0,0,0,4,4,4,4,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,0,0,0,0,4,4,4,4,1,1,1,1,2 },
		{ 0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,1,1,1,1,1 }
	 };



/* #################################################################################
	 						AFFICHAGE CONSOLE / DEBUG
   ################################################################################# */
/*
* Fonction qui affiche table_ouverte
*/
void print_table_ouverte() {
	int i;
	for (i = 0; i < nb_table_ouverte; ++i) {
		printf(" %d %d %d |", table_ouverte[i]->x, table_ouverte[i]->y, table_ouverte[i]->poids + table_ouverte[i]->heuristique);
	}
	printf("\n");
}

/*
* Fonction qui affiche table_fermee
*/
void print_table_fermee() {
	int i;
	for (i = 0; i < nb_table_fermee; ++i) {
		printf(" %d %d %d |", table_fermee[i]->x, table_fermee[i]->y, table_fermee[i]->poids + table_fermee[i]->heuristique);
	}
	printf("\n");
}

/*
 * Affichage le meilleur chemin trouve par l'algo A star
 */
void print_meilleur_chemin(pStarNoeud noeud_tresor) {
	int top = -1;
	pStarNoeud noeud_courant = noeud_tresor;

	while( noeud_courant )
	{
		chemin_final[++top] = noeud_courant;
		noeud_courant = noeud_courant->parent;
	}

	printf("Le chemin trouve est :\n");
	while (top >= 0)
	{
		if (top > 0)
		{
			printf("(%d,%d)-->", chemin_final[top]->x, chemin_final[top]->y);
		}
		else
		{
			printf("(%d,%d)", chemin_final[top]->x, chemin_final[top]->y);
		}
		/* On met a jour la carte pour l'affichage de la solution */
		carte[chemin_final[top]->x][chemin_final[top]->y].est_solution = 1;
		top--;
	}
}

/*
 * Print tous les coordonnees de la carte
 */
void print_coordonnees_carte() {
	int x, y;
 	for( x = 0; x < X_CARTE; ++x )
	{
		for ( y = 0; y < Y_CARTE; ++y )
		{
			printf("%d:%d\t", carte[x][y].x, carte[x][y].y);
		}
		printf("\n");
	}
}



/* #################################################################################
	 						INTERACTION UTILISATEUR
   ################################################################################# */
/*
* Demander a l'utilisateur les coordonnees de depart, des coffres (Les coffres ne sont pas le vrai tresor se sont des fausses pistes!), et du vrai tresor.
*/
void demander_les_coordonnees() {
	 /*
	 * Demandons a l'utilisateur les coordonness de 3 endroits qui ne correspondent pas au tresor!
	 */
	 int i,x,y;
	for(i = 0; i < NB_COFFRE; ++i){
		do {
			x = -1;
			y = -1;
			printf( "Veuillez saisir une fausse coordonnee pour le coffre (numero %d)! (Choisir les coordonness d'un terrain plat: )",i+1);
			fflush( stdout );
			scanf( "%d %d", &x, &y);
			if(check_coordonnees(x,y) && carte_donnee[x][y] == TERRAIN_PLAT) {
				carte_donnee[x][y] = COFFRE;
			}
		}while (check_coordonnees(x,y) && carte_donnee[x][y] != COFFRE);

	}
	 /*
	 * Demandons a l'utilisateur les coordonness du tresor!
	 */

	do {
		x = -1;
		y = -1;
		printf( "Veuillez saisir les coordonnees du tresor ! (Choisir les coordonness d'un terrain plat: )");
		fflush( stdout );
		scanf( "%d %d", &x, &y);
		if(check_coordonnees(x,y) && carte_donnee[x][y] == TERRAIN_PLAT) {
			carte_donnee[x][y] = TRESOR;
		}
	}while (check_coordonnees(x,y) && carte_donnee[x][y] != TRESOR);

	 /*
	 * Demandons a l'utilisateur les coordonness de depart!
	 */
	do {
		x = -1;
		y = -1;
		printf( "Veuillez saisir les coordonnees de depart! (Choisir les coordonness d'un terrain plat: ",i);
		fflush( stdout );
		scanf( "%d %d", &x, &y);
		if(check_coordonnees(x,y) && carte_donnee[x][y] == TERRAIN_PLAT) {
			carte_donnee[x][y] = DEPART;
		}
	}while (check_coordonnees(x,y) && carte_donnee[x][y] != DEPART);

}

/* #################################################################################
	 						INITIALISATION DES AUTRES DONNEES
   ################################################################################# */
/*
 * Initialisation de la carte
 * Cette fonction affiche aussi la carte avec le type de chaque cellule
 */
void init_carte() {
	int x,y;
	int numero_coffre = 0;
	 for( x = 0; x < X_CARTE; ++x )
	 {
		for ( y = 0; y < Y_CARTE; ++y )
		{
			/*  Initialisation de tous les noeuds */
			carte[x][y].x = x;
			carte[x][y].y = y;
			carte[x][y].poids = 0;
			carte[x][y].heuristique = 0;
			carte[x][y].type = carte_donnee[x][y];
			carte[x][y].parent = NULL;
			carte[x][y].est_dans_la_table_fermee = 0;
			carte[x][y].est_dans_la_table_ouverte = 0;
			carte[x][y].est_solution = 0;

			/* On definie le noeud de depart */
			if (carte_donnee[x][y] == DEPART) {
				noeud_depart = &(carte[x][y]);
			}

			/* On definie le noeud ou le tresor est present! */
			if (carte_donnee[x][y] == TRESOR) {
				noeud_tresor = &(carte[x][y]);
			}

			/* On definie le noeud ou le tresor est present! */
			if (carte_donnee[x][y] == COFFRE) {
				if(numero_coffre == 0)
					noeud_coffre1 = &(carte[x][y]);
				if(numero_coffre == 1)
					noeud_coffre2 = &(carte[x][y]);
				if(numero_coffre == 2)
					noeud_coffre3 = &(carte[x][y]);
				numero_coffre++;
			}
			printf("%d ", carte[x][y].type);
		}
		printf("\n");
	 }
}



/*
 * AFFICHAGE A LECRAN
 */
void afficherEcran(pStarNoeud noeud_courant){
  SDL_Color Green = {24, 106, 59};
  SDL_Color Red = {120, 40, 31 };
	SDL_Rect rect;
	/* couleur de fond */
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	int x,y;
	/* Affichage de la grille */
 	for( x = 0; x < X_CARTE; ++x )
	{
		for ( y = 0; y < Y_CARTE; ++y )
		{
			SDL_Surface* surfaceMessage;
			SDL_Texture * texture;
			if(carte[x][y].est_dans_la_table_ouverte || carte[x][y].est_dans_la_table_fermee) {
				SDL_Color c;
				if(carte[x][y].est_dans_la_table_ouverte)
					c = Green;
				if(carte[x][y].est_dans_la_table_fermee)
					c = Red;
				char text_cell[MAX_SIZE_CELL];
				sprintf(text_cell,"%d:%d:%d", carte[x][y].poids, carte[x][y].heuristique, carte[x][y].poids + carte[x][y].heuristique);
				surfaceMessage = TTF_RenderText_Solid(Sans, text_cell, c); /* as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first */
				texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
			}
			/* dessiner en blanc */
			if(carte[x][y].type == TERRAIN_PLAT) {
				SDL_SetRenderDrawColor(renderer, 255,255,255,0);
			}
			if(carte[x][y].type == TRESOR) {
				SDL_SetRenderDrawColor(renderer, 40, 180, 99 ,0);
			}
			else if(carte[x][y].type == FORET) {
				SDL_SetRenderDrawColor(renderer, 19, 141, 117 ,0);
			}
			else if(carte[x][y].type == RIVIERE) {
				SDL_SetRenderDrawColor(renderer, 52, 152, 219,0);
			}
			else if(carte[x][y].type == PIEGE) {
				SDL_SetRenderDrawColor(renderer, 231, 76, 60,0);
			}
			else if(carte[x][y].type == DEPART) {
				SDL_SetRenderDrawColor(renderer, 214, 137, 16 , 0);
			}
			else if(carte[x][y].type == MONTAGNE) {
				SDL_SetRenderDrawColor(renderer, 166, 172, 175,0);
			}
			else if(carte[x][y].type == COFFRE) {
				SDL_SetRenderDrawColor(renderer, 165, 105, 189,0);
			}

			if(carte[x][y].x == noeud_courant->x && carte[x][y].y == noeud_courant->y) {
				SDL_SetRenderDrawColor(renderer, 241, 196, 15 , 0);
			}

			if(carte[x][y].est_solution) {
				SDL_SetRenderDrawColor(renderer, 214, 137, 16 , 0);
			}
			/*
			 * Attention pour l'affichage il faut inverser
			*/
			rect.y = (carte[x][y].x * (HAUTEUR / X_CARTE));
			rect.x = (carte[x][y].y * (LARGEUR / Y_CARTE));
			rect.w = LARGEUR / Y_CARTE - ESPACE_CELL;
			rect.h = HAUTEUR / X_CARTE - ESPACE_CELL;

			SDL_RenderFillRect(renderer, &rect );
			if(carte[x][y].est_dans_la_table_ouverte || carte[x][y].est_dans_la_table_fermee) {
				SDL_RenderCopy(renderer, texture, NULL, &rect);
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surfaceMessage);
			}
			/* afficher à l'ecran */
			SDL_RenderPresent(renderer);
		}
	}
}

/* #################################################################################
	 						EXECUTION
   ################################################################################# */
int main(int argc, char* argv[])
{

	/*
	* Initialisation de la fonte
	*/
	TTF_Init();
	recuperer_chemin_programme();
	initFont();

	/*
	 * Interaction avec l'utilisateur pour lui demander les coordonnees du tresor du point de depart des 3 coffres
	*/
	demander_les_coordonnees();

	/*
	* Initialisation de l'interface graphique
	*/
	initInterfaceGraphique();

	int z;
	for(z = 0; z < NB_COFFRE + 1; ++z) {
		/*
		* INITIALISATION DE LA CARTE
		*/
		init_carte();
		print_coordonnees_carte();
		int execution = 0;

		if(z < NB_COFFRE + 1)
			execution = 1;

		SDL_Event event;

		/*
	    * UTILISATION DE L'ALGO A *
		*/
		nb_table_ouverte = 0;
		nb_table_fermee = 0;
		AStarNoeud * noeud_position_depart;  /* Notre position pour le depart */
		AStarNoeud * noeud_position_arriver; /* Notre position pour l'arriv�e */

		/*
		* Dans le cas de l'enonce, on choisit le chemin suivant
		* noeud_depart -> un noeud_coffre -> un autre noeud_coffre -> un autre noeud_coffre -> noeud_tresor
		*/

		if(z == 0) {
			noeud_position_depart  = noeud_depart;
			noeud_position_arriver = noeud_coffre1;
		}
		if(z == 1) {
			noeud_position_depart = noeud_coffre1;
			noeud_position_arriver = noeud_coffre2;
		}

		if(z == 2) {
			noeud_position_depart = noeud_coffre2;
			noeud_position_arriver = noeud_coffre3;
		}

		if(z == 3) {
			noeud_position_depart = noeud_coffre3;
			noeud_position_arriver = noeud_tresor;
		}

		/*
		*  0. Initialisation du point de depart
		*/
		noeud_position_depart->est_dans_la_table_ouverte = 1; /* Il est bien dans la table ouverte */
		noeud_position_depart->poids = 0; /* Le poids du point de depart est de 0  */
		noeud_position_depart->heuristique = calcul_heuristique(noeud_position_depart->x, noeud_position_depart->y, noeud_position_arriver->x, noeud_position_arriver->y); /* Calcul de l'heuristique */
		noeud_position_depart->parent = NULL; /* Pas de parent  */
		noeud_position_depart->est_solution = 1; /* le noeud de depart fait forcement partie de la solution */

		/* 1. Ajout du noeud de depart a la table ouverte */
		table_ouverte[nb_table_ouverte++] = noeud_position_depart;/* On l'ajoute dans la table ouverte  */

		int est_terminee = 0;
		while (execution) {
			while (SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_MOUSEBUTTONDOWN:
						if(!est_terminee){ /* Ici on ne veut pas qu'� la fin le programme se quitte du coup on enleve l'event MOUSEDUTTONDOWN lorsque le tresor est trouve!
								/*
							 * 2. A chaque iteration on recupere le noeud qui a le plus petit cout (heuristique + poids) dans la table ouverte.
							 * Ce noeud c'est le premier de  la table ouverte puisque qu'elle est triee.
							 */
							noeud_courant = table_ouverte[0];

							/*
							 * 3. Ajouter le noeud choisit a la table fermee. Cette table represente les noeuds deja etudies.
							 */
							if (nb_table_fermee + 1 < MAX_SIZE) {
								table_fermee[nb_table_fermee++] = noeud_courant;
								noeud_courant->est_dans_la_table_fermee = 1; /* Le noeud est desormais dans la table fermee */
								noeud_courant->est_dans_la_table_ouverte = 0; /* Le noeud est etudie il n'est plus dans la table ouverte */
							}

							/*
							 * 4. On v�rifie que le tresor n'est pas trouve
							 */
							if (verifier_tresor_trouve(noeud_courant, noeud_position_arriver)) {
							/*
							 * 4.1 Dans ce cas on a termine on sort
							*/
							 	print_meilleur_chemin(noeud_position_arriver);
							 	/* On affiche la solution graphiquement avant de quitter */
							 	afficherEcran(noeud_courant);
							 	printf("z=%d", z);
							 	/* SDL_Delay(5000);	*/
							 	/* A la fin on reste en pause */
							 	execution = 1;
							 	est_terminee = 1;
							 	break;
							 }

							 /*
							  * 4.2 On verifie qu'il y a encore des noeuds a etudier
							  * Dans le cas ou il n'y a plus de noeuds a etudier on arrete. Il n'y a pas de solution
							  */
							if(verifier_aucun_chemin()) {
							 	printf("Il n'y a pas de chemin jusqu'au tresor!\n");
							 	est_terminee = 1;
							 	execution = 0;
							 	break;
							}

							 /*
							 * 5. Si le tresor n'est pas trouve on ajoute les voisins du noeuds courant a la table ouverte et on recommence!
							 */
							ajouter_les_voisins(noeud_courant, noeud_position_arriver);

							/* 6. On retire le premier element qu'on vient d'etudier */
					 		retirer_premier_element_table_ouverte();

					 		/* Pour tester on affiche les valeurs de la table ouverte */
					 		/*
					 		printf("Les valeurs (x, y, cout = poids + heuristique) dans la table ouverte sont :");
							print_table_ouverte();
							printf("\n");
							*/
							 /*
							 * 7. Une fois les nouveaux voisins ajoutes la table ouverte est triee pour la prochaine iteration
							 */
							heapSort();

							afficherEcran(noeud_courant);
							break;
						}
					case SDL_QUIT :
						execution = 0;
						break;
				}
			}
			SDL_Delay(1);
		}
	}

	/* On ferme correctement la partie graphique */
	desctructionGraphique();

	/* Au revoir! */
	return EXIT_SUCCESS;
}
