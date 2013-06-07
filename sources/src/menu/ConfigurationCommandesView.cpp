
#include <sstream>

using namespace std;

#include "SDL.h"

#include <agar/core.h>
#include <agar/gui.h>

#include "main/Focus.h"
#include "main/Fabrique.h"
#include "menu/Viewer.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "main/Cfg.h"

#include "menu/ConfigurationCommandesView.h"

extern CCfg Config;
extern JktMenu::CFocus* pFocus;

int ConfigurationCommandesView::_commandToWait = 0;

ConfigurationCommandesView::ConfigurationCommandesView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Commandes");

    // Avancer
    AG_Box* boxAvancer = AG_BoxNewHoriz(m_window, 0);
    AG_ButtonNewFn(boxAvancer, 0, "Avancer", beginWaitUserCommandChoice, "%i", ConfigurationCommandesView::AVANCER);
	_labelAvancer = AG_LabelNew(boxAvancer, 0, "   ");

    // Reculer
	AG_Box* boxReculer = AG_BoxNewHoriz(m_window, 0);
    AG_ButtonNewFn(boxReculer, 0, "Reculer", beginWaitUserCommandChoice, "%i", ConfigurationCommandesView::RECULER);
    _labelReculer = AG_LabelNew(boxReculer, 0, "   ");

    // Gauche
    AG_Box* boxGauche = AG_BoxNewHoriz(m_window, 0);
    AG_ButtonNewFn(boxGauche, 0, "Gauche", beginWaitUserCommandChoice, "%i", ConfigurationCommandesView::GAUCHE);
    _labelGauche = AG_LabelNew(boxGauche, 0, "   ");

    // Droite
    AG_Box* boxDroite = AG_BoxNewHoriz(m_window, 0);
    AG_ButtonNewFn(boxDroite, 0, "Droite", beginWaitUserCommandChoice, "%i", ConfigurationCommandesView::DROITE);
    _labelDroite = AG_LabelNew(boxDroite, 0, "   ");

    // Tir1
    AG_Box* boxTir1 = AG_BoxNewHoriz(m_window, 0);
    AG_ButtonNewFn(boxTir1, 0, "Tir1", beginWaitUserCommandChoice, "%i", ConfigurationCommandesView::TIR1);
    _labelTir1 = AG_LabelNew(boxTir1, 0, "   ");

    // Tir2
    AG_Box* boxTir2 = AG_BoxNewHoriz(m_window, 0);
    AG_ButtonNewFn(boxTir2, 0, "Tir2", beginWaitUserCommandChoice, "%i", ConfigurationCommandesView::TIR2);
    _labelTir2 = AG_LabelNew(boxTir2, 0, "   ");

    // Monter
    AG_Box* boxMonter = AG_BoxNewHoriz(m_window, 0);
    AG_ButtonNewFn(boxMonter, 0, "Monter", beginWaitUserCommandChoice, "%i", ConfigurationCommandesView::MONTER);
    _labelMonter = AG_LabelNew(boxMonter, 0, "   ");

    AG_SeparatorNewHoriz(m_window);
	
	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", m_controllerCallback, "%i", Controller::ShowConfigurationMenuAction);
    
	// Bouton appliquer
	AG_ButtonNewFn(box, 0, "Appliquer", m_controllerCallback, "%i", Controller::ShowConfigurationVideoViewAction);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    hide();
}

ConfigurationCommandesView::~ConfigurationCommandesView(void) {
}

void ConfigurationCommandesView::show(void) {
	 AG_LabelText(_labelAvancer, Config.Commandes.resolve(Config.Commandes.Avancer));
	 AG_LabelText(_labelReculer, Config.Commandes.resolve(Config.Commandes.Reculer));
	 AG_LabelText(_labelGauche, Config.Commandes.resolve(Config.Commandes.Gauche));
	 AG_LabelText(_labelDroite, Config.Commandes.resolve(Config.Commandes.Droite));
	 AG_LabelText(_labelTir1, Config.Commandes.resolve(Config.Commandes.Tir1));
	 AG_LabelText(_labelTir2, Config.Commandes.resolve(Config.Commandes.Tir2));
	 AG_LabelText(_labelMonter, Config.Commandes.resolve(Config.Commandes.Monter));

	View::show();
}

bool ConfigurationCommandesView::eventInterceptor(SDL_Event* event) {
	SDLKey key = (SDLKey)0;
	int mouse = 0;
	bool commandAcquired = false;
	bool escapeAcquired = false;

	switch(event->type) {
	case SDL_KEYDOWN:
		if(event->key.keysym.sym!=SDLK_ESCAPE) {
			key = event->key.keysym.sym;
			mouse = 0;
			commandAcquired = true;
		}
		else {
			escapeAcquired = true;
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		key = (SDLKey)0;
		mouse = event->button.button;
		commandAcquired = true;
		break;
	}

	if(commandAcquired) {
		switch(_commandToWait) {
		case AVANCER:
			updateCommande(Config.Commandes.Avancer, key, mouse);
			break;
		case RECULER:
			updateCommande(Config.Commandes.Reculer, key, mouse);
			break;
		case GAUCHE:
			updateCommande(Config.Commandes.Gauche, key, mouse);
			break;
		case DROITE:
			updateCommande(Config.Commandes.Droite, key, mouse);
			break;
		case TIR1:
			updateCommande(Config.Commandes.Tir1, key, mouse);
			break;
		case TIR2:
			updateCommande(Config.Commandes.Tir2, key, mouse);
			break;
		case MONTER:
			updateCommande(Config.Commandes.Monter, key, mouse);
			break;
		}
	}

	if(commandAcquired || escapeAcquired) {
		pFocus->removeEventInterceptor();
		Viewer* viewer = Fabrique::getAgarView();
		viewer->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);
	}

	return commandAcquired || escapeAcquired;	// Indique si l'événement a été consommé ou non
}

void ConfigurationCommandesView::updateCommande(CCfg::CComID& commandeToUpdate, const SDLKey& key, const int& mouse) {
	commandeToUpdate.key = key;
	commandeToUpdate.mouse = mouse;
	Config.Ecrit();	// Sauvegarde la configuration
}

void ConfigurationCommandesView::beginWaitUserCommandChoice(AG_Event* event) {
	int commandeId = AG_INT(1);

	_commandToWait = commandeId;

	pFocus->setEventInterceptor(eventInterceptor);
}
