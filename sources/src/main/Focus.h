
#ifndef __JKT__FOCUS_H
#define __JKT__FOCUS_H

namespace jkt
{

class CFocus
{
	void (*play_handle_key_down)( SDL_Event *event );			// Handle en cours de jeu
	void (*menu_handle_key_down)( SDL_Event *event );			// Handle pour le menu

	/**
	 * Intercepteur d'événements.
	 * Renvoi :
	 *    - true pour indiquer que l'évenement a été consommé
	 *    - false pour indiquer que l'événement n'a pas été consomme
	 */
	bool (*_event_interceptor)(SDL_Event *event);

	void (*focus_actif_handle_key_down)( SDL_Event *event );	// Handle pour le focus actif

public:
	CFocus(void (*play)(SDL_Event *event), void (*menu)(SDL_Event *event));

	/**
	 * Place un intercepteur sur la pile de consommation des événements.
	 * Cet intercepteur renvoie :
	 *    - true pour indiquer que l'événement a été consommé
	 *    - false pour indiquer que l'événement n'a pas été consommé
	 */
	void setEventInterceptor(bool (*eventInterceptor)(SDL_Event*));

	bool isPlayFocus();

	void SetPlayFocus();												// Place le focus sur le jeu
	void SetMenuFocus();												// Place le focus sur le menu
	void SetMenuAgarFocus();											// Place le focus sur le menu
	void SwitchPlayOrConsoleFocus();									// Place le focus sur le menu

	void ExecFocus( SDL_Event *Event );	// Exécute la fonction de focus
};

}	// JktMenu

#endif

