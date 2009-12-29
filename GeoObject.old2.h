
#ifndef __JKT__GEOOBJECT_H
#define __JKT__GEOOBJECT_H

/************************************************************/
/*															*/
/*			Objet affichable 3D classique					*/
/*			et le plus général qui soit.					*/
/*															*/
/************************************************************/

#include <map>
#include <fstream>

using namespace std;

#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/materiau/MaterialTexture.h"

#include "spatial/geo/Geo.h"

namespace JKT_PACKAGE_MOTEUR3D
{

class CC3D;
class CMap;

class CChanTex
{
public:
	float *TexVertex;
	int *TexFaces;

	CChanTex() {
		TexVertex = 0;
		TexFaces = 0;
	}

	~CChanTex() {
		if( TexVertex )
			delete[] TexVertex;
		if( TexFaces )
			delete[] TexFaces;
	}
};

class CGeoObject:public CGeo
{
private:
	int m_OffsetMateriaux;		// Sert lors de la lecture des références matériaux
	CMaterial::MAT_TYPE m_TypeMateriau;	// Type du matériau associé (standard, multi,...)
protected:
		// Paramètres d'initialisation
	float minX, minY, minZ, maxX, maxY, maxZ;	// Coordonnées du pavé englobant l'objet géo
	float m_Color[3];		// Couleur de l'objet géo
	bool m_bSolid;				// Indique si l'objet est solide ou non
	float m_Centre[3];			// Centre de la sphère englobant l'objet
	float m_Rayon;				// Rayon de la sphère englobant l'objet

	float testContactTriangle( unsigned int i, const float *pos, float dist );

public:
	bool m_bMaterialTexture;		// Doit être à "true" si l'objet géo est associé à un matériau
	int m_MaterialTexture;			// Référence du matériau associé
	CMaterial *m_Material;		// Matériau utilisé
		// Nombres d'éléments des tableaux
	int m_NumVertex;			// Nbre de sommets
	int m_NumFaces;				// Nbre d'index de sommets
	int m_NumTexVertex;			// Nbre de sommets de texture, "0" si pas de texture associée
	int m_NumTexFaces;			// Nbre d'index de sommets de texture

	float *m_TabVertex;			// Pointeur sur le tableau de sommets
	int *m_TabFaces;			// Pointeur sur le tableau d'index des sommets
	int *m_TabSubMat;			// Index des éventuels sous-matériau
	float *m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
	float *m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux
	map<int,CChanTex*> TabTex;	// Contient tous les canaux de texture

		//Constructeurs / Destructeur
	CGeoObject( CMap *Map, unsigned int nbrVertex, unsigned int nbrFaces );
	CGeoObject( CMap *Map );

	~CGeoObject();

		//Fonctions membres
private:
	void MinMax();			// Calcul les variables MinX,...,MaxZ de cet objet géométrique
	void Bulle();			// Calcul les variables 'centre' et rayon
	void ConstruitBase();	// Construit les vecteurs normaux aux triangles de l'objet géo

	void AfficheWithMaterialMultiTexture( CMaterialMulti *mat );
	void AfficheWithMaterialTexture( CMaterialTexture *mat );
	void AfficheWithMaterialSimple( CMaterial *mat );

public:
	void EchangeXY();			// Echange les coordonnées X et Y de l'objet
	void EchangeXZ();			// Echange les coordonnées X et Z de l'objet
	void EchangeYZ();			// Echange les coordonnées Y et Z de l'objet
	void Scale( float scaleX, float scaleY, float scaleZ );	// Homothétie pondérée selon X, Y et Z de l'objet
	void setMaterial( int matRef );					// Associe l'objet au matériau de référence matRef
	void Color( float r, float g, float b );		// défini la couleur de l'objet
	bool TestContactPave( const float pos[3], float dist );	// 'pos' est-il dans le pavé constitué des distances min/max de l'objet géo
	void GereContactPlayer( const float pos[3], CPlayer *player );
	float GereLaser( float pos[3], CV3D &Dir, float dist);	// Voir la définition de la fonction

	void setVertex( float *tab, int num );		// Implémente les sommets
	void setFaces( int *tab, int num );			// Implémente les indices de sommets
	void setNormalVertex( float *tab );			// Implémente les normales aux sommets
	void setSubMat( int *tab );					// Implémente les réf de sous matériau

		// Fonctions pour l'interface CGeo
	bool LitFichier( CIfstreamMap &fichier );			// Lit un objet géo dans un fichier Map
	bool LitFichierGeoObject( CIfstreamMap &fichier );	// Lit un objet géo dans un fichier
	bool SaveNameType( ofstream &fichier );				// Sauve le nom du type d'objet géométrique
	bool SaveFichierMap( ofstream &fichier );			// Sauve l'objet géo dans un fichier Map
	void OffsetMateriau( int offset );					// Décale la référence matériau de l'offset

	bool Contact( const float pos[3], float dist );

	void Affiche();								// Affiche cet objet géo
	void Init();								// Initialisation de l'objet
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

