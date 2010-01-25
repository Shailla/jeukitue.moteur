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

Viewer::Viewer(AG_EventFn controllerCallback)
{
    m_isVisible = false;

	// Initialisation des styles
	m_defaultWindowStyle = new AG_Style(agStyleDefault);

	// Initialisation des fenêtres
    m_mainMenuView = new MainMenuView(controllerCallback, this);
	m_multijoueursView = new MultijoueursView(controllerCallback);
	m_centralisateurView = new CentralisateurView(controllerCallback);
	m_configurationView = new ConfigurationView(controllerCallback);
	m_configCentralisateurView = new ConfigCentralisateurView(controllerCallback);
    m_openSceneView = new OpenSceneView(controllerCallback);
	m_openSceneASEView = new OpenSceneASEView(controllerCallback);
	m_openSceneMapView = new OpenSceneMapView(controllerCallback);
	_openSceneAseEcraseRepView = new OpenSceneASEEcraseRepView(controllerCallback);
}

Viewer::~Viewer(void)
{
    delete m_mainMenuView;
    delete m_multijoueursView;
	delete m_configurationView;
	delete m_configCentralisateurView;
	delete m_openSceneView;
	delete m_openSceneASEView;
	delete m_openSceneMapView;
	delete _openSceneAseEcraseRepView;
}

void Viewer::draw(void) {
    if(m_isVisible)
    {
		AG_Window *win;

        AG_LockVFS(agView);

		AG_BeginRendering();

        AG_TAILQ_FOREACH(win, &agView->windows, windows) {
	        AG_ObjectLock(win);

	        if (!win->visible) {
		        AG_ObjectUnlock(win);
		        continue;
	        }

	        AG_WidgetDraw(win);

	        AG_ObjectUnlock(win);
        }

		AG_EndRendering();

        AG_UnlockVFS(agView);
    }
}

AG_Style* Viewer::getDefaultWindowStyle()
{
	return m_defaultWindowStyle;
}

bool Viewer::IsVisible(void)
{
    return m_isVisible;
}

void Viewer::SetVisible(const bool isVisible)
{
    m_isVisible = isVisible;
}

void Viewer::showMainMenu(void)
{
    m_mainMenuView->show();
	m_centralisateurView->hide();
	m_multijoueursView->hide();
	m_configurationView->hide();
    m_configCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->hide();
}

void Viewer::showMultijoueurs(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->hide();
	m_multijoueursView->show();
	m_configurationView->hide();
    m_configCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->hide();
}

void Viewer::showCentralisateur(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->show();
	m_multijoueursView->hide();
	m_configurationView->hide();
    m_configCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->hide();
}

void Viewer::showConfiguration(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->hide();
	m_multijoueursView->hide();
	m_configurationView->show();
	m_configCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->hide();
}


void Viewer::showConfigCentralisateur(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->hide();
	m_multijoueursView->hide();
	m_configurationView->hide();
    m_configCentralisateurView->show();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->hide();
}

void Viewer::showOpenScene(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->hide();
	m_multijoueursView->hide();
	m_configurationView->hide();
    m_configCentralisateurView->hide();
	m_openSceneView->show();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->hide();
}

void Viewer::showOpenSceneASE(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->hide();
	m_multijoueursView->hide();
	m_configurationView->hide();
    m_configCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->show();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->hide();
}

void Viewer::showOpenSceneASEEcraseRep(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->hide();
	m_multijoueursView->hide();
	m_configurationView->hide();
    m_configCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
	_openSceneAseEcraseRepView->show();
}

void Viewer::showOpenSceneMap(void)
{
    m_mainMenuView->hide();
	m_centralisateurView->hide();
	m_multijoueursView->hide();
	m_configurationView->hide();
    m_configCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->show();
	_openSceneAseEcraseRepView->hide();
}

ConfigCentralisateurView* Viewer::getConfigCentralisateurView(void)
{
    return m_configCentralisateurView;
}

OpenSceneMapView* Viewer::getOpenSceneMapView(void)
{
    return m_openSceneMapView;
}

OpenSceneASEView* Viewer::getOpenSceneASEView(void)
{
    return m_openSceneASEView;
}

CentralisateurView* Viewer::getCentralisateurView(void)
{
    return m_centralisateurView;
}
