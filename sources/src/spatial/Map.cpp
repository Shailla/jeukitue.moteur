
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>

using namespace std;

#include "SDL.h"
#include "tinyxml.h"

class CGame;

#include "spatial/XmlVocabulaire.h"
#include "util/Trace.h"
#include "util/Erreur.h"
#include "main/Fabrique.h"
#include "util/Tableau.cpp"
#include <spatial/moteur/neige/simple/Particule.h>
#include <spatial/moteur/MoteurParticules.h>
#include <spatial/moteur/neige/simple/MoteurParticulesNeige.h>
#include <spatial/moteur/neige/mieux/MoteurNeige.h>
#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMaker.h"
#include "spatial/IfstreamMap.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"
#include "spatial/light/LightOmni.h"
#include "spatial/light/LightTarget.h"
#include "spatial/light/LightMaker.h"
#include <spatial/basic/Refreshable.h>
#include "spatial/objet/Dirigeable.h"
#include <spatial/basic/Geometrical.h>
#include "spatial/geo/EntryPoint.h"
#include "spatial/geo/SimpleGeo.h"
#include "spatial/geo/SimpleMaterialGeo.h"
#include "spatial/geo/MultiMaterialGeo.h"
#include "spatial/geo/TextureMaterialGeo.h"
#include "spatial/geo/GeoObject.h"
#include "spatial/objet/Porte.h"
#include "spatial/objet/Navette.h"
#include "spatial/objet/CheckPlayerInZone.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "util/V3D.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/geo/EntryPointMaker.h"
#include "ressource/RessourcesLoader.h"
#include "spatial/MapLogger.h"
#include "spatial/Map.h"

using namespace jkt;

namespace jkt
{

const char* CMap::identifier = "Map";

CMap::CMap(CMap* parent) : MapObject(parent) {
	LOGDEBUG(("CMap::CMap(*parent) %T", this ));

	_bSelection = false;
	_Selection = 0;
	_isGlActivated = false;
	_isPluginsActivated = false;
}

CMap::CMap(CMap* parent, const string& nomFichier) throw(jkt::CErreur) : MapObject(parent) {
	LOGDEBUG(("CMap::CMap(*parent,nomFichier=%s)%T", nomFichier.c_str(), this ));

	if( !Lit(nomFichier, 0) ) {
		LOGERROR(("Erreur de lecture de la Map '%s'", nomFichier.c_str()));
	}
	else {
		LOGINFO(("Fichier Map lu '%s'", nomFichier.c_str()));
	}

	_bSelection = false;
	_Selection = 0;
	_isGlActivated = false;
	_isPluginsActivated = false;

	init();
}

CMap::CMap(CMap* parent, const string& nomFichier, MapLogger* mapLogger) throw(jkt::CErreur) : MapObject(parent) {
	LOGDEBUG(("CMap::CMap(*parent,nomFichier=%s)%T", nomFichier.c_str(), this ));

	if( !Lit(nomFichier, mapLogger) ) {
		LOGERROR(("Erreur de lecture de la Map '%s'", nomFichier.c_str()));
	}
	else {
		LOGINFO(("Fichier Map lu '%s'", nomFichier.c_str()));
	}

	_bSelection = false;
	_Selection = 0;
	_isGlActivated = false;
	_isPluginsActivated = false;

	init();
}

CMap::~CMap() {
	// Destruction des objets géo
	vector<Geometrical*>::iterator iterGeo;
	for( iterGeo=_geos.begin() ; iterGeo!=_geos.end() ; iterGeo++ )
		delete *iterGeo;

	_geos.clear();
	_mouves.clear();

	// Destruction des matériaux
	vector<CMaterial*>::iterator iterMat;

	for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ )
		delete *iterMat;

	m_TabMaterial.clear();

	// Destruction des lumières
	vector<CLight*>::iterator iterLight;

	for( iterLight=_lights.begin() ; iterLight!=_lights.end() ; iterLight++ )
		delete *iterLight;

	_lights.clear();
}

MapObject* CMap::clone() {
	return new CMap(*this);
}

