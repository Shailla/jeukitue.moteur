	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
using namespace std;

#include "fmod.h"

#include "Menu/DlgBoite.h"
#include "Menu/Menu.h"

#include "main/Game.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

#include "Menu/MAudio.h"

using namespace JKT_PACKAGE_MENU;

extern CGame Game;
extern CMenu MenuPrinc2;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;
#include "Menu/MAudioDrivers.h"
extern char **remarquesDriver;

namespace JKT_PACKAGE_MENU
{
	/************************************************/
	/*												*/
	/*   MENU CONFIGURATION AUDIO DRIVERS DRIVER	*/
	/*												*/
	/************************************************/

CMenu *MenuDriver;

void retourDriver(void *var)	// Libérations mémoire et retour au menu supérieur
{
TRACE().p( TRACE_MENU, "retourDriver(var=%x)", var );
	CDlg::SetMenuActif( 0 );
	delete MenuDriver;

	lanceMenuConfigAudioDrivers( 0 );
}

void lanceDriver(void *arg)
{
TRACE().p( TRACE_MENU, "lanceDriver(var=%x)", arg );
//	FSOUND_SetDriver( (int)arg );
	Config.Audio.m_Driver = *(int*)arg;
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(0);
}

void lanceMenuConfigAudioDriversDriver(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigDriversDriver(var=%x)", var );
	int nbrDriver = FSOUND_GetNumDrivers();	// Nombre driver possibles

	PF *liste_suivant;
	char **item_menu;
	void **liste_argument;

	liste_suivant = new PF[nbrDriver];
	liste_argument = new void*[nbrDriver];
	item_menu = new char*[nbrDriver];

	for( int i=0 ; i<nbrDriver ; i++ )
	{
		liste_suivant[ i ] = lanceDriver;

		item_menu[ i ] = new char[strlen((char*)FSOUND_GetDriverName(i))+1];
		liste_argument[ i ] = (void*)new int;
		*((int*)liste_argument[ i ]) = i;
		strcpy( item_menu[ i ], (char*)FSOUND_GetDriverName(i) );
	}

	MenuDriver = new CMenu( "DRIVER",
						item_menu, nbrDriver,
						liste_suivant, retourDriver, liste_argument );

	for( int i=0 ; i<nbrDriver ; i++ )
	{
		MenuDriver->add_ItemsRem( i, remarquesDriver[i] );
	}

	delete[] liste_suivant;
	delete[] liste_argument;
	delete[] item_menu;

	CDlg::SetMenuActif( MenuDriver );
}

}	// JKT_PACKAGE_MENU
