#include "Viewer.h"

#include <iostream>

#include <agar/gui/style.h>
#include <agar/core.h>
#include <agar/gui.h>

#include "menu/MainMenuView.h"
#include "menu/AboutView.h"
#include "menu/CentralisateurView.h"
#include "menu/MultijoueursView.h"
#include "menu/ConfigurationView.h"
#include "menu/ConfigCentralisateurView.h"
#include "menu/OpenSceneView.h"
#include "menu/OpenSceneASEView.h"
#include "menu/OpenSceneMapView.h"
#include "menu/OpenSceneASEEcraseRepView.h"
#include "menu/ConsoleAvancementView.h"
#include "menu/LanceServeurView.h"
#include "menu/ConfigurationVideoView.h"
#include "menu/ConfigurationJoueurView.h"
#include "menu/ConsoleView.h"
#include "menu/ProgressBarView.h"

Viewer::Viewer(AG_EventFn controllerCallback) {
	// Initialisation des fenêtres
	_views[MAIN_MENU_VIEW] = new MainMenuView(controllerCallback, this);
	_views[ABOUT_VIEW] = new AboutView(controllerCallback, this);
	_views[MULTIJOUEURS_VIEW] = new MultijoueursView(controllerCallback);
	_views[CENTRALISATEUR_VIEW] = new CentralisateurView(controllerCallback);
	_views[CONFIGURATION_VIEW] = new ConfigurationView(controllerCallback);
	_views[CONFIG_CENTRALISATEUR_VIEW] = new ConfigCentralisateurView(controllerCallback);
	_views[OPEN_SCENE_VIEW] = new OpenSceneView(controllerCallback);
	_views[OPEN_SCENE_ASE_VIEW] = new OpenSceneASEView(controllerCallback);
	_views[OPEN_SCENE_MAP_VIEW] = new OpenSceneMapView(controllerCallback);
	_views[OPEN_SCENE_ASE_ECRASE_REP_VIEW] = new OpenSceneASEEcraseRepView(controllerCallback);
	_views[CONSOLE_AVANCEMENT_VIEW] = new ConsoleAvancementView(controllerCallback);
	_views[LANCE_SERVEUR_VIEW] = new LanceServeurView(controllerCallback);
	_views[CONFIGURATION_VIDEO_VIEW] = new ConfigurationVideoView(controllerCallback);
	_views[CONFIGURATION_JOUEUR_VIEW] = new ConfigurationJoueurView(controllerCallback);
	_views[CONSOLE_VIEW] = new ConsoleView(controllerCallback);
	_views[PROGRESS_BAR_VIEW] = new ProgressBarView(controllerCallback);
}

Viewer::~Viewer(void) {
	map<VIEWS, View*>::iterator iter;

	for(iter = _views.begin() ; iter != _views.end() ; iter++) {
		delete (*iter).second;
	}
}

void Viewer::showView(VIEWS view) {
	// Hide all views but show the requested view
	map<VIEWS, View*>::iterator iter;

	for(iter = _views.begin() ; iter != _views.end() ; iter++) {
		if((*iter).first == view) {
			(*iter).second->show();
		}
		else {
			(*iter).second->hide();
		}
	}
}

void Viewer::hideAllViews(void) {
	// Hide all views but show the requested view
	map<VIEWS, View*>::iterator iter;

	for(iter = _views.begin() ; iter != _views.end() ; iter++) {
		(*iter).second->hide();
	}
}

View* Viewer::getView(VIEWS view) {
	return _views[view];
}

void Viewer::draw(void) {
	AG_Window *win;

    AG_LockVFS(agView);

	//AG_BeginRendering();

    AG_TAILQ_FOREACH(win, &agView->windows, windows) {
        AG_ObjectLock(win);

        if (!win->visible) {
	        AG_ObjectUnlock(win);
	        continue;
        }

        AG_WidgetDraw(win);

        AG_ObjectUnlock(win);
    }

	//AG_EndRendering();

    AG_UnlockVFS(agView);
}


