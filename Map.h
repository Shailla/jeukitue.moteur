
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <vector>

#include "V3D.h"
#include "Tableau.h"
#include "Erreur.h"

class CGame;
class CPlayer;

using namespace JKT_PACKAGE_UTILS;

namespace JKT_PACKAGE_MOTEUR3D
{
class CGeo;
class CLight;
class CMouve;
class CMaterial;
class CPorte;
class CNavette;
class CGeoObject;

class CMap
{
	vector<CGeo*> m_TabGeo;				// Liste des objets géométriques
	vector<CMouve*> m_TabMouve;			// Liste des objets nécessitant une actualisation (portes,...)

	bool Init() throw(JKT_PACKAGE_UTILS::CErreur);								// Initialisation de la CMap
//	bool LitFichier(const string &nomFichier);	// Lecture de la CMap depuis un fichier *.map
	bool Lit(const string &nomFichier);

	int m_Selection;	// Object géo sélectionné
	bool m_bSelection;	// Indique si le mode sélection est actif ou non
public:
	vector<CLight*> m_TabLight;			// Liste des lumières
	vector<CMaterial*> m_TabMaterial;	// Liste des matériaux A VOIR : devrait être membre privé
	vector<CV3D> m_EntreeJoueurs;		// Liste des points d'entrée des joueurs sur la Map

		// Constructeurs / destructeur
	CMap();
	CMap(const string &nomFichier ) throw(JKT_PACKAGE_UTILS::CErreur);
	~CMap();

		// Fonctions membres
//	bool SaveFichierMap(const string &nomFichier);		// Sauvegarde du CMap dans un fichier *.map
	bool Save(const string &nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

	void Affiche();						// Affiche l'ensemble des éléments 3D de cette Map
	void Refresh( CGame *game );		// Rafraichissement des classes listées dans m_TabMouve

	void Add(CGeo *geo);			// Ajoute un GeoObject à la map
	void Add(CMaterial *mat);		// Ajoute un matériau à la map
	void Add(CLight *light);		// Ajoute une lumière à la map

	void Add(CPorte *porte);		// Ajoute une porte à la map
	void Add(CNavette *navette);	// Ajoute une navette à la map

	bool Contact(const float pos[3], float dist);	// Indique s'il y a un triangle à une distance inférieure à 'dist' de la position 'pos'

	void EchangeXY();							// Echange les coordonnées X et Y des objets géo du map
	void EchangeXZ();							// Echange les coordonnées X et Z des objets géo du map
	void EchangeYZ();							// Echange les coordonnées Y et Z des objets géo du map
	void Scale(float scaleX, float sclaeY, float scaleZ);	// Homothétie sur les objets
														// géo (ils sont multipliés par scale)
		// A VOIR Fonctions qui n'ont rien à faire ici
	void GereContactPlayer(CPlayer *player);	// Gère tous les contacts entre la map et les joueurs
	float GereLaserPlayer(float *pos, CV3D &Dir, float dist );	// Envoie d'un laser sur la map

	void afficheToutesTextures(int x, int y);

		// Mode sélection
	void incrementeSelection();	// Sélectionne l'objet géo suivant
	void decrementeSelection();	// Sélectionne l'objet géo précédent
	void ChangeSelectionMode();
	const char* getSelectedName();
	bool IsSelectionMode();
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