const char* CMap::toString() {
	ostringstream ttt;
	ttt << identifier << " Nom=" << getName() << " Mat=";

	tostring = ttt.str();

	return tostring.c_str();
}

void CMap::Affiche() {	// Affiche tous les objets géo de du MAP
	// Si nécessaire, initialise les éléments OpenGL de la MAP
	if(!_isGlActivated) {
		initGL();
	}

	if(!_isPluginsActivated) {
		initPlugins();
	}

	glEnableClientState( GL_VERTEX_ARRAY );

	// Affichage des moteurs de particules
	for(CMoteurParticules* engine : _particulesEngines) {
		glEnable( GL_BLEND );
		glDepthMask( GL_FALSE );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );

		engine->affiche();	// Le moteur de particules affiche toutes ses particules

		glDisable( GL_BLEND );
		glDepthMask( GL_TRUE );
	}


	// Affichage des objets
	vector<Drawable*>::iterator iter;

	if(_bSelection) {
		int i=0;

		for(iter = _drawables.begin() ; iter != _drawables.end() ; iter++) {
			if(i != _Selection)
				(*iter)->Affiche();			// Affichage de l'objet géo
			else
				(*iter)->AfficheSelection(1.0f, 0.0f, 0.0f);

			i++;
		}
	}
	else {
		for( iter = _drawables.begin() ; iter != _drawables.end() ; iter++ ) {
			(*iter)->Affiche();			// Affichage de l'objet géo
		}
	}

	glDisable( GL_VERTEX_ARRAY );
}

void CMap::AfficheSelection(float r,float v,float b) {	// Affiche tous les objets géo de du MAP
	// Si nécessaire, initialise les éléments OpenGL de la MAP
	if(!_isGlActivated) {
		initGL();
	}

	if(!_isPluginsActivated) {
		initPlugins();
	}

	glEnableClientState( GL_VERTEX_ARRAY );

	vector<Drawable*>::iterator iter;

	for(iter=_drawables.begin() ; iter!=_drawables.end() ; iter++) {
		(*iter)->AfficheSelection(r, v, b);
	}

	glDisable( GL_VERTEX_ARRAY );
}

void CMap::addDescription(int ref, MapObject* geo, MapLogger* mapLogger) {
	if(_geoDescriptions.find(ref) != _geoDescriptions.end()) {
		mapLogger->logError("Map corrompue, la référence est en doublon");
	}

	_geoDescriptions[ref] = geo;
}

MapObject* CMap::getDescription(int ref) {
	try {
		return _geoDescriptions.at(ref);
	}
	catch(out_of_range& exception) {
		return 0;
	}
}

void CMap::incrementeSelection() {
	_Selection++;

	if(_Selection >= (int)_geos.size()) {
		_Selection = 0;
	}
}

void CMap::decrementeSelection() {
	_Selection--;

	if(_Selection < 0) {
		_Selection = (int)_geos.size()-1;

		if(_Selection < 0)
			_Selection = 0;
	}
}

void CMap::ChangeSelectionMode() {
	_bSelection = !_bSelection;
	_Selection = 0;
}

bool CMap::IsSelectionMode() {
	return _bSelection;
}

const char* CMap::getSelectedName() {
	Object* object = _objects[_Selection];
	return object->toString();
}

