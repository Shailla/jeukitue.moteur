
#include <string>

using namespace std;

#include "util/Erreur.h"
#include "spatial/XmlVocabulaire.h"
#include "spatial/geo/EntryPoint.h"

#include "spatial/geo/EntryPointMaker.h"

using namespace jkt;

namespace jkt
{

EntryPoint* EntryPointMaker::Lit(TiXmlElement* el, MapLogger* mapLogger) {
	if(strcmp(el->Value(), Xml::ENTRYPOINT)) {
		string erreur = "Fichier map corrompu : '";
		erreur += Xml::ENTRYPOINT;
		erreur += "' attendu, '";
		erreur += el->Value();
		erreur += "' recu";

		mapLogger->logError(erreur);

		throw CErreur(erreur);
	}

	EntryPoint* entry = new EntryPoint();
	entry->Lit(el, mapLogger);

	return entry;
}

} /* namespace jkt */

