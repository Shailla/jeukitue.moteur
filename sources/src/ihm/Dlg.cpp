
#include "util/Trace.h"

#include "Dlg.h"

namespace JktMenu
{

CDlg* CDlg::pMenuActif = 0;
CDlg* CDlg::m_old_MenuActif = 0;

void CDlg::PushMenuActif()
{
	m_old_MenuActif = pMenuActif;	
}

void CDlg::PopMenuActif()
{
	pMenuActif = m_old_MenuActif;	
}


void CDlg::SetMenuActif(CDlg *pMenu)
{
	// A FAIRE : Précédure de suppression d'instance quand on quitte le menu actif
	pMenuActif = pMenu;
}

void CDlg::menu_handle_key_down(SDL_Event *event)
{
	if( pMenuActif )
		pMenuActif->handle_key_down( event );
}

void CDlg::afficheMenuActif() {
	if( pMenuActif ) {
		LOGINFO(("YOOOOOOOOOOOOOO"));
		pMenuActif->go();
	}
}

CDlg::CDlg(void)
{
}

CDlg::~CDlg(void)
{
}

}	// JktMenu
