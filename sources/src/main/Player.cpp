#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <map>
#include <cmath>

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

#include <fmod.h>
#include <SDL.h>

class CGame;

#include "Constantes.h"
#include "main/Fabrique.h"
#include "util/GLUtils.h"
#include "util/math_vectoriel.h"
#include "util/Trace.h"
#include "util/V3D.h"
#include "util/Tableau.cpp"
#include "spatial/widget/Icone.h"
#include "main/Laser.h"
#include "main/Rocket.h"
#include "main/Projectil.h"
#include "main/Clavier.h"
#include "spatial/Map.h"
#include "spatial/geo/EntryPoint.h"
#include "son/DemonSons.h"
#include "son/ReqSon.h"
#include "util/Erreur.h"
#include "spatial/materiau/Textures.h"
#include "main/Cfg.h"
#include "main/Game.h"
#include "reseau/SPA.h"

#include "main/Player.h"

using namespace std;
using namespace jkt;

extern CCfg Config;
extern CDemonSons *demonSons;
extern CGame Game;

Icone* CPlayer::_weaponsChoice = NULL;

CPlayer::CPlayer() {
	_spa = 0;

	_armeActif = 0;			// Pas d'arme active
	_nbrArmes = 3;			// Nombre d'armes

	_position[0] = 0.0f;	// Position d'origine
	_position[1] = 0.0f;
	_position[2] = 0.0f;

	_vitesse[0] = 0.0f;		// Vitesse nulle
	_vitesse[1] = 0.0f;
	_vitesse[2] = 0.0f;

	_teta = -90.0f;			// Orientation d'origine
	_phi = 0.0f;

	_posVue[0] = 0.0f;		// Position du point de vue par rapport au joueur
	_posVue[1] = 0.1f;
	_posVue[2] = 0.137f;

	_rayonSolidbox = 0.30f;	// Rayon de la sphère qui représente le joueur dans la amp. Le joueur mesure arbitrairement 1m70, son rayon la moitier

	_pClavier = 0;
	_contactFunc = 0;		// Pas d'action à réaliser lors d'un contact avec la map

	_pSkin = 0;				// Pas de skin associé par défaut

	_cri = 0;				// Le cri du joueur n'a pas encore été chargé
	_criRequest = 0;			// Y'a pas encore la requête sur le cri non plus

	createClavier();		// Crée la classe qui gère les requêtes de mouvement, tir ...

	if(!_weaponsChoice) {
		try {
			Uint8* pixelsWeaponsChoice = jkt::litFichierImage("@Icone/Armes.bmp", 0.75f);
			_weaponsChoice = jkt::litFichierIcone("@Icone/Armes.bmp", 0.75f, pixelsWeaponsChoice);
		}
		catch(CErreur& erreur) {
			LOGDEBUG(("CPlayer::CPlayer() Echec de lecture de texture d'icone des armes%T", this));
			_weaponsChoice = 0;
		}
	}
}

void CPlayer::setId(int id) {
	_id = id;
}

int CPlayer::getId() const {
	return _id;
}

bool CPlayer::openInClientMode(const IPaddress &address) {				// Ouverture en mode client
	_spa = new jkt::CSPA();
	return _spa->openInClientMode(address);
}

void CPlayer::close() {			// Ouverture en mode client
	if(_spa)
		delete _spa;
	_spa = 0;
}

void CPlayer::afficheIconesArmes() {
	if(_weaponsChoice != NULL) {
		float X = (float)Config.Display.X;
		float Y = (float)Config.Display.Y/2;

		// Affiche les ic�nes des armes
		_weaponsChoice->affiche(X-50.0f, X, Y-(_nbrArmes*50/2), Y+(_nbrArmes*50/2));

		// Affichage du focus sur l'arme active
		glDisable( GL_TEXTURE_2D );
		glDepthMask( GL_TRUE );
		glDisable( GL_BLEND );

		glLineWidth( 4 );
		glColor3f( 1.0f, 0.0f, 0.0f );

		glBegin( GL_LINE_LOOP );
		glVertex2f( X-50.0f,	Y - (_nbrArmes*50/2) + 50	+ _armeActif*50.0f	);
		glVertex2f( X,			Y - (_nbrArmes*50/2) + 50	+ _armeActif*50.0f	);
		glVertex2f( X,			Y - (_nbrArmes*50/2) + 0	+ _armeActif*50.0f	);
		glVertex2f( X-50.0f,	Y - (_nbrArmes*50/2) + 0	+ _armeActif*50.0f	);
		glEnd();
	}
}

