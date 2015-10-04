	/****************************/
	/*							*/
	/*   MENU PRINCIPAL SIMPLE	*/
	/*							*/
	/****************************/

#include <fstream>
#include <iostream>
using namespace std;

#include "ihm/DlgBoite.h"
#include "ihm/Menu.h"

#include "main/Focus.h"
#include "main/Cfg.h"
#include "util/Trace.h"

#include "ihm/MMenuPrinc.h"

using namespace JktMenu;

class CGame;

extern CCfg Config;

#include "ihm/MConfiguration.h"

namespace JktMenu
{
void lanceMenuConfigCommandesAvancer(void *arg);
void lanceMenuConfigCommandesReculer(void *arg);
void lanceMenuConfigCommandesGauche(void *arg);
void lanceMenuConfigCommandesDroite(void *arg);
void lanceMenuConfigCommandesTir1(void *arg);
void lanceMenuConfigCommandesTir2(void *arg);
void lanceMenuConfigCommandesMonter(void *arg);

	/************************************/
	/*									*/
	/*   MENU CONFIGURATION COMMANDES	*/
	/*									*/
	/************************************/

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

const char *item_menu_config_commandes[] =
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

void lanceMenuConfigCommandes(void *var) {
LOGDEBUG(("lanceMenuConfigCommandes(var=%x)", var));
	MenuConfigCommandes.bItemsDroits = false;
	MenuConfigCommandes.mode = 0;

	MenuConfigCommandes.add_ItemsDroits( 0, Config.Commandes.resolve(Config.Commandes.Avancer) );
	MenuConfigCommandes.add_ItemsDroits( 1, Config.Commandes.resolve(Config.Commandes.Reculer) );
	MenuConfigCommandes.add_ItemsDroits( 2, Config.Commandes.resolve(Config.Commandes.Gauche) );
	MenuConfigCommandes.add_ItemsDroits( 3, Config.Commandes.resolve(Config.Commandes.Droite) );
	MenuConfigCommandes.add_ItemsDroits( 4, Config.Commandes.resolve(Config.Commandes.Tir1) );
	MenuConfigCommandes.add_ItemsDroits( 5, Config.Commandes.resolve(Config.Commandes.Tir2) );
	MenuConfigCommandes.add_ItemsDroits( 6, Config.Commandes.resolve(Config.Commandes.Monter) );
	MenuConfigCommandes.add_ItemsDroits( 6, Config.Commandes.resolve(Config.Commandes.SelectWeaponUp) );
	MenuConfigCommandes.add_ItemsDroits( 6, Config.Commandes.resolve(Config.Commandes.SelectWeaponDown) );

	CDlg::SetMenuActif( &MenuConfigCommandes );
}

void lanceMenuConfigCommandesAvancer(void *arg) {
LOGDEBUG(("lanceMenuConfigCommandesAvancer(var=%x)", arg));
	if( arg==0 ) {
		MenuConfigCommandes.mode = 1;		// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else {
		SDL_Event *event = (SDL_Event*)arg;

		switch(event->type) {
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE ) {
				Config.Commandes.Avancer.key = event->key.keysym.sym;
				Config.Commandes.Avancer.mouse = 0;
				Config.Ecrit();	// Sauvegarde la configuration

				lanceMenuConfigCommandes( 0 );
			}
			else {
				lanceMenuConfigCommandes( 0 );
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Avancer.mouse = event->button.button;
			Config.Commandes.Avancer.key = (SDLKey)0;
			Config.Ecrit();		// Sauvegarde la configuration

			lanceMenuConfigCommandes( 0 );
			break;

		default:
			break;
		}
	}
}

void lanceMenuConfigCommandesReculer(void *arg)
{
LOGDEBUG(("lanceMenuConfigCommandesReculer(var=%x)", arg));
	if( arg==0 )
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
				Config.Commandes.Reculer.mouse = 0;
				Config.Ecrit();	// Sauvegarde la configuration

				lanceMenuConfigCommandes( 0 );
			}
			else
			{
				lanceMenuConfigCommandes( 0 );
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Reculer.mouse = event->button.button;
			Config.Commandes.Reculer.key = (SDLKey)0;
			Config.Ecrit();		// Sauvegarde la configuration

			lanceMenuConfigCommandes( 0 );
			break;

		default:
			break;
		}
	}
}

