

#include <io.h>
#include <string>
#include <iostream>

using namespace std;

#include "sdl.h"
#include "sdl_net.h"

#include "Cfg.h"
#include "TableauIndex.h"
#include "Tableau.cpp"
#include "reseau/SPA.h"
#include "Player.h"
#include "reseau/Reseau.h"
#include "Game.h"
#include "enumReseau.h"

#include "Portail.h"

extern CGame Game;
extern CReseau Reseau;
extern CCfg Config;
extern char **remarquesDriver;

void quit_tutorial( int code );

void lanceMenuConvertASE(void *);
void lanceMenuOpenMAP(void *);

	/****************/
	/*				*/
	/*   MENU AIDE	*/
	/*				*/
	/****************/

	// Menu d'aide principal
char title_menu_aide[] = "LISTE DES TOUCHES";

PF liste_suivant_aide[] =
{
	lanceMenuAideEssai,
	lanceMenuAideEssai,
	lanceMenuAideEssai,
	NULL,
};


char *item_menu_aide[] =
{
	"G DESACTIVE OU ACTIVE LA GRAVITE",
	"C CHANGE DE JOUEUR ACTIF",
	"O REVIENT A L ORIGINE",
	"P AFFICHE LES COORDONNEES",
};

CMenu MenuAide( title_menu_aide, item_menu_aide, 4, liste_suivant_aide, NULL );

	// Sous menu d'aide
char title_menu_Aide_Essai[] = "ESSAI";

void fonctionAideEssai1(void *var)
{
	cout << "FONCTION 1\n";
}

void fonctionAideEssai2(void *var)
{
	cout << "FONCTION 2\n";
}

void fonctionAideEssai3(void *var)
{
	cout << "FONCTION 3\n";
}

PF liste_suivant_Aide_Essai[] =
{
	fonctionAideEssai1,
	fonctionAideEssai2,
	fonctionAideEssai3,
};

char *item_menu_Aide_Essai[] =
{
	"CHOIX 1",
	"CHOIX 2",
	"CHOIX 3",
};

CMenu MenuAideEssai(title_menu_Aide_Essai, item_menu_Aide_Essai,
					3, liste_suivant_Aide_Essai, lanceMenuAide );

	/****************/
	/*				*/
	/*   MENU MODE	*/
	/*				*/
	/****************/

	// Menu mode principal
char title_menu_mode[] = "MODES";

PF liste_suivant_mode[] =
{
	lanceMenuModeSelection,
};


char *item_menu_mode[] =
{
	"MODE SELECTION",
};

CMenu MenuMode(title_menu_mode, item_menu_mode, 1, liste_suivant_mode, NULL );

	// Sous Menu mode
char title_menu_Mode_Selection[] = "MODE SELECTION";

void fonctionModeSelection1(void *var)
{
	cout << "Solidité 1\n";
}

void fonctionModeSelection2(void *var)
{
	cout << "Suppression 2\n";
}

PF liste_suivant_Mode_Selection[] =
{
	fonctionModeSelection1,
	fonctionModeSelection2,
};

char *item_menu_Mode_Selection[] =
{
	"SOLIDITE",
	"SUPPRESSION",
};

CMenu MenuModeSelection(title_menu_Mode_Selection, item_menu_Mode_Selection, 2,
						liste_suivant_Mode_Selection, lanceMenuMode );

	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

//prototypes
void quit_JKT();
void quit_JKTici(void *var);
void lanceMenuConfig(void *var);		// Menu de configuration vidéo, son, réseau, clavier,...
void lanceMenuOpenScene(void *var);		// Menu d'ouverture fichiers ASE et MAP
void lanceMenuMultijoueur(void *var);	// Menu pour le jeu multi-joueurs
void lanceMenuTest2(void *var);
void lanceMenuTest(void *var);

CBouton bouton1( "Menu", lanceMenuPrinc );
CBouton bouton2( "Test", lanceMenuTest );
CBouton bouton3( "Test 2", lanceMenuTest2 );

CDlgBoite BoiteTest( "Test 1", "Coucou !!!", lanceMenuTest2, JKT_DLG_ERREUR );
CDlgBoite BoiteTest2( "Test 2", "Coucou ???", lanceMenuPrinc, bouton1, bouton2, bouton3, JKT_DLG_CONFIRM );

void lanceMenuTest2(void *var)
{
	CDlg::SetMenuActif( &BoiteTest2 );
}

void lanceMenuTest(void *var)
{
	BoiteTest.addBouton( 1, "Salut", lanceMenuPrinc );
	BoiteTest.addBouton( 3, "Coucou", lanceMenuPrinc );
	CDlg::SetMenuActif( &BoiteTest );
}

void quit_JKTici(void *var)
{	quit_tutorial( 0 );	}

char title_menu_princ[] = "MENU PRINCIPAL";

PF liste_suivant_princ[] =
{
	lanceMenuOpenScene,
	lanceMenuMultijoueur,
	lanceMenuConfig,
	lanceMenuTest,
	quit_JKTici,
};

char *item_menu_princ[] =
{
	"Ouvrir une scene",
	"Multi-joueurs",
	"Configuration",
	"TEST",
	"Quitter",
};

CMenu MenuPrinc( title_menu_princ, item_menu_princ, 5, liste_suivant_princ, NULL );

	/****************************/
	/*							*/
	/*   MENU PRINCIPAL 2		*/
	/*							*/
	/****************************/
void quit_PartieEnCours(void *var)
{
	Game.setModeNull();

	lanceMenuPrinc( 0 );
}

PF liste_suivant_princ2[] =
{
	lanceMenuOpenScene,
	lanceMenuMultijoueur,
	lanceMenuConfig,
	quit_PartieEnCours,
	quit_JKTici,
};

char *item_menu_princ2[] =
{
	"Ouvrir une scene",
	"Multi-joueurs",
	"Configuration",
	"Quitter la partie",
	"Quitter",
};

CMenu MenuPrinc2( title_menu_princ, item_menu_princ2, 5, liste_suivant_princ2, NULL );

	/************************/
	/*						*/
	/*   MENU OPEN SCENE	*/
	/*						*/
	/************************/

char title_menu_open_scene[] = "OUVRIR UNE SCENE";

PF liste_suivant_open_scene[] =
{
	lanceMenuConvertASE,
	lanceMenuOpenMAP,
};

char *item_menu_open_scene[] =
{
	"Convertir une scene ASE",
	"Ouvrir une scene MAP",
};

