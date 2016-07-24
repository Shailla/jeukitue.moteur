
#ifndef __JKT__MENU_H
#define __JKT__MENU_H

namespace jkt
{

typedef void (*PF)(void *);
typedef void (*PFV)();

class CMenu : public CDlg
{
	int choixY;				// Item vertical actif
	int nbrChoixY;			// Nombre d'items verticaux
	int ajust;				// Décalage du focus
	void **arg;				// Arguments des fonctions suivantes
	char *titre;			// Titre du menu	
	char **items;			// Liste des items du menu
	char **items_droits;	// Items affichés à droite
	char **items_rem;		// Items remarques
	PF *fonction_suivante;	// Fonctions d'accès aux sous menu/item
	PF fonction_retour;		// Fonction retour de ce sous menu
	PFV fct_Refresh;		// Fonction de rafraichissement du menu
	void KeyDown( SDL_Event *event );
	void MouseBoutonDown( SDL_Event *event );

public:
	int mode;			// 0=normal 1=saisie de touche
	
	bool bItemsDroits;		// Items droits sélectionné ou non

	CMenu(const char *newTitre, const char **newItems, int nbrItems, PF *fct_suivante, PF fct_retour, void **liste_arg=0, PFV fct_refresh=0);
	~CMenu();

	void afficheTitre();			// Affiche le titre associé à cette classe menu
	void afficheItems();			// Affiche les items du menu
	void afficheItemsDroits();		// Affiche les items droit
	void afficheItemsRem();			// Afficher les remarques
	void down();					// Déplace vers le bas dans le menu
	void up();						// Déplace vers le haut dans le menu

	void add_ItemsDroits(int num, const char *txt);	// Implémente les items droits
	void add_ItemsRem(int num, char *txt);			// Implémente les items droits
	
	void del_Items();
	void del_ItemsDroits();			// Efface un item droit
	void del_ItemsRem();			// Efface un item droit

	PF enter();						// Retourne la fonction correspondant à l'item focussé
	void *argument();				// Retourne l'argument voulu pour l'item sélectionné

		// Fonctions virtuelles de la classe de base CDlg
	void go();									// Affiche le menu
	void handle_key_down( SDL_Event *event );	// Réagit aux touches claviers
};

}	// JktMenu

#endif

