#ifndef __JKT__AGARCONTROLLER_H
#define __JKT__AGARCONTROLLER_H

#include <agar/core.h>
#include <agar/gui.h>

#include "Viewer.h"

class Controller
{
public:
    static const int MainMenuAction;
	static const int OpenCentralisateurAction;
    static const int OpenSceneAction;
	static const int OpenSceneASEAction;
	static const int OpenSceneMapAction;
	static const int OpenOneSceneMapAction;
    static const int QuitAction;
    static const int ConnectCentralisateurAction;

    static Viewer* m_agarView;

    Controller(Viewer* agarView);
    ~Controller(void);

    static void executeAction(AG_Event *event);
};

#endif
