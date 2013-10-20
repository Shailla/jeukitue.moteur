#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "main/Player.h"
#include "main/Game.h"

#include "menu/PlayersView.h"

extern CGame Game;

PlayersView::PlayersView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Liste des joueurs");

	// Liste des données
	_playersTable = AG_TableNew(m_window, AG_TABLE_EXPAND);
	AG_TableAddCol(_playersTable, "Nom", "<XXXXXXXXXXXXXXXXXX>", NULL);
	AG_TableAddCol(_playersTable, "IP", "<XXXXXXXXXXXXXXX>", NULL);
	AG_TableAddCol(_playersTable, "Port", "<XXXXX>", NULL);

	AG_SeparatorNewHoriz(m_window);

	// Boutons
	AG_Box* boxButtons = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(boxButtons);

	// Bouton retour
	AG_ButtonNewFn(boxButtons, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);

	// Bouton rafraîchir
	AG_ButtonNewFn(boxButtons, 0, "Rafraichir", PlayersView::refresh, "%p", this);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_TC, 100, 50);
	hide();
}

PlayersView::~PlayersView(void) {
}


void PlayersView::refresh(AG_Event* event) {
	PlayersView* thiz = (PlayersView*)AG_PTR(1);
	thiz->refreshList();
}

void PlayersView::refreshList() {

	AG_TableBegin(_playersTable);

	if(Game._pTabIndexPlayer) {
		int curseur = -1;
		CPlayer* player;

		while(Game._pTabIndexPlayer->Suivant(curseur)) {
			player = Game._pTabIndexPlayer->operator []( curseur );
			AG_TableAddRow(_playersTable, "%s:%s:%s:", player->nom().c_str(), "xxx.xxx.xxx.xxx", "xxxxx");
		}
	}
	AG_TableEnd(_playersTable);

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);
}

void PlayersView::show(void) {
	refreshList();
	View::show();
}

