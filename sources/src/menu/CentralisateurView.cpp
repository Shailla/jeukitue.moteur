#include <vector>
#include <iostream>

#include <agar/core.h>
#include <agar/gui.h>

#include "SDL_net.h"

#include "menu/View.h"
#include "menu/Controller.h"
#include "reseau/TcpUtils.h"
#include "exception/centralisateur/CentralisateurTcpException.h"

#include "menu/CentralisateurView.h"

using namespace std;
using namespace JktNet;

CentralisateurView::CentralisateurView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS | AG_WINDOW_NOMOVE);
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
	AG_Button* buttonMAJDownloads = AG_ButtonNewFn(tabDownload, 0, "Recharger", controllerCallback, "%i", Controller::ReloadDownloadFilesAction);
	_downloadFileTable = AG_TableNew(tabDownload, 0);
	AG_TableAddCol(_downloadFileTable, "Nom", "<8888888888888888>", NULL);
	AG_TableAddCol(_downloadFileTable, "Categorie", "<888888888888888>", NULL);
	AG_TableAddCol(_downloadFileTable, "Version", "<888888>", NULL);
	AG_TableAddCol(_downloadFileTable, "Taille", "<888888>", NULL);
	AG_TableAddCol(_downloadFileTable, "Description", NULL, NULL);
	AG_TableAddCol(_downloadFileTable, "Action", "<88888888888>", NULL);
	AG_Expand(_downloadFileTable);

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

void CentralisateurView::updateDownloadFileList(vector<DownloadFileItem> items)
{
	AG_TableBegin(_downloadFileTable);

	for(vector<DownloadFileItem>::iterator iter = items.begin() ; iter != items.end() ; iter++)
	{
		DownloadFileItem item = *iter;

		const char* category;
		if(item._category == MAP) {
			category = "Map de jeu";
		}
		else if(item._category == MAP_PLAYER) {
			category = "Map de joueur";
		}
		else {
			category = "Inconnu";
		}

		AG_Button* buttonDownload = AG_ButtonNewFn(NULL, 0, "Telecharger", m_controllerCallback, "%i,%li", Controller::DownloadOneFileAction, item._identifier);
		AG_TableAddRow(_downloadFileTable, "%s:%s:%s:%d:%s:%[W]", item._nom.c_str(), category, "Version", item._taille, item._description.c_str(), buttonDownload);
	}

	AG_TableEnd(_downloadFileTable);
	AG_Expand(_downloadFileTable);
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
