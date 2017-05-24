
#include <string>
#include <fstream>
#include <set>
#include <cmath>
#include <map>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#endif

class CGame;

#include "spatial/XmlVocabulaire.h"
#include "util/V3D.h"

#include "spatial/moteur/EngineMaker.h"
#include "spatial/moteur/neige/mieux/MoteurNeige.h"

using namespace std;
using namespace jkt;

class CCfg;

namespace jkt
{

CMoteurParticules* EngineMaker::Lit(TiXmlElement* el, CMap* map, MapLogger* mapLogger) throw(CErreur) {
	const char* elType = el->Value();
	CMoteurParticules* engine;

	if(!strcmp(Xml::NEIGE, elType)) {
		engine = new MoteurNeige(map);
	}
	else {
		throw jkt::CErreur("Fichier Map corrompu : Moteur de particules de type inconnu");
	}

	engine->Lit(el, mapLogger);

	return engine;
}

}	// JktMoteur
