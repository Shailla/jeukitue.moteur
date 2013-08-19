
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

class CGame;

#include "util/Erreur.h"
#include "util/V3D.h"
#include "util/Tableau.cpp"
#include "util/TableauIndex.cpp"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/RequeteProcess.h"
#include "spatial/Map.h"
#include "reseau/NetworkManager.h"
#include "util/Trace.h"
#include "main/Clavier.h"

#include "reseau/enumReseau.h"
#include "main/Game.h"

using namespace JktNet;
using namespace JktMoteur;

extern NetworkManager _networkManager;

#define Pi 3.14159265f			//nombre pi pour les calculs

CGame::CGame() {
	_mode = JKT_MODE_PARTIE_NULL;
	_pTabIndexPlayer = NULL;	// Pas de liste de joueurs
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

void CGame::setStatutClient( JktNet::StatutClient statut ) {
TRACE().p( TRACE_ERROR, "CGame::setStatusClient(status=%d) begin%T", statut, this );

	_networkManager.getClient()->setStatut( statut );

TRACE().p( TRACE_ERROR, "CGame::setStatusClient() end%T", this );
}

JktNet::StatutClient CGame::getStatutClient() {
TRACE().p( TRACE_ERROR, "CGame::getStatutClient() begin%T", this );

	JktNet::StatutClient statut;
	if( _mode==JKT_MODE_PARTIE_CLIENT )
		statut = _networkManager.getClient()->getStatut();
	else
		statut = JKT_STATUT_CLIENT_NULL;

TRACE().p( TRACE_ERROR, "CGame::getStatutClient() -> %d end%T", statut, this );
	return statut;
}

int CGame::getMaxPlayers() const {
	return _pTabIndexPlayer->getMax();
}

void CGame::setPlayerList(int nbr) {
	if(_pTabIndexPlayer) {
		delete _pTabIndexPlayer;
	}

	_pTabIndexPlayer = new CTableauIndex<CPlayer>(nbr);

	cout << endl << "setPlayerList";

	int curseur = -1;

	while(_pTabIndexPlayer->Suivant(curseur)) {
		CPlayer* player = _pTabIndexPlayer->operator [](curseur);

		cout << endl << "\t" << curseur << ":" << player;
	}
}

void CGame::quit() {
	if(_pTabIndexPlayer)
		delete _pTabIndexPlayer;

	_pTabIndexPlayer = 0;
}

bool CGame::openMap(const string &nomFichierMap) throw(JktUtils::CErreur) {
TRACE().p( TRACE_INFO, "CGame::openMap(nomFichierMap=%s) begin%T", nomFichierMap.c_str(), this );

	bool result = true;

	if( _map )
		delete _map;

	try {
		_map = new JktMoteur::CMap( nomFichierMap );
	}
	catch(JktUtils::CErreur& erreur) {
		cerr << endl << __FILE__ << ":" << __LINE__ <<  " " << erreur.toString() << endl;
		result = false;
	}

TRACE().p( TRACE_INFO, "CGame::openMap() -> %b end%T", result, this );
	return result;
}

void CGame::changeActiveMap(JktMoteur::CMap* map) {
	if( _map )
		delete _map;

	_map = map;
}

void CGame::setStatutServer(JktNet::StatutServer statut) {
	if( _mode==JKT_MODE_PARTIE_SERVER )
		_networkManager.getServer()->setStatut( statut );
}

JktNet::StatutServer CGame::getStatutServer() {
	if( _mode==JKT_MODE_PARTIE_SERVER )
		return _networkManager.getServer()->getStatut();
	else
		return JKT_STATUT_SERVER_NULL;
}

void CGame::Quit() {
	_mode = JKT_MODE_PARTIE_NULL;

	_erwin = 0;		// Oublie qu'il y a eu un joueur principal

	if( _map ) {
		delete _map;
		_map = 0;
	}

	cout << "\nDestruction des joueurs begin";
	int curseur = -1;

	if(_pTabIndexPlayer) {
		while(_pTabIndexPlayer->Suivant(curseur)) {
			CPlayer* player = (*_pTabIndexPlayer)[curseur];

			if(player) {
				cout << endl << "Destruction du joueur '" << player->nom() << "'" << flush;
				delete player;
			}
		}

		cout << "\nDestruction de la liste des joueurs";
		delete _pTabIndexPlayer;	// Destruction du tableau des joueurs
		_pTabIndexPlayer = 0;
	}
	cout << "\nDestruction des joueurs end";

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

JktNet::CClient *CGame::getClient()
{	return _networkManager.getClient();		}

JktNet::CServer *CGame::getServer()
{	return _networkManager.getServer();		}

void CGame::Refresh() {
		// Rafraichissement de la map
	_map->Refresh( this );

		// Rafraichissement des projectils des joueurs
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer->Suivant(curseur)) {		// Pour chaque joueur
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->RefreshProjectils();
		}
	}
}