CMenu MenuOpenScene(title_menu_open_scene, item_menu_open_scene, 2,
						liste_suivant_open_scene, lanceMenuPrinc );

	/************************************/
	/*									*/
	/*   MENU MULTI-JOUEURS CLIENT		*/
	/*									*/
	/************************************/

void lanceInfoserver( void *arg );		// Envoie une requête d'info du serveur
void lancePingserver( void *arg );		// Fait un ping sur le serveur
void lanceJoindrePartie( void *arg );	// Se connecte sur la partie active sur le serveur

char title_menu_multijoueurclient[] = "CLIENT MULTI-JOUEURS";

void retourMultijoueurclient( void *arg )
{
	Reseau.fermeClient();
	lanceMenuPrinc( NULL );
}

PF liste_suivant_multijoueurclient[] =
{
	lanceInfoserver,		// Envoie une requête d'info du serveur
	lanceJoindrePartie,		// Se connecte sur la partie active sur le serveur
	lancePingserver,
};

char *item_menu_multijoueurclient[] =
{
	"Info serveur",
	"Joindre partie",
	"Ping serveur",
};

CMenu MenuMultijoueurclient( title_menu_multijoueurclient, item_menu_multijoueurclient, 3,
						liste_suivant_multijoueurclient, retourMultijoueurclient );

void lanceMenuMultijoueurclientconnect(void *var)
{
	if( !Reseau.ouvreClient() )
	{
		cerr << endl << "Echec de connexion au serveur";
		Reseau.On = false;			// Signale que le réseau ne peut pas être utilisé
		lanceMenuPrinc( NULL );		// Retour au menu principal
	}
	else
	{
		CDlg::SetMenuActif( &MenuMultijoueurclient );
	}
}

void lanceMenuMultijoueurclient(void *arg)
{	CDlg::SetMenuActif( &MenuMultijoueurclient);	}

void lanceJoindrePartie( void *arg)
{	Reseau.sendJoinTheGame();		}

	/************************************/
	/*									*/
	/*   MENU REQUETE INFO SERVEUR		*/
	/*									*/
	/************************************/

char title_menu_infoserver[] = "INFOS SERVEUR";
void refreshInfoserver();

PF liste_suivant_infoserver[] =
{
	NULL,
	NULL,
};

char *item_menu_infoserver[] =
{
	"Nom serveur",
	"Map active",
};

void retourInfoserver( void *arg )
{
	lanceMenuMultijoueurclient( NULL );
}

CMenu MenuInfoserver(title_menu_infoserver, item_menu_infoserver, 2,
						liste_suivant_infoserver, retourInfoserver, NULL, refreshInfoserver );

void refreshInfoserver()
{
	Uint32 ping = Reseau.getPingClientServer();
	if( Reseau.infoServer.ready==true )
	{
		MenuInfoserver.add_ItemsDroits( 0, Reseau.infoServer.nom.c_str() );
		MenuInfoserver.add_ItemsDroits( 1, Reseau.infoServer.map.c_str() );

		Reseau.infoServer.ready = false;
	}
}

void lanceInfoserver(void *arg)
{
	MenuInfoserver.add_ItemsDroits( 0, "????" );
	MenuInfoserver.add_ItemsDroits( 1, "????" );
	Reseau.sendRequestInfoServer();		// Envoie ses infos au serveur

	CDlg::SetMenuActif( &MenuInfoserver );
}

	/************************************/
	/*									*/
	/*   MENU REQUETE PING SERVEUR		*/
	/*									*/
	/************************************/

char title_menu_pingserver[] = "PING SERVEUR";
Uint32 timePing;
void actuPing( void *arg );
void refreshPingserver();

Uint32 timeLaunchPing = 0;

PF liste_suivant_pingserver[] =
{
	actuPing,
};

char *item_menu_pingserver[] =
{
	"Actualiser",
};

void retourPingserver( void *arg)
{
	CDlg::SetMenuActif( 0 );
	lanceMenuMultijoueurclient( 0 );
}

CMenu MenuPingserver(title_menu_pingserver, item_menu_pingserver, 1,
						liste_suivant_pingserver, retourPingserver, 0, refreshPingserver );

void refreshPingserver()
{
	Uint32 ping = Reseau.getPingClientServer();
	if( ping!=-1 )
	{
		if( ping >9999 )
			ping = 9999;
		char cou[8];
		string str;
		_itoa( ping, cou, 10 );				// Convertit le temps en string
		str += cou;
		str += " ms";

		MenuPingserver.add_ItemsDroits( 0, str.c_str() );
	}
}

void lancePingserver(void *arg)
{
	Reseau.sendPingClientServer();		// Envoie un ping au serveur
	MenuPingserver.add_ItemsDroits( 0, "????" );

	CDlg::SetMenuActif( &MenuPingserver );
}

void actuPing( void *arg )	// Réactualise la valeur du ping
{
	MenuPingserver.add_ItemsDroits( 0, "????" );
	Reseau.sendPingClientServer();		// Envoie un ping au serveur
}

	/************************************/
	/*									*/
	/*   MENU MULTI-JOUEURS SERVEUR		*/
	/*									*/
	/************************************/

void lanceMenuPartiemulti( void *arg );

char title_menu_multijoueurserveur[] = "SERVEUR MULTI-JOUEURS";

PF liste_suivant_multijoueurserveur[] =
{
	lanceMenuPartiemulti,
};

char *item_menu_multijoueurserveur[] =
{
	"Lancer une partie",
};

void retourMultiserveur( void *arg )
{
	Reseau.fermeServer();
	lanceMenuPrinc( NULL );
}

CMenu MenuMultijoueurserveur(title_menu_multijoueurserveur, item_menu_multijoueurserveur, 1,
						liste_suivant_multijoueurserveur, retourMultiserveur );

