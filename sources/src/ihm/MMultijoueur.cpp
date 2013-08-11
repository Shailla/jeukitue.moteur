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

#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "reseau/Reseau.h"
#include "util/Trace.h"

#include "ihm/MMultijoueur.h"

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;

extern JktNet::CReseau Reseau;
extern CCfg Config;

	/************************************/
	/*									*/
	/*   MENU MULTI-JOUEURS CLIENT		*/
	/*									*/
	/************************************/

#include "ihm/MMenuPrinc.h"
#include "ihm/MMultijoueurClientInfo.h"
#include "ihm/MMultijoueurClientPing.h"
#include "ihm/MMultijoueur.h"

using namespace JktMenu;


namespace JktMenu
{
void lanceMenuMultijoueurserveur( void *arg );

void retourMultijoueurclient( void *arg )
{
TRACE().p( TRACE_MENU, "retourMultijoueurclient(var=%x)", arg );
	Reseau.fermeClient();
	lanceMenuPrinc( 0 );
}

PF liste_suivant_multijoueurclient[] =
{
	lanceInfoserver,		// Envoie une requête d'info du serveur
	lanceJoindrePartie,		// Se connecte sur la partie active sur le serveur
	lancePingserver,
};

const char *item_menu_multijoueurclient[] =
{
	"Info serveur",
	"Joindre partie",
	"Ping serveur",
};

CMenu MenuMultijoueurclient( "CLIENT MULTI-JOUEURS", item_menu_multijoueurclient, 3,
						liste_suivant_multijoueurclient, retourMultijoueurclient );

void lanceMenuMultijoueurclientconnect(void *var) {
TRACE().p( TRACE_MENU, "lanceMenuMultijoueurclientconnect(var=%x)", var );
	if( !Reseau.ouvreClient() )
	{
		cerr << endl << __FILE__ << ":" << __LINE__ << " Echec de connexion au serveur";
		Reseau.setOn( false );		// Signale que le réseau ne peut pas être utilisé
		lanceMenuPrinc( 0 );		// Retour au menu principal
	}
	else
	{
		CDlg::SetMenuActif( &MenuMultijoueurclient );
	}
}

void lanceMenuMultijoueurclient(void *arg) {
TRACE().p( TRACE_MENU, "lanceMenuMultijoueurclient(var=%x)", arg );
	CDlg::SetMenuActif( &MenuMultijoueurclient);
}

void lanceJoindrePartie( void *arg) {
TRACE().p( TRACE_MENU, "lanceJoindrePartie(var=%x)", arg );
	Reseau.getClient()->sendJoinTheGame(Config.Joueur.nom);
}

void lanceMenuMultijoueur(void *var) {
TRACE().p( TRACE_MENU, "lanceMenuMultijoueur(var=%x)", var );

	if(Config.Reseau.isServeur())
		lanceMenuMultijoueurserveur( (void*)true );	// Avec demande de connection réseau
	else
		lanceMenuMultijoueurclientconnect( 0 );
}

}	// JktMenu
