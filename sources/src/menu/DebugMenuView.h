#ifndef __JKT__DEBUGMENUVIEW_H
#define __JKT__DEBUGMENUVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;

class DebugMenuView : public View
{
    AG_Button* _buttonMapTree;
	AG_Button* _buttonErreurOpenGl;
    AG_Button* _buttonRetour;
public:
    DebugMenuView(const AG_EventFn controllerCallback);
    ~DebugMenuView(void);
};

#endif
