#ifndef __JKT__ABOUTVIEW_H
#define __JKT__ABOUTVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;

class AboutView : public View
{
    AG_Button* _buttonRetour;
public:
    AboutView(const AG_EventFn controllerCallback, Viewer* agarView);
    ~AboutView(void);
};

#endif