void CPlayer::armeUp() {	// Rends l'arme suivante active
	_armeActif++;

	if( _armeActif >= _nbrArmes ) {
		_armeActif = 0;
	}
}

/**
 * Active l'arme pr�c�dente de la liste des armes.
 */
void CPlayer::armeDown() {
	_armeActif--;

	if( _armeActif < 0 ) {
		_armeActif = _nbrArmes-1;
	}
}

CPlayer::~CPlayer() {
	if( _pClavier ) {
		delete _pClavier;
		_pClavier = 0;
	}

	if( _cri ) {	// Destruction du cri du personnage
		demonSons->remove( _cri );
		_criRequest = 0;
		_cri = 0;
	}

	if( _pSkin ) {
		delete _pSkin;
		_pSkin = 0;
	}
}

/**
 * Définit le cri du joueur
 */
void CPlayer::setCri(const char *nomFichier) {
	_cri = demonSons->createSon( nomFichier );
	_criRequest = demonSons->playID( _cri, true );
}

CClavier *CPlayer::getClavier() {
	return _pClavier;
}

/**
 * Change la position du joueur.
 */
void CPlayer::setPosition(float x, float y, float z) {
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
}

/**
 * Choisit un point d'entr�e pour le joueur dans la MAP parmi les points d'entr�e de la MAP courante.
 */
void CPlayer::choiceOneEntryPoint() {
	if(Game.getMap()->getEntryPointsList().size() >= 1) {
		CPlayer *player;
		bool valide;
		float pos[3];
		int curseur;
		CV3D distance;
		EntryPoint* entryPoint;
		vector< vector<EntryPoint*>::iterator > candidates;
		vector<EntryPoint*>::iterator iterEntry;
		vector<CPlayer*>::iterator iterPlayer;

		// Fait la liste des points d'entrée se trouvant à une distance supérieure à DISTANCE_INTER_JOUEURS_ENTRY_POINT de tout autre joueur
		for(iterEntry=Game.getMap()->getEntryPointsList().begin() ; iterEntry!=Game.getMap()->getEntryPointsList().end() ; iterEntry++) {
			curseur = -1;
			valide = false;

			while((player = Game.nextPlayer(curseur))) {
				if( player != this ) {	// Si le joueur en question n'est pas le joueur actuel
					entryPoint = *iterEntry;
					distance = entryPoint->getEntryPosition();
					player->getPosition(pos);
					distance -= pos;

					if(distance.norme() < DISTANCE_INTER_JOUEURS_ENTRY_POINT) {
						valide = false;		// Le point d'entrée est trop proche d'un des joueurs
						break;				// On passe au point d'entrée suivant
					}
					else {
						valide = true;
					}
				}
			}

			if(valide) {	// Si le point d'entrée est suffisament distant de tout joueur, souviens-t'en
				candidates.push_back(iterEntry);
			}
		}

		// Choisi un entry point
		size_t nbr = candidates.size();

		if(nbr) {	// S'il y a des entrées à une distance convenable de tout joueur
			int choice = rand() % nbr;
			setPosition((*(candidates[choice]))->getEntryPosition());	// alors choisi l'une d'elles au hasard
			LOGINFO(("Choix d'un entry point éloigné des autres joueurs : %d", choice));
		}
		else {							// sinon prends-en une au hasard dans la liste
			nbr = Game.getMap()->getEntryPointsList().size();
			int choice = rand() % nbr;
			setPosition(Game.getMap()->getEntryPointsList()[choice]->getEntryPosition());
			LOGINFO(("Choix d'un entry point au hasard : %d", choice));
		}
	}
	else {
		CV3D pos( 1.0, 0.5, 0.0 );
		setPosition( pos );
	}
}

/**
 * Change la position du joueur.
 */
void CPlayer::setPosition(const CV3D& pos) {
	_position[0] = pos.X;
	_position[1] = pos.Y;
	_position[2] = pos.Z;
}

/**
 * Change la position du joueur.
 */
void CPlayer::setPosition(const float pos[3]) {
	_position[0] = pos[0];
	_position[1] = pos[1];
	_position[2] = pos[2];
}

/**
 * Retourne la position du joueur.
 */
