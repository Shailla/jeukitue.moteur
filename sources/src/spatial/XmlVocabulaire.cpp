
#include <sstream>

using namespace std;

#include "util/Erreur.h"

#include "spatial/XmlVocabulaire.h"

using namespace JktUtils;

// Map
const char* Xml::MAP = "Map";
const char* Xml::ENTRYPOINTS = "EntryPoints";
const char* Xml::MATERIAUX = "Materiaux";
const char* Xml::LUMIERES = "Lumieres";
const char* Xml::GEOS = "Geos";

// Points d'entrée joeurs
const char* Xml::ENTRYPOINT = "EntryPoint";

// Géo
const char* Xml::GEO = "Geo";
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
const char* Xml::SIMPLEMATERIAL = "SimpleMaterial";
const char* Xml::NBRFACES = "NbrFaces";

// Matériaux
const char* Xml::MATERIAU = "Materiau";
const char* Xml::MULTI = "Multi";
const char* Xml::SIMPLE = "Simple";
const char* Xml::SOUSMATERIAU = "SousMateriau";
const char* Xml::SOUSMATERIAUX = "SousMateriaux";
const char* Xml::TEXTURE = "Texture";
const char* Xml::CANAL = "Canal";
const char* Xml::CANAUX = "Canaux";
const char* Xml::ID = "Id";
const char* Xml::MULTITEXVERTEX = "MultiTexVertex";
// Lumières
const char* Xml::FALLOFF = "FallOff";
const char* Xml::LUMIERE = "Lumiere";
const char* Xml::OMNI = "Target";
const char* Xml::TARGET = "Target";

// Spatial
const char* Xml::DIRECTION = "Direction";
const char* Xml::X = "X";
const char* Xml::Y = "Y";
const char* Xml::Z = "Z";
const char* Xml::U = "U";
const char* Xml::V = "V";

// Couleur
const char* Xml::AMBIANTE = "Ambiante";
const char* Xml::DIFFUSE = "Diffuse";
const char* Xml::POSITION = "Position";
const char* Xml::SPECULAR = "Specular";

const char* Xml::R = "R";
const char* Xml::G = "G";
const char* Xml::B = "B";

// Divers
const char* Xml::COULEUR = "Couleur";
const char* Xml::FAUX = "false";
const char* Xml::FICHIER = "Fichier";
const char* Xml::NBR = "Nbr";
const char* Xml::NOM = "Nom";
const char* Xml::REF = "Ref";
const char* Xml::TYPE = "Type";
const char* Xml::VALEUR = "Valeur";
const char* Xml::VRAI = "true";

void Xml::SaveElement(TiXmlElement* element, const char* name, float valeur)
{
	stringstream ss;
	ss << valeur;

	TiXmlElement* el = new TiXmlElement(name);
	el->SetAttribute(Xml::VALEUR, ss.str().c_str());

	element->LinkEndChild(el);
}

void Xml::SaveElement(TiXmlElement* element, const char* name, int valeur)
{
	TiXmlElement* el = new TiXmlElement(name);
	el->SetAttribute(Xml::VALEUR, valeur);

	element->LinkEndChild(el);
}

void Xml::SaveElement(TiXmlElement* element, const char* name, bool valeur)
{
	TiXmlElement* el = new TiXmlElement(name);

	if(valeur)
		el->SetAttribute(Xml::VALEUR, Xml::VRAI);
	else
		el->SetAttribute(Xml::VALEUR, Xml::FAUX);

	element->LinkEndChild(el);
}

void Xml::SaveAttribut(TiXmlElement* element, const char* name, float valeur)
{
	stringstream ss;
	ss << valeur;

	element->SetAttribute(name, ss.str().c_str());
}

void Xml::Lit3fv(TiXmlElement* el, const char* name, const char* X1, const char* X2, const char* X3, float* valeur)
{
	double x1, x2, x3;

	if(!el)
		throw CErreur(0, "Fichier map corrompu : Lit3fv ");

	TiXmlElement *elLig = el->FirstChildElement(name);

	if(!elLig->Attribute(X1, &x1))
		throw CErreur(0, "Xml::LitCouleur3fv> Fichier map corrompu : Lit3fv X1");

	if(!elLig->Attribute(X2, &x2))
		throw CErreur(0, "Xml::LitCouleur3fv>Fichier map corrompu : Lit3fv X2");

	if(!elLig->Attribute(X3, &x3))
		throw CErreur(0, "Xml::LitCouleur3fv>Fichier map corrompu : Lit3fv X3");

	valeur[0] = (float)x1;
	valeur[1] = (float)x2;
	valeur[2] = (float)x3;
}

void Xml::LitCouleur3fv(TiXmlElement* el, const char* name, float* couleur) {
	Lit3fv(el, name, Xml::R, Xml::G, Xml::B, couleur);
}

void Xml::LitPosition3fv(TiXmlElement* el, const char* name, float* position) {
	Lit3fv(el, name, Xml::X, Xml::Y, Xml::Z, position);
}

void Xml::LitDirection3fv(TiXmlElement* el, const char* name, float* direction) {
	Lit3fv(el, name, Xml::X, Xml::Y, Xml::Z, direction);
}

unsigned int Xml::LitMaterialRef(TiXmlElement* el)
{
	double ref;
	TiXmlElement* elMat = el->FirstChildElement(Xml::MATERIAU);

	if(!elMat)
		throw CErreur(0, "Fichier Map corrompu : LitMaterialRef 1");

	if(!elMat->Attribute(Xml::REF, &ref))
		throw CErreur(0, "Fichier Map corrompu : LitMaterialRef 2");

	return (unsigned int)ref;
}

double Xml::LitValeur(TiXmlElement* el, const char* name)
{
	double valeur;
	TiXmlElement* elVal = el->FirstChildElement(name);

	if(!elVal)
		throw CErreur(0, "Fichier Map corrompu : LitValeur 1");

	if(!elVal->Attribute(Xml::VALEUR, &valeur))
		throw CErreur(0, "Fichier Map corrompu : LitValeur 2");

	return valeur;
}

bool Xml::LitSolidite(TiXmlElement* el)
{
	TiXmlElement* elSol = el->FirstChildElement(Xml::SOLIDE);
	bool solidite;

	if(!elSol)
		throw CErreur(0, "Fichier Map corrompu : solidite");

	const char* sol = elSol->Attribute(Xml::VALEUR);

	if(!sol)
		throw CErreur(0, "Fichier Map corrompu : solidite");

	if(!strcmp(sol, Xml::VRAI))
		solidite = true;
	else if(!strcmp(sol, Xml::FAUX))
		solidite = false;
	else
		throw CErreur(0, "Fichier Map corrompu : solidite");

	return solidite;
}
