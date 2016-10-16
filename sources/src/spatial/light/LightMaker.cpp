
#include "util/Erreur.h"
#include "spatial/XmlVocabulaire.h"
#include "spatial/light/Light.h"
#include "spatial/light/LightOmni.h"
#include "spatial/light/LightTarget.h"

#include "spatial/light/LightMaker.h"

namespace jkt
{

CLight* CLightMaker::Lit(TiXmlElement* element, MapLogger* mapLogger, CMap& map)
{
	const char* type = element->Attribute(Xml::TYPE);
	CLight* light;

	if(!strcmp(Xml::OMNI, type))
		light = new CLightOmni(&map);
	else if(!strcmp(Xml::TARGET, type))
		light = new CLightTarget(&map);
	else
		throw jkt::CErreur("Fichier Map corrompu : Lumiere de type inconnu");

	light->Lit(element, mapLogger);

	return light;
}

}
