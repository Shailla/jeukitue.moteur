
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

using namespace std;

class CGame;

#include "util/Erreur.h"
#include "util/V3D.h"
#include "main/Fabrique.h"
#include "util/Tableau.cpp"
#include "util/TableauIndex.cpp"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/RequeteProcess.h"
#include "spatial/Map.h"
#include "spatial/objet/Dirigeable.h"
#include "reseau/NetworkManager.h"
#include "util/Trace.h"
#include "main/Clavier.h"

#include "reseau/enumReseau.h"
#include "main/Game.h"

using namespace jkt;

extern NetworkManager* _networkManager;

#define Pi 3.14159265f			//nombre pi pour les calculs

CGame::CGame() : _players(false) {
	_mode = JKT_MODE_PARTIE_NULL;
	_localDataTree = NULL;
	_serverDataTree = NULL;
	_clientDataTree = NULL;
	_map = NULL;			// Pas de map
	_erwin = NULL;			// Pas de joueur actif
	_gravite = true;		// Par défaut la gravité est active
}

bool CGame::isModeServer() {
	return _mode == JKT_MODE_PARTIE_SERVER;
}

bool CGame::isModeClient() {
	return _mode == JKT_MODE_PARTIE_CLIENT;
}

bool CGame::isModeLocal() {
	return _mode == JKT_MODE_PARTIE_LOCAL;
}

bool CGame::isModeNull() {
	return _mode == JKT_MODE_PARTIE_NULL;
}

void CGame::setStatutClient( jkt::StatutClient statut ) {
	_networkManager->getClient()->setStatut( statut );
}

jkt::StatutClient CGame::getStatutClient() {
	jkt::StatutClient statut;
	if( _mode==JKT_MODE_PARTIE_CLIENT )
		statut = _networkManager->getClient()->getStatut();
	else
		statut = JKT_STATUT_CLIENT_NULL;

	return statut;
}

int CGame::getMaxPlayers() const {
	return _players.getMax();
}

int CGame::getNbrPlayers() const {
	return _players.getNbr();
}

void CGame::createPlayerList(int size) {
	_players.reinit(size);

	LOGINFO(("Set player list in game :"));

	int curseur = -1;

	while(_players.Suivant(curseur)) {
		CPlayer* player = _players[curseur];
		LOGINFO(("ID=%d : '%s'", curseur, player->getName()));
	}
}

CPlayer* CGame::nextPlayer(int& pos) {
	if(_players.Suivant(pos)) {
		return _players[pos];
	}
	else {
		return 0;
	}
}

bool CGame::openMap(const string &nomFichierMap) throw(jkt::CErreur) {
	LOGDEBUG(("Opening Map '%s'", nomFichierMap.c_str()));

	bool result = true;

	if( _map )
		delete _map;

	try {
		_map = new jkt::CMap(0, nomFichierMap);
	}
	catch(jkt::CErreur& erreur) {
		LOGERROR(("Echec d'ouverture de la Map '%s' : %s", nomFichierMap.c_str(), erreur.what()));
		result = false;
	}

	LOGDEBUG(("Opening Map '%s'", nomFichierMap.c_str()));
	return result;
}

void CGame::changeActiveMap(jkt::CMap* map) {
	if( _map )
		delete _map;

	_map = map;
}

void CGame::setStatutServer(jkt::StatutServer statut) {
	if( _mode==JKT_MODE_PARTIE_SERVER )
		_networkManager->getServer()->setStatut( statut );
}

jkt::StatutServer CGame::getStatutServer() {
	if( _mode==JKT_MODE_PARTIE_SERVER )
		return _networkManager->getServer()->getStatut();
	else
		return JKT_STATUT_SERVER_NULL;
}

void CGame::setLocalDataTree(LocalDataTree* localDataTree) {
	_localDataTree = localDataTree;
}

void CGame::setServerDataTree(ServeurDataTree* serverDataTree) {
	_serverDataTree = serverDataTree;
}

LocalDataTree* CGame::getLocalDataTree() {
	return _localDataTree;
}

