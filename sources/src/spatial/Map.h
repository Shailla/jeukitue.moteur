
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <string>
#include <vector>
#include <map>

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
	std::string tostring;

	std::string _filename;								// Nom du fichier de la Map (par exemple "Monde.map.xml")
	std::string _binariesDirectory;						// Répertoires des binaires de la Map (textures, plugins, ...)

	std::map<int, CMap*> _subMaps;

	std::vector<MapObject*> _objects;
	std::map<int, MapObject*> _objectDescriptions;
	std::map<std::string, int> _references;						// clé=référence ; valeur=identifiant : Référence de chaque objet dans la Map (référence = identifiant normalement unique au sein de la Map)

	// Objets de la Map
	std::vector<MapObject*> _geos;
	std::vector<MapObject*> _solidAndTargettables;
	std::vector<MapObject*> _drawables;
	std::vector<MapObject*> _refreshables;					// Liste des objets nécessitant une actualisation (portes,...)

	// Lumières et autres caractéristiques de la Map
	std::vector<CLight*> _lights;								// Lumières
	std::vector<EntryPoint*> _entryPoints;						// Points d'entrée des joueurs sur la Map

	// Objets complexes de la Map
	std::vector<CMoteurParticules*> _particulesEngines;			// Liste des moteurs de particules de la Map

	std::vector<std::string> _plugins;							// Liste des plugins de la Map

	bool _isGlInitialized;										// Indique si les éléments OpenGL de la MAP ont été initialisés
	bool _isPluginsInitialized;									// Indique si les plugins de la MAP ont été initialisés

	bool afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex, int& posX, int& posY, int& index);

	void addMapObject(MapObject* object);
public:
	std::vector<CMaterial*> _materials;		// Liste des mat�riaux A VOIR : devrait �tre membre priv�

		// Constructeurs / destructeur
	CMap(CMap* parent);
	~CMap();

	void clear();
	MapObject* clone();

	const char* toString();						// Description résumée de l'objet

	// Object
	void init() noexcept(false) override;	// Initialisation de la CMap
	static bool Lit(CMap& map, const std::string &mapName, MapLogger* mapLogger) noexcept(false);
	bool Lit(const std::string &nomFichier, MapLogger* mapLogger) noexcept(false);
	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false) override;
	bool Save(TiXmlElement* element) noexcept(false) override;			// Sauve l'objet géo dans un fichier Map

	// Gestion des plugins de la Map
	void initPlugins();			// Chargement / exécution des plugins de la Map
	void freePlugins();			// Libération des plugins de la Map

	// Drawable
	void initGL() override;		// Initialisation du contexte OpenGL
	void freeGL() override;		// Libération du contexte OpenGL

	void affiche() override;											// Affiche l'ensemble des éléments 3D de cette Map
	void afficheHighlighted(float r,float v,float b) override;			// Affiche l'objet géométrique en couleur unique

	bool Save(const std::string nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

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

	/** Find the MapObject in all sub-maps */
	MapObject* findMapObject(int id);

	/** Find the MapObject in the current Map instance */
	MapObject* getMapObject(int id);

	/** Find the MapObject in the current Map instance with the reference of the object in the Map */
	int getMapObjectIdByReference(const std::string& reference);
	MapObject* getMapObjectByReference(const std::string& reference);

	void add(CMap* map);										// Intègre tous les éléments d'une autre Map dans celle-ci
	void merge(int mapId, MapLogger* mapLogger = 0);			// Intègre tous les éléments d'une autre Map dans celle-ci

	void add(MapObject* object);					// Ajoute un GeoObject à la map
	void add(Dirigeable* dirigeable);
	void add(CPorte *porte);						// Ajoute une porte à la map
	void add(CNavette *navette);					// Ajoute une navette à la map
	void add(CMoteurParticules* engine);			// Ajoute un moteur de particules à la map
	void add(CLight *light);						// Ajoute une lumière à la map
	void add(CMaterial *mat);						// Ajoute un matériau à la map
	void add(EntryPoint* entryPoint);
	void add(CheckPlayerInZone* detector);

	// Geometrical
	void echangeXY() override;										// Echange les coordonnées X et Y des objets géo du map
	void echangeXZ() override;										// Echange les coordonnées X et Z des objets géo du map
	void echangeYZ() override;										// Echange les coordonnées Y et Z des objets géo du map
	void scale(float scaleX, float sclaeY, float scaleZ) override;	// Homothétie la Map (coordonnées multipliées par scale)
	void translate(float x, float y, float z) override;				// Translation de la Map selon x, y, z

	// Gestion des contacts
	bool checkContact(const float pos[3], const float dist) override;	// Indique s'il y a un triangle à une distance inférieure à 'dist' de la position 'pos'

	/**
	 * positionPlayer : position du jouer ou position calculé ce qui permet d'optimiser certains contacts quand un objet a bougé en considérant que c'est plutôt le
	 */
	void gereContactPlayer(float positionPlayer[3], CPlayer *player) override;			// Gère tous les contacts entre la map et les joueurs
	float gereLaserPlayer(float pos[3], jkt::CV3D &Dir, float dist) override;							// Envoie d'un laser sur la map

	std::map<int, CMap*>& getSubMaps();
	std::vector<CLight*>& getLights();
	std::vector<MapObject*>& getMapObjects();

	const std::string& getBinariesDirectory();

	// Points d'entrée des personnages joueurs
	std::vector<EntryPoint*>& getEntryPointsList();

	void listObjectProximite(std::vector<MapObject*>& contacts, const float pos[3], const float dist);
	void listObjectIntersectionsPave(std::vector<MapObject*>& intersections, const float position1[3], const float position2[3]);
};

}	// JktMoteur

#endif

