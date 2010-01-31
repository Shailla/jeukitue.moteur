#ifndef __JKT__LANCESERVEURVIEW_H
#define __JKT__LANCESERVEURVIEW_H

#include <vector>
#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class LanceServeurView : public View
{
	AG_Scrollview* _scrollview;
	std::vector<AG_Button*> _mapButtons;
	std::vector<std::string> _mapNames;
public:
	LanceServeurView(const AG_EventFn controllerCallback);
	~LanceServeurView(void);

	std::string getMapName(const int mapNumber);
	void show(void);
};

#endif