void CMap::merge(CMap& map) {
	// Objets de la Map
	vector<MapObject*>::iterator iterObject;
	vector<Geometrical*>::iterator iterGeo;
	vector<Refreshable*>::iterator iterRefresh;
	vector<SolidAndTargettable*>::iterator iterSolid;
	vector<Drawable*>::iterator iterDrawable;

	// Caractéristiques de la Map
	vector<CLight*>::iterator iterLight;
	vector<CMaterial*>::iterator iterMaterial;
	vector<EntryPoint>::iterator iterEntryPoint;

	// Objets de la Map
	for(iterObject=map._objects.begin() ; iterObject!=map._objects.end() ; iterObject++) {
		(*iterObject)->setMap(this);
		_objects.push_back(*iterObject);
	}

	for(iterGeo=map._geos.begin() ; iterGeo!=map._geos.end() ; iterGeo++) {
		_geos.push_back(*iterGeo);
	}

	for(iterRefresh=map._mouves.begin() ; iterRefresh!=map._mouves.end() ; iterRefresh++) {
		_mouves.push_back(*iterRefresh);
	}

	for(iterSolid=map._solidAndTargettables.begin() ; iterSolid!=map._solidAndTargettables.end() ; iterSolid++) {
		_solidAndTargettables.push_back(*iterSolid);
	}

	for(iterDrawable = map._drawables.begin() ; iterDrawable != map._drawables.end() ; iterDrawable++) {
		_drawables.push_back(*iterDrawable);
	}

	// Caractéristiques de la Map
	for(iterLight=map._lights.begin() ; iterLight!=map._lights.end() ; iterLight++) {
		_lights.push_back(*iterLight);
	}

	for(iterMaterial=map.m_TabMaterial.begin() ; iterMaterial!=map.m_TabMaterial.end() ; iterMaterial++) {
		m_TabMaterial.push_back(*iterMaterial);
	}

	for(iterEntryPoint=map._entryPoints.begin() ; iterEntryPoint!=map._entryPoints.end() ; iterEntryPoint++) {
		_entryPoints.push_back(*iterEntryPoint);
	}
}

vector<EntryPoint>& CMap::getEntryPointsList() {
	return _entryPoints;
}

void CMap::add(EntryPoint entryPoint) {
	_entryPoints.push_back(entryPoint);
}

void CMap::add(CMoteurParticules* engine) {
	_particulesEngines.push_back(engine);
}

void CMap::add(Dirigeable* dirigeable) {
	_objects.push_back(dirigeable);
	_drawables.push_back(dirigeable);
	_mouves.push_back(dirigeable);
	_geos.push_back(dirigeable);
}

void CMap::add(MapObject* object) {
	_objects.push_back(object);
	_geos.push_back(object);
	_drawables.push_back(object);
	_solidAndTargettables.push_back(object);
}

void CMap::add(CMaterial *mat) {
	m_TabMaterial.push_back(mat);	// Ajoute mat à la liste des objets affichables
}

void CMap::add(CLight *light) {
	_lights.push_back(light);	// Ajoute light à la liste des objets affichables
	_geos.push_back(light);	// Ajoute light à la liste des objets affichables
}

void CMap::add(CPorte *porte) {
	_objects.push_back(porte);
	_geos.push_back(porte);
	_mouves.push_back(porte);
	_drawables.push_back(porte);
}

void CMap::add(CheckPlayerInZone* detector) {
	_geos.push_back(detector);
}

void CMap::add(CNavette *navette) {		// Une navette est avant tout un objet géo
	_geos.push_back(navette);
	_mouves.push_back(navette);
	_drawables.push_back(navette);
	_objects.push_back(navette);
}

void CMap::GereContactPlayer(float positionPlayer[3], CPlayer *player ) {
	float positionPlayerDefault[3];

	if(!positionPlayer) {
		player->getPosition(positionPlayerDefault);
		positionPlayer = positionPlayerDefault;
	}

	vector<SolidAndTargettable*>::iterator iter;

	for(iter=_solidAndTargettables.begin() ; iter!=_solidAndTargettables.end() ; iter++)
		(*iter)->GereContactPlayer(positionPlayer, player);	// Gère les contacts entre l'objet géo et le joueur
}

float CMap::GereLaserPlayer(float pos[3], CV3D &Dir, float dist) {
	// Renvoie la distance du premier point de contact entre un rayon laser parti du point 'pos'
	// dans la direction 'Dir' si cette distance est inférieure à 'dist', renvoie 'dist' sinon
	vector<SolidAndTargettable*>::iterator iter;

	for(iter=_solidAndTargettables.begin() ; iter!=_solidAndTargettables.end() ; iter++)
		dist = (*iter)->GereLaserPlayer( pos, Dir, dist );

	return dist;	// Renvoie la distance du premier contact trouvé entre le laser et une face d'objet géo
}