void CPlayer::getPosition(float pos[3]) const {
	pos[0] = _position[0];
	pos[1] = _position[1];
	pos[2] = _position[2];
}

/**
 * Change la vitesse du joueur.
 */
void CPlayer::setVitesse(float vx, float vy, float vz) {
	_vitesse[0] = vx;
	_vitesse[1] = vy;
	_vitesse[2] = vz;
}

float CPlayer::getRayon() const {
	return _rayonSolidbox;
}

/**
 * Retourne l'accélération du joueur.
 */
void CPlayer::getAcceleration(float vit[3]) const {
	vit[0] = _acceleration[0];
	vit[1] = _acceleration[1];
	vit[2] = _acceleration[2];
}

/**
 * Retourne la vitesse du joueur.
 */
void CPlayer::getVitesse(float vit[3]) const {
	vit[0] = _vitesse[0];
	vit[1] = _vitesse[1];
	vit[2] = _vitesse[2];
}

void CPlayer::getDeplacement(float deplacement[3]) const {
	deplacement[0] = _deplacement[0];
	deplacement[1] = _deplacement[1];
	deplacement[2] = _deplacement[2];
}

/**
 * Retourne la vitesse du joueur.
 */
float CPlayer::getVitesse() const {
	return norme(_vitesse);
}

/**
 * Change la vitesse du joueur.
 */
void CPlayer::setVitesse(const float vit[3]) {
	_vitesse[0] = vit[0];
	_vitesse[1] = vit[1];
	_vitesse[2] = vit[2];
}

/**
 * Change la vitesse du joueur.
 */
void CPlayer::setDeplacement(const float deplacement[3]) {
	_deplacement[0] = deplacement[0];
	_deplacement[1] = deplacement[1];
	_deplacement[2] = deplacement[2];
}

void CPlayer::changeContact(void (*contact)(CPlayer *player, float *normal, float distanceW)) {
	_contactFunc = contact;	//Définit l'action à réaliser lors d'un contact avec la map
}

void CPlayer::Affiche() {
	glPushMatrix();

	glTranslatef(_position[0], _position[1], -_position[2]);
	glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	glRotated(-_teta, 0.0f, 1.0f, 0.0f); 	// Rotation par rapport à l'axe verticale

	// Affiche le skin du joueur s'il existe et si le jeu est configuré pour
	if(Config.Joueur.skinVisibility) {
		if(_pSkin) {
			_pSkin->affiche();
		}
		else {
			// Si le joueur n'a pas de Skin, affiche un elipsoïde à sa place
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
			Fabrique::getGlUtils()->drawSphere(0.1f, 16, 16);

			glDisable(GL_BLEND);
		}
	}

	// Affiche un ellipsïde qui trace les contours de la hitbox du joueur si le jeu est configuré pour
	if(Config.Joueur.hitboxVisibility) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.57f, 0.43f, 0.85f, 0.7f);
		Fabrique::getGlUtils()->drawSphere(0.1f, 16, 16);

		glDisable(GL_BLEND);
	}

	// Affiche un ellipsoïde qui trace les contours physiques du joueur si le jeu est configuré pour
	if(Config.Joueur.solidboxVisibility) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.33f, 1.0f, 0.0f, 0.3f);
		Fabrique::getGlUtils()->drawSphere(_rayonSolidbox, 16, 16);

		glDisable(GL_BLEND);
	}

	glPopMatrix();
}

void CPlayer::tuer() {
	if( !_criRequest->isPlaying() )
		demonSons->play(_criRequest);	// Joue le cri du joueur
}

void CPlayer::afficheProjectils() {		// Affiche tous les projectils du joueur
	CProjectil *pro;

	Tableau<CProjectil>::Adr *adr = TabProjectil.BeginAdr();

	while(adr) {
		pro = adr->m_adrX;

		pro->Affiche();			// Affichage de l'objet g�o

		adr = TabProjectil.Suivant( adr );	//Passe � l'objet géométrique suivant
	}
}

/**
 * Fait un tir avec l'arme active du joueur.
 */
void CPlayer::tir() {
	CV3D Dir;

	switch(_armeActif) {
	// Si l'arme active est le laser, alors tire un laser
	case 1:
		TabProjectil.Ajoute(new CLaser(this));
		break;

		// Si l'arme active est le lance-rocket, alors tire un rocket
	case 2:
		TabProjectil.Ajoute(new CRocket(this));
		break;
	}
}

