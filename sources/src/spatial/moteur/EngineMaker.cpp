
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

using namespace std;
using namespace jkt;

class CCfg;

namespace jkt
{

//Constructeurs / Destructeur
EngineMaker::EngineMaker(CMap *map) {
}

CMoteurParticules* EngineMaker::Lit(TiXmlElement* el, CMap* map, MapLogger* mapLogger) throw(CErreur) {
	const char* elType = el->Value();
	CMoteurParticules* engine;

	if(!strcmp(Xml::NEIGE, elType)) {
		engine = new CMoteurParticulesNeige(map);
	}
	else if(!strcmp(Xml::NEIGE_OLD, elType)) {
		engine = new CMoteurParticules(map);
	}
	else {
		throw jkt::CErreur("Fichier Map corrompu : Moteur de particules de type inconnu");
	}

	engine->Lit(el, mapLogger);

	return engine;
}

EngineMaker::~EngineMaker() {
}

}	// JktMoteur
