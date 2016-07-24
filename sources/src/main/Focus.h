
#ifndef __JKT__FOCUS_H
#define __JKT__FOCUS_H

namespace jkt
{

class CFocus
{
	void (*play_handle_key_down)( SDL_Event *event );			// Handle en cours de jeu
	void (*menu_handle_key_down)( SDL_Event *event );			// Handle pour le menu

	/**
	 * Intercepteur d'�v�nements.
	 * Renvoi :
	 *    - true pour indiquer que l'�venement a �t� consomm�
	 *    - false pour indiquer que l'�v�nement n'a pas �t� consomme
	 */
	bool (*_event_interceptor)(SDL_Event *event);

	void (*focus_actif_handle_key_down)( SDL_Event *event );	// Handle pour le focus actif

public:
	CFocus(void (*play)(SDL_Event *event), void (*menu)(SDL_Event *event));

	/**
	 * Place un intercepteur sur la pile de consommation des �v�nements.
	 * Cet intercepteur renvoie :
	 *    - true pour indiquer que l'�v�nement a �t� consomm�
	 *    - false pour indiquer que l'�v�nement n'a pas �t� consomm�
	 */
	void setEventInterceptor(bool (*eventInterceptor)(SDL_Event*));

	bool isPlayFocus();

	void SetPlayFocus();												// Place le focus sur le jeu
	void SetMenuFocus();												// Place le focus sur le menu
	void SetMenuAgarFocus();											// Place le focus sur le menu
	void SwitchPlayOrConsoleFocus();									// Place le focus sur le menu

	void ExecFocus( SDL_Event *Event );	// Ex�cute la fonction de focus
};

}	// JktMenu

#endif

