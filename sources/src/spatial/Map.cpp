
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>

using namespace std;

#include "SDL.h"
#include "tinyxml.h"

class CGame;

#include "spatial/XmlVocabulaire.h"
#include "util/Trace.h"
#include "util/TraceMethod.h"
#include "util/Erreur.h"
#include "main/Fabrique.h"
#include "util/Tableau.cpp"
#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMaker.h"
#include "spatial/IfstreamMap.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/light/Light.h"
#include "spatial/light/LightOmni.h"
#include "spatial/light/LightTarget.h"
#include "spatial/light/LightMaker.h"
#include "spatial/Mouve.h"
#include "spatial/geo/Geo.h"
#include "spatial/geo/EntryPoint.h"
#include "spatial/geo/SimpleGeo.h"
#include "spatial/geo/SimpleMaterialGeo.h"
#include "spatial/geo/MultiMaterialGeo.h"
#include "spatial/geo/TextureMaterialGeo.h"
#include "spatial/geo/GeoObject.h"
#include "spatial/objet/Porte.h"
#include "spatial/objet/Navette.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "util/V3D.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/geo/EntryPointMaker.h"
#include "ressource/RessourcesLoader.h"

#include "spatial/Map.h"

using namespace JktUtils;

namespace JktMoteur
{

const char* CMap::identifier = "Map";

CMap::CMap(CMap* parent) : CGeo(parent) {
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap() begin%T", this );
	m_bSelection = false;
	m_Selection = 0;
	_isGlActivated = false;
	_isPluginsActivated = false;
}

CMap::CMap(CMap* parent, const string& nomFichier) throw(JktUtils::CErreur) : CGeo(parent) {
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap(nomFichier=%s) begin%T", nomFichier.c_str(), this );
	if( !Lit(nomFichier) ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur à la lecture du fichier MAP : " << nomFichier;
	}
	else {
		cout << endl << "Fichier MAP lu : " << nomFichier;
	}

	m_bSelection = false;
	m_Selection = 0;
	_isGlActivated = false;
	_isPluginsActivated = false;

	Init();
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap() end%T", this );
}

CMap::~CMap() {
TRACE().p( TRACE_MOTEUR3D, "CMap::~CMap() begin%T", this );

	// Destruction des objets géo
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		delete *iterGeo;

	m_TabGeo.clear();
	m_TabMouve.clear();

	// Destruction des matériaux
	vector<CMaterial*>::iterator iterMat;

	for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ )
		delete *iterMat;

	m_TabMaterial.clear();

	// Destruction des lumières
	vector<CLight*>::iterator iterLight;

	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		delete *iterLight;

	m_TabLight.clear();

TRACE().p( TRACE_MOTEUR3D, "CMap::~CMap() end%T", this );
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

	vector<CGeo*>::iterator iterGeo;

	if(m_bSelection) {
		int i=0;

		for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
			if(i != m_Selection)
				(*iterGeo)->Affiche();			// Affichage de l'objet géo
			else
				(*iterGeo)->AfficheSelection(1.0f, 0.0f, 0.0f);

			i++;
		}
	}
	else {
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ ) {
			(*iterGeo)->Affiche();			// Affichage de l'objet géo
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

	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		(*iterGeo)->AfficheSelection(r, v, b);
	}

	glDisable( GL_VERTEX_ARRAY );
}

void CMap::add(CGeo* geo) {
	m_TabGeo.push_back(geo);	// Ajoute geo à la liste des objets affichables
}

void CMap::add(CMaterial *mat) {
	m_TabMaterial.push_back(mat);	// Ajoute mat à la liste des objets affichables
}

void CMap::add(CLight *light) {
	m_TabLight.push_back(light);	// Ajoute light à la liste des objets affichables
}

void CMap::incrementeSelection() {
	m_Selection++;

	if(m_Selection >= (int)m_TabGeo.size()) {
		m_Selection = 0;
	}
}

