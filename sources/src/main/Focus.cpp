
#include <iostream>

using namespace std;

#include "SDL.h"
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/sdl.h>

#include "util/Trace.h"

#include "Focus.h"

namespace jkt
{

CFocus::CFocus(	void (*play)(SDL_Event *event), void (*menu)(SDL_Event *event)) {
	LOGDEBUG(("CFocus::CFocus(play=%x,menu=%x)%T", play, menu, this));
	play_handle_key_down = play;			// Impl�mente le focus d�di� � une partie en cours
	menu_handle_key_down = menu;			// Impl�mente le focus d�di� au menu

	focus_actif_handle_key_down = menu;		// Choix du focus actif par d�faut

	_event_interceptor = 0;
}

/**
 * Met en place un intercepteur d'�v�nements.
 */
void CFocus::setEventInterceptor(bool (*eventInterceptor)(SDL_Event*)) {
	_event_interceptor = eventInterceptor;
}

bool CFocus::isPlayFocus() {
	return focus_actif_handle_key_down == play_handle_key_down;
}

/**
 * Choisi la fonction focus d�di�e � une partie en cours
 */
void CFocus::SetPlayFocus() {
	SDL_WarpMouse( 250, 250 );			// Positionne la curseur de la souris
	SDL_ShowCursor( SDL_DISABLE );		// Cache le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	// Active le mode d�placement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

	focus_actif_handle_key_down = play_handle_key_down;
}

/**
 * Choisi la fonction focus d�di�e au menu
 */
void CFocus::SetMenuFocus() {
	SDL_WarpMouse( 250, 250 );			// Positionne le curseur de la souris
	SDL_ShowCursor( SDL_ENABLE );		// Affiche le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	// Active le mode d�placement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

	focus_actif_handle_key_down = menu_handle_key_down;
}

/*
 * Choisi la fonction focus d�di�e au menu
 */
void CFocus::SetMenuAgarFocus() {
	SDL_WarpMouse( 250, 250 );			// Positionne le curseur de la souris
	SDL_ShowCursor( SDL_ENABLE );		// Affiche le curseur de la souris
	SDL_WM_GrabInput( SDL_GRAB_ON );	// Active le mode d�placement relatif de la souris

	SDL_EnableKeyRepeat( 500, 500 );	// R�p�tition des touches clavier

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
 * Ex�cute la fonction de focus pour l'�v�nement 'event'
 */
void CFocus::ExecFocus(SDL_Event *event) {
	if(!_event_interceptor || !_event_interceptor(event)) {	// S'il n'y a pas d'intercepteur ou que rien n'a �t� intercept�
		if(focus_actif_handle_key_down) {
			focus_actif_handle_key_down(event);
		}

		/* ****************************************** */
		/* Gestion �v�nements menu Agar               */
		/* ****************************************** */
		AG_DriverEvent dev;
		AG_SDL_TranslateEvent(agDriverSw, event, &dev);
		AG_ProcessEvent(NULL, &dev);
	}
}

}	// JktMenu
