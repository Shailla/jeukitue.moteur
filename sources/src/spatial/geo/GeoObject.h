#pragma warning( disable : 4290 )	// Evite les warnings sur les d�claration de throw

#ifndef __JKT__GEOOBJECT_H
#define __JKT__GEOOBJECT_H

/************************************************************/
/*															*/
/*			Objet affichable 3D classique					*/
/*			et le plus g�n�ral qui soit.					*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/materiau/MaterialTexture.h"

#include "spatial/basic/MapObject.h"


namespace jkt
{

class CC3D;
class CChanTex;
class CMap;

class CGeoObject : public MapObject {
private:
	CMaterial::MAT_TYPE m_TypeMateriau;	// Type du mat�riau associ� (standard, multi,...)
	std::string tostring;

protected:
	// Param�tres d'initialisation
	float minX, minY, minZ, maxX, maxY, maxZ;	// Coordonn�es du pav� englobant l'objet g�o
	float m_Color[3];		// Couleur de l'objet g�o
	bool m_bSolid;			// Indique si l'objet est solide ou non
	float m_Centre[3];		// Centre de la sph�re englobant l'objet
	float m_Rayon;			// Rayon de la sph�re englobant l'objet

	float testContactTriangle( unsigned int i, const float *pos, float dist );

public:
	static const char* identifier;
	bool m_bMaterialTexture;		// Doit �tre � "true" si l'objet g�o est associ� � un mat�riau
	int m_MaterialTexture;			// R�f�rence du mat�riau associ�
	CMaterial *m_Material;		// Mat�riau utilis�
		// Nombres d'�l�ments des tableaux
	int m_NumVertex;			// Nbre de sommets
	int m_NumFaces;				// Nbre d'index de sommets
	int m_NumTexVertex;			// Nbre de sommets de texture, "0" si pas de texture associ�e
	int m_NumTexFaces;			// Nbre d'index de sommets de texture

	float *m_TabVertex;			// Pointeur sur le tableau de sommets
	int *m_TabFaces;			// Pointeur sur le tableau d'index des sommets
	int *m_TabSubMat;			// Index des �ventuels sous-mat�riau
	float *m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs pr�liminaires � la gestion des contacts)
	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux
	std::map<int,CChanTex*> TabTex;	// Contient tous les canaux de texture

		//Constructeurs / Destructeur
	CGeoObject( CMap *Map, unsigned int nbrVertex, unsigned int nbrFaces );
	CGeoObject( CMap *Map );

	~CGeoObject();

		//Fonctions membres
private:
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet g�om�trique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet g�o

	void AfficheWithMaterialMultiTexture( CMaterialMulti *mat );
	void AfficheWithMaterialTexture(CMaterialTexture *mat, int canal);
	void AfficheWithMaterialSimple( CMaterial *mat );
	int getOffsetMateriau() throw(CErreur);

	void AfficheNormals();

public:
	void echangeXY() override;												// Echange les coordonn�es X et Y de l'objet
	void echangeXZ() override;												// Echange les coordonn�es X et Z de l'objet
	void echangeYZ() override;												// Echange les coordonn�es Y et Z de l'objet
	void scale( float scaleX, float scaleY, float scaleZ ) override;		// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	void translate( float x, float y, float z ) override;					// Translation pond�r�e selon X, Y et Z de l'objet

	void setMaterial(int matRef);											// Associe l'objet au mat�riau de r�f�rence matRef
	void Color( float r, float g, float b );								// d�fini la couleur de l'objet

	bool TestContactPave( const float pos[3], float dist );					// 'pos' est-il dans le pav� constitu� des distances min/max de l'objet g�o
	void gereContactPlayer(float positionPlayer[3], CPlayer *player, float deltaTime) override;
	float gereLaserPlayer( float pos[3], CV3D &Dir, float dist) override;	// Voir la d�finition de la fonction

	void setVertex( float *tab, int num );		// Impl�mente les sommets
	void setFaces( int *tab, int num );			// Impl�mente les indices de sommets
	void setNormalVertex( float *tab );			// Impl�mente les normales aux sommets
	void setSubMat( int *tab );					// Impl�mente les r�f de sous mat�riau

	// Fonctions pour l'interface CGeo
	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) override {return true;}
	bool Save(TiXmlElement* element) throw(CErreur) override;

	bool checkContact( const float pos[3], float dist ) override;

	void affiche() override;									// Affiche cet objet g�o
	void afficheHighlighted(float r,float v,float b) override;	// Affiche l'objet en couleur unique

	void init() throw(CErreur) override;						// Initialisation de l'objet
	const char* toString() override;
};

}	// JktMoteur

#endif

