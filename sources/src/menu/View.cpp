#include <iostream>

#include "menu/View.h"

using namespace std;

View::View(const AG_EventFn controllerCallback) {
    m_controllerCallback = controllerCallback;
    m_window = NULL;
}

View::~View(void) {
	AG_ObjectDelete(m_window);	// Frees all window resources and window's children resources
}

void View::refresh() {
}

void View::show(void) {
	AG_WindowShow(m_window);
	AG_WidgetFocus(m_window);
}

void View::hide(void) {
	AG_WindowHide(m_window);
	AG_WidgetUnfocus(m_window);
}
