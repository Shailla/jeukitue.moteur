#ifndef __JKT__OPENSCENEASEVIEW_H
#define __JKT__OPENSCENEASEVIEW_H

#include <vector>

#include "menu/View.h"

class OpenSceneASEView : public View
{
	AG_Scrollview* _scrollview;
	std::vector<AG_Button*> _aseButtons;
	std::vector<std::string> _aseNames;
public:
	OpenSceneASEView(const AG_EventFn controllerCallback);
	~OpenSceneASEView(void);

	std::string getAseName(const int aseNumber);
	void show(void);
};

#endif

