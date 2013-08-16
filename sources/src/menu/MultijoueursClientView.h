#ifndef __JKT__MULTIJOUEURSCLIENTVIEW_H
#define __JKT__MULTIJOUEURSCLIENTVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include <string>

#include "menu/View.h"

class MultijoueursClientView : public View {
	char* _serverName;
	char* _activeMap;
	int _ping;

	AG_Mutex _agMutex;

public:
	MultijoueursClientView(const AG_EventFn controllerCallback);
	~MultijoueursClientView(void);

	void show(void);
	void hide(void);

	void setServerName(const std::string& serverName);
	void setActiveMap(const std::string& activeMap);
	void setPing(int ping);
};

#endif

