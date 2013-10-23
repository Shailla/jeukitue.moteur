#ifndef __JKT__NETWORKCONNECTIONSSVIEW_H
#define __JKT__NETWORKCONNECTIONSSVIEW_H

#include <map>
#include <vector>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;

class NetworkConnectionsView : public View {
	AG_Table* _playersTable;

	void refreshList();

public:
	NetworkConnectionsView(const AG_EventFn controllerCallback);
    ~NetworkConnectionsView(void);

    static void refresh(AG_Event* event);

    void show();
};

#endif
