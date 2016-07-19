
#include "util/Trace.h"

#include "Dlg.h"

namespace JktMenu
{

CDlg* CDlg::pMenuActif = 0;

void CDlg::SetMenuActif(CDlg *pMenu) {
	// A FAIRE : Précédure de suppression d'instance quand on quitte le menu actif
	pMenuActif = pMenu;
}

void CDlg::menu_handle_key_down(SDL_Event *event) {
	if( pMenuActif )
		pMenuActif->handle_key_down( event );
}

void CDlg::afficheMenuActif() {
	if( pMenuActif ) {
		pMenuActif->go();
	}
}

CDlg::CDlg(void) {
}

CDlg::~CDlg(void) {
}

}	// JktMenu
