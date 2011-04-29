	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
using namespace std;

#include "Menu/DlgBoite.h"
#include "Menu/Menu.h"

#include "main/Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

#include "Menu/MMenuPrinc.h"

using namespace JktMenu;

class CGame;

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;

#include "Menu/MConfiguration.h"

namespace JktMenu
{
extern CMenu MenuPrinc2;

void actu_menu_config_debug_SonPerformances( void *arg );
void actu_menu_config_debug_SonSpectre( void *arg );
void actu_menu_config_debug_AfficheFichier( void *arg );
void actu_menu_config_debug_ReseauDebit( void *arg );
void actu_menu_config_debug_AfficheNormaux( void *arg );

	/************************************/
	/*									*/
	/*   MENU CONFIGURATION DEBUG		*/
	/*									*/
	/************************************/

PF liste_suivant_config_debug[] =
{
	actu_menu_config_debug_SonPerformances,
	actu_menu_config_debug_SonSpectre,
	actu_menu_config_debug_AfficheFichier,
	actu_menu_config_debug_ReseauDebit,
	actu_menu_config_debug_AfficheNormaux,
};

char *item_menu_config_debug[] =
{
	"Son, performances",
	"Son spectre",
	"Affiche ASE",
	"Debit reseau",
	"Affiche vecteurs normaux",
};

void retourMenuConfigDebug( void *arg)
{
TRACE().p( TRACE_MENU, "retourMenuConfigDebug(var=%x)", arg );
	lanceMenuConfig( 0 );
}

CMenu MenuConfigDebug( "DEBUG", item_menu_config_debug, 5,
						liste_suivant_config_debug, retourMenuConfigDebug );

void lanceMenuConfigDebug(void *arg)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigDebug(var=%x)", arg );
	if( Config.Debug.bSonPerformances )
		MenuConfigDebug.add_ItemsDroits( 0, "Oui" );
	else
		MenuConfigDebug.add_ItemsDroits( 0, "Non" );

	if( Config.Debug.bSonSpectre )
		MenuConfigDebug.add_ItemsDroits( 1, "Oui" );
	else
		MenuConfigDebug.add_ItemsDroits( 1, "Non" );

	if( Config.Debug.bAfficheFichier )
		MenuConfigDebug.add_ItemsDroits( 2, "Oui" );
	else
		MenuConfigDebug.add_ItemsDroits( 2, "Non" );

	if( Config.Debug.bReseauDebit )
		MenuConfigDebug.add_ItemsDroits( 3, "Oui" );
	else
		MenuConfigDebug.add_ItemsDroits( 3, "Non" );

	if( Config.Debug.bAfficheNormaux )
		MenuConfigDebug.add_ItemsDroits( 4, "Oui" );
	else
		MenuConfigDebug.add_ItemsDroits( 4, "Non" );

	CDlg::SetMenuActif( &MenuConfigDebug );
}

void actu_menu_config_debug_SonPerformances(void *arg)
{
	Config.Debug.bSonPerformances = !Config.Debug.bSonPerformances;
	Config.Ecrit();

	lanceMenuConfigDebug( 0 );
}

void actu_menu_config_debug_ReseauDebit(void *arg)
{
	Config.Debug.bReseauDebit = !Config.Debug.bReseauDebit;
	Config.Ecrit();

	lanceMenuConfigDebug( 0 );
}

void actu_menu_config_debug_SonSpectre(void *arg)
{
	Config.Debug.bSonSpectre = !Config.Debug.bSonSpectre;
	Config.Ecrit();

	lanceMenuConfigDebug( 0 );
}

void actu_menu_config_debug_AfficheFichier(void *arg)
{
	Config.Debug.bAfficheFichier = !Config.Debug.bAfficheFichier;
	Config.Ecrit();

	lanceMenuConfigDebug( 0 );
}

void actu_menu_config_debug_AfficheNormaux(void *arg)
{
	Config.Debug.bAfficheNormaux = !Config.Debug.bAfficheNormaux;
	Config.Ecrit();

	lanceMenuConfigDebug( 0 );
}

}	// JktMenu
