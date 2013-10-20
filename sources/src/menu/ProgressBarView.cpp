#include <sstream>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"
#include "main/Cfg.h"

#include "menu/ProgressBarView.h"

ProgressBarView::ProgressBarView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
    m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS | AG_WINDOW_NOMOVE | AG_WINDOW_NORESIZE | AG_WINDOW_MODAL);
    AG_WindowSetCaption(m_window, "Avancement");

	// Barre de progression
	AG_ProgressBar* progressBar = AG_ProgressBarNew(m_window, AG_PROGRESS_BAR_HORIZ, AG_PROGRESS_BAR_SHOW_PCT);
	progressBar->min = 0;
	progressBar->max = 100;	// Barre en pourcentage
	progressBar->value = 0;
	AG_Expand(progressBar);
	_progressPourcentagePtr = &progressBar->value;

	// Message indiquant l'opération en cours
	AG_Label* label = AG_LabelNewPolled(m_window, 0, "%s", _currentOperationMessage);
	AG_Expand(label);

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 30, 15);
    hide();
}

ProgressBarView::~ProgressBarView(void)
{
}

void ProgressBarView::setProgressPercentage(const int progressPourcentage) {
	*_progressPourcentagePtr = progressPourcentage;
}

void ProgressBarView::setCurrentOperationMessage(const char* currentOperationMessage) {
	size_t maxLength = strlen(_currentOperationMessage);

	// Copie du message en faisant gaffe à ne pas dépasser la taille de la chaîne de caractères
	if(strlen(currentOperationMessage) >= (maxLength - 1) ) {
		strncpy(_currentOperationMessage, currentOperationMessage, maxLength - 1);
		_currentOperationMessage[maxLength - 1] = '\0';
	}
	else {
		strcpy(_currentOperationMessage, currentOperationMessage);
	}	
}
