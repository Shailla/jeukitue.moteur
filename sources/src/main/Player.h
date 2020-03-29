
#ifndef __JKT__PLAYER_H
#define __JKT__PLAYER_H

#include <string>
#include <vector>

#include "util/Tableau.h"
#include "util/V3D.h"
#include "reseau/SPA.h"

class CRequete;
class CProjectil;
class CClavier;

namespace jkt
{
class CMap;
class Icone;
class CSon;
class CReqSon;
}

class CPlayer {
	int _id;

	static jkt::Icone* _weaponsChoice;	// Textures de l'icone des armes

	CClavier *_pClavier;	//pointeur sur la class des requêtes clavier

	//POINTEURS SUR LES FONCTIONS SPECIALES
	void (*_actionFunc)(Uint32 now, CPlayer *player, float deltaTime);		// Fonction périodique à réaliser (gravité par exemple)
	void (*_contactFunc)(CPlayer *player, float *normal, float distanceW); 	// Focntion agissant lors d'un contact avec la map
	int _armeActif;							// Numéro de l'arme actuellement active
	int _nbrArmes;							// Nombre d'armes

	jkt::Tableau<CProjectil> TabProjectil;		// Liste des projectils de ce joueur

	jkt::CSon* _cri;
	jkt::CReqSon* _criRequest;		// Requête son de cri du personnage

	// Position, vitesse et orientation du joueur
	float _vitesse[3];				// Vitesse du joueur
	float _deplacement[3];			// Déplacement voulu du joueur, calculé en fonction de la vitesse et du temps écoulé
	float _position[3];				// Position du joueur



	float _accelerationClavier[3];	// Accélération demandée par le joueur avec le clavier

	float _teta;					// Orientation du joueur par rapport à la verticale
	float _phi;						// Orientation du joueur par rapport au plan horizontal

	float _posVue[3];				// Position des yeux du joueur par rapport au joueur
	float _reculVue;				// Recul de la vue du joueur par rapport à ses yeux

	float _rayonSolidbox;			// Rayon de la sphère englobant le joueur pour la gestion des contacts
	float _pente;					// Sert à calculer si le joueur glisse sur une pente de la map ou non
	std::string _name;				// Nom du perso
	jkt::CMap *_pSkin;				// Gueule du joueur

	void createClavier();			// Crée la gestion par clavier du joueur

public :
	// Conctructeurs / destructeurs
	CPlayer();				// Crée un joueur de vitesse, position et orientaions nuls
	virtual ~CPlayer();
	void init();			// Initialise certaines données du joueur
	void setId(int id);
	int getId() const;

	// Réseau
	jkt::CSPA* _spa;		// spa associé au proxy-joueur
	bool openInClientMode(const IPaddress &address);				// Ouverture en mode client
	void close();

	// Armes
	void tir();
	void refreshProjectils(Uint32 now, float deltaTime);

	// Position
	void setPosition(float x, float y, float z);		// change la position du joueur
	void setPosition(const float position[3]);			// change la position du joueur
	void setPosition(const jkt::CV3D& pos );			// change la position du joueur
	void setVitesse(const float vit[3]);				// Change la vitesse du joueur
	void setDeplacement(const float deplacement[3]);	// Change la vitesse du joueur
	void getPosition(float pos[3]) const;				// renvoie le pointeur sur la position du joueur
	void choiceOneEntryPoint();							// Choisi un point d'entr�e sur la Map

	// Vitesse
	void setVitesse(float vx, float vy, float vz);	// change la vitesse du joueur
	void getVitesse(float vit[3]) const;				// renvoie le pointeur sur la vitesse du joueur
	float getVitesse() const;							// Renvoie la vitesse du joueur en m/s
	void getDeplacement(float deplacement[3]) const;	// renvoie le pointeur sur le déplacement du joueur

	// Géométrie
	float getRayon() const;

	// Orientation
	float phi() const;
	float teta() const;
	void phi( float phi );
	void teta( float teta );

	// Caméra
	float phiVue() const;
	float tetaVue() const;
	void tetaVue(float tetaVue);
	void phiVue(float phiVue);
	void getPosVue(float vect[3]) const;
	void setPosVue(const float vect[3]);
	float getReculVue() const;
	void setReculVue(float reculVue);

	float pente() const;
	void pente( float pente );

	void setName(const std::string &name);
	std::string getName() const;

	void Skin( jkt::CMap *skin );

	void setCri( const char *nomFichier );									// Indique le son du cri du joueur

	CClavier *getClavier();													// Retourne le pointeur sur la classe clavier du joueur
	void calculeEnvironment(Uint32 now, float deltaTime);						// Exécute l'action périodique associée au joueur
	void exeContactFunc( float *normal, float distanceW);	// Exécute fonction gestion contacts avec joueur

	// Affichage
	void Affiche();				// Fonction d'affichage du joueur
	void initGL();
	void freeGL();

	// Arme
	void armeUp();				// Rends l'arme suivante active
	void armeDown();			// Rends l'arme précédente active
	void afficheIconesArmes();
	int armeActif();			// Renvoi le numéro de l'arme active
	void afficheProjectils();	// Affiche les projectils lancés par ce joueur

	// INTERACTIONS
	void tuer();				// Tue le joueur

	//FONCTIONS DE MISE EN PLACE DES FONCTIONS SPECIALES
	void changeAction(void (*action)(Uint32 now, CPlayer *player, float deltaTime));		// Change la fonction à effectuer par défaut
	void changeContact(void (*contact)(CPlayer *player, float *normal, float distanceW));	// Change la fonction à effectuer lors d'un contact avec la map
	void calculeVitesse(float deltaTime);
	void resetDeplacement();
	void calculeDeplacement(float deltaTime);
	void deplace(Uint32 now);
	void faitRequeteClavier(Uint32 now, float deltaTime);
};

#endif

