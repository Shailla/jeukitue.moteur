#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <map>
#include <cmath>

using namespace std;

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

#include "fmod.h"
#include "SDL.h"

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

using namespace jkt;

extern CCfg Config;
extern CDemonSons *DemonSons;
extern CGame Game;

#include "main/Player.h"

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
	_phi = 0.0;

	_posVue[0] = 0.0f;		// Position du point de vue par rapport au joueur
	_posVue[1] = 0.1f;
	_posVue[2] = 0.137f;

	_rayonSolidbox = 0.30f;	// Rayon de la sph�re qui repr�sente le joueur dans la amp. Le joueur mesure arbitrairement 1m70, son rayon la moitier

	_pClavier = NULL;
	_actionFunc = NULL;		// Pas d'action p�riodique � r�aliser
	_contactFunc = NULL;	// Pas d'action � r�aliser lors d'un contact avec la map

	_pSkin = NULL;			// Pas de skin associ� par d�faut

	ID_Cri = NULL;			// Le cri du joueur n'a pas encore �t� charg�
	ID_ReqCri = NULL;		// Y'a pas encore la requ�te sur le cri non plus

	createClavier();		// Cr�e la classe qui g�re les requ�tes de mouvement, tir ...

	if(_weaponsChoice == NULL) {
		try {
			Uint8* pixelsWeaponsChoice = jkt::litFichierImage("@Icone/Armes.bmp", 0.75f);
			_weaponsChoice = jkt::litFichierIcone("@Icone/Armes.bmp", 0.75f, pixelsWeaponsChoice);
		}
		catch(CErreur& erreur) {
			LOGDEBUG(("CPlayer::CPlayer() Echec de lecture de texture d'icone des armes%T", this));
			_weaponsChoice = NULL;
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

void CPlayer::AfficheIconesArmes() {
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

	if( _armeActif >= _nbrArmes )
		_armeActif = 0;
}

/**
 * Active l'arme pr�c�dente de la liste des armes.
 */
void CPlayer::armeDown() {
	_armeActif--;

	if( _armeActif < 0 )
		_armeActif = _nbrArmes-1;
}

CPlayer::~CPlayer() {
	if( _pClavier ) {
		delete _pClavier;
		_pClavier = 0;
	}

	if( ID_Cri ) {	// Destruction du cri du personnage
		DemonSons->Delete( ID_Cri );
		ID_ReqCri = 0;
		ID_Cri = 0;
	}

	if( _pSkin ) {
		delete _pSkin;
		_pSkin = 0;
	}
}

/**
 * D�finit le cri du joueur � l'instant o� est bless�.
 */
void CPlayer::setCri(const char *nomFichier) {
	ID_Cri = DemonSons->CreateSon( nomFichier );
	ID_ReqCri = DemonSons->PlayID( ID_Cri, true );
}

CClavier *CPlayer::getClavier()
{	return _pClavier;	}

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
		vector< vector<EntryPoint*>::iterator > liste;
		vector<EntryPoint*>::iterator iterEntry;
		vector<CPlayer*>::iterator iterPlayer;

			// Fait la liste des points d'entr�e se trouvant � une distance sup�rieure
			// � DISTANCE_INTER_JOUEURS_ENTRY_POINT de tout autre joueur
		for(iterEntry=Game.getMap()->getEntryPointsList().begin() ; iterEntry!=Game.getMap()->getEntryPointsList().end() ; iterEntry++) {
			curseur = -1;
			valide = false;

			while((player = Game.nextPlayer(curseur))) {
				if( player != this ) {	// Si le joueur en question n'est pas le joueur actuel
					distance = (*iterEntry)->getEntryPosition();
					player->getPosition(pos);
					distance -= pos;

					if(distance.norme() < DISTANCE_INTER_JOUEURS_ENTRY_POINT) {
						valide = false;		// Le point d'entr�e est trop proche d'un des joueurs
						break;				// On passe au point d'entr�e suivant
					}
					else {
						valide = true;
					}
				}
			}

			if( valide )	// Si le point d'entr�e est suffisament distant de tout joueur, souviens-t'en
				liste.push_back(iterEntry);
		}


		size_t nbr = liste.size();

		if(nbr) {	// S'il y a des entr�es � une distance convenable de tout joueur
			int choice = rand() % nbr;
			setPosition((*(liste[choice]))->getEntryPosition());	// alors choisi l'une d'elles au hasard
			LOGINFO(("Choix d'un entry point �loign� des autres joueurs : %d", choice));
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
void CPlayer::changeVitesse(float vx, float vy, float vz) {
	_vitesse[0] = vx;
	_vitesse[1] = vy;
	_vitesse[2] = vz;
}

float CPlayer::getRayon() const {
	return _rayonSolidbox;
}

/**
 * Retourne la vitesse du joueur.
 */
void CPlayer::getVitesse(float vit[3]) const {
	vit[0] = _vitesse[0];
	vit[1] = _vitesse[1];
	vit[2] = _vitesse[2];
}

/**
 * Change la vitesse du joueur.
 */
void CPlayer::setVitesse(const float vit[3]) {
	_vitesse[0] = vit[0];
	_vitesse[1] = vit[1];
	_vitesse[2] = vit[2];
}

void CPlayer::changeAction(void (*action)(CPlayer *player)) {
	_actionFunc = action;	//d�finit l'action p�riodique � r�aliser
}

void CPlayer::changeContact(void (*contact)(CPlayer *player, float *normal, float distanceW)) {
	_contactFunc = contact;	//d�finit l'action � r�aliser lors d'un contact avec la map
}

void CPlayer::Affiche() {
	glPushMatrix();

	glTranslatef(_position[0], _position[1], -_position[2]);
	glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	glRotated(-_teta, 0.0f, 1.0f, 0.0f); //Rotation par rapport � l'axe verticale

	// Affiche le skin du joueur s'il existe et si le jeu est configur� pour
	if(Config.Joueur.skinVisibility && _pSkin)
		_pSkin->Affiche();

	// Affiche un ellipso�de qui trace les contours de la hitbox du joueur si le jeu est configur� pour
	if(Config.Joueur.hitboxVisibility) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.57f, 0.43f, 0.85f, 0.7f);
		Fabrique::getGlUtils()->drawSphere(0.1f, 16, 16);

		glDisable(GL_BLEND);
	}

	// Affiche un ellipso�de qui trace les contours physiques du joueur si le jeu est configur� pour
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
	if( !ID_ReqCri->IsPlaying() )
		DemonSons->Play( ID_ReqCri );	// Joue le cri du joueur
}

void CPlayer::AfficheProjectils() {		// Affiche tous les projectils du joueur
	CProjectil *pro;

	Tableau<CProjectil>::Adr *adr = TabProjectil.BeginAdr();

	while(adr) {
		pro = adr->m_adrX;

		pro->Affiche();			// Affichage de l'objet g�o

		adr = TabProjectil.Suivant( adr );	//Passe � l'objet g�om�trique suivant
	}
}

/**
 * Fait un tir avec l'arme active du joueur.
 */
void CPlayer::Tir() {
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

void CPlayer::RefreshProjectils() {
	CProjectil *pro;
	Tableau<CProjectil>::Adr *adrSuivant;

	Tableau<CProjectil>::Adr *adr = TabProjectil.BeginAdr();

	while(adr) {	//tant que ce n'est pas le dernier objet g�om�trique de la liste
		pro = adr->m_adrX;

		if(!pro->Refresh())			// Affichage de l'objet g�o
		{
			// L'objet 'pro' a signal� la fin de sa vie, il doit �tre d�truit
			adrSuivant = TabProjectil.Suivant( adr );	//Passe � l'objet g�om�trique suivant

			TabProjectil.Enleve( adr );
			delete pro;
		}
		else {
			// L'objet 'pro' a signal� qu'il reste vivant
			adrSuivant = TabProjectil.Suivant( adr );	//Passe � l'objet g�om�trique suivant
		}

		adr = adrSuivant;
	}
}

void CPlayer::init() {
}

/**
 * Initialise les �l�ments OpenGL du joueur.
 */
void CPlayer::initGL() {
	_pSkin->initPlugins();
	_pSkin->initGL();
}

/**
 * Lib�re les �l�ments OpenGL du joueur.
 */
void CPlayer::freeGL() {
	_pSkin->freePlugins();
	_pSkin->freeGL();
}

void CPlayer::createClavier()
{	_pClavier = new CClavier();	}

void CPlayer::exeActionFunc() {	// Ex�cute l'action p�riodique associ�e au joueur
	if(_actionFunc) {
		_actionFunc( this );
	}
}

void CPlayer::exeContactFunc(float *normal, float distanceW) {	// Ex�cute fonction gestion contacts avec joueur
	if(_contactFunc) {
		_contactFunc( this, normal, distanceW );
	}
}

float CPlayer::Phi() const {
	return _phi;
}

float CPlayer::Teta() const {
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

void CPlayer::Phi(float phi) {
	_phi = phi;
}

void CPlayer::Teta(float teta) {
	_teta = teta;
}

float CPlayer::Pente() const {
	return _pente;
}

void CPlayer::Pente(float pente) {
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

void CPlayer::deplace() {
	float norm;
	float vect[3];

	if( fabsf(Pente())>0.707f ) {	// Si la pente est inf�rieure � 45�
		if( (fabsf(getClavier()->m_fDroite)<QUANTUM_VITESSE_PLAYER/(5.f)) && (fabsf(getClavier()->m_fAvance)<QUANTUM_VITESSE_PLAYER/(5.f)) )
				// S'il n'y a pas de requ�te de d�placement=>ralenti le joueur
		{
			getVitesse( vect );

			if(norme(vect) > QUANTUM_VITESSE_PLAYER/2) {
				float var = 1 - ( QUANTUM_VITESSE_PLAYER/(2*norme( vect )) );

				vect[0] = vect[0]*var;
				vect[1] = vect[1]*var;
				vect[2] = vect[2]*var;

				setVitesse( vect );
			}
			else {
				vect[0] = 0;
				vect[1] = 0;
				vect[2] = 0;
				setVitesse( vect );
			}
		}
	}

	float position[3], vitesse[3];
	getPosition(position);
	getVitesse(vitesse);
	position[0] += vitesse[0];	//incr�mente la position du joueur
	position[1] += vitesse[1];	//de sa vitesse
	position[2] += vitesse[2];
	setPosition(position);


	if(fabsf(Pente()) > 0.707f) {	//Si la pente est inf�rieure � 45�=>limite la vitesse
		norm = norme( vitesse );

		if(norm > MAX_VITESSE_PLAYER) {	//Limitation de la vitesse du joueur
			normalise( vitesse );
			vitesse[0] = vitesse[0]*MAX_VITESSE_PLAYER;
			vitesse[1] = vitesse[1]*MAX_VITESSE_PLAYER;
			vitesse[2] = vitesse[2]*MAX_VITESSE_PLAYER;

			setVitesse( vitesse );
		}
	}
}

void CPlayer::faitRequeteClavier() {
	float cosTeta = /*FastCos0(erwin->Teta/180.0f*Pi);*/	cosf(_teta/180.0f*Pi);
	float sinTeta = /*FastSin0(erwin->Teta/180.0f*Pi);*/	sinf(_teta/180.0f*Pi);
	float vect[3];

	getVitesse( vect );

	vect[1] += QUANTUM_VITESSE_PLAYER*_pClavier->m_fMonte;

	vect[0] += cosTeta*QUANTUM_VITESSE_PLAYER*_pClavier->m_fDroite;
	vect[2] -= sinTeta*QUANTUM_VITESSE_PLAYER*_pClavier->m_fDroite;

	vect[0] += sinTeta*QUANTUM_VITESSE_PLAYER*_pClavier->m_fAvance;
	vect[2] += cosTeta*QUANTUM_VITESSE_PLAYER*_pClavier->m_fAvance;

	setVitesse( vect );

	_pClavier->reset();	// R�initialise les requ�tes clavier du joueur
}
