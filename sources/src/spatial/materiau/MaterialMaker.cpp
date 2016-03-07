
#include <string>

using namespace std;

#include "spatial/XmlVocabulaire.h"
#include "util/Erreur.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/materiau/MaterialMaker.h"

using namespace JktUtils;

namespace JktMoteur
{

CMaterial* CMaterialMaker::Lit(TiXmlElement* el, string &repertoire, MapLogger* mapLogger) {
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
		mat = new CMaterial();
	}
	else if(!strcmp(Xml::TEXTURE, type)) {
		mat = new CMaterialTexture();
	}
	else if(!strcmp(Xml::MULTI, type)) {
		mat = new CMaterialMulti();
	}
	else {
		mapLogger->logError("Fichier map corrompu");
		throw CErreur("Fichier map corrompu");
	}

	mat->Lit(el, repertoire, mapLogger);

	return mat;
}

}

