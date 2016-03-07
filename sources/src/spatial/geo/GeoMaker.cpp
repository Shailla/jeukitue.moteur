
#include <string>
#include <fstream>
#include <set>
#include <cmath>
#include <map>
#include <sstream>

#ifdef WIN32
	#include <windows.h>
#endif

using namespace std;

class CGame;

#include "spatial/XmlVocabulaire.h"
#include "util/Tableau.cpp"
#include "util/math_vectoriel.h"
#include "util/V3D.h"

#include "spatial/geo/TexVertexList.h"
#include "spatial/materiau/Material.h"
#include "spatial/geo/Geo.h"
#include "spatial/geo/SimpleGeo.h"
#include "spatial/geo/SimpleMaterialGeo.h"
#include "spatial/geo/MultiMaterialGeo.h"
#include "spatial/geo/TextureMaterialGeo.h"
#include "spatial/geo/ChanTex.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"
#include "spatial/Map.h"

#include "spatial/geo/GeoMaker.h"

using namespace JktUtils;

class CCfg;

namespace JktMoteur
{

		//Constructeurs / Destructeur
CGeoMaker::CGeoMaker(CMap *map) {
	m_Map = map;
	m_Color = NULL;

	m_OffsetMateriau = -1;
	m_bMaterialTexture = false;	// "true" si l'objet est associé à un matériau
	m_MaterialRef = 0;		// Référence du matériau associé

	m_NumVertex = 0;			// Nbre de sommets
	m_TabVertex = NULL;			// Pointeur sur le tableau de sommets

	m_NumFaces = 0;				// Nbre d'index de sommets
	m_TabFaces = NULL;				// Pointeur sur le tableau d'index des sommets

	m_TabVectNormaux = NULL;		// Tableau des vecteurs normaux

	m_TabSubMat = NULL;			// Index des éventuels sous-matériau

	m_bSolid = true;

	m_TexVertexListe = NULL;
	m_NumColor = 0;
}

CGeoMaker::~CGeoMaker()
{
	if(m_Color) {
		delete[] m_Color;
	}
	if(m_TabVertex) {
		delete[] m_TabVertex;
	}
	if(m_TabFaces) {
		delete[] m_TabFaces;
	}
	if(m_TabVectNormaux) {
		delete[] m_TabVectNormaux;
	}
	if(m_TabSubMat) {
		delete[] m_TabSubMat;
	}
}

void CGeoMaker::setOffsetMateriau(int offset)
{
	m_OffsetMateriau = offset;
}

int CGeoMaker::getOffsetMateriau() throw(CErreur)
{
	if(m_OffsetMateriau < 0) {
		throw CErreur("GeoMaker erreur : m_OffsetMateriau non initialisé");
	}

	return m_OffsetMateriau;
}

void CGeoMaker::setName(const string& nom) {
	m_Nom = nom;
}

void CGeoMaker::setColor3(float r, float v, float b) {
	if(m_Color) {
		delete[] m_Color;
	}

	m_NumColor = 3;
	m_Color = new float[3];

	m_Color[0] = r;
	m_Color[1] = v;
	m_Color[2] = b;
}

void CGeoMaker::setColor4(float r, float v, float b, float a) {
	if(m_Color) {
		delete[] m_Color;
	}

	m_NumColor = 4;
	m_Color = new float[4];

	m_Color[0] = r;
	m_Color[1] = v;
	m_Color[2] = b;
	m_Color[3] = a;
}

void CGeoMaker::setVertex(int nbr, float* tab)	// Données du tableau des sommets
{
	m_NumVertex = nbr;
	m_TabVertex = tab;
}

void CGeoMaker::setFaces(int nbr, int* tab)		// Données du tableau des index de faces
{
	if(m_TabFaces) {
		delete[] m_TabFaces;
	}

	m_NumFaces = nbr;
	m_TabFaces = tab;
}

void CGeoMaker::setNormals(int nbr, float* tab)	throw(CErreur)
{		// Données du tableau des vecteurs normaux
	if(nbr != m_NumFaces*9)	// Vérification du nombre de vecteurs normaux
	{
		throw CErreur("Nombre de normales incorrect");
	}

	if(m_TabVectNormaux) {
		delete[] m_TabVectNormaux;
	}

	m_TabVectNormaux = tab;
}

void CGeoMaker::setMaterialRef(int ref)
{
	m_bMaterialTexture = true;
	m_MaterialRef = ref;
}

void CGeoMaker::setSubMat(int* tab)
{
	if(m_TabSubMat) {
		delete[] m_TabSubMat;
	}

	m_TabSubMat = new int[ m_NumFaces ];	// Création du tableau des références
											// sous-matériaux
	for( int i=0 ; i<m_NumFaces ; i++ )
		m_TabSubMat[ i ] = tab[ i ];	// Copie des références sous-matériaux
}

CGeo* CGeoMaker::makeNewGeoInstance()	// Crée une instance de l'objet géométrique
{							// optimisé correspondant aux données receuillies
	CGeo* geo = NULL;

	if( m_bMaterialTexture )	// S'il y a un matériau associé
	{
		int nbrMat = m_MaterialRef + getOffsetMateriau();	// Décalage de la référence matériau de l'offset demandé

			// Vérification du type de matériau
		if( nbrMat >= (int)m_Map->m_TabMaterial.size() )
		{
			stringstream txt;
			txt << "Erreur (CGeoMaker::makeNewGeoInstance) : Materiau introuvable, réf=";
			txt << nbrMat;
			throw CErreur(txt);
		}

		CMaterial *mat = m_Map->m_TabMaterial[ nbrMat ];

		if(mat->Type() == CMaterial::MAT_TYPE_SIMPLE)
		{
			geo = makeSimpleMaterialGeo((CMaterial*)mat);
		}
		else if(mat->Type() == CMaterial::MAT_TYPE_TEXTURE)
		{
			if(m_TabVertex && (m_TabChanTex.size()==1))
				geo = makeTextureMaterialGeo((CMaterialTexture*)mat);
		}
		else if(mat->Type() == CMaterial::MAT_TYPE_MULTI)
		{
			if(m_TabVertex && (m_TabChanTex.size()==1))	// S'il n'y a qu'un seul mapping de texture
				geo = makeMultiMaterialGeo((CMaterialMulti*)mat);
		}
	}
	else
	{
			// S'il n'y a pas de matériau
		geo = makeSimpleGeo();
	}

	if(geo) {
		geo->Init();
	}

	if(!geo)
		cout << endl << "Objet ignore : " << m_Nom;

	return geo;
}

CSimpleMaterialGeo* CGeoMaker::makeSimpleMaterialGeo(CMaterial* mat) {
	// Désindexe les sommets
	if(m_TabFaces)
		lineariseVertex();

	CSimpleMaterialGeo* geo = new CSimpleMaterialGeo(m_Map, m_Nom, mat, m_NumVertex, m_TabVertex, m_TabVectNormaux);

	m_TabVectNormaux = 0;
	m_TabVertex = 0;

	return geo;
}

CTextureMaterialGeo* CGeoMaker::makeTextureMaterialGeo(CMaterialTexture* mat)
{
	// Enlève l'indexation des vertex
	lineariseVertex();

	// Enlève l'indexation des sommets de texture
	lineariseTexVertex();

	// Récupère et copie le canal de texture unique
	unsigned int numTV = m_TabChanTex[1]->getNumTexVertex();
	float* texvertex = new float[numTV*2];
	float* tab = m_TabChanTex[1]->getTexVertex();
	for(unsigned int i=0 ; i<numTV*2 ; i++)
		texvertex[i] = tab[i];

	CTextureMaterialGeo* geo = new CTextureMaterialGeo(m_Map, m_Nom, mat, m_NumVertex, m_TabVertex, m_TabVectNormaux, texvertex);

	m_TabVertex = 0;
	m_TabVectNormaux = 0;

	return geo;
}

CMultiMaterialGeo* CGeoMaker::makeMultiMaterialGeo(CMaterialMulti* mat) {
	map<int,int> canauxnumbers;

	// Linéarise les sommets
	lineariseVertex();			// Enlève l'indexation des vertex

	// Linéarise les coordonnées de texture de tous les canaux
	lineariseTexVertex();

	// Rassemble les sommets/texvertex/normals par canal de texture
	// pour un affichage plus rapide
	optimiseSubMat(canauxnumbers);

	int nbrTexVertex = m_TexVertexListe->getNbrTexVertex(0);
	float* texVertex = new float[nbrTexVertex*2];
	for(int i=0 ; i<nbrTexVertex*2 ; i++)
		texVertex[i] = m_TexVertexListe->getTexVertex(0)[i];

	// Création de l'objet géométrique
	CMultiMaterialGeo* geo = new CMultiMaterialGeo(m_Map, m_Nom, mat, m_NumVertex, m_TabVertex, m_TabVectNormaux, texVertex, canauxnumbers);

	cout << "\nmakeMultiMaterialGeo";
	for(int p=0 ; p<10 ; p++)
		cout << endl << m_TabVertex[p];

	m_TabVectNormaux = 0;
	m_TabVertex = 0;

	return geo;
}

void CGeoMaker::optimiseSubMat(map<int,int> &canauxnumbers)	// Rassemble les faces associés au même sous-matériau
{
	float* vertexRes = new float[m_NumVertex*3];
	float* normalsRes = 0;
	if(m_TabVectNormaux)
		normalsRes = new float[m_NumVertex*3];
	vector<int> listeSousMateriaux;	// Liste des numéros de sous-matériaux

	// Liste les sous-matériaux
	for(int i=0; i<m_NumFaces; i++)
	{
		bool indic = true;

		vector<int>::iterator iter = listeSousMateriaux.begin();

			// Vérifie si le sous-matériau est déjà dans la liste
		for( ; iter!=listeSousMateriaux.end(); iter++)
		{
			if((*iter) == m_TabSubMat[i])	// S'il y est
				indic = false;			// ne pas l'ajouter à la liste
		}

		if(indic)	// Si le matériau n'est pas encore dans liste
			listeSousMateriaux.push_back(m_TabSubMat[i]);
	}

	// Associe les canaux aux index des sommets dans l'objet
	map<int,vector<int> > canalordre;	// First = canal
										// Second = liste des index de sommets associés

	vector<int>::iterator iter = listeSousMateriaux.begin();
	for( ; iter!=listeSousMateriaux.end() ; iter++)
	{
		int sousmat = *iter;	// Sous-matériau
		vector<int> ordre;		// Index des sommets de du canal 'sousmat'

		for(int i=0; i<m_NumFaces; i++)
			if(m_TabSubMat[i] == sousmat)
				ordre.push_back(i);

		canalordre[sousmat] = ordre;
	}

	cout << endl << "LISTE DES SOUS-MATERIAUX, NOMBRE DE FACES ET ORDRE: ";
	map<int,vector<int> >::iterator iterLL = canalordre.begin();
	for(; iterLL!=canalordre.end() ; iterLL++)
	{
		cout << endl << "Canal " << iterLL->first << " " << iterLL->second.size() << "\t" << endl;
		vector<int>::iterator iterMM = iterLL->second.begin();
		for(; iterMM!=iterLL->second.end() ; iterMM++)
			cout << " " << *iterMM;
	}

	// Initialisation de la liste des coordonnées de texture
	m_TexVertexListe = new CTexVertexList((int)m_TabChanTex.size());
	for(int k=0 ; k<m_TexVertexListe->getNbrCanaux() ; k++)
		m_TexVertexListe->setTexVertex(k, new float[m_NumVertex*2], m_NumVertex);

	int index = 0;
	int canal = 0;
	map<int, vector<int> >::iterator iterMap = canalordre.begin();

	for( ; iterMap!=canalordre.end() ; iterMap++)
	{
		vector<int> ordre = iterMap->second;

		// Regroupement des données du canal 'canal'
		vector<int>::iterator iterOrdre = ordre.begin();

		for( ; iterOrdre!=ordre.end() ; iterOrdre++)
		{
			int indexSource = *iterOrdre;

			// Déplacements des 3 sommets de la face
			vertexRes[index*9 + 0] = m_TabVertex[9*indexSource + 0];
			vertexRes[index*9 + 1] = m_TabVertex[9*indexSource + 1];
			vertexRes[index*9 + 2] = m_TabVertex[9*indexSource + 2];
			vertexRes[index*9 + 3] = m_TabVertex[9*indexSource + 3];
			vertexRes[index*9 + 4] = m_TabVertex[9*indexSource + 4];
			vertexRes[index*9 + 5] = m_TabVertex[9*indexSource + 5];
			vertexRes[index*9 + 6] = m_TabVertex[9*indexSource + 6];
			vertexRes[index*9 + 7] = m_TabVertex[9*indexSource + 7];
			vertexRes[index*9 + 8] = m_TabVertex[9*indexSource + 8];

			// Déplacements des 3 vecteurs normaux de la face de la face
			if(normalsRes)
			{
				normalsRes[index*9 + 0] = m_TabVectNormaux[9*indexSource + 0];
				normalsRes[index*9 + 1] = m_TabVectNormaux[9*indexSource + 1];
				normalsRes[index*9 + 2] = m_TabVectNormaux[9*indexSource + 2];
				normalsRes[index*9 + 3] = m_TabVectNormaux[9*indexSource + 3];
				normalsRes[index*9 + 4] = m_TabVectNormaux[9*indexSource + 4];
				normalsRes[index*9 + 5] = m_TabVectNormaux[9*indexSource + 5];
				normalsRes[index*9 + 6] = m_TabVectNormaux[9*indexSource + 6];
				normalsRes[index*9 + 7] = m_TabVectNormaux[9*indexSource + 7];
				normalsRes[index*9 + 8] = m_TabVectNormaux[9*indexSource + 8];
			}

			// Déplacement des coordonnées de texture
			map<int,CChanTex*>::iterator iterCanal = m_TabChanTex.begin();
			for(int m=0 ; m<m_TexVertexListe->getNbrCanaux() ; m++)
			{
				float* tvSource = iterCanal->second->getTexVertex();
				float* tvDest = m_TexVertexListe->getTexVertex(m);

				tvDest[index*6 + 0] = tvSource[6*indexSource + 0];
				tvDest[index*6 + 1] = tvSource[6*indexSource + 1];
				tvDest[index*6 + 2] = tvSource[6*indexSource + 2];
				tvDest[index*6 + 3] = tvSource[6*indexSource + 3];
				tvDest[index*6 + 4] = tvSource[6*indexSource + 4];
				tvDest[index*6 + 5] = tvSource[6*indexSource + 5];

				iterCanal++;
			}

			index++;
		}

		// Nombre de sommets concerné par le canal de texture
		canauxnumbers[canal++] = (int)ordre.size();
	}

	assert(index == m_NumFaces);

	// Copie des résultats dans les tableaux et suppression des tableaux temporaires

	// Sommets
	delete[] m_TabVertex;
	m_TabVertex = vertexRes;

	// Vecteurs normaux
	delete[] m_TabVectNormaux;
	m_TabVectNormaux = normalsRes;
}

void CGeoMaker::lineariseTexVertex()
{
	map<int, CChanTex*>::iterator iter = m_TabChanTex.begin();

	for( ; iter!=m_TabChanTex.end() ; iter++)
	{
		CChanTex* canalsource = iter->second;
		float* sourcetexvertex = canalsource->getTexVertex();
		int* sourcetexfaces = canalsource->getTexFaces();

		float* desttexvertex = new float[m_NumVertex*2];

		if(sourcetexfaces)
		{
			// Copie avec indexation
			for(int i=0; i<m_NumVertex; i++)
			{
				desttexvertex[2*i + 0] = sourcetexvertex[ 2*sourcetexfaces[i] + 0];
				desttexvertex[2*i + 1] = sourcetexvertex[ 2*sourcetexfaces[i] + 1];
			}
		}
		else
		{
			// Copie sans indexation
			for(int i=0; i<m_NumFaces; i++)
			{
				desttexvertex[2*i + 0] = sourcetexvertex[ 2*i + 0];
				desttexvertex[2*i + 1] = sourcetexvertex[ 2*i + 1];
			}
		}

		canalsource->setTexFaces(0, 0);
		canalsource->setTexVertex(m_NumVertex, desttexvertex);
	}
}

void CGeoMaker::lineariseVertex()	// Renvoie le tableau de vertex sous forme non-indexée
{
	if(m_TabFaces)
	{
		m_NumVertex = m_NumFaces*3;
		float* vertex = new float[3*m_NumVertex];

			// Linéarise les vertex à partir du tableau des indices et des vertex indexés
		for(int i=0; i<m_NumVertex ; i++)	// Pour chaque face de la forme
		{
			vertex[(i*3) + 0] = m_TabVertex[(3*m_TabFaces[i]) + 0];
			vertex[(i*3) + 1] = m_TabVertex[(3*m_TabFaces[i]) + 1];
			vertex[(i*3) + 2] = m_TabVertex[(3*m_TabFaces[i]) + 2];
		}

		delete[] m_TabVertex;
		m_TabVertex = vertex;
	}
}

CSimpleGeo* CGeoMaker::makeSimpleGeo()
{
	CSimpleGeo* geo;

	geo = new CSimpleGeo(m_Map,m_Nom,m_NumVertex,m_TabVertex,m_NumFaces,m_TabFaces,m_Color,m_bSolid);

		// Oublie les éléments qui ont été transmis à l'instance de CSimpleGeo
		// afin qu'ils ne soient pas supprimés par l'instance de CGeoMaker
	m_TabVertex = NULL;
	m_TabFaces = NULL;

	if(m_Color) {
		delete[] m_Color;
	}

	m_Color = NULL;

	return geo;
}

void CGeoMaker::SaveVertex(TiXmlElement *element, unsigned int nbr, float* vertex)
{
	stringstream ss0, ss1, ss2;

		// Sommets
	TiXmlElement* elSom = new TiXmlElement("Sommets");
	elSom->SetAttribute(Xml::NBR, nbr);

	for(unsigned int i=0 ; i<nbr ; i++)
	{
		stringstream ss0, ss1, ss2;

		ss0 << vertex[3*i + 0];
		ss1 << vertex[3*i + 1];
		ss2 << vertex[3*i + 2];

		TiXmlElement* el = new TiXmlElement("S");
		el->SetAttribute("X", ss0.str().c_str());
		el->SetAttribute("Y", ss1.str().c_str());
		el->SetAttribute("Z", ss2.str().c_str());

		elSom->LinkEndChild(el);
	}

	element->LinkEndChild(elSom);
}

float* CGeoMaker::LitVertex(TiXmlElement *element, int &nbr)
{
	TiXmlElement* elSom = element->FirstChildElement(Xml::SOMMETS);
	if(!elSom) {
		string str = "Fichier Map corrompu : LitVertex - ";
		str += element->Value();
		throw CErreur(str);
	}

	// Nombre de sommets
	if(!elSom->Attribute(Xml::NBR, &nbr))
		throw CErreur("Fichier Map corrompu : LitVertex 2");

	// Vertex
	float* vertex = new float[nbr*3];
	int i=0;

	double a, b, c;
	for(TiXmlElement* el=elSom->FirstChildElement(Xml::S); el!=0; el=el->NextSiblingElement())
	{
		if(i >= nbr) {
			delete[] vertex;
			throw CErreur("Fichier Map corrompu : LitVertex 7");
		}

		if(!el->Attribute(Xml::X, &a)) {
			delete[] vertex;
			throw CErreur("Fichier Map corrompu : LitVertex 4");
		}

		if(!el->Attribute(Xml::Y, &b)) {
			delete[] vertex;
			throw CErreur("Fichier Map corrompu : LitVertex 5");
		}

		if(!el->Attribute(Xml::Z, &c)) {
			delete[] vertex;
			throw CErreur("Fichier Map corrompu : LitVertex 6");
		}

		vertex[3*i + 0] = (float)a;
		vertex[3*i + 1] = (float)b;
		vertex[3*i + 2] = (float)c;

		i++;
	}

	if(i != nbr) {
		delete[] vertex;
		throw CErreur("Fichier Map corrompu : Pas assez de sommets");
	}

	return vertex;
}

float* CGeoMaker::LitTexVertex(TiXmlElement *element, int &nbr)
{
	double nombre;

	TiXmlElement* elSom = element->FirstChildElement(Xml::SOMMETS2TEXTURE);
	if(!elSom) {
		string str = "Fichier Map corrompu : LitTexVertex - ";
		str += element->Value();
		throw CErreur(str);
	}

	// Nombre de sommets
	if(!elSom->Attribute(Xml::NBR, &nombre))
		throw CErreur("Fichier Map corrompu : LitTexVertex 2");
	nbr = (unsigned int)nombre;

	// Sommets de texture
	float* texvertex = new float[nbr*2];
	int i=0;

	double a, b;
	for(TiXmlElement* el=elSom->FirstChildElement(Xml::S); el!=0; el=el->NextSiblingElement())
	{
		if(i >= nbr) {
			delete[] texvertex;
			throw CErreur("Fichier Map corrompu : Pas assez de sommets de texture");
		}

		if(!el->Attribute(Xml::U, &a)) {
			delete[] texvertex;
			throw CErreur("Fichier Map corrompu : LitTexVertex 4");
		}

		if(!el->Attribute(Xml::V, &b)) {
			delete[] texvertex;
			throw CErreur("Fichier Map corrompu : LitTexVertex 5");
		}

		texvertex[2*i + 0] = (float)a;
		texvertex[2*i + 1] = (float)b;

		i++;
	}

	if(i != nbr) {
		delete[] texvertex;
		throw CErreur("Fichier Map corrompu : Trop de sommets de texture");
	}

	return texvertex;
}

CTexVertexList* CGeoMaker::LitMultiTexVertex(TiXmlElement* element)
{
	double nombre;
	CTexVertexList* texVertexListe = 0;

	TiXmlElement* elMulti = element->FirstChildElement(Xml::MULTITEXVERTEX);
	if(!elMulti) {
		string str = "Fichier Map corrompu : LitMultiTexVertex - ";
		str += element->Value();
		throw CErreur(str);
	}

	// Nombre de listes de sommets de texture
	if(!elMulti->Attribute(Xml::NBR, &nombre))
		throw CErreur("Fichier Map corrompu : LitMultiTexVertex 2");
	int nbrCanaux = (unsigned int)nombre;
	texVertexListe = new CTexVertexList(nbrCanaux);

	int nbrTexVertex = 0;
	float* texVertex = 0;
	for(int i=0 ; i<nbrCanaux ; i++)
	{
		// Lecture d'un tableau de sommets de texture
		texVertex = LitTexVertex(elMulti, nbrTexVertex);
		texVertexListe->setTexVertex(i, texVertex, nbrTexVertex);
	}

	return texVertexListe;
}

float* CGeoMaker::LitVecteursNormaux(TiXmlElement *element, int& nbr)
{
	TiXmlElement* elSom = element->FirstChildElement(Xml::VECTEURSNORMAUX);
	if(!elSom)
		return 0;

	// Nombre de sommets
	if(!elSom->Attribute(Xml::NBR, &nbr))
		throw CErreur("Fichier Map corrompu : LitVecteursNormaux 2");

	// Vecteurs normaux
	float* vectnormaux = new float[nbr*3];
	int i=0;

	double a, b, c;
	for(TiXmlElement* el=elSom->FirstChildElement(Xml::S); el!=0; el=el->NextSiblingElement())
	{
		if(i >= nbr) {
			delete[] vectnormaux;
			throw CErreur("Fichier Map corrompu : Trop de vecteurs normaux");
		}

		if(!el->Attribute(Xml::X, &a)) {
			delete[] vectnormaux;
			throw CErreur("Fichier Map corrompu : LitVecteursNormaux 4");
		}

		if(!el->Attribute(Xml::Y, &b)) {
			delete[] vectnormaux;
			throw CErreur("Fichier Map corrompu : LitVecteursNormaux 5");
		}

		if(!el->Attribute(Xml::Z, &c)) {
			delete[] vectnormaux;
			throw CErreur("Fichier Map corrompu : LitVecteursNormaux 6");
		}

		vectnormaux[3*i + 0] = (float)a;
		vectnormaux[3*i + 1] = (float)b;
		vectnormaux[3*i + 2] = (float)c;

		i++;
	}

	if(i != nbr) {
		delete[] vectnormaux;
		throw CErreur("Fichier Map corrompu : Pas assez de vecteurs normaux");
	}

	return vectnormaux;
}

void CGeoMaker::SaveVecteursNormaux(TiXmlElement *element, unsigned int nbr, float* normaux)
{
	TiXmlElement* elSom = new TiXmlElement(Xml::VECTEURSNORMAUX);
	elSom->SetAttribute(Xml::NBR, nbr);

	for(unsigned int i=0 ; i<nbr ; i++)
	{
		stringstream ss0, ss1, ss2;

		ss0 << normaux[3*i + 0];
		ss1 << normaux[3*i + 1];
		ss2 << normaux[3*i + 2];

		TiXmlElement* el = new TiXmlElement("S");
		el->SetAttribute(Xml::X, ss0.str().c_str());
		el->SetAttribute(Xml::Y, ss1.str().c_str());
		el->SetAttribute(Xml::Z, ss2.str().c_str());

		elSom->LinkEndChild(el);
	}

	element->LinkEndChild(elSom);
}

void CGeoMaker::SaveTexVertex(TiXmlElement *element, unsigned int nbr, float* texvertex)
{
		// Sommets
	TiXmlElement* elSom = new TiXmlElement(Xml::SOMMETS2TEXTURE);
	elSom->SetAttribute(Xml::NBR, nbr);

	for(unsigned int i=0 ; i<nbr ; i++)
	{
		stringstream ss0, ss1;

		ss0 << texvertex[2*i + 0];
		ss1 << texvertex[2*i + 1];

		TiXmlElement* el = new TiXmlElement(Xml::S);
		el->SetAttribute(Xml::U, ss0.str().c_str());
		el->SetAttribute(Xml::V, ss1.str().c_str());

		elSom->LinkEndChild(el);
	}

	element->LinkEndChild(elSom);
}

void CGeoMaker::SaveMultiTexVertex(TiXmlElement *element, CTexVertexList* texvertexliste)
{
		// Sommets
	TiXmlElement* elMulti = new TiXmlElement(Xml::MULTITEXVERTEX);
	elMulti->SetAttribute(Xml::NBR, texvertexliste->getNbrCanaux());

	for(int canal=0 ; canal<texvertexliste->getNbrCanaux() ; canal++)
		SaveTexVertex(elMulti, texvertexliste->getNbrTexVertex(canal), texvertexliste->getTexVertex(canal));

	element->LinkEndChild(elMulti);
}

void CGeoMaker::SaveCouleur3fv(TiXmlElement* element, const char* name, float* couleur)
{
	stringstream ss0, ss1, ss2;
	ss0 << couleur[0];
	ss1 << couleur[1];
	ss2 << couleur[2];

		// Couleur
	TiXmlElement* elCol = new TiXmlElement(name);
	elCol->SetAttribute(Xml::R, ss0.str().c_str());
	elCol->SetAttribute(Xml::G, ss1.str().c_str());
	elCol->SetAttribute(Xml::B, ss2.str().c_str());
	element->LinkEndChild(elCol);
}

void CGeoMaker::SaveFaces(TiXmlElement *element, unsigned int nbr, int* faces)
{
			// Index des sommets
	TiXmlElement* elFac = new TiXmlElement(Xml::FACES);
	elFac->SetAttribute(Xml::NBR, nbr);
	element->LinkEndChild(elFac);

	for(unsigned int i=0 ; i<nbr ; i++)
	{
		TiXmlElement* el = new TiXmlElement(Xml::SOMMET);
		el->SetAttribute(Xml::S1, faces[3*i + 0]);
		el->SetAttribute(Xml::S2, faces[3*i + 1]);
		el->SetAttribute(Xml::S3, faces[3*i + 2]);

		elFac->LinkEndChild(el);
	}
}

int* CGeoMaker::LitFaces(TiXmlElement *element, int &nbr)
{
	TiXmlElement* elFac = element->FirstChildElement(Xml::FACES);
	if(!elFac)
		throw CErreur("Fichier Map corrompu : LitFaces 1");

	// Nombre de sommets
	if(!elFac->Attribute(Xml::NBR, &nbr))
		throw CErreur("Fichier Map corrompu : LitFaces 2");

	// Vertex
	int* faces = new int[nbr*3];
	int i=0;

	double a, b, c;
	for(TiXmlElement* el=elFac->FirstChildElement(Xml::SOMMET); el!=0; el=el->NextSiblingElement())
	{
		if(i >= nbr) {
			delete[] faces;
			throw CErreur("Fichier Map corrompu : Trop de faces");
		}

		if(!el->Attribute(Xml::S1, &a)) {
			delete[] faces;
			throw CErreur("Fichier Map corrompu : S1 manquant");
		}

		if(!el->Attribute(Xml::S2, &b)) {
			delete[] faces;
			throw CErreur("Fichier Map corrompu : S2 manquant");
		}

		if(!el->Attribute(Xml::S3, &c)) {
			delete[] faces;
			throw CErreur("Fichier Map corrompu : S3 manquant");
		}

		faces[3*i + 0] = (int)a;
		faces[3*i + 1] = (int)b;
		faces[3*i + 2] = (int)c;

		i++;
	}

	if(i != nbr) {
		delete[] faces;
		throw CErreur("Fichier Map corrompu : Il manque des faces");
	}

	return faces;
}

void CGeoMaker::SaveTexIndex(TiXmlElement *element, unsigned int nbr, int* texindex)
{
			// Index des sommets
	TiXmlElement* elFac = new TiXmlElement(Xml::INDEX2SOMMETS2TEXTURE);
	elFac->SetAttribute(Xml::NBR, nbr);
	element->LinkEndChild(elFac);

	for(unsigned int i=0 ; i<nbr ; i++)
		Xml::SaveElement(elFac, "I", texindex[3*i]);
}

void CGeoMaker::SaveSolidite(TiXmlElement* element, bool solidite)
{
	TiXmlElement* elSol = new TiXmlElement(Xml::SOLIDE);

	if(solidite)
		elSol->SetAttribute(Xml::VALEUR, Xml::VRAI);
	else
		elSol->SetAttribute(Xml::VALEUR, Xml::FAUX);

	element->LinkEndChild(elSol);
}

void CGeoMaker::SaveMateriau(TiXmlElement* element, unsigned int refMat)
{
	TiXmlElement* elMat = new TiXmlElement(Xml::MATERIAU);

	elMat->SetAttribute(Xml::REF, refMat);

	element->LinkEndChild(elMat);
}

CGeo* CGeoMaker::Lit(TiXmlElement* el, CMap* pMap)
{
	const char* type = el->Attribute(Xml::TYPE);
	CGeo* geo = 0;

	if(!strcmp(Xml::SIMPLE, type)) {
		geo = new CSimpleGeo(pMap);
		geo->Lit(el);
	}
	else if(!strcmp(Xml::SIMPLEMATERIAL, type)) {
		CSimpleMaterialGeo* geoSM = new CSimpleMaterialGeo(pMap);
		geoSM->setOffsetMateriau(0);
		geo = geoSM;
		geo->Lit(el);
	}
	else if(!strcmp(Xml::TEXTURE, type)) {
		CTextureMaterialGeo* geoTM = new CTextureMaterialGeo(pMap);
		geoTM->setOffsetMateriau(0);
		geo = geoTM;
		geo->Lit(el);
	}
	else if(!strcmp(Xml::MULTI, type)) {
		CMultiMaterialGeo* geoMM = new CMultiMaterialGeo(pMap);
		geoMM->setOffsetMateriau(0);
		geo = geoMM;
		geo->Lit(el);
	}
	else
		throw JktUtils::CErreur("Fichier Map corrompu : Geo de type inconnu");

	return geo;
}

map<int,int>* CGeoMaker::LitSousMateriaux(TiXmlElement* el)
{
	double nbr, var1, var2;
	map<int,int>* canaux = new map<int,int>();
	map<int,int>& refcanaux = *canaux;

	cout << "\n--" << el->Value();
	cout << "\n--" << el->Attribute("Nom");

	TiXmlElement* elCan = el->FirstChildElement(Xml::SOUSMATERIAUX);

	if(!elCan) {
		string str = "Fichier Map corrompu : LitSousMateriaux 1 - ";
		str += el->Value();
		throw CErreur(str);
	}

	if(!elCan->Attribute(Xml::NBR, &nbr)) {
		string str = "Fichier Map corrompu : LitSousMateriaux 2 - ";
		str += el->Value();
		throw CErreur(str);
	}

	TiXmlElement* elC = elCan->FirstChildElement(Xml::SOUSMATERIAU);

	for( ; elC!=0 ; elC=elC->NextSiblingElement()) {
		if(!elC->Attribute(Xml::ID, &var1)) {
			string str = "Fichier Map corrompu : LitSousMateriaux 3 - ";
			str += elC->Value();
			throw CErreur(str);
		}

		if(!elC->Attribute(Xml::NBRFACES, &var2)) {
			string str = "Fichier Map corrompu : LitSousMateriaux 4 - ";
			str += elC->Value();
			throw CErreur(str);
		}

		refcanaux[(int)var1] = (int)var2;
	}

	if(canaux->size() != nbr) {
		string str = "Fichier Map corrompu : LitSousMateriaux 5 - ";
		str += el->Value();
		throw CErreur(str);
	}

	return canaux;
}

void CGeoMaker::SaveSousMateriaux(TiXmlElement* el, map<int,int>& canaux)
{
	TiXmlElement* elCan = new TiXmlElement(Xml::SOUSMATERIAUX);
	map<int,int>::iterator iter;
	elCan->SetAttribute(Xml::NBR, (int)canaux.size());

	for(iter=canaux.begin(); iter!=canaux.end(); iter++)
	{
		TiXmlElement* elC = new TiXmlElement(Xml::SOUSMATERIAU);
		elC->SetAttribute(Xml::ID,(*iter).first);
		elC->SetAttribute(Xml::NBRFACES,(*iter).second);

		elCan->LinkEndChild(elC);
	}
	el->LinkEndChild(elCan);
}

}	// JktMoteur
