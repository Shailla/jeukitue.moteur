

#include <iostream>
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

#include "util/Erreur.h"
#include "util/V3D.h"
#include "main/Cfg.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "spatial/contact.h"
#include "main/Game.h"
#include "util/Trace.h"

#include "enumReseau.h"
#include "reseau/NetworkManager.h"

extern CCfg Config;
extern CGame Game;

void JktMoteur::contactPlayer(CPlayer *player, float *normal, float distanceW);

namespace JktNet
{

NetworkManager::NetworkManager() {
	_On = false;	// Réseau inactivé par défaut
	_Client = 0;
	_Server = 0;
}

NetworkManager::~NetworkManager() {
	_On = false;	// Réseau inactivé par défaut

	if( _Client ) {
		delete _Client;
		_Client = 0;
	}

	if( _Server ) {
		delete _Server;
		_Server = 0;
	}
}

void NetworkManager::setOn( bool on ) {
TRACE().p( TRACE_RESEAU, "CReseau::setOn(on=%b)%T", on, this );
	_On = on;
}

bool NetworkManager::getOn() {
	return _On;
}

bool NetworkManager::ouvreServer() {
TRACE().p( TRACE_RESEAU, "CReseau::ouvreServer() begin%T", this );
	bool result = false;

	if(_Client) {
		delete _Client;				// détruit le client
		_Client = 0;
	}

	if(_Server) {
		delete _Server;				// Détruit le serveur
		_Server = 0;
	}

	_Server = new CServer();

	NotConnectedInterlocutor2* notConnectedServerInterlocutor = _Server->ouvre(Config.Reseau.getPort(), Config.Reseau.getPortTree());

	if(notConnectedServerInterlocutor) {
		Game.setModeServer();
		_Server->setStatut( JKT_STATUT_SERVER_READY );
		_On = true;			// Indique que le réseau est actif
		result = true;
	}
	else {
		result = false;
	}

TRACE().p( TRACE_RESEAU, "CReseau::ouvreServer() -> %b end%T", result, this );
	return result;
}

void NetworkManager::fermeServer() {
TRACE().p( TRACE_RESEAU, "CReseau::fermeServer()%T", this );
	cout << endl << "Fermeture du serveur UDP";

	if( _Server )
		delete _Server;
	_Server = 0;

	_On = false;			// Indique que le réseau est inactif

	Game.Quit();
}

Interlocutor2* NetworkManager::ouvreClient() {
TRACE().p( TRACE_RESEAU, "CReseau::ouvreClient() begin%T", this );

	bool result = false;

	if( _Client ) {
		delete _Client;				// détruit le client
		_Client = 0;
	}

	if( _Server ) {
		delete _Server;				// Détruit le serveur
		_Server = 0;
	}

	_Client = new CClient();

	Interlocutor2* interlocutor = _Client->ouvre(Config.Reseau.getIpServer(), Config.Reseau.getPort(), Config.Reseau.getPortTree());

	if(interlocutor) {
		Game.setModeClient();
		_On = true;		// Signale que le réseau est prêt
		result = true;
	}
	else {
		Game.setModeNull();
		_On = true;		// Signale que le réseau n'est pas prêt
		result = false;
	}

TRACE().p( TRACE_RESEAU, "CReseau::ouvreClient() -> %b end%T", result, this );
	return interlocutor;
}

void NetworkManager::fermeClient() {
TRACE().p( TRACE_RESEAU, "CReseau::fermeClient()%T", this );
	cout << endl << "Deconnexion du serveur";

	if( _Client )
		delete _Client;
	_Client = 0;

	_On = false;		// Signale que le réseau n'est plus prêt

	Game.Quit();
}

void NetworkManager::recoitServer() {
	int numReady;
	numReady = SDLNet_CheckSockets( _Server->socketSet, 0 );	// Nombre de sockets ayant une activité détectée

	if( numReady==-1 ) {
		cout << "SDLNet_CheckSockets: " << SDLNet_GetError();
	}
	else if( numReady ) {
		CPlayer *player;

		if( SDLNet_SocketReady( _Server->spaMaitre.getSocket() ) ) {
			_Server->decodeServerUDP( &_Server->spaMaitre );
			numReady--;
		}

		int curseur = -1;

		while( Game._pTabIndexPlayer->Suivant(curseur) && numReady ) {	// S'il reste de l'activité sur un socket et des proxy joueurs à voir
			player = _Server->GetPlayer( curseur );

			if( SDLNet_SocketReady( player->spa.getSocket() ) ) {	// S'il y a de l'activité sur ce socket
				_Server->decodeProxyPlayer( player );
				numReady--;
			}
		}
	}
}

CServer *NetworkManager::getServer() {
	return _Server;
}

CClient *NetworkManager::getClient() {
	return _Client;
}

}	// JktNet
