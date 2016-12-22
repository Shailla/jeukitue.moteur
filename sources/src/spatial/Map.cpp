
#include <algorithm>
#include <string>
#include <sstream>
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
	_isGlInitialized = false;
	_isPluginsInitialized = false;
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
	_isGlInitialized = false;
	_isPluginsInitialized = false;
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
	_isGlInitialized = false;
	_isPluginsInitialized = false;
}

CMap::~CMap() {
	// Destruction des objets géo
	for(Geometrical* geo : _geos)
		delete geo;

	// Destruction des matériaux
	for(CMaterial* mat : _materials)
		delete mat;

	// Destruction des lumières
	for(CLight* light : _lights)
		delete light;

	// Destruction des sous-Map
	for(auto& subMap : _subMaps) {
		delete subMap.second;
	}

	clear();
}

void CMap::clear() {
	_subMaps.clear();
	_geoDescriptions.clear();
	_objects.clear();
	_geos.clear();
	_solidAndTargettables.clear();
	_drawables.clear();
	_refreshables.clear();
	_lights.clear();
	_entryPoints.clear();
	_particulesEngines.clear();
	_materials.clear();
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
	initGL();

	if(!_isPluginsInitialized) {
		initPlugins();
	}

	glEnableClientState( GL_VERTEX_ARRAY );

	// Affichage des moteurs de particules
	for(CMoteurParticules* engine : _particulesEngines) {
		glEnable( GL_BLEND );
		glDepthMask( GL_FALSE );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );

		engine->affiche();	// Le moteur de particules affiche toutes ses particules
	}

	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );


	// Affichage des objets
	if(_bSelection) {
		int i=0;

		for(Drawable* drawable : _drawables) {
			if(drawable->isSelected())
				drawable->Affiche();			// Affichage de l'objet géo
			else
				drawable->AfficheSelection(1.0f, 0.0f, 0.0f);

			i++;
		}
	}
	else {
		for(Drawable* drawable : _drawables) {
			drawable->Affiche();			// Affichage de l'objet géo
		}
	}

	glDisable( GL_VERTEX_ARRAY );

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->Affiche();
	}
}

void CMap::AfficheSelection(float r,float v,float b) {	// Affiche tous les objets géo de du MAP
	// Si nécessaire, initialise les éléments OpenGL de la MAP
	if(!_isGlInitialized) {
		initGL();
	}

	if(!_isPluginsInitialized) {
		initPlugins();
	}

	glEnableClientState( GL_VERTEX_ARRAY );

	for(Drawable* drawable : _drawables) {
		drawable->AfficheSelection(r, v, b);
	}

	glDisable( GL_VERTEX_ARRAY );

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->AfficheSelection(r, v, b);
	}
}

void CMap::addMapObject(int id, MapObject* geo, MapLogger* mapLogger) {
	if(_geoDescriptions.find(id) != _geoDescriptions.end()) {
		mapLogger->logError("Map corrompue, la référence est en doublon");
	}

	_geoDescriptions[id] = geo;
}

MapObject* CMap::getMapObject(int id) {
	map<int, MapObject*>::iterator object = _geoDescriptions.find(id);

	if(object != _geoDescriptions.end()) {
		return object->second;
	}
	else {
		return 0;
	}
}

MapObject* CMap::findMapObject(int id) {
	if(getId() == id) {
		return this;
	}

	MapObject* object = getMapObject(id);

	if(object) {
		return object;
	}

	for(auto& subMap : _subMaps) {
		object = subMap.second->getMapObject(id);

		if(object) {
			return object;
		}
	}

	return 0;
}

void CMap::ChangeSelectionMode() {
	_bSelection = !_bSelection;
}

bool CMap::IsSelectionMode() {
	return _bSelection;
}