void CMap::decrementeSelection() {
	m_Selection--;

	if(m_Selection < 0) {
		m_Selection = (int)m_TabGeo.size()-1;

		if(m_Selection < 0)
			m_Selection = 0;
	}
}

void CMap::ChangeSelectionMode() {
	m_bSelection = !m_bSelection;
	m_Selection = 0;
}

bool CMap::IsSelectionMode() {
	return m_bSelection;
}

const char* CMap::getSelectedName() {
	CGeo* geo = m_TabGeo[m_Selection];
	return geo->toString();
}

void CMap::merge(CMap& map) {
	vector<EntryPoint>::iterator iterEntryPoint;
	vector<CGeo*>::iterator iterGeo;
	vector<CMouve*>::iterator iterMouve;
	vector<CLight*>::iterator iterLight;
	vector<CMaterial*>::iterator iterMaterial;

	for(iterEntryPoint=map._entryPoints.begin() ; iterEntryPoint!=map._entryPoints.end() ; iterEntryPoint++)
		_entryPoints.push_back(*iterEntryPoint);

	for(iterGeo=map.m_TabGeo.begin() ; iterGeo!=map.m_TabGeo.end() ; iterGeo++) {
		(*iterGeo)->setMap(this);
		m_TabGeo.push_back(*iterGeo);
	}

	for(iterMouve=map.m_TabMouve.begin() ; iterMouve!=map.m_TabMouve.end() ; iterMouve++)
		m_TabMouve.push_back(*iterMouve);

	for(iterLight=map.m_TabLight.begin() ; iterLight!=map.m_TabLight.end() ; iterLight++)
		m_TabLight.push_back(*iterLight);

	for(iterMaterial=map.m_TabMaterial.begin() ; iterMaterial!=map.m_TabMaterial.end() ; iterMaterial++)
		m_TabMaterial.push_back(*iterMaterial);
}

void CMap::add(EntryPoint entryPoint) {
	_entryPoints.push_back(entryPoint);
}

vector<EntryPoint>& CMap::getEntryPointsList() {
	return _entryPoints;
}

void CMap::add( CPorte *porte ) {
	// Une porte est avant tout un objet géo
	m_TabGeo.push_back( porte );		// Ajoute porte à la liste des objets affichables
	m_TabMouve.push_back( porte );		// Ajoute porte à la liste des objets à rafraichir
}

void CMap::add( CNavette *navette ) {		// Une navette est avant tout un objet géo
	m_TabGeo.push_back( navette );		// Ajoute porte à la liste des objets affichables
	m_TabMouve.push_back( navette );	// Ajoute porte à la liste des objets à rafraichir
}

void CMap::GereContactPlayer(float positionPlayer[3], CPlayer *player ) {
	if(!positionPlayer) {
		float positionPlayerDefault[3];
		positionPlayerDefault[0] = 0.0;
		positionPlayerDefault[1] = 0.0;
		positionPlayerDefault[2] = 0.0;

		positionPlayer = positionPlayerDefault;
	}

	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++)
		(*iterGeo)->GereContactPlayer(positionPlayer, player);	// Gère les contacts entre l'objet géo et le joueur
}

float CMap::GereLaserPlayer(float pos[3], CV3D &Dir, float dist) {
	// Renvoie la distance du premier point de contact entre un rayon laser parti du point 'pos'
	// dans la direction 'Dir' si cette distance est inférieure à 'dist', renvoie 'dist' sinon
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		dist = (*iterGeo)->GereLaserPlayer( pos, Dir, dist );

	return dist;	// Renvoie la distance du premier contact trouvé entre le laser et une face d'objet géo
}

void CMap::EchangeXY() {
TRACE().p( TRACE_MOTEUR3D, "CMap::EchangeXY()%T", this );

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeXY();

	// Geo
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		(*iterGeo)->EchangeXY();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->EchangeXY();
}

void CMap::EchangeXZ() {
TRACE().p( TRACE_MOTEUR3D, "CMap::EchangeXZ()%T", this );

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeXZ();

	// Geo
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		(*iterGeo)->EchangeXZ();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->EchangeXZ();
}

