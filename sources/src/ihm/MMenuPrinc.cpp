	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "util/Trace.h"

#include "main/Game.h"
#include "main/Focus.h"

#include "ihm/MMenuPrinc.h"

using namespace jkt;

extern CFocus *pFocus;
extern bool Aide;
extern CGame Game;

namespace jkt
{
extern CMenu MenuPrinc2;

void quit_JKTici(void *var);
void lanceMenuConfig(void *var);		// Menu de configuration vid�o, son, r�seau, clavier,...
void lanceMenuOpenScene(void *var);		// Menu d'ouverture fichiers ASE et MAP
void lanceMenuTest(void *var);
void lanceMenuTestencours( void *var );

void lanceMenuTest(void *var)
{
LOGDEBUG(("lanceMenuTest(var=%x)", var));
	CDlgBoite* BoiteTest = new CDlgBoite( "Test 1", "Coucou comment ca va gros trou du cul qui pue de la moule frite sous son blouson", lanceMenuPrinc, CDlgBoite::JKT_DLG_ERREUR );
	BoiteTest->addBouton( 1, "Menu", lanceMenuPrinc );
	BoiteTest->addBouton( 2, "Test", lanceMenuTestencours );
	BoiteTest->addBouton( 3, "Rien", 0 );

	CDlg::SetMenuActif( BoiteTest );
}

void lanceMenuTestencours( void *var ) {
LOGDEBUG(("lanceMenuTestencours(var=%x)", var));
	CDlgBoite *BoiteTestEnCours = new CDlgBoite( "Tache en cours", "Ouverture du fichier ASE en cours...", lanceMenuPrinc, CDlgBoite::JKT_DLG_ENCOURS );
	CDlg::SetMenuActif( BoiteTestEnCours );
}

PF liste_suivant_princ[] =
{
	lanceMenuOpenScene,
	0,
	lanceMenuConfig,
	lanceMenuTest,
	quit_JKTici,
};

const char *item_menu_princ[] =
{
	"Ouvrir une sc\u00e8ne",
	"Multi-joueurs",
	"Configuration",
	"TEST",
	"Quitter",
};

CMenu MenuPrinc( "MENU PRINCIPAL", item_menu_princ, 5, liste_suivant_princ, 0 );

void lanceMenuPrinc(void *var) {
LOGDEBUG(("lanceMenuPrinc(var=%x)", var));

	if(Game.isModeNull())
		CDlg::SetMenuActif( &MenuPrinc );
	else
		CDlg::SetMenuActif( &MenuPrinc2 );

	pFocus->SetMenuFocus();		// Mets le focus sur le menu
	Aide = true;
}

}	// JktMenu