void CMap::merge(int mapId, MapLogger* mapLogger) {
	CMap* subMap = _subMaps.at(mapId);

	if(subMap) {
		// Objets de la Map
		for(MapObject* object : subMap->_objects) {
			object->setMap(this);
			_objects.push_back(object);
		}

		for(Geometrical* geo : subMap->_geos) {
			_geos.push_back(geo);
		}

		for(auto& geoDesc : subMap->_geoDescriptions) {
			_geoDescriptions[geoDesc.first] = geoDesc.second;
		}

		for(Refreshable* refreshable : subMap->_refreshables) {
			_refreshables.push_back(refreshable);
		}

		for(SolidAndTargettable* solid : subMap->_solidAndTargettables) {
			_solidAndTargettables.push_back(solid);
		}

		for(Drawable* drawable : subMap->_drawables) {
			_drawables.push_back(drawable);
		}

		// Merge des lumiéres
		for(CLight* light : subMap->_lights) {
			_lights.push_back(light);
		}

		// Merge des matériaux
		for(CMaterial* mat : subMap->_materials) {
			_materials.push_back(mat);
		}

		// Merge des entry points
		for(EntryPoint* entry : subMap->_entryPoints) {
			_entryPoints.push_back(entry);
		}

		// Merge des sous-subMap
		for(auto& mapVar : subMap->_subMaps) {
			_subMaps.insert(pair<int, CMap*>(mapVar.first, mapVar.second));
		}

		subMap->clear();
		_subMaps.erase(mapId);
		delete subMap;
	}
	else {
		stringstream msg;
		msg << "Map à merger introuvable : " << mapId;

		if(mapLogger) {
			mapLogger->logError(msg.str());
		}
		else {
			LOGWARN((msg.str()));
		}
	}
}

vector<EntryPoint*>& CMap::getEntryPointsList() {
	return _entryPoints;
}

void CMap::add(CMap* subMap) {
	if(subMap) {
		_subMaps.insert(pair<int, CMap*>(subMap->getId(), subMap));
	}
	else {
		LOGERROR(("Sub Map nulle"));
	}
}

void CMap::add(EntryPoint* entryPoint) {
	_geos.push_back(entryPoint);
	_entryPoints.push_back(entryPoint);
}

void CMap::add(CMoteurParticules* engine) {
	_particulesEngines.push_back(engine);
}

void CMap::add(Dirigeable* dirigeable) {
	_objects.push_back(dirigeable);
	_drawables.push_back(dirigeable);
	_refreshables.push_back(dirigeable);
	_geos.push_back(dirigeable);
}

void CMap::add(MapObject* object) {
	_objects.push_back(object);
	_geos.push_back(object);
	_drawables.push_back(object);
	_solidAndTargettables.push_back(object);
}

void CMap::add(CMaterial *mat) {
	_materials.push_back(mat);	// Ajoute mat à la liste des objets affichables
}

void CMap::add(CLight *light) {
	_lights.push_back(light);	// Ajoute light à la liste des objets affichables
	_geos.push_back(light);	// Ajoute light à la liste des objets affichables
}

void CMap::add(CPorte *porte) {
	_objects.push_back(porte);
	_geos.push_back(porte);
	_refreshables.push_back(porte);
	_drawables.push_back(porte);
}

void CMap::add(CheckPlayerInZone* detector) {
	_geos.push_back(detector);
	_refreshables.push_back(detector);
	_drawables.push_back(detector);
}

void CMap::add(CNavette *navette) {		// Une navette est avant tout un objet géo
	_geos.push_back(navette);
	_refreshables.push_back(navette);
	_drawables.push_back(navette);
	_objects.push_back(navette);
}

vector<MapObject*>& CMap::getMapObjects() {
	return _objects;
}

void CMap::GereContactPlayer(float positionPlayer[3], CPlayer *player ) {
	float positionPlayerDefault[3];

	if(!positionPlayer) {
		player->getPosition(positionPlayerDefault);
		positionPlayer = positionPlayerDefault;
	}

	for(SolidAndTargettable* solid : _solidAndTargettables) {
		solid->GereContactPlayer(positionPlayer, player);	// Gére les contacts entre l'objet géo et le joueur
	}

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->GereContactPlayer(positionPlayer, player);
	}
}

