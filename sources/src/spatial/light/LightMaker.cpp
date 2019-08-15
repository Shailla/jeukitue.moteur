
#include "util/Erreur.h"
#include "spatial/XmlVocabulaire.h"
#include "spatial/light/Light.h"
#include "spatial/light/LightOmni.h"
#include "spatial/light/LightTarget.h"

#include "spatial/light/LightMaker.h"

namespace jkt
{

CLight* CLightMaker::Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) {
	const char* type = element->Value();
	CLight* light = 0;

	if(!strcmp(Xml::LIGHTOMNI, type)) {
		light = new CLightOmni(&map);
	}
	else if(!strcmp(Xml::LIGHTTARGET, type)) {
		light = new CLightTarget(&map);
	}

	if(light) {
		light->Lit(element, map, mapLogger);
	}

	return light;
}

}
