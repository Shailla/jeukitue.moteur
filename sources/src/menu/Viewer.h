#ifndef __JKT__AGARVIEW_H
#define __JKT__AGARVIEW_H

#include <agar/core.h>
#include <agar/gui.h>
#include <map>

using namespace std;

#include "menu/View.h"
class MainMenuView;
class ConfigurationView;
class ConfigCentralisateurView;
class CentralisateurView;
class OpenSceneView;
class OpenSceneASEView;
class OpenSceneMapView;
class OpenSceneASEEcraseRepView;
class ConsoleAvancementView;
class LanceServeurView;
class ConfigurationVideoView;
class ConfigurationJoueurView;
class ConsoleView;
class ProgressBarView;

class Viewer {
public:
	enum VIEWS {
		HIDE_ALL_VIEW = 0,
		MAIN_MENU_VIEW,
		ABOUT_VIEW,
		CENTRALISATEUR_VIEW,
		MULTIJOUEURS_VIEW,
		CONFIGURATION_VIEW,
		CONFIG_CENTRALISATEUR_VIEW,
		OPEN_SCENE_VIEW,
		OPEN_SCENE_ASE_VIEW,
		OPEN_SCENE_MAP_VIEW,
		OPEN_SCENE_ASE_ECRASE_REP_VIEW,
		CONSOLE_AVANCEMENT_VIEW,
		LANCE_SERVEUR_VIEW,
		CONFIGURATION_VIDEO_VIEW,
		CONFIGURATION_JOUEUR_VIEW,
		CONSOLE_VIEW,
		PROGRESS_BAR_VIEW,
		PLUGINS_MANAGEMENT_VIEW,
		DEBUG_MENU_VIEW,
		MAP_TREE_VIEW
	};

	map<VIEWS, View*> _views;
public:
    Viewer(AG_EventFn controllerCallback);
    ~Viewer(void);
    void showView(VIEWS view);
    View* getView(VIEWS view);
    void hideAllViews(void);
    void draw(void);
};

#endif
