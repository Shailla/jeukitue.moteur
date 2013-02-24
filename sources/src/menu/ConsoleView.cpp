#include <vector>
#include <iostream>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "util/StringUtils.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "reseau/SPA.h"
#include "main/Statistics.h"

#include "menu/ConsoleView.h"

extern float delta;
Uint32 aaa;

ConsoleView::ConsoleView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_PLAIN|AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);

	AG_MutexInit(&_agMutex);

	AG_Notebook* book = AG_NotebookNew(m_window, 0);
	AG_Expand(book);

	/******************************
	 *	ONGLET PRINCIPAL
	 ******************************/

	AG_NotebookTab* tabMain = AG_NotebookAddTab(book, "Principal", AG_BOX_VERT);
	_console = AG_ConsoleNew(tabMain, AG_CONSOLE_EXPAND);

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HFILL);
	_commande = AG_TextboxNew(box, AG_TEXTBOX_STATIC|AG_TEXTBOX_HFILL, "Commande : ");
	AG_WidgetFocus(_commande);
	AG_WidgetEnable(_commande);
	AG_SetEvent(_commande, "textbox-return", controllerCallback, "%i", Controller::ExecuteUserCommandeAction); // L'appui sur ENTER est interprêté

	AG_ButtonNewFn(box, 0, "Ok", controllerCallback, "%i", Controller::ExecuteUserCommandeAction);


	/******************************
		ONGLET DES SCORES
	******************************/
	AG_NotebookTab* tabScores = AG_NotebookAddTab(book, "Scores", AG_BOX_VERT);
	AG_LabelNew(tabScores, 0, "Scores 1");
	AG_LabelNew(tabScores, 0, "Scores 2");
	AG_LabelNew(tabScores, 0, "Scores 3");


	/******************************
		ONGLET DES INFO
	******************************/
	AG_NotebookTab* tabInfo = AG_NotebookAddTab(book, "Info", AG_BOX_VERT);
	AG_Notebook* subbookInfo = AG_NotebookNew(tabInfo, 0);

	/******************************
		Sous-onglet Info / Partie
	******************************/
	AG_NotebookTab* subtabPartie = AG_NotebookAddTab(subbookInfo, "Partie", AG_BOX_VERT);
	AG_Scrollview* scrollInfo = AG_ScrollviewNew(subtabPartie, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);

	// Nom de la MAP ouverte dans la partie en cours
	memset(_mapOuverteName, '\0', sizeof(_mapOuverteName));
	AG_Label* label = AG_LabelNewPolledMT(scrollInfo, 0, &_agMutex, "Map ouverte : %s", _mapOuverteName);
	AG_LabelSizeHint(label, 1, "Map ouverte : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	// Distance parcourue par le tir laser
	label = AG_LabelNewPolledMT(scrollInfo, 0, &_agMutex, "Distance laser : %f", &delta);
	AG_LabelSizeHint(label, 1, "Distance laser : xxxxxx");

	label = AG_LabelNewPolledMT(scrollInfo, 0,&_agMutex, "Durees : calcules=%i ms, display=%i ms", &_dureeCalcules, &_dureeDisplay);
	AG_LabelSizeHint(label, 1, "Ecarts : total=xxxx ms, timer=xxxx ms, display=xxxx ms");

	/******************************
		Sous-onglet Info / Son
	******************************/
	/*AG_NotebookTab* subtabSon = */AG_NotebookAddTab(subbookInfo, "Son", AG_BOX_VERT);

	/******************************
		Sous-onglet Info / Réseau
	******************************/
	AG_NotebookTab* subtabReseau = AG_NotebookAddTab(subbookInfo, "Réseau", AG_BOX_VERT);
	AG_Scrollview* scrollReseau = AG_ScrollviewNew(subtabReseau, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);

	// Débit réseau en émission
	label = AG_LabelNewPolledMT(scrollReseau, 0, Statistics::getInstance()->getMutex(), "Debit en emission : %f ko/s (%f o)", &Statistics::getInstance()->_fDebitEm, &Statistics::getInstance()->_fTailleEm);
	AG_LabelSizeHint(label, 1, "Debit en emission : xxxx ko/s (xxxx o)");

	// Débit réseau en réception
	label = AG_LabelNewPolledMT(scrollReseau, 0, Statistics::getInstance()->getMutex(), "Debit en reception : %f ko/s (%f o)", &Statistics::getInstance()->_fDebitRec, &Statistics::getInstance()->_fTailleRec);
	AG_LabelSizeHint(label, 1, "Map ouverte : Debit en reception : xxxx ko/s (xxxx o)");

	// Disposition de la fenêtre	
	AG_WidgetUpdate(book);
	AG_NotebookSelectTab (book, tabMain);
	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_BL, 40, 30);
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

void ConsoleView::setMapOuverteName(const std::string& mapName) {
	AG_MutexLock(&_agMutex);
	mapName.copy(_mapOuverteName, sizeof(_mapOuverteName));
	AG_MutexUnlock(&_agMutex);
}

void ConsoleView::setDureeCalcules(Uint32 ecart) {
	AG_MutexLock(&_agMutex);
	_dureeCalcules = ecart;
	AG_MutexUnlock(&_agMutex);
}

void ConsoleView::setDureeDisplay(Uint32 ecart) {
	AG_MutexLock(&_agMutex);
	_dureeDisplay = ecart;
	AG_MutexUnlock(&_agMutex);
}

