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
#include "reseau/Reseau.h"

#include "ihm/MMultijoueur.h"

using namespace JktMenu;

class CMaterial;
class CLight;
class CMouve;
class CGame;
class CGeoObject;
class CPorte;

extern JktNet::CReseau Reseau;
extern CCfg Config;

#include "ihm/MMenuPrinc.h"
#include "ihm/MMultijoueurServeurLancePartie.h"


namespace JktMenu
{
	/************************************/
	/*									*/
	/*   MENU MULTI-JOUEURS SERVEUR		*/
	/*									*/
	/************************************/
CDlgBoite *BoiteErreurMultijoueurserveur = 0;

PF liste_suivant_multijoueurserveur[] =
{
	lanceMenuPartiemulti,
};

const char *item_menu_multijoueurserveur[] =
{
	"Lancer une partie",
};

void retourMultiserveur( void *arg )
{
TRACE().p( TRACE_MENU, "retourMultiserveur(var=%x)", arg );
	Reseau.fermeServer();
	lanceMenuPrinc( 0 );
}

CMenu MenuMultijoueurserveur( "SERVEUR MULTI-JOUEURS", item_menu_multijoueurserveur, 1,
						liste_suivant_multijoueurserveur, retourMultiserveur );

void retourErreurMultijoueurserveur( void *arg )
{
TRACE().p( TRACE_MENU, "retourErreurMultijoueurserveur(var=%x)", arg );
	if( BoiteErreurMultijoueurserveur )
		delete BoiteErreurMultijoueurserveur;
	BoiteErreurMultijoueurserveur = 0;
	lanceMenuPrinc( 0 );
}

void lanceMenuMultijoueurserveur(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuMultijoueurserveur(var=%x)", var );

	if( var )		// S'il y a demande de connection réseau
	{
		if( !Reseau.ouvreServer( Config.Reseau.getPort() ) )
		{
			cout << endl << "Echec ouverture server";
			Reseau.setOn( false );		// Signale que le réseau ne peut pas être utilisé
			BoiteErreurMultijoueurserveur = new CDlgBoite( "Erreur", "Le serveur ne peut etre ouvert", retourErreurMultijoueurserveur, CDlgBoite::JKT_DLG_ERREUR );
			BoiteErreurMultijoueurserveur->addBouton( 2, "Ok", retourErreurMultijoueurserveur );
			CDlg::SetMenuActif( BoiteErreurMultijoueurserveur );
		}
		else
		{
			CDlg::SetMenuActif( &MenuMultijoueurserveur );
		}
	}
	else
	{
		CDlg::SetMenuActif( &MenuMultijoueurserveur );
	}
}

}	// JktMenu
