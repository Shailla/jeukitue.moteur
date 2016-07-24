
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#include "spatial/XmlVocabulaire.h"
#include "spatial/materiau/Material.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/materiau/MaterialMaker.h"
#include "spatial/IfstreamMap.h"
#include "util/Trace.h"

#include "MaterialMulti.h"

namespace jkt
{

CMaterialMulti::CMaterialMulti() : CMaterial() {
	m_Type = MAT_TYPE_MULTI;
	m_TabMat = 0;			// Pas de sous-matériaux
	m_NbrTex = 0;			// Pas de sous-matériaux
}

int CMaterialMulti::NbrTex()
{	return m_NbrTex;	}

void CMaterialMulti::NbrTex( int nbr ) {
	m_NbrTex = nbr;
	m_TabMat = new CMaterial*[nbr];	// Création du tableau des sous-matériaux

	for( int i=0 ; i<nbr ; i++ )
		m_TabMat[i] = NULL;
}

CMaterial* CMaterialMulti::getMat(int i) {
	return m_TabMat[i];
}

CMaterialMulti::~CMaterialMulti() {
	if( m_TabMat ) {
		for( int i=0 ; i<m_NbrTex ; i++ )
			if( m_TabMat[ i ] )
				delete m_TabMat[ i ];

		delete[] m_TabMat;	// Création du tableau des sous-matériaux
	}
}

void CMaterialMulti::initGL() {
	cout << endl << "Lecture des fichiers de texture multiple (ref. " << getRef() << ")";

	for(int i=0 ; i<NbrTex() ; i++)
		m_TabMat[i]->initGL();
}

void CMaterialMulti::freeGL()  {
	cout << endl << "Libération des fichiers de texture multiple (ref. " << getRef() << ")";

	for(int i=0 ; i<NbrTex() ; i++)
		m_TabMat[i]->freeGL();
}

bool CMaterialMulti::LitFichier(CIfstreamMap &fichier) {
	string mot;
	int nbrSM;		// Nombre de sous-matériaux

	fichier >> mot;
	if( mot!="Reference" )
		return false;

	fichier >> m_Ref;			// Référence du matériau

	fichier >> mot;				// "Fichier de texture"
	if( mot!="NbrSousMateriaux" )
		return false;	// Fichier corrompu

	fichier >> nbrSM;
	NbrTex( nbrSM );

	fichier >> mot;
	if( mot!="Ambient" )
		return false;	// Fichier corrompu

	fichier >> m_Ambient[0] >> m_Ambient[1] >> m_Ambient[2];

	fichier >> mot;
	if( mot!="Diffuse" )
		return false;	// Fichier corrompu
	fichier >> m_Diffuse[0] >> m_Diffuse[1] >> m_Diffuse[2];

	fichier >> mot;
	if( mot!="Specular" )
		return false;

	fichier >> m_Specular[0] >> m_Specular[1] >> m_Specular[2];

	fichier >> mot;
	if( mot!="Sous-materiaux" )
		return false;

	fichier >> mot;
	if( mot!="debut" )
		return false;

	for(int i=0 ; i<NbrTex() ; i++) {
		fichier >> mot;

		if(mot=="MateriauSimple") {
			CMaterial *pMatSimple = new CMaterial();
			if(!pMatSimple->LitFichier(fichier)) {
				cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Materiau simple corrompu dans un multi-materiau" << endl;
				delete pMatSimple;
				return false;
			}

			m_TabMat[ i ] = pMatSimple;
		}
		else if( mot=="MateriauTexture" ) {
			CMaterialTexture *pMatTex = new CMaterialTexture();
			if(!pMatTex->LitFichier(fichier)) {
				cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Materiau de texture corrompu dans un multi-materiau" << endl;
				delete pMatTex;
				return false;
			}

			m_TabMat[ i ] = pMatTex;
		}
		else {
			return false;
		}
	}

	fichier >> mot;
	if( mot!="Sous-materiaux" )
		return false;

	fichier >> mot;
	if( mot!="fin" )
		return false;

	return true;
}

bool CMaterialMulti::SaveFichierMap(ofstream &fichier) {
	fichier << "\n\tNbrSousMateriaux\t" << NbrTex();

	fichier << "\n\tAmbient\t\t" << m_Ambient[0] << "\t" << m_Ambient[1] << "\t" << m_Ambient[2];
	fichier << "\n\tDiffuse\t\t" << m_Diffuse[0] << "\t" << m_Diffuse[1] << "\t" << m_Diffuse[2];
	fichier << "\n\tSpecular\t" << m_Specular[0] << "\t" << m_Specular[1] << "\t" << m_Specular[2];

	fichier << "\nSous-materiaux debut";

	for(int i=0 ; i<NbrTex() ; i++) {
		m_TabMat[ i ]->SaveFichierMap( fichier );
	}

	fichier << "\nSous-materiaux fin";

	return true;
}

bool CMaterialMulti::Lit(TiXmlElement* element, string& repertoire, MapLogger* mapLogger) {
	// Référence
	double ref;
	if(!element->Attribute(Xml::REF, &ref))
		throw CErreur("Fichier map corrompu : Reference materiau");

	m_Ref = (unsigned int)ref;

	// Couleurs
	Xml::LitCouleur3fv(element, Xml::AMBIANTE, m_Ambient);
	Xml::LitCouleur3fv(element, Xml::DIFFUSE, m_Diffuse);
	Xml::LitCouleur3fv(element, Xml::SPECULAR, m_Specular);

	TiXmlElement* elSma = element->FirstChildElement(Xml::SOUSMATERIAUX);

	if(!elSma) {
		mapLogger->logError("Fichier map corrompu : Sous-materiaux");
		throw CErreur("Fichier map corrompu : Sous-materiaux");
	}


	// Nombre de sous-matériaux
	double nbrSSMat;
	if(!elSma->Attribute(Xml::NBR, &nbrSSMat)) {
		mapLogger->logError("Fichier map corrompu : Nombre de sous-materiaux");
		throw CErreur("Fichier map corrompu : Nombre de sous-materiaux");
	}

	m_NbrTex = (int)nbrSSMat;

	// Liste des sous-matériaux
	int i=0;
	m_TabMat = new CMaterial*[m_NbrTex];

	for(TiXmlElement* el=elSma->FirstChildElement(Xml::MATERIAU); el!=0; el=el->NextSiblingElement()) {
		el->Value();
		el->Attribute(Xml::REF);

		if(i >= NbrTex()) {
			mapLogger->logError("Fichier map corrompu : Reference sous-materiau");
			throw CErreur("Fichier map corrompu : Reference sous-materiau");
		}

		CMaterial* mat = CMaterialMaker::Lit(el, repertoire, mapLogger);
		m_TabMat[i++] = mat;
	}

	return true;
}

bool CMaterialMulti::Save(TiXmlElement* element) {
	// Nom, référence...
	TiXmlElement* elMat = new TiXmlElement(Xml::MATERIAU);
	elMat->SetAttribute(Xml::TYPE, Xml::MULTI);
	elMat->SetAttribute(Xml::REF, getRef());
	element->LinkEndChild(elMat);

	// Couleurs
	CGeoMaker::SaveCouleur3fv(elMat, Xml::AMBIANTE, m_Ambient);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::DIFFUSE, m_Diffuse);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::SPECULAR, m_Specular);

	// Sous-matériaux
	TiXmlElement* elSma = new TiXmlElement(Xml::SOUSMATERIAUX);
	elSma->SetAttribute(Xml::NBR, NbrTex());
	for( int i=0 ; i<NbrTex() ; i++ )
		m_TabMat[i]->Save(elSma);

	elMat->LinkEndChild(elSma);

	return true;
}

const char* CMaterialMulti::toString() {
	return CMaterial::toString();
}

}	// JktMoteur
