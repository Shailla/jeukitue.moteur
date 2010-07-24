#ifndef __JKT__CONSOLEVIEW_H
#define __JKT__CONSOLEVIEW_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConsoleView : public View
{
	AG_Textbox* _consoleTextbox;
	AG_Textbox* _textboxToExecute;
	std::string _consoleText;
	char _commandToExecute[200];
	std::map<string, string> _consoleEntries;

	AG_Label* _mapOuverteLabel;
public:
	ConsoleView(const AG_EventFn controllerCallback);
	~ConsoleView(void);

	const char* getCommandToExecute() const;
	void clearTextToSend();
	void addChatMessage(const char* playerName, const char* message);

	void setMapOuverte(const std::string& mapName);

	void addConsoleEntry(const std::string& entryName, const std::string& entryValue);
};

#endif