void lanceMenuConfigCommandesGauche(void *arg)
{
LOGDEBUG(("lanceMenuConfigCommandesGauche(var=%x)", arg));
	if( arg==0 )
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
				Config.Commandes.Gauche.mouse = 0;
				Config.Ecrit();	// Sauvegarde la configuration

				lanceMenuConfigCommandes(0);
			}
			else
			{
				lanceMenuConfigCommandes( 0 );
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Gauche.mouse = event->button.button;
			Config.Commandes.Gauche.key = (SDLKey)0;
			Config.Ecrit();		// Sauvegarde la configuration

			lanceMenuConfigCommandes(0);
			break;

		default:
			break;
		}
	}
}

void lanceMenuConfigCommandesDroite(void *arg)
{
LOGDEBUG(("lanceMenuConfigCommandesDroite(var=%x)", arg));
	if( arg==0 )
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
				Config.Commandes.Droite.mouse = 0;
				Config.Ecrit();	// Sauvegarde la configuration

				lanceMenuConfigCommandes(0);
			}
			else
			{
				lanceMenuConfigCommandes( 0 );
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Droite.mouse = event->button.button;
			Config.Commandes.Droite.key = (SDLKey)0;
			Config.Ecrit();		// Sauvegarde la configuration

			lanceMenuConfigCommandes(0);
			break;

		default:
			break;
		}
	}
}

void lanceMenuConfigCommandesTir1(void *arg) {
LOGDEBUG(("lanceMenuConfigCommandesTir1(var=%x)", arg));
	if( arg==0 )
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
				Config.Commandes.Tir1.mouse = 0;
				Config.Ecrit();	// Sauvegarde la configuration

				lanceMenuConfigCommandes(0);
			}
			else
			{
				lanceMenuConfigCommandes( 0 );
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Tir1.mouse = event->button.button;
			Config.Commandes.Tir1.key = (SDLKey)0;
			Config.Ecrit();		// Sauvegarde la configuration

			lanceMenuConfigCommandes(0);
			break;

		default:
			break;
		}
	}
}

void lanceMenuConfigCommandesTir2(void *arg) {
LOGDEBUG(("lanceMenuConfigCommandesTir2(var=%x)", arg));
	if( arg==0 ) {
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else {
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type ) {
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE ) {
				Config.Commandes.Tir2.key = event->key.keysym.sym;
				Config.Commandes.Tir2.mouse = 0;
				Config.Ecrit();	// Sauvegarde la configuration

				lanceMenuConfigCommandes(0);
			}
			else {
				lanceMenuConfigCommandes( 0 );
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Tir2.mouse = event->button.button;
			Config.Commandes.Tir2.key = (SDLKey)0;
			Config.Ecrit();		// Sauvegarde la configuration

			lanceMenuConfigCommandes(0);
			break;

		default:
			break;
		}
	}
}

void lanceMenuConfigCommandesMonter(void *arg)
{
LOGDEBUG(("lanceMenuConfigCommandesMonter(var=%x)", arg));
	if( arg==0 )
	{
		MenuConfigCommandes.mode = 1;	// Mode saisie de touche
		MenuConfigCommandes.bItemsDroits = true;
	}
	else
	{
		SDL_Event *event = (SDL_Event*)arg;

		switch( event->type ) {
		case SDL_KEYDOWN:
			if( event->key.keysym.sym!=SDLK_ESCAPE ) {
				Config.Commandes.Monter.key = event->key.keysym.sym;
				Config.Commandes.Monter.mouse = 0;
				Config.Ecrit();	// Sauvegarde la configuration

				lanceMenuConfigCommandes(0);
			}
			else {
				lanceMenuConfigCommandes( 0 );
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			Config.Commandes.Monter.mouse = event->button.button;
			Config.Commandes.Monter.key = (SDLKey)0;
			Config.Ecrit();		// Sauvegarde la configuration

			lanceMenuConfigCommandes(0);
			break;

		default:
			break;
		}
	}
}

}	// JktMenu
