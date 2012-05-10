
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

CMap::CMap()
{
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap() begin%T", this );
	m_bSelection = false;
}

CMap::CMap(const string &nomFichier) throw(JktUtils::CErreur)
{
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap(nomFichier=%s) begin%T", nomFichier.c_str(), this );
	if( !Lit(nomFichier) ) {
		cerr << endl << "Erreur à la lecture du fichier MAP : " << nomFichier;
	}

	m_bSelection = false;

	Init();
TRACE().p( TRACE_MOTEUR3D, "CMap::CMap() end%T", this );
}

CMap::~CMap()
{
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

void CMap::Affiche()	// Affiche tous les objets géo de du Map
{
	glEnableClientState( GL_VERTEX_ARRAY );

	vector<CGeo*>::iterator iterGeo;
	if(m_bSelection)
	{
		int i=0;
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		{
			if(i != m_Selection)
				(*iterGeo)->Affiche();			// Affichage de l'objet géo
			else
				(*iterGeo)->AfficheSelection(1.0,0.0,0.0);

			i++;
		}
	}
	else
	{
		for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ ) {
			(*iterGeo)->Affiche();			// Affichage de l'objet géo
		}
	}

	glDisable( GL_VERTEX_ARRAY );
}

void CMap::Add(CGeo* geo)
{
	m_TabGeo.push_back( geo );	// Ajoute geo à la liste des objets affichables
}

void CMap::Add( CMaterial *mat ) {
	m_TabMaterial.push_back( mat );	// Ajoute mat à la liste des objets affichables
}

void CMap::Add( CLight *light )
{
	m_TabLight.push_back( light );	// Ajoute light à la liste des objets affichables
}

