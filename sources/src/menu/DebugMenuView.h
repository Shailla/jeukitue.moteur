#ifndef __JKT__DEBUGMENUVIEW_H
#define __JKT__DEBUGMENUVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;

class DebugMenuView : public View {
public:
    DebugMenuView(const AG_EventFn controllerCallback);
    ~DebugMenuView(void);
};

#endif
