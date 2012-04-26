
#ifndef __JKT__SIMPLEMATERIALGEO_H
#define __JKT__SIMPLEMATERIALGEO_H

/************************************************************/
/*															*/
/*			Objet affichable 3D simple associé à un			*/
/*			matériau unique. Faces non-indexées,			*/
/*			peut être éclairé.								*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

using namespace std;

#include "spatial/geo/Geo.h"
#include "spatial/geo/GeoMaker.h"

namespace JktMoteur
{

class CSimpleMaterialGeo:public CGeo
{
	int m_OffsetMateriaux;		// Sert lors de la lecture des références matériaux
	CMaterial* m_Material;		// Matériau utilisé
	string tostring;

	// VBO
	enum {	VBO_VERTEX=0, VBO_NORMAUX };
	static const int VBO_BUFFER_SIZE = 2;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];
public:
	static const char* identifier;
	CSimpleMaterialGeo(CMap *map, const string& name, CMaterial *mat, unsigned int nbrVertex, float* vertex, float* normals, bool solid=true);
	CSimpleMaterialGeo(CMap *map);
	void Init();								// Initialisation de l'objet
	void initGL();
	void freeGL();
private:
	float minX, minY, minZ, maxX, maxY, maxZ;	// Coordonnées du pavé englobant l'objet géo
	float m_Centre[3];			// Centre de la sphère englobant l'objet
	float m_Rayon;				// Rayon de la sphère englobant l'objet
	float* m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	bool m_bSolid;				// Indique si l'objet est solide ou non
	int m_NumVertex;			// Nbre de sommets
	float* m_TabVertex;			// Pointeur sur le tableau de sommets
	int getOffsetMateriau() throw(CErreur);
	void initVBO();
	void freeVBO();

public:
		// Destructeur
	~CSimpleMaterialGeo();

	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux
private:
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet géométrique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet géo
	void AfficheNormals();
public:
	void EchangeXY();			// Echange les coordonnées X et Y de l'objet
	void EchangeXZ();			// Echange les coordonnées X et Z de l'objet
	void EchangeYZ();			// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ);	// Homothétie pondérée selon X, Y et Z de l'objet

	bool TestContactPave(const float pos[3], float dist);	// 'pos' est-il dans le pavé constitué des distances min/max de l'objet géo
	void GereContactPlayer(const float pos[3], CPlayer *player);
	float GereLaser(float pos[3], CV3D &Dir, float dist);	// Voir la définition de la fonction

	void setMaterial(int matRef);	// Associe l'objet au matériau de référence matRef
	void setNormalVertex(float *tab);			// Implémente les normales aux sommets
	void setSubMat(int *tab);					// Implémente les réf de sous matériau
	void setVertex(int num, float *tab);		// Implémente les sommets
	void setOffsetMateriau(int offset);					// Décale la référence matériau de l'offset

		// Fonctions pour l'interface CGeo
	//bool LitFichier(CIfstreamMap &fichier);			// Lit un objet géo dans un fichier Map
	//bool SaveNameType(ofstream &fichier);				// Sauve le nom du type d'objet géométrique
	//bool SaveFichierMap(ofstream &fichier);			// Sauve l'objet géo dans un fichier Map
	bool Save(TiXmlElement* element);
	bool Lit(TiXmlElement* element);

	bool Contact( const float pos[3], float dist );

	void Affiche();									// Affiche cet objet géo
	void AfficheSelection(float r,float v,float b);	// Affiche l'objet en couleur unique
	const char* toString();
};

}	// JktMoteur

#endif

