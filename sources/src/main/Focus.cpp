
#include <iostream>

using namespace std;

#include "SDL.h"

#include "util/Trace.h"

#include "Focus.h"

namespace JktMenu
{

CFocus::CFocus(	void (*play)(SDL_Event *event), void (*menu)(SDL_Event *event)) {
	LOGDEBUG(("CFocus::CFocus(play=%x,menu=%x)%T", play, menu, this));
	play_handle_key_down = play;			// Implémente le focus dédié à une partie en cours
	menu_handle_key_down = menu;			// Implémente le focus dédié au menu

	focus_actif_handle_key_down = menu;		// Choix du focus actif par défaut

	_event_interceptor = 0;
}

/**
 * Met en place un intercepteur d'événements.
 */
void CFocus::setEventInterceptor(bool (*eventInterceptor)(SDL_Event*)) {
	_event_interceptor = eventInterceptor;
}

/**
 * Supprime l'intercepteur d'événements.
 */
void CFocus::removeEventInterceptor() {
	_event_interceptor = 0;
}

bool CFocus::isPlayFocus() {
	return focus_actif_handle_key_down == play_handle_key_down;
}

/**
 * Choisi la fonction focus dédiée à une partie en cours
 */
void CFocus::SetPlayFocus() {
	SDL_WarpMouse( 250, 250 );			// Positionne la curseur de la souris
	SDL_ShowCursor( SDL_DISABLE );		// Cache le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	// Active le mode déplacement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// Répétition des touches clavier

	focus_actif_handle_key_down = play_handle_key_down;
}

/**
 * Choisi la fonction focus dédiée au menu
 */
void CFocus::SetMenuFocus() {
	SDL_WarpMouse( 250, 250 );			// Positionne le curseur de la souris
	SDL_ShowCursor( SDL_ENABLE );		// Affiche le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	// Active le mode déplacement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// Répétition des touches clavier

	focus_actif_handle_key_down = menu_handle_key_down;
}

/*
 * Choisi la fonction focus dédiée au menu
 */
void CFocus::SetMenuAgarFocus() {
	SDL_WarpMouse( 250, 250 );			// Positionne le curseur de la souris
	SDL_ShowCursor( SDL_ENABLE );		// Affiche le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	// Active le mode déplacement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// Répétition des touches clavier

	focus_actif_handle_key_down = 0;
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
 * Exécute la fonction de focus pour l'évênement 'event'
 */
void CFocus::ExecFocus(SDL_Event *event) {
	bool (*event_interceptor)(SDL_Event *event) = _event_interceptor;

	if(focus_actif_handle_key_down) {
		if(event_interceptor) {
			if(!event_interceptor(event)) {
				focus_actif_handle_key_down(event);
			}
		}
		else {
			focus_actif_handle_key_down(event);
		}
	}
}

}	// JktMenu
