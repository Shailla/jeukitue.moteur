
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#include "tinyxml.h"

#include "util/Tableau.h"
#include "util/Erreur.h"
#include "util/V3D.h"
#include "spatial/MapLogger.h"
#include "spatial/basic/MapObject.h"

class CGame;
class CPlayer;

using namespace jkt;

namespace jkt {
class CLight;
class Dirigeable;
class Refreshable;
class CMaterial;
class CPorte;
class CNavette;
class CGeoObject;
class EntryPoint;
class CMoteurParticules;
class CheckPlayerInZone;

class CMap : public MapObject {
	static const char* identifier;
	string tostring;

	string _filename;								// Nom du fichier de la Map (par exemple "Monde.map.xml")
	string _binariesDirectory;						// Répertoires des binaires de la Map (textures, plugins, ...)

	map<int, CMap*> _subMaps;

	map<int, MapObject*> _geoDescriptions;

	// Objets de la Map
	vector<MapObject*> _objects;						// Liste des objets g�om�triques
	vector<Geometrical*> _geos;							// Liste des objets g�om�triques
	vector<SolidAndTargettable*> _solidAndTargettables;	// Liste des objets g�om�triques
	vector<Drawable*> _drawables;						// Liste des objets � afficher
	vector<Refreshable*> _refreshables;					// Liste des objets n�cessitant une actualisation (portes,...)

	// Lumières et autres caractéristiques de la Map
	vector<CLight*> _lights;							// Lumi�res
	vector<EntryPoint*> _entryPoints;					// Points d'entrée des joueurs sur la Map

	// Objets complexes de la Map
	vector<CMoteurParticules*> _particulesEngines;	// Liste des moteurs de particules de la Map

	vector<string> _plugins;						// Liste des plugins de la Map

	bool _bSelection;								// Indique si le mode s�lection est actif ou non

	bool _isGlInitialized;							// Indique si les �l�ments OpenGL de la MAP ont �t� initialis�s
	bool _isPluginsInitialized;						// Indique si les plugins de la MAP ont été initialis�s

	CMap(CMap* parent, const string& nomFichier, MapLogger* mapLogger) throw(jkt::CErreur);	// Construction de la Map par lecture d'un fichier *.map.xml

	bool afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex, int& posX, int& posY, int& index);
public:
	vector<CMaterial*> _materials;		// Liste des matériaux A VOIR : devrait être membre privé

		// Constructeurs / destructeur
	CMap(CMap* parent);
	CMap(CMap* parent, const string& nomFichier) throw(jkt::CErreur);	// Construction de la Map par lecture d'un fichier *.map.xml
	~CMap();

	void clear();
	MapObject* clone();

	const char* toString();						// Description résumée de l'objet

	// Object
	void init() throw(jkt::CErreur) override;	// Initialisation de la CMap
	static bool Lit(CMap& map, const string &mapName, MapLogger* mapLogger);
	bool Lit(const string &nomFichier, MapLogger* mapLogger);
	bool Lit(TiXmlElement* el, MapLogger* mapLogger) override;
	bool Save(TiXmlElement* element) override;			// Sauve l'objet géo dans un fichier Map

	// Gestion des plugins de la Map
	void initPlugins();		// Chargement / exécution des plugins de la Map
	void freePlugins();		// Libération des plugins de la Map

	// Drawable
	void initGL() override;		// Initialisation du contexte OpenGL
	void freeGL() override;		// Libération du contexte OpenGL

	void Affiche() override;											// Affiche l'ensemble des éléments 3D de cette Map
	void AfficheSelection(float r,float v,float b) override;			// Affiche l'objet géométrique en couleur unique

	bool Save(const string nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

	// Refreshable

	void refresh(CGame *game) override;									// Rafraichissement des classes list�es dans m_TabMouve

	/**
	 * Affiche toutes les textures de la MAP dans un rectangle dont le coin bas gauche est en (x,y) et les dimensions sont (tailleX, tailleY).
	 *
	 * @param x position horizontale du coin bas gauche du rectangle dans lequel les texture vont être affichees
	 * @param y position verticale du coin bas gauche du rectangle dans lequel les texture vont être affichees
	 * @param tailleX taille horizontale du rectangle dans lequel les texture vont être affichees
	 * @param tailleY taille verticale du rectangle dans lequel les texture vont être affichees
	 * @param nbrX nombre de textures sur une ligne
	 * @param nbrY nombre de textures sur une colonne
	 * @param page numero de la page  de texures à afficher (sur une page on affiche nbrX*nbrY textures)
	 * @return numero de la page de texture affichee ou 0 si on a depassé la dernière page
	 */
	int afficheDamierTextures(int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int page);

	void addMapObject(int id, MapObject* geo, MapLogger* mapLogger);

	/** Find the MapObject in all sub-maps */
	MapObject* findMapObject(int id);

	/** Find the MapObject in the current Map instance */
	MapObject* getMapObject(int id);

	void add(CMap* map);										// Intègre tous les éléments d'une autre Map dans celle-ci
	void merge(int mapId, MapLogger* mapLogger = 0);			// Intègre tous les éléments d'une autre Map dans celle-ci

	void add(MapObject* object);			// Ajoute un GeoObject à la map
	void add(Dirigeable* dirigeable);
	void add(CPorte *porte);				// Ajoute une porte à la map
	void add(CNavette *navette);			// Ajoute une navette à la map
	void add(CMoteurParticules* engine);	// Ajoute un moteur de particules à la map
	void add(CLight *light);				// Ajoute une lumière à la map
	void add(CMaterial *mat);				// Ajoute un matériau à la map
	void add(CheckPlayerInZone* detector);

	// Geometrical
	void EchangeXY() override;										// Echange les coordonnées X et Y des objets géo du map
	void EchangeXZ() override;										// Echange les coordonnées X et Z des objets géo du map
	void EchangeYZ() override;										// Echange les coordonnées Y et Z des objets géo du map
	void Scale(float scaleX, float sclaeY, float scaleZ) override;	// Homothétie la Map (coordonnées multipliées par scale)
	void translate(float x, float y, float z) override;				// Translation de la Map selon x, y, z

	// Gestion des contacts
	bool Contact(const float pos[3], const float dist) override;	// Indique s'il y a un triangle � une distance inférieure à 'dist' de la position 'pos'

	/**
	 * positionPlayer : position du jouer ou position calculé ce qui permet d'optimiser certains contacts quand un objet a bougé en considérant que c'est plutôt le
	 */
	void GereContactPlayer(float positionPlayer[3], CPlayer *player) override;								// Gère tous les contacts entre la map et les joueurs
	float GereLaserPlayer(float pos[3], jkt::CV3D &Dir, float dist) override;	// Envoie d'un laser sur la map

	map<int, CMap*>& getSubMaps();
	vector<CLight*>& getLights();
	vector<MapObject*>& getMapObjects();

	// Sélection
	void ChangeSelectionMode();
	bool IsSelectionMode();

	// Points d'entrée des personnages joueurs
	void add(EntryPoint* entryPoint);
	vector<EntryPoint*>& getEntryPointsList();
};

}	// JktMoteur

#endif