void lanceMenuMultijoueurserveur(void *var)
{
	if( var )		// S'il y a demande de connection réseau
	{
		if( !Reseau.ouvreServer( PORT_SERVER ) )
		{
			cout << endl << "Echec ouverture server";
			Reseau.On = false;		// Signale que le réseau ne peut pas être utilisé
			lanceMenuPrinc( NULL );
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

	/****************************************************/
	/*													*/
	/*   MENU LANCE UNE PARTIE MULTIJOUEUR SERVEUR		*/
	/*													*/
	/****************************************************/
	// Prototypes
void openMAP(void *arg);
void openMAP2( string &nomFichierMap );

CMenu *MenuPartiemulti;

char title_menu_partiemulti[] = "LANCER UNE PARTIE";

void retourPartiemulti( void *arg )
{
	CDlg::SetMenuActif( 0 );
	delete MenuPartiemulti;

	lanceMenuMultijoueurserveur( (void*)false );	// Sans demande de connection
}

void suivantPartiemulti(void *arg)
{
	CServer *server = Game.getServer();
	pFocus->SetPlayFocus();
	Aide = false;
	server->nomMAP = (char*)arg;		// Informe le serveur sur le nom de la MAP lancée
	server->bGame = true;				// Indique qu'une partie est en cours
	openMAP2( server->nomMAP );			// Ouvre cette MAP
	Game.pTabIndexPlayer = new CTableauIndex<CPlayer>( server->maxPlayers, true );
	Game.setStatutServer( JKT_STATUT_SERVER_PLAY );
}

void lanceMenuPartiemulti(void *var)
{

	struct _finddata_t fileinfo;
	long hFile;
	int nbrFichier = 0;		// Nombre de fichiers ASE à prendre en compte

	PF *liste_suivant_partiemulti;
	char **item_menu_partiemulti;
	void **liste_argument_partiemulti;

	string name;

	hFile = _findfirst( "./MAP/*.MAP", &fileinfo );
	if( hFile!=-1L )
	{
		nbrFichier = 1;
		while( _findnext( hFile, &fileinfo )!=-1L )
			nbrFichier++;
	}

	_findclose( hFile );

	liste_suivant_partiemulti = (PF*)malloc(nbrFichier*sizeof(PF));
	liste_argument_partiemulti = (void**)malloc(nbrFichier*sizeof(char*));
	item_menu_partiemulti = (char**)malloc(nbrFichier*sizeof(char*));

	nbrFichier = 0;

	hFile = _findfirst( "./MAP/*.MAP", &fileinfo );
	if( hFile!=-1L )
	{
		name = fileinfo.name;
		name.erase( name.find_last_of( "." ) );
		liste_suivant_partiemulti[nbrFichier] = suivantPartiemulti;

		item_menu_partiemulti[nbrFichier] = new char[strlen(name.c_str())+1];
		liste_argument_partiemulti[nbrFichier] = new char[strlen(name.c_str())+1];
		strcpy( item_menu_partiemulti[nbrFichier], name.c_str() );
		strcpy( (char*)(liste_argument_partiemulti[nbrFichier]), name.c_str() );
		nbrFichier++;
		while( _findnext( hFile, &fileinfo )!=-1L )
		{
			name = fileinfo.name;
			name.erase( name.find_last_of( "." ) );

			liste_suivant_partiemulti[nbrFichier] = suivantPartiemulti;

			item_menu_partiemulti[nbrFichier] = new char[strlen(name.c_str())+1];
			liste_argument_partiemulti[nbrFichier] = new char[strlen(name.c_str())+1];
			strcpy( item_menu_partiemulti[nbrFichier], name.c_str() );
			strcpy( (char*)liste_argument_partiemulti[nbrFichier], name.c_str() );
			nbrFichier++;
		}
	}

	MenuPartiemulti = new CMenu(title_menu_partiemulti, item_menu_partiemulti, nbrFichier,
						liste_suivant_partiemulti, retourPartiemulti, liste_argument_partiemulti );

	CDlg::SetMenuActif( MenuPartiemulti );
}

	/************************/
	/*						*/
	/*   MENU OPEN MAP		*/
	/*						*/
	/************************/

char title_menu_open_MAP[] = "OUVRIR UNE SCENE MAP";
void retourMAP(void *var);

CMenu *MenuOpenMAP;

void suivantMAP(void *arg)
{
	pFocus->SetPlayFocus();
	Aide = false;
	openMAP( arg );
	Game.setModeLocal();
}

void lanceMenuOpenMAP(void *var)
{
	struct _finddata_t fileinfo;
	long hFile;
	int nbrFichier = 0;		// Nombre de fichiers ASE à prendre en compte

	PF *liste_suivant_open_MAP;
	char **item_menu_open_MAP;
	void **liste_argument_open_MAP;

	string name;

	hFile = _findfirst( "./MAP/*.MAP", &fileinfo );
	if( hFile!=-1L )
	{
		nbrFichier = 1;
		while( _findnext( hFile, &fileinfo )!=-1L )
			nbrFichier++;
	}

	_findclose( hFile );

	liste_suivant_open_MAP = (PF*)malloc(nbrFichier*sizeof(PF));
	liste_argument_open_MAP = (void**)malloc(nbrFichier*sizeof(char*));
	item_menu_open_MAP = (char**)malloc(nbrFichier*sizeof(char*));

	nbrFichier = 0;

	hFile = _findfirst( "./MAP/*.MAP", &fileinfo );
	if( hFile!=-1L )
	{
		name = fileinfo.name;
		name.erase( name.find_last_of( "." ) );
		liste_suivant_open_MAP[nbrFichier] = suivantMAP;

		item_menu_open_MAP[nbrFichier] = new char[strlen(name.c_str())+1];
		liste_argument_open_MAP[nbrFichier] = new char[strlen(name.c_str())+1];
		strcpy( item_menu_open_MAP[nbrFichier], name.c_str() );
		strcpy( (char*)(liste_argument_open_MAP[nbrFichier]), name.c_str() );
		nbrFichier++;
		while( _findnext( hFile, &fileinfo )!=-1L )
		{
			name = fileinfo.name;
			name.erase( name.find_last_of( "." ) );

			liste_suivant_open_MAP[nbrFichier] = suivantMAP;

			item_menu_open_MAP[nbrFichier] = new char[strlen(name.c_str())+1];
			liste_argument_open_MAP[nbrFichier] = new char[strlen(name.c_str())+1];
			strcpy( item_menu_open_MAP[nbrFichier], name.c_str() );
			strcpy( (char*)liste_argument_open_MAP[nbrFichier], name.c_str() );
			nbrFichier++;
		}
	}

	MenuOpenMAP = new CMenu(title_menu_open_MAP, item_menu_open_MAP, nbrFichier,
						liste_suivant_open_MAP, retourMAP, liste_argument_open_MAP );

	CDlg::SetMenuActif( MenuOpenMAP );
}

void retourMAP(void *var)	// Libérations mémoire et retour au menu supérieur
{
	delete MenuOpenMAP;
	CDlg::SetMenuActif( 0 );

	lanceMenuOpenScene( 0 );
}

	/************************/
	/*						*/
	/*   MENU OPEN ASE		*/
	/*						*/
	/************************/

	// Prototypes
void lanceMenuConvertASEConfirmOK();
void lanceMenuConvertASEConfirmNO( int msg );
void lanceBoiteConvertASE( void *var );

CMenu *MenuOpenASE;

char title_menu_open_ASE[] = "Convertir une Scene ASE";

void retourASE(void *var)	// Libérations mémoire et retour au menu supérieur
{
	delete MenuOpenASE;
	CDlg::SetMenuActif( 0 );

	lanceMenuOpenScene( 0 );
}

/*void suivantASE(void *arg)
{
	int msg = convertASE( arg );
	switch( msg )
	{
	case 0:		// La conversion a réussi
		lanceMenuConvertASEConfirmOK();
		break;
	default:	// La conversion a échoué
		lanceMenuConvertASEConfirmNO( msg );
		break;
	}
}*/

void suivantASE(void *arg)
{
	Game.RequeteProcess.demandeConvertASE( (char*)arg );
	lanceBoiteConvertASE( 0 );
}

void lanceMenuConvertASE(void *var)
{
	struct _finddata_t fileinfo;
	long hFile;
	int nbrFichier = 0;		// Nombre de fichiers ASE à prendre en compte

	PF *liste_suivant_open_ASE;
	char **item_menu_open_ASE;
	void **liste_argument_open_ASE;

	string name;

	hFile = _findfirst( "./ASE/*.ASE", &fileinfo );
	if( hFile!=-1L )
	{
		nbrFichier = 1;
		while( _findnext( hFile, &fileinfo )!=-1L )
			nbrFichier++;
	}

	_findclose( hFile );

	liste_suivant_open_ASE = (PF*)malloc(nbrFichier*sizeof(PF));
	liste_argument_open_ASE = (void**)malloc(nbrFichier*sizeof(char*));
	item_menu_open_ASE = (char**)malloc(nbrFichier*sizeof(char*));

	nbrFichier = 0;

	hFile = _findfirst( "./ASE/*.ASE", &fileinfo );
	if( hFile!=-1L )
	{
		name = fileinfo.name;
		name.erase( name.find_last_of( "." ) );
		liste_suivant_open_ASE[nbrFichier] = suivantASE;

		item_menu_open_ASE[nbrFichier] = new char[strlen(name.c_str())+1];
		liste_argument_open_ASE[nbrFichier] = new char[strlen(name.c_str())+1];
		strcpy( item_menu_open_ASE[nbrFichier], name.c_str() );
		strcpy( (char*)(liste_argument_open_ASE[nbrFichier]), name.c_str() );
		nbrFichier++;
		while( _findnext( hFile, &fileinfo )!=-1L )
		{
			name = fileinfo.name;
			name.erase( name.find_last_of( "." ) );

			liste_suivant_open_ASE[nbrFichier] = suivantASE;

			item_menu_open_ASE[nbrFichier] = new char[strlen(name.c_str())+1];
			liste_argument_open_ASE[nbrFichier] = new char[strlen(name.c_str())+1];
			strcpy( item_menu_open_ASE[nbrFichier], name.c_str() );
			strcpy( (char*)liste_argument_open_ASE[nbrFichier], name.c_str() );
			nbrFichier++;
		}
	}

	MenuOpenASE = new CMenu(title_menu_open_ASE, item_menu_open_ASE, nbrFichier,
						liste_suivant_open_ASE, retourASE, liste_argument_open_ASE );
	CDlg::SetMenuActif( MenuOpenASE );
}

CDlgBoite BoiteConvertASE( "Info", "Ouverture du fichier ASE en cours...", 0, JKT_DLG_INFO );

void lanceBoiteConvertASE( void *var )
{
	CDlg::SetMenuActif( &BoiteConvertASE );
}

	/********************************************/
	/*											*/
	/*   CONFIRM CONVERSION FICHIER ASE	REUSSIE	*/
	/*											*/
	/********************************************/

char title_menu_convert_ASE_confirmOK[] = "CONVERSION REUSSIE";

PF liste_suivant_convert_ASE_confirmOK[] =
{
	lanceMenuOpenScene,
};

char *item_menu_convert_ASE_confirmOK[] =
{
	"",
};

CDlgBoite BoiteConfirmOK( "Confirm", "La conversion du fichier ASE a reussi", lanceMenuOpenScene );

//CMenu MenuConvertASEConfirmOK(title_menu_convert_ASE_confirmOK, item_menu_convert_ASE_confirmOK, 1,
//							liste_suivant_convert_ASE_confirmOK, lanceMenuOpenScene );


void lanceMenuConvertASEConfirmOK()
{
	CDlg::SetMenuActif( &BoiteConfirmOK );
}

	/********************************************/
	/*											*/
	/*   CONFIRM CONVERSION FICHIER ASE	RATEE	*/
	/*											*/
	/********************************************/
void retour_confirm_ASE_NO(void *var);

CMenu *menu;

char title_menu_convert_ASE_confirmNO[] = "CONVERSION RATEE";

PF liste_suivant_convert_ASE_confirmNO[] =
{
	lanceMenuOpenScene,
};

void retour_confirm_ASE_NO(void *var)
{
	delete menu;
	lanceMenuOpenScene( NULL );
}

void lanceMenuConvertASEConfirmNO( int msg )
{
	char erreur_invalide[] = "Fichier ASE invalide";
	char erreur_creationRep[] = "Creation de repertoire texture impossible";
	char erreur_copieTex[] = "Copie de fichier de texture impossible";

	char **item_menu_convert_ASE_confirmNO = new char*;

	switch( msg )
	{
	case 1:
		item_menu_convert_ASE_confirmNO[0] = new char[strlen( erreur_invalide )+1];
		strcpy( item_menu_convert_ASE_confirmNO[0], erreur_invalide );
		break;

	case 2:
		item_menu_convert_ASE_confirmNO[0] = new char[strlen( erreur_creationRep )+1];
		strcpy( item_menu_convert_ASE_confirmNO[0], erreur_creationRep );
		break;

	case 3:
		item_menu_convert_ASE_confirmNO[0] = new char[strlen( erreur_copieTex )+1];
		strcpy( item_menu_convert_ASE_confirmNO[0], erreur_copieTex );
		break;

	default:
		break;
	}


	menu = new CMenu(title_menu_convert_ASE_confirmNO, item_menu_convert_ASE_confirmNO,
		1, liste_suivant_convert_ASE_confirmNO, retour_confirm_ASE_NO );

	CDlg::SetMenuActif( menu );
}

	/************************/
	/*						*/
	/*   MENU CONFIGURATION	*/
	/*						*/
	/************************/
void lanceMenuConfigVideo(void *arg);
void lanceMenuConfigAudio(void *arg);
void lanceMenuConfigCommandes(void *arg);
void lanceMenuConfigReseau(void *arg);
void lanceMenuConfigDebug(void *arg);

char title_menu_config[] = "CONFIGURATION";

PF liste_suivant_config[] =
{
	lanceMenuConfigVideo,
	lanceMenuConfigAudio,
	lanceMenuConfigCommandes,
	lanceMenuConfigReseau,
	lanceMenuConfigDebug,
};

char *item_menu_config[] =
{
	"Video",
	"Audio",
	"Commandes",
	"Reseau",
	"Debug",
};

CMenu MenuConfig(title_menu_config, item_menu_config, 5,
						liste_suivant_config, lanceMenuPrinc );

void lanceMenuConfig(void *var)
{
	CDlg::SetMenuActif( &MenuConfig );
}

	/************************************/
	/*									*/
	/*   MENU CONFIGURATION DEBUG		*/
	/*									*/
	/************************************/
void actu_menu_config_debug_SonUsageCPU(void *arg);
void actu_menu_config_debug_AfficheFichier( void *arg);

char title_menu_config_debug[] = "DEBUG";

PF liste_suivant_config_debug[] =
{
	actu_menu_config_debug_SonUsageCPU,
	actu_menu_config_debug_AfficheFichier,

};

char *item_menu_config_debug[] =
{
	"Son, usage CPU",
	"Affiche ASE",
};

void retourMenuConfigDebug( void *arg)
{
	lanceMenuConfig( NULL );
}

CMenu MenuConfigDebug(title_menu_config_debug, item_menu_config_debug, 2,
						liste_suivant_config_debug, retourMenuConfigDebug );

void lanceMenuConfigDebug(void *arg)
{
	if( Config.Debug.bSonUsageCPU )
		MenuConfigDebug.add_ItemsDroits( 0, "Oui" );
	else
		MenuConfigDebug.add_ItemsDroits( 0, "Non" );

	if( Config.Debug.bAfficheFichier )
		MenuConfigDebug.add_ItemsDroits( 1, "Oui" );
	else
		MenuConfigDebug.add_ItemsDroits( 1, "Non" );

	CDlg::SetMenuActif( &MenuConfigDebug );
}

void actu_menu_config_debug_SonUsageCPU(void *arg)
{
	Config.Debug.bSonUsageCPU = !Config.Debug.bSonUsageCPU;
	Config.Ecrit();

	lanceMenuConfigDebug( NULL );
}

void actu_menu_config_debug_AfficheFichier(void *arg)
{
	Config.Debug.bAfficheFichier = !Config.Debug.bAfficheFichier;
	Config.Ecrit();

	lanceMenuConfigDebug( NULL );
}

	/********************************/
	/*								*/
	/*   MENU CONFIGURATION	RESEAU	*/
	/*								*/
	/********************************/

void lanceMenuConfigReseauServeur(void *arg);
void lanceMenuConfigReseauIp(void *arg);

CMenu *MenuConfigReseau;

char title_menu_config_reseau[] = "CONFIGURATION RESEAU";

void retourReseau(void *var)	// Libérations mémoire et retour au menu supérieur
{	// A VOIR y'a un problème parce retourReseau appelle lanceMenuConfig, alors que
	// lanceMenuConfigReseauServeur appelle retourReseau mais aussi lanceMenuConfigReseau

	CDlg::SetMenuActif( 0 );
	delete MenuConfigReseau;

	lanceMenuConfig(0);
}

void lanceMenuConfigReseau(void *var)
{
	PF *liste_suivant_config_reseau;
	char **item_menu_config_reseau;
	void **liste_argument_config_reseau;

	int nbr = 1;	// Nombre de lignes dans ce menu (minimum = 1)

	if( !Config.Reseau.serveur )	// S'il s'agit d'un client (à l'opposé d'un serveur)
		nbr ++;	// Ajoute une ligne pour l'adresse ip du serveur

	liste_suivant_config_reseau = (PF*)malloc( nbr*sizeof(PF) );
	liste_argument_config_reseau = (void**)malloc( nbr*sizeof(char*) );
	item_menu_config_reseau = (char**)malloc( nbr*sizeof(char*) );

	int i = 0;

	item_menu_config_reseau[i] = new char[strlen("Serveur/Client")+1];
	strcpy( item_menu_config_reseau[i], "Serveur/Client" );
	liste_suivant_config_reseau[i++] = lanceMenuConfigReseauServeur;
	if( !Config.Reseau.serveur )	// Si c'est un client
	{
		item_menu_config_reseau[i] = new char[strlen("IP")+1];
		strcpy( item_menu_config_reseau[i], "IP" );
		liste_suivant_config_reseau[i++] = lanceMenuConfigReseauIp;
	}

	MenuConfigReseau = new CMenu(title_menu_config_reseau, item_menu_config_reseau, nbr,
						liste_suivant_config_reseau, retourReseau );

	CDlg::SetMenuActif( MenuConfigReseau );

	if( Config.Reseau.serveur )
	{
		MenuConfigReseau->add_ItemsDroits( 0, "Serveur" );
	}
	else
	{
		char *ip = new char[ Config.Reseau.ip.length()+1 ];
		strcpy( ip, Config.Reseau.ip.c_str() );

		MenuConfigReseau->add_ItemsDroits( 0, "Client" );
		MenuConfigReseau->add_ItemsDroits( 1, ip );

		delete ip;
	}
}

void lanceMenuConfigReseauServeur(void *arg)
{
	retourReseau( NULL );
	Config.Reseau.serveur = !Config.Reseau.serveur;
	Config.Ecrit();
	lanceMenuConfigReseau( NULL );
}

void lanceMenuConfigReseauIp(void *arg)
{
	Config.Ecrit();
}

	/********************************/
	/*								*/
	/*   MENU CONFIGURATION	VIDEO	*/
	/*								*/
	/********************************/

void lanceMenuConfigVideoDisplay(void*);

char title_menu_config_video[] = "CONFIGURATION VIDEO";

PF liste_suivant_config_video[] =
{
	lanceMenuConfigVideoDisplay,

};

char *item_menu_config_video[] =
{
	"Display",
};

CMenu MenuConfigVideo(title_menu_config_video, item_menu_config_video, 1,
						liste_suivant_config_video, lanceMenuConfig );

void lanceMenuConfigVideo(void *var)
{
	CDlg::SetMenuActif( &MenuConfigVideo );
}

	/****************************************/
	/*										*/
	/*   MENU CONFIGURATION	VIDEO DISPLAY	*/
	/*										*/
	/****************************************/

void lanceDisplay640_468(void*);
void lanceDisplay640_480(void*);
void lanceDisplay800_600(void*);
void lanceDisplay1024_768(void*);
void lanceDisplay1152_864(void*);
void lanceDisplay1280_720(void*);
void lanceDisplay1280_768(void*);
void lanceDisplay1280_960(void*);
void lanceDisplay1280_1024(void*);
void lanceDisplay1600_900(void*);
void lanceDisplay1600_1024(void*);
void lanceDisplay1600_1200(void*);

char title_menu_config_video_display[] = "DISPLAY";

PF liste_suivant_config_video_display[] =
{
	lanceDisplay640_468,
	lanceDisplay640_480,
	lanceDisplay800_600,
	lanceDisplay1024_768,
	lanceDisplay1152_864,
	lanceDisplay1280_720,
	lanceDisplay1280_768,
	lanceDisplay1280_960,
	lanceDisplay1280_1024,
	lanceDisplay1600_900,
	lanceDisplay1600_1024,
	lanceDisplay1600_1200,
};

char *item_menu_config_video_display[] =
{
	"640 * 468",
	"640 * 480",
	"800 * 600",
	"1024 * 768",
	"1152 * 864",
	"1280 * 720",
	"1280 * 768",
	"1280 * 960",
	"1280 * 1024",
	"1600 * 900",
	"1600 * 1024",
	"1600 * 1200"
};

CMenu MenuConfigVideoDisplay(title_menu_config_video_display, item_menu_config_video_display, 12,
						liste_suivant_config_video_display, lanceMenuConfigVideo );

void lanceMenuConfigVideoDisplay(void *var)
{
	CDlg::SetMenuActif( &MenuConfigVideoDisplay );
}

void lanceDisplay640_468(void*)
{
	Config.Display.X = 640;
	Config.Display.Y = 468;
	Config.Ecrit();
}

void lanceDisplay640_480(void*)
{
	Config.Display.X = 640;
	Config.Display.Y = 480;
	Config.Ecrit();
}

void lanceDisplay800_600(void*)
{
	Config.Display.X = 800;
	Config.Display.Y = 600;
	Config.Ecrit();
}

void lanceDisplay1024_768(void*)
{
	Config.Display.X = 1024;
	Config.Display.Y = 768;
	Config.Ecrit();
}

void lanceDisplay1152_864(void*)
{
	Config.Display.X = 1152;
	Config.Display.Y = 864;
	Config.Ecrit();
}

void lanceDisplay1280_720(void*)
{
	Config.Display.X = 1280;
	Config.Display.Y = 720;
	Config.Ecrit();
}

void lanceDisplay1280_768(void*)
{
	Config.Display.X = 1280;
	Config.Display.Y = 768;
	Config.Ecrit();
}

void lanceDisplay1280_960(void*)
{
	Config.Display.X = 1280;
	Config.Display.Y = 960;
	Config.Ecrit();
}

void lanceDisplay1280_1024(void*)
{
	Config.Display.X = 1280;
	Config.Display.Y = 1024;
	Config.Ecrit();
}

void lanceDisplay1600_900(void*)
{
	Config.Display.X = 1600;
	Config.Display.Y = 900;
	Config.Ecrit();
}

void lanceDisplay1600_1024(void*)
{
	Config.Display.X = 1600;
	Config.Display.Y = 1024;
	cout << "X " << Config.Display.X << "\tY " << Config.Display.Y << endl;
	Config.Ecrit();
}

void lanceDisplay1600_1200(void*)
{
	Config.Display.X = 1600;
	Config.Display.Y = 1200;
	Config.Ecrit();
}

	/************************************/
	/*									*/
	/*   MENU CONFIGURATION COMMANDES	*/
	/*									*/
	/************************************/

void lanceMenuConfigCommandesAvancer(void *arg);
void lanceMenuConfigCommandesReculer(void *arg);
void lanceMenuConfigCommandesGauche(void *arg);
void lanceMenuConfigCommandesDroite(void *arg);
void lanceMenuConfigCommandesTir1(void *arg);
void lanceMenuConfigCommandesTir2(void *arg);
void lanceMenuConfigCommandesMonter(void *arg);

char title_menu_config_commandes[] = "COMMANDES";

PF liste_suivant_config_commandes[] =
{
	lanceMenuConfigCommandesAvancer,
	lanceMenuConfigCommandesReculer,
	lanceMenuConfigCommandesGauche,
	lanceMenuConfigCommandesDroite,
	lanceMenuConfigCommandesTir1,
	lanceMenuConfigCommandesTir2,
	lanceMenuConfigCommandesMonter,
};

char *item_menu_config_commandes[] =
{
	"Avancer",
	"Reculer",
	"Gauche",
	"Droite",
	"Tir1",
	"Tir2",
	"Monter",
};

CMenu MenuConfigCommandes(title_menu_config_commandes, item_menu_config_commandes, 7,
						liste_suivant_config_commandes, lanceMenuConfig );

void lanceMenuConfigCommandes(void *var)
{
	MenuConfigCommandes.add_ItemsDroits( 0, Config.Commandes.resolve(Config.Commandes.Avancer) );
	MenuConfigCommandes.add_ItemsDroits( 1, Config.Commandes.resolve(Config.Commandes.Reculer) );
	MenuConfigCommandes.add_ItemsDroits( 2, Config.Commandes.resolve(Config.Commandes.Gauche) );
	MenuConfigCommandes.add_ItemsDroits( 3, Config.Commandes.resolve(Config.Commandes.Droite) );
	MenuConfigCommandes.add_ItemsDroits( 4, Config.Commandes.resolve(Config.Commandes.Tir1) );
	MenuConfigCommandes.add_ItemsDroits( 5, Config.Commandes.resolve(Config.Commandes.Tir2) );
	MenuConfigCommandes.add_ItemsDroits( 6, Config.Commandes.resolve(Config.Commandes.Monter) );
	CDlg::SetMenuActif( &MenuConfigCommandes );
}

void lanceMenuConfigCommandesAvancer(void *arg)
{
	if( arg==NULL )
	{
		MenuConfigCommandes.mode = 1;		// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
	SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE )
			{
				Config.Commandes.Avancer.key = event->key.keysym.sym;
				Config.Commandes.Avancer.mouse = NULL;
				Config.Ecrit();	// Sauvegarde la configuration
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Avancer.mouse = event->button.button;
			Config.Commandes.Avancer.key = (SDLKey)NULL;
			Config.Ecrit();		// Sauvegarde la configuration
			break;

		default:
			break;
		}


		MenuConfigCommandes.bItemsDroits = false;
		MenuConfigCommandes.mode = 0;
		lanceMenuConfigCommandes(NULL);
	}
}

void lanceMenuConfigCommandesReculer(void *arg)
{
	if( arg==NULL )
	{
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE )
			{
				Config.Commandes.Reculer.key = event->key.keysym.sym;
				Config.Commandes.Reculer.mouse = NULL;
				Config.Ecrit();	// Sauvegarde la configuration
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Reculer.mouse = event->button.button;
			Config.Commandes.Reculer.key = (SDLKey)NULL;
			Config.Ecrit();		// Sauvegarde la configuration
			break;

		default:
			break;
		}

		MenuConfigCommandes.bItemsDroits = false;
		MenuConfigCommandes.mode = 0;
		lanceMenuConfigCommandes(NULL);
	}
}

void lanceMenuConfigCommandesGauche(void *arg)
{
	if( arg==NULL )
	{
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE )
			{
				Config.Commandes.Gauche.key = event->key.keysym.sym;
				Config.Commandes.Gauche.mouse = NULL;
				Config.Ecrit();	// Sauvegarde la configuration
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Gauche.mouse = event->button.button;
			Config.Commandes.Gauche.key = (SDLKey)NULL;
			Config.Ecrit();		// Sauvegarde la configuration
			break;

		default:
			break;
		}

		MenuConfigCommandes.bItemsDroits = false;
		MenuConfigCommandes.mode = 0;
		lanceMenuConfigCommandes(NULL);
	}
}

