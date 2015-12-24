
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <fstream>
#include <map>
#include <iostream>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "util/Trace.h"
#include "util/TraceMethod.h"
#include "util/Erreur.h"
#include "util/V3D.h"
#include "util/TableauIndex.cpp"
#include "spatial/IfstreamMap.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "reseau/Server.h"
#include "main/Clavier.h"
#include "spatial/contact.h"
#include "main/Game.h"

#include "reseau/enumReseau.h"
#include "reseau/Server.h"

void JktMoteur::contactPlayer(CPlayer *player, float *normal, float distanceW);
void gravitePlayer(CPlayer *player);

extern CGame Game;
class CMap;

namespace JktNet
{

CServer::CServer()
{
	LOGDEBUG(("CServer::CServer() begin%T", this ));
	maxPlayers = 10;					// 10 joueurs maxi pas d�faut
	m_uNbrPlayers = 0;					// Aucun joueur
	nom = "ServerJKT";					// Nom de serveur par d�faut
	bGame = false;						// Pas de partie en cours
	nomMAP = "";						//donc pas de nom de partie
	m_Statut = JKT_STATUT_SERVER_INIT;	// Indique que le serveur est en cours d'initialisation

	socketSet = 0;

	_serverUdpInterlocutor = 0;

	LOGDEBUG(("CServer::CServer() end%T", this ));
}

CServer::~CServer() {
}

void CServer::disconnect() {
	LOGDEBUG(("CServer::~CServer() begin%T", this ));
	SDLNet_FreeSocketSet( socketSet );		// Lib�re le socket set du serveur
	socketSet = 0;

	if(_serverUdpInterlocutor)
		_serverUdpInterlocutor->close();

	delete _serverUdpInterlocutor;
	LOGDEBUG(("CServer::~CServer() end%T", this ));
}

void CServer::setStatut( StatutServer statut )
{	m_Statut = statut;		}

StatutServer CServer::getStatut()
{	return m_Statut;		}

int CServer::AjoutePlayer( CPlayer *player ) {
	LOGDEBUG(("CServer::AjoutePlayer(player=%x)%T", player, this ));

	int indexPlayer;
	if( m_uNbrPlayers < this->maxPlayers ) {
		m_uNbrPlayers++;		// Incr�mente le nbr de joueurs sur le serveur

		indexPlayer = Game.addPlayer(player);	// Ajoute le joueur � la liste
	}
	else {
		indexPlayer = -1;
	}

	LOGDEBUG(("CServer::AjoutePlayer() -> indexPlayer=%d end%T", indexPlayer, this ));
	return indexPlayer;
}

int CServer::nbrPlayers()
{	return m_uNbrPlayers;	}


void CServer::decodeServerUDP( CSPA *spa ) {
	Uint16 code1, code2;
	if( spa->recoit() )			// Si des paquets ont �t� re�us
	{
		spa->init();
		spa->readCode( code1, code2 );
		switch( code1 )
		{
		case CLIENT_INFO:		// Un client demande les info serveur
			switchInfo( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

		case CLIENT_PING:	// Un client a envoy� un ping
			switchPing( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

		case CLIENT_JTG:		// Un client demande � entrer dans le jeu en cours sur le serveur
			switchJTG( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

		default:
			LOGERROR(("Paquet inconnu decodeServerUDP (%d)", code1));
			break;
		}
	}
}

void CServer::decodeProxyPlayer( CPlayer *player ) {
	Uint16 code1, code2;

	if(player->_spa) {
		if( player->_spa->recoit() ) {	// Si des paquets ont �t� re�us
			player->_spa->init();
			player->_spa->readCode( code1, code2 );
			switch( code1 ) {
			case CLIENT_RECAP:		// R�ception d'une r�capitulation de partie
				switchRecapServer( player, code2 );
				break;

			default:
				LOGWARN(("Reception d'un paquet PROXYPLAYER inconnu"));
				break;
			}
		}
	}
}

bool CServer::acceptPlayer( CSPA *spa ) {
	TRACEMETHOD();
	bool result = true;

	char nomNewPlayer[50];
	spa->readChar( nomNewPlayer );							// R�cup�re le nom du nouveau joueur


	CPlayer* newPlayer = new CPlayer();						// Cr�e le r�ceptacle du nouveau joueur
	newPlayer->changeAction( gravitePlayer );				// Associe au joueur une fonction de gravit�
	newPlayer->changeContact( JktMoteur::contactPlayer );	// Associe une fonction de gestion des contacts avec la map
	newPlayer->nom( nomNewPlayer );							// Enregistre le nom du nouveau joueur
	newPlayer->init();

	// Ouverture d'un lien SPA et cr�ation du joueur
	result = newPlayer->openInClientMode( spa->getPacketIn()->address );

	if( result ) {
		if( SDLNet_UDP_AddSocket( socketSet, newPlayer->_spa->getSocket() )==-1 ) {
			LOGERROR(("SDLNet_AddSocket : ", SDLNet_GetError()));

			newPlayer->close();
			delete newPlayer;

			result = false;
		}
	}

	if(result) {
		int IDPlayer = AjoutePlayer( newPlayer );	// Ajoute le joueur pour de bon et identifie-le
		// TODO G�rer le cas o� IDPlayer vaut -1, lorsque l'ajout du joueur a �chou�

		LOGINFO(("Cr�ation du nouveau joueur %d:%s", IDPlayer, nomNewPlayer));

		/* /////////////////////////////////////////////////////////////////////////////////
		// Dis au client qu'il est accept� dans le jeu et donne-lui les infos de la partie
		// /////////////////////////////////////////////////////////////////////////////////*/

		newPlayer->_spa->init();
		newPlayer->_spa->addCode( SERVER_JTG, SERVER_ACK );		// Acquitte la jonction du joueur � la partie
		newPlayer->_spa->add16( (Uint16)IDPlayer );				// Envoie son identifiant au nouveau proxy-joueur
		newPlayer->_spa->add( nomMAP );							// Envoie le nom de la MAP en cours
		newPlayer->_spa->add16( (Uint16)Game.getMaxPlayers() );			// Envoie le nombre max de joueurs sur la MAP
		newPlayer->_spa->add16( (Uint16)Game.getNbrPlayers() );	// Envoie le nombre de joueurs en cours

		// Envoie les infos concernant chaque joueur
		int curseur = -1;

		CPlayer* varPlayer;

		while((varPlayer = Game.nextPlayer(curseur))) {
			newPlayer->_spa->add16( (Uint16)curseur );			// Envoie l'identifiant de ce proxy-joueur
			newPlayer->_spa->add( varPlayer->nom() );			// Envoie son nom

			newPlayer->_spa->addRecapFromServer( *varPlayer );
		}

		newPlayer->_spa->send();
	}

	return result;	// OK
}

void CServer::switchRecapServer( CPlayer *player, Uint16 code2 ) {
	Uint16 flags;

	switch( code2 ) {
	case CLIENT_NULL:
		flags = player->_spa->read16();

		player->getClavier()->m_fAvance = 0.0f;
		player->getClavier()->m_fDroite = 0.0f;
		player->getClavier()->m_fMonte = 0.0f;

		if( flags&0x0001 ) {	// S'il y a des requ�tes
			player->getClavier()->m_bIndic = true;	// Indique la pr�sence d'une requ�te

			if( flags&0x0010 ) {	// Y a-t-il une requ�te de mouvement en avant
				player->getClavier()->m_fAvance = player->_spa->readf();
			}

			if( flags&0x0100 ) {	// Y a-t-il une requ�te de mouvement � droite
				player->getClavier()->m_fDroite = player->_spa->readf();
			}

			if( flags&0x1000 ) {	// Y a-t-il une requ�te de mouvement vers le haut
				player->getClavier()->m_fMonte = player->_spa->readf();
			}
		}
		else {
			player->getClavier()->m_bIndic = false;
		}

		player->Phi( player->_spa->readf() );
		player->Teta( player->_spa->readf() );

		break;

	default:
		LOGERROR(("Reception d'un paquet RECAP inconnu"));
		break;
	}
}

void CServer::switchInfo( CSPA *spa, Uint16 code2 ) {	// R�ception d'une demande d'info serveur
	LOGDEBUG(("CServer::switchInfo(spa=%x,code2=%d) begin%T", spa, code2, this ));
	switch( code2 ) {
	case CLIENT_NULL:
		LOGINFO(("Server says : Reception d'une demande d'info serveur"));
		spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );

		if( spa->getPacketOut()->channel==-1 ) {
			LOGERROR(("SDLNet_UDP_Bind : ", SDLNet_GetError()));
			LOGERROR(("Reponse info serveur impossible"));
		}
		else {
			// Envoie des infos serveur au client
			LOGINFO(("Server says : Emmission des infor serveur"));

			spa->init();
			spa->addCode( SERVER_INFO, SERVER_ACK );
			spa->add( nom );
			spa->add( nomMAP );
			spa->send();
		}
		break;

	default:
		LOGDEBUG(("CServer::switchInfo() : Packet inconnu%T", this ));
		LOGINFO(("Reception d'un paquet inconnu"));
		break;
	}
	LOGDEBUG(("CServer::switchInfo() end%T", this ));
}

void CServer::switchPing( CSPA *spa, Uint16 code2 )	// R�ception d'une demande de ping
{
	LOGDEBUG(("CServer::switchPing(spa=%x,code2=%d) begin%T", spa, code2, this ));
	switch( code2 ) {
	case CLIENT_NULL:
		if( spa->getPacketIn()->len==8 ) {
			LOGINFO(("Server says : Reception d'une demande de ping"));
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );

			if( spa->getPacketOut()->channel==-1 ) {
				LOGERROR(("SDLNet_UDP_Bind : %s", SDLNet_GetError()));
				LOGERROR(("Reponse ping impossible"));
			}
			else {
				LOGDEBUG(("CServer::switchPing() : Envoi reponse ping%T", this ));
				// Envoie de la r�ponse au ping
				Uint32 time;
				LOGINFO(("Server says : Emmission de la reponse au ping"));
				time = spa->read32();

				spa->init();
				spa->addCode( SERVER_PING, SERVER_ACK );
				spa->add32( time );
				spa->send();
			}
		}
		break;

	default:
		LOGWARN(("Reception d'un paquet inconnu"));
		break;
	}
	LOGDEBUG(("CServer::switchPing() end%T", this ));
}

void CServer::switchJTG( CSPA *spa, Uint16 code2 )	// R�ception d'une demande de JTG
{
	LOGDEBUG(("CServer::switchJTG(spa=%x,code2=%d) begin%T", spa, code2, this ));
	switch( code2 )
	{
	case CLIENT_NULL:
		LOGINFO(("Server says : Reception de demande a se joindre a la partie"));
		if( m_Statut!=JKT_STATUT_SERVER_PLAY )	// S'il n'y a pas de partie en cours
		{
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );

			if( spa->getPacketOut()->channel==-1 ) {
				LOGDEBUG(("CServer::switchJTG() SDLNet_UDP_Bind : %s%T", SDLNet_GetError(), this );
				LOGERROR(("SDLNet_UDP_Bind : %s", SDLNet_GetError())));
				LOGERROR(("Reponse ping impossible"));
			}
			else {
				LOGDEBUG(("CServer::switchJTG() : Pas de partie en cours%T", this ));
				// Envoie d'une erreur signalant qu'il n'y a pas de partie en cours
				LOGINFO(("Demande refusee, pas de partie en cours"));
				spa->init();
				spa->addCode( SERVER_JTG, SERVER_ERROR );
				spa->add16( UDP_ERROR_NOGAME );
				spa->send();
				SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			}
			break;
		}
		if( m_uNbrPlayers>=maxPlayers )	// Si le nbr max de joueurs est atteint
		{
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );
			if( spa->getPacketOut()->channel==-1 ) {
				LOGDEBUG(("CServer::switchJTG() SDLNet_UDP_Bind : %s%T", SDLNet_GetError(), this ));
				LOGERROR(("SDLNet_UDP_Bind : %s", SDLNet_GetError()));
				LOGERROR(("Reponse ping impossible"));
			}
			else {
				LOGDEBUG(("CServer::switchJTG() : Serveur sature%T", this ));
				LOGINFO(("Serveur satur�"));
				spa->init();
				spa->addCode( SERVER_JTG, SERVER_ERROR );
				spa->add16( UDP_ERROR_SATURE );
				spa->send();
				SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			}
			break;
		}

		if( acceptPlayer( spa ) )		// Ajoute le client associ� � spa en tant que joueur
			LOGINFO(("Demande accept�e"));
		else
			LOGERROR((" Demande infructueuse"));
		break;
	default:
		LOGDEBUG(("CServer::switchJTG() : Packet inconnu%T", this ));
		LOGERROR(("Reception d'un paquet inconnu 3"));
		break;
	}
	LOGDEBUG(("CServer::switchJTG() end%T", this ));
}

