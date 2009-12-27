
#include "Erreur.h"
#include "XmlVocabulaire.h"
#include "Light.h"
#include "LightOmni.h"
#include "LightTarget.h"

#include "LightMaker.h"

namespace JKT_PACKAGE_MOTEUR3D
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
		throw JKT_PACKAGE_UTILS::CErreur(0, "Fichier Map corrompu : Lumiere de type inconnu");

	light->Lit(element);

	return light;
}

}
