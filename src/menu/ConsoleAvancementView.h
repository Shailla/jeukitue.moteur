#ifndef __JKT__CONSOLEAVANCEMENT_H
#define __JKT__CONSOLEAVANCEMENT_H

#include <string>
using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;

class ConsoleAvancementView : public View
{
	AG_Button* _buttonOk;
	AG_Console* _console;
public:
    ConsoleAvancementView(const AG_EventFn controllerCallback);
    ~ConsoleAvancementView(void);

	void init(const char* caption);
	void addMsg(const string& msg);
	void enableOkButton();
};

#endif
