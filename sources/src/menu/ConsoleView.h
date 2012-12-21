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
	AG_Console* _console;
	AG_Textbox* _commande;
	char _commandToExecute[200];
	AG_Label* _mapOuverteLabel;

public:
	ConsoleView(const AG_EventFn controllerCallback);
	~ConsoleView(void);

	void println(const char* texte);
	void executeCommande(void);
	void setMapOuverte(const std::string& mapName);
};

#endif

