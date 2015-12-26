/*
 * MapLoader.cpp
 *
 *  Created on: 18 avr. 2012
 *      Author: Erwin
 */

#include "game/MapLoader.h"

#include "SDL.h"

#include "util/Trace.h"
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

void MapLoader::launchLocalGameLoading(GameDto* gameDto) {
	SDL_CreateThread(loadLocalGameThread, (void*)gameDto); 	// Lance l'ouverture de la MAP dans un thread
}

int MapLoader::loadLocalGameThread(void* gameDtoVar) {
	GameDto* gameDto = (GameDto*)gameDtoVar;

	ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));

	/**************************************
	 * Lecture du fichier MAP
	 **************************************/

	string mapName(gameDto->getMapName());

	console->println(ConsoleView::COT_INFO, string("Lecture de la MAP '").append(mapName).append("'..."));
	LOGINFO(("Lecture de la MAP '%s'...", mapName.c_str()));

	CMap* map = new CMap(0, mapName);
	gameDto->setMap(map);

	// Création joueurs
	gameDto->setPlayersMaxNumber(20);	// Indique que la partie peut contenir jusqu'à 20 joueurs


	/**************************************
	 * Chargement de sons pour les joueurs
	 **************************************/

	// Récupération des ressources de cris des personnages
	console->println(ConsoleView::COT_INFO, "Lecture du premier cri...");
	LOGINFO(("Lecture du premier cri..."));

	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);

	console->println(ConsoleView::COT_INFO, "Lecture du second cri...");
	LOGINFO(("Lecture du second cri..."));

	string cri2 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri2);


	/***************************************
	 * Création du joueur principal (Erwin)
	 ***************************************/
	{
		// Chargement du skin
		string mapJoueur;
		mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

		console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'..."));
		LOGINFO(("Lecture du skin '%s'...", mapJoueur.c_str()));

		CMap *pMapJoueur = new CMap(0, mapJoueur);
		pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
		console->println(ConsoleView::COT_INFO, "Scaling du skin");
		LOGINFO(("Scaling du skin"));
		pMapJoueur->Scale(-0.6f, 0.6f, 0.6f);

		// Création du joueur
		console->println(ConsoleView::COT_INFO, "Creation du joueur principal...");
		LOGINFO(("Creation du joueur principal..."));

		CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
		erwin->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravité
		erwin->changeContact(contactPlayer);		// Associe une fonction de gestion des contacts avec la map
		erwin->Skin(pMapJoueur);
		erwin->setCri( cri1.c_str() );				// Cri du joueur
		erwin->nom("Erwin");
		erwin->init();								// Initialise certaines données
		gameDto->setErwin(erwin);
	}


	/***************************************
	 * Création du second joueur (Julien)
	 ***************************************/
	{
		// Chargement du skin
		console->println(ConsoleView::COT_INFO, "Lecture du skin 'JoueurTex'...");
		LOGINFO(("Lecture du skin 'JoueurTex'..."));

		CMap *pMapJoueurJulien = new CMap(0, "@Joueur\\joueurTex");
		pMapJoueurJulien->EchangeXZ();					// Ajuste les coordonnées
		console->println(ConsoleView::COT_INFO, "Scaling du skin");
		LOGINFO(("Scaling du skin"));
		pMapJoueurJulien->Scale(-0.6f, 0.6f, 0.6f);

		// Création d'un second joueur
		console->println(ConsoleView::COT_INFO, "Creation du second joueur...");
		LOGINFO(("Creation du second joueur..."));

		CPlayer *julien;
		julien = new CPlayer();						// Crée un autre joueur
		julien->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravité
		julien->changeContact(contactPlayer);		// Associe une fonction pour les contacts avec la map
		julien->Skin( pMapJoueurJulien );
		julien->setCri( cri1.c_str() );
		julien->nom( "Julien" );
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

		console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'..."));
		LOGINFO(("Lecture du skin '%s'", mapJoueur.c_str()));

		CMap *pMapJoueur = new CMap(0, mapJoueur);
		pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
		console->println(ConsoleView::COT_INFO, "Scaling du skin");
		LOGINFO(("Scaling du skin"));
		pMapJoueur->Scale(-0.6f, 0.6f, 0.6f);

		// Création d'un troisième joueur
		console->println(ConsoleView::COT_INFO, "Creation du troisieme joueur...");
		LOGINFO(("Creation du troisieme joueur..."));

		CPlayer *sprite;
		sprite = new CPlayer();						// Crée un autre joueur
		sprite->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravité
		sprite->changeContact(contactSprite);		// Associe une fonction pour les contacts avec la map
		sprite->Skin( pMapJoueur );
		sprite->setCri( cri2.c_str() );
		sprite->nom( "Sprite" );
		sprite->init();
		gameDto->getPlayers().push_back(sprite);				// Ajoute le joueur à la liste des joueurs
	}

	console->println(ConsoleView::COT_INFO, "Map lue");
	LOGINFO(("Map lue"));

	Game.RequeteProcess.setOuvreMapLocaleEtape(CRequeteProcess::OMLE_OUVERTURE);

	return 0;
}



