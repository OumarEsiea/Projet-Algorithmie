#include "astar.h"

/* #################################################################################
	 									ALGO A STAR
   ################################################################################# */
/*
* L'heuristique repr�sente le calcul de la distance entre le point de d�part et le point d'arriv�e
*
*/
int calcul_heuristique(int x_noeud_depart, int y_noeud_depart, int x_noeud_arriver, int y_noeud_arriver ) {
	return abs(x_noeud_arriver - x_noeud_depart) + abs(y_noeud_arriver - y_noeud_depart);
}

/*
* Ajouter les nouveaux noeuds dans la table ouverte
*/
void ajouter_table_ouverte(int x, int y, int poids, pStarNoeud noeud_courant, pStarNoeud noeud_tresor) {
	/* On verifie que la position actuelle n'est pas dans la table fermee dans ce cas la on ne fait rien */
	if(carte[x][y].est_dans_la_table_fermee)  {
		return;
	}

	/* Il ne faut pas que se soit un piege! */
	if(carte[x][y].type == PIEGE)  {
		return;
	}

	/* Si il n'est pas dans la table ouverte on l'ajoute */
	if(!carte[x][y].est_dans_la_table_ouverte)
	{
		carte[x][y].poids = noeud_courant->poids + poids;
		carte[x][y].heuristique = calcul_heuristique(x, y, noeud_tresor->x, noeud_tresor->y);
		carte[x][y].parent = noeud_courant;
		carte[x][y].est_dans_la_table_ouverte = 1;
		table_ouverte[nb_table_ouverte++] = &(carte[x][y]);
	}

	/* Si il est deja dans la table ouverte on regarde si le noeud courant est une meilleure solution */
	if(carte[x][y].est_dans_la_table_ouverte && (noeud_courant->poids + poids < carte[x][y].poids)) {
		/*
		* Dans ce cas on met a jour la poids entre la position et le noeud courant qui est la meilleure solution par rapport aux autres noeuds voisins
		*/
		carte[x][y].poids = noeud_courant->poids + poids;
		/*
		* Puis on ajoute le noeud courant en temps que parent (c'est la meilleure solution)
		*/
		carte[x][y].parent = noeud_courant;
	}
}

/*
* Calcul du cout en fonction des coordonnees d'un noeud sur la carte
*/
int calcul_cout_par_type(int x, int y) {

	/*
	#define MONTAGNE	 1
	#define FORET		 2
	#define PIEGE		 3
	#define RIVIERE		 4
	#define COFFRE		 5
	#define TRESOR		 6
	#define	DEPART		 7
	*/
	if(carte[x][y].type == TERRAIN_PLAT) {
		return 0; /* Un terrain plat n'est pas une difficulte! */
	}

	if(carte[x][y].type == MONTAGNE) {
		return 600;
	}

	if(carte[x][y].type == FORET) {
		return 400;
	}

	if(carte[x][y].type == RIVIERE) {
		return 200;
	}

	return 0;
}
/*
* Cette fonction ajoute tous les voisins dans la table ouverte (8 voisins potentiels au maximum pour chaque position)
*/
void ajouter_les_voisins(pStarNoeud noeud_courant, pStarNoeud noeud_tresor) {
	/*
	 * Au cas ou on verifie si on peut ajouter le nouveau noeud a la table si il reste de la place
	*/
	if(nb_table_ouverte + 1 >= MAX_SIZE) {
		return;
	}

	int x = noeud_courant->x;
	int y = noeud_courant->y;

	if(check_coordonnees(x+1,y)) {
		ajouter_table_ouverte(x+1, y, DISTANCE_LIGNE_DROITE + calcul_cout_par_type(x+1, y), noeud_courant, noeud_tresor);
	}

	if(check_coordonnees(x-1,y)) {
		ajouter_table_ouverte(x-1, y, DISTANCE_LIGNE_DROITE + calcul_cout_par_type(x-1, y), noeud_courant, noeud_tresor);
	}

	if(check_coordonnees(x,y+1)) {
		ajouter_table_ouverte(x, y+1, DISTANCE_LIGNE_DROITE + calcul_cout_par_type(x, y+1), noeud_courant, noeud_tresor);
	}

	if(check_coordonnees(x,y-1)) {
		ajouter_table_ouverte(x, y-1, DISTANCE_LIGNE_DROITE + calcul_cout_par_type(x, y-1), noeud_courant, noeud_tresor);
	}

	if(check_coordonnees(x+1,y+1)) {
		ajouter_table_ouverte(x+1, y+1, DISTANCE_DIAGONALE + calcul_cout_par_type(x+1, y+1), noeud_courant, noeud_tresor);
	}

	if(check_coordonnees(x-1,y+1)) {
		ajouter_table_ouverte(x-1, y+1, DISTANCE_DIAGONALE + calcul_cout_par_type(x-1, y+1), noeud_courant, noeud_tresor);
	}

	if(check_coordonnees(x+1,y-1)) {
		ajouter_table_ouverte(x+1, y-1, DISTANCE_DIAGONALE + + calcul_cout_par_type(x+1, y-1), noeud_courant, noeud_tresor);
	}

	if(check_coordonnees(x-1,y-1)) {
		ajouter_table_ouverte(x-1, y-1, DISTANCE_DIAGONALE + + calcul_cout_par_type(x-1, y-1), noeud_courant, noeud_tresor);
	}
}

