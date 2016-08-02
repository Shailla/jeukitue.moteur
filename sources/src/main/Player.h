
#ifndef __JKT__PLAYER_H
#define __JKT__PLAYER_H

#include <string>
#include <vector>
using namespace std;

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
	static jkt::Icone* _weaponsChoice;	// Textures de l'icone des armes

	CClavier *_pClavier;	//pointeur sur la class des requ�tes clavier

	//POINTEURS SUR LES FONCTIONS SPECIALES
	void (*_actionFunc)(CPlayer *player);	//fonction p�riodique � r�aliser (gravit� par exemple)
	void (*_contactFunc)(CPlayer *player, float *normal, float distanceW); //focntion agissant lors d'un contact avec la map
	int _armeActif;							// Num�ro de l'arme actuellement active
	int _nbrArmes;							// Nombre d'armes

	jkt::Tableau<CProjectil> TabProjectil;		// Liste des projectils de ce joueur

	jkt::CSon* ID_Cri;
	jkt::CReqSon* ID_ReqCri;	// Requete son de cri du personnage

	// Position, vitesse et orientation du joueur
	float _position[3];			// Vecteur position du joueur
	float _vitesse[3];			// Vecteur vitesse du joueur
	float _teta;				// Orientation du joueur par rapport � la verticale
	float _phi;					// Orientation du joueur par rapport au plan horizontal

	float _posVue[3];			// Position et orientation du point de vue par rapport au joueur

	float _rayonSolidbox;		// Rayon de la sph�re englobant le joueur pour la gestion des contacts
	float _pente;				// Sert � calculer si le joueur glisse sur une pente de la map ou non
	string _name;				// Nom du perso
	jkt::CMap *_pSkin;			// Gueule du joueur

	void createClavier();	// Cr�e la gestion par clavier du joueur

public :
	// Conctructeurs / destructeurs
	CPlayer();				// Cr�e un joueur de vitesse, position et orientaions nuls
	~CPlayer();
	void init();			// Initialise certaines donn�es du joueur

	// R�seau
	jkt::CSPA* _spa;		// spa associ� au proxy-joueur
	bool openInClientMode(const IPaddress &address);				// Ouverture en mode client
	void close();

	// Armes
	void Tir();
	void RefreshProjectils();

	// Position
	void setPosition(float x, float y, float z);		// change la position du joueur
	void setPosition(const float position[3]);			// change la position du joueur
	void setPosition(const jkt::CV3D& pos );			// change la position du joueur
	void setVitesse(const float vit[3]);				// Change la vitesse du joueur
	void getPosition(float pos[3]) const;				// renvoie le pointeur sur la position du joueur
	void choiceOneEntryPoint();							// Choisi un point d'entr�e sur la Map

	// Vitesse
	void changeVitesse(float vx, float vy, float vz);	// change la vitesse du joueur
	void getVitesse(float vit[3]) const;				// renvoie le pointeur sur la vitesse du joueur

	// G�om�trie
	float getRayon() const;

	// Orientation
	float Phi() const;
	float Teta() const;
	void Phi( float phi );
	void Teta( float teta );

	// Cam�ra
	float PhiVue() const;
	float TetaVue() const;
	void TetaVue( float tetaVue );
	void PhiVue( float phiVue );
	void getPosVue( float vect[3] ) const;
	void setPosVue( const float vect[3] );

	float Pente() const;
	void Pente( float pente );

	void setName(const string &name);
	string getName() const;

	void Skin( jkt::CMap *skin );

	void setCri( const char *nomFichier );					// Indique le son du cri du joueur

	CClavier *getClavier();		// Retourne le pointeur sur la classe clavier du joueur
	void exeActionFunc();		// Ex�cute l'action p�riodique associ�e au joueur
	void exeContactFunc( float *normal, float distanceW);	// Ex�cute fonction gestion contacts avec joueur

	// Affichage
	void Affiche();				// Fonction d'affichage du joueur
	void initGL();
	void freeGL();

	// Arme
	void armeUp();				// Rends l'arme suivante active
	void armeDown();			// Rends l'arme pr�c�dente active
	void AfficheIconesArmes();
	int ArmeActif();			// Renvoi le num�ro de l'arme active
	void AfficheProjectils();	// Affiche les projectils lanc�s par ce joueur

	// INTERACTIONS
	void tuer();				// Tue le joueur

	//FONCTIONS DE MISE EN PLACE DES FONCTIONS SPECIALES
	void changeAction(void (*action)(CPlayer *player));//change la fonction � effectuer par d�faut
	void changeContact(void (*contact)(CPlayer *player, float *normal, float distanceW));//change la fonction � effectuer lors d'un contact avec la map
	void deplace();
	void faitRequeteClavier();
};

#endif