void CMap::EchangeXY() {
	LOGDEBUG(("CMap::EchangeXY()%T", this ));

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeXY();

	// Geo
	vector<Geometrical*>::iterator iterGeo;
	for( iterGeo=_geos.begin() ; iterGeo!=_geos.end() ; iterGeo++ )
		(*iterGeo)->EchangeXY();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=_lights.begin() ; iterLight!=_lights.end() ; iterLight++ )
		(*iterLight)->EchangeXY();
}

void CMap::EchangeXZ() {
	LOGDEBUG(("CMap::EchangeXZ()%T", this ));

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeXZ();

	// Geo
	vector<Geometrical*>::iterator iterGeo;
	for( iterGeo=_geos.begin() ; iterGeo!=_geos.end() ; iterGeo++ )
		(*iterGeo)->EchangeXZ();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=_lights.begin() ; iterLight!=_lights.end() ; iterLight++ )
		(*iterLight)->EchangeXZ();
}

void CMap::EchangeYZ() {
	LOGDEBUG(("CMap::EchangeYZ()%T", this ));

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeYZ();

	// Geo
	vector<Geometrical*>::iterator iterGeo;
	for( iterGeo=_geos.begin() ; iterGeo!=_geos.end() ; iterGeo++ )
		(*iterGeo)->EchangeYZ();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=_lights.begin() ; iterLight!=_lights.end() ; iterLight++ )
		(*iterLight)->EchangeYZ();
}

void CMap::Scale(float scaleX, float scaleY, float scaleZ) {
	LOGDEBUG(("CMap::Scale(scaleX=%f,sclaeY=%f,scaleZ=%f)%T", scaleX, scaleY, scaleZ, this ));

	if(scaleX!=1.0 || scaleY!=1.0 || scaleZ!=1.0) {
		// Entry points
		vector<EntryPoint>::iterator iterEntry;
		for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
			(*iterEntry).Scale(scaleX, scaleY, scaleZ);

		// Geo
		vector<Geometrical*>::iterator iterGeo;
		for( iterGeo=_geos.begin() ; iterGeo!=_geos.end() ; iterGeo++ )
			(*iterGeo)->Scale( scaleX, scaleY, scaleZ );

		// Lights
		vector<CLight*>::iterator iterLight;
		for( iterLight=_lights.begin() ; iterLight!=_lights.end() ; iterLight++ )
			(*iterLight)->Scale( scaleX, scaleY, scaleZ );
	}
}

void CMap::translate(float x, float y, float z) {
	LOGDEBUG(("CMap::translate(x=%f,y=%f,z=%f)%T", x, y, z, this ));

	if(x!=0.0 || y!=0.0 || z!=0.0) {
		// Entry points
		vector<EntryPoint>::iterator iterEntry;

		for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
			(*iterEntry).translate(x, y, z);

		// Geo
		vector<Geometrical*>::iterator iterGeo;

		for( iterGeo=_geos.begin() ; iterGeo!=_geos.end() ; iterGeo++ )
			(*iterGeo)->translate(x, y, z);

		// Lights
		vector<CLight*>::iterator iterLight;

		for( iterLight=_lights.begin() ; iterLight!=_lights.end() ; iterLight++ )
			(*iterLight)->translate(x, y, z);
	}
}

bool CMap::Lit(const string &nomFichier, MapLogger* mapLogger) {
	return Lit(*this, nomFichier, mapLogger);
}

bool CMap::Lit(TiXmlElement* el, MapLogger* mapLogger) {
	return false;
}

bool CMap::Save(TiXmlElement* element) {			// Sauve l'objet géo dans un fichier Map
	return false;
}