void lanceMenuConfigCommandesDroite(void *arg)
{
	if( arg==NULL )
	{
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE )
			{
				Config.Commandes.Droite.key = event->key.keysym.sym;
				Config.Commandes.Droite.mouse = NULL;
				Config.Ecrit();	// Sauvegarde la configuration
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Droite.mouse = event->button.button;
			Config.Commandes.Droite.key = (SDLKey)NULL;
			Config.Ecrit();		// Sauvegarde la configuration
			break;

		default:
			break;
		}

		MenuConfigCommandes.bItemsDroits = false;
		MenuConfigCommandes.mode = 0;
		lanceMenuConfigCommandes(NULL);
	}
}

void lanceMenuConfigCommandesTir1(void *arg)
{
	if( arg==NULL )
	{
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE )
			{
				Config.Commandes.Tir1.key = event->key.keysym.sym;
				Config.Commandes.Tir1.mouse = NULL;
				Config.Ecrit();	// Sauvegarde la configuration
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Tir1.mouse = event->button.button;
			Config.Commandes.Tir1.key = (SDLKey)NULL;
			Config.Ecrit();		// Sauvegarde la configuration
			break;

		default:
			break;
		}

		MenuConfigCommandes.bItemsDroits = false;
		MenuConfigCommandes.mode = 0;
		lanceMenuConfigCommandes(NULL);
	}
}