float CMap::GereLaserPlayer(float pos[3], CV3D &Dir, float dist) {
	// Renvoie la distance du premier point de contact entre un rayon laser parti du point 'pos'
	// dans la direction 'Dir' si cette distance est inférieure à 'dist', renvoie 'dist' sinon

	for(SolidAndTargettable* solid : _solidAndTargettables) {
		dist = solid->GereLaserPlayer( pos, Dir, dist );
	}

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->GereLaserPlayer( pos, Dir, dist );
	}

	return dist;	// Renvoie la distance du premier contact trouvé entre le laser et une face d'objet géo
}

void CMap::EchangeXY() {
	LOGDEBUG(("CMap::EchangeXY()%T", this ));

	// Entry points
	for(EntryPoint* entry : _entryPoints)
		entry->EchangeXY();

	// Geo
	for(Geometrical* geo : _geos)
		geo->EchangeXY();

	// Lights
	for(CLight* light : _lights)
		light->EchangeXY();

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps)
		subMap.second->EchangeXY();
}

void CMap::EchangeXZ() {
	LOGDEBUG(("CMap::EchangeXZ()%T", this ));

	// Entry points
	for(EntryPoint* entry : _entryPoints)
		entry->EchangeXZ();

	// Geo
	for(Geometrical* geo : _geos)
		geo->EchangeXZ();

	// Lights
	for(CLight* light : _lights)
		light->EchangeXZ();

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps)
		subMap.second->EchangeXZ();
}

void CMap::EchangeYZ() {
	LOGDEBUG(("CMap::EchangeYZ()%T", this ));

	// Entry points
	for(EntryPoint* entry : _entryPoints)
		entry->EchangeYZ();

	// Geo
	for(Geometrical* geo : _geos)
		geo->EchangeYZ();

	// Lights
	for(CLight* light : _lights)
		light->EchangeYZ();

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps)
		subMap.second->EchangeYZ();
}

void CMap::Scale(float scaleX, float scaleY, float scaleZ) {
	LOGDEBUG(("CMap::Scale(scaleX=%f,sclaeY=%f,scaleZ=%f)%T", scaleX, scaleY, scaleZ, this ));

	if(scaleX!=1.0 || scaleY!=1.0 || scaleZ!=1.0) {
		// Entry points
		for(EntryPoint* entry : _entryPoints)
			entry->Scale(scaleX, scaleY, scaleZ);

		// Geo
		for(Geometrical* geo : _geos)
			geo->Scale( scaleX, scaleY, scaleZ );

		// Lights
		for(CLight* light : _lights)
			light->Scale( scaleX, scaleY, scaleZ );
	}

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->Scale( scaleX, scaleY, scaleZ );
	}
}

