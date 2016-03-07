
#ifndef __JKT__MULTIMATERIALGEO_H
#define __JKT__MULTIMATERIALGEO_H

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

#include "spatial/geo/ChanTex.h"
#include "spatial/geo/Geo.h"
#include "spatial/geo/GeoMaker.h"

namespace JktMoteur
{

class CMultiMaterialGeo:public CGeo {
	static const char* identifier;
	int m_OffsetMateriaux;			// Sert lors de la lecture des références matériaux
	string tostring;
	CMaterialMulti* m_Material;		// Matériau multiple associé

	float minX, minY, minZ, maxX, maxY, maxZ;	// Coordonnées du pavé englobant l'objet géo
	float m_Centre[3];			// Centre de la sphère englobant l'objet
	float m_Rayon;				// Rayon de la sphère englobant l'objet
	float* m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	bool m_bSolid;				// Indique si l'objet est solide ou non
	int m_NumVertex;				// Nbre de sommets
	float* m_TabVertex;			// Pointeur sur le tableau de sommets
	int getOffsetMateriau() throw(CErreur);
		// Canaux et coordonées de texture
	map<int,int> m_CanauxNumbers;	// first=numéro de canal ; second=nombre de faces du canal
	float* m_TabTexVertex;			// Liste des coordonnées de texture des canaux
		// VBO
	enum {	VBO_VERTEX=0, VBO_NORMAUX, VBO_TEXVERTEX };
	static const int VBO_BUFFER_SIZE = 3;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];
	void initGL();
	void freeGL();

	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet géométrique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet géo
	void AfficheNormals();

public:
	CMultiMaterialGeo(CMap* map, const string& name, CMaterialMulti* mat, unsigned int nbrfaces, float* vertex, float* normals, float* texvertex, std::map<int,int> &canauxnumbers, bool solid=true);
	CMultiMaterialGeo(CMap *map);
	~CMultiMaterialGeo();
	CGeo* clone();
	void Init();					// Initialisation de l'objet

	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux

	// Transformations
	void EchangeXY();			// Echange les coordonnées X et Y de l'objet
	void EchangeXZ();			// Echange les coordonnées X et Z de l'objet
	void EchangeYZ();			// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ);	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate(float x, float y, float z);				// Translation pondérée selon X, Y et Z de l'objet

	void setMaterial(int matRef);	// Associe l'objet au matériau de référence matRef
	void setNormalVertex(float *tab);			// Implémente les normales aux sommets
	void setSubMat(int *tab);					// Implémente les réf de sous matériau
	void setVertex(int num, float *tab);		// Implémente les sommets
	void setTexVertex(float *tab);				// Implémente les sommets de texture
	void setOffsetMateriau(int offset);			// Décale la référence matériau de l'offset

	// Affichage
	void Affiche();									// Affiche cet objet géo
	void AfficheSelection(float r,float v,float b);	// Affiche l'objet en couleur unique

	// Sérialisation
	bool Save(TiXmlElement* element);
	bool Lit(TiXmlElement* el, MapLogger* mapLogger);

	// Gestion des contacts
	bool Contact( const float pos[3], float dist );
	bool TestContactPave(const float pos[3], float dist);	// 'pos' est-il dans le pavé constitué des distances min/max de l'objet géo
	void GereContactPlayer(float positionPlayer[3], CPlayer *player);
	float GereLaserPlayer(float pos[3], CV3D &Dir, float dist);	// Voir la définition de la fonction

	const char* toString();
};

}	// JktMoteur

#endif

