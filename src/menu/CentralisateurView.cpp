#include <vector>
#include <iostream>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/CentralisateurView.h"

CentralisateurView::CentralisateurView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Centralisateur");

	AG_Notebook* book = AG_NotebookNew(m_window, 0);
	AG_Expand(book);

	/******************************
		Onglet de chat
	******************************/
	AG_NotebookTab* tabChat = AG_NotebookAddTab(book, "Chat", AG_BOX_VERT);
	AG_Pane* paneHoriz = AG_PaneNewHoriz(tabChat, AG_PANE_EXPAND);
	AG_Button* button1 = AG_ButtonNew(paneHoriz->div[0], 0, "Coucou");
	AG_PaneMoveDividerPct(paneHoriz, 20);

	AG_Pane* paneVert = AG_PaneNewVert(paneHoriz->div[1], AG_PANE_EXPAND);
	AG_PaneMoveDividerPct(paneVert, 70);

	// Panel des messages reçus
	_receivedTextbox = 
		AG_TextboxNew(	paneVert->div[0],
						AG_TEXTBOX_MULTILINE|AG_TEXTBOX_EXPAND,
						NULL);
	AG_WidgetDisable(_receivedTextbox);
	
	// Panel d'envoi de messages
	_textboxToSend = 
		AG_TextboxNew(	paneVert->div[1],
						AG_TEXTBOX_MULTILINE|AG_TEXTBOX_EXPAND,
						NULL);
	memset(_textToSend, '\0', sizeof(_textToSend));
	AG_TextboxBindUTF8(_textboxToSend, _textToSend, sizeof(_textToSend));
	AG_Button* buttonSend = AG_ButtonNewFn(paneVert->div[1], 0, "Envoyer", controllerCallback, "%i", Controller::SendGlobalChatTextAction);
	

	/******************************
		Onglet des scores
	******************************/
	AG_NotebookTab* tabScores = AG_NotebookAddTab(book, "Scores", AG_BOX_VERT);


	/******************************
		Onglet des téléchargements
	******************************/
	AG_NotebookTab* tabDownload = AG_NotebookAddTab(book, "Telechargements", AG_BOX_VERT);


	// Bouton retour
	AG_SeparatorNewHoriz(m_window);
    AG_Button* buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::MultijoueursAction);

	// Disposition de la fenêtre
    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 70, 70);
	AG_WindowShow(m_window);
    hide();
}

CentralisateurView::~CentralisateurView(void)
{
}

const char* CentralisateurView::getTextToSend(void) const
{
	return _textToSend;
}

void CentralisateurView::addChatMessage(const char* playerName, const char *message)
{
	_receivedText.append("\n");
	_receivedText.append(playerName);
	_receivedText.append("> ");
	_receivedText.append(message);
	AG_TextboxSetString(_receivedTextbox, _receivedText.c_str());
}

void CentralisateurView::setPlayerList(char** playerList, int nombreJoueurs)
{
	std::vector<char*>::iterator it;
	for(it=_playerList.begin() ; it<_playerList.end() ; it++) {
		delete *it;
	}

	_playerList.clear();

	for(int i=0 ; i<nombreJoueurs ; i++) {
		_playerList.push_back(playerList[i]);
		std::cout << "Joueur connecté au chat :" << playerList[i];
	}
}


void CentralisateurView::clearTextToSend(void)
{
	memset(_textToSend, '\0', sizeof(_textToSend));
}
