	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#ifdef WIN32
	#include <io.h>
	#include <windows.h>
#endif
using namespace std;

#include "util/FindFolder.h"
#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "util/Trace.h"

#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/Focus.h"
#include "reseau/Reseau.h"
#include "main/Cfg.h"
#include "main/Game.h"
#include "reseau/enumReseau.h"

#include "ihm/MMultijoueurServeurLancePartie.h"

using namespace JktMenu;

extern CGame Game;
extern CFocus *pFocus;
extern bool Aide;

#include "ihm/MMenuPrinc.h"
#include "ihm/MMultijoueurServeur.h"

void openMap( const string &nomFichierMap );

namespace JktMenu
{

	/****************************************************/
	/*													*/
	/*   MENU LANCE UNE PARTIE MULTIJOUEUR SERVEUR		*/
	/*													*/
	/****************************************************/

CMenu *MenuPartiemulti = 0;

void retourPartiemulti( void *arg )
{
TRACE().p( TRACE_MENU, "retourPartiemulti(var=%x)", arg );
	CDlg::SetMenuActif( 0 );
	if( MenuPartiemulti )
		delete MenuPartiemulti;

	MenuPartiemulti = 0;

	lanceMenuMultijoueurserveur( (void*)false );	// Sans demande de connection
}

void suivantPartiemulti(void *arg)
{
	JktNet::CServer *server = Game.getServer();
	pFocus->SetPlayFocus();
	Aide = false;
	server->nomMAP = (char*)arg;		// Informe le serveur sur le nom de la MAP lancée
	openMap( server->nomMAP );			// Ouvre cette MAP
	Game.setPlayerList( server->maxPlayers );
	server->setStatut( JktNet::JKT_STATUT_SERVER_PLAY );
	server->bGame = true;				// Indique qu'une partie est en cours
}

void lanceMenuPartiemulti(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuPartiemulti(var=%x)", var );
	int nbrFichier = 0;		// Nombre de fichiers ASE à prendre en compte

	PF *liste_suivant_partiemulti;
	char **item_menu_partiemulti;
	void **liste_argument_partiemulti;

	string name;

	CFindFolder folder( "./Map/", 0, ".map.xml" );
	nbrFichier = folder.nbr();

//	hFile = _findfirst( "./MAP/*.MAP", &fileinfo );
//	if( hFile!=-1L )
//	{
//		nbrFichier = 1;
//		while( _findnext( hFile, &fileinfo )!=-1L )
//			nbrFichier++;
//	}
//
//	_findclose( hFile );

	liste_suivant_partiemulti = new PF[ nbrFichier ];
	liste_argument_partiemulti = new void*[ nbrFichier ];
	item_menu_partiemulti = new char*[ nbrFichier ];

	nbrFichier = 0;
	folder.reset();

	while( folder.findNext(name) )
	{
		name.erase( name.find_last_of( "." ) );
		liste_suivant_partiemulti[nbrFichier] = suivantPartiemulti;

		item_menu_partiemulti[nbrFichier] = new char[name.size()+1];
		liste_argument_partiemulti[nbrFichier] = new char[name.size()+1];
		strcpy( item_menu_partiemulti[nbrFichier], name.c_str() );
		strcpy( (char*)(liste_argument_partiemulti[nbrFichier]), name.c_str() );
		nbrFichier++;
	}

//	hFile = _findfirst( "./MAP/*.MAP", &fileinfo );
//	if( hFile!=-1L )
//	{
//		name = fileinfo.name;
//		name.erase( name.find_last_of( "." ) );
//		liste_suivant_partiemulti[nbrFichier] = suivantPartiemulti;
//
//		item_menu_partiemulti[nbrFichier] = new char[name.size()+1];
//		liste_argument_partiemulti[nbrFichier] = new char[name.size()+1];
//		strcpy( item_menu_partiemulti[nbrFichier], name.c_str() );
//		strcpy( (char*)(liste_argument_partiemulti[nbrFichier]), name.c_str() );
//		nbrFichier++;
//		while( _findnext( hFile, &fileinfo )!=-1L )
//		{
//			name = fileinfo.name;
//			name.erase( name.find_last_of( "." ) );
//
//			liste_suivant_partiemulti[nbrFichier] = suivantPartiemulti;
//
//			item_menu_partiemulti[nbrFichier] = new char[name.size()+1];
//			liste_argument_partiemulti[nbrFichier] = new char[name.size()+1];
//			strcpy( item_menu_partiemulti[nbrFichier], name.c_str() );
//			strcpy( (char*)liste_argument_partiemulti[nbrFichier], name.c_str() );
//			nbrFichier++;
//		}
//	}

	MenuPartiemulti = new CMenu( "LANCER UNE PARTIE SERVEUR", (const char**)item_menu_partiemulti, nbrFichier,
						liste_suivant_partiemulti, retourPartiemulti, liste_argument_partiemulti );

	for( int i=0 ; i< nbrFichier ; i++ )
	{
		if( item_menu_partiemulti[ i ] )
			delete item_menu_partiemulti[ i ];
	}

	delete[] liste_suivant_partiemulti;
	delete[] liste_argument_partiemulti;
	delete[] item_menu_partiemulti;

	CDlg::SetMenuActif( MenuPartiemulti );
}

}	// JktMenu