/*
* Retire le premier element de la tableau ouverte
*/
void retirer_premier_element_table_ouverte() {
	int i;
	if(nb_table_ouverte > 0) { /* On verifie qu'il reste des elements a retirer */
		for(i = 0; i < nb_table_ouverte; i++) {
			table_ouverte[i] = table_ouverte[i + 1];
		}
		/* Ne pas oublier de decrementer le nombre d'elements dans le tableau */
		nb_table_ouverte--;
	}
}

/*
* Verifie si la position du noeud courant correspond a la position du tresor. Retourne 1 si oui 0 sinon.
*/
int verifier_tresor_trouve(pStarNoeud noeud_courant, pStarNoeud noeud_tresor) {
	if(noeud_courant->x == noeud_tresor->x && noeud_courant->y == noeud_tresor->y)
		return 1;
	return 0;
}

/*
* Verifie si il n'y a pas de noeud a etudier dans la table ouverte dans ce cas aucune solution n'est trouvee. Retourne 1 si il n'y a pas de chemin jusqu'au tresor. 0 sinon.
*/
int verifier_aucun_chemin() {
	if(nb_table_ouverte <= 0) {
		return 1;
	}
	return 0;
}

/*
* Verifie que les coordonn�es x & y sont bien dans la carte. Retourne 1 si ok 0 sinon.
*/
int check_coordonnees(int x, int y) {
	if(x >= 0 && x < X_CARTE && y >= 0 && y < Y_CARTE)
		return 1;
	return 0;
}

/* #################################################################################
	 						ALGO DE TRI - HEAP SORTING
   ################################################################################# */
/*
* Echanger deux elements dans la table ouverte
* info: https://www.programiz.com/dsa/heap-sort
*/
void echanger(int indice1, int indice2)
{
	pStarNoeud tmp = table_ouverte[indice1];
	table_ouverte[indice1] = table_ouverte[indice2];
	table_ouverte[indice2] = tmp;
}

/* HEAP SORTING - Algorithm de tri
 * info: https://www.programiz.com/dsa/heap-sort
 */
void heapify(int n, int indice) {
	int plus_grand;
	int gauche;
	int droite;
	plus_grand = indice;
    gauche = 2 * indice + 1;
    droite  = 2 * indice + 2;

    if (gauche < n) {

    	int poids_gauche = table_ouverte[gauche]->poids + table_ouverte[gauche]->heuristique;
    	int poids_plus_grand = table_ouverte[plus_grand]->poids + table_ouverte[plus_grand]->heuristique;

    	if(poids_gauche > poids_plus_grand) {
    		plus_grand = gauche;
    	}
    }

    if (droite < n) {

    	int poids_droite= table_ouverte[droite]->poids + table_ouverte[droite]->heuristique;
    	int poids_plus_grand = table_ouverte[plus_grand]->poids + table_ouverte[plus_grand]->heuristique;

    	if(poids_droite > poids_plus_grand) {
    		plus_grand = droite;
    	}
    }

    /* On echange les noeuds tant que c'est pas dans l'ordre */
    if (plus_grand != indice) {
       echanger(indice, plus_grand);
       heapify(n, plus_grand);
    }
}

 /*
 * HEAP SORTING - Algorithme de tri
 * info: https://www.programiz.com/dsa/heap-sort
 */
  void heapSort() {
  	int n = nb_table_ouverte;
  	int i;
    for (i = n / 2 - 1; i >= 0; i--)
		heapify(n, i);

    for (i = n - 1; i >= 0; i--) {
		echanger(0, i);
        /* On continue le tri et a chaque iteration on veut le plus petit en premier */
        heapify(i, 0);
    }
}
