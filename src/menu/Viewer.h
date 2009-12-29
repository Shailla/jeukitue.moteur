#ifndef __JKT__AGARVIEW_H
#define __JKT__AGARVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
class MainMenuView;
class OpenCentralisateurView;
class OpenSceneView;
class OpenSceneASEView;
class OpenSceneMapView;

class Viewer
{
    bool m_isVisible;

	// Fenêtres
    View* m_mainMenuView;
    OpenCentralisateurView* m_openCentralisateurView;
	View* m_openSceneView;
	View* m_openSceneASEView;
	View* m_openSceneMapView;


	// Styles
	AG_Style *m_defaultWindowStyle;
public:
    Viewer(AG_EventFn controllerCallback);
    ~Viewer(void);
    void draw(void);
    void SetVisible(bool);
    bool IsVisible(void);

    void showMainMenu(void);
    void showOpenCentralisateur(void);
	void showOpenScene(void);
	void showOpenSceneASE(void);
	void showOpenSceneMap(void);

	// Accesseurs des fenêtres
    OpenCentralisateurView* getOpenCentralisateurView(void);

	// Accesseurs des styles
	AG_Style* Viewer::getDefaultWindowStyle();
};

#endif
