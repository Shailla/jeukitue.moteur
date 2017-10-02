
#include <string>

#include "util/Erreur.h"
#include "spatial/XmlVocabulaire.h"
#include "spatial/geo/EntryPoint.h"

#include "spatial/geo/EntryPointMaker.h"

using namespace std;
using namespace jkt;

namespace jkt
{

EntryPoint* EntryPointMaker::Lit(CMap& map, TiXmlElement* el, MapLogger* mapLogger) throw(CErreur) {
	if(strcmp(el->Value(), Xml::ENTRYPOINT)) {
		string erreur = "Fichier map corrompu : '";
		erreur += Xml::ENTRYPOINT;
		erreur += "' attendu, '";
		erreur += el->Value();
		erreur += "' recu";

		mapLogger->logError(erreur);

		throw CErreur(erreur);
	}

	EntryPoint* entry = new EntryPoint(&map);
	entry->Lit(el, map, mapLogger);

	return entry;
}

} /* namespace jkt */

