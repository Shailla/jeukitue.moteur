#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "util/Couleurs.h"
#include "util/StringUtils.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "reseau/SPA.h"
#include "main/Statistics.h"
#include "data/ServeurDataTree.h"

#include "menu/ConsoleView.h"

using namespace std;

extern float delta;
Uint32 aaa;

ConsoleView::ConsoleView(const AG_EventFn controllerCallback)
:View(controllerCallback) {
	m_window = AG_WindowNew(AG_WINDOW_PLAIN | AG_WINDOW_NOBUTTONS | AG_WINDOW_NOBACKGROUND | AG_WINDOW_NOMOVE);

	AG_MutexInit(&_agMutex);

	AG_Notebook* book = AG_NotebookNew(m_window, 0);
	AG_Expand(book);

	_penteErwin = -1;

	/******************************
	 *	ONGLET CONSOLE
	 ******************************/

	AG_NotebookTab* tabMain = AG_NotebookAddTab(book, "Console", AG_BOX_VERT);
	_console = AG_ConsoleNew(tabMain, AG_CONSOLE_EXPAND);

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HFILL);
	_commande = AG_TextboxNew(box, AG_TEXTBOX_EXCL|AG_TEXTBOX_HFILL, "Commande : ");
	AG_WidgetFocus(_commande);
	AG_WidgetEnable(_commande);
	AG_SetEvent(_commande, "textbox-return", controllerCallback, "%i", Controller::ExecuteUserCommandeAction); // L'appui sur ENTER est interpr�t�

	_buttonOk = AG_ButtonNewFn(box, 0, "Ok", controllerCallback, "%i", Controller::ExecuteUserCommandeAction);

	/******************************
		ONGLET DES INFO
	 ******************************/
	AG_NotebookTab* tabGame = AG_NotebookAddTab(book, "Partie", AG_BOX_VERT);
	AG_Scrollview* scrollGame = AG_ScrollviewNew(tabGame, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);

	// Nom de la MAP ouverte dans la partie en cours
	memset(_mapOuverteName, '\0', sizeof(_mapOuverteName));
	AG_Label* label = AG_LabelNewPolledMT(scrollGame, 0, &_agMutex, "Map ouverte : %s", _mapOuverteName);
	AG_LabelSizeHint(label, 1, "Map ouverte : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	// Nom du joueur courant
	memset(_activePlayerName, '\0', sizeof(_activePlayerName));
	label = AG_LabelNewPolledMT(scrollGame, 0, &_agMutex, "Joueur : %s", _activePlayerName);
	AG_LabelSizeHint(label, 1, "Joueur : xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

	// Distance parcourue par le tir laser
	label = AG_LabelNewPolledMT(scrollGame, 0, &_agMutex, "Distance laser : %fm", &delta);
	AG_LabelSizeHint(label, 1, "Distance laser : xxxxxx");

	// Vitesse du perso principal
	label = AG_LabelNewPolledMT(scrollGame, 0, &_agMutex, "Vitesse : %fm/s", &_vitesseErwin);
	AG_LabelSizeHint(label, 1, "Vitesse : xxxxxxxx");

	// Pente du perso principal avec le sol
	label = AG_LabelNewPolledMT(scrollGame, 0, &_agMutex, "Pente : %f°", &_penteErwin);
	AG_LabelSizeHint(label, 1, "Pente : xxxxxxxx");

	label = AG_LabelNewPolledMT(scrollGame, 0,&_agMutex, "Durees : calcules=%i ms, display=%i ms", &_dureeCalcules, &_dureeDisplay);
	AG_LabelSizeHint(label, 1, "Ecarts : total=xxxx ms, timer=xxxx ms, display=xxxx ms");


	/******************************
		ONGLET DES INFO
	 ******************************/
	AG_NotebookTab* tabInfo = AG_NotebookAddTab(book, "Info", AG_BOX_VERT);
	AG_Notebook* subbookInfo = AG_NotebookNew(tabInfo, 0);

	{
		/******************************
		Sous-onglet Info / Son
		 ******************************/
		/*AG_NotebookTab* subtabSon = */AG_NotebookAddTab(subbookInfo, "Son", AG_BOX_VERT);

		/******************************
		Sous-onglet Info / Réseau
		 ******************************/
		AG_NotebookTab* subtabReseau = AG_NotebookAddTab(subbookInfo, "R\u00e9seau", AG_BOX_VERT);
		AG_Scrollview* scrollReseau = AG_ScrollviewNew(subtabReseau, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);

		// Débit réseau en émission
		label = AG_LabelNewPolledMT(scrollReseau, 0, Statistics::getInstance()->getMutex(), "Debit en emission : %f ko/s (%f o)", &Statistics::getInstance()->_fDebitEm, &Statistics::getInstance()->_fTailleEm);
		AG_LabelSizeHint(label, 1, "Debit en emission : xxxx ko/s (xxxx o)");

		// Débit réseau en réception
		label = AG_LabelNewPolledMT(scrollReseau, 0, Statistics::getInstance()->getMutex(), "Debit en reception : %f ko/s (%f o)", &Statistics::getInstance()->_fDebitRec, &Statistics::getInstance()->_fTailleRec);
		AG_LabelSizeHint(label, 1, "Map ouverte : Debit en reception : xxxx ko/s (xxxx o)");
	}

	// Disposition de la fenêtre
	AG_WidgetUpdate(book);
	AG_NotebookSelectTab (book, tabMain);
	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_BL, 70, 30);
	AG_WindowShow(m_window);

	hide();
}

