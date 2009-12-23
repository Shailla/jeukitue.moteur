
#ifndef __JKT__PLAYER_H
#define __JKT__PLAYER_H

#include <string>
#include <vector>
using namespace std;

#include "Tableau.h"
#include "V3D.h"
#include "SPA.h"

class CRequete;
class CProjectil;
class CClavier;
namespace JKT_PACKAGE_MOTEUR3D
{
	class CMap;
}
namespace JKT_PACKAGE_SON
{	
	class CSon;	
	class CReqSon;	
}

class CPlayer
{
	CClavier *m_pClavier;	//pointeur sur la class des requêtes clavier

		//POINTEURS SUR LES FONCTIONS SPECIALES
	void (*actionFunc)(CPlayer *player);	//fonction périodique à réaliser (gravité par exemple)
	void (*contactFunc)(CPlayer *player, float *normal, float distanceW); //focntion agissant lors d'un contact avec la map
	int m_ArmeActif;		// Numéro de l'arme actuellement active
	int m_NbrArmes;			// Nombre d'armes
	static int m_TexArmes;	// Textures de l'icone des armes

	JKT_PACKAGE_UTILS::Tableau<CProjectil> TabProjectil;		// Liste des projectils de ce joueur

	JKT_PACKAGE_SON::CSon* ID_Cri;
	JKT_PACKAGE_SON::CReqSon* ID_ReqCri;	// Requete son de cri du personnage

		// Position, vitesse et orientation du joueur
	float m_Position[3];		// Vecteur position du joueur
	float m_Vitesse[3];			// Vecteur vitesse du joueur
	float m_Teta;				// Orientation du joueur par rapport à la verticale
	float m_Phi;				// Orientation du joueur par rapport au plan horizontal

		// Position et orientation du point de vue par rapport au joueur
	float m_TetaVue;
	float m_PhiVue;
	float m_PosVue[3];

	float m_Pente;	// Sert à calculer si le joueur glisse sur une pente de la map ou non
	string m_Nom;							// Nom du perso
	JKT_PACKAGE_MOTEUR3D::CMap *m_pSkin;	// Gueule du joueur

public :		
	JKT_PACKAGE_RESEAU::CSPA spa;		// spa associé au proxy-joueur

	void Tir();
	void RefreshProjectils();

		// Conctructeurs / destructeurs
	CPlayer();		// Crée un joueur de vitesse, position et orientaions nuls
	~CPlayer();

		// Fonctions spatiales (vitesse, position, ...) et accesseurs
	void setPosition(float x, float y, float z);		// change la position du joueur
	void setPosition(const float position[3]);		// change la position du joueur
	void setPosition(const JKT_PACKAGE_UTILS::CV3D& pos );				// change la position du joueur
	void setVitesse(const float vit[3]);				// Change la vitesse du joueur
	void getPosition(float pos[3]) const;				// renvoie le pointeur sur la position du joueur
	void choiceOneEntryPoint();							// Choisi un point d'entrée sur la Map
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
	void Skin( JKT_PACKAGE_MOTEUR3D::CMap *skin );
	void setCri( char *nomFichier );					// Indique le son du cri du joueur

		// FONCTIONS
private:
	void createClavier();	// Crée la gestion par clavier du joueur
public:
	void init();			// Initialise certaines données du joueur
	CClavier *getClavier();	// Retourne le pointeur sur la classe clavier du joueur
	void exeActionFunc();	// Exécute l'action périodique associée au joueur
	void exeContactFunc( float *normal, float distanceW);	// Exécute fonction gestion contacts avec joueur
	void Affiche();			// Fonction d'affichage du joueur

	void ActiveArmeUp();		// Rends l'arme suivante active
	void ActiveArmeDown();		// Rends l'arme précédente active 
	void AfficheIconesArmes();
	int ArmeActif();			// Renvoi le numéro de l'arme active
	void AfficheProjectils();	// Affiche les projectils lancés par ce joueur
		// INTERACTIONS
	void tuer();	// Tue le joueur

		//FONCTIONS DE MISE EN PLACE DES FONCTIONS SPECIALES
	void changeAction(void (*action)(CPlayer *player));//change la fonction à effectuer par défaut
	void changeContact(void (*contact)(CPlayer *player, float *normal, float distanceW));//change la fonction à effectuer lors
														//d'un contact avec la map
	void deplace();
	void faitRequeteClavier();
};

#endif