void lanceMenuConfigCommandesTir2(void *arg)
{
	if( arg==NULL )
	{
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE )
			{
				Config.Commandes.Tir2.key = event->key.keysym.sym;
				Config.Commandes.Tir2.mouse = NULL;
				Config.Ecrit();	// Sauvegarde la configuration
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Tir2.mouse = event->button.button;
			Config.Commandes.Tir2.key = (SDLKey)NULL;
			Config.Ecrit();		// Sauvegarde la configuration
			break;

		default:
			break;
		}

		MenuConfigCommandes.bItemsDroits = false;
		MenuConfigCommandes.mode = 0;
		lanceMenuConfigCommandes(NULL);
	}
}

void lanceMenuConfigCommandesMonter(void *arg)
{
	if( arg==NULL )
	{
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE )
			{
				Config.Commandes.Monter.key = event->key.keysym.sym;
				Config.Commandes.Monter.mouse = NULL;
				Config.Ecrit();	// Sauvegarde la configuration
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Monter.mouse = event->button.button;
			Config.Commandes.Monter.key = (SDLKey)NULL;
			Config.Ecrit();		// Sauvegarde la configuration
			break;

		default:
			break;
		}

		MenuConfigCommandes.bItemsDroits = false;
		MenuConfigCommandes.mode = 0;
		lanceMenuConfigCommandes(NULL);
	}
}

	/********************************/
	/*								*/
	/*   MENU CONFIGURATION	AUDIO	*/
	/*								*/
	/********************************/