void CMap::incrementeSelection()
{
	m_Selection++;
	if(m_Selection >= (int)m_TabGeo.size())
	{
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

void CMap::add(EntryPoint entryPoint) {
	_entryPoints.push_back(entryPoint);
}

vector<EntryPoint>& CMap::getEntryPointsList() {
	return _entryPoints;
}

void CMap::Add( CPorte *porte ) {
	// Une porte est avant tout un objet géo
	m_TabGeo.push_back( porte );		// Ajoute porte à la liste des objets affichables
	m_TabMouve.push_back( porte );		// Ajoute porte à la liste des objets à rafraichir
}

void CMap::Add( CNavette *navette )
{		// Une navette est avant tout un objet géo
	m_TabGeo.push_back( navette );		// Ajoute porte à la liste des objets affichables
	m_TabMouve.push_back( navette );	// Ajoute porte à la liste des objets à rafraichir
}

void CMap::GereContactPlayer( CPlayer *player )
{
	float pos[3];
	player->getPosition( pos );

	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		(*iterGeo)->GereContactPlayer( pos, player );	// Gère les contacts entre l'objet géo et le joueur
}

float CMap::GereLaserPlayer( float *pos, CV3D &Dir, float dist )
{	// Renvoie la distance du premier point de contact entre un rayon laser parti du point 'pos'
	// dans la direction 'Dir' si cette distance est inférieure à 'dist', renvoie 'dist' sinon
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
		dist = (*iterGeo)->GereLaser( pos, Dir, dist );

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

bool CMap::Lit(const string &nomFichier)
{
	string nomFichierComplet = string(nomFichier);
	bool isResource = JktUtils::RessourcesLoader::getFileRessource(nomFichierComplet);

	// Si le fichier Map n'est pas une resource alors c'est une map classique
	if(!isResource) {
		nomFichierComplet = ".\\map\\" + nomFichierComplet;
	}

	string nomFichierXml = nomFichierComplet + ".map.xml";	// Chemin complet fichier Map XML
	string repertoireBinaires = string(nomFichierComplet);	// Chemin des fichier binaires de la Map
	bool result = false;

	TiXmlDocument document(nomFichierXml.c_str());

	if(document.LoadFile()) {
		// Element de map
		TiXmlElement* elMap = document.FirstChildElement(Xml::MAP);
		if(!elMap)
			throw CErreur(0,"Fichier map corrompu");

		// Lecture des point d'entrée des joueurs
		{
			TiXmlElement* elEntry = elMap->FirstChildElement(Xml::ENTRYPOINTS);

			if(elEntry) {
				for(TiXmlElement* el=elEntry->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
					if(strcmp(Xml::ENTRYPOINT, el->Value())) {
						string erreur = "Fichier MAP corrompu : '";
						erreur += Xml::MATERIAU;
						erreur += "' attendu, '";
						erreur += el->Value();
						erreur += "' recu";
						throw CErreur(0, erreur);
					}

					EntryPoint* entry = EntryPointMaker::Lit(el);

					if(entry)
						add(*entry);
				}
			}
		}

		// Lecture des matériaux
		TiXmlElement* elMat = elMap->FirstChildElement(Xml::MATERIAUX);

		if(elMat) {
			for(TiXmlElement* el=elMat->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				if(strcmp(Xml::MATERIAU, el->Value())) {
					string erreur = "Fichier MAP corrompu : '";
					erreur += Xml::MATERIAU;
					erreur += "' attendu, '";
					erreur += el->Value();
					erreur += "' recu";
					throw CErreur(0, erreur);
				}

				CMaterial* mat = CMaterialMaker::Lit(el, repertoireBinaires);

				if(mat)
					Add(mat);
			}
		}

		// Lecture des lumières
		TiXmlElement* elLight = elMap->FirstChildElement(Xml::LUMIERES);

		if(elLight) {
			for(TiXmlElement* el=elLight->FirstChildElement(); el!=0; el=el->NextSiblingElement())
			{
				if(strcmp(Xml::LUMIERE, el->Value()))
				{
					string erreur = "Fichier map corrompu : '";
					erreur += Xml::LUMIERE;
					erreur += "' attendu, '";
					erreur += el->Value();
					erreur += "' recu";
					throw CErreur(0, erreur);
				}

				CLight* lum = CLightMaker::Lit(el);

				if(lum)
					Add(lum);
			}
		}

		// Lecture des objets géométriques
		TiXmlElement* elGeo = elMap->FirstChildElement(Xml::GEOS);

		if(elGeo) {
			for(TiXmlElement* el=elGeo->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
				if(strcmp(Xml::GEO, el->Value())) {
					string erreur = "Fichier map corrompu : '";
					erreur += Xml::GEO;
					erreur += "' attendu, '";
					erreur += el->Value();
					erreur += "' recu";
					throw CErreur(0, erreur);
				}

				CGeo* geo = CGeoMaker::Lit(el, this);

				if(geo)
					Add(geo);
			}
		}

		result = true;
	}
	else {
		cerr << endl << "Ouverture impossible : " << nomFichierXml;
	}

	return result;
}

/*bool CMap::LitFichier(const string &nomFichier)
{
TRACEMETHOD TRACE_MOTEUR3D, this, "CMap::LitFichier(nomFichier=%s)%T", nomFichier.c_str(),this );
	string nomFichierMap = nomFichier + ".map";
	string mot;

	cout << endl;

	try
	{
			// Ouverture du fichier
		CIfstreamMap::m_OffsetMateriaux = 0;
		CIfstreamMap fichier( &m_TabMaterial );
		if( !fichier.open( nomFichierMap ) )
		{
			string erreur( "Ouverture de fichier impossible (" );
			erreur += nomFichierMap;
			erreur += ")";
			throw CErreur( 0, erreur );
		}

		cout << "\nOuverture d'un fichier Map (" << nomFichierMap << ")";

		while( !!fichier )	// Si la fin du fichier n'est pas atteinte
		{
			if( !(fichier>>mot) )
				break;

			if( mot=="EntreeJoueurs" )		// Lecture d'un point d'entrée
			{
				float pos[3];
				fichier >> pos[0] >> pos[1] >> pos[2];
				m_EntreeJoueurs.push_back( CV3D( pos ) );
				cout << "\nPoint d'entree joueur : " << pos[0] << "\t" << pos[1] << "\t" << pos[2];
				cout << "--->";
				for( int i=0 ; i<3 ; i++ )
					cout << "\nPoint d'entree joueur : " << m_EntreeJoueurs[i].X << "\t" << m_EntreeJoueurs[i].Y << "\t" << m_EntreeJoueurs[i].Z;
				cout << "<---";
			}
			else if( mot==CGeoObject::identifier )		// Lecture d'un objet géométrique
			{
				CGeoObject *geo = new CGeoObject(this);	// Nouvel objet géométrique

				geo->setOffsetMateriau( fichier.OffsetMateriaux() );	// Décale les réf matériau de l'offset

				if( !geo->LitFichier( fichier ) )		// Lit cet objet dans le fichier
				{
					cerr << "\nObjet géométrique (" << geo->getName() << ") corrompu (1) dans : " << nomFichierMap;
					delete geo;
					return false;		// Echec de la lecture du fichier Map
				}

				geo->Init();
				Add( geo );			// Ajoute-le à la map
			}
			else if( mot==CTextureMaterialGeo::identifier )		// Lecture d'un objet géométrique
			{
				CTextureMaterialGeo *geo = new CTextureMaterialGeo(this);	// Nouvel objet géométrique

				geo->setOffsetMateriau( fichier.OffsetMateriaux() );	// Décale les réf matériau de l'offset

				if( !geo->LitFichier( fichier ) )		// Lit cet objet dans le fichier
				{
					cerr << "\nObjet géométrique (" << geo->getName() << ") corrompu (1) dans : " << nomFichierMap;
					delete geo;
					return false;		// Echec de la lecture du fichier Map
				}

				geo->Init();
				Add( geo );			// Ajoute-le à la map
			}
			else if( mot==CSimpleMaterialGeo::identifier )		// Lecture d'un objet géométrique
			{
				CSimpleMaterialGeo *geo = new CSimpleMaterialGeo(this);	// Nouvel objet géométrique

				geo->setOffsetMateriau(fichier.OffsetMateriaux());	// Décale les réf matériau de l'offset

				if( !geo->LitFichier( fichier ) )		// Lit cet objet dans le fichier
				{
					cerr << "\nObjet géométrique (" << geo->getName() << ") (2) corrompu dans : " << nomFichierMap;
					delete geo;
					return false;		// Echec de la lecture du fichier Map
				}

				geo->Init();
				Add( geo );			// Ajoute-le à la map
			}
			else if( mot==CSimpleGeo::identifier )	// Lecture d'un objet géométrique
			{
				cout << endl << "Lecture d'un SimpleGeo";
				CSimpleGeo *geo = new CSimpleGeo(this);	// Nouvel objet géométrique

				if( !geo->LitFichier( fichier ) )		// Lit cet objet dans le fichier
				{
					cerr << "\nObjet géométrique (" << geo->getName() << ") (3) corrompu dans : " << nomFichierMap;
					delete geo;
					return false;		// Echec de la lecture du fichier Map
				}

				geo->Init();
				Add( geo );			// Ajoute-le à la map
			}
			else if( mot==CPorte::identifier)			// Lecture d'une porte
			{
				CPorte *porte = new CPorte( this );			// Nouvel objet géométrique

				porte->setOffsetMateriau( fichier.OffsetMateriaux() );	// Décale les réf matériau de l'offset

				if( !porte->LitFichier( fichier ) )		// Lit cet objet dans le fichier
				{
					cerr << "\nPorte corrompue dans : " << nomFichierMap;
					delete porte;
					return false;		// Echec de la lecture du fichier Map
				}

				porte->Init();
				Add( porte );			// Ajoute-la à la map
			}
			else if( mot==CNavette::identifier )		// Lecture d'une navette
			{
				CNavette *navette = new CNavette( this );			// Nouvel objet géométrique

				navette->setOffsetMateriau( fichier.OffsetMateriaux() );	// Décale les réf matériau de l'offset

				if( !navette->LitFichier( fichier ) )		// Lit cet objet dans le fichier
				{
					cerr << "\nPorte corrompue dans : " << nomFichierMap;
					delete navette;
					return false;		// Echec de la lecture du fichier Map
				}

				navette->Init();
				Add( navette );			// Ajoute-la à la map
			}
			else if( mot=="MateriauSimple" )	// Lecture d'un matériau simple
			{
				CMaterial *mat = new CMaterial();
				if( !mat->LitFichier( fichier ) )
				{
					cerr << "\nMatériau corrompu dans : " << nomFichierMap;
					delete mat;
					return false;		// Echec de la lecture du fichier Map
				}
				Add( mat );
			}
			else if( mot=="MateriauMulti" )		// Lecture d'un matériau multiple
			{
				CMaterialMulti *matMulti = new CMaterialMulti();
				if( !matMulti->LitFichier( fichier ) )
				{
					cerr << "\nMatériauMulti corrompu dans : " << nomFichierMap;
					delete matMulti;
					return false;		// Echec de la lecture du fichier Map
				}
				Add( matMulti );
			}
			else if( mot=="MateriauTexture" )	// Lecture d'un matériau texture
			{
				CMaterialTexture *matTex = new CMaterialTexture();
				if( !matTex->LitFichier( fichier ) )
				{
					cerr << "\nMateriau corrompu dans : " << nomFichierMap;
					delete matTex;
					return false;		// Echec de la lecture du fichier Map
				}
				Add( matTex );
			}
			else if( mot=="LumiereOmni" )		// Lecture d'une lumière omnidirectionnelle
			{
				cout << endl << "Lecture d'une lumiere omni dans le fichier MAP";
				CLightOmni *lightOmni = new CLightOmni();
				if( !lightOmni->LitFichier( fichier ) )
				{
					cerr << "\nLumière omni corrompue dans : " << nomFichierMap;
					delete lightOmni;
					return false;			// Echec de la lecture du fichier Map
				}
				Add( lightOmni );
			}
			else if( mot=="LumiereTarget" )		// Lecture d'une lumière directive
			{
				cout << endl << "Lecture d'une lumiere target dans le fichier MAP";
				CLightTarget *lightTarget = new CLightTarget();
				if( !lightTarget->LitFichier( fichier ) )
				{
					cerr << "\nLumière target corrompue dans : " << nomFichierMap;
					delete lightTarget;
					return false;			// Echec de la lecture du fichier Map
				}
				Add( lightTarget );
			}
			else if( mot=="make" )				// Lecture d'une construction
			{
				fichier >> mot;
				if( mot==CPorte::identifier )
				{
					fichier >> mot;
					if( mot=="with" )
					{
						fichier >> mot;
						if( mot==CGeoObject::identifier )
						{
							fichier >> mot;

							while( mot=="MateriauTexture" )			// Un matériau a été trouvé ?
							{
								CMaterialTexture *matTex = new CMaterialTexture();
								if( !matTex->LitFichier( fichier ) )
								{
									cerr << "\nMatériau corrompu dans : " << nomFichierMap;
									delete matTex;
									return false;		// Echec de la lecture du fichier Map
								}
								Add( matTex );

								fichier >> mot;
							}

							if( mot==CGeoObject::identifier )
							{
								CPorte *porte = new CPorte( this );			// Nouvel objet géométrique
								porte->setOffsetMateriau( fichier.OffsetMateriaux() );	// Si un matériau est référencé décale sa réf de l'offset
								if( !porte->LitFichierGeoObject( fichier ) )
								{
									cerr << "\nGeoObject de la Porte corrompue (" << nomFichierMap << ")";
									delete porte;
									return false;
								}

                                fichier >> mot;
								if( mot=="DATA_Porte" )
								{
									porte->LitFichierPorte( fichier );	// Lit les données porte

									porte->Init();
									Add( porte );			// Ajoute-la à la map
								}
								else
								{
									cerr << "\nPorte corrompue 2 (" << nomFichierMap << ")";
									delete porte;
									return false;
								}
							}
							else
							{
								cerr << "\nPorte corrompue 3 (" << nomFichierMap << ")";
								return false;
							}
						}
					}
				}
				else if( mot=="Navette" )
				{
					fichier >> mot;
					if( mot=="with" )
					{
						fichier >> mot;
						if( mot==CGeoObject::identifier )
						{
							fichier >> mot;

							while( mot=="MateriauTexture" )			// Un matériau a été trouvé ?
							{
								CMaterialTexture *matTex = new CMaterialTexture();
								if( !matTex->LitFichier( fichier ) )
								{
									cerr << "\nMatériau corrompu dans : " << nomFichierMap;
									delete matTex;
									return false;		// Echec de la lecture du fichier Map
								}
								Add( matTex );

								fichier >> mot;
							}

							if( mot==CGeoObject::identifier )
							{
								CNavette *navette = new CNavette( this );			// Nouvel objet géométrique
								navette->setOffsetMateriau( fichier.OffsetMateriaux() );	// Si un matériau est référencé décale sa réf de l'offset
								if( !navette->LitFichierGeoObject( fichier ) )
								{
									cerr << "\nGeoObject de la Navette corrompu (" << nomFichierMap << ")";
									delete navette;
									return false;
								}

                                fichier >> mot;
								if( mot=="DATA_Navette" )
								{
									navette->LitFichierNavette( fichier );	// Lit les données porte

									navette->Init();
									Add( navette );			// Ajoute-la à la map
								}
								else
								{
									cerr << "\nNavette corrompue 2 (" << nomFichierMap << ")";
									delete navette;
									return false;
								}
							}
							else
							{
								cerr << "\nNavette corrompue 3 (" << nomFichierMap << ")";
								return false;
							}
						}
					}
				}
			}
		}

		cout << "\nLecture du fichier MAP (" << nomFichier << ") Ok !!!\n";
		return true;
	}

	catch( CErreur erreur )
	{
		cerr << "\nErreur (exeption) : " << erreur.toString();
	}

	return false;
}*/

bool CMap::Init() throw(JktUtils::CErreur) {	// Initialisation de la CMap
	// Initialisation des object géométriques
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->Init();
	}

	return true;
}

bool CMap::initGL() {
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

	return true;
}

bool CMap::freeGL()
{
	// Initialisation des object géométriques dans le contexte OpenGL
	vector<CGeo*>::iterator iterGeo;

	for(iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++) {
		CGeo* geo = (*iterGeo);
		geo->freeGL();
	}

	return true;
}

/*bool CMap::SaveFichierMap( const string &nomFichier )
{
TRACE().p( TRACE_MOTEUR3D, "CMap::SaveFichierMap() %T", this );

		// CREATION DES FICHIERS
	string nomFichierMap = "./map/" + nomFichier + ".map";

	ofstream fichier( nomFichierMap.c_str() );

	fichier << "Start";		// Indique le début d'une description de Map ou de sous-Map

		// SAUVEGARDE DES MATERIAUX
	vector<CMaterial*>::iterator iterMat;
	for( iterMat=m_TabMaterial.begin() ; iterMat!=m_TabMaterial.end() ; iterMat++ )
		(*iterMat)->SaveFichierMap(fichier);		// Sauvegarde du matériau

		// SAUVEGARDE DES LUMIERES
	vector<CLight*>::iterator iterLight;
	for( iterLight=m_TabLight.begin() ; iterLight!=m_TabLight.end() ; iterLight++ )
		(*iterLight)->SaveFichierMap( fichier );

		// SAUVEGARDE DES OBJETS GEOMETRIQUES
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
	{
		(*iterGeo)->SaveNameType( fichier );		// Sauvegarde du nom du type d'objet géo
		(*iterGeo)->SaveFichierMap( fichier );		// Sauvegarde des paramètres de l'objet
	}

	fichier.close();
	cout << endl << "Sauvegarde du fichier MAP Ok !!";

TRACE().p( TRACE_MOTEUR3D, "CMap::SaveFichierMap() Sauvegarde du fichier MAP Ok%T", this );
	return true;
}*/

bool CMap::Save(const string nomFichier)
{
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

bool CMap::Contact( const float pos[3], float dist )
{
	bool var = false;	// Pas de contact par défaut
	vector<CGeo*>::iterator iterGeo;
	for( iterGeo=m_TabGeo.begin() ; iterGeo!=m_TabGeo.end() ; iterGeo++ )
	{
		var = (*iterGeo)->Contact( pos, dist );
		if( var )	// Si un triangle a été trouvé à une distance inférieure à 'dist' de la position 'pos'
			break;
	}

	return var;
}

void CMap::Refresh( CGame *game )
{
	vector<CMouve*>::iterator iterMouve;
	for( iterMouve=m_TabMouve.begin() ; iterMouve!=m_TabMouve.end() ; iterMouve++ )
		(*iterMouve)->Refresh( game );
}

void CMap::afficheToutesTextures( int x, int y )
{
	glEnable( GL_TEXTURE_2D );
	int nbrX = 0;
	int nbrY = 0;

	vector<CMaterial*>::iterator iter;
	for( iter=m_TabMaterial.begin() ; iter!=m_TabMaterial.end() ; iter++ )
	{
		CMaterial *m = *iter;

		if( m->Type() == CMaterial::MAT_TYPE_TEXTURE )
		{
				// Initialisation pour la texture
			CMaterialTexture *matRef = (CMaterialTexture*)m;

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture( GL_TEXTURE_2D, matRef->texName );

			glBegin( GL_QUADS );		//AVOIR : mettre les coord de tex dans glArrayElement
				glTexCoord2f( 0.0f, 0.0f );
				glVertex2i( (x/8)*nbrX,		(y/8)*nbrY		);

				glTexCoord2f( 1.0f, 0.0f );
				glVertex2i( (x/8)*(nbrX+1), (y/8)*nbrY		);

				glTexCoord2f( 1.0f, 1.0f );
				glVertex2i( (x/8)*(nbrX+1), (y/8)*(nbrY+1)	);

				glTexCoord2f( 0.0f, 1.0f );
				glVertex2i( (x/8)*nbrX,		(y/8)*(nbrY+1)	);
			glEnd();

			glDisable( GL_TEXTURE_2D );
			glColor3f( 1.0f, 0.0f, 0.0f );
			glLineWidth( 2 );

			glBegin( GL_LINE_LOOP );		//AVOIR : mettre les coord de tex dans glArrayElement
				glVertex2i( (x/8)*nbrX,		(y/8)*nbrY		);
				glVertex2i( (x/8)*(nbrX+1), (y/8)*nbrY		);
				glVertex2i( (x/8)*(nbrX+1), (y/8)*(nbrY+1)	);
				glVertex2i( (x/8)*nbrX,		(y/8)*(nbrY+1)	);
			glEnd();

			glEnable( GL_TEXTURE_2D );

			if( nbrX >= 8 )
			{
				nbrX = 0;
				nbrY++;
			}
			else
			{
				nbrX++;
			}

		}
		else if( m->Type() == CMaterial::MAT_TYPE_SIMPLE )
		{
		}
		else if( m->Type() == CMaterial::MAT_TYPE_MULTI )
		{
/*					// Initialisation pour la texture
			CMaterialMulti *matMulti = (CMaterialMulti*)m;
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			for( int i=0; i < m_NumTexFaces ; i++ )	//dessine toutes les faces de la map
			{
				float *texVertex = TabTex[ matMulti->m_TabMat[ m_TabSubMat[i] ]->m_CanalTex ]->TexVertex;
				int *texFaces = TabTex[ matMulti->m_TabMat[ m_TabSubMat[i] ]->m_CanalTex ]->TexFaces;

				glBindTexture( GL_TEXTURE_2D, matMulti->m_TabMat[ m_TabSubMat[i] ]->texName );

				glLineWidth( 1 );

				glBegin( JKT_RenderMode );	//AVOIR : mettre les coord de tex dans glArrayElement
					glTexCoord2fv( &texVertex[3*texFaces[3*i]] );
					glArrayElement( m_TabFaces[3*i] );

					glTexCoord2fv( &texVertex[3*texFaces[(3*i)+1]] );
					glArrayElement( m_TabFaces[(3*i)+1] );

					glTexCoord2fv( &texVertex[3*texFaces[(3*i)+2]] );
					glArrayElement( m_TabFaces[(3*i)+2] );
				glEnd();
			}
*/			}
		else
		{
			cerr << "\nCMap::afficheToutesTextures : Materiau de type inconnu";
		}
	}
}

}	// JktMoteur
