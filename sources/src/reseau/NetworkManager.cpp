#include <vector>
#include <fstream>
#include <set>
#include <map>
#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

class CGame;

#include "util/Trace.h"
#include "util/TraceMethod.h"
#include "util/Erreur.h"
#include "util/IpUtils.h"
#include "util/V3D.h"
#include "main/Cfg.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "spatial/contact.h"
#include "main/Game.h"

#include "enumReseau.h"
#include "reseau/NetworkManager.h"

extern CGame Game;

namespace JktNet
{

NetworkManager::NetworkManager() {
	_on = false;	// Réseau inactivé par défaut
	_client = new CClient();
	_server = 0;
}

NetworkManager::~NetworkManager() {
	_on = false;	// Réseau inactivé par défaut

	if( _client ) {
		delete _client;
		_client = 0;
	}

	if( _server ) {
		delete _server;
		_server = 0;
	}
}

void NetworkManager::setOn( bool on ) {
	LOGDEBUG(("CReseau::setOn(on=%b)%T", on, this));
	_on = on;
}

bool NetworkManager::getOn() {
	return _on;
}

NotConnectedInterlocutor2* NetworkManager::ouvreServer(Uint16 serverPort, Uint16 serverTreePort) {
	LOGDEBUG(("CReseau::ouvreServer() begin%T", this));
	bool result = false;

	if(_client) {
		delete _client;				// détruit le client
		_client = 0;
	}

	if(_server) {
		delete _server;				// Détruit le serveur
		_server = 0;
	}

	_server = new CServer();

	NotConnectedInterlocutor2* notConnectedServerInterlocutor = _server->connect(serverPort, serverTreePort);

	if(notConnectedServerInterlocutor) {
		_server->setStatut( JKT_STATUT_SERVER_READY );
		_on = true;			// Indique que le réseau est actif
		result = true;
	}
	else {
		result = false;
	}

	LOGDEBUG(("CReseau::ouvreServer() -> %b end%T", result, this));

	return notConnectedServerInterlocutor;
}

void NetworkManager::fermeServer() {
	LOGDEBUG(("CReseau::fermeServer()%T", this));
	LOGINFO(("Fermeture du serveur UDP"));

	if( _server )
		delete _server;
	_server = 0;

	_on = false;			// Indique que le réseau est inactif

	Game.Quit();
}

Interlocutor2* NetworkManager::ouvreClient(const string& serverIp, Uint16 serverPort, Uint16 serverTreePort) {
	LOGDEBUG(("CReseau::ouvreClient() begin%T", this));

	bool result = false;

	if( _client ) {
		delete _client;				// détruit le client
		_client = 0;
	}

	if( _server ) {
		delete _server;				// Détruit le serveur
		_server = 0;
	}

	_client = new CClient();

	Interlocutor2* interlocutor = _client->connect(serverIp, serverPort, serverTreePort);

	if(interlocutor) {
		_on = true;		// Signale que le réseau est prêt
		result = true;
	}
	else {
		_on = true;		// Signale que le réseau n'est pas prêt
		result = false;
	}

	LOGDEBUG(("CReseau::ouvreClient() -> %b end%T", result, this));
	return interlocutor;
}

void NetworkManager::fermeClient() {
	LOGDEBUG(("CReseau::fermeClient()%T", this));
	LOGINFO(("Deconnexion du serveur"));

	if( _client )
		delete _client;
	_client = 0;

	_on = false;		// Signale que le réseau n'est plus prêt

	Game.Quit();
}

void NetworkManager::recoitServer() {
	int numReady;
	numReady = SDLNet_CheckSockets( _server->socketSet, 0 );	// Nombre de sockets ayant une activité détectée

	if( numReady==-1 ) {
		LOGERROR(("SDLNet_CheckSockets : %s", SDLNet_GetError()));
	}
	else if( numReady ) {
		if( SDLNet_SocketReady( _server->spaMaitre.getSocket() ) ) {
			IPaddress inAddress = _server->spaMaitre.getPacketIn()->address;

			CPlayer* player = _server->getPlayerByAddress(inAddress);

			_server->spaMaitre.recoit();

			if(!player) {
				LOGINFO(("Joueur inconnu %", JktUtils::IpUtils::translateAddress(inAddress).c_str()));

				Uint16 code1, code2;
				_server->spaMaitre.init();
				_server->spaMaitre.readCode(code1, code2);

				if(code1 == GLOBAL_JETON) {
					LOGINFO(("Joueur avec un jeton %d", code2));
					player = _server->getPlayerByJeton(code2);

					if(player) {
						_server->registerPlayerByAddress(inAddress, player);
					}
					else {
						LOGINFO(("Jeton inconnu %d", code2));
					}
				}
				else {
					LOGINFO(("Pas de jeton"));
				}
			}

			if(player) {
				_server->decodeProxyPlayer(player, &_server->spaMaitre);
			}
			else {
				_server->decodeServerUDP( &_server->spaMaitre );
			}
		}

//		int curseur = -1;
//		CPlayer *player;
//
//		while( (player = Game.nextPlayer(curseur)) && numReady ) {			// S'il reste de l'activité sur un socket et des proxy joueurs à voir
//			if(player->_spa) {
//				if( SDLNet_SocketReady( player->_spa->getSocket() ) ) {		// S'il y a de l'activité sur ce socket
//					_server->decodeProxyPlayer( player );
//					numReady--;
//				}
//			}
//		}
	}
}

CServer *NetworkManager::getServer() {
	return _server;
}

CClient *NetworkManager::getClient() {
	return _client;
}

}	// JktNet
