#ifndef __JKT__MULTIJOUEURSVIEW_H
#define __JKT__MULTIJOUEURSVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include <string>

#include "menu/View.h"

class MultijoueursView : public View {

public:
	MultijoueursView(const AG_EventFn controllerCallback);
	~MultijoueursView(void);

	void show(void);
};

#endif

