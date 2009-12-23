#include "AgarView.h"

AgarView::AgarView(AG_EventFn controllerCallback)
{
    m_isVisible = false;
    m_mainMenuView = new MainMenuView(controllerCallback);
    m_openCentralisateurView = new OpenCentralisateurView(controllerCallback);
}

AgarView::~AgarView(void)
{
    delete m_mainMenuView;
    delete m_openCentralisateurView;
}

void AgarView::draw(void) {
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

bool AgarView::IsVisible(void)
{
    return m_isVisible;
}

void AgarView::SetVisible(const bool isVisible)
{
    m_isVisible = isVisible;
}

void AgarView::showMainMenu(void)
{
    m_mainMenuView->show();
    m_openCentralisateurView->hide();
}

void AgarView::showOpenCentralisateur(void)
{
    m_mainMenuView->hide();
    m_openCentralisateurView->show();
}

OpenCentralisateurView* AgarView::getOpenCentralisateurView(void)
{
    return m_openCentralisateurView;
}
