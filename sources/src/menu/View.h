#ifndef __JKT__VIEW_H
#define __JKT__VIEW_H

#include <agar/core.h>
#include <agar/gui.h>

class View
{
protected:
	AG_Window *m_window;
	AG_EventFn m_controllerCallback;
	bool m_isScrollviewEmpty;
public:
	View(const AG_EventFn controllerCallback);
	virtual ~View(void);

	virtual void show(void);
    virtual void hide(void);
};

#endif


