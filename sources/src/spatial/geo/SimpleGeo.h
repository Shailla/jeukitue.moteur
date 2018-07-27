
#ifndef __JKT__SIMPLEGEO_H
#define __JKT__SIMPLEGEO_H

/************************************************************/
/*															*/
/*			Objet affichable 3D le plus simple				*/
/*			couleur unique sans mat�riau, ni texture.		*/
/*			Faces index�es, peut �tre �clair�.				*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/GL.H>

#include "util/Erreur.h"
#include "spatial/basic/MapObject.h"

namespace jkt
{

class CSimpleGeo : public MapObject {
	friend class GeoMaker;
	std::string tostring;

	float _minX, _minY, _minZ, _maxX, _maxY, _maxZ;	// Coordonnées du pavé englobant l'objet géo
	float _centre[3];			// Centre de la sphère englobant l'objet
	float _rayon;				// Rayon de la sphère englobant l'objet
	float *_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	float _color[3];			// Couleur de l'objet géo
	bool _bSolid;				// Indique si l'objet est solide ou non
	int _numVertex;				// Nbre de sommets
	int _numFaces;				// Nbre d'index de sommets
	float* m_TabVertex;			// Tableau de sommets
	int* m_TabFaces;			// Tableau d'index des sommets

	// VBO
	enum {	VBO_VERTEX=0, VBO_FACES };
	static const int VBO_BUFFER_SIZE = 2;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];

private:
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet géométrique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet géo

	bool TestContactPave(const float pos[3], float dist);						// 'pos' est-il dans le pavé constitu� des distances min/max de l'objet géo

public:
	static const char* identifier;

	CSimpleGeo(CMap *map, const std::string& name, unsigned int nbrVertex, float* vertex, unsigned int nbrFaces, int* faces, float* color, bool solid=true);
	CSimpleGeo(CMap *map);
	CSimpleGeo(const CSimpleGeo& other);
	~CSimpleGeo();
	MapObject* clone() override;

	void setVertex(int num, float *tab);		// Implémente les sommets
	void setFaces(int num, int *tab);			// Implémente les indices de sommets
	void Color(float r, float g, float b);		// définit la couleur de l'objet

	void init() throw(CErreur) override;		// Initialisation de l'objet
	void initGL() override;
	void freeGL() override;

	void echangeXY() override;										// Echange les coordonnées X et Y de l'objet
	void echangeXZ() override;										// Echange les coordonnées X et Z de l'objet
	void echangeYZ() override;										// Echange les coordonnées Y et Z de l'objet
	void scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;			// Translation pondérée selon X, Y et Z de l'objet

	bool checkContact( const float pos[3], float dist ) override;
	void gereContactPlayer(float positionPlayer[3], CPlayer *player) override;
	float gereLaserPlayer(float pos[3], CV3D &Dir, float dist) override;		// Voir la d�finition de la fonction

	// Fonctions pour l'interface CGeo
	bool Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) throw(CErreur) override;
	bool Save(TiXmlElement* element) throw(CErreur) override;

	void affiche() override;									// Affiche l'objet géo
	void afficheHighlighted(float r,float v,float b) override;	// Affiche l'objet en couleur unique
	const char* toString() override;
};

}	// JktMoteur

#endif

