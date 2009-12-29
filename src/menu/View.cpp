#include "menu/View.h"

View::View(void)
{
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
