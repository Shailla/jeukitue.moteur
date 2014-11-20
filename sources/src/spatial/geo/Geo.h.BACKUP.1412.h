
#ifndef __JKT__GEO_H
#define __JKT__GEO_H

#include <string>

using namespace std;

#include "tinyxml.h"

#include "util/V3D.h"
#include "util/GenRef.h"

/************************************************************************/
/*																		*/
/*			Cette classe est une interface,								*/
/*			elle est la base de tout affichable en 3D.					*/
/*																		*/
/************************************************************************/

class CPlayer;

using namespace JktUtils;

namespace JktMoteur
{

class CMap;
class CIfstreamMap;

class CGeo {
	static JktUtils::CGenRef GENREF;
	CMap *pMap;							// Navigabilité vers l'objet parent
	string _name;						// Nom donné à au géo
protected:
	unsigned int m_Reference;			// Référence identifiant l'objet (pour la sérialisation)
public:
	// Constructeurs / destructeur
	CGeo(CMap *map);
	virtual ~CGeo();

	// Gestion identiant, nom et référence
	CMap *getMap();						// Navigabilité vers l'objet qui contient cet objet (la Map étant l'objet racine)
	void setMap(CMap* map);
	void setName(const string &nom);	// Implémente le nom du géo
	const char *getName();				// Renvoie le nom de l'objet
	unsigned int getReference();		// Renvoie la référence de l'objet

<<<<<<< HEAD
	// Fonctions d'intialisation et libération
	virtual void initGL() = 0;								// Initialisation de l'objet géométrique
	virtual void freeGL() = 0;								// Libération des ressources de l'objet dans le contexte OpenGL
	virtual void Init() = 0;								// Initialisation de l'objet géométrique
	virtual const char* toString() = 0;						// Description résumée de l'objet

	// Affichage
	virtual void Affiche() = 0;									// Affiche l'objet géométrique
	virtual void AfficheSelection(float r,float v,float b) = 0;	// Affiche l'objet géométrique en couleur unique

	// Sérialisation
	virtual bool Lit(TiXmlElement* el) = 0;
	virtual bool Save(TiXmlElement* element) = 0;			// Sauve l'objet géo dans un fichier Map

	// Transformations 3D
	virtual void EchangeXY() = 0;							// Echange les coordonnées X et Y de l'objet
	virtual void EchangeXZ() = 0;							// Echange les coordonnées X et Z de l'objet
	virtual void EchangeYZ() = 0;							// Echange les coordonnées Y et Z de l'objet
=======
		// Interface
	virtual void initGL() = 0;											// Initialisation de l'objet géométrique
	virtual void freeGL() = 0;											// Libération des ressources de l'objet dans le contexte OpenGL
	virtual void Init() = 0;											// Initialisation de l'objet géométrique
	virtual void Affiche() = 0;											// Affiche l'objet géométrique
	virtual void AfficheSelection(float r,float v,float b) = 0;			// Affiche l'objet géométrique en couleur unique
	virtual bool Lit(TiXmlElement* el) = 0;
	virtual bool Save(TiXmlElement* element) = 0;						// Sauve l'objet géo dans un fichier Map
	virtual void EchangeXY() = 0;										// Echange les coordonnées X et Y de l'objet
	virtual void EchangeXZ() = 0;										// Echange les coordonnées X et Z de l'objet
	virtual void EchangeYZ() = 0;										// Echange les coordonnées Y et Z de l'objet
>>>>>>> Mise en place Dirigeable
	virtual void Scale( float scaleX, float scaleY, float scaleZ ) = 0;	// Homothétie pondérée selon X, Y et Z de l'objet
	virtual void translate(float x, float y, float z) = 0;				// Translation pondérée selon X, Y et Z de l'objet

	// Gestion des contacts
	virtual void GereContactPlayer(float positionPlayer[3], CPlayer *player ) = 0;	// Gère les contact entre 'player' et l'objet géo
	virtual float GereLaserPlayer( float pos[3], CV3D &Dir, float dist) = 0;	// Voir la définition de la fonction
	virtual bool Contact( const float pos[3], float dist );
};

}	// JktMoteur

#endif

