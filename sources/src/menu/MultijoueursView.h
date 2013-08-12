#ifndef __JKT__MULTIJOUEURSVIEW_H
#define __JKT__MULTIJOUEURSVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include <string>

#include "menu/View.h"

class MultijoueursView : public View {
	char* _serverName;
	char* _activeMap;
	int _ping;

	AG_Mutex _agMutex;

public:
	MultijoueursView(const AG_EventFn controllerCallback);
	~MultijoueursView(void);

	void setServerName(const std::string& serverName);
	void setActiveMap(const std::string& activeMap);
	void setPing(int ping);
};

#endif