void lanceMenuConfigAudioDrivers(void*);

char title_menu_config_audio[] = "CONFIGURATION AUDIO";

PF liste_suivant_config_audio[] =
{
	lanceMenuConfigAudioDrivers,
};

char *item_menu_config_audio[] =
{
	"Drivers",
};

CMenu MenuConfigAudio(title_menu_config_audio, item_menu_config_audio, 1,
						liste_suivant_config_audio, lanceMenuConfig );

void lanceMenuConfigAudio(void *var)
{
	CDlg::SetMenuActif( &MenuConfigAudio );
}

	/****************************************/
	/*										*/
	/*   MENU CONFIGURATION AUDIO DRIVERS	*/
	/*										*/
	/****************************************/
#include "son/audio.h"
void lanceMenuConfigAudioDriversOutput(void *arg);
void lanceMenuConfigAudioDriversMixer(void *arg) {};
void lanceMenuConfigAudioDriversDriver(void *arg);
void lanceMenuConfigAudioDriversRecord(void *arg);

char title_menu_config_audio_drivers[] = "DRIVERS";

PF liste_suivant_config_audio_drivers[] =
{
	lanceMenuConfigAudioDriversOutput,
	lanceMenuConfigAudioDriversMixer,
	lanceMenuConfigAudioDriversDriver,
	lanceMenuConfigAudioDriversRecord,
};