void CMap::EchangeYZ() {
TRACE().p( TRACE_MOTEUR3D, "CMap::EchangeYZ()%T", this );

	// Entry points
	vector<EntryPoint>::iterator iterEntry;
	for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
		(*iterEntry).EchangeYZ();

	// Geo
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		(*iterGeo)->EchangeYZ();

	// Lights
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->EchangeYZ();
}

void CMap::Scale(float scaleX, float scaleY, float scaleZ) {
TRACE().p( TRACE_MOTEUR3D, "CMap::Scale(scaleX=%f,sclaeY=%f,scaleZ=%f)%T", scaleX, scaleY, scaleZ, this );

	if(scaleX!=1.0 || scaleY!=1.0 || scaleZ!=1.0) {
		// Entry points
		vector<EntryPoint>::iterator iterEntry;
		for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
			(*iterEntry).Scale(scaleX, scaleY, scaleZ);

		// Geo
		vector<CGeo*>::iterator iterGeo;
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
			(*iterGeo)->Scale( scaleX, scaleY, scaleZ );

		// Lights
		vector<CLight*>::iterator iterLight;
		for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
			(*iterLight)->Scale( scaleX, scaleY, scaleZ );
	}
}

void CMap::translate(float x, float y, float z) {
TRACE().p( TRACE_MOTEUR3D, "CMap::translate(x=%f,y=%f,z=%f)%T", x, y, z, this );

	if(x!=0.0 || y!=0.0 || z!=0.0) {
		// Entry points
		vector<EntryPoint>::iterator iterEntry;
		for( iterEntry=_entryPoints.begin() ; iterEntry!=_entryPoints.end() ; iterEntry++ )
			(*iterEntry).translate(x, y, z);

		// Geo
		vector<CGeo*>::iterator iterGeo;
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
			(*iterGeo)->translate(x, y, z);

		// Lights
		vector<CLight*>::iterator iterLight;
		for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
			(*iterLight)->translate(x, y, z);
	}
}

bool CMap::Lit(const string &nomFichier) {
	return Lit(*this, nomFichier);
}

bool CMap::Lit(TiXmlElement* el) {
	return false;
}

bool CMap::Save(TiXmlElement* element) {			// Sauve l'objet géo dans un fichier Map
	return false;
}

bool CMap::Lit(CMap& map, const string& mapName) {
	// Répertoire et fichier de la Map
	string partialFileName = mapName;
	bool isResource = JktUtils::RessourcesLoader::getFileRessource(partialFileName);

	// Si le fichier Map n'est pas une resource alors c'est une map classique
	if(!isResource) {
		partialFileName = ".\\map\\" + partialFileName;
	}

	map.setName(mapName);										// Nom de la Map
	map._filename = partialFileName + ".map.xml";				// Chemin complet fichier Map XML
	map._binariesDirectory = partialFileName + "\\";			// Chemin des fichier binaires de la Map

	// Lecture XML de la Map
	bool result = false;

	TiXmlDocument document(map._filename.c_str());

	if(document.LoadFile()) {
		// Element de map
		TiXmlElement* elMap = document.FirstChildElement(Xml::MAP);
		if(!elMap)
			throw CErreur(0,"Fichier map corrompu");

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

					if(!subMapName)
						throw CErreur(0, "Fichier Map corrompu : Nom de la sous-Map a importer manquant");

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
					Lit(subMap, string(subMapName));

					subMap.translate(translation[0], translation[1], translation[2]);
					subMap.Scale(scaling[0], scaling[1], scaling[2]);

					// Fusion des Map
					map.merge(subMap);
					subMap.m_TabGeo.clear();
					subMap.m_TabMouve.clear();
					subMap.m_TabLight.clear();
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

					EntryPoint* entry = EntryPointMaker::Lit(el);

					if(entry) {
						map.add(*entry);
						delete entry;
					}
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

				CMaterial* mat = CMaterialMaker::Lit(el, map._binariesDirectory);

				if(mat)
					map.add(mat);
			}
		}

		// Lecture des lumières
		TiXmlElement* elLight = elMap->FirstChildElement(Xml::LUMIERES);

		if(elLight) {
			for(TiXmlElement* el=elLight->FirstChildElement(); el!=0; el=el->NextSiblingElement())
			{
				if(strcmp(Xml::LUMIERE, el->Value())) {
					Xml::throwCorruptedMapFileException(Xml::LUMIERE, el->Value());
				}

				CLight* lum = CLightMaker::Lit(el);

				if(lum)
					map.add(lum);
			}
		}

		// Lecture des objets géométriques
		TiXmlElement* elGeo = elMap->FirstChildElement(Xml::GEOS);

		if(elGeo) {
			for(TiXmlElement* el=elGeo->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				if(strcmp(Xml::GEO, el->Value())) {
					Xml::throwCorruptedMapFileException(Xml::GEO, el->Value());
				}

				CGeo* geo = CGeoMaker::Lit(el, &map);

				if(geo)
					map.add(geo);
			}
		}

		result = true;
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Ouverture impossible : " << map._filename;
	}

	return result;
}

