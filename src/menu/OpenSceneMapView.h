#ifndef __JKT__OPENSCENEMAPVIEW_H
#define __JKT__OPENSCENEMAPVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class OpenSceneMapView : public View
{
	AG_Button* m_buttonRetour;
	AG_Scrollview* m_scrollview;
public:
	OpenSceneMapView(const AG_EventFn controllerCallback);
	~OpenSceneMapView(void);

	void show(void);
};

#endif
