#ifndef __JKT__VIEW_H
#define __JKT__VIEW_H

#include <agar/core.h>
#include <agar/gui.h>

class View
{
protected:
	AG_Window *m_window;
public:
	View(void);
	~View(void);

	void show(void);
    void hide(void);
};

#endif


