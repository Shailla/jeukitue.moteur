#ifndef __JKT__PROGRESSBARVIEW_H
#define __JKT__PROGRESSBARVIEW_H

#include <string>
#include <vector>
using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ProgressBarView : public View
{
	int* _progress;
	char _currentOperationMessage[50];

public:
	ProgressBarView(const AG_EventFn controllerCallback);
	~ProgressBarView(void);

	int* getProgressPtr();
	char* getCurrentOperationMessagePtr();
};

#endif

