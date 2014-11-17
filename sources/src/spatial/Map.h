
#ifndef __JKT__MAP_H
#define __JKT__MAP_H

#include <vector>

using namespace std;

#include "util/Tableau.h"
#include "util/Erreur.h"
#include "util/V3D.h"

class CGame;
class CPlayer;

using namespace JktUtils;

namespace JktMoteur {
class CGeo;
class CLight;
class CMouve;
class CMaterial;
class CPorte;
class CNavette;
class CGeoObject;
class EntryPoint;

class CMap {
	string _name;							// Nom de la Map ("Monde" par exemple pour "Monde.map.xml")
	string _filename;						// Nom du fichier de la Map (par exemple "Monde.map.xml")
	string _binariesDirectory;				// Répertoires des binaires de la Map (textures, plugins, ...)
	vector<CGeo*> m_TabGeo;					// Liste des objets géométriques
	vector<CMouve*> m_TabMouve;				// Liste des objets nécessitant une actualisation (portes,...)
	vector<EntryPoint> _entryPoints;		// Liste des points d'entrée des joueurs sur la Map
	vector<CLight*> m_TabLight;				// Liste des lumières
	vector<string> _plugins;				// Liste des plugins de la Map

	bool Init() throw(JktUtils::CErreur);	// Initialisation de la CMap
	bool Lit(const string &nomFichier);
	static bool Lit(CMap& map, const string &mapName);

	int m_Selection;						// Object géo sélectionné
	bool m_bSelection;						// Indique si le mode sélection est actif ou non

	bool _isGlActivated;					// Indique si les éléments OpenGL de la MAP ont été initialisés
	bool _isPluginsActivated;				// Indique si les plugins de la MAP ont été initialisés

	void afficheMaterial(CMaterial* material, int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex, int& posX, int& posY, int& index);
public:
	vector<CMaterial*> m_TabMaterial;		// Liste des matériaux A VOIR : devrait être membre privé

		// Constructeurs / destructeur
	CMap();
	CMap(const string &nomFichier) throw(JktUtils::CErreur);
	~CMap();

		// Fonctions membres
	void initPlugins();	// Chargement ou exécution des plugins de la Map
	void freePlugins();
	bool initGL();		// Initialisation dans le contexte OpenGL
	bool freeGL();

//	bool SaveFichierMap(const string &nomFichier);		// Sauvegarde du CMap dans un fichier *.map
	bool Save(const string nomFichier);	// Sauvegarde du CMap dans un fichier *.map.xml

	void Affiche();						// Affiche l'ensemble des éléments 3D de cette Map
	void Refresh( CGame *game );		// Rafraichissement des classes listées dans m_TabMouve

	void add(CGeo *geo);			// Ajoute un GeoObject à la map
	void add(CMaterial *mat);		// Ajoute un matériau à la map
	void add(CLight *light);		// Ajoute une lumière à la map

	void add(CPorte *porte);		// Ajoute une porte à la map
	void add(CNavette *navette);	// Ajoute une navette à la map

	bool Contact(const float pos[3], const float dist);	// Indique s'il y a un triangle à une distance inférieure à 'dist' de la position 'pos'

	void EchangeXY();							// Echange les coordonnées X et Y des objets géo du map
	void EchangeXZ();							// Echange les coordonnées X et Z des objets géo du map
	void EchangeYZ();							// Echange les coordonnées Y et Z des objets géo du map
	void Scale(float scaleX, float sclaeY, float scaleZ);	// Homothétie la Map (coordonnées multipliées par scale)
	void translate(float x, float y, float z);				// Translation de la Map selon x, y, z
	/** Intègre tous les éléments d'une autre Map dans celle-ci. */
	void merge(CMap& map);
		// A VOIR Fonctions qui n'ont rien à faire ici
	void GereContactPlayer(CPlayer *player);								// Gère tous les contacts entre la map et les joueurs
	float GereLaserPlayer(float *pos, JktUtils::CV3D &Dir, float dist );	// Envoie d'un laser sur la map

	vector<CLight*>& getLights();

	/**
	 * Affiche toutes les textures de la MAP dans un rectangle dont le coin bas gauche est en (x,y) et les dimensions sont (tailleX, tailleY).
	 *
	 * @param x position horizontale du coin bas gauche du rectangle dans lequel les texture vont être affichées
	 * @param y position verticale du coin bas gauche du rectangle dans lequel les texture vont être affichées
	 * @param tailleX taille horizontale du rectangle dans lequel les texture vont être affichées
	 * @param tailleY taille verticale du rectangle dans lequel les texture vont être affichées
	 * @param nbrX nombre de textures sur une ligne
	 * @param nbrY nombre de textures sur une colonne
	 * @param firstIndex numéro de la première texture à afficher
	 */
	void afficheToutesTextures(int x, int y, int tailleX, int tailleY, int nbrX, int nbrY, int firstIndex);

		// Mode sélection
	void incrementeSelection();	// Sélectionne l'objet géo suivant
	void decrementeSelection();	// Sélectionne l'objet géo précédent
	void ChangeSelectionMode();
	const char* getSelectedName();
	bool IsSelectionMode();

	// Points d'entrée des personnages joueurs
	void add(EntryPoint entryPoint);
	vector<EntryPoint>& getEntryPointsList();
};

}	// JktMoteur

#endif

