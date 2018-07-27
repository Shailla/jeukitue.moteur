
#ifndef __JKT__SIMPLEMATERIALGEO_H
#define __JKT__SIMPLEMATERIALGEO_H

/************************************************************/
/*															*/
/*			Objet affichable 3D simple associ� � un			*/
/*			mat�riau unique. Faces non-index�es,			*/
/*			peut �tre �clair�.								*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

#include <GL/gl.h>

#include "spatial/geo/GeoMaker.h"
#include "util/Erreur.h"
#include "spatial/basic/MapObject.h"

namespace jkt
{

class CSimpleMaterialGeo : public MapObject {
	CMaterial* m_Material;		// Mat�riau utilis�
	std::string tostring;

	// VBO
	enum {	VBO_VERTEX=0, VBO_NORMAUX };
	static const int VBO_BUFFER_SIZE = 2;
	GLuint m_VboBufferNames[VBO_BUFFER_SIZE];
	float _minX, _minY, _minZ, _maxX, _maxY, _maxZ;	// Coordonn�es du pav� englobant l'objet g�o
	float m_Centre[3];			// Centre de la sph�re englobant l'objet
	float m_Rayon;				// Rayon de la sph�re englobant l'objet
	float* m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs pr�liminaires � la gestion des contacts)
	bool m_bSolid;				// Indique si l'objet est solide ou non
	int m_NumVertex;			// Nbre de sommets
	float* m_TabVertex;			// Pointeur sur le tableau de sommets

	int getOffsetMateriau() throw(CErreur);
	void initVBO();
	void freeVBO();
	float testContactTriangle( unsigned int i, const float *pos, float dist );
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet g�om�trique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet g�o
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

	void echangeXY() override;			// Echange les coordonn�es X et Y de l'objet
	void echangeXZ() override;			// Echange les coordonn�es X et Z de l'objet
	void echangeYZ() override;			// Echange les coordonn�es Y et Z de l'objet
	void scale(float scaleX, float scaleY, float scaleZ) override;	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;			// Translation pond�r�e selon X, Y et Z de l'objet

	bool TestContactPave(const float pos[3], float dist);						// 'pos' est-il dans le pav� constitu� des distances min/max de l'objet g�o
	void gereContactPlayer(float positionPlayer[3], CPlayer *player) override;
	float gereLaserPlayer(float pos[3], CV3D &Dir, float dist) override;		// Voir la d�finition de la fonction

	void setMaterial(const std::string& matRef);				// Associe l'objet au mat�riau de r�f�rence matRef
	void setNormalVertex(float *tab);			// Impl�mente les normales aux sommets
	void setSubMat(int *tab);					// Impl�mente les r�f de sous mat�riau
	void setVertex(int num, float *tab);		// Impl�mente les sommets
	void setOffsetMateriau(int offset);			// D�cale la r�f�rence mat�riau de l'offset

		// Fonctions pour l'interface CGeo
	//bool LitFichier(CIfstreamMap &fichier);			// Lit un objet g�o dans un fichier Map
	//bool SaveNameType(ofstream &fichier);				// Sauve le nom du type d'objet g�om�trique
	//bool SaveFichierMap(ofstream &fichier);			// Sauve l'objet g�o dans un fichier Map
	bool Save(TiXmlElement* element) throw(CErreur) override;
	bool Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) throw(CErreur) override;

	bool checkContact( const float pos[3], float dist ) override;

	void affiche() override;									// Affiche cet objet g�o
	void afficheHighlighted(float r,float v,float b) override;	// Affiche l'objet en couleur unique
	const char* toString() override;
};

}	// JktMoteur

#endif

