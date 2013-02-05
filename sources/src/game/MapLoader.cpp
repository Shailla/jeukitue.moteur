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
	SDL_CreateThread(loadGameThread, (void*)gameDto); 	// Lance l'ouverture de la MAP dans un thread
}

int MapLoader::loadGameThread(void* gameDtoVar) {
	GameDto* gameDto = (GameDto*)gameDtoVar;

	ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));

	/**************************************
	* Lecture du fichier MAP
	**************************************/

	string mapName(gameDto->getMapName());

	console->println(string("Lecture de la MAP '").append(mapName).append("'...").c_str());
	cout << endl << "Lecture de la MAP '" << mapName << "'..." << flush;

	CMap* map = new CMap(mapName);
	gameDto->setMap(map);

	// Création joueurs
	gameDto->setPlayersMaxNumber(10);	// Indique que la partie peut contenir jusqu'à 10 joueurs


	/**************************************
	* Chargement de sons pour les joueurs
	**************************************/

	// Récupération des ressources de cris des personnages
	console->println("Lecture du premier cri...");
	cout << endl << "Lecture du premier cri...";

	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);

	console->println("Lecture du second cri...");
	cout << endl << "Lecture du second cri...";

	string cri2 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri2);


	/***************************************
	 * Création du joueur principal (Erwin)
	 ***************************************/
	{
		// Chargement du skin
		string mapJoueur;
		mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

		console->println(string("Lecture du skin '").append(mapJoueur).append("'...").c_str());
		cout << endl << "Lecture du skin '" << mapJoueur << "'...";

		CMap *pMapJoueur = new CMap(mapJoueur);
		pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
		console->println("Scaling du skin");
		cout << endl << "Scaling du skin";
		pMapJoueur->Scale( -0.03f, 0.03f, 0.03f );

		// Création du joueur
		console->println("Creation du joueur principal...");
		cout << endl << "Creation du joueur principal..." << flush;

		CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
		erwin->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravité
		erwin->changeContact(contactPlayer);		// Associe une fonction de gestion des contacts avec la map
		erwin->Skin(pMapJoueur);
		erwin->setCri( cri1.c_str() );				// Cri du joueur
		erwin->nom("ERWIN");
		erwin->init();								// Initialise certaines données
		gameDto->setErwin(erwin);
	}


	/***************************************
	 * Création du second joueur (Julien)
	 ***************************************/
	{
		// Chargement du skin
		console->println("Lecture du skin 'GrosBonhomme'...");
		cout << "\nLecture du skin 'GrosBonhomme'...";

		CMap *pMapJoueurJulien = new CMap("GrosBonhomme");
		pMapJoueurJulien->EchangeXZ();					// Ajuste les coordonnées
		console->println("Scaling du skin");
		cout << endl << "Scaling du skin";
		pMapJoueurJulien->Scale( -0.10f, 0.10f, 0.10f );

		// Création d'un second joueur
		console->println("Creation du second joueur...");
		cout << endl << "Creation du second joueur..." << flush;

		CPlayer *julien;
		julien = new CPlayer();						// Crée un autre joueur
		julien->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravité
		julien->changeContact(contactPlayer);		// Associe une fonction pour les contacts avec la map
		julien->Skin( pMapJoueurJulien );
		julien->setCri( cri1.c_str() );
		julien->nom( "JULIEN" );
		julien->init();
		gameDto->getPlayers().push_back(julien);				// Ajoute le joueur à la liste des joueurs
	}


	/***************************************
	 * Création du troisième joueur (Sprite)
	 ***************************************/
	{
		// Chargement du skin
		string mapJoueur;
		mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

		console->println(string("Lecture du skin '").append(mapJoueur).append("'...").c_str());
		cout << endl << "Lecture du skin '" << mapJoueur << "'...";

		CMap *pMapJoueur = new CMap(mapJoueur);
		pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
		console->println("Scaling du skin");
		cout << endl << "Scaling du skin";
		pMapJoueur->Scale( -0.03f, 0.03f, 0.03f );

		// Création d'un troisième joueur
		console->println("Creation du troisieme joueur...");
		cout << endl << "Creation du troisieme joueur..." << flush;

		CPlayer *sprite;
		sprite = new CPlayer();						// Crée un autre joueur
		sprite->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravité
		sprite->changeContact(contactSprite);		// Associe une fonction pour les contacts avec la map
		sprite->Skin( pMapJoueur );
		sprite->setCri( cri2.c_str() );
		sprite->nom( "SPRITE" );
		sprite->init();
		gameDto->getPlayers().push_back(sprite);				// Ajoute le joueur à la liste des joueurs
	}

	console->println("Map lue");
	cout << endl << "Map lue" << flush;

	Game.RequeteProcess.setOuvreMapLocaleEtape(CRequeteProcess::OMLE_OUVERTURE);

	return 0;
}



void MapLoader::launcheGameServerLoading(GameDto* gameDto) {
	SDL_CreateThread(loadGameServerThread, (void*)gameDto); 	// Lance l'ouverture de la MAP dans un thread
}

int MapLoader::loadGameServerThread(void* gameDtoVar) {
	GameDto* gameDto = (GameDto*)gameDtoVar;

	ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));

	/**************************************
	* Lecture du fichier MAP
	**************************************/

	string mapName(gameDto->getMapName());

	console->println(string("Lecture de la MAP '").append(mapName).append("'...").c_str());
	cout << endl << "Lecture de la MAP '" << mapName << "'..." << flush;

	CMap* map = new CMap(mapName);
	gameDto->setMap(map);

	// Création joueurs
	gameDto->setPlayersMaxNumber(10);	// Indique que la partie peut contenir jusqu'à 10 joueurs


	/**************************************
	* Chargement de sons pour les joueurs
	**************************************/

	// Récupération des ressources de cris des personnages
	console->println("Lecture du cri...");
	cout << endl << "Lecture du cri...";

	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);


	/***************************************
	 * Création du joueur principal (Erwin)
	 ***************************************/
	{
		// Chargement du skin
		string mapJoueur;
		mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

		console->println(string("Lecture du skin '").append(mapJoueur).append("'...").c_str());
		cout << endl << "Lecture du skin '" << mapJoueur << "'...";

		CMap *pMapJoueur = new CMap(mapJoueur);
		pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
		console->println("Scaling du skin");
		cout << endl << "Scaling du skin";
		pMapJoueur->Scale( -0.03f, 0.03f, 0.03f );

		// Création du joueur
		console->println("Creation du joueur principal...");
		cout << endl << "Creation du joueur principal..." << flush;

		CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
		erwin->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravité
		erwin->changeContact(contactPlayer);		// Associe une fonction de gestion des contacts avec la map
		erwin->Skin(pMapJoueur);
		erwin->setCri( cri1.c_str() );				// Cri du joueur
		erwin->nom("ERWIN");
		erwin->init();								// Initialise certaines données
		gameDto->setErwin(erwin);
	}

	console->println("Map lue");
	cout << endl << "Map lue" << flush;

	Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_OUVERTURE);

	return 0;
}