void MapLoader::launchClientGameLoading(GameDto* gameDto) {
	SDL_CreateThread(loadClientGameThread, (void*)gameDto); 	// Lance l'ouverture de la MAP dans un thread
}

int MapLoader::loadClientGameThread(void* gameDtoVar) {
	GameDto* gameDto = (GameDto*)gameDtoVar;

	ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));

	/**************************************
	 * Lecture du fichier MAP
	 **************************************/

	string mapName(gameDto->getMapName());

	console->println(ConsoleView::COT_INFO, string("Lecture de la MAP '").append(mapName).append("'..."));
	LOGINFO(("Lecture de la MAP '%s'", mapName.c_str()));

	CMap* map = new CMap(0, mapName);
	gameDto->setMap(map);


	/***************************************************
	 * Chargement des ressources pour les joueurs
	 ****************************************************/

	// Cris des personnages
	console->println(ConsoleView::COT_INFO, "Lecture du cri...");
	LOGINFO(("Lecture du cri..."));

	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);

	// Chargement du skin
	string mapJoueur;
	mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

	console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'...").c_str());
	LOGINFO(("Lecture du skin '%s'", mapJoueur.c_str()));

	CMap *pMapJoueur = new CMap(0, mapJoueur);
	pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
	console->println(ConsoleView::COT_INFO, "Scaling du skin");
	LOGINFO(("Scaling du skin"));
	pMapJoueur->Scale(-0.6f, 0.6f, 0.6f);


	/***************************************
	 * Initialisation des joueurs
	 ***************************************/

	// Initialisation des joueurs
	console->println(ConsoleView::COT_INFO, "Initialisation des joueurs...");
	LOGINFO(("Initialisation des joueurs..."));

	int curseur = -1;
	CPlayer* player;

	while((player = Game.nextPlayer(curseur))) {
		player->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravité
		player->changeContact(contactPlayer);			// Associe une fonction de gestion des contacts avec la map
		player->Skin(pMapJoueur);
		player->setCri( cri1.c_str() );					// Cri du joueur
		player->init();
	}

	console->println(ConsoleView::COT_INFO, "Partie chargee");
	LOGINFO(("Partie chargée"));

	Game.RequeteProcess.setOuvreMapClientEtape(CRequeteProcess::OMCE_OUVERTURE);

	return 0;
}



void MapLoader::launchServerGameLoading(GameDto* gameDto) {
	SDL_CreateThread(loadServerGameThread, (void*)gameDto); 	// Lance l'ouverture de la MAP dans un thread
}

int MapLoader::loadServerGameThread(void* gameDtoVar) {
	GameDto* gameDto = (GameDto*)gameDtoVar;

	ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));

	/**************************************
	 * Lecture du fichier MAP
	 **************************************/

	string mapName(gameDto->getMapName());

	console->println(ConsoleView::COT_INFO, string("Lecture de la MAP '").append(mapName).append("'..."));
	LOGINFO(("Lecture de la MAP '%s'", mapName.c_str()));

	CMap* map = new CMap(0, mapName);
	gameDto->setMap(map);

	// Création joueurs
	gameDto->setPlayersMaxNumber(10);	// Indique que la partie peut contenir jusqu'à 10 joueurs


	/**************************************
	 * Chargement de sons pour les joueurs
	 **************************************/

	// Récupération des ressources de cris des personnages
	console->println(ConsoleView::COT_INFO, "Lecture du cri...");
	LOGINFO(("Lecture du cri..."));

	string cri1 = "@Bruit\\cri_1.wav";
	JktUtils::RessourcesLoader::getFileRessource(cri1);


	/***************************************
	 * Création du joueur principal (Erwin)
	 ***************************************/
	{
		// Chargement du skin
		string mapJoueur;
		mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

		console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'...").c_str());
		LOGINFO(("Lecture du skin '%s'", mapJoueur.c_str()));

		CMap *pMapJoueur = new CMap(0, mapJoueur);
		pMapJoueur->EchangeXZ();					// Ajuste les coordonnées
		console->println(ConsoleView::COT_INFO, "Scaling du skin");
		LOGINFO(("Scaling du skin"));
		pMapJoueur->Scale(-0.6f, 0.6f, 0.6f);

		// Création du joueur
		console->println(ConsoleView::COT_INFO, "Creation du joueur principal...");
		LOGINFO(("Creation du joueur principal..."));

		CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
		erwin->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravité
		erwin->changeContact(contactPlayer);		// Associe une fonction de gestion des contacts avec la map
		erwin->Skin(pMapJoueur);
		erwin->setCri( cri1.c_str() );				// Cri du joueur
		erwin->nom("Erwin");
		erwin->init();								// Initialise certaines données

		gameDto->setErwin(erwin);
	}

	console->println(ConsoleView::COT_INFO, "Map lue");
	LOGINFO(("Map lue"));

	Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_OUVERTURE);

	return 0;
}