ConsoleView::~ConsoleView(void) {
}

string ConsoleView::getCommandAndClearCommandLine() {
	// R�cup�ration de la saisie de l'utilisateur
	char* cmd = AG_TextboxDupString(_commande);
	string commande = cmd;

	// Vide la ligne de saisie
	AG_TextboxSetString(_commande, "");
	free(cmd);

	return commande;
}

void ConsoleView::println(ConsoleOutputType type, const stringstream& texte) {
	println(type, texte.str());
}

void ConsoleView::println(ConsoleOutputType type, const string& texte) {
	ostringstream str;
	AG_Color* lineColor;

	switch(type) {
	case COT_ECHO:
		str << "you> ";
		lineColor = &couleur_gris_clair;
		break;
	case COT_CHAT:
		str << "msg> ";
		lineColor = &couleur_violet;
		break;
	case COT_ERROR:
		str << "err> ";
		lineColor = &couleur_rouge;
		break;
	case COT_COMMAND_RESULT:
		str << "res> ";
		lineColor = &couleur_vert;
		break;
	default:
		str << "> ";
		lineColor = &couleur_blanc;
		break;
	}

	str << texte;

	AG_ConsoleLine* line = AG_ConsoleAppendLine(_console, str.str().c_str());
	AG_ConsoleMsgColor(line, lineColor);
}

void ConsoleView::setMapOuverteName(const std::string& mapName) {
	AG_MutexLock(&_agMutex);
	jkt::StringUtils::toChars(mapName, _mapOuverteName, sizeof(_mapOuverteName));
	AG_MutexUnlock(&_agMutex);
}

void ConsoleView::setActivePlayerName(const std::string& activePlayerName) {
	AG_MutexLock(&_agMutex);
	jkt::StringUtils::toChars(activePlayerName, _activePlayerName, sizeof(_activePlayerName));
	AG_MutexUnlock(&_agMutex);
}

void ConsoleView::setDureeCalcules(Uint32 computateDuration) {
	AG_MutexLock(&_agMutex);
	_dureeCalcules = computateDuration;
	AG_MutexUnlock(&_agMutex);
}

void ConsoleView::setVitesseErwin(float vitesse) {
	AG_MutexLock(&_agMutex);
	_vitesseErwin = vitesse;
	AG_MutexUnlock(&_agMutex);
}

void ConsoleView::setPenteErwin(float pente) {
	AG_MutexLock(&_agMutex);
	_penteErwin = pente;
	AG_MutexUnlock(&_agMutex);
}

void ConsoleView::setDureeDisplay(Uint32 displayDuration) {
	AG_MutexLock(&_agMutex);
	_dureeDisplay = displayDuration;
	AG_MutexUnlock(&_agMutex);
}

