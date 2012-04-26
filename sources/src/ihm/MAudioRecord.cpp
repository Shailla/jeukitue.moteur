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

	/************************************************/
	/*												*/
	/*   MENU CONFIGURATION AUDIO DRIVERS RECORD	*/
	/*												*/
	/************************************************/
#include "ihm/MAudioDrivers.h"

namespace JktMenu
{

CMenu *MenuRecord;

void retourRecord(void *var)	// Libérations mémoire et retour au menu supérieur
{
TRACE().p( TRACE_MENU, "retourRecord(var=%x)", var );
	CDlg::SetMenuActif( 0 );
	delete MenuRecord;

	lanceMenuConfigAudioDrivers( 0 );
}

void lanceRecord(void *arg)
{
TRACE().p( TRACE_MENU, "lanceRecord(var=%x)", arg );
//	FSOUND_Record_SetDriver( (int)arg );
	Config.Audio.m_DriverRecord = *(int*)arg;
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(0);
}

void lanceMenuConfigAudioDriversRecord(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigAudioDriversRecord(var=%x)", var );
	int nbrRecord = FSOUND_Record_GetNumDrivers();	// Nombre driver possibles

	PF *liste_suivant;
	char **item_menu;
	void **liste_argument;

	liste_suivant = new PF[nbrRecord];
	liste_argument = new void*[nbrRecord];
	item_menu = new char*[nbrRecord];

	for( int i=0 ; i<nbrRecord ; i++ )
	{
		liste_suivant[ i ] = lanceRecord;

		item_menu[ i ] = new char[strlen((char*)FSOUND_Record_GetDriverName(i))+1];
		liste_argument[ i ] = (void*)new int;
		*((int*)liste_argument[ i ]) = i;
		strcpy( item_menu[ i ], (char*)FSOUND_Record_GetDriverName(i) );
	}

	MenuRecord = new CMenu( "RECORD",
						(const char**)item_menu, nbrRecord,
						liste_suivant, retourRecord, liste_argument );

	delete[] liste_suivant;
	delete[] item_menu;
	delete[] liste_argument;

	CDlg::SetMenuActif( MenuRecord );
}

}	// JktMenu
