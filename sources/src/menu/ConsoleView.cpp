#include <vector>
#include <iostream>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "util/StringUtils.h"
#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/ConsoleView.h"

ConsoleView::ConsoleView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_PLAIN|AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);

	AG_Notebook* book = AG_NotebookNew(m_window, 0);
	AG_Expand(book);

	/******************************
	 *	Onglet principal
	 ******************************/

	AG_NotebookTab* tabMain = AG_NotebookAddTab(book, "Principal", AG_BOX_VERT);
	_console = AG_ConsoleNew(tabMain, AG_CONSOLE_EXPAND);

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HFILL);
	_commande = AG_TextboxNew(box, AG_TEXTBOX_STATIC|AG_TEXTBOX_HFILL, "Commande : ");
	AG_ButtonNewFn(box, 0, "Ok", controllerCallback, "%i", Controller::ConsoleUserExecuteAction);


	/******************************
		Onglet des scores
	******************************/
	AG_NotebookTab* tabScores = AG_NotebookAddTab(book, "Scores", AG_BOX_VERT);
	AG_LabelNew(tabScores, 0, "Scores 1");
	AG_LabelNew(tabScores, 0, "Scores 2");
	AG_LabelNew(tabScores, 0, "Scores 3");


	/******************************
		Onglet des info
	******************************/
	AG_NotebookTab* tabInfo = AG_NotebookAddTab(book, "Info", AG_BOX_VERT);
	AG_Scrollview* scrollInfo = AG_ScrollviewNew(tabInfo, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);
	_mapOuverteLabel = AG_LabelNew(scrollInfo, 0, "Map ouverte : ");


	// Disposition de la fenêtre	
	AG_WidgetUpdate(book);
	AG_NotebookSelectTab (book, tabMain);
	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_BL, 40, 60);
	AG_WindowShow(m_window);

    hide();
}

ConsoleView::~ConsoleView(void) {
}

void ConsoleView::executeCommande() {
	char* commande = AG_TextboxDupString(_commande);
	if(!JktUtils::StringUtils::isBlank(commande)) {
		println(commande);
		AG_TextboxSetString(_commande, "");
	}
	free(commande);
}

void ConsoleView::println(const char* texte) {
	AG_ConsoleMsgS(_console, texte);
}

void ConsoleView::setMapOuverte(const std::string& mapName) {
	std::string label;
	label.append("Map ouverte : '").append(mapName).append("'");
	AG_LabelText(_mapOuverteLabel, label.c_str() );
}
