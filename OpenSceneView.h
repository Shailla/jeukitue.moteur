#ifndef __JKT__OPENSCENEVIEW_H
#define __JKT__OPENSCENEVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "View.h"

class OpenSceneView : public View
{
	AG_Button* m_buttonConvertirSceneASE;
    AG_Button* m_buttonOuvrirSceneMap;
    AG_Button* m_buttonRetour;
public:
	OpenSceneView(const AG_EventFn controllerCallback);
	~OpenSceneView(void);
};

#endif