void CGame::GereContactPlayers() {	// Gère les contacts de tous les joueurs avec la map
	CPlayer *player;
	int curseur = -1;
	while(_pTabIndexPlayer->Suivant(curseur)) {		// Pour chaque joueur
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->Pente(0.0f);
			_map->GereContactPlayer(player);		// Gère les contact avec la map de player
		}
	}
}

void CGame::AffichePlayers() {
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer->Suivant(curseur)) {
		player = _pTabIndexPlayer->operator [](curseur);

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

	while(_pTabIndexPlayer->Suivant(curseur)) {	// Pour chaque joueur
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->AfficheProjectils();
		}
	}
}

CPlayer *CGame::Erwin() {
	return _erwin;
}

void CGame::quitCurrentMap() {
	CMap* currentMap = getMap();

	if(currentMap) {
		currentMap->freeGL();
		changeActiveMap(0);
	}
}

void CGame::deletePlayers() {
	if(_pTabIndexPlayer) {
		CPlayer *player;
		int playerIndex = -1;

		while(_pTabIndexPlayer->Suivant(playerIndex)) {
			player = _pTabIndexPlayer->operator [](playerIndex);
			player->freeGL();
		}

		delete _pTabIndexPlayer;
		_pTabIndexPlayer = 0;
	}
}

void CGame::Erwin(CPlayer *erwin) {
	_erwin = erwin;
}

JktMoteur::CMap *CGame::getMap() {
	return _map;
}

void CGame::afficheToutesTextures(int x, int y, int tailleX, int tailleY) const {	// Affiche à l'écran toutes les textures de la Map
	if(_map)
		_map->afficheToutesTextures(x, y, tailleX, tailleY, 6, 6, 0);
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

	while(_pTabIndexPlayer && _pTabIndexPlayer->Suivant(curseur)) {
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->deplace();
		}
	}
}

void CGame::faitTousRequetesClavier() {
	CPlayer *player;
	int curseur = -1;

	while(_pTabIndexPlayer && _pTabIndexPlayer->Suivant(curseur)) {	//exécute les requêtes clavier sur les joueurs
		player = _pTabIndexPlayer->operator[](curseur);

		if(player) {
			player->faitRequeteClavier();
		}
	}
}

void CGame::faitTousPlayerGravite() {
	int curseur = -1;
	CPlayer *player;			//Prends le premier player

	while(_pTabIndexPlayer && _pTabIndexPlayer->Suivant(curseur)) {
		player = _pTabIndexPlayer->operator [](curseur);

		if(player) {
			player->exeActionFunc();	//exécute l'action périodique (gravité,...) du joueur
		}
	}
}

void CGame::timer() {
	if( _gravite )				// Si la gravité est active
		faitTousPlayerGravite();

	if(_map)
		Refresh();					// Rafraichi les classes qui ont besoin de l'être (celles de type CMouve et CProjectil)

	faitTousRequetesClavier();		// Exécute les requêtes clavier sur tous les joueurs


	if(_map)
		GereContactPlayers();		// Gère tous les contacts entre la map et les joueurs

	deplaceTousPlayer();			// Déplace tous les joueurs de leurs vitesses
}

void CGame::Gravite(bool gravite) {
	_gravite = gravite;
}

bool CGame::Gravite() const {
	return _gravite;
}
