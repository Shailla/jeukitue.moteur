#include <vector>
#include <iostream>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

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
	AG_Pane* vPane = AG_PaneNewVert(tabMain, AG_PANE_DIV1FILL);
	AG_PaneMoveDividerPct(vPane, 50);

	// Panel des messages reçus
	_consoleTextbox = AG_TextboxNew(vPane->div[0], AG_TEXTBOX_MULTILINE|AG_TEXTBOX_EXPAND, NULL);
	
	
	AG_WidgetDisable(_consoleTextbox);
	
	// Ligne de saisie de commande utilisateur
	_textboxToExecute = AG_TextboxNew(vPane->div[1], AG_TEXTBOX_HFILL, NULL);

	memset(_commandToExecute, '\0', sizeof(_commandToExecute));
	memcpy(_commandToExecute, "Execution", sizeof("Execution"));
	AG_TextboxBindUTF8(_textboxToExecute, _commandToExecute, sizeof(_commandToExecute));

	//AG_Button* buttonSend = AG_ButtonNewFn(paneHoriz->div[1], 0, "Executer", controllerCallback, "%i", Controller::ConsoleUserExecuteAction);

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

const char* ConsoleView::getCommandToExecute(void) const {
	return _commandToExecute;
}

void ConsoleView::println(const char* event) {
	_consoleText.append("\n").append(event);
	AG_LabelText(_mapOuverteLabel, _consoleText.c_str());
}

void ConsoleView::setMapOuverte(const std::string& mapName) {
	std::string label;
	label.append("Map ouverte : '").append(mapName).append("'");
	AG_LabelText(_mapOuverteLabel, label.c_str() );
}
