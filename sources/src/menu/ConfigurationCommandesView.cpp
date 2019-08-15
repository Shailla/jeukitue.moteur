
#include <sstream>
#include <iostream>

#include "GL/glew.h"
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

using namespace std;

extern CCfg Config;
extern jkt::CFocus* pFocus;

ConfigurationCommandesView* ConfigurationCommandesView::THIZ = 0;

ConfigurationCommandesView::ConfigurationCommandesView(const AG_EventFn controllerCallback) : View(controllerCallback) {
	THIZ = this;

	_waitingCommandUserChoice = false;
	_commandToWait = NONE;

	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE|AG_WINDOW_MAXIMIZED);
	AG_WindowSetCaption(m_window, "Commandes");

	// Box des commandes
	AG_Scrollview* _scroll = AG_ScrollviewNew(m_window, AG_SCROLLVIEW_EXPAND);
	AG_Box* _boxCommandes = AG_BoxNewVert(_scroll, AG_BOX_EXPAND);


	/* ********************************************* */
	/* Commandes de déplacement                      */
	/* ********************************************* */

	AG_LabelNew(_boxCommandes, 0, " ");	// Saute une ligne
	AG_LabelNew(_boxCommandes, 0, "Deplacements");

	addCommandeButton(AGOBJECT(_boxCommandes), AVANCER, 				"Avancer",					"");
	addCommandeButton(AGOBJECT(_boxCommandes), RECULER, 				"Reculer", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), GAUCHE,  				"Gauche", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), DROITE,  				"Droite", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), MONTER,					"Monter", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), GRAVITY,					"Gravit\u00e9 on/off", 		"");


	/* ********************************************* */
	/* Commandes des armes                           */
	/* ********************************************* */

	AG_LabelNew(_boxCommandes, 0, " ");	// Saute une ligne
	AG_LabelNew(_boxCommandes, 0, "Cam\u00e9ra");
	addCommandeButton(AGOBJECT(_boxCommandes), CAMERA_MONTER,    		"Monter", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), CAMERA_DESCENDRE,   		"Descendre",				"");
	addCommandeButton(AGOBJECT(_boxCommandes), CAMERA_DROITE,    		"Droite", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), CAMERA_GAUCHE,    		"Gauche", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), CAMERA_RECULER,    		"Reculer", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), CAMERA_AVANCER,    		"Avancer", 					"");


	/* ********************************************* */
	/* Commandes des armes                           */
	/* ********************************************* */

	AG_LabelNew(_boxCommandes, 0, " ");	// Saute une ligne
	AG_LabelNew(_boxCommandes, 0, "Armes");
	addCommandeButton(AGOBJECT(_boxCommandes), TIR1,    				"Tir 1", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), TIR2,    				"Tir 2", 					"");
	addCommandeButton(AGOBJECT(_boxCommandes), SELECT_WEAPON_UP,		"Arme suivante", 			"");
	addCommandeButton(AGOBJECT(_boxCommandes), SELECT_WEAPON_DOWN,		"Arme precedente", 			"");


	/* ********************************************* */
	/* Commandes diverses                            */
	/* ********************************************* */

	AG_LabelNew(_boxCommandes, 0, " ");	// Saute une ligne
	AG_LabelNew(_boxCommandes, 0, "Divers");
	addCommandeButton(AGOBJECT(_boxCommandes), TEXTURES,				"Damier textures on/off", 	"");


	AG_SeparatorNewHoriz(m_window);

	// Bouton retour
	AG_Box* box = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(box);
	AG_ButtonNewFn(box, 0, "Retour", m_controllerCallback, "%i", Controller::ShowConfigurationMenuAction);

	AG_WindowSetGeometryMax(m_window);
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
	refreshCommandButton(CAMERA_MONTER);
	refreshCommandButton(CAMERA_DESCENDRE);
	refreshCommandButton(CAMERA_DROITE);
	refreshCommandButton(CAMERA_GAUCHE);
	refreshCommandButton(CAMERA_RECULER);
	refreshCommandButton(CAMERA_AVANCER);
	refreshCommandButton(TIR1);
	refreshCommandButton(TIR2);
	refreshCommandButton(MONTER);
	refreshCommandButton(SELECT_WEAPON_UP);
	refreshCommandButton(SELECT_WEAPON_DOWN);
	refreshCommandButton(GRAVITY);
	refreshCommandButton(TEXTURES);
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
	case CAMERA_MONTER:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.CameraMonter));
		break;
	case CAMERA_DESCENDRE:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.CameraDescendre));
		break;
	case CAMERA_DROITE:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.CameraDroite));
		break;
	case CAMERA_GAUCHE:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.CameraGauche));
		break;
	case CAMERA_RECULER:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.CameraReculer));
		break;
	case CAMERA_AVANCER:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.CameraAvancer));
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
	case GRAVITY:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Gravity));
		break;
	case TEXTURES:
		AG_LabelText(label, Config.Commandes.resolve(Config.Commandes.Textures));
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
		case CAMERA_MONTER:
			setCommande(Config.Commandes.CameraMonter, key, mouse);
			break;
		case CAMERA_DESCENDRE:
			setCommande(Config.Commandes.CameraDescendre, key, mouse);
			break;
		case CAMERA_DROITE:
			setCommande(Config.Commandes.CameraDroite, key, mouse);
			break;
		case CAMERA_GAUCHE:
			setCommande(Config.Commandes.CameraGauche, key, mouse);
			break;
		case CAMERA_RECULER:
			setCommande(Config.Commandes.CameraReculer, key, mouse);
			break;
		case CAMERA_AVANCER:
			setCommande(Config.Commandes.CameraAvancer, key, mouse);
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
		case GRAVITY:
			setCommande(Config.Commandes.Gravity, key, mouse);
			break;
		case TEXTURES:
			setCommande(Config.Commandes.Textures, key, mouse);
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