void CMap::translate(float x, float y, float z) {
	LOGINFO(("CMap::translate(x=%f,y=%f,z=%f)%T", x, y, z, this ));

	if(x!=0.0 || y!=0.0 || z!=0.0) {
		// Entry points
		for(EntryPoint* entry : _entryPoints) {
			entry->translate(x, y, z);
		}

		// Geo
		for(Geometrical* geo : _geos) {
			geo->translate(x, y, z);
		}

		// Lights
		for(CLight* light : _lights) {
			light->translate(x, y, z);
		}
	}

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->translate(x, y, z);
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
	string mapDirectory, mapFilename;
	bool isResource = jkt::RessourcesLoader::getRessource(mapName, mapDirectory, mapFilename);

	// Si le fichier Map n'est pas une resource alors c'est une map classique
	if(!isResource) {
		mapDirectory = MAP_DIRECTORY + mapName + MAP_EXTENSION;
		mapFilename = mapName + MAP_FILE_EXTENSION;
	}

	map.setName(mapName);								// Nom de la Map
	map._filename = mapDirectory + "/" + mapFilename;	// Chemin complet fichier Map XML
	map._binariesDirectory = mapDirectory;				// Chemin des fichier binaires de la Map

	if(!mapLogger) {
		mapLogger = new MapLogger(mapDirectory + "/" + mapFilename);
	}

	// Lecture XML de la Map
	stringstream msg;
	msg << "Lecture fichier Map '" << map._filename << "'";
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

					const char* importMode = el->Attribute(Xml::MODE);	// Lecture du mode d'import
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
					CMap* subMap = new CMap(&map);

					Lit(*subMap, string(subMapName), mapLogger);

					stringstream msg;
					msg << "Translation " << subMap->getId() << " : " << translation[0] << " " << translation[1] << " " << translation[2];
					mapLogger->logInfo(msg.str());

					subMap->translate(translation[0], translation[1], translation[2]);

					msg.clear();//clear any bits set
					msg.str(std::string());
					msg << "Scaling " << subMap->getId() << " : " << scaling[0] << " " << scaling[1] << " " << scaling[2];
					mapLogger->logInfo(msg.str());

					subMap->Scale(scaling[0], scaling[1], scaling[2]);

					// Fusion des Map
					if(importMode && !strcmp(Xml::IMPORT_MODE_MERGE, importMode)) {
						msg.clear();//clear any bits set
						msg.str(std::string());
						msg << "Merge sous-Map '" << subMap->getId() << "'";
						mapLogger->logInfo(msg.str());

						map.add(subMap);
						map.merge(subMap->getId());
					}
					else if(importMode && !strcmp(Xml::IMPORT_MODE_ADD, importMode)) {
						msg.clear();//clear any bits set
						msg.str(std::string());
						msg << "Add sous-Map '" << subMap->getId() << "'";

						mapLogger->logInfo(msg.str());
						map.add(subMap);
					}
					else {
						msg.clear();//clear any bits set
						msg.str(std::string());
						msg << "Merge sous-Map (default) '" << subMap->getId() << "'";
						mapLogger->logInfo(msg.str());

						map.add(subMap);
						map.merge(subMap->getId());
					}
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

					EntryPoint* entry = EntryPointMaker::Lit(&map, el, mapLogger);

					if(entry) {
						map.add(entry);
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

				CLight* lum = CLightMaker::Lit(el, mapLogger, map);

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
						map.addMapObject(geo->getId(), geo, mapLogger);
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

					MapObject* object = map.getMapObject(description);

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

		map.init();

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
	LOGINFO(("Init Map %d", getId()));

	// Initialisation des object géométriques
	vector<MapObject*>::iterator iterObject;

	for(iterObject = _objects.begin() ; iterObject != _objects.end() ; iterObject++) {
		MapObject* object = (*iterObject);
		object->init();
	}

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->init();
	}
}

void CMap::initPlugins() {
	vector<string>::iterator it;

	for(it = _plugins.begin() ; it != _plugins.end() ; it++) {
		Fabrique::getPluginEngine()->activateMapPlugin(this, *it, _binariesDirectory);
	}

	_isPluginsInitialized = true;	// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::freePlugins() {
	Fabrique::getPluginEngine()->deactivateMapPlugins();
	_isPluginsInitialized = false;			// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::initGL() {
	if(!_isGlInitialized) {
		LOGINFO(("Init GL Map %d", getId()));

		// Initialisation GL des fichiers de texture
		for(CMaterial* material : _materials) {
			material->initGL();
		}

		// Initialisation GL des object géométriques dans le contexte OpenGL
		for(Drawable* drawable : _drawables) {
			drawable->initGL();
		}

		// Initialisation GL des moteurs de particules
		for(CMoteurParticules *engine : _particulesEngines) {
			engine->initGL();
		}

		_isGlInitialized = true;	// Indique que les éléments OpenGL de la MAP ont bien été initialisés
	}
}

