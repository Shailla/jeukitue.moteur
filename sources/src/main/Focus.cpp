
#include <iostream>

using namespace std;

#include "SDL.h"

#include "util/Trace.h"

#include "Focus.h"

namespace JktMenu
{

CFocus::CFocus(	void (*play)(SDL_Event *event),
				void (*menu)(SDL_Event *event),
				void (*menuAgar)(SDL_Event *event) )
{
LOGDEBUG(("CFocus::CFocus(play=%x,menu=%x)%T", play, menu, this));
	play_handle_key_down = play;	// Impl�mente le focus d�di� � une partie en cours
	menu_handle_key_down = menu;	// Impl�mente le focus d�di� au menu
	menu_agar_handle_key_down = menuAgar;	// Impl�mente le focus d�di� au menu

	focus_actif_handle_key_down = menu;	// Choix du focus actif par d�faut

	_event_interceptor = NULL;
}

/**
 * Met en place un intercepteur d'�v�nements.
 */
void CFocus::setEventInterceptor(bool (*eventInterceptor)(SDL_Event*)) {
	_event_interceptor = eventInterceptor;
}

/**
 * Supprime l'intercepteur d'�v�nements.
 */
void CFocus::removeEventInterceptor() {
	_event_interceptor = NULL;
}

/**
 * Choisi la fonction focus d�di�e � une partie en cours
 */
void CFocus::SetPlayFocus() {
LOGDEBUG(("CFocus::SetPlayFocus()%T", this));
	if( focus_actif_handle_key_down != play_handle_key_down )
	{
		SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
		SDL_ShowCursor( SDL_DISABLE );				//Cache le curseur de la souris
		SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode d�placement relatif de la souris

		SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

		focus_actif_handle_key_down = play_handle_key_down;
	}
}

/**
 * Choisi la fonction focus d�di�e au menu
 */
void CFocus::SetMenuFocus() {
LOGDEBUG(("CFocus::SetMenuFocus()%T", this));
	if( focus_actif_handle_key_down != menu_handle_key_down )
	{
		SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
		SDL_ShowCursor( SDL_ENABLE );		//Cache le curseur de la souris
		SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode d�placement relatif de la souris

		SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

		focus_actif_handle_key_down = menu_handle_key_down;
	}
}

/*
 * Choisi la fonction focus d�di�e au menu
 */
void CFocus::SetMenuAgarFocus() {
LOGDEBUG(("CFocus::SetMenuFocus()%T", this));
	if( focus_actif_handle_key_down != menu_handle_key_down )
	{
		SDL_WarpMouse( 250, 250 );			//positionnement du curseur de la souris
		SDL_ShowCursor( SDL_ENABLE );		//Cache le curseur de la souris
		SDL_WM_GrabInput( SDL_GRAB_ON );	//Active le mode d�placement relatif de la souris

		SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

		focus_actif_handle_key_down = menu_agar_handle_key_down;
	}
}

void CFocus::SwitchPlayOrConsoleFocus() {
LOGDEBUG(("CFocus::ChangePlayOrConsoleFocus()%T", this));
	if( focus_actif_handle_key_down == play_handle_key_down ) {
		SetMenuAgarFocus();	
	}
	else {
		SetPlayFocus();
	}
}

/*
 * Ex�cute la fonction de focus pour l'�v�nement 'event'
 */
void CFocus::ExecFocus(SDL_Event *event) {
	bool (*event_interceptor)(SDL_Event *event) = _event_interceptor;

	if(event_interceptor) {
		if(!event_interceptor(event)) {
			focus_actif_handle_key_down(event);
		}
	}
	else {
		focus_actif_handle_key_down(event);
	}
}

}	// JktMenu
