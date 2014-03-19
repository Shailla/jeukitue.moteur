#include <iostream>

using namespace std;

#include "menu/View.h"

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
	cerr << endl << "yoyo1 " << m_window << endl << flush;
	AG_WindowShow(m_window);
	cerr << endl << "yoyo2 " << m_window << endl << flush;
	AG_WidgetFocus(m_window);
	cerr << endl << "yoyo3 " << m_window << endl << flush;
}

void View::hide(void) {
    AG_WindowHide(m_window);
    AG_WidgetUnfocus(m_window);
}
