#include <sstream>

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "main/Cfg.h"

#include "menu/ProgressBarView.h"

extern CCfg Config;

ProgressBarView::ProgressBarView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
    m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS | AG_WINDOW_NOMOVE | AG_WINDOW_MODAL);
    AG_WindowSetCaption(m_window, "Avancement");

	// Barre de progression
	AG_ProgressBar* progressBar = AG_ProgressBarNew(m_window, AG_PROGRESS_BAR_HORIZ, AG_PROGRESS_BAR_SHOW_PCT);
	progressBar->min = 0;
	progressBar->max = 100;	// Barre en pourcentage
	progressBar->value = 0;

	_progress = &progressBar->value;

	// Message indiquant l'opération en cours
//	AG_LabelText(m_window, _currentOperationMessage);

    hide();
}

ProgressBarView::~ProgressBarView(void)
{
}

int* ProgressBarView::getProgressPtr() {
	return _progress;
}

char* ProgressBarView::getCurrentOperationMessagePtr() {
	return _currentOperationMessage;
}
