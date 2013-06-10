
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

bool ConfigurationCommandesView::_waitingCommandUserChoice = false;
int ConfigurationCommandesView::_commandToWait = 0;

ConfigurationCommandesView::ConfigurationCommandesView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Commandes");

    // Box Des commandes
	_boxCommandes = AG_BoxNewVert(m_window, 0);
	AG_Expand(_boxCommandes);

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

void ConfigurationCommandesView::addCommande(const COMMANDE_ID commandId, const char* commandeLabel, const char* commandName) {
    AG_Box* boxHoriz = AG_BoxNewHoriz(_boxCommandes, 0);
    AG_BoxSetHomogenous(boxHoriz, 1);
    AG_ExpandHoriz(boxHoriz);

    // Description de la commande
    AG_Box* boxLabel = AG_BoxNewVert(boxHoriz, 0);
    AG_LabelNew(boxLabel, 0, commandeLabel);

    // Action
    AG_Box* boxAction = AG_BoxNewVert(boxHoriz, 0);

    if(_waitingCommandUserChoice && _commandToWait == commandId) {
    	AG_ButtonNew(boxAction, 0, "Choix");
    }
    else {
    	AG_ButtonNewFn(boxAction, 0, "Changer", beginWaitUserCommandChoice, "%i", commandId);
    }

    // Nom technique de la commande (touche du clavier par exemple)
    AG_Box* boxCommande = AG_BoxNewVert(boxHoriz, 0);
	AG_LabelNew(boxCommande, 0, commandName);
}

void ConfigurationCommandesView::show(void) {
	// Réinitialise et affiche la liste des commandes
	AG_ObjectFreeChildren(_boxCommandes);

	addCommande(AVANCER, 	"Avancer",	"rrr"); //Config.Commandes.resolve(Config.Commandes.Avancer));
	addCommande(RECULER, 	"Reculer", 	"rrr"); //Config.Commandes.resolve(Config.Commandes.Reculer));
	addCommande(GAUCHE,  	"Gauche", 	"rrr"); //Config.Commandes.resolve(Config.Commandes.Gauche));
	addCommande(DROITE,  	"Droite", 	"rrr"); //Config.Commandes.resolve(Config.Commandes.Droite));
	addCommande(TIR1,    	"Tir 1", 	"rrr"); //Config.Commandes.resolve(Config.Commandes.Tir1));
	addCommande(TIR2,    	"Tir 2", 	"rrr"); //Config.Commandes.resolve(Config.Commandes.Tir2));
	addCommande(MONTER,		"Monter", 	"rrr"); //Config.Commandes.resolve(Config.Commandes.Monter));

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

bool ConfigurationCommandesView::eventInterceptor(SDL_Event* event) {
	SDLKey key = (SDLKey)0;
	int mouse = 0;
	bool commandAcquired = false;
	bool escapeAcquired = false;

	switch(event->type) {
	case SDL_KEYDOWN:
		if(event->key.keysym.sym != SDLK_ESCAPE) {
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
		_waitingCommandUserChoice = false;
	}

	// Réactualise la fenêtre
	Viewer* viewer = Fabrique::getAgarView();
	viewer->showMenuView(Viewer::CONFIGURATION_COMMANDES_VIEW);

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
	_waitingCommandUserChoice = true;

	pFocus->setEventInterceptor(eventInterceptor);

	// Réactualise la fenêtre
	Viewer* viewer = Fabrique::getAgarView();
	viewer->showView(Viewer::CONFIGURATION_COMMANDES_VIEW);
}