char *item_menu_config_audio_drivers[] =
{
	"Output",
	"Mixer",
	"Driver",
	"Record",
};

CMenu MenuConfigAudioDrivers(title_menu_config_audio_drivers, item_menu_config_audio_drivers, 4,
						liste_suivant_config_audio_drivers, lanceMenuConfigAudio );

void lanceMenuConfigAudioDrivers(void *var)
{
	MenuConfigAudioDrivers.add_ItemsDroits( 0, resolveOutput(Config.Audio.Output) );
	MenuConfigAudioDrivers.add_ItemsDroits( 1, resolveMixer(Config.Audio.Mixer) );
	MenuConfigAudioDrivers.add_ItemsDroits( 2, (char*)resolveDriver(Config.Audio.Driver) );
	MenuConfigAudioDrivers.add_ItemsDroits( 3, (char*)resolveDriverRecord(Config.Audio.DriverRecord) );

	CDlg::SetMenuActif( &MenuConfigAudioDrivers );
}

	/************************************************/
	/*												*/
	/*   MENU CONFIGURATION AUDIO DRIVERS OUTPUT	*/
	/*												*/
	/************************************************/
#include "fmod.h"
void lanceOutput1(void *arg);
void lanceOutput2(void *arg);
void lanceOutput3(void *arg);
void lanceOutput4(void *arg);

