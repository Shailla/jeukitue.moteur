/*
 * MapLoader.cpp
 *
 *  Created on: 18 avr. 2012
 *      Author: Erwin
 */

#include "game/MapLoader.h"

#include "SDL.h"

#include "main/Cfg.h"
#include "main/Fabrique.h"
#include "main/jkt.h"
#include "main/RequeteProcess.h"
#include "main/Game.h"
#include "ressource/RessourcesLoader.h"
#include "menu/ConsoleView.h"
#include "spatial/Map.h"
#include "spatial/contact.h"

using namespace JktMoteur;

extern CGame Game;
extern CCfg Config;		// Contient la configuration du jeu

MapLoader::MapLoader() {
}

MapLoader::~MapLoader() {
}

void MapLoader::launcheGameLoading(GameDto* gameDto) {
	SDL_Thread* thread = SDL_CreateThread(loadGameThread, (void*)gameDto); 	// Lance l'ouverture de la MAP dans un thread
}

int MapLoader::loadGameThread(void* gameDtoVar) {
	GameDto* gameDto = (GameDto*)gameDtoVar;

	ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));

	/**************************************
	* Lecture du fichier MAP
	**************************************/

	string mapName(gameDto->getMapName());

	console->println(string("Lecture de la MAP '").append(mapName).append("'...").c_str());
	cout << "\nLecture de la MAP '" << mapName << "'...";
	cout.flush();

	CMap* map = new CMap(mapName);
	gameDto->setMap(map);

	// Création joueurs
	gameDto->setPlayersMaxNumber(10);	// Indique que la partie peut contenir jusqu'à 10 joueurs


	/**************************************
	* Chargement de Map pour les joueurs
	**************************************/

	// Chargement premier skin joueur
	string mapJoueurPrincipal;
	mapJoueurPrincipal.append("@Joueur\\").append(Config.Joueur.mapName);

	console->println(string("Lecture du skin '").append(mapJoueurPrincipal).append("'...").c_str());
	cout << string("Lecture du skin '").append(mapJoueurPrincipal).append("'...").c_str();

	CMap *pMapJoueur = new CMap( mapJoueurPrincipal );
	pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
	console->println("Scaling du skin");
	cout << "Scaling du skin";
	pMapJoueur->Scale( -0.03f, 0.03f, 0.03f );

	// Chargement second skin joueur
	console->println("Lecture du skin 'GrosBonhomme'...");
	cout << "\nLecture du skin 'GrosBonhomme'...";

	CMap *pMapJoueur2 = new CMap( "GrosBonhomme" );
	pMapJoueur2->EchangeXZ();					// Ajuste les coordonnées
	console->println("Scaling du skin");
	cout << "Scaling du skin";
	pMapJoueur2->Scale( -0.10f, 0.10f, 0.10f );


	/**************************************
	* Chargement de sons pour les joueurs
	**************************************/

	// Récupération des ressources de cris des personnages
	console->println("Lecture du premier cri...");
	cout << "\nLecture du premier cri...";

	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);

	console->println("Lecture du second cri...");
	cout << "\nLecture du second cri...";

	string cri2 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri2);


	/***********************************
	* Création des joueurs dans la Map
	***********************************/

	// Création du joueur principal
	console->println("Creation du joueur principal...");
	cout << "\nCreation du joueur principal...";
	cout.flush();

	CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
	erwin->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravité
	erwin->changeContact(contactPlayer);		// Associe une fonction de gestion des contacts avec la map
	erwin->Skin( pMapJoueur );
	erwin->setCri( cri1.c_str() );				// Cri du joueur
	erwin->nom( "ERWIN" );
	erwin->init();								// Initialise certaines données
	gameDto->setErwin(erwin);

	// Création d'un second joueur
	console->println("Creation du second joueur...");
	cout << "\nCreation du second joueur...";
	cout.flush();

	CPlayer *julien;
	julien = new CPlayer();						// Crée un autre joueur
	julien->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravité
	julien->changeContact(contactPlayer);		// Associe une fonction pour les contacts avec la map
	julien->Skin( pMapJoueur2 );
	julien->setCri( cri1.c_str() );
	julien->nom( "JULIEN" );
	julien->init();
	gameDto->getPlayers().push_back(julien);				// Ajoute le joueur à la liste des joueurs

	// Création d'un troisième joueur
	console->println("Creation du troisieme joueur...");
	cout << "\nCreation du troisieme joueur...";
	cout.flush();

	CPlayer *sprite;
	sprite = new CPlayer();						// Crée un autre joueur
	sprite->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravité
	sprite->changeContact(contactSprite);		// Associe une fonction pour les contacts avec la map
	sprite->Skin( pMapJoueur );
	sprite->setCri( cri2.c_str() );
	sprite->nom( "SPRITE" );
	sprite->init();
	gameDto->getPlayers().push_back(sprite);				// Ajoute le joueur à la liste des joueurs

	Game.RequeteProcess.setOuvreMapLocaleEtape(CRequeteProcess::OMLE_OUVERTURE);

	console->println("Map lue");
	cout << "\nMap lue";
	cout.flush();

	return 0;
}