NotConnectedInterlocutor2* CServer::connect(Uint16 port, Uint16 portTree) {

	/* ***********************************************************************
	 * Ouverture d'un serveur primitif
	 * **********************************************************************/

	bool result = true;
	NotConnectedInterlocutor2* notConnectedServerInterlocutor = 0;
	LOGINFO(("Ouverture d'un serveur sur port %d et portTree %d", port, portTree));

	if( !spaMaitre.openInServerMode( port ) ) {
		LOGDEBUG(("CServer::ouvre() : %s %T", SDLNet_GetError(), this ));
		result = false;
	}

	if(result) {
		socketSet = SDLNet_AllocSocketSet( 20 );	// Nombre maxi de sockets � �couter

		if( !socketSet ) {
			LOGDEBUG(("CServer::ouvre() : %s %T", SDLNet_GetError(), this ));
			LOGERROR(("SDLNet_AllocSocketSet : %s", SDLNet_GetError()));

			spaMaitre.close();

			result = false;
		}
	}

	if(result) {
		if( SDLNet_UDP_AddSocket( socketSet, spaMaitre.getSocket() )==-1 ) {
			LOGDEBUG(("CServer::ouvre() : %s %T", SDLNet_GetError(), this ));
			disconnect();

			result = false;
		}
	}


	/* ***********************************************************************
	 * Ouverture d'un serveur moderne
	 * **********************************************************************/

	if(result) {
		_serverUdpInterlocutor = new ServerUdpInterlocutor("Server", portTree);
		notConnectedServerInterlocutor = _serverUdpInterlocutor->connect();
	}

	LOGDEBUG(("CServer::ouvre() -> %b end%T", result, this));
	return notConnectedServerInterlocutor;	// Ouverture serveur r�ussie
}

void CServer::emet() {
	CPlayer *player, *player2;
	int curseur = -1, curseur2;

	while((player = Game.nextPlayer(curseur))) {	// Envoie � chaque client-joueur
		if(player->_spa) {
			player->_spa->init();
			player->_spa->addCode( SERVER_RECAP, SERVER_NULL );		// Code de r�capitulation
			player->_spa->add16( (Uint16)nbrPlayers() );				// Envoie le nombre de joueur

			// Envoie les infos concernant chaque joueur
			curseur2 = -1;

			while((player2 = Game.nextPlayer(curseur2))) {
				player->_spa->add16( (Uint16)curseur2 );			// Envoie l'identifiant de ce proxy-joueur
				player->_spa->addRecapFromServer( *player2 );
			}

			player->_spa->send();
		}
	}
}

}	// JTK_PACKAGE_RESEAU
