
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

namespace JktMoteur
{
	class CMap;
	class Icone;
}

namespace JktSon
{
	class CSon;
	class CReqSon;
}

class CPlayer {
	static bool _contourVisibility; 		// Affiche ou non un ellipso�de dessinant les contours physiques du joueur
	static JktMoteur::Icone* _weaponsChoice;	// Textures de l'icone des armes

	CClavier *m_pClavier;	//pointeur sur la class des requ�tes clavier

	//POINTEURS SUR LES FONCTIONS SPECIALES
	void (*_actionFunc)(CPlayer *player);	//fonction p�riodique � r�aliser (gravit� par exemple)
	void (*_contactFunc)(CPlayer *player, float *normal, float distanceW); //focntion agissant lors d'un contact avec la map
	int m_ArmeActif;		// Num�ro de l'arme actuellement active
	int m_NbrArmes;			// Nombre d'armes

	JktUtils::Tableau<CProjectil> TabProjectil;		// Liste des projectils de ce joueur

	JktSon::CSon* ID_Cri;
	JktSon::CReqSon* ID_ReqCri;	// Requete son de cri du personnage

	// POSITION, VITESSE ET ORIENTATION DU JOUEUR
	float m_Position[3];		// Vecteur position du joueur
	float m_Vitesse[3];			// Vecteur vitesse du joueur
	float m_Teta;				// Orientation du joueur par rapport � la verticale
	float m_Phi;				// Orientation du joueur par rapport au plan horizontal

	// POSITION ET ORIENTATION DU POINT DE VUE PAR RAPPORT AU JOUEUR
	float m_TetaVue;
	float m_PhiVue;
	float m_PosVue[3];

	float m_Pente;	// Sert � calculer si le joueur glisse sur une pente de la map ou non
	string m_Nom;							// Nom du perso
	JktMoteur::CMap *m_pSkin;	// Gueule du joueur

public :
	JktNet::CSPA* _spa;		// spa associ� au proxy-joueur

	void Tir();
	void RefreshProjectils();

		// Conctructeurs / destructeurs
	CPlayer();		// Cr�e un joueur de vitesse, position et orientaions nuls
	~CPlayer();

	bool openInClientMode(const IPaddress &address);				// Ouverture en mode client
	void close();

		// Fonctions spatiales (vitesse, position, ...) et accesseurs
	void setPosition(float x, float y, float z);		// change la position du joueur
	void setPosition(const float position[3]);		// change la position du joueur
	void setPosition(const JktUtils::CV3D& pos );				// change la position du joueur
	void setVitesse(const float vit[3]);				// Change la vitesse du joueur
	void getPosition(float pos[3]) const;				// renvoie le pointeur sur la position du joueur
	void choiceOneEntryPoint();							// Choisi un point d'entr�e sur la Map
	void changeVitesse(float vx, float vy, float vz);	// change la vitesse du joueur
	void getVitesse(float vit[3]) const;				// renvoie le pointeur sur la vitesse du joueur
	float Phi() const;
	float Teta() const;
	float PhiVue() const;
	float TetaVue() const;
	void Phi( float phi );
	void Teta( float teta );
	void TetaVue( float tetaVue );
	void PhiVue( float phiVue );
	void getPosVue( float vect[3] ) const;
	void setPosVue( const float vect[3] );
	float Pente() const;
	void Pente( float pente );
	void nom( const string &nom );
	string nom() const;
	void Skin( JktMoteur::CMap *skin );
	void setCri( const char *nomFichier );					// Indique le son du cri du joueur

private:
	void createClavier();	// Cr�e la gestion par clavier du joueur
public:
	void init();			// Initialise certaines donn�es du joueur
	void initGL();
	void freeGL();

	CClavier *getClavier();		// Retourne le pointeur sur la classe clavier du joueur
	void exeActionFunc();		// Ex�cute l'action p�riodique associ�e au joueur
	void exeContactFunc( float *normal, float distanceW);	// Ex�cute fonction gestion contacts avec joueur
	void Affiche();				// Fonction d'affichage du joueur

	void ActiveArmeUp();		// Rends l'arme suivante active
	void ActiveArmeDown();		// Rends l'arme pr�c�dente active
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