void CMap::Init() throw(JktUtils::CErreur) {	// Initialisation de la CMap
	// Initialisation des object géométriques
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->Init();
	}
}

void CMap::initPlugins() {
	vector<string>::iterator it;

	for(it = _plugins.begin() ; it != _plugins.end() ; it++) {
		Fabrique::getPluginEngine()->activateMapPlugin(*it, _binariesDirectory);
	}

	_isPluginsActivated = true;	// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::freePlugins() {
	Fabrique::getPluginEngine()->deactivateMapPlugins();
	_isPluginsActivated = false;			// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::initGL() {
	// Letcure des fichiers de texture
	vector<CMaterial*>::iterator iterMat;

	for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ ) {
		CMaterial* material = *iterMat;
		material->initGL();
	}

	// Initialisation des object géométriques dans le contexte OpenGL
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->initGL();
	}

	_isGlActivated = true;	// Indique que les éléments OpenGL de la MAP ont bien été initialisés
}

void CMap::freeGL() {
	// Initialisation des object géométriques dans le contexte OpenGL
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->freeGL();
	}

	_isGlActivated = false;
}

bool CMap::Save(const string nomFichier) {
TRACE().p( TRACE_MOTEUR3D, "CMap::Save() %T", this );

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

		for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
			(*iterLight)->Save( elLum );
	}

	// Sauvegarde des objets geometriques
	{
		TiXmlElement *elGeo = new TiXmlElement(Xml::GEOS);
		elMap->LinkEndChild(elGeo);
		vector<CGeo*>::iterator iterGeo;

		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
			(*iterGeo)->Save( elGeo );		// Sauvegarde des paramètres de l'objet
	}

	document.SaveFile(nomFichierMap.c_str());

	cout << endl << "Sauvegarde du fichier MAP Ok !!";

TRACE().p( TRACE_MOTEUR3D, "CMap::SaveFichierMap() Sauvegarde du fichier MAP Ok%T", this );
	return true;
}

bool CMap::Contact(const float pos[3], const float dist) {
	bool var = false;	// Pas de contact par défaut
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		var = (*iterGeo)->Contact( pos, dist );

		if(var)	// Si un triangle a été trouvé à une distance inférieure à 'dist' de la position 'pos'
			break;
	}

	return var;
}

void CMap::Refresh(CGame *game) {
	vector<CMouve*>::iterator iterMouve;

	for(iterMouve=m_TabMouve.begin() ; iterMouve!=m_TabMouve.end() ; iterMouve++)
		(*iterMouve)->Refresh(game);
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
			cerr << endl << __FILE__ << ":" << __LINE__ << " CMap::afficheToutesTextures : Materiau de type inconnu";
		}
	}
}

vector<CLight*>& CMap::getLights() {
	return m_TabLight;
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