bool CMap::Lit(CMap& map, const string& mapName, MapLogger* mapLogger) {
	// Répertoire et fichier de la Map
	string partialFileName = mapName;
	bool isResource = jkt::RessourcesLoader::getFileRessource(partialFileName);

	// Si le fichier Map n'est pas une resource alors c'est une map classique
	if(!isResource) {
		partialFileName = ".\\map\\" + partialFileName;
	}

	map.setName(mapName);										// Nom de la Map
	map._filename = partialFileName + ".map.xml";				// Chemin complet fichier Map XML
	map._binariesDirectory = partialFileName + "\\";			// Chemin des fichier binaires de la Map

	if(!mapLogger) {
		mapLogger = new MapLogger(partialFileName);
	}

	// Lecture XML de la Map
	stringstream msg;
	msg << "Lecture fichier Map '" << mapName << "'";
	mapLogger->logInfo(msg.str());

	bool result = false;

	TiXmlDocument document(map._filename.c_str());

	if(document.LoadFile()) {
		// Element de map
		TiXmlElement* elMap = document.FirstChildElement(Xml::MAP);
		if(!elMap) {
			mapLogger->logError("Fichier map corrompu");
			throw CErreur("Fichier map corrompu");
		}

		// Lecture des plugins de la Map
		{
			TiXmlElement* elPlugins = elMap->FirstChildElement(Xml::PLUGINS);

			if(elPlugins) {
				for(TiXmlElement* el=elPlugins->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
					if(strcmp(Xml::LOAD, el->Value())) {
						Xml::throwCorruptedMapFileException(Xml::LOAD, el->Value());
					}

					string pluginFilename = el->Attribute(Xml::NOM);	// Lecture nom de la Map à importer
					RessourcesLoader::getFileRessource(map._binariesDirectory, pluginFilename);
					map._plugins.push_back(pluginFilename);
				}
			}
		}


		// Lecture des imports de sous-Map
		{
			TiXmlElement* elImports = elMap->FirstChildElement(Xml::IMPORTS);

			if(elImports) {
				for(TiXmlElement* el=elImports->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
					if(strcmp(Xml::IMPORT, el->Value())) {
						Xml::throwCorruptedMapFileException(Xml::IMPORT, el->Value());
					}

					const char* subMapName = el->Attribute(Xml::NOM);	// Lecture nom de la Map à importer

					if(!subMapName) {
						mapLogger->logError("Fichier Map corrompu : Nom de la sous-Map a importer manquant");
						throw CErreur("Fichier Map corrompu : Nom de la sous-Map a importer manquant");
					}

					// Lecture de la translation à appliquer à la Map
					float translation[3];

					if(!Xml::Lit3fv(el, Xml::TRANSLATE, Xml::X, Xml::Y, Xml::Z, translation)) {
						translation[0] = 0.0;
						translation[1] = 0.0;
						translation[2] = 0.0;
					}

					// Lecture du scaling à appliquer à la Map
					float scaling[3];

					if(!Xml::Lit3fv(el, Xml::SCALE, Xml::X, Xml::Y, Xml::Z, scaling)) {
						scaling[0] = 1.0;
						scaling[1] = 1.0;
						scaling[2] = 1.0;
					}

					// Lecture de la Map
					CMap subMap(0);
					Lit(subMap, string(subMapName), mapLogger);

					subMap.translate(translation[0], translation[1], translation[2]);
					subMap.Scale(scaling[0], scaling[1], scaling[2]);

					// Fusion des Map
					map.merge(subMap);
					subMap._geos.clear();
					subMap._mouves.clear();
					subMap._lights.clear();
					subMap.m_TabMaterial.clear();
				}
			}
		}

		// Lecture des point d'entrée des joueurs
		{
			TiXmlElement* elEntry = elMap->FirstChildElement(Xml::ENTRYPOINTS);

			if(elEntry) {
				for(TiXmlElement* el=elEntry->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
					if(strcmp(Xml::ENTRYPOINT, el->Value())) {
						Xml::throwCorruptedMapFileException(Xml::ENTRYPOINT, el->Value());
					}

					EntryPoint* entry = EntryPointMaker::Lit(el, mapLogger);

					if(entry) {
						map.add(*entry);
						delete entry;
					}
				}
			}
		}

		// Lecture des moteurs de particules
		{
			TiXmlElement* elEntry = elMap->FirstChildElement(Xml::PARTICULES_ENGINES);

			if(elEntry) {
				for(TiXmlElement* el=elEntry->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
					if(strcmp(Xml::NEIGE, el->Value())) {
						Xml::throwCorruptedMapFileException(Xml::NEIGE, el->Value());
					}

					// Lecture du nombre de particules à afficher
					int nbrParticules;

					if(!el->Attribute(Xml::NBR_PARTICULES, &nbrParticules)) {		// Lecture nom de la Map à importer
						mapLogger->logError("Fichier Map corrompu : Nombre de particules du moteur manquant");
						throw CErreur("Fichier Map corrompu : Nombre de particules du moteur manquant");
					}

					// Lecture de la position du moteur de neige
					float position[3];

					if(!Xml::Lit3fv(el, Xml::POSITION, Xml::X, Xml::Y, Xml::Z, position)) {
						position[0] = 0.0;
						position[1] = 0.0;
						position[2] = 0.0;
					}

					// Lecture des dimensions du moteur de neige
					float dimension[3];

					if(!Xml::Lit3fv(el, Xml::DIMENSION, Xml::X, Xml::Y, Xml::Z, dimension)) {
						dimension[0] = 0.0;
						dimension[1] = 0.0;
						dimension[2] = 0.0;
					}

					CV3D posMoteurParticulesNeige(position[0], position[1], position[2]);
					CV3D tailleMoteurParticulesNeige(dimension[0], dimension[1], dimension[2]);
					//					map.add(new CMoteurParticulesNeige(nbrParticules, posMoteurParticulesNeige, tailleMoteurParticulesNeige));
					map.add(new MoteurNeige(nbrParticules, nbrParticules/3, posMoteurParticulesNeige, tailleMoteurParticulesNeige));
				}
			}
		}

		// Lecture des matériaux
		TiXmlElement* elMat = elMap->FirstChildElement(Xml::MATERIAUX);

		if(elMat) {
			for(TiXmlElement* el=elMat->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				if(strcmp(Xml::MATERIAU, el->Value())) {
					Xml::throwCorruptedMapFileException(Xml::MATERIAU, el->Value());
				}

				CMaterial* mat = CMaterialMaker::Lit(el, map._binariesDirectory, mapLogger);

				if(mat) {
					map.add(mat);
				}
				else {
					mapLogger->logError("Matériau corrompu ?");
				}
			}
		}

		// Lecture des lumières
		TiXmlElement* elLight = elMap->FirstChildElement(Xml::LUMIERES);

		if(elLight) {
			for(TiXmlElement* el=elLight->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				if(strcmp(Xml::LUMIERE, el->Value())) {
					Xml::throwCorruptedMapFileException(Xml::LUMIERE, el->Value());
				}

				CLight* lum = CLightMaker::Lit(el, mapLogger);

				if(lum) {
					map.add(lum);
				}
				else {
					mapLogger->logError("Lumière corrompue ?");
				}
			}
		}

		// Lecture des objets géométriques
		TiXmlElement* elGeo = elMap->FirstChildElement(Xml::GEOS);

		if(elGeo) {

			for(TiXmlElement* el=elGeo->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				// Descriptions d'objets géométriques
				if(!strcmp(Xml::GEODESCRIPTION, el->Value())) {
					MapObject* geo = CGeoMaker::Lit(el, &map, mapLogger);

					if(geo) {
						map.addDescription(geo->getReference(), geo, mapLogger);
					}
					else {
						mapLogger->logError("Géo description corrompue ?");
					}
				}
			}

			for(TiXmlElement* el=elGeo->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				// Objets géométriques réels
				if(!strcmp(Xml::GEO, el->Value())) {
					MapObject* object = CGeoMaker::Lit(el, &map, mapLogger);

					if(object) {
						map.add(object);
					}
					else {
						mapLogger->logError("Géo corrompue ?");
					}
				}
				// Objets géométriques réels
				else if(!strcmp(Xml::GEOINSTANCE, el->Value())) {
					double translation[3];
					translation[0] = translation[1] = translation[2] = 0.0f;
					int description = -1;

					if(!el->Attribute(Xml::X, &translation[0])) {
						mapLogger->logError("Fichier Map corrompu");
						throw CErreur("Fichier Map corrompu");
					}

					if(!el->Attribute(Xml::Y, &translation[1])) {
						mapLogger->logError("Fichier Map corrompu");
						throw CErreur("Fichier Map corrompu");
					}

					if(!el->Attribute(Xml::Z, &translation[2])) {
						mapLogger->logError("Fichier Map corrompu");
						throw CErreur("Fichier Map corrompu");
					}

					if(!el->Attribute(Xml::DESCRIPTION, &description)) {
						mapLogger->logError("Fichier Map corrompu");
						throw CErreur("Fichier Map corrompu");
					}

					MapObject* object = map.getDescription(description);

					if(object) {
						MapObject* clone = object->clone();
						clone->translate(translation[0], translation[1], translation[2]);
						map.add(clone);
					}
					else {
						mapLogger->logError("Géo description introuvable");
					}
				}
			}
		}

		result = true;
	}
	else {
		stringstream msg;
		msg << "Ouverture impossible '" << map._filename << "'";
		mapLogger->logError(msg.str());
	}

	return result;
}