void CPlayer::refreshProjectils(Uint32 now, float deltaTime) {
	CProjectil *pro;
	Tableau<CProjectil>::Adr *adrSuivant;

	Tableau<CProjectil>::Adr *adr = TabProjectil.BeginAdr();

	while(adr) {	//tant que ce n'est pas le dernier objet géométrique de la liste
		pro = adr->m_adrX;

		if(!pro->Refresh(now, deltaTime))			// Affichage de l'objet géo
		{
			// L'objet 'pro' a signal� la fin de sa vie, il doit être détruit
			adrSuivant = TabProjectil.Suivant( adr );	//Passe à l'objet géométrique suivant

			TabProjectil.Enleve( adr );
			delete pro;
		}
		else {
			// L'objet 'pro' a signalé qu'il reste vivant
			adrSuivant = TabProjectil.Suivant( adr );	//Passe à l'objet géométrique suivant
		}

		adr = adrSuivant;
	}
}

void CPlayer::init() {
}

/**
 * Initialise les éléments OpenGL du joueur.
 */
void CPlayer::initGL() {
	if(_pSkin) {
		_pSkin->initPlugins();
		_pSkin->initGL();
	}
}

/**
 * Libère les éléments OpenGL du joueur.
 */
void CPlayer::freeGL() {
	if(_pSkin) {
		_pSkin->freePlugins();
		_pSkin->freeGL();
	}
}

void CPlayer::createClavier()
{	_pClavier = new CClavier();	}

void CPlayer::exeContactFunc(float *normal, float distanceW) {	// Exécute fonction gestion contacts avec joueur
	if(_contactFunc) {
		_contactFunc(this, normal, distanceW);
	}
}

float CPlayer::phi() const {
	return _phi;
}

float CPlayer::teta() const {
	return _teta;
}

void CPlayer::getPosVue(float vect[3]) const {
	vect[0] = _posVue[0];
	vect[1] = _posVue[1];
	vect[2] = _posVue[2];
}

void CPlayer::setPosVue(const float posVue[3]) {
	_posVue[0] = posVue[0];
	_posVue[1] = posVue[1];
	_posVue[2] = posVue[2];
}

float CPlayer::getReculVue() const {
	return _reculVue;
}

void CPlayer::setReculVue(float reculVue) {
	_reculVue = reculVue;
}

void CPlayer::phi(float phi) {
	_phi = phi;
}

void CPlayer::teta(float teta) {
	_teta = teta;
}

float CPlayer::pente() const {
	return _pente;
}

void CPlayer::pente(float pente) {
	_pente = pente;
}

void CPlayer::setName(const string &name) {
	_name = name;
}

string CPlayer::getName() const {
	return _name;
}

void CPlayer::Skin(jkt::CMap *skin) {
	_pSkin = skin;
}

void CPlayer::calculeAcceleration(bool gravity) {

	/* *****************************************************
	 * Réinitialise les calcules précédents de l'accélération (on la recalcule intégralement à chaque fois)
	 * * ****************************************************/

	_acceleration[0] = 0.0f;
	_acceleration[1] = 0.0f;
	_acceleration[2] = 0.0f;
	_resistance = 0.0f;


	/* *****************************************************
	 * Applique la gravité si elle est définie et active
	 * * ****************************************************/

	if(gravity) {
		_acceleration[1] -= GRAVITY_ACCELERATION;
	}


	/* *****************************************************
	 * Gère les choix du joueur (joystick / clavier)
	 * ****************************************************/

	// Gestion vitesse horizontale
	if( _pente < COSINUS_45 ) {		// Si le joueur est en contact avec le sol (pente inférieure à 45°)

		// Si le joueur avance
		if( (_accelerationClavier[0] != 0.0f) || (_accelerationClavier[2] != 0.0f) ) {
			// Applique l'accélération du clavier
			_acceleration[0] += _accelerationClavier[0] * PLAYER_ACCELERATION;
			_acceleration[2] += _accelerationClavier[2] * PLAYER_ACCELERATION;
		}
		// Si le joueur n'avance pas
		else {
			// Ralentit la vitesse du joueur
			_resistance = 8.0f;					// Le joueur perd toute sa vitesse en un cinqui�me de seconde
		}
	}

	// Gestion vitesse verticale
	_acceleration[1] += _accelerationClavier[1] * PLAYER_ACCELERATION;
}

