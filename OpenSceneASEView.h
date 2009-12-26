#ifndef __JKT__OPENSCENEASEVIEW_H
#define __JKT__OPENSCENEASEVIEW_H

#include "View.h"

class OpenSceneASEView : public View
{
	AG_Button* m_buttonRetour;
public:
	OpenSceneASEView(const AG_EventFn controllerCallback);
	~OpenSceneASEView(void);
};

#endif

