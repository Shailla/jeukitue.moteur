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
class OpenSceneASEEcraseRepView;
class ConsoleAvancementView;
class LanceServeurView;
class ConfigurationVideoView;
class ConfigurationJoueurView;
class ConsoleView;

class Viewer {
	// Fenêtres
    View* _mainMenuView;
	CentralisateurView* _centralisateurView;
	View* _multijoueursView;
	View* _configurationView;
    ConfigCentralisateurView* _configCentralisateurView;
	View* _openSceneView;
	OpenSceneASEView* _openSceneASEView;
	OpenSceneMapView* _openSceneMapView;
	OpenSceneASEEcraseRepView* _openSceneAseEcraseRepView;
	ConsoleAvancementView* _consoleAvancementView;
	LanceServeurView* _lanceServeurView;
	ConfigurationVideoView* _configurationVideoView;
	ConfigurationJoueurView* _configurationJoueurView;
	ConsoleView* _consoleView;

	void showOrHide(View* view, View* viewToShow);
	void showView(View* view);
public:
    Viewer(AG_EventFn controllerCallback);
    ~Viewer(void);
    void draw(void);

	void hideAll(void);
    void showMainMenu(void);
	void showMultijoueurs(void);
	void showCentralisateur(void);
	void showConfiguration(void);
    void showConfigCentralisateur(void);
	void showOpenScene(void);
	void showOpenSceneASE(void);
	void showOpenSceneASEEcraseRep(void);
	void showOpenSceneMap(void);
	void showConsoleAvancementView(void);
	void showLanceServeurView(void);
	void showConfigurationVideoView(void);
	void showConfigurationJoueurView(void);
	
	void showConsoleView(void);
	void hideConsoleView(void);

	// Accesseurs des fenêtres
    ConfigCentralisateurView* getConfigCentralisateurView(void);
	OpenSceneMapView* getOpenSceneMapView(void);
	OpenSceneASEView* getOpenSceneASEView(void);
	CentralisateurView* getCentralisateurView(void);
	OpenSceneASEEcraseRepView* getOpenSceneASEEcraseRepView(void);
	ConsoleAvancementView* getConsoleAvancementView(void);
	LanceServeurView* getLanceServeurView(void);
	ConsoleView* getConsoleView(void);
	ConfigurationJoueurView* getConfigurationJoueurView(void);
};

#endif
