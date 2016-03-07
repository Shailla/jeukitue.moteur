
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#include "util/Tableau.h"
#include "util/Erreur.h"
#include "util/V3D.h"
#include "spatial/geo/Geo.h"

class CGame;
class CPlayer;

using namespace JktUtils;

namespace JktMoteur {
class CLight;
class Dirigeable;
class CMouve;
class CMaterial;
class CPorte;
class CNavette;
class CGeoObject;
class EntryPoint;
class CMoteurParticules;

class CMap : CGeo {
	static const char* identifier;
	string tostring;

	string _filename;								// Nom du fichier de la Map (par exemple "Monde.map.xml")
	string _binariesDirectory;						// R�pertoires des binaires de la Map (textures, plugins, ...)

	map<unsigned int, CGeo*> _geoDescriptions;

	vector<CGeo*> _geos;							// Liste des objets g�om�triques
	vector<CMouve*> _mouves;						// Liste des objets n�cessitant une actualisation (portes,...)
	vector<Dirigeable*> _dirigeables;
	vector<CLight*> _lights;						// Liste des lumi�res
	vector<EntryPoint> _entryPoints;				// Liste des points d'entr�e des joueurs sur la Map

	vector<CMoteurParticules*> _particulesEngines;	// Liste des moteurs de particules de la Map

	vector<string> _plugins;						// Liste des plugins de la Map

	int _Selection;								// Object g�o s�lectionn�
	bool _bSelection;								// Indique si le mode s�lection est actif ou non

	bool _isGlActivated;							// Indique si les �l�ments OpenGL de la MAP ont �t� initialis�s
	bool _isPluginsActivated;						// Indique si les plugins de la MAP ont �t� initialis�s

	void afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex, int& posX, int& posY, int& index);
public:
	vector<CMaterial*> m_TabMaterial;		// Liste des mat�riaux A VOIR : devrait �tre membre priv�

		// Constructeurs / destructeur
	CMap(CMap* parent);
	CMap(CMap* parent, const string& nomFichier) throw(JktUtils::CErreur);	// Construction de la Map par lecture d'un fichier *.map.xml
	~CMap();
	CGeo* clone();

	const char* toString();						// Description r�sum�e de l'objet

	// S�rialisation
	static bool Lit(CMap& map, const string &mapName);
	bool Lit(const string &nomFichier);
	bool Lit(TiXmlElement* el);
	bool Save(TiXmlElement* element);			// Sauve l'objet g�o dans un fichier Map

	void Init() throw(JktUtils::CErreur);	// Initialisation de la CMap

	// Gestion des plugins de la Map
	void initPlugins();		// Chargement / ex�cution des plugins de la Map
	void freePlugins();		// Lib�ration des plugins de la Map

	// Gestion de objets OpenGL de la Map
	void initGL();		// Initialisation du contexte OpenGL
	void freeGL();		// Lib�ration du contexte OpenGL

	bool Save(const string nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

	// Affichage
	void Affiche();											// Affiche l'ensemble des �l�ments 3D de cette Map
	void AfficheSelection(float r,float v,float b);			// Affiche l'objet g�om�trique en couleur unique
	void Refresh( CGame *game );							// Rafraichissement des classes list�es dans m_TabMouve

	/**
	 * Affiche toutes les textures de la MAP dans un rectangle dont le coin bas gauche est en (x,y) et les dimensions sont (tailleX, tailleY).
	 *
	 * @param x position horizontale du coin bas gauche du rectangle dans lequel les texture vont �tre affich�es
	 * @param y position verticale du coin bas gauche du rectangle dans lequel les texture vont �tre affich�es
	 * @param tailleX taille horizontale du rectangle dans lequel les texture vont �tre affich�es
	 * @param tailleY taille verticale du rectangle dans lequel les texture vont �tre affich�es
	 * @param nbrX nombre de textures sur une ligne
	 * @param nbrY nombre de textures sur une colonne
	 * @param firstIndex num�ro de la premi�re texture � afficher
	 */
	void afficheToutesTextures(int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex);

	void addDescription(unsigned int ref, CGeo* geo);
	CGeo* getDescription(unsigned int ref);

	void add(Dirigeable* dirigeable);
	void add(CGeo *geo);					// Ajoute un GeoObject � la map
	void add(CMaterial *mat);				// Ajoute un mat�riau � la map
	void add(CLight *light);				// Ajoute une lumi�re � la map
	void add(CPorte *porte);				// Ajoute une porte � la map
	void add(CNavette *navette);			// Ajoute une navette � la map
	void add(CMoteurParticules* engine);	// Ajoute un moteur de particules � la map

	// Transformations
	void EchangeXY();										// Echange les coordonn�es X et Y des objets g�o du map
	void EchangeXZ();										// Echange les coordonn�es X et Z des objets g�o du map
	void EchangeYZ();										// Echange les coordonn�es Y et Z des objets g�o du map
	void Scale(float scaleX, float sclaeY, float scaleZ);	// Homoth�tie la Map (coordonn�es multipli�es par scale)
	void translate(float x, float y, float z);				// Translation de la Map selon x, y, z
	/** Int�gre tous les �l�ments d'une autre Map dans celle-ci. */
	void merge(CMap& map);

	// Gestion des contacts
	bool Contact(const float pos[3], const float dist);	// Indique s'il y a un triangle � une distance inf�rieure � 'dist' de la position 'pos'

	/**
	 * positionPlayer : position du jouer ou position calcul� ce qui permet d'optimiser certains contacts quand un objet a boug� en consid�rant que c'est plut�t le
	 */
	void GereContactPlayer(float positionPlayer[3], CPlayer *player);								// G�re tous les contacts entre la map et les joueurs
	float GereLaserPlayer(float pos[3], JktUtils::CV3D &Dir, float dist);	// Envoie d'un laser sur la map

	vector<CLight*>& getLights();

	// S�lection
	void incrementeSelection();	// S�lectionne l'objet g�o suivant
	void decrementeSelection();	// S�lectionne l'objet g�o pr�c�dent
	void ChangeSelectionMode();
	const char* getSelectedName();
	bool IsSelectionMode();

	// Points d'entr�e des personnages joueurs
	void add(EntryPoint entryPoint);
	vector<EntryPoint>& getEntryPointsList();
};

}	// JktMoteur

#endif

