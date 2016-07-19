
#include <sstream>
#include <iostream>

using namespace std;

#include "SDL.h"

#include <agar/core.h>
#include <agar/gui.h>

#include "util/Trace.h"
#include "main/Focus.h"
#include "main/Fabrique.h"
#include "menu/Viewer.h"
#include "menu/View.h"
#include "menu/Controller.h"
#include "main/Cfg.h"

#include "menu/ConfigurationCommandesView.h"

extern CCfg Config;
extern JktMenu::CFocus* pFocus;

ConfigurationCommandesView* ConfigurationCommandesView::THIZ = 0;

ConfigurationCommandesView::ConfigurationCommandesView(const AG_EventFn controllerCallback) : View(controllerCallback) {
	THIZ = this;

	_waitingCommandUserChoice = false;
	_commandToWait = NONE;

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE|AG_WINDOW_MAXIMIZED);
	AG_WindowSetCaption(m_window, "Commandes");

	// Box Des commandes
	AG_Box* _boxCommandes = AG_BoxNewVert(m_window, AG_BOX_EXPAND);


	/* ********************************************* */
	/* Commandes de déplacement                      */
	/* ********************************************* */

	AG_LabelNew(_boxCommandes, 0, " ");	// Saute une ligne
	AG_LabelNew(_boxCommandes, 0, "Deplacements");

	addCommandeButton(AGOBJECT(_boxCommandes), AVANCER, 				"Avancer",			"");
	addCommandeButton(AGOBJECT(_boxCommandes), RECULER, 				"Reculer", 			"");
	addCommandeButton(AGOBJECT(_boxCommandes), GAUCHE,  				"Gauche", 			"");
	addCommandeButton(AGOBJECT(_boxCommandes), DROITE,  				"Droite", 			"");
	addCommandeButton(AGOBJECT(_boxCommandes), MONTER,				"Monter", 			"");


	/* ********************************************* */
	/* Commandes des armes                           */
	/* ********************************************* */

	AG_LabelNew(_boxCommandes, 0, " ");	// Saute une ligne
	AG_LabelNew(_boxCommandes, 0, "Armes");
	addCommandeButton(AGOBJECT(_boxCommandes), TIR1,    				"Tir 1", 			"");
	addCommandeButton(AGOBJECT(_boxCommandes), TIR2,    				"Tir 2", 			"");
	addCommandeButton(AGOBJECT(_boxCommandes), SELECT_WEAPON_UP,		"Arme suivante", 	"");
	addCommandeButton(AGOBJECT(_boxCommandes), SELECT_WEAPON_DOWN,	"Arme precedente", 	"");


	AG_SeparatorNewHoriz(m_window);

	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", m_controllerCallback, "%i", Controller::ShowConfigurationMenuAction);

	// Bouton appliquer
	AG_ButtonNewFn(box, 0, "Appliquer", m_controllerCallback, "%i", Controller::ShowConfigurationVideoViewAction);

	//	AG_WindowSetGeometryMax(m_window);
	AG_WindowShow(m_window);

	hide();
}

ConfigurationCommandesView::~ConfigurationCommandesView(void) {
}

void ConfigurationCommandesView::addCommandeButton(AG_Object* parent, const COMMANDE_ID commandId, const char* commandeLabel, const char* commandName) {
	AG_Box* boxHoriz = AG_BoxNewHoriz(parent, AG_BOX_HFILL | AG_BOX_HOMOGENOUS | AG_BOX_FRAME);

	// Description de la commande
	AG_Label* description = AG_LabelNew(boxHoriz, 0, commandeLabel);
	AG_LabelJustify(description, AG_TEXT_RIGHT);
	AG_Expand(description);

	// Action
	AG_Button* button = AG_ButtonNewFn(boxHoriz, 0, "Changer", m_controllerCallback, "%i,%i", Controller::WaitUserCommandChoice, commandId);
	AG_Expand(button);
	_buttons[commandId] = button;

	// Nom technique de la commande (touche du clavier par exemple)
	AG_Label* commande = AG_LabelNew(boxHoriz, 0, commandName);
	AG_LabelJustify(commande, AG_TEXT_CENTER);
	AG_Expand(commande);
	_labels[commandId] = commande;
}

void ConfigurationCommandesView::show(void) {
	refresh();

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);
	View::show();
}

void ConfigurationCommandesView::refresh(void) {
	refreshCommandButton(AVANCER);
	refreshCommandButton(RECULER);
	refreshCommandButton(GAUCHE);
	refreshCommandButton(DROITE);
	refreshCommandButton(TIR1);
	refreshCommandButton(TIR2);
	refreshCommandButton(MONTER);
	refreshCommandButton(SELECT_WEAPON_UP);
	refreshCommandButton(SELECT_WEAPON_DOWN);
}

int varrrrrrrrrrrrrrrr = 0;

void ConfigurationCommandesView::refreshCommandButton(COMMANDE_ID commandId) {
	// Mise à jour du bouton
	AG_Button* button = _buttons[commandId];

	if(_waitingCommandUserChoice && (_commandToWait == commandId)) {
		AG_ButtonTextS(button, "Choix en cours");
	}
	else {
		AG_ButtonTextS(button, "Changer");
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
		LOGERROR(("Commande inconnue %d", _commandToWait));
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
		switch(THIZ->_commandToWait) {
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
		case NONE:
		default:
			break;
		}
	}

	if(commandAcquired || escapeAcquired) {
		// Mise à jour du bouton
		AG_Button* button = THIZ->_buttons[THIZ->_commandToWait];
		AG_ButtonTextS(button, "Changer");

		// Met fin à l'interception des événements
		pFocus->setEventInterceptor(0);
		THIZ->_waitingCommandUserChoice = false;
		THIZ->_commandToWait = NONE;
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
	THIZ->_commandToWait = (COMMANDE_ID)commandId;
	THIZ->_waitingCommandUserChoice = true;

	// Mise à jour du bouton
	AG_Button* button = THIZ->_buttons[THIZ->_commandToWait];
	AG_ButtonTextS(button, "Choix en cours");

	pFocus->setEventInterceptor(eventInterceptor);
}
