#ifndef __JKT__AGARVIEW_H
#define __JKT__AGARVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
class MainMenuView;
class ConfigurationView;
class ConfigCentralisateurView;
class CentralisateurView;
class OpenSceneView;
class OpenSceneASEView;
class OpenSceneMapView;

class Viewer
{
    bool m_isVisible;

	// Fenêtres
    View* m_mainMenuView;
	CentralisateurView* m_centralisateurView;
	View* m_multijoueursView;
	View* m_configurationView;
    ConfigCentralisateurView* m_configCentralisateurView;
	View* m_openSceneView;
	OpenSceneASEView* m_openSceneASEView;
	OpenSceneMapView* m_openSceneMapView;
	View* _openSceneAseEcraseRepView;

	// Styles
	AG_Style *m_defaultWindowStyle;
public:
    Viewer(AG_EventFn controllerCallback);
    ~Viewer(void);
    void draw(void);
    void SetVisible(bool);
    bool IsVisible(void);

    void showMainMenu(void);
	void showMultijoueurs(void);
	void showCentralisateur(void);
	void showConfiguration(void);
    void showConfigCentralisateur(void);
	void showOpenScene(void);
	void showOpenSceneASE(void);
	void showOpenSceneASEEcraseRep(void);
	void showOpenSceneMap(void);

	// Accesseurs des fenêtres
    ConfigCentralisateurView* getConfigCentralisateurView(void);
	OpenSceneMapView* getOpenSceneMapView(void);
	OpenSceneASEView* getOpenSceneASEView(void);
	CentralisateurView* getCentralisateurView(void);

	// Accesseurs des styles
	AG_Style* Viewer::getDefaultWindowStyle();
};

#endif
