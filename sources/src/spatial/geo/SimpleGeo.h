
#ifndef __JKT__SIMPLEGEO_H
#define __JKT__SIMPLEGEO_H

/************************************************************/
/*															*/
/*			Objet affichable 3D le plus simple				*/
/*			couleur unique sans matériau, ni texture.		*/
/*			Faces indexées, peut être éclairé.				*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

using namespace std;

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/GL.H>

#include "spatial/geo/Geo.h"

namespace JktMoteur
{

class CSimpleGeo:public CGeo
{
	friend class GeoMaker;
	string tostring;

	float _minX, _minY, _minZ, _maxX, _maxY, _maxZ;	// Coordonnées du pavé englobant l'objet géo
	float m_Centre[3];			// Centre de la sphère englobant l'objet
	float m_Rayon;				// Rayon de la sphère englobant l'objet
	float *m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	float m_Color[3];			// Couleur de l'objet géo
	bool m_bSolid;				// Indique si l'objet est solide ou non
	int m_NumVertex;			// Nbre de sommets
	int m_NumFaces;				// Nbre d'index de sommets
	float* m_TabVertex;			// Pointeur sur le tableau de sommets
	int* m_TabFaces;			// Pointeur sur le tableau d'index des sommets

	// VBO
	enum {	VBO_VERTEX=0, VBO_FACES };
	static const int VBO_BUFFER_SIZE = 2;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];

public:
	static const char* identifier;

	CSimpleGeo(CMap *map, const string& name, unsigned int nbrVertex, float* vertex, unsigned int nbrFaces, int* faces, float* color, bool solid=true);
	CSimpleGeo(CMap *map);
	~CSimpleGeo();

	void Init();								// Initialisation de l'objet
	void initGL();
	void freeGL();

private:
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet géométrique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet géo

public:
	void EchangeXY();			// Echange les coordonnées X et Y de l'objet
	void EchangeXZ();			// Echange les coordonnées X et Z de l'objet
	void EchangeYZ();			// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ);	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate( float x, float y, float z );			// Translation pondérée selon X, Y et Z de l'objet
	void Color(float r, float g, float b);		// défini la couleur de l'objet
	bool TestContactPave(const float pos[3], float dist);	// 'pos' est-il dans le pavé constitué des distances min/max de l'objet géo
	void GereContactPlayer(const float pos[3], CPlayer *player);
	float GereLaser(float pos[3], CV3D &Dir, float dist);	// Voir la définition de la fonction

	void setVertex(int num, float *tab);		// Implémente les sommets
	void setFaces(int num, int *tab);			// Implémente les indices de sommets

		// Fonctions pour l'interface CGeo
	//bool LitFichier( CIfstreamMap &fichier );			// Lit un objet géo dans un fichier Map
	//bool SaveNameType( ofstream &fichier );				// Sauve le nom du type d'objet géométrique
	//bool SaveFichierMap( ofstream &fichier );			// Sauve l'objet géo dans un fichier Map
	bool Lit(TiXmlElement* element);
	bool Save(TiXmlElement* element);

	bool Contact( const float pos[3], float dist );

	void Affiche();								// Affiche l'objet géo
	void AfficheSelection(float r,float v,float b);	// Affiche l'objet en couleur unique
	const char* toString();
};

}	// JktMoteur

#endif

