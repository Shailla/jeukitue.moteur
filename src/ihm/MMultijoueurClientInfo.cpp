	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#ifdef WIN32
	#include <windows.h>
#endif

using namespace std;

#include "Menu/DlgBoite.h"
#include "Menu/Menu.h"
#include "util/Trace.h"

#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "reseau/Reseau.h"

#include "Menu/MMultijoueur.h"

#include "Menu/MMultijoueurClientInfo.h"

using namespace JktMenu;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;
class CReseau;

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;
extern JktNet::CReseau Reseau;
extern CCfg Config;


namespace JktMenu
{
	/************************************/
	/*									*/
	/*   MENU REQUETE INFO SERVEUR		*/
	/*									*/
	/************************************/

void refreshInfoserver();

PF liste_suivant_infoserver[] =
{
	0,
	0,
};

char *item_menu_infoserver[] =
{
	"Nom serveur",
	"Map active",
};

void retourInfoserver( void *arg )
{
TRACE().p( TRACE_MENU, "retourInfoserver(var=%x)", arg );
	lanceMenuMultijoueurclient( 0 );
}

CMenu MenuInfoserver( "INFOS SERVEUR", item_menu_infoserver, 2,
						liste_suivant_infoserver, retourInfoserver, 0, refreshInfoserver );

void refreshInfoserver()
{
	if( Reseau.getInfoServer().Ready() == true )
	{
		MenuInfoserver.add_ItemsDroits( 0, Reseau.getInfoServer().nom.c_str() );
		MenuInfoserver.add_ItemsDroits( 1, Reseau.getInfoServer().map.c_str() );

		Reseau.getInfoServer().Ready( false );
	}
}

void lanceInfoserver(void *arg)
{
TRACE().p( TRACE_MENU, "lanceInfoServer(var=%x)", arg );

	MenuInfoserver.add_ItemsDroits( 0, "????" );
	MenuInfoserver.add_ItemsDroits( 1, "????" );
	Reseau.sendRequestInfoServer();		// Envoie ses infos au serveur

	CDlg::SetMenuActif( &MenuInfoserver );
}

}	// JktMenu
