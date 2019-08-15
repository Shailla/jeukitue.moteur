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

using namespace jkt;
using namespace std;

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

	CMap* currentMap;

	try {
		currentMap = new CMap(0);
		currentMap->Lit(mapName, 0);
		gameDto->setMap(currentMap);
		currentMap->init();
	}
	catch(CErreur& error) {
		stringstream msg;
		msg << "Echec de lecture de la Map : " << error.what();
		LOGWARN((msg));
		console->println(ConsoleView::COT_ERROR, msg);
		delete currentMap;
		currentMap = 0;
		Game.RequeteProcess.setOuvreMapLocaleEtape(CRequeteProcess::OMLE_AUCUNE);
		return 0;
	}

	// Création joueurs
	gameDto->setPlayersMaxNumber(20);	// Indique que la partie peut contenir jusqu'� 20 joueurs


	/**************************************
	 * Chargement de sons pour les joueurs
	 **************************************/

	// Récupération des ressources de cris des personnages
	console->println(ConsoleView::COT_INFO, "Lecture du premier cri...");
	LOGINFO(("Lecture du premier cri..."));

	string cri1 = "@Bruit\\cri_1.wav";
	jkt::RessourcesLoader::getFileRessource(cri1);

	console->println(ConsoleView::COT_INFO, "Lecture du second cri...");
	LOGINFO(("Lecture du second cri..."));

	string cri2 = "@Bruit\\cri_1.wav";
	jkt::RessourcesLoader::getFileRessource(cri2);


	/***************************************
	 * Création du joueur principal (Erwin)
	 ***************************************/
	{
		// Chargement du skin
		string mapJoueur;
		mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

		console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'..."));
		LOGINFO(("Lecture du skin '%s'...", mapJoueur.c_str()));

		CMap *pMapJoueur;

		try {
			pMapJoueur = new CMap(0);
			pMapJoueur->Lit(mapJoueur, 0);
			pMapJoueur->echangeXZ();			// Ajuste les coordonnées
			console->println(ConsoleView::COT_INFO, "Scaling du skin");
			LOGINFO(("Scaling du skin"));
			pMapJoueur->scale(-0.6f, 0.6f, 0.6f);
		}
		catch(CErreur& error) {
			stringstream msg;
			msg << "Erreur de lecture du skin du joueur principal : " << error.what();
			LOGWARN((msg));
			console->println(ConsoleView::COT_ERROR, msg.str());
			delete pMapJoueur;

			pMapJoueur = 0;
		}

		// Création du joueur
		console->println(ConsoleView::COT_INFO, "Cr\u00e9ation du joueur principal...");
		LOGINFO(("Creation du joueur principal..."));

		CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui géré par le clavier et l'écran)
		erwin->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravité
		erwin->changeContact(contactPlayer);		// Associe une fonction de gestion des contacts avec la map
		erwin->Skin(pMapJoueur);
		erwin->setCri( cri1.c_str() );				// Cri du joueur
		erwin->setName("Erwin");
		erwin->init();								// Initialise certaines données
		gameDto->setErwin(erwin);
	}


	/***************************************
	 * Création du second joueur (Julien)
	 ***************************************/
	{
		// Chargement du skin
//		string mapJoueur = "@Joueur\\GrosBonhomme";
		string mapJoueur = "@Joueur\\JoueurTex";

		console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'..."));
		LOGINFO(("Lecture du skin '%s'", mapJoueur.c_str()));

		CMap *pMapJoueur;

		try {
			pMapJoueur = new CMap(0);
			pMapJoueur->Lit(mapJoueur, 0);
			pMapJoueur->echangeXZ();			// Ajuste les coordonnées
			console->println(ConsoleView::COT_INFO, "Scaling du skin");
			LOGINFO(("Scaling du skin"));
			pMapJoueur->scale(-0.6f, 0.6f, 0.6f);
		}
		catch(CErreur& error) {
			stringstream msg;
			msg << "Erreur de lecture du skin du joueur principal : " << error.what();
			LOGWARN((msg));
			console->println(ConsoleView::COT_ERROR, msg.str());
			delete pMapJoueur;

			pMapJoueur = 0;
		}

		// Création d'un second joueur
		console->println(ConsoleView::COT_INFO, "Cr\u00e9ation du second joueur...");
		LOGINFO(("Création du second joueur..."));

		CPlayer *julien;
		julien = new CPlayer();						// Crée un autre joueur
		julien->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravité
		julien->changeContact(contactPlayer);		// Associe une fonction pour les contacts avec la map
		julien->Skin( pMapJoueur );
		julien->setCri( cri1.c_str() );
		julien->setName( "Julien" );
		julien->init();
		gameDto->getPlayers().push_back(julien);				// Ajoute le joueur à la liste des joueurs
	}


	/***************************************
	 * Création du troisième joueur (Sprite)
	 ***************************************/
	{
		// Chargement du skin
//		string mapJoueur = "@Joueur\\GrosBonhomme";
		string mapJoueur = "@Joueur\\JoueurTex";

		console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'..."));
		LOGINFO(("Lecture du skin '%s'", mapJoueur.c_str()));

		CMap *pMapJoueur;

		try {
			pMapJoueur = new CMap(0);
			pMapJoueur->Lit(mapJoueur, 0);
			pMapJoueur->echangeXZ();			// Ajuste les coordonnées
			console->println(ConsoleView::COT_INFO, "Scaling du skin");
			LOGINFO(("Scaling du skin"));
			pMapJoueur->scale(-0.6f, 0.6f, 0.6f);
		}
		catch(CErreur& error) {
			stringstream msg;
			msg << "Erreur de lecture du skin du joueur principal : " << error.what();
			LOGWARN((msg));
			console->println(ConsoleView::COT_ERROR, msg.str());
			delete pMapJoueur;

			pMapJoueur = 0;
		}

		// Création d'un troisième joueur
		console->println(ConsoleView::COT_INFO, "Cr\u00e9ation du troisieme joueur...");
		LOGINFO(("Creation du troisieme joueur..."));

		CPlayer *sprite;
		sprite = new CPlayer();						// Crée un autre joueur
		sprite->changeAction(gravitePlayer);		// Associe au joueur une fonction de gravit�
		sprite->changeContact(contactSprite);		// Associe une fonction pour les contacts avec la map
		sprite->Skin( pMapJoueur );
		sprite->setCri( cri2.c_str() );
		sprite->setName( "Sprite" );
		sprite->init();
		gameDto->getPlayers().push_back(sprite);				// Ajoute le joueur � la liste des joueurs
	}

	console->println(ConsoleView::COT_COMMAND_RESULT, "Map lue.");
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

	CMap* currentMap;

	try {
		currentMap = new CMap(0);
		currentMap->Lit(mapName, 0);
		gameDto->setMap(currentMap);
		currentMap->init();
	}
	catch(CErreur& error) {
		stringstream msg;
		msg << "Echec de lecture de la Map : " << error.what();
		LOGWARN((msg));
		console->println(ConsoleView::COT_ERROR, msg);
		delete currentMap;
		currentMap = 0;
		Game.RequeteProcess.setOuvreMapClientEtape(CRequeteProcess::OMCE_AUCUNE);
		return 0;
	}


	/***************************************************
	 * Chargement des ressources pour les joueurs
	 ****************************************************/

	// Cris des personnages
	console->println(ConsoleView::COT_INFO, "Lecture du cri...");
	LOGINFO(("Lecture du cri..."));

	string cri1 = "@Bruit\\cri_1.wav";
	jkt::RessourcesLoader::getFileRessource(cri1);

	// Chargement du skin
	string mapJoueur;
	mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

	console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'...").c_str());
	LOGINFO(("Lecture du skin '%s'", mapJoueur.c_str()));

	CMap *pMapJoueur;

	try {
		pMapJoueur = new CMap(0);
		pMapJoueur->Lit("@Joueur\\joueurTex", 0);
		pMapJoueur->echangeXZ();			// Ajuste les coordonnées
		console->println(ConsoleView::COT_INFO, "Scaling du skin");
		LOGINFO(("Scaling du skin"));
		pMapJoueur->scale(-0.6f, 0.6f, 0.6f);
	}
	catch(CErreur& error) {
		stringstream msg;
		msg << "Erreur de lecture du skin du joueur principal : " << error.what();
		LOGWARN((msg));
		console->println(ConsoleView::COT_ERROR, msg);
		delete pMapJoueur;

		pMapJoueur = 0;
	}


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

	console->println(ConsoleView::COT_INFO, "Partie charg\u00e9e");
	LOGINFO(("Partie charg�e"));

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

	CMap* currentMap;

	try {
		currentMap = new CMap(0);
		currentMap->Lit(mapName, 0);
		gameDto->setMap(currentMap);
		currentMap->init();
	}
	catch(CErreur& error) {
		stringstream msg;
		msg << "Echec de lecture de la Map : " << error.what();
		LOGWARN((msg));
		console->println(ConsoleView::COT_ERROR, msg);
		delete currentMap;
		currentMap = 0;
		Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_AUCUNE);
		return 0;
	}

	// Création joueurs
	gameDto->setPlayersMaxNumber(10);	// Indique que la partie peut contenir jusqu'� 10 joueurs


	/**************************************
	 * Chargement de sons pour les joueurs
	 **************************************/

	// Récupération des ressources de cris des personnages
	console->println(ConsoleView::COT_INFO, "Lecture du cri...");
	LOGINFO(("Lecture du cri..."));

	string cri1 = "@Bruit\\cri_1.wav";
	jkt::RessourcesLoader::getFileRessource(cri1);


	/***************************************
	 * Création du joueur principal (Erwin)
	 ***************************************/
	{
		// Chargement du skin
		string mapJoueur;
		mapJoueur.append("@Joueur\\").append(Config.Joueur.mapName);

		console->println(ConsoleView::COT_INFO, string("Lecture du skin '").append(mapJoueur).append("'...").c_str());
		LOGINFO(("Lecture du skin '%s'", mapJoueur.c_str()));

		CMap *pMapJoueur;

		try {
			pMapJoueur = new CMap(0);
			pMapJoueur->Lit(mapJoueur, 0);
			pMapJoueur->echangeXZ();			// Ajuste les coordonnées
			console->println(ConsoleView::COT_INFO, "Scaling du skin");
			LOGINFO(("Scaling du skin"));
			pMapJoueur->scale(-0.6f, 0.6f, 0.6f);
		}
		catch(CErreur& error) {
			stringstream msg;
			msg << "Erreur de lecture du skin du joueur principal : " << error.what();
			LOGWARN((msg));
			console->println(ConsoleView::COT_ERROR, msg);
			delete pMapJoueur;

			pMapJoueur = 0;
		}

		// Création du joueur
		console->println(ConsoleView::COT_INFO, "Cr\u00e9ation du joueur principal...");
		LOGINFO(("Creation du joueur principal..."));

		CPlayer *erwin = new CPlayer();				// Crée le joueur principal (celui g�r� par le clavier et l'�cran)
		erwin->changeAction(gravitePlayer);			// Associe au joueur une fonction de gravit�
		erwin->changeContact(contactPlayer);		// Associe une fonction de gestion des contacts avec la map
		erwin->Skin(pMapJoueur);
		erwin->setCri( cri1.c_str() );				// Cri du joueur
		erwin->setName("Erwin");
		erwin->init();								// Initialise certaines donn�es

		gameDto->setErwin(erwin);
	}

	console->println(ConsoleView::COT_INFO, "Map lue");
	LOGINFO(("Map lue"));

	Game.RequeteProcess.setOuvreMapServerEtape(CRequeteProcess::OMSE_OUVERTURE);

	return 0;
}
