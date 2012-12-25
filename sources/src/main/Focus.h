
#ifndef __JKT__FOCUS_H
#define __JKT__FOCUS_H

namespace JktMenu
{

class CFocus
{
	void (*play_handle_key_down)( SDL_Event *event );			// Handle en cours de jeu
	void (*menu_handle_key_down)( SDL_Event *event );			// Handle pour le menu
	void (*menu_agar_handle_key_down)(SDL_Event *event);		// Handle pour le menu Agar

	void (*focus_actif_handle_key_down)( SDL_Event *event );	// Handle pour le focus actif

public:
	CFocus(void (*play)(SDL_Event *event), void (*menu)(SDL_Event *event), void (*menuAgar)(SDL_Event *event) );

	void SetPlayFocus();						// Place le focus sur le jeu
	void SetMenuFocus();						// Place le focus sur le menu
	void SetMenuAgarFocus();					// Place le focus sur le menu
	void SwitchPlayOrConsoleFocus();			// Place le focus sur le menu

	void ExecFocus( SDL_Event *Event );	// Exécute la fonction de focus
};

}	// JktMenu

#endif

