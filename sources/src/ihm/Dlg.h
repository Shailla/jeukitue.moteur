
#ifndef __JKT__DIAG_H
#define __JKT__DIAG_H

#include "SDL.h"

namespace JktMenu
{

class CDlg {
	static CDlg* pMenuActif;		// Pointeur sur le menu actuellement actif

public:
	CDlg(void);
	virtual ~CDlg(void);

	static void SetMenuActif(CDlg *pMenu);				// Active un CDlg
	static void menu_handle_key_down(SDL_Event *event);	// Appelle le gestionnaire du CDlg actif
	static void afficheMenuActif();						// Affiche le CDlg actif

	virtual void go()=0;								// Affiche le menu
	virtual void handle_key_down( SDL_Event *event )=0;	// Réagit aux frappes de touches
};

}	// JktMenu

#endif