void CMap::init() throw(jkt::CErreur) {	// Initialisation de la CMap
	// Initialisation des object géométriques
	vector<MapObject*>::iterator iterObject;

	for(iterObject = _objects.begin() ; iterObject != _objects.end() ; iterObject++) {
		MapObject* object = (*iterObject);
		object->init();
	}
}

void CMap::initPlugins() {
	vector<string>::iterator it;

	for(it = _plugins.begin() ; it != _plugins.end() ; it++) {
		Fabrique::getPluginEngine()->activateMapPlugin(this, *it, _binariesDirectory);
	}

	_isPluginsActivated = true;	// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::freePlugins() {
	Fabrique::getPluginEngine()->deactivateMapPlugins();
	_isPluginsActivated = false;			// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::initGL() {
	// Letcure des fichiers de texture
	for(CMaterial* material : m_TabMaterial) {
		material->initGL();
	}

	// Initialisation des object géométriques dans le contexte OpenGL
	for(Drawable* drawable : _drawables) {
		drawable->initGL();
	}

	// Letcure des moteurs de particules
	for(CMoteurParticules *engine : _particulesEngines) {
		engine->initGL();
	}

	_isGlActivated = true;	// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::freeGL() {
	// Letcure des moteurs de particules
	for(CMoteurParticules *engine : _particulesEngines) {
		engine->freeGL();
	}

	// Initialisation des object géométriques dans le contexte OpenGL
	for(Drawable* drawable : _drawables) {
		drawable->freeGL();
	}

	// Letcure des fichiers de texture
	for(CMaterial* material : m_TabMaterial) {
		material->freeGL();
	}

	_isGlActivated = false;
}

