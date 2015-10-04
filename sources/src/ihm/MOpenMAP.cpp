	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#include <iostream>
#ifdef WIN32
	#include <io.h>
#endif
using namespace std;

#include "util/FindFolder.h"
#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "main/Game.h"
#include "main/Focus.h"
#include "util/Trace.h"
#include "ihm/MMenuPrinc.h"

#include "MOpenScene.h"

#include "MOpenMAP.h"

using namespace JktMenu;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;

bool deprecatedOpenMAP(const void *arg);

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;

namespace JktMenu
{

	/************************/
	/*						*/
	/*   MENU OPEN MAP		*/
	/*						*/
	/************************/
void retourMAP(void *var);

CMenu *MenuOpenMAP;

void suivantMAP(void *arg)
{
	if(deprecatedOpenMAP( arg )) {
		Aide = false;
		pFocus->SetPlayFocus();
		Game.setModeLocal();
	}
	else {
		CDlgBoite* BoiteEchec = new CDlgBoite( "Echec lecture fichier Map", "La lecture du fichier Map a echoue", lanceMenuPrinc, CDlgBoite::JKT_DLG_ERREUR );
		BoiteEchec->addBouton( 2, "Ok", lanceMenuPrinc );

		CDlg::SetMenuActif(BoiteEchec);
	}
}

void lanceMenuOpenMAP(void *var)
{
LOGDEBUG(("lanceMenuOpenMAP(var=%x)", var));
	int nbrFichier = 0;		// Nombre de fichiers ASE � prendre en compte

	PF *liste_suivant_open_MAP;
	char **item_menu_open_MAP;
	void **liste_argument_open_MAP;

	string name;

	CFindFolder folder( "./Map/", 0, ".map.xml" );
	nbrFichier = folder.nbr();

	liste_suivant_open_MAP = new PF[ nbrFichier ];
	liste_argument_open_MAP = new void*[ nbrFichier ];
	item_menu_open_MAP = new char*[ nbrFichier ];

	nbrFichier = 0;
	folder.reset();

	while( folder.findNext( name ) )
	{
		name.erase( name.find_last_of( "." ) );
		name.erase( name.find_last_of( "." ) );
		liste_suivant_open_MAP[nbrFichier] = suivantMAP;
		item_menu_open_MAP[nbrFichier] = new char[name.size()+1];
		liste_argument_open_MAP[nbrFichier] = new char[name.size()+1];
		strcpy( item_menu_open_MAP[nbrFichier], name.c_str() );
		strcpy( (char*)(liste_argument_open_MAP[nbrFichier]), name.c_str() );
		nbrFichier++;
	}

	MenuOpenMAP = new CMenu( "OUVRIR UNE SCENE MAP", (const char**)item_menu_open_MAP, nbrFichier,
						liste_suivant_open_MAP, retourMAP, liste_argument_open_MAP );

	CDlg::SetMenuActif( MenuOpenMAP );
}

void retourMAP(void *var)	// Lib�rations m�moire et retour au menu sup�rieur
{
LOGDEBUG(("retourMAP(var=%x)", var));
	delete MenuOpenMAP;
	CDlg::SetMenuActif( 0 );

	lanceMenuOpenScene( 0 );
}

}	// JktMenu
