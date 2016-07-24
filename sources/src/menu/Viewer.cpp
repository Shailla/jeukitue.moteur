#include "Viewer.h"

#include <iostream>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/CentralisateurView.h"
#include "menu/MultijoueursView.h"
#include "menu/MultijoueursClientView.h"
#include "menu/ConfigCentralisateurView.h"
#include "menu/OpenSceneView.h"
#include "menu/OpenSceneASEView.h"
#include "menu/OpenSceneMapView.h"
#include "menu/OpenSceneASEEcraseRepView.h"
#include "menu/LanceServeurView.h"
#include "menu/ConfigurationCommandesView.h"
#include "menu/ConfigurationVideoView.h"
#include "menu/ConfigurationJoueurView.h"
#include "menu/ConfigurationReseauView.h"
#include "menu/ConsoleView.h"
#include "menu/ProgressBarView.h"
#include "menu/PluginsManagementView.h"
#include "menu/DebugMenuView.h"
#include "menu/MapTreeView.h"
#include "menu/DataTreeView.h"
#include "menu/PlayersView.h"
#include "menu/QuestsView.h"
#include "menu/NetworkConnectionsView.h"

Viewer::Viewer(AG_EventFn controllerCallback) {
	// Initialisation des fenêtres
	addMenuView(MULTIJOUEURS_VIEW, new MultijoueursView(controllerCallback));
	addMenuView(MULTIJOUEURS_CLIENT_VIEW, new MultijoueursClientView(controllerCallback));
	addMenuView(CENTRALISATEUR_VIEW, new CentralisateurView(controllerCallback));
	addMenuView(CONFIG_CENTRALISATEUR_VIEW, new ConfigCentralisateurView(controllerCallback));
	addMenuView(OPEN_SCENE_VIEW, new OpenSceneView(controllerCallback));
	addMenuView(PLUGINS_MANAGEMENT_VIEW, new PluginsManagementView(controllerCallback));
	addMenuView(OPEN_SCENE_ASE_VIEW, new OpenSceneASEView(controllerCallback));
	addMenuView(OPEN_SCENE_MAP_VIEW, new OpenSceneMapView(controllerCallback));
	addMenuView(OPEN_SCENE_ASE_ECRASE_REP_VIEW, new OpenSceneASEEcraseRepView(controllerCallback));
	addMenuView(LANCE_SERVEUR_VIEW, new LanceServeurView(controllerCallback));
	addMenuView(CONFIGURATION_COMMANDES_VIEW, new ConfigurationCommandesView(controllerCallback));
	addMenuView(CONFIGURATION_VIDEO_VIEW, new ConfigurationVideoView(controllerCallback));
	addMenuView(CONFIGURATION_JOUEUR_VIEW, new ConfigurationJoueurView(controllerCallback));
	addMenuView(CONFIGURATION_RESEAU_VIEW, new ConfigurationReseauView(controllerCallback));
	addMenuView(PROGRESS_BAR_VIEW, new ProgressBarView(controllerCallback));
	addMenuView(DEBUG_MENU_VIEW, new DebugMenuView(controllerCallback));
	addMenuView(MAP_TREE_VIEW, new MapTreeView(controllerCallback));
	addMenuView(DATA_TREE_VIEW, new DataTreeView(controllerCallback));
	addMenuView(PLAYERS_LIST_VIEW, new PlayersView(controllerCallback));
	addMenuView(NETWORK_CONNECTIONS_VIEW, new NetworkConnectionsView(controllerCallback));

	// Console
	addSimpleView(CONSOLE_VIEW, new ConsoleView(controllerCallback));

	// Fenêtre des quêtes
	addSimpleView(QUESTS_VIEW, new QuestsView(controllerCallback));
}

void Viewer::addMenuView(VIEWS viewId, View* view) {
	_views[viewId] = view;
	_menuViews[viewId] = view;
}

void Viewer::addSimpleView(VIEWS viewId, View* view) {
	_views[viewId] = view;
}

Viewer::~Viewer(void) {
	map<VIEWS, View*>::iterator iter;

	for(iter = _views.begin() ; iter != _views.end() ; iter++) {
		delete (*iter).second;
	}
}

View* Viewer::getView(VIEWS viewId) {
	map<VIEWS, View*>::iterator iter = _views.find(viewId);

	if(iter != _views.end()) {
		return iter->second;
	}
	else {
		return 0;
	}
}

void Viewer::showView(VIEWS viewId) {
	map<VIEWS, View*>::iterator iter = _views.find(viewId);

	if(iter != _views.end()) {
		iter->second->show();
	}
}

void Viewer::hideView(VIEWS viewId) {
	map<VIEWS, View*>::iterator iter = _views.find(viewId);

	if(iter != _views.end()) {
		iter->second->hide();
	}
}

void Viewer::showMenuView(VIEWS viewId) {
	// Hide all views but show the requested view
	map<VIEWS, View*>::iterator iter;

	for(iter = _menuViews.begin() ; iter != _menuViews.end() ; iter++) {
		if((*iter).first == viewId) {
			(*iter).second->show();
		}
		else {
			(*iter).second->hide();
		}
	}
}

void Viewer::hideAllMenuViews(void) {
	// Hide all views but show the requested view
	map<VIEWS, View*>::iterator iter;

	for(iter = _menuViews.begin() ; iter != _menuViews.end() ; iter++) {
		(*iter).second->hide();
	}
}

void Viewer::draw(void) {
	AG_Window *win;

    AG_LockVFS(&agDrivers);

    AG_FOREACH_WINDOW(win, agDriverSw) {
        if(!win->visible) {
        	continue;
        }

        AG_ObjectLock(win);
        AG_WindowDraw(win);
        AG_ObjectUnlock(win);
    }

    AG_UnlockVFS(&agDrivers);
}