bool CMap::Save(const string nomFichier) {
	// CREATION DES FICHIERS
	string nomFichierMap = "./map/" + nomFichier + ".map.xml";

	// Initialisation du doc XML
	TiXmlDocument document;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","");
	document.LinkEndChild(decl);

	TiXmlElement *elMap = new TiXmlElement("Map");
	document.LinkEndChild(elMap);

	// Sauvegarde des points d'entrée des joueurs
	{
		TiXmlElement *elEntryPoint = new TiXmlElement(Xml::ENTRYPOINTS);
		elMap->LinkEndChild(elEntryPoint);
		vector<EntryPoint>::iterator iterEntryPoint;

		for( iterEntryPoint=_entryPoints.begin() ; iterEntryPoint!=_entryPoints.end() ; iterEntryPoint++ )
			(*iterEntryPoint).Save( elEntryPoint );		// Sauvegarde des paramètres de l'objet
	}

	// Sauvegarde des materiaux
	{
		TiXmlElement *elMat = new TiXmlElement(Xml::MATERIAUX);
		elMap->LinkEndChild(elMat);
		vector<CMaterial*>::iterator iterMat;

		for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ )
			(*iterMat)->Save(elMat);		// Sauvegarde du matériau
	}

	// Sauvegarde des lumieres
	{
		TiXmlElement *elLum = new TiXmlElement(Xml::LUMIERES);
		elMap->LinkEndChild(elLum);
		vector<CLight*>::iterator iterLight;

		for( iterLight=_lights.begin() ; iterLight!=_lights.end() ; iterLight++ )
			(*iterLight)->Save( elLum );
	}

	// Sauvegarde des objets geometriques
	{
		TiXmlElement *elGeo = new TiXmlElement(Xml::GEOS);
		elMap->LinkEndChild(elGeo);
		vector<MapObject*>::iterator iterObject;

		for(iterObject = _objects.begin() ; iterObject != _objects.end() ; iterObject++)
			(*iterObject)->Save( elGeo );		// Sauvegarde des paramètres de l'objet
	}

	document.SaveFile(nomFichierMap.c_str());

	cout << endl << "Sauvegarde du fichier MAP Ok !!";

	LOGDEBUG(("CMap::SaveFichierMap() Sauvegarde du fichier MAP Ok%T", this ));
	return true;
}

