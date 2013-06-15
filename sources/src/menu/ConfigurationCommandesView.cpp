
#include <sstream>
#include <iostream>

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
    AG_Scrollview* scroll = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_NOPAN_X);
    AG_Expand(scroll);

	_boxCommandes = AG_BoxNewVert(scroll, 0);
	AG_Expand(_boxCommandes);

	// Initialise la la liste des commandes
	addCommande(AGOBJECT(_boxCommandes), AVANCER, 				"Avancer",			"");
	addCommande(AGOBJECT(_boxCommandes), RECULER, 				"Reculer", 			"");
	addCommande(AGOBJECT(_boxCommandes), GAUCHE,  				"Gauche", 			"");
	addCommande(AGOBJECT(_boxCommandes), DROITE,  				"Droite", 			"");
	addCommande(AGOBJECT(_boxCommandes), TIR1,    				"Tir 1", 			"");
	addCommande(AGOBJECT(_boxCommandes), TIR2,    				"Tir 2", 			"");
	addCommande(AGOBJECT(_boxCommandes), MONTER,				"Monter", 			"");
	addCommande(AGOBJECT(_boxCommandes), SELECT_WEAPON_UP,		"Arme suivante", 	"");
	addCommande(AGOBJECT(_boxCommandes), SELECT_WEAPON_DOWN,	"Arme precedente", 	"");

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

void ConfigurationCommandesView::addCommande(AG_Object* parent, const COMMANDE_ID commandId, const char* commandeLabel, const char* commandName) {
    AG_Box* boxHoriz = AG_BoxNewHoriz(parent, 0);
    AG_BoxSetHomogenous(boxHoriz, 1);
    AG_ExpandHoriz(boxHoriz);

    // Description de la commande
    AG_Box* boxLabel = AG_BoxNewVert(boxHoriz, 0);
    AG_Expand(boxLabel);
    AG_LabelNew(boxLabel, 0, commandeLabel);

    // Action
    AG_Box* boxAction = AG_BoxNewVert(boxHoriz, 0);
    AG_Expand(boxAction);
   	AG_Button* button = AG_ButtonNewFn(boxAction, 0, "Changer", m_controllerCallback, "%i,%i", Controller::WaitUserCommandChoice, commandId);
   	_buttons[commandId] = button;

    // Nom technique de la commande (touche du clavier par exemple)
    AG_Box* boxCommande = AG_BoxNewVert(boxHoriz, 0);
    AG_Expand(boxCommande);
	AG_Label* label = AG_LabelNew(boxCommande, 0, commandName);
	_labels[commandId] = label;
}

void ConfigurationCommandesView::show(void) {
	refresh();

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);
	View::show();
}

void ConfigurationCommandesView::refresh(void) {
	updateCommande(AVANCER);
	updateCommande(RECULER);
	updateCommande(GAUCHE);
	updateCommande(DROITE);
	updateCommande(TIR1);
	updateCommande(TIR2);
	updateCommande(MONTER);
	updateCommande(SELECT_WEAPON_UP);
	updateCommande(SELECT_WEAPON_DOWN);
}

void ConfigurationCommandesView::updateCommande(COMMANDE_ID commandId) {
	// Mise à jour du bouton
	AG_Button* button = _buttons[commandId];

	if(_waitingCommandUserChoice && (_commandToWait == commandId)) {
		AG_ButtonText(button, "Choix en cours");
	}
	else if(_waitingCommandUserChoice) {
		AG_ButtonText(button, "-");
	}
	else {
		AG_ButtonText(button, "Changer");
	}
	AG_ExpandHoriz(button);

	// Mise à jour du label
	AG_Label* label = _labels[commandId];

	switch(commandId) {
	case AVANCER:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Avancer));
		break;
	case RECULER:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Reculer));
		break;
	case GAUCHE:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Gauche));
		break;
	case DROITE:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Droite));
		break;
	case TIR1:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Tir1));
		break;
	case TIR2:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Tir2));
		break;
	case MONTER:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Monter));
		break;
	case SELECT_WEAPON_UP:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.SelectWeaponUp));
		break;
	case SELECT_WEAPON_DOWN:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.SelectWeaponDown));
		break;
	default:
		cerr << endl << "Commande inconnue '" << commandId << "'";
		break;
	}

	AG_ExpandHoriz(label);
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
			setCommande(Config.Commandes.Avancer, key, mouse);
			break;
		case RECULER:
			setCommande(Config.Commandes.Reculer, key, mouse);
			break;
		case GAUCHE:
			setCommande(Config.Commandes.Gauche, key, mouse);
			break;
		case DROITE:
			setCommande(Config.Commandes.Droite, key, mouse);
			break;
		case TIR1:
			setCommande(Config.Commandes.Tir1, key, mouse);
			break;
		case TIR2:
			setCommande(Config.Commandes.Tir2, key, mouse);
			break;
		case MONTER:
			setCommande(Config.Commandes.Monter, key, mouse);
			break;
		case SELECT_WEAPON_UP:
			setCommande(Config.Commandes.SelectWeaponUp, key, mouse);
			break;
		case SELECT_WEAPON_DOWN:
			setCommande(Config.Commandes.SelectWeaponDown, key, mouse);
			break;
		}
	}

	if(commandAcquired || escapeAcquired) {
		pFocus->removeEventInterceptor();
		_waitingCommandUserChoice = false;
		_commandToWait = 0;
	}

	Viewer* viewer = Fabrique::getAgarView();
	viewer->getView(Viewer::CONFIGURATION_COMMANDES_VIEW)->show();

	return commandAcquired || escapeAcquired;	// Indique si l'événement a été consommé ou non
}

void ConfigurationCommandesView::setCommande(CCfg::CComID& commandeToUpdate, const SDLKey& key, const int& mouse) {
	commandeToUpdate.key = key;
	commandeToUpdate.mouse = mouse;
	Config.Ecrit();	// Sauvegarde la configuration
}

void ConfigurationCommandesView::beginWaitUserCommandChoice(int commandId) {
	_commandToWait = commandId;
	_waitingCommandUserChoice = true;

	pFocus->setEventInterceptor(eventInterceptor);
}
