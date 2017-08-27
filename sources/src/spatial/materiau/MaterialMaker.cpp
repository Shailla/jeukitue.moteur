
#include <sstream>
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
	CMaterial* mat = NULL;

	const char* materialType = el->Value();

	if(!strcmp(Xml::MATERIAUSIMPLE, materialType)) {
		mat = new CMaterial(&map);
	}
	else if(!strcmp(Xml::MATERIAUTEXTURE, materialType)) {
		mat = new CMaterialTexture(&map);
	}
	else if(!strcmp(Xml::MATERIAUMULTI, materialType)) {
		mat = new CMaterialMulti(&map);
	}
	else {
		stringstream msg;
		msg << "Fichier map corrompu, type de matériau inconnu '" << materialType << "'";
		mapLogger->logError(msg.str());
		throw CErreur(msg);
	}

	mat->Lit(el, map, mapLogger);

	return mat;
}

}