ServeurDataTree* CGame::getServerDataTree() {
	return _serverDataTree;
}

void CGame::setClientDataTree(ClientDataTree* clientDataTree) {
	_clientDataTree = clientDataTree;
}


DataTree* CGame::getDataTree() {
	if(_serverDataTree != 0) {
		return _serverDataTree;
	}
	else if(_clientDataTree != 0) {
		return _clientDataTree;
	}
	else if(_localDataTree != 0) {
		return _localDataTree;
	}
	else {
		return 0;
	}
}

ClientDataTree* CGame::getClientDataTree() {
	return _clientDataTree;
}

void CGame::Quit() {
	_mode = JKT_MODE_PARTIE_NULL;

	_erwin = 0;		// Oublie qu'il y a eu un joueur principal

	if( _map ) {
		_map->freePlugins();
		_map->freeGL();
		delete _map;
		_map = 0;
	}

	LOGDEBUG(("Destruction des joueurs begin"));
	_players.clear();
	LOGDEBUG(("Destruction des joueurs end"));

	_mode = JKT_MODE_PARTIE_NULL;	// Passe effectivemtn en mode 'aucune partie en cours'
}

void CGame::setModeNull() {
	_mode = JKT_MODE_PARTIE_NULL;
}

void CGame::setModeLocal() {
	_mode = JKT_MODE_PARTIE_LOCAL;
}

void CGame::setModeClient() {
	_mode = JKT_MODE_PARTIE_CLIENT;
}

void CGame::setModeServer() {
	_mode = JKT_MODE_PARTIE_SERVER;
}

jkt::CClient *CGame::getClient()
{	return _networkManager->getClient();		}

jkt::CServer *CGame::getServer() {
	return _networkManager->getServer();
}

void CGame::refresh() {
	Fabrique::getPluginEngine()->sendRefreshEvent();

	// Rafraichissement de la map
	_map->refresh(this);

	// Rafraichissement des projectils des joueurs
	CPlayer *player;
	int curseur = -1;

	while(_players.Suivant(curseur)) {		// Pour chaque joueur
		player = _players[curseur];

		if(player) {
			player->RefreshProjectils();
		}
	}
}

void CGame::GereContactPlayers() {	// Gère les contacts de tous les joueurs avec la map
	CPlayer *player;
	int curseur = -1;

	while(_players.Suivant(curseur)) {		// Pour chaque joueur
		player = _players[curseur];

		if(player) {
			player->Pente(0.0f);
			_map->GereContactPlayer(0, player);		// Gère les contact avec la map de player
		}
	}
}

void CGame::AfficheDirigeables() {
	DataTree* dataTree = getDataTree();

	vector<string> dirigeablesBrPath;
	dirigeablesBrPath.push_back("map");
	dirigeablesBrPath.push_back("sprites");
	dirigeablesBrPath.push_back("dirigeables");

	try {
		Branche* dirigeablesBr = dataTree->getBranche(0, dirigeablesBrPath);

		vector<Branche*>* subBranches = dirigeablesBr->getSubBranches(0);
		vector<Branche*>::const_iterator itBr;

		for(itBr = subBranches->begin() ; itBr != subBranches->end() ; itBr++) {
			Dirigeable::Affiche(*itBr);
		}
	}
	catch(NotExistingBrancheException& exception) {
		LOGWARN(("Unmanaged NotExistingBrancheException : %s", exception.what()));
	}
}

void CGame::AffichePlayers() {
	CPlayer *player;
	int curseur = -1;

	while(_players.Suivant(curseur)) {
		player = _players[curseur];

		if(player) {
			player->Affiche(); 				//affiche un seul joueur pour le moment
			player->AfficheProjectils(); 	// Affiche les projectils lancés par 'player'
		}
	}
}

void CGame::AfficheProjectils() {	// Affiche tous les projectils
	// Rafraichissement des projectils des joueurs
	CPlayer *player;
	int curseur = -1;

	while(_players.Suivant(curseur)) {	// Pour chaque joueur
		player = _players[curseur];

		if(player) {
			player->AfficheProjectils();
		}
	}
}

