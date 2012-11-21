#include "menu/View.h"

#include <iostream>

View::View(const AG_EventFn controllerCallback) {
    m_controllerCallback = controllerCallback;
    m_window = NULL;
}

View::~View(void)
{
}

void View::show(void)
{
	AG_WindowShow(m_window);
	AG_WidgetFocus(m_window);
}

void View::hide(void)
{
    AG_WindowHide(m_window);
    AG_WidgetUnfocus(m_window);
}
