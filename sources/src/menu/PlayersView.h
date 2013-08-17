#ifndef __JKT__PLAYERSVIEW_H
#define __JKT__PLAYERSVIEW_H

#include <map>
#include <vector>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"

class Viewer;

class PlayersView : public View {
	AG_Table* _playersTable;

	void refreshList();

public:
	PlayersView(const AG_EventFn controllerCallback);
    ~PlayersView(void);

    static void refresh(AG_Event* event);

    void show();
};

#endif
