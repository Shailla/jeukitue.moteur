
#include <string>

using namespace std;

#include "util/Erreur.h"
#include "spatial/XmlVocabulaire.h"
#include "spatial/geo/EntryPoint.h"

#include "spatial/geo/EntryPointMaker.h"

using namespace JktUtils;

namespace JktMoteur
{

EntryPoint* EntryPointMaker::Lit(TiXmlElement* el)
{
	if(strcmp(el->Value(), Xml::ENTRYPOINT))
	{
		string erreur = "Fichier map corrompu : '";
		erreur += Xml::ENTRYPOINT;
		erreur += "' attendu, '";
		erreur += el->Value();
		erreur += "' recu";
		throw CErreur(0, erreur.c_str());
	}

	EntryPoint* entry = new EntryPoint();
	entry->Lit(el);

	return entry;
}

} /* namespace JktMoteur */

