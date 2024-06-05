#include <stdio.h>
#include <stdlib.h>

#define TERRAIN_PLAT 0
#define MONTAGNE	   1
#define FORET		     2
#define PIEGE		     3
#define RIVIERE		   4
#define COFFRE		   5
#define TRESOR		   6
#define	DEPART		   7

/* Definition de la taille de la carte */
#define X_CARTE 	25
#define Y_CARTE		21

#define MAX_SIZE	  300
#define NB_COFFRE	  3

/* On definie une valeur arbitraire pour la distance d'un deplacement en ligne droite
 * Les deplacements en ligne droite sont : haut, bas, gauche, droite
*/
#define DISTANCE_LIGNE_DROITE	10
/* On definie une valeur arbitraire pour la distance d'un deplacement en diagonale
 * Les deplacements en ligne droite sont : en haut � droite, en haut � gauche, en bas � droite, en bas � gauche
*/
#define DISTANCE_DIAGONALE		15
#define MAX_SIZE_CELL 20

/* Affichage des cellules de la grille (utilise pour l'affichage de la carte) */
#define LARGEUR_CELL 	20
#define HAUTEUR_CELL	20
#define ESPACE_CELL		1

/* Definition de la taille de la carte */
#define X_CARTE 	25
#define Y_CARTE		21



typedef struct AStarNoeud
{
	 int x; /* coordonnees x d'un noeud */
	 int y; /* coodonnees y d'un noeud */
	 int poids; /* Cout pour aller sur ce point. Le poids c'est l'addition de la distance (distance en ligne droite ou distance en diagonale)
	et le cout par type de case (montage, riviere...). La distance et le cout sont choisis arbitrairement */
	 int heuristique; /*  distance de ce point au point contenant le tr�sor pr�dit par la fonction heuristique */
	 int type; /*  Type de noeud (Motagne, foret, rivoere, coffre, tresor, depart...) */
	 struct AStarNoeud * parent; /*  Noeud parent */
	 int est_dans_la_table_fermee; /* Est-ce que ce noeud est dans la table ferm�e (0 non, 1 oui) */
	 int est_dans_la_table_ouverte;  /*  Est-ce que ce noeud est dans la table ouverte (0 non, 1 oui) */
	 int est_solution; /* Utilise pour afficher la meilleur solution */
}AStarNoeud, *pStarNoeud;

/* Definition des tailles maximum */


AStarNoeud carte[X_CARTE][Y_CARTE]; /* La matrice 2d representant la carte */
pStarNoeud table_ouverte[MAX_SIZE]; /* La table des noeuds ouverts */
pStarNoeud table_fermee[MAX_SIZE]; /*  La table des noeuds fermees */
pStarNoeud chemin_final[MAX_SIZE]; /*  Le chemin final qui representera la solution */
pStarNoeud coffre[NB_COFFRE];
AStarNoeud * noeud_depart; /* Noeud de depart */
AStarNoeud * noeud_coffre1; /* Noeud coffre numero 1 */
AStarNoeud * noeud_coffre2; /* Noeud coffre numero 2 */
AStarNoeud * noeud_coffre3; /* Noeud coffre numero 3 */
AStarNoeud * noeud_tresor; /* Noeud d'arriver */
AStarNoeud * noeud_courant; /* Position actuelle */
int nb_table_ouverte; /*  Le nombre de noeuds dans la table ouverte */
int nb_table_fermee; /*  Le nombre de noeuds dans la table fermee */

#ifndef ASTAR_H
#define ASTAR_H




int calcul_heuristique(int x_noeud_depart, int y_noeud_depart, int x_noeud_arriver, int y_noeud_arriver);
void ajouter_table_ouverte(int x, int y, int poids, pStarNoeud noeud_courant, pStarNoeud noeud_tresor);
int calcul_cout_par_type(int x, int y);
void ajouter_les_voisins(pStarNoeud noeud_courant, pStarNoeud noeud_tresor);
void retirer_premier_element_table_ouverte();
int verifier_tresor_trouve(pStarNoeud noeud_courant, pStarNoeud noeud_tresor);
int verifier_aucun_chemin();
int check_coordonnees(int x, int y);

void echanger(int indice1, int indice2);
void heapify(int n, int indice);
void heapSort();


#endif