bool CMap::Contact(const float pos[3], const float dist) {
	bool var = false;	// Pas de contact par défaut
	vector<SolidAndTargettable*>::iterator iter;

	for(iter = _solidAndTargettables.begin() ; iter != _solidAndTargettables.end() ; iter++) {
		var = (*iter)->Contact( pos, dist );

		if(var)	// Si un triangle a été trouvé à une distance inférieure à 'dist' de la position 'pos'
			break;
	}

	return var;
}

void CMap::Refresh(CGame *game) {
	vector<Refreshable*>::iterator iterMouve;

	for(iterMouve=_mouves.begin() ; iterMouve!=_mouves.end() ; iterMouve++)
		(*iterMouve)->refresh(game);
}

void CMap::afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex, int& posX, int& posY, int& index) {
	if(index < firstIndex + (nbrX * nbrY)) {
		if(material->Type() == CMaterial::MAT_TYPE_TEXTURE) {
			if(index > firstIndex) {
				CMaterialTexture *matTexture = (CMaterialTexture*)material;
				matTexture->Active();

				int x1 = x + posX*tailleX/nbrX;
				int x2 = x1 + tailleX/nbrX;
				int y1 = y + posY*tailleY/nbrY;
				int y2 = y1 + tailleY/nbrY;

				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2i(x1, 	y1);

				glTexCoord2f(1.0f, 0.0f);
				glVertex2i(x2, 	y1);

				glTexCoord2f(1.0f, 1.0f);
				glVertex2i(x2, 	y2);

				glTexCoord2f(0.0f, 1.0f);
				glVertex2i(x1, 	y2);
				glEnd();

				glDisable(GL_TEXTURE_2D);
				glColor3f(1.0f, 1.0f, 1.0f);
				glLineWidth(2);

				glBegin(GL_LINE_LOOP);
				glVertex2i(x1, 	y1);
				glVertex2i(x2, 	y1);
				glVertex2i(x2, 	y2);
				glVertex2i(x1, 	y2);
				glEnd();

				if(++posX >= nbrX) {
					posX = 0;
					posY++;
				}
			}

			index++;
		}
		else if( material->Type() == CMaterial::MAT_TYPE_SIMPLE ) {
		}
		else if( material->Type() == CMaterial::MAT_TYPE_MULTI ) {
			// Initialisation pour la texture
			CMaterialMulti *matMulti = (CMaterialMulti*)material;

			for(int i=0 ; i<matMulti->NbrTex() ; i++) {
				CMaterial* sousMat = matMulti->getMat(i);
				afficheMaterial(sousMat, x, y, tailleX, tailleY, nbrX, nbrY, firstIndex, posX, posY, index);
			}
		}
		else {
			LOGERROR((" CMap::afficheToutesTextures : Materiau de type inconnu"));
		}
	}
}

vector<CLight*>& CMap::getLights() {
	return _lights;
}

void CMap::afficheToutesTextures(int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex) {
	int posX = 0;
	int posY = 0;
	int index = firstIndex;
	vector<CMaterial*>::iterator iter;

	for(iter=m_TabMaterial.begin() ; iter!=m_TabMaterial.end() ; iter++) {
		CMaterial* material = *iter;
		afficheMaterial(material, x, y, tailleX, tailleY, nbrX, nbrY, firstIndex, posX, posY, index);
	}
}

}	// JktMoteur
