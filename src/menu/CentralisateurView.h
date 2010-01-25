#ifndef __JKT__CENTRALISATEURVIEW_H
#define __JKT__CENTRALISATEURVIEW_H

#include <string>
#include <vector>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class CentralisateurView : public View
{
	AG_Textbox* _receivedTextbox;
	AG_Textbox* _textboxToSend;
	std::string _receivedText;
	char _textToSend[200];
	std::vector<char*> _playerList;
public:
	CentralisateurView(const AG_EventFn controllerCallback);
	~CentralisateurView(void);

	const char* getTextToSend() const;
	void clearTextToSend();
	void addChatMessage(const char* playerName, const char* message);
	void setPlayerList(char** playerList, int nombreJoueurs);
};

#endif

