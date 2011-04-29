
#include "util/Erreur.h"
#include "spatial/XmlVocabulaire.h"
#include "spatial/light/Light.h"
#include "spatial/light/LightOmni.h"
#include "spatial/light/LightTarget.h"

#include "spatial/light/LightMaker.h"

namespace JktMoteur
{

CLight* CLightMaker::Lit(TiXmlElement* element)
{
	const char* type = element->Attribute(Xml::TYPE);
	CLight* light;

	if(!strcmp(Xml::OMNI, type))
		light = new CLightOmni();
	else if(!strcmp(Xml::TARGET, type))
		light = new CLightTarget();
	else
		throw JktUtils::CErreur(0, "Fichier Map corrompu : Lumiere de type inconnu");

	light->Lit(element);

	return light;
}

}
