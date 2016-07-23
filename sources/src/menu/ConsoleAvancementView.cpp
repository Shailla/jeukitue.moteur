
#include <agar/core.h>
#include <agar/gui.h>

#include "util/Couleurs.h"
#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/ConsoleAvancementView.h"

ConsoleAvancementView::ConsoleAvancementView(const AG_EventFn controllerCallback) : View(controllerCallback) {
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);

	_console = AG_ConsoleNew(m_window, 0);
	AG_Expand(_console);

	AG_SeparatorNewHoriz(m_window);
    _buttonOk = AG_ButtonNewFn(m_window, 0, "Ok", controllerCallback, "%i", Controller::ShowMenuAction);

    AG_ExpandHoriz(_buttonOk);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConsoleAvancementView::~ConsoleAvancementView(void) {
}

void ConsoleAvancementView::init(const char* caption) {
	AG_WindowSetCaption(m_window, caption);
	AG_WidgetDisable(_buttonOk);
}

void ConsoleAvancementView::addMsg(const string& msg) {
	AG_ConsoleLine* line = AG_ConsoleAppendLine(_console, msg.c_str());
	AG_ConsoleMsgColor(line, &couleur_blanc);
}

void ConsoleAvancementView::addMsgError(const string& msg) {
	AG_ConsoleLine* line = AG_ConsoleAppendLine(_console, msg.c_str());
	AG_ConsoleMsgColor(line, &couleur_rouge);
}


void ConsoleAvancementView::enableOkButton() {
	AG_WidgetEnable(_buttonOk);
}
