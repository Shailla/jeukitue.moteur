#include "Viewer.h"

#include <agar/gui/style.h>

#include "MainMenuView.h"
#include "OpenSceneView.h"
#include "OpenSceneASEView.h"
#include "OpenSceneMapView.h"
#include "OpenCentralisateurView.h"

Viewer::Viewer(AG_EventFn controllerCallback)
{
    m_isVisible = false; 

	// Initialisation des styles
	m_defaultWindowStyle = new AG_Style(agStyleDefault);

	// Initialisation des fenêtres
    m_mainMenuView = new MainMenuView(controllerCallback, this);
	m_openCentralisateurView = new OpenCentralisateurView(controllerCallback);
    m_openSceneView = new OpenSceneView(controllerCallback);
	m_openSceneASEView = new OpenSceneASEView(controllerCallback);
	m_openSceneMapView = new OpenSceneMapView(controllerCallback);
}

Viewer::~Viewer(void)
{
    delete m_mainMenuView;
    delete m_openCentralisateurView;
	delete m_defaultWindowStyle;
}

void Viewer::draw(void) {
    if(m_isVisible)
    {
        AG_LockVFS(agView);

        AG_Window *win;

        AG_TAILQ_FOREACH(win, &agView->windows, windows) {
	        AG_ObjectLock(win);
    		
	        if (!win->visible) {
		        AG_ObjectUnlock(win);
		        continue;
	        }

	        AG_WidgetDraw(win);
    	
	        if (!(win->flags & AG_WINDOW_NOUPDATERECT)) {
		        AG_QueueVideoUpdate(AGWIDGET(win)->x, AGWIDGET(win)->y,
		                            AGWIDGET(win)->w, AGWIDGET(win)->h);
	        }
    	
	        AG_ObjectUnlock(win);
        }

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
    m_openCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
}

void Viewer::showOpenCentralisateur(void)
{
    m_mainMenuView->hide();
    m_openCentralisateurView->show();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
}

void Viewer::showOpenScene(void)
{
    m_mainMenuView->hide();
    m_openCentralisateurView->hide();
	m_openSceneView->show();
	m_openSceneASEView->hide();
	m_openSceneMapView->hide();
}

void Viewer::showOpenSceneASE(void)
{
    m_mainMenuView->hide();
    m_openCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->show();
	m_openSceneMapView->hide();
}

void Viewer::showOpenSceneMap(void)
{
    m_mainMenuView->hide();
    m_openCentralisateurView->hide();
	m_openSceneView->hide();
	m_openSceneASEView->hide();
	m_openSceneMapView->show();
}

OpenCentralisateurView* Viewer::getOpenCentralisateurView(void)
{
    return m_openCentralisateurView;
}
