

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
#include "reseau/Reseau.h"

extern CCfg Config;
extern CGame Game;

void JktMoteur::contactPlayer(CPlayer *player, float *normal, float distanceW);

namespace JktNet
{

CReseau::CReseau() {
	m_On = false;	// Réseau inactivé par défaut
	m_Client = 0;
	m_Server = 0;
}

CReseau::~CReseau() {
	m_On = false;	// Réseau inactivé par défaut

	if( m_Client ) {
		delete m_Client;
		m_Client = 0;
	}

	if( m_Server ) {
		delete m_Server;
		m_Server = 0;
	}
}

void CReseau::setOn( bool on ) {
TRACE().p( TRACE_RESEAU, "CReseau::setOn(on=%b)%T", on, this );
	m_On = on;
}

bool CReseau::getOn() {
	return m_On;
}

bool CReseau::ouvreServer() {
TRACE().p( TRACE_RESEAU, "CReseau::ouvreServer() begin%T", this );
	bool result = false;

	if(m_Client) {
		delete m_Client;				// détruit le client
		m_Client = 0;
	}

	if(m_Server) {
		delete m_Server;				// Détruit le serveur
		m_Server = 0;
	}

	m_Server = new CServer();

	NotConnectedInterlocutor2* notConnectedServerInterlocutor = m_Server->ouvre(Config.Reseau.getPort(), Config.Reseau.getPortTree());

	if(notConnectedServerInterlocutor) {
		Game.setModeServer();
		m_Server->setStatut( JKT_STATUT_SERVER_READY );
		m_On = true;			// Indique que le réseau est actif
		result = true;
	}
	else {
		result = false;
	}

TRACE().p( TRACE_RESEAU, "CReseau::ouvreServer() -> %b end%T", result, this );
	return result;
}

void CReseau::fermeServer() {
TRACE().p( TRACE_RESEAU, "CReseau::fermeServer()%T", this );
	cout << endl << "Fermeture du serveur UDP";

	if( m_Server )
		delete m_Server;
	m_Server = 0;

	m_On = false;			// Indique que le réseau est inactif

	Game.Quit();
}

Interlocutor2* CReseau::ouvreClient() {
TRACE().p( TRACE_RESEAU, "CReseau::ouvreClient() begin%T", this );

	bool result = false;

	if( m_Client ) {
		delete m_Client;				// détruit le client
		m_Client = 0;
	}

	if( m_Server ) {
		delete m_Server;				// Détruit le serveur
		m_Server = 0;
	}

	m_Client = new CClient();

	Interlocutor2* interlocutor = m_Client->ouvre(Config.Reseau.getIpServer(), Config.Reseau.getPort(), Config.Reseau.getPortTree());

	if(interlocutor) {
		Game.setModeClient();
		m_On = true;		// Signale que le réseau est prêt
		result = true;
	}
	else {
		Game.setModeNull();
		m_On = true;		// Signale que le réseau n'est pas prêt
		result = false;
	}

TRACE().p( TRACE_RESEAU, "CReseau::ouvreClient() -> %b end%T", result, this );
	return interlocutor;
}

void CReseau::fermeClient() {
TRACE().p( TRACE_RESEAU, "CReseau::fermeClient()%T", this );
	cout << endl << "Deconnexion du serveur";

	if( m_Client )
		delete m_Client;
	m_Client = 0;

	m_On = false;		// Signale que le réseau n'est plus prêt

	Game.Quit();
}

void CReseau::recoitServer() {
	int numReady;
	numReady = SDLNet_CheckSockets( m_Server->socketSet, 0 );	// Nombre de sockets ayant une activité détectée

	if( numReady==-1 ) {
		cout << "SDLNet_CheckSockets: " << SDLNet_GetError();
	}
	else if( numReady ) {
		CPlayer *player;

		if( SDLNet_SocketReady( m_Server->spaMaitre.getSocket() ) ) {
			m_Server->decodeServerUDP( &m_Server->spaMaitre );
			numReady--;
		}

		int curseur = -1;

		while( Game._pTabIndexPlayer->Suivant(curseur) && numReady ) {	// S'il reste de l'activité sur un socket et des proxy joueurs à voir
			player = m_Server->GetPlayer( curseur );

			if( SDLNet_SocketReady( player->spa.getSocket() ) ) {	// S'il y a de l'activité sur ce socket
				m_Server->decodeProxyPlayer( player );
				numReady--;
			}
		}
	}
}

CServer *CReseau::getServer() {
	return m_Server;
}

CClient *CReseau::getClient() {
	return m_Client;
}

}	// JktNet
