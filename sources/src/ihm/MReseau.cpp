	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <sstream>
#include <string>
#include <iostream>
using namespace std;

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"
#include "util/Trace.h"
#include "main/Focus.h"
#include "main/Cfg.h"

#include "ihm/MReseau.h"
#include "ihm/MConfiguration.h"

using namespace JktMenu;

class CGame;

extern CGame Game;
extern CMenu MenuPrinc2;
extern CFocus *pFocus;
extern bool Aide;
extern CCfg Config;

namespace JktMenu
{

	/********************************/
	/*								*/
	/*   MENU CONFIGURATION	RESEAU	*/
	/*								*/
	/********************************/
void lanceMenuConfigReseauSwitch(void *arg);
void lanceMenuConfigReseauIp(void *arg);
void lanceMenuConfigReseauPort(void *arg);

CMenu *MenuConfigReseau = 0;

string stringPort;
int nbrMenuPort;		// Position du numéro de port dans le menu

char title_menu_config_reseau[] = "CONFIGURATION RESEAU";

void retourReseau(void *var)	// Libérations mémoire et retour au menu supérieur
{	// A VOIR y'a un problème parce retourReseau appelle lanceMenuConfig, alors que
	// lanceMenuConfigReseauSwitch appelle retourReseau mais aussi lanceMenuConfigReseau
TRACE().p( TRACE_MENU, "retourReseau(var=%x)", var );
	CDlg::SetMenuActif( 0 );
	if( MenuConfigReseau )
		delete MenuConfigReseau;
	MenuConfigReseau = 0;

	lanceMenuConfig(0);
}

void lanceMenuConfigReseau(void *var)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigReseau(var=%x)", var );
	PF *liste_suivant_config_reseau;
	char **item_menu_config_reseau;

	stringPort.clear();

	int nbr = 2;	// Nombre de lignes dans ce menu (minimum = 1)

	if( !Config.Reseau.serveur )	// S'il s'agit d'un client (à l'opposé d'un serveur)
		nbr ++;	// Ajoute une ligne pour l'adresse ip du serveur

	liste_suivant_config_reseau = new PF[ nbr ];
	item_menu_config_reseau = new char*[ nbr ];

	int i = 0;

	item_menu_config_reseau[i] = new char[strlen("Serveur/Client")+1];
	strcpy( item_menu_config_reseau[i], "Serveur/Client" );
	liste_suivant_config_reseau[i++] = lanceMenuConfigReseauSwitch;

	if( !Config.Reseau.serveur )	// Si c'est un client, on affiche l'adresse IP du serveur
	{
		char txt[] = "IP du serveur";
		item_menu_config_reseau[i] = new char[ strlen(txt) + 1 ];
		strcpy( item_menu_config_reseau[i], txt );
		liste_suivant_config_reseau[i] = 0;

		i++;
	}
		// Client ou serveur, on affiche le port principal du serveur
	nbrMenuPort = i;
	char txt[] = "Port du serveur";
	item_menu_config_reseau[i] = new char[ strlen(txt) + 1 ];
	strcpy( item_menu_config_reseau[i], txt );
	liste_suivant_config_reseau[i] = lanceMenuConfigReseauPort;
	i++;

	if( MenuConfigReseau )
		delete MenuConfigReseau;

	MenuConfigReseau = new CMenu(title_menu_config_reseau, item_menu_config_reseau, nbr,
						liste_suivant_config_reseau, retourReseau );

	delete[] liste_suivant_config_reseau;
	delete[] item_menu_config_reseau;

	CDlg::SetMenuActif( MenuConfigReseau );

	i = 0;
	if( Config.Reseau.serveur )
	{
		MenuConfigReseau->add_ItemsDroits( i++, "Serveur" );
	}
	else
	{
		MenuConfigReseau->add_ItemsDroits( i++, "Client" );
		MenuConfigReseau->add_ItemsDroits( i++, Config.Reseau.getIpServer().c_str() );
	}

	ostringstream port;
	port << Config.Reseau.getPort();
	MenuConfigReseau->add_ItemsDroits( i++, port.str().c_str() );
}

void lanceMenuConfigReseauSwitch(void *arg)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigReseauSwitch(var=%x)", arg );
	retourReseau( 0 );
	Config.Reseau.serveur = !Config.Reseau.serveur;
	Config.Ecrit();
	lanceMenuConfigReseau( 0 );
}

void lanceMenuConfigReseauPort(void *arg)
{
TRACE().p( TRACE_MENU, "lanceMenuConfigReseauPort(var=%x)", arg );
	if( arg==0 )
	{
		MenuConfigReseau->mode = 1;	// Mode saisie de touche
		MenuConfigReseau->bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type )
		{
		case SDL_KEYDOWN:
			switch( event->key.keysym.sym )
			{
			case SDLK_0:
			case SDLK_KP0:
				if( stringPort.size() < 4 )
					stringPort += '0';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_1:
			case SDLK_KP1:
				if( stringPort.size() < 4 )
					stringPort += '1';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_2:
			case SDLK_KP2:
				if( stringPort.size() < 4 )
					stringPort += '2';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_3:
			case SDLK_KP3:
				if( stringPort.size() < 4 )
					stringPort += '3';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_4:
			case SDLK_KP4:
				if( stringPort.size() < 4 )
					stringPort += '4';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_5:
			case SDLK_KP5:
				if( stringPort.size() < 4 )
					stringPort += '5';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_6:
			case SDLK_KP6:
				if( stringPort.size() < 4 )
					stringPort += '6';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_7:
			case SDLK_KP7:
				if( stringPort.size() < 4 )
					stringPort += '7';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_8:
			case SDLK_KP8:
				if( stringPort.size() < 4 )
					stringPort += '8';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_9:
			case SDLK_KP9:
				if( stringPort.size() < 4 )
					stringPort += '9';
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_RETURN:
				int port;
				port = atoi( stringPort.c_str() );
				cout << endl <<stringPort << "\t" << port << endl;
				Config.Reseau.setPort( port );
				Config.Ecrit();	// Sauvegarde la configuration
				lanceMenuConfigReseau( 0 );
				break;

			case SDLK_BACKSPACE:
				if( stringPort.size() > 0 )
					stringPort = string( stringPort.begin(), stringPort.end()-1 );
				MenuConfigReseau->add_ItemsDroits( nbrMenuPort, stringPort.c_str() );
				break;

			case SDLK_ESCAPE:
				lanceMenuConfigReseau( 0 );
				break;
            default:
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			lanceMenuConfigReseau( 0 );
			break;

		default:
			break;
		}
	}
}

}	// JktMenu
