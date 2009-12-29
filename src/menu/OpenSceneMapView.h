#ifndef __JKT__OPENSCENEMAPVIEW_H
#define __JKT__OPENSCENEMAPVIEW_H

#include "menu/View.h"

class OpenSceneMapView : public View
{
	AG_Button* m_buttonRetour;
public:
	OpenSceneMapView(const AG_EventFn controllerCallback);
	~OpenSceneMapView(void);
};

#endif
