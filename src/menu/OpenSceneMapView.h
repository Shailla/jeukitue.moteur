#ifndef __JKT__OPENSCENEMAPVIEW_H
#define __JKT__OPENSCENEMAPVIEW_H

#include <vector>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class OpenSceneMapView : public View
{
	AG_Scrollview* _scrollview;
	std::vector<AG_Button*> _mapButtons;
	std::vector<std::string> _mapNames;
public:
	OpenSceneMapView(const AG_EventFn controllerCallback);
	~OpenSceneMapView(void);

	std::string getMapName(const int mapNumber);
	void show(void);
};

#endif
