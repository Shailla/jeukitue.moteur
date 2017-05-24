
#ifndef __JKT__TEXTUREMATERIALGEO_H
#define __JKT__TEXTUREMATERIALGEO_H

/************************************************************/
/*															*/
/*			Objet affichable 3D simple associé à un			*/
/*			matériau unique. Faces non-indexées,			*/
/*			peut être éclairé.								*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

#include "tinyxml.h"

#include "spatial/geo/ChanTex.h"
#include "spatial/basic/MapObject.h"
#include "spatial/geo/GeoMaker.h"

namespace jkt
{

class CTextureMaterialGeo : public MapObject {
	bool _isGlInitialized;

	int m_OffsetMateriaux;		// Sert lors de la lecture des références matériaux
	std::string tostring;
	CMaterialTexture* m_Material;	// Matériau multiple associé
	float _minX, _minY, _minZ, _maxX, _maxY, _maxZ;	// Coordonnées du pavé englobant l'objet géo
	float m_Centre[3];			// Centre de la sphère englobant l'objet
	float m_Rayon;				// Rayon de la sphère englobant l'objet
	float* m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	bool m_bSolid;				// Indique si l'objet est solide ou non
	int m_NumVertex;				// Nbre de faces
	float* m_TabVertex;			// Pointeur sur le tableau de sommets
	int getOffsetMateriau() throw(CErreur);
	float* m_TabTexVertex;			// Coordonnées de texture
	// VBO
	enum {	VBO_VERTEX=0, VBO_NORMAUX, VBO_TEXVERTEX };
	static const int VBO_BUFFER_SIZE = 3;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];

	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet géométrique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet géo
	void AfficheNormals();

	void setMaterial(int matRef);	// Associe l'objet au matériau de référence matRef
	void setNormalVertex(float *tab);			// Implémente les normales aux sommets
	void setVertex(int num, float *tab);		// Implémente les sommets
	void setTexVertex(int num, float *tab);		// Implémente les sommets de texture

public:
	static const char* identifier;

	CTextureMaterialGeo(CMap* map, const std::string& name, CMaterialTexture* mat, unsigned int nbrfaces, float* vertex, float* normals, float* texvertex, bool solid=true);
	CTextureMaterialGeo(CMap *map);
	CTextureMaterialGeo(const CTextureMaterialGeo& geo);
	~CTextureMaterialGeo();
	MapObject* clone() override;

	void init() throw(CErreur) override;				// Initialisation de l'objet
	void initGL() override;
	void freeGL() override;

	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux

	void EchangeXY() override;										// Echange les coordonnées X et Y de l'objet
	void EchangeXZ() override;										// Echange les coordonnées X et Z de l'objet
	void EchangeYZ() override;										// Echange les coordonnées Y et Z de l'objet
	void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;			// Translation pondérée selon X, Y et Z de l'objet

	bool TestContactPave(const float pos[3], float dist);	// 'pos' est-il dans le pavé constitué des distances min/max de l'objet géo
	void GereContactPlayer(float positionPlayer[3], CPlayer *player) override;
	float GereLaserPlayer(float pos[3], CV3D &Dir, float dist) override;	// Voir la définition de la fonction

		// Fonctions pour l'interface CGeo
	//bool LitFichier(CIfstreamMap &fichier);			// Lit un objet géo dans un fichier Map
	//bool SaveNameType(ofstream &fichier);				// Sauve le nom du type d'objet géométrique
	//bool SaveFichierMap(ofstream &fichier);			// Sauve l'objet géo dans un fichier Map
	bool Save(TiXmlElement* element) override;
	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) override;

	void setOffsetMateriau(int offset);			// Décale la référence matériau de l'offset
	bool checkContact( const float pos[3], float dist ) override;

	void Affiche() override;							// Affiche cet objet géo
	void AfficheHighlighted(float r,float v,float b) override;		// Affiche l'objet en couleur unique
	const char* toString() override;
};

}	// JktMoteur

#endif

