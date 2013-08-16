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

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "util/Trace.h"

#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "reseau/NetworkManager.h"

#include "ihm/MMultijoueur.h"

#include "ihm/MMultijoueurClientInfo.h"

using namespace JktMenu;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;
class NetworkManager;

extern JktNet::NetworkManager Reseau;


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

const char *item_menu_infoserver[] =
{
	"Nom serveur",
	"Map active",
};

void retourInfoserver( void *arg ) {
TRACE().p( TRACE_MENU, "retourInfoserver(var=%x)", arg );
	lanceMenuMultijoueurclient( 0 );
}

CMenu MenuInfoserver( "INFOS SERVEUR", item_menu_infoserver, 2,
						liste_suivant_infoserver, retourInfoserver, 0, refreshInfoserver );

void refreshInfoserver() {
	JktNet::CClient::CInfoServer infoServer = Reseau.getClient()->getInfoServer();

	if( infoServer.Ready() == true )
	{
		MenuInfoserver.add_ItemsDroits( 0, infoServer.nom.c_str() );
		MenuInfoserver.add_ItemsDroits( 1, infoServer.map.c_str() );

		infoServer.Ready( false );
	}
}

void lanceInfoserver(void *arg)
{
TRACE().p( TRACE_MENU, "lanceInfoServer(var=%x)", arg );

	MenuInfoserver.add_ItemsDroits( 0, "????" );
	MenuInfoserver.add_ItemsDroits( 1, "????" );
	Reseau.getClient()->sendRequestInfoToServer();		// Demande ses infos au serveur

	CDlg::SetMenuActif( &MenuInfoserver );
}

}	// JktMenu
