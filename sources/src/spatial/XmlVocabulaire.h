
#ifndef __JKT__XML_H
#define __JKT__XML_H

#include "tinyxml.h"

class Xml
{
public:
	static const char* MAP;
	static const char* IMPORTS;
	static const char* IMPORT;
	static const char* ENTRYPOINTS;
	static const char* ENTRYPOINT;
	static const char* MATERIAUX;
	static const char* LUMIERES;
	static const char* LUMIERE;
	static const char* GEOS;
	static const char* GEO;
	static const char* FAUX;
	static const char* VRAI;
	static const char* VALEUR;
	static const char* SOLIDE;
	static const char* INDEX2SOMMETS2TEXTURE;
	static const char* SOMMETS2TEXTURE;
	static const char* COULEUR;
	static const char* FACES;
	static const char* S;
	static const char* S1;
	static const char* S2;
	static const char* S3;
	static const char* U;
	static const char* V;
	static const char* CANAL;
	static const char* CANAUX;
	static const char* ID;
	static const char* SOMMET;
	static const char* SOMMETS;
	static const char* VECTEURSNORMAUX;
	static const char* SIMPLEMATERIAL;
	static const char* MATERIAU;
	static const char* MULTITEXVERTEX;
	static const char* REF;
	static const char* SIMPLE;
	static const char* MULTI;
	static const char* TEXTURE;
	static const char* AMBIANTE;
	static const char* DIFFUSE;
	static const char* SPECULAR;
	static const char* SOUSMATERIAUX;
	static const char* NBRFACES;
	static const char* SOUSMATERIAU;
	static const char* NBR;
	static const char* FICHIER;
	static const char* NOM;
	static const char* R;
	static const char* G;
	static const char* B;
	static const char* TARGET;
	static const char* OMNI;
	static const char* FALLOFF;
	static const char* POSITION;
	static const char* DIRECTION;
	static const char* X;
	static const char* Y;
	static const char* Z;
	static const char* TYPE;

	// Boîte à outils XML
	static void SaveElement(TiXmlElement* element, const char* name, float valeur);
	static void SaveElement(TiXmlElement* element, const char* name, int valeur);
	static void SaveAttribut(TiXmlElement* element, const char* name, float valeur);
	static void SaveElement(TiXmlElement* element, const char* name, bool valeur);
	static void Lit3fv(TiXmlElement* el, const char* name, const char* X1, const char* X2, const char* X3, float* valeur);
	static void LitCouleur3fv(TiXmlElement* el, const char* name, float* couleur);
	static void LitPosition3fv(TiXmlElement* el, const char* name, float* couleur);
	static void LitDirection3fv(TiXmlElement* el, const char* name, float* direction);
	static unsigned int LitMaterialRef(TiXmlElement* el);
	static double LitValeur(TiXmlElement* el, const char* name);
	static bool LitSolidite(TiXmlElement* el);
};

#endif

