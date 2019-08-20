
#include <sstream>

#include "util/Erreur.h"

#include "spatial/XmlVocabulaire.h"

using namespace std;
using namespace jkt;

// Map
const char* Xml::MAP = "Map";
const char* Xml::IMPORTS = "Imports";
const char* Xml::ENTRYPOINTS = "EntryPoints";
const char* Xml::MATERIAUX = "Materiaux";
const char* Xml::LIGHTS = "Lights";
const char* Xml::GEOS = "Geos";

// Plugins
const char* Xml::PLUGINS = "Plugins";
const char* Xml::LOAD = "Load";

// Imports
const char* Xml::IMPORT = "Import";
const char* Xml::IMPORT_MODE_ADD = "add";
const char* Xml::IMPORT_MODE_MERGE = "merge";

// Objets
const char* Xml::GEO = "Geo";
const char* Xml::GEODESCRIPTION = "GeoDescription";
const char* Xml::GEOINSTANCE = "GeoInstance";
const char* Xml::LUMIERE = "Lumiere";

const char* Xml::MATERIAU = "Materiau";

const char* Xml::ENTRYPOINT = "EntryPoint";		// Points d'entr�e des joueurs

const char* Xml::PARTICULES_ENGINES = "ParticulesEngines";
const char* Xml::NBR_PARTICULES = "NbrParticules";						// Nombre de particules � afficher dans le moteur de neige
const char* Xml::NBR_PARTICULES_ON_GROUND = "NbrParticulesOnGround";	// Nombre de particules � afficher dans le moteur de neige
const char* Xml::NEIGE = "Neige";										// Moteur de particules de neige

// Opérations
const char* Xml::SCALE = "Scale";
const char* Xml::TRANSLATE = "Translate";

// Géo
const char* Xml::GEOSIMPLE = "GeoSimple";
const char* Xml::GEOSIMPLEMATERIAL = "GeoSimpleMaterial";
const char* Xml::GEOOBJECT = "GeoOBbject";
const char* Xml::GEOTEXTURE = "GeoTexture";
const char* Xml::GEOMULTI = "GeoMulti";
const char* Xml::GEOGROUP = "GeoGroup";
const char* Xml::SOLIDE = "Solide";
const char* Xml::INDEX2SOMMETS2TEXTURE = "IndexDeSommetsDeTexture";
const char* Xml::SOMMETS2TEXTURE = "SommetsDeTexture";
const char* Xml::FACES = "Faces";
const char* Xml::S = "S";
const char* Xml::S1 = "S1";
const char* Xml::S2 = "S2";
const char* Xml::S3 = "S3";
const char* Xml::SOMMET = "Sommet";
const char* Xml::SOMMETS = "Sommets";
const char* Xml::VECTEURSNORMAUX = "VecteursNormaux";

const char* Xml::NBRFACES = "NbrFaces";

// Matériaux
const char* Xml::MATERIAUSIMPLE = "MateriauSimple";
const char* Xml::MATERIAUTEXTURE = "MateriauTexture";
const char* Xml::MATERIAUMULTI = "MateriauMulti";
const char* Xml::SOUSMATERIAU = "SousMateriau";
const char* Xml::SOUSMATERIAUX = "SousMateriaux";
const char* Xml::TRANSFORMATION = "Transformation";
const char* Xml::TEXTURE = "Texture";
const char* Xml::CANAL = "Canal";
const char* Xml::CANAUX = "Canaux";
const char* Xml::ID = "Id";
const char* Xml::MULTITEXVERTEX = "MultiTexVertex";

// Lumières
const char* Xml::FALLOFF = "FallOff";
const char* Xml::LIGHTOMNI = "LightOmni";
const char* Xml::LIGHTTARGET = "LightTarget";

// Spatial
const char* Xml::CENTRE = "Centre";
const char* Xml::DIMENSION = "Dimension";
const char* Xml::DIRECTION = "Direction";
const char* Xml::POSITION = "Position";

const char* Xml::X = "X";
const char* Xml::Y = "Y";
const char* Xml::Z = "Z";
const char* Xml::W = "W";

const char* Xml::U = "U";
const char* Xml::V = "V";

const char* Xml::T1 = "T1";
const char* Xml::T2 = "T2";
const char* Xml::T3 = "T3";
const char* Xml::T4 = "T4";

// Couleur
const char* Xml::AMBIANTE = "Ambiante";
const char* Xml::DIFFUSE = "Diffuse";
const char* Xml::SPECULAR = "Specular";

const char* Xml::R = "R";
const char* Xml::G = "G";
const char* Xml::B = "B";

// Divers
const char* Xml::COULEUR = "Couleur";
const char* Xml::DESCRIPTION = "Description";
const char* Xml::FAUX = "false";
const char* Xml::FICHIER = "Fichier";
const char* Xml::MODE = "Mode";
const char* Xml::NBR = "Nbr";
const char* Xml::NOM = "Nom";
const char* Xml::REF = "Ref";
const char* Xml::ABSTRACT = "Abstract";
const char* Xml::TYPE = "Type";
const char* Xml::VALEUR = "Valeur";
const char* Xml::VRAI = "true";

void Xml::throwCorruptedMapFileException(const char* expected, const char* value) {
	string erreur = "Fichier MAP corrompu, element '";
	erreur += expected;
	erreur += "' attendu, mais '";
	erreur += value;
	erreur += "' trouve";
	throw CErreur(erreur);
}

void Xml::SaveElement(TiXmlElement* element, const char* name, float valeur) {
	stringstream ss;
	ss << valeur;

	TiXmlElement* el = new TiXmlElement(name);
	el->SetAttribute(Xml::VALEUR, ss.str().c_str());

	element->LinkEndChild(el);
}

