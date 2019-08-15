
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

#include "spatial/geo/ChanTex.h"
#include "spatial/basic/MapObject.h"
#include "spatial/geo/GeoMaker.h"

namespace jkt
{

class CMultiMaterialGeo : public MapObject {
	static const char* identifier;
	std::string tostring;
	CMaterialMulti* m_Material;		// Matériau multiple associé

	float minX, minY, minZ, maxX, maxY, maxZ;	// Coordonnées du pavé englobant l'objet géo
	float m_Centre[3];							// Centre de la sphère englobant l'objet
	float m_Rayon;								// Rayon de la sphère englobant l'objet
	float* m_pNormalTriangle;					// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	bool m_bSolid;								// Indique si l'objet est solide ou non
	int m_NumVertex;							// Nbre de sommets
	float* m_TabVertex;							// Pointeur sur le tableau de sommets
	int getOffsetMateriau() noexcept(false);
		// Canaux et coordonées de texture
	std::map<int,int> m_CanauxNumbers;	// first=numéro de canal ; second=nombre de faces du canal
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
	CMultiMaterialGeo(CMap* map, const std::string& name, CMaterialMulti* mat, unsigned int nbrfaces, float* vertex, float* normals, float* texvertex, std::map<int,int> &canauxnumbers, bool solid=true);
	CMultiMaterialGeo(CMap *map);
	~CMultiMaterialGeo();
	MapObject* clone() override;
	void init() noexcept(false) override;					// Initialisation de l'objet

	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux

	// Transformations
	void echangeXY() override;										// Echange les coordonnées X et Y de l'objet
	void echangeXZ() override;										// Echange les coordonnées X et Z de l'objet
	void echangeYZ() override;										// Echange les coordonnées Y et Z de l'objet
	void scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	void translate(float x, float y, float z) override;				// Translation pondérée selon X, Y et Z de l'objet

	void setMaterial(const std::string& matRef);	// Associe l'objet au matériau de référence matRef
	void setNormalVertex(float *tab);				// Implémente les normales aux sommets
	void setSubMat(int *tab);						// Implémente les réf de sous matériau
	void setVertex(int num, float *tab);			// Implémente les sommets
	void setTexVertex(float *tab);					// Implémente les sommets de texture
	void setOffsetMateriau(int offset);				// Décale la référence matériau de l'offset

	// Affichage
	void affiche() override;									// Affiche cet objet géo
	void afficheHighlighted(float r,float v,float b) override;	// Affiche l'objet en couleur unique

	// Sérialisation
	bool Save(TiXmlElement* element) noexcept(false) override;
	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false) override;

	// Gestion des contacts
	bool checkContact( const float pos[3], float dist ) override;
	bool TestContactPave(const float pos[3], float dist);					// 'pos' est-il dans le pavé constitué des distances min/max de l'objet géo
	void gereContactPlayer(float positionPlayer[3], CPlayer *player) override;
	float gereLaserPlayer(float pos[3], CV3D &Dir, float dist) override;	// Voir la définition de la fonction

	const char* toString() override;
};

}	// JktMoteur

#endif

