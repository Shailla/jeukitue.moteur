
#include <string>

#include "spatial/XmlVocabulaire.h"
#include "util/Erreur.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/materiau/MaterialMaker.h"

using namespace std;
using namespace jkt;

namespace jkt
{

CMaterial* CMaterialMaker::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) {
	if(strcmp(el->Value(), Xml::MATERIAU)) {
		string erreur = "Fichier map corrompu : '";
		erreur += Xml::MATERIAU;
		erreur += "' attendu, '";
		erreur += el->Value();
		erreur += "' recu";

		mapLogger->logInfo(erreur);

		throw CErreur(erreur);
	}

	CMaterial* mat = NULL;
	const char* type = el->Attribute(Xml::TYPE);

	if(!strcmp(Xml::SIMPLE, type)) {
		mat = new CMaterial(&map);
	}
	else if(!strcmp(Xml::TEXTURE, type)) {
		mat = new CMaterialTexture(&map);
	}
	else if(!strcmp(Xml::MULTI, type)) {
		mat = new CMaterialMulti(&map);
	}
	else {
		mapLogger->logError("Fichier map corrompu");
		throw CErreur("Fichier map corrompu");
	}

	mat->Lit(el, map, mapLogger);

	return mat;
}

}

