#ifndef __JKT__AGARCONTROLLER_H
#define __JKT__AGARCONTROLLER_H

#include <agar/core.h>
#include <agar/gui.h>

#include "AgarView.h"

class AgarController
{
public:
    static const int OpenCentralisateurAction;
    static const int OpenSceneAction;
    static const int QuitAction;
    static const int ConnectCentralisateurAction;

    static AgarView* m_agarView;

    AgarController(AgarView* agarView);
    ~AgarController(void);

    static void executeAction(AG_Event *event);
};

#endif
