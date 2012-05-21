
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <vector>

#include "util/Tableau.h"
#include "util/Erreur.h"
#include "util/V3D.h"

class CGame;
class CPlayer;

using namespace JktUtils;

namespace JktMoteur
{
class CGeo;
class CLight;
class CMouve;
class CMaterial;
class CPorte;
class CNavette;
class CGeoObject;
class EntryPoint;

class CMap
{
	vector<CGeo*> m_TabGeo;				// Liste des objets géométriques
	vector<CMouve*> m_TabMouve;			// Liste des objets nécessitant une actualisation (portes,...)

	bool Init() throw(JktUtils::CErreur);		// Initialisation de la CMap
	bool Lit(const string &nomFichier);

	int m_Selection;				// Object géo sélectionné
	bool m_bSelection;				// Indique si le mode sélection est actif ou non
	vector<EntryPoint> _entryPoints;		// Liste des points d'entrée des joueurs sur la Map
public:
	vector<CLight*> m_TabLight;			// Liste des lumières
	vector<CMaterial*> m_TabMaterial;	// Liste des matériaux A VOIR : devrait être membre privé

		// Constructeurs / destructeur
	CMap();
	CMap(const string &nomFichier ) throw(JktUtils::CErreur);
	~CMap();

		// Fonctions membres
	bool initGL();		// Initialisation dans le contexte OpenGL
	bool freeGL();		// Initialisation dans le contexte OpenGL

//	bool SaveFichierMap(const string &nomFichier);		// Sauvegarde du CMap dans un fichier *.map
	bool Save(const string nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

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
	void Scale(float scaleX, float sclaeY, float scaleZ);	// Homothétie sur les objets géo (ils sont multipliés par scale)
		// A VOIR Fonctions qui n'ont rien à faire ici
	void GereContactPlayer(CPlayer *player);								// Gère tous les contacts entre la map et les joueurs
	float GereLaserPlayer(float *pos, JktUtils::CV3D &Dir, float dist );	// Envoie d'un laser sur la map

	void afficheToutesTextures(int x, int y);

		// Mode sélection
	void incrementeSelection();	// Sélectionne l'objet géo suivant
	void decrementeSelection();	// Sélectionne l'objet géo précédent
	void ChangeSelectionMode();
	const char* getSelectedName();
	bool IsSelectionMode();

	// Points d'entrée des personnages joueurs
	void add(EntryPoint entryPoint);
	vector<EntryPoint>& getEntryPointsList();
};

}	// JktMoteur

#endif