char title_menu_config_audio_drivers_output[] = "DRIVERS OUTPUT";

PF liste_suivant_config_audio_drivers_output[] =
{
	lanceOutput1,
	lanceOutput2,
	lanceOutput3,
	lanceOutput4,
};

char *item_menu_config_audio_drivers_output[] =
{
#ifdef _WIN32
	"Direct Sound",
	"Windows Multimedia Waveout",
	"ASIO",
	"NoSound",
#elif defined(__linux__)
	"OSS - Open Sound System"),
    "ESD - Elightment Sound Daemon"),
    "ALSA 0.9 - Advanced Linux Sound Architecture"),
	"NoSound",
#endif
};

CMenu MenuConfigAudioDriversOutput(title_menu_config_audio_drivers_output,
								   item_menu_config_audio_drivers_output, 4,
									liste_suivant_config_audio_drivers_output,
									lanceMenuConfigAudioDrivers );

void lanceMenuConfigAudioDriversOutput(void *var)
{
	CDlg::SetMenuActif( &MenuConfigAudioDriversOutput );
}

void lanceOutput1(void *arg)
{
#ifdef _WIN32
	Config.Audio.Output = FSOUND_OUTPUT_DSOUND;
#elif defined(__linux__)
	Config.Audio.Output = FSOUND_OUTPUT_OSS;
#endif
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(NULL);
}

void lanceOutput2(void *arg)
{
#ifdef _WIN32
	Config.Audio.Output = FSOUND_OUTPUT_WINMM;
#elif defined(__linux__)
	Config.Audio.Output = FSOUND_OUTPUT_ESD;
#endif
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(NULL);
}

void lanceOutput3(void *arg)
{
#ifdef _WIN32
	Config.Audio.Output = FSOUND_OUTPUT_ASIO;
#elif defined(__linux__)
	Config.Audio.Output = FSOUND_OUTPUT_ALSA;
#endif
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(NULL);
}

void lanceOutput4(void *arg)
{
	Config.Audio.Output = FSOUND_OUTPUT_NOSOUND;
//	FSOUND_SetOutput( Config.Audio.Output );
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(NULL);
}

	/************************************************/
	/*												*/
	/*   MENU CONFIGURATION AUDIO DRIVERS DRIVER	*/
	/*												*/
	/************************************************/

char title_menu_config_audio_drivers_driver[] = "DRIVER";

CMenu *MenuDriver;

void retourDriver(void *var)	// Libérations mémoire et retour au menu supérieur
{
	CDlg::SetMenuActif( 0 );
	delete MenuDriver;

	lanceMenuConfigAudioDrivers( 0 );
}

void lanceDriver(void *arg)
{
//	FSOUND_SetDriver( (int)arg );
	Config.Audio.Driver = *(int*)arg;
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(NULL);
}

void lanceMenuConfigAudioDriversDriver(void *var)
{
	int nbrDriver = FSOUND_GetNumDrivers();	// Nombre driver possibles

	PF *liste_suivant;
	char **item_menu;
	void **liste_argument;

	liste_suivant = (PF*)malloc(nbrDriver*sizeof(PF));
	liste_argument = (void**)malloc(nbrDriver*sizeof(char*));
	item_menu = (char**)malloc(nbrDriver*sizeof(char*));

	for( int i=0 ; i<nbrDriver ; i++ )
	{
		liste_suivant[ i ] = lanceDriver;

		item_menu[ i ] = new char[strlen((char*)FSOUND_GetDriverName(i))+1];
		liste_argument[ i ] = (void*)new int;
		*((int*)liste_argument[ i ]) = i;
		strcpy( item_menu[ i ], (char*)FSOUND_GetDriverName(i) );
	}

	MenuDriver = new CMenu(title_menu_config_audio_drivers_driver,
						item_menu, nbrDriver,
						liste_suivant, retourDriver, liste_argument );

	for( int i=0 ; i<nbrDriver ; i++ )
	{
		MenuDriver->add_ItemsRem( i, remarquesDriver[i] );
	}

	CDlg::SetMenuActif( MenuDriver );
}

	/************************************************/
	/*												*/
	/*   MENU CONFIGURATION AUDIO DRIVERS RECORD	*/
	/*												*/
	/************************************************/

char title_menu_config_audio_drivers_record[] = "RECORD";

CMenu *MenuRecord;

void retourRecord(void *var)	// Libérations mémoire et retour au menu supérieur
{
	CDlg::SetMenuActif( 0 );
	delete MenuRecord;

	lanceMenuConfigAudioDrivers( 0 );
}

void lanceRecord(void *arg)
{
//	FSOUND_Record_SetDriver( (int)arg );
	Config.Audio.DriverRecord = *(int*)arg;
	Config.Ecrit();
	lanceMenuConfigAudioDrivers(NULL);
}

void lanceMenuConfigAudioDriversRecord(void *var)
{
	int nbrRecord = FSOUND_Record_GetNumDrivers();	// Nombre driver possibles

	PF *liste_suivant;
	char **item_menu;
	void **liste_argument;

	liste_suivant = (PF*)malloc(nbrRecord*sizeof(PF));
	liste_argument = (void**)malloc(nbrRecord*sizeof(char*));
	item_menu = (char**)malloc(nbrRecord*sizeof(char*));

	for( int i=0 ; i<nbrRecord ; i++ )
	{
		liste_suivant[ i ] = lanceRecord;

		item_menu[ i ] = new char[strlen((char*)FSOUND_Record_GetDriverName(i))+1];
		liste_argument[ i ] = (void*)new int;
		*((int*)liste_argument[ i ]) = i;
		strcpy( item_menu[ i ], (char*)FSOUND_Record_GetDriverName(i) );
	}

	MenuRecord = new CMenu(title_menu_config_audio_drivers_record,
						item_menu, nbrRecord,
						liste_suivant, retourRecord, liste_argument );
	CDlg::SetMenuActif( MenuRecord );
}
