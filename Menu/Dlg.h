
#ifndef __JKT__DIAG_H
#define __JKT__DIAG_H

#include "SDL.h"

namespace JKT_PACKAGE_MENU
{

class CDlg
{
	static CDlg* pMenuActif;		// Pointeur sur le menu actuellement actif
	static CDlg* m_old_MenuActif;	// Sert à la sauvegarde de l'ancien CDlg actif (PopMenuActif et PushMenuActif)

public:
	CDlg(void);
	virtual ~CDlg(void);

	static void SetMenuActif(CDlg *pMenu);				// Active un CDlg
	static void menu_handle_key_down(SDL_Event *event);	// Appelle le gestionnaire du CDlg actif
	static void afficheMenuActif();						// Affiche le CDlg actif
	static void PushMenuActif();						// Souviens-toi du menu actuellement actif
	static void PopMenuActif();							// Replace-toi sur le menu actif sauvé

	virtual void go()=0;								// Affiche le menu
	virtual void handle_key_down( SDL_Event *event )=0;	// Réagit aux frappes de touches
};

}	// JKT_PACKAGE_MENU

#endif
