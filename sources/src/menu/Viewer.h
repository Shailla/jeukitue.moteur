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
class ConfigurationReseauView;
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
		MULTIJOUEURS_CLIENT_VIEW,
		CONFIGURATION_VIEW,
		CONFIG_CENTRALISATEUR_VIEW,
		OPEN_SCENE_VIEW,
		OPEN_SCENE_ASE_VIEW,
		OPEN_SCENE_MAP_VIEW,
		OPEN_SCENE_ASE_ECRASE_REP_VIEW,
		CONSOLE_AVANCEMENT_VIEW,
		LANCE_SERVEUR_VIEW,
		CONFIGURATION_AUDIO_VIEW,
		CONFIGURATION_COMMANDES_VIEW,
		CONFIGURATION_VIDEO_VIEW,
		CONFIGURATION_JOUEUR_VIEW,
		CONFIGURATION_RESEAU_VIEW,
		CONSOLE_VIEW,
		PROGRESS_BAR_VIEW,
		PLUGINS_MANAGEMENT_VIEW,
		DEBUG_MENU_VIEW,
		MAP_TREE_VIEW,
		DATA_TREE_VIEW,
		PLAYERS_LIST_VIEW,
		NETWORK_CONNECTIONS_VIEW
	};

private:
	map<VIEWS, View*> _views;
	map<VIEWS, View*> _menuViews;
	void addMenuView(VIEWS viewId, View* view);
	void addSimpleView(VIEWS viewId, View* view);
public:
    Viewer(AG_EventFn controllerCallback);
    ~Viewer(void);

    void draw(void);

    // General view methods
    View* getView(VIEWS viewId);
    void showView(VIEWS viewId);
    void hideView(VIEWS viewId);

    // Menu view methods
    void showMenuView(VIEWS viewId);
    void hideAllMenuViews(void);

};

#endif
