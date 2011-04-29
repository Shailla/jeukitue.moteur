#include "Viewer.h"

#include <iostream>

#include <agar/gui/style.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/view.h>

#include "menu/MainMenuView.h"
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
    _mainMenuView = new MainMenuView(controllerCallback, this);
	_multijoueursView = new MultijoueursView(controllerCallback);
	_centralisateurView = new CentralisateurView(controllerCallback);
	_configurationView = new ConfigurationView(controllerCallback);
	_configCentralisateurView = new ConfigCentralisateurView(controllerCallback);
    _openSceneView = new OpenSceneView(controllerCallback);
	_openSceneASEView = new OpenSceneASEView(controllerCallback);
	_openSceneMapView = new OpenSceneMapView(controllerCallback);
	_openSceneAseEcraseRepView = new OpenSceneASEEcraseRepView(controllerCallback);
	_consoleAvancementView = new ConsoleAvancementView(controllerCallback);
	_lanceServeurView = new LanceServeurView(controllerCallback);
	_configurationVideoView = new ConfigurationVideoView(controllerCallback);
	_configurationJoueurView = new ConfigurationJoueurView(controllerCallback);
	_consoleView = new ConsoleView(controllerCallback);
	_progressBarView = new ProgressBarView(controllerCallback);
}

Viewer::~Viewer(void) {
    delete _mainMenuView;
    delete _multijoueursView;
	delete _configurationView;
	delete _configCentralisateurView;
	delete _openSceneView;
	delete _openSceneASEView;
	delete _openSceneMapView;
	delete _openSceneAseEcraseRepView;
	delete _consoleAvancementView;
	delete _lanceServeurView;
	delete _configurationVideoView;
	delete _configurationJoueurView;
	delete _consoleView;
	delete _progressBarView;
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

void Viewer::hideAll(void) {
	showView(NULL);
}

void Viewer::showOrHide(View* view, View* viewToShow) {
	if(view == viewToShow) {
		view->show();
	}
	else {
		view->hide();
	}
}

void Viewer::showView(View* view) {
	showOrHide(_mainMenuView, view);
	showOrHide(_centralisateurView, view);
	showOrHide(_multijoueursView, view);
	showOrHide(_configurationView, view);
    showOrHide(_configCentralisateurView, view);
	showOrHide(_openSceneView, view);
	showOrHide(_openSceneASEView, view);
	showOrHide(_openSceneMapView, view);
	showOrHide(_openSceneAseEcraseRepView, view);
	showOrHide(_consoleAvancementView, view);
	showOrHide(_lanceServeurView, view);
	showOrHide(_configurationVideoView, view);
	showOrHide(_configurationJoueurView, view);
	showOrHide(_progressBarView, view);
}

void Viewer::showMainMenu(void) {
    showView(_mainMenuView);
}

void Viewer::showMultijoueurs(void) {
	showView(_multijoueursView);
}

void Viewer::showConsoleView(void) {
	_consoleView->show();
}

void Viewer::hideConsoleView(void) {
	_consoleView->hide();
}

void Viewer::showCentralisateur(void) {
	showView(_centralisateurView);
}

void Viewer::showConfiguration(void) {
	showView(_configurationView);
}

void Viewer::showConfigCentralisateur(void) {
    showView(_configCentralisateurView);
}

void Viewer::showOpenScene(void) {
	showView(_openSceneView);
}

void Viewer::showOpenSceneASE(void) {
	showView(_openSceneASEView);
}

void Viewer::showOpenSceneASEEcraseRep(void) {
	showView(_openSceneAseEcraseRepView);
}

void Viewer::showOpenSceneMap(void) {
	showView(_openSceneMapView);
}

void Viewer::showConsoleAvancementView(void) {
	showView(_consoleAvancementView);
}

void Viewer::showLanceServeurView(void) {
	showView(_lanceServeurView);
}

void Viewer::showConfigurationVideoView(void) {
	showView(_configurationVideoView);
}

void Viewer::showConfigurationJoueurView(void) {
	showView(_configurationJoueurView);
}

ConfigCentralisateurView* Viewer::getConfigCentralisateurView(void) {
    return _configCentralisateurView;
}

OpenSceneMapView* Viewer::getOpenSceneMapView(void) {
    return _openSceneMapView;
}

OpenSceneASEView* Viewer::getOpenSceneASEView(void) {
    return _openSceneASEView;
}

CentralisateurView* Viewer::getCentralisateurView(void) {
    return _centralisateurView;
}

ProgressBarView* Viewer::getProgressBarView(void) {
    return _progressBarView;
}

OpenSceneASEEcraseRepView* Viewer::getOpenSceneASEEcraseRepView(void) {
    return _openSceneAseEcraseRepView;
}

ConsoleAvancementView* Viewer::getConsoleAvancementView(void) {
    return _consoleAvancementView;
}

LanceServeurView* Viewer::getLanceServeurView(void) {
    return _lanceServeurView;
}

ConsoleView* Viewer::getConsoleView(void) {
    return _consoleView;
}

ConfigurationJoueurView* Viewer::getConfigurationJoueurView(void) {
    return _configurationJoueurView;
}

