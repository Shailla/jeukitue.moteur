
#ifndef __JKT__XML_H
#define __JKT__XML_H

#include "tinyxml.h"

#include "util/Erreur.h"

class Xml
{
public:
	static const char* MAP;
	static const char* IMPORTS;
	static const char* IMPORT;
	static const char* IMPORT_MODE_ADD;
	static const char* IMPORT_MODE_MERGE;
	static const char* ENTRYPOINTS;
	static const char* ENTRYPOINT;
	static const char* MATERIAUX;
	static const char* LIGHTS;
	static const char* LUMIERE;
	static const char* GEOS;
	static const char* GEO;
	static const char* GEODESCRIPTION;
	static const char* GEOINSTANCE;
	static const char* FAUX;
	static const char* VRAI;
	static const char* VALEUR;
	static const char* GEOSIMPLE;
	static const char* GEOSIMPLEMATERIAL;
	static const char* GEOOBJECT;
	static const char* GEOTEXTURE;
	static const char* GEOMULTI;
	static const char* GEOGROUP;
	static const char* SOLIDE;
	static const char* INDEX2SOMMETS2TEXTURE;
	static const char* SOMMETS2TEXTURE;
	static const char* COULEUR;
	static const char* DESCRIPTION;
	static const char* FACES;
	static const char* S;
	static const char* S1;
	static const char* S2;
	static const char* S3;
	static const char* U;
	static const char* V;
	static const char* PLUGINS;
	static const char* LOAD;
	static const char* CANAL;
	static const char* CANAUX;
	static const char* ID;
	static const char* SOMMET;
	static const char* SOMMETS;
	static const char* VECTEURSNORMAUX;
	static const char* MATERIAU;
	static const char* MATERIAUSIMPLE;
	static const char* MATERIAUTEXTURE;
	static const char* MATERIAUMULTI;
	static const char* MULTITEXVERTEX;
	static const char* ABSTRACT;
	static const char* REF;
	static const char* TEXTURE;
	static const char* AMBIANTE;
	static const char* DIFFUSE;
	static const char* SPECULAR;
	static const char* SOUSMATERIAUX;
	static const char* TRANSFORMATION;
	static const char* NBRFACES;
	static const char* SOUSMATERIAU;
	static const char* MODE;
	static const char* NBR;
	static const char* FICHIER;
	static const char* NOM;
	static const char* R;
	static const char* G;
	static const char* B;
	static const char* LIGHTTARGET;
	static const char* LIGHTOMNI;
	static const char* FALLOFF;
	static const char* POSITION;
	static const char* DIRECTION;
	static const char* TRANSLATE;
	static const char* SCALE;
	static const char* X;
	static const char* Y;
	static const char* Z;
	static const char* W;
	static const char* T1;
	static const char* T2;
	static const char* T3;
	static const char* T4;
	static const char* TYPE;
	static const char* PARTICULES_ENGINES;
	static const char* NEIGE;
	static const char* NBR_PARTICULES;
	static const char* NBR_PARTICULES_ON_GROUND;
	static const char* CENTRE;
	static const char* DIMENSION;

	// Boîte à outils XML
	static void throwCorruptedMapFileException(const char* expected, const char* value) throw(jkt::CErreur);
	static void SaveElement(TiXmlElement* element, const char* name, float valeur);
	static void SaveElement(TiXmlElement* element, const char* name, int valeur);
	static void SaveAttribut(TiXmlElement* element, const char* name, float valeur);
	static void SaveElement(TiXmlElement* element, const char* name, bool valeur);
	static bool Lit3fv(TiXmlElement* el, const char* name, const char* X1, const char* X2, const char* X3, float valeur[3]) throw(jkt::CErreur);
	static bool Lit4fv(TiXmlElement* el, const char* name, const char* X1, const char* X2, const char* X3, const char* x4, float valeur[4]) throw(jkt::CErreur);
	static bool LitCouleur3fv(TiXmlElement* el, const char* name, float couleur[3]);
	static bool LitPosition3fv(TiXmlElement* el, const char* name, float couleur[3]);
	static bool LitDirection3fv(TiXmlElement* el, const char* name, float direction[3]);
	static std::string LitMaterialRef(TiXmlElement* el) throw(jkt::CErreur);
	static double LitValeur(TiXmlElement* el, const char* name);
	static bool LitSolidite(TiXmlElement* el) throw(jkt::CErreur);
	static bool LitBooleanMandatory(const char* value) throw(jkt::CErreur);
	static bool LitBooleanNotMandatory(const char* value, bool defaultValue) throw(jkt::CErreur);
};

#endif

