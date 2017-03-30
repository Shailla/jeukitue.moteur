#ifndef __JKT__PROGRESSBARVIEW_H
#define __JKT__PROGRESSBARVIEW_H

#include <string>
#include <vector>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class ProgressBarView : public View {
	int* _progressPourcentagePtr;
	char _currentOperationMessage[50];

public:
	ProgressBarView(const AG_EventFn controllerCallback);
	~ProgressBarView(void);

	void setProgressPercentage(const int progressPourcentage);
	void setCurrentOperationMessage(const char* currentOperationMessage);
};

#endif