CPlayer *CGame::Erwin() {
	return _erwin;
}

void CGame::quitCurrentMap() {
	LOGINFO(("Fermeture de la Map courante"));

	CMap* currentMap = getMap();

	if(currentMap) {
		currentMap->freePlugins();
		currentMap->freeGL();
		changeActiveMap(0);
	}
}

void CGame::deletePlayers() {
	LOGINFO(("Suppression des joueurs"));

	CPlayer *player;
	int playerIndex = -1;

	while(_players.Suivant(playerIndex)) {
		player = _players[playerIndex];
		player->freeGL();
	}

	_players.clear();
}

void CGame::setErwin(CPlayer *erwin) {
	if(erwin) {
		LOGINFO(("Changement de joueur Erwin"));
	}
	else {
		LOGINFO(("Annulation du joueur Erwin"));
	}

	_erwin = erwin;
}

jkt::CMap *CGame::getMap() {
	return _map;
}

int CGame::afficheDamierTextures(int x, int y, int tailleX, int tailleY, int page, int nbrHoriz, int nbrVert) const {	// Affiche à l'écran toutes les textures de la Map
	if(_map) {
		return _map->afficheDamierTextures(x, y, tailleX, tailleY, nbrHoriz, nbrVert, page);
	}
	else {
		return 0;	// Page 0 : pas d'affichage de damier des textures actif
	}
}

void CGame::afficheViseur(int x, int y) const {
	// Affichage du focus sur l'arme active
	glDisable( GL_TEXTURE_2D );
	glDepthMask( GL_FALSE );
	glDisable( GL_BLEND );
	glDisable( GL_DEPTH_TEST );

	glLineWidth( 1.5 );
	glColor3f( 1.0f, 1.0f, 1.0f );

	glPushMatrix();
	glTranslatef( (float)x, (float)y, 0 );
	glBegin( GL_LINES );
	glVertex2i( -13,	0		);
	glVertex2i( -3,		0		);

	glVertex2i( 13,		0		);
	glVertex2i( 3,		0		);

	glVertex2i( 0,		-13		);
	glVertex2i( 0,		-3		);

	glVertex2i( 0,		13		);
	glVertex2i( 0,		3		);
	glEnd();
	glPopMatrix();
}

void CGame::deplaceTousPlayer() {
	CPlayer *player;
	int curseur = -1;

	while(_players.Suivant(curseur)) {
		player = _players[curseur];

		if(player) {
			player->deplace();
		}
	}
}

void CGame::faitTousRequetesClavier() {
	CPlayer *player;
	int curseur = -1;

	while(_players.Suivant(curseur)) {	//exécute les requêtes clavier sur les joueurs
		player = _players[curseur];

		if(player) {
			player->faitRequeteClavier();
		}
	}
}

CPlayer* CGame::getPlayer(int id) {
	return _players[id];
}

bool CGame::addPlayer(int id, CPlayer *player) {
	bool result = _players.add(id, player );

	if(result) {
		player->setId(id);
	}

	return result;
}

int CGame::addPlayer(CPlayer *player) {
	int id = _players.add( player );
	player->setId(id);
	return id;
}

void CGame::faitTousPlayerGravite() {
	int curseur = -1;
	CPlayer *player;			//Prends le premier player

	while(_players.Suivant(curseur)) {
		player = _players[curseur];

		if(player) {
			player->exeActionFunc();	//exécute l'action périodique (gravité,...) du joueur
		}
	}
}

void CGame::timer() {
	if(_map) {
		if( _gravite )					// Si la gravité est active
			faitTousPlayerGravite();

		refresh();						// Rafraichi les classes qui ont besoin de l'être (celles de type CMouve et CProjectil)
		faitTousRequetesClavier();		// Exécute les requêtes clavier sur tous les joueurs
		GereContactPlayers();			// Gère tous les contacts entre la map et les joueurs
		deplaceTousPlayer();			// Déplace tous les joueurs de leurs vitesses
	}
}

void CGame::setGravite(bool gravite) {
	_gravite = gravite;
}

bool CGame::getGravite() const {
	return _gravite;
}
