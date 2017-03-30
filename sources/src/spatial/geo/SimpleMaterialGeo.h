
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

#include "spatial/geo/GeoMaker.h"
#include "util/Erreur.h"
#include "spatial/basic/MapObject.h"

namespace jkt
{

class CSimpleMaterialGeo : public MapObject {
	int m_OffsetMateriaux;		// Sert lors de la lecture des références matériaux
	CMaterial* m_Material;		// Matériau utilisé
	std::string tostring;

	// VBO
	enum {	VBO_VERTEX=0, VBO_NORMAUX };
	static const int VBO_BUFFER_SIZE = 2;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];
	float _minX, _minY, _minZ, _maxX, _maxY, _maxZ;	// Coordonnées du pavé englobant l'objet géo
	float m_Centre[3];			// Centre de la sphère englobant l'objet
	float m_Rayon;				// Rayon de la sphère englobant l'objet
	float* m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	bool m_bSolid;				// Indique si l'objet est solide ou non
	int m_NumVertex;			// Nbre de sommets
	float* m_TabVertex;			// Pointeur sur le tableau de sommets

	int getOffsetMateriau() throw(CErreur);
	void initVBO();
	void freeVBO();
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet géométrique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet géo
	void AfficheNormals();

public:
	static const char* identifier;

	CSimpleMaterialGeo(CMap *map, const std::string& name, CMaterial *mat, unsigned int nbrVertex, float* vertex, float* normals, bool solid=true);
	CSimpleMaterialGeo(CMap *map);
	CSimpleMaterialGeo(const CSimpleMaterialGeo& other);
	~CSimpleMaterialGeo();
	MapObject* clone() override;

	void init() throw(CErreur) override;		// Initialisation de l'objet
	void initGL() override;
	void freeGL() override;

	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux

	void EchangeXY() override;			// Echange les coordonnées X et Y de l'objet
	void EchangeXZ() override;			// Echange les coordonnées X et Z de l'objet
	void EchangeYZ() override;			// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;			// Translation pondérée selon X, Y et Z de l'objet

	bool TestContactPave(const float pos[3], float dist);						// 'pos' est-il dans le pavé constitué des distances min/max de l'objet géo
	void GereContactPlayer(float positionPlayer[3], CPlayer *player) override;
	float GereLaserPlayer(float pos[3], CV3D &Dir, float dist) override;		// Voir la définition de la fonction

	void setMaterial(int matRef);				// Associe l'objet au matériau de référence matRef
	void setNormalVertex(float *tab);			// Implémente les normales aux sommets
	void setSubMat(int *tab);					// Implémente les réf de sous matériau
	void setVertex(int num, float *tab);		// Implémente les sommets
	void setOffsetMateriau(int offset);			// Décale la référence matériau de l'offset

		// Fonctions pour l'interface CGeo
	//bool LitFichier(CIfstreamMap &fichier);			// Lit un objet géo dans un fichier Map
	//bool SaveNameType(ofstream &fichier);				// Sauve le nom du type d'objet géométrique
	//bool SaveFichierMap(ofstream &fichier);			// Sauve l'objet géo dans un fichier Map
	bool Save(TiXmlElement* element) override;
	bool Lit(TiXmlElement* element, MapLogger* mapLogger) override;

	bool checkContact( const float pos[3], float dist ) override;

	void Affiche() override;									// Affiche cet objet géo
	void AfficheHighlighted(float r,float v,float b) override;	// Affiche l'objet en couleur unique
	const char* toString() override;
};

}	// JktMoteur

#endif

