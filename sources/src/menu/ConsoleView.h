#ifndef __JKT__CONSOLEVIEW_H
#define __JKT__CONSOLEVIEW_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#include "SDL.h"
#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ConsoleView : public View
{
	AG_Console* _console;
	AG_Textbox* _commande;
	AG_Button* _buttonOk;
	char _commandToExecute[200];

	char _mapOuverteName[30];
	Uint32 _dureeCalcules, _dureeDisplay;
	AG_Mutex _agMutex;

public:
	/** Types of output in the user console. */
	enum ConsoleOutputType {
		COT_ECHO,				// To echo the user commande line
		COT_CHAT,				// Chat message
		COT_COMMAND_ERROR,		// Command execution error message
		COT_COMMAND_RESULT,		// Command execution result message
		COT_INFO				// Info message
	};

	ConsoleView(const AG_EventFn controllerCallback);
	~ConsoleView(void);

	void println(ConsoleOutputType type, const string& texte);
	string getCommandAndClearCommandLine(void);
	void setMapOuverteName(const std::string& mapName);
	void setDureeCalcules(Uint32 ecart);
	void setDureeDisplay(Uint32 ecart);

};

#endif