void CPlayer::calculeVitesse(float deltaTime) {

	/* *****************************************************
	 * Calcule de la vitesse théorique
	 * ****************************************************/

	// Prise en compte de l'accélération
	_vitesse[0] += _acceleration[0] * deltaTime;
	_vitesse[1] += _acceleration[1] * deltaTime;
	_vitesse[2] += _acceleration[2] * deltaTime;

	// Résistance de l'environnement
	if(_resistance != 0.0f) {
		float factor = 1.0f - (_resistance * deltaTime);

		factor = (factor > 0.0f) ? factor : 0.0f;

		_vitesse[0] *= factor;
		_vitesse[1] *= factor;
		_vitesse[2] *= factor;
	}

	/* *****************************************************
	 * Limite la vitesse au sol
	 * ****************************************************/

	// Si le joueur est en contact avec le sol (pente inférieure à 45°)
	if( _pente < COSINUS_45 ) {
		float speedHorizCarre = _vitesse[0]*_vitesse[0] + _vitesse[2]*_vitesse[2];

		// Limite la vitesse au sol
		if(speedHorizCarre > MAX_SPEED_PLAYER_ON_GROUND*MAX_SPEED_PLAYER_ON_GROUND) {
			float ajust = MAX_SPEED_PLAYER_ON_GROUND / sqrtf(speedHorizCarre);
			_vitesse[0] *= ajust;
			_vitesse[2] *= ajust;
		}
	}


	/* *****************************************************
	 * Limite la vitesse globale
	 * ****************************************************/

	// Limite la vitesse du joueur dans tous les axes
	float speedCarre = normeCarre(_vitesse);

	if(speedCarre > MAX_SPEED_PLAYER*MAX_SPEED_PLAYER) {
		float ajust = MAX_SPEED_PLAYER / sqrtf(speedCarre);
		_vitesse[0] *= ajust;
		_vitesse[1] *= ajust;
		_vitesse[2] *= ajust;
	}
}

void CPlayer::resetDeplacement() {
	_deplacement[0] = 0.0f;
	_deplacement[1] = 0.0f;
	_deplacement[2] = 0.0f;
}

void CPlayer::calculeDeplacement(float deltaTime) {
	_deplacement[0] = _vitesse[0] * deltaTime;
	_deplacement[1] = _vitesse[1] * deltaTime;
	_deplacement[2] = _vitesse[2] * deltaTime;
}

void CPlayer::deplace(float deltaTime) {

	// On met � jour volontairement la vitesse en fonction du d�placement avant d'�ventuellement
	// annuler le d�placement s'il est trop petit
	// Ceci afin de permettre au joueur d'acc�l�rer d�s les premiers instants
	_vitesse[0] = _deplacement[0] / deltaTime;
	_vitesse[1] = _deplacement[1] / deltaTime;
	_vitesse[2] = _deplacement[2] / deltaTime;

	// Si le joueur est en contact avec le sol (pente inférieure à 45°)
	if( _pente < COSINUS_45 ) {
		// Si le d�placement est trop faible alors on bouge pas
		float speedCarre = normeCarre(_vitesse);

		if(speedCarre < MIN_SPEED_PLAYER_ON_GROUND*MIN_SPEED_PLAYER_ON_GROUND) {
			_deplacement[0] = 0;
			_deplacement[1] = 0;
			_deplacement[2] = 0;
		}
	}

	// Calcule la nouvelle position
	_position[0] += _deplacement[0];
	_position[1] += _deplacement[1];
	_position[2] += _deplacement[2];
}

void CPlayer::faitRequeteClavier(Uint32 now, float deltaTime) {
	float cosTeta = /*FastCos0(erwin->Teta/180.0f*Pi);*/	cosf(_teta/180.0f*Pi);
	float sinTeta = /*FastSin0(erwin->Teta/180.0f*Pi);*/	sinf(_teta/180.0f*Pi);

	// Poussée verticale
	_accelerationClavier[1] = _pClavier->m_fMonte;

	// Poussée horizontale
	_accelerationClavier[0] = (sinTeta * _pClavier->m_fAvance) + (cosTeta * _pClavier->m_fDroite);
	_accelerationClavier[2] = (cosTeta * _pClavier->m_fAvance) - (sinTeta * _pClavier->m_fDroite);

	_pClavier->reset();	// Réinitialise les requêtes clavier du joueur
}
