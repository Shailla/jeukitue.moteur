	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
using namespace std;

#include "fmod.h"

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"

#include "main/Game.h"
#include "main/Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

#include "ihm/MAudio.h"

using namespace JktMenu;

extern CCfg Config;
#include "ihm/MAudioDrivers.h"
extern char **remarquesDriver;

namespace JktMenu
{
	/************************************************/
	/*												*/
	/*   MENU CONFIGURATION AUDIO DRIVERS DRIVER	*/
	/*												*/
	/************************************************/

CMenu *MenuDriver;

void retourDriver(void *var)	// Libérations mémoire et retour au menu supérieur
{
LOGDEBUG(("retourDriver(var=%x)", var));
	CDlg::SetMenuActif( 0 );
	delete MenuDriver;

	lanceMenuConfigAudioDrivers( 0 );
}

void lanceDriver(void *arg)
{
LOGDEBUG(("lanceDriver(var=%x)", arg));
//	FSOUND_SetDriver( (int)arg );
	Config.Audio.m_Driver = *(int*)arg;
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(0);
}

void lanceMenuConfigAudioDriversDriver(void *var)
{
LOGDEBUG(("lanceMenuConfigDriversDriver(var=%x)", var));
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
						(const char**)item_menu, nbrDriver,
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

}	// JktMenu
