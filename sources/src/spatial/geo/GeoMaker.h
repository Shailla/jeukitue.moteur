#pragma warning( disable : 4290 )	// Evite les warnings sur les d�claration de throw

#ifndef __JKT__GEOMAKER_H
#define __JKT__GEOMAKER_H

/**	Cette classe a pour r�le de servir de r�ceptable des donn�es concernant un
	nouvel objet g�om�trique, puis une fois toutes les donn�es disponibles,
	elle sert surtout � cr�er un objet g�om�trique (CGeo) optimis� pour
	le type d'affichage de l'objet (avec ou sans texture, indexation,...).
*/

#include <map>
#include <fstream>

#include "tinyxml.h"

#include "util/Erreur.h"
#include "spatial/MapLogger.h"

#include "spatial/basic/MapObject.h"

namespace jkt
{
class CMap;
class CSimpleGeo;
class CSimpleMaterialGeo;
class CMultiMaterialGeo;
class CTextureMaterialGeo;
class CMaterial;
class CMaterialSimple;
class CMaterialMulti;
class CMaterialTexture;
class CChanTex;
class CTexVertexList;

class CGeoMaker
{
	std::string m_Nom;	// Nom de l'objet
	CMap* m_Map;

	int m_NumColor;		// Valeur 0, 3 ou 4 qui correspond � 'pas de couleur', 'couleur sans alpha' ou 'couleur avec alpha'
	float *m_Color;

	// Mat�riau
	bool m_bMaterialTexture;	// "true" si l'objet est associ� � un mat�riau
	int m_MaterialRef;			// R�f�rence du mat�riau associ�
	int m_OffsetMateriau;
	int* m_TabSubMat;			// Index des �ventuels sous-mat�riau

	// Sommets
	int m_NumVertex;			// Nbre de sommets
	float* m_TabVertex;			// Pointeur sur le tableau de sommets

	// Index de sommets
	int m_NumFaces;				// Nbre d'index de sommets
	int* m_TabFaces;			// Pointeur sur le tableau d'index des sommets

	// Vecteurs normaux
	float* m_TabVectNormaux;	// Tableau des vecteurs normaux

	// Coordonn�es de texture
	CTexVertexList *m_TexVertexListe;	// Liste des tableaux de coordonn�es de texture

	// Divers
	bool m_bSolid;

	int getOffsetMateriau() throw(CErreur);
public:
	std::map<int,CChanTex*> m_TabChanTex;	// Contient tous les canaux de texture

	//Constructeurs / Destructeur
	CGeoMaker(CMap* map);
	~CGeoMaker();

	void setOffsetMateriau(int offset);
	void setName(const std::string& nom);
	void setColor3(float r, float v, float b);
	void setColor4(float r, float v, float b, float a);
	void setVertex(int nbr, float* tab);	// Données du tableau des sommets
	void setFaces(int nbr, int* tab);		// Données du tableau des index de faces
	void setNormals(int nbr, float* tab) throw(jkt::CErreur);	// Données du tableau des vecteurs normaux
	void setMaterialRef(int ref);			// Associe l'objet à un matériau
	void setSubMat(int* tab);
	static void SaveVertex(TiXmlElement* element, unsigned int nbr, float* vertex);		// Sauve les sommets d'un objet géo
	static void SaveFaces(TiXmlElement* element, unsigned int nbr, int* faces);	// Sauve les sommets de texture d'un object géo
	static void SaveTexVertex(TiXmlElement* element, unsigned int nbr, float* texvertex);	// Sauve les sommets de texture d'un object géo
	static void SaveTexIndex(TiXmlElement* element, unsigned int nbr, int* texindex);	// Sauve les sommets de texture d'un object géo
	static void SaveSolidite(TiXmlElement* element, bool solidite);	// Sauve les sommets de texture d'un object géo
	static void SaveMateriau(TiXmlElement* element, unsigned int refMat);
	static void SaveCouleur3fv(TiXmlElement* element, const char* name, float* couleur);	// Sauve les sommets de texture d'un object géo
	static void SaveVecteursNormaux(TiXmlElement* element,unsigned int nbr, float* normaux);
	static void SaveSousMateriaux(TiXmlElement* element, std::map<int,int>& m_CanauxNumbers);
	static void SaveMultiTexVertex(TiXmlElement *element, CTexVertexList* texvertexliste);
	static int* LitFaces(TiXmlElement *element, int& nbr);
	static float* LitVertex(TiXmlElement *element, int& nbr) throw(CErreur);
	static float* LitTexVertex(TiXmlElement *element, int& nbr) throw(CErreur);
	static CTexVertexList* LitMultiTexVertex(TiXmlElement* element);
	static float* LitVecteursNormaux(TiXmlElement* element, int& nbr) throw(CErreur);
	static void LitSousMateriaux(TiXmlElement* el, std::map<int, int>& canauxNumbers) throw(CErreur);

	static MapObject* Lit(TiXmlElement* el, CMap& pMap, MapLogger* mapLogger) throw(CErreur);
	MapObject* makeNewGeoInstance();	// Crée une instance de l'objet géométrique
private:
	CSimpleGeo* makeSimpleGeo();
	CSimpleMaterialGeo* makeSimpleMaterialGeo(CMaterial* mat);
	CMultiMaterialGeo* makeMultiMaterialGeo(CMaterialMulti* mat);
	CTextureMaterialGeo* makeTextureMaterialGeo(CMaterialTexture* mat);

	void lineariseVertex();		// Désindexe les sommets
	void lineariseTexVertex();	// Désindexe les coord. de texture dans leurs canaux
	void optimiseSubMat(std::map<int,int> &canauxnumbers);
};

}	// JktMoteur

#endif

