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
#include "reseau/Reseau.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "reseau/Reseau.h"

#include "ihm/MMultijoueur.h"

#include "ihm/MMultijoueurClientInfo.h"

using namespace JktMenu;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;
class CReseau;

extern JktNet::CReseau Reseau;


namespace JktMenu
{
	/************************************/
	/*									*/
	/*   MENU REQUETE PING SERVEUR		*/
	/*									*/
	/************************************/

char title_menu_pingserver[] = "PING SERVEUR";
void actuPing( void *arg );
void refreshPingserver();

bool bAttenteMenuPing = false;

PF liste_suivant_pingserver[] =
{
	actuPing,
};

const char *item_menu_pingserver[] =
{
	"Actualiser",
};

void retourPingserver( void *arg)
{
TRACE().p( TRACE_MENU, "retourPingserver(var=%x)", arg );
	CDlg::SetMenuActif( 0 );
	lanceMenuMultijoueurclient( 0 );
}

CMenu MenuPingserver(title_menu_pingserver, item_menu_pingserver, 1,
						liste_suivant_pingserver, retourPingserver, 0, refreshPingserver );

void refreshPingserver()
{
	if( bAttenteMenuPing )
	{
		Uint32 ping = Reseau.getPingClientServer();
		if(ping> 0) {
			if( ping >=9999 )	// Délai trop long !
			{
				MenuPingserver.add_ItemsDroits( 0, "----" );
				bAttenteMenuPing = false;
			}
			else
			{
				stringstream str;
				str << ping << " ms";;

				MenuPingserver.add_ItemsDroits( 0, str.str().c_str() );
				bAttenteMenuPing = false;
			}
		}
	}
}

void lancePingserver(void *arg)
{
TRACE().p( TRACE_MENU, "lancePingServer(var=%x)", arg );

	actuPing( 0 );								// Demande l'actualisation du ping

	CDlg::SetMenuActif( &MenuPingserver );
}

void actuPing( void *arg )	// Réactualise la valeur du ping
{
	bAttenteMenuPing = true;			// On passe en état d'attente d'un ping
	Reseau.sendPingClientServer();		// Envoie un ping au serveur
	MenuPingserver.add_ItemsDroits( 0, "????" );
}

}	// JktMenu