void Xml::SaveElement(TiXmlElement* element, const char* name, int valeur) {
	TiXmlElement* el = new TiXmlElement(name);
	el->SetAttribute(Xml::VALEUR, valeur);

	element->LinkEndChild(el);
}

void Xml::SaveElement(TiXmlElement* element, const char* name, bool valeur) {
	TiXmlElement* el = new TiXmlElement(name);

	if(valeur)
		el->SetAttribute(Xml::VALEUR, Xml::VRAI);
	else
		el->SetAttribute(Xml::VALEUR, Xml::FAUX);

	element->LinkEndChild(el);
}

void Xml::SaveAttribut(TiXmlElement* element, const char* name, float valeur) {
	stringstream ss;
	ss << valeur;

	element->SetAttribute(name, ss.str().c_str());
}

bool Xml::Lit3fv(TiXmlElement* el, const char* name, const char* X1, const char* X2, const char* X3, float valeur[3]) {
	double x1, x2, x3;

	if(!el)
		throw CErreur("Fichier map corrompu : Lit3fv ");

	TiXmlElement *elLig = el->FirstChildElement(name);

	if(elLig) {
		if(!elLig->Attribute(X1, &x1))
			throw CErreur("Xml::LitCouleur3fv> Fichier map corrompu : Lit3fv X1");

		if(!elLig->Attribute(X2, &x2))
			throw CErreur("Xml::LitCouleur3fv> Fichier map corrompu : Lit3fv X2");

		if(!elLig->Attribute(X3, &x3))
			throw CErreur("Xml::LitCouleur3fv> Fichier map corrompu : Lit3fv X3");

		valeur[0] = (float)x1;
		valeur[1] = (float)x2;
		valeur[2] = (float)x3;

		return true;
	}
	else {
		return false;
	}
}

bool Xml::Lit4fv(TiXmlElement* el, const char* name, const char* X1, const char* X2, const char* X3, const char* X4, float valeur[4]) {
	double x1, x2, x3, x4;

	if(!el)
		throw CErreur("Fichier map corrompu : Lit4fv ");

	TiXmlElement *elLig = el->FirstChildElement(name);

	if(elLig) {
		if(!elLig->Attribute(X1, &x1))
			throw CErreur("Xml::LitCouleur3fv> Fichier map corrompu : Lit4fv X1");

		if(!elLig->Attribute(X2, &x2))
			throw CErreur("Xml::LitCouleur3fv>Fichier map corrompu : Lit4fv X2");

		if(!elLig->Attribute(X3, &x3))
			throw CErreur("Xml::LitCouleur3fv>Fichier map corrompu : Lit4fv X3");

		if(!elLig->Attribute(X4, &x4))
			throw CErreur("Xml::LitCouleur3fv>Fichier map corrompu : Lit4fv X4");

		valeur[0] = (float)x1;
		valeur[1] = (float)x2;
		valeur[2] = (float)x3;
		valeur[3] = (float)x4;

		return true;
	}
	else {
		return false;
	}
}

bool Xml::LitCouleur3fv(TiXmlElement* el, const char* name, float couleur[3]) {
	return Lit3fv(el, name, Xml::R, Xml::G, Xml::B, couleur);
}

bool Xml::LitPosition3fv(TiXmlElement* el, const char* name, float position[3]) {
	return Lit3fv(el, name, Xml::X, Xml::Y, Xml::Z, position);
}

bool Xml::LitDirection3fv(TiXmlElement* el, const char* name, float direction[3]) {
	return Lit3fv(el, name, Xml::X, Xml::Y, Xml::Z, direction);
}

string Xml::LitMaterialRef(TiXmlElement* el) {
	TiXmlElement* elMat = el->FirstChildElement(Xml::MATERIAU);

	if(!elMat)
		throw CErreur("Fichier Map corrompu : LitMaterialRef 1");

	const char* matRef = elMat->Attribute(Xml::REF);

	if(!matRef)
		throw CErreur("Fichier Map corrompu : LitMaterialRef 2 - La référence du matériau est manquante");

	return matRef;
}

double Xml::LitValeur(TiXmlElement* el, const char* name) {
	double valeur;
	TiXmlElement* elVal = el->FirstChildElement(name);

	if(!elVal)
		throw CErreur("Fichier Map corrompu : LitValeur 1");

	if(!elVal->Attribute(Xml::VALEUR, &valeur))
		throw CErreur("Fichier Map corrompu : LitValeur 2");

	return valeur;
}

bool Xml::LitSolidite(TiXmlElement* el) {
	TiXmlElement* elSol = el->FirstChildElement(Xml::SOLIDE);
	bool solidite;

	if(!elSol)
		throw CErreur("Fichier Map corrompu : solidite");

	const char* sol = elSol->Attribute(Xml::VALEUR);

	if(!sol)
		throw CErreur("Fichier Map corrompu : solidite");

	if(!strcmp(sol, Xml::VRAI))
		solidite = true;
	else if(!strcmp(sol, Xml::FAUX))
		solidite = false;
	else
		throw CErreur("Fichier Map corrompu : solidite");

	return solidite;
}

bool Xml::LitBooleanMandatory(const char* value) {
	if(!value)
		throw CErreur("Fichier Map corrompu : boolean manquant");

	if(!strcmp(value, Xml::VRAI))
		return true;
	else if(!strcmp(value, Xml::FAUX))
		return false;
	else
		throw CErreur("Fichier Map corrompu : expected 'true' or 'false'");
}

bool Xml::LitBooleanNotMandatory(const char* value, bool defaultValue) {
	if(!value)
		return defaultValue;

	if(!strcmp(value, Xml::VRAI))
		return true;
	else if(!strcmp(value, Xml::FAUX))
		return false;
	else
		throw CErreur("Fichier Map corrompu : expected 'true', 'false' or null");
}