void CMap::freeGL() {
	// Libération GL des moteurs de particules
	for(CMoteurParticules *engine : _particulesEngines) {
		engine->freeGL();
	}

	// Libération GL des object géométriques dans le contexte OpenGL
	for(Drawable* drawable : _drawables) {
		drawable->freeGL();
	}

	// Libération GL des fichiers de texture
	for(CMaterial* material : _materials) {
		material->freeGL();
	}

	// Libération GL des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->freeGL();
	}

	_isGlInitialized = false;
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
		vector<EntryPoint*>::iterator iterEntryPoint;

		for( iterEntryPoint=_entryPoints.begin() ; iterEntryPoint!=_entryPoints.end() ; iterEntryPoint++ )
			(*iterEntryPoint)->Save( elEntryPoint );		// Sauvegarde des paramètres de l'objet
	}

	// Sauvegarde des materiaux
	{
		TiXmlElement *elMat = new TiXmlElement(Xml::MATERIAUX);
		elMap->LinkEndChild(elMat);
		vector<CMaterial*>::iterator iterMat;

		for( iterMat=_materials.begin() ; iterMat!=_materials.end() ; iterMat++ )
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

	// Initialisation des sous-Map
	if(!var) {
		for(auto& subMap : _subMaps) {
			var = subMap.second->Contact( pos, dist );

			if(var)	// Si un triangle a été trouvé à une distance inférieure à 'dist' de la position 'pos'
				break;
		}
	}

	return var;
}

void CMap::refresh(CGame *game) {
	vector<Refreshable*>::iterator iter;

	for(iter = _refreshables.begin() ; iter != _refreshables.end() ; iter++)
		(*iter)->refresh(game);

	// Initialisation des sous-Map
	for(auto& subMap : _subMaps) {
		subMap.second->refresh(game);
	}
}

bool CMap::afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndexOfPage, int& posX, int& posY, int& indexOfPages) {
	bool again = true;

	// Si on n'a pas dépassé l'index de la dernière texture de la page active du damier on s'arrête là
	if(indexOfPages >= firstIndexOfPage + (nbrX * nbrY)) {
		again = false;
	}
	else if(material->Type() == CMaterial::MAT_TYPE_TEXTURE) {
		// On compte une texture à afficher de plus (affichée ou non sur la page active du damier)
		indexOfPages++;

		// Si la texture est sur la page active du damier on l'affiche
		if(indexOfPages > firstIndexOfPage) {
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

			// Colonne suivante
			posX++;

			// Si on est au bout d'une ligne alors change de ligne
			if(posX >= nbrX) {
				posX = 0;
				posY++;
			}
		}
	}
	else if( material->Type() == CMaterial::MAT_TYPE_SIMPLE ) {
	}
	else if( material->Type() == CMaterial::MAT_TYPE_MULTI ) {
		// Initialisation pour la texture
		CMaterialMulti *matMulti = (CMaterialMulti*)material;

		for(int i = 0 ; i < matMulti->NbrTex() && again ; i++) {
			CMaterial* sousMat = matMulti->getMat(i);
			again = afficheMaterial(sousMat, x, y, tailleX, tailleY, nbrX, nbrY, firstIndexOfPage, posX, posY, indexOfPages);
		}
	}
	else {
		LOGERROR((" CMap::afficheToutesTextures : Materiau de type inconnu"));
	}

	return again;
}

map<int, CMap*>& CMap::getSubMaps() {
	return _subMaps;
}

vector<CLight*>& CMap::getLights() {
	return _lights;
}

int CMap::afficheDamierTextures(int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int page) {
	int posX = 0;
	int posY = 0;

	int firstIndex = (page - 1) * nbrX * nbrY;
	int index = 0;

	vector<CMaterial*>::iterator iter;
	bool again = true;

	// On balaie toutes les textures, on affiche que celles qui sont sur la page active du damier
	for(iter = _materials.begin() ; iter != _materials.end() && again ; iter++) {
		again = afficheMaterial(*iter, x, y, tailleX, tailleY, nbrX, nbrY, firstIndex, posX, posY, index);
	}

	// Retourne le numéro de la page ou 0 si la page affichée ne contient aucune texture
	// Si index > firstIndex ça veut dire qu'on a affiché au moins une texture sur la page de damier active
	return (index > firstIndex) ? page : 0;
}

}	// JktMoteur
