	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#include <iostream>

using namespace std;

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"

#include "main/Game.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

#include "ihm/MAide.h"

using namespace JktMenu;

extern CFocus *pFocus;
extern bool Aide;


namespace JktMenu
{

	/****************/
	/*				*/
	/*   MENU AIDE	*/
	/*				*/
	/****************/

	// Menu d'aide principal
PF liste_suivant_aide[] =
{
	lanceMenuAideEssai,
	lanceMenuAideEssai,
	lanceMenuAideEssai,
	0,
};


const char* item_menu_aide[] =
{
	"G DESACTIVE OU ACTIVE LA GRAVITE",
	"C CHANGE DE JOUEUR ACTIF",
	"O REVIENT A L ORIGINE",
	"P AFFICHE LES COORDONNEES",
};

CMenu MenuAide( "LISTE DES TOUCHES", item_menu_aide, 4, liste_suivant_aide, 0 );

	// Sous menu d'aide

void fonctionAideEssai1(void *var)
{
	cout << "FONCTION 1\n";
}

void fonctionAideEssai2(void *var)
{
	cout << "FONCTION 2\n";
}

void fonctionAideEssai3(void *var)
{
	cout << "FONCTION 3\n";
}

PF liste_suivant_Aide_Essai[] =
{
	fonctionAideEssai1,
	fonctionAideEssai2,
	fonctionAideEssai3,
};

void lanceMenuAide(void *var)
{
TRACE().debug("lanceMenuAide(var=%x)", var);
	CDlg::SetMenuActif( &MenuAide );
	pFocus->SetMenuFocus();		// Mets le focus sur le menu
	Aide = true;
}

const char *item_menu_Aide_Essai[] =
{
	"CHOIX 1",
	"CHOIX 2",
	"CHOIX 3",
};

CMenu MenuAideEssai( "ESSAI", item_menu_Aide_Essai,	3, liste_suivant_Aide_Essai, lanceMenuAide );

void lanceMenuAideEssai(void *var)
{
TRACE().debug("lanceMenuAideEssai(var=%x)", var);
	CDlg::SetMenuActif( &MenuAideEssai );
}

}	// JktMenu
