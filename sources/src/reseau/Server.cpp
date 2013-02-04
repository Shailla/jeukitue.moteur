
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
#include "util/Trace.h"
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
TRACE().p( TRACE_RESEAU, "CServer::CServer() begin%T", this );
	maxPlayers = 10;					// 10 joueurs maxi pas défaut
	m_uNbrPlayers = 0;					// Aucun joueur
	nom = "ServerJKT";					// Nom de serveur par défaut
	bGame = false;						// Pas de partie en cours
	nomMAP = "";						//donc pas de nom de partie
	m_Statut = JKT_STATUT_SERVER_INIT;	// Indique que le serveur est en cours d'initialisation

	socketSet = 0;
TRACE().p( TRACE_RESEAU, "CServer::CServer() end%T", this );
}

CServer::~CServer()
{
TRACE().p( TRACE_RESEAU, "CServer::~CServer() begin%T", this );
	SDLNet_FreeSocketSet( socketSet );		// Libère le socket set du serveur
	socketSet = 0;

	Game.quit();
TRACE().p( TRACE_RESEAU, "CServer::~CServer() end%T", this );
}

CPlayer *CServer::GetPlayer( int pos )
{	return Game.pTabIndexPlayer->operator []( pos );	}

void CServer::setStatut( StatutServer statut )
{	m_Statut = statut;		}

StatutServer CServer::getStatut()
{	return m_Statut;		}

int CServer::AjoutePlayer( CPlayer *player )
{
TRACE().p( TRACE_RESEAU, "CServer::AjoutePlayer(player=%x)%T", player, this );

	int nbr;
	if( m_uNbrPlayers<this->maxPlayers )
	{
		m_uNbrPlayers++;		// Incrémente le nbr de joueurs sur le serveur
		nbr = Game.pTabIndexPlayer->Ajoute( player );	// Ajoute le joueur à la liste
	}
	else
	{
		nbr = -1;
	}

TRACE().p( TRACE_RESEAU, "CServer::AjoutePlayer() -> %d end%T", nbr, this );
	return nbr;
}

int CServer::nbrPlayers()
{	return m_uNbrPlayers;	}


void CServer::decodeServerUDP( CSPA *spa )
{
	Uint16 code1, code2;
	if( spa->recoit() )			// Si des paquets ont été reçus
	{
		spa->init();
		spa->readCode( code1, code2 );
		switch( code1 )
		{
		case CLIENT_INFO:		// Un client demande les info serveur
			switchInfo( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

   		case CLIENT_PING:	// Un client a envoyé un ping
			switchPing( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

		case CLIENT_JTG:		// Un client demande à entrer dans le jeu en cours sur le serveur
			switchJTG( spa, code2 );
			SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			break;

		default:
			cerr << endl << "Paquet inconnu decodeServerUDP";
			break;
		}
	}
}

void CServer::decodeProxyPlayer( CPlayer *player )
{
	Uint16 code1, code2;

	if( player->spa.recoit() )	// Si des paquets ont été reçus
	{
		player->spa.init();
		player->spa.readCode( code1, code2 );
		switch( code1 )
		{
		case CLIENT_RECAP:		// Réception d'une récapitulation de partie
			switchRecapServer( player, code2 );
			break;

		default:
			cerr << endl << "Reception d'un paquet PROXYPLAYER inconnu";
			break;
		}
	}
}

bool CServer::acceptPlayer( CSPA *spa )
{
TRACE().p( TRACE_RESEAU, "CServer::acceptPlayer(spa=%x) begin%T", spa, this );

	bool result = true;
	char nomNewPlayer[50];
	CPlayer *player, *player2;

	player = new CPlayer();							// Crée le réceptacle du nouveau joueur
	player->changeAction( gravitePlayer );			// Associe au joueur une fonction de gravité
	player->changeContact( JktMoteur::contactPlayer );	// Associe une fonction de gestion des contacts avec la map
	spa->readChar( nomNewPlayer );					// Récupère le nom du nouveau joueur
	player->nom( nomNewPlayer );					// Enregistre le nom du nouveau joueur
	player->init();

			// Ouverture d'un lien SPA et création du joueur
	result = player->spa.open( spa->getPacketIn()->address );

	if( result )
	{
		if( SDLNet_UDP_AddSocket( socketSet, player->spa.getSocket() )==-1 )
		{
TRACE().p( TRACE_ERROR, "CServer::acceptPlayer() : %s%T", SDLNet_GetError(), this );
			cout << endl << "SDLNet_AddSocket : " << SDLNet_GetError();

			player->spa.close();

			delete player;

			result = false;
		}
	}

	int IDPlayer = AjoutePlayer( player );	// Ajoute le joueur pour de bon et identifie-le
	Game.Erwin( player );			// Place le nouveau joueur comme joueur principal
		//////////////////////////////////////////////////////////////////////////////////////
		// Dis au client qu'il est accepté dans le jeu et donne-lui les infos de la partie	//
		//////////////////////////////////////////////////////////////////////////////////////
	player->spa.init();
	player->spa.addCode( SERVER_JTG, SERVER_ACK );		// Acquitte la jonction du joueur à la partie
	player->spa.add16( (Uint16)IDPlayer );				// Envoie son identifiant au nouveau proxy-joueur
	player->spa.add( nomMAP );				// Envoie le nom de la MAP en cours
	player->spa.add16( (Uint16)maxPlayers );			// Envoie le nombre max de joueurs sur la MAP
	player->spa.add16( (Uint16)this->m_uNbrPlayers );	// Envoie le nombre de joueurs en cours

	// Envoie les infos concernant chaque joueur
	int curseur = -1;

	while(Game.pTabIndexPlayer->Suivant(curseur)) {
		player2 = GetPlayer( curseur );

		player->spa.add16( (Uint16)curseur );			// Envoie l'identifiant de ce proxy-joueur
		player->spa.add( player2->nom() );	// Envoie son nom

		player->spa.addRecapFromServer( *player2 );
	}

	player->spa.send();

TRACE().p( TRACE_RESEAU, "CServer::acceptPlayer() -> %b end%T", result, this );
	return result;	// OK
}

void CServer::switchRecapServer( CPlayer *player, Uint16 code2 )
{
	Uint16 flags;

	switch( code2 )
	{
	case CLIENT_NULL:
		flags = player->spa.read16();

		player->getClavier()->m_fAvance = 0.0f;
		player->getClavier()->m_fDroite = 0.0f;
		player->getClavier()->m_fMonte = 0.0f;

		if( flags&0x0001 )	// S'il y a des requêtes
		{
			player->getClavier()->m_bIndic = true;	// Indique la présence d'une requête

			if( flags&0x0010 )	// Y a-t-il une requête de mouvement en avant
			{
				player->getClavier()->m_fAvance = player->spa.readf();
			}

			if( flags&0x0100 )	// Y a-t-il une requête de mouvement à droite
			{
				player->getClavier()->m_fDroite = player->spa.readf();
			}

			if( flags&0x1000 )	// Y a-t-il une requête de mouvement vers le haut
			{
				player->getClavier()->m_fMonte = player->spa.readf();
			}
		}
		else
		{
			player->getClavier()->m_bIndic = false;
		}

		player->Phi( player->spa.readf() );
		player->Teta( player->spa.readf() );


		break;
	default:
		cerr << endl << "Reception d'un paquet RECAP inconnu";
		break;
	}
}

void CServer::switchInfo( CSPA *spa, Uint16 code2 )	// Réception d'une demande d'info serveur
{
TRACE().p( TRACE_RESEAU, "CServer::switchInfo(spa=%x,code2=%d) begin%T", spa, code2, this );
	switch( code2 )
	{
	case CLIENT_NULL:
		cerr << endl << "Reception d'une demande d'info serveur";
		spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );
		if( spa->getPacketOut()->channel==-1 )
		{
TRACE().p( TRACE_ERROR, "CServer::switchInfo() %s%T", SDLNet_GetError(), this );
			cerr << endl << "SDLNet_UDP_Bind : " << SDLNet_GetError();
			cerr << endl << "Reponse info serveur impossible";
		}
		else
		{
TRACE().p( TRACE_INFO, "CServer::switchInfo() : Reponse info%T", this );
				// Envoie des infos serveur au client
			spa->init();
			spa->addCode( SERVER_INFO, SERVER_ACK );
			spa->add( nom );
			spa->add( nomMAP );
			spa->send();
		}
		break;

	default:
TRACE().p( TRACE_ERROR, "CServer::switchInfo() : Packet inconnu%T", this );
		cerr << endl << "Reception d'un paquet inconnu 1";
		break;
	}
TRACE().p( TRACE_RESEAU, "CServer::switchInfo() end%T", this );
}

void CServer::switchPing( CSPA *spa, Uint16 code2 )	// Réception d'une demande de ping
{
TRACE().p( TRACE_RESEAU, "CServer::switchPing(spa=%x,code2=%d) begin%T", spa, code2, this );
	switch( code2 )
	{
	case CLIENT_NULL:
		if( spa->getPacketIn()->len==8 )
		{
			cout << endl << "Reception d'une demande de ping";
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );
			if( spa->getPacketOut()->channel==-1 )
			{
TRACE().p( TRACE_ERROR, "CServer::switchPing() : %s%T", SDLNet_GetError(), this );
				cerr << endl << "SDLNet_UDP_Bind : " << SDLNet_GetError();
				cerr << endl << "Reponse ping impossible";
			}
			else
			{
TRACE().p( TRACE_INFO, "CServer::switchPing() : Envoi reponse ping%T", this );
					// Envoie de la réponse au ping
				Uint32 time;
				cout << endl << "Emmission de la reponse au ping";
				time = spa->read32();

				spa->init();
				spa->addCode( SERVER_PING, SERVER_ACK );
				spa->add32( time );
				spa->send();
			}
		}
		break;

	default:
TRACE().p( TRACE_ERROR, "CServer::switchPing() : Packet inconnu%T", this );
		cerr << endl << "Reception d'un paquet inconnu 2";
		break;
	}
TRACE().p( TRACE_RESEAU, "CServer::switchPing() end%T", this );
}

void CServer::switchJTG( CSPA *spa, Uint16 code2 )	// Réception d'une demande de JTG
{
TRACE().p( TRACE_RESEAU, "CServer::switchJTG(spa=%x,code2=%d) begin%T", spa, code2, this );
	switch( code2 )
	{
	case CLIENT_NULL:
		cout << endl << "Reception de demande a se joindre a la partie";
		if( m_Statut!=JKT_STATUT_SERVER_PLAY )	// S'il n'y a pas de partie en cours
		{
			spa->getPacketOut()->channel = SDLNet_UDP_Bind( spa->getSocket(), -1, &spa->getPacketIn()->address );
			if( spa->getPacketOut()->channel==-1 )
			{
TRACE().p( TRACE_ERROR, "CServer::switchJTG() SDLNet_UDP_Bind : %s%T", SDLNet_GetError(), this );
				cerr << endl << "SDLNet_UDP_Bind : " << SDLNet_GetError();
				cerr << endl << "Reponse ping impossible";
			}
			else
			{
TRACE().p( TRACE_INFO, "CServer::switchJTG() : Pas de partie en cours%T", this );
					// Envoie d'une erreur signalant qu'il n'y a pas de partie en cours
				cout << endl << "Demande refusee, pas de partie en cours";
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
			if( spa->getPacketOut()->channel==-1 )
			{
TRACE().p( TRACE_ERROR, "CServer::switchJTG() SDLNet_UDP_Bind : %s%T", SDLNet_GetError(), this );
				cerr << endl << "SDLNet_UDP_Bind : " << SDLNet_GetError();
				cerr << endl << "Reponse ping impossible";
			}
			else
			{
TRACE().p( TRACE_INFO, "CServer::switchJTG() : Serveur sature%T", this );
				cout << endl << "Serveur saturé";
				spa->init();
				spa->addCode( SERVER_JTG, SERVER_ERROR );
				spa->add16( UDP_ERROR_SATURE );
				spa->send();
				SDLNet_UDP_Unbind( spa->getSocket(), spa->getPacketOut()->channel );
			}
			break;
		}

		if( acceptPlayer( spa ) )		// Ajoute le client associé à spa en tant que joueur
			cout << endl << "Demande acceptée";
		else
			cerr << endl << "Demande infructueuse";
		break;
	default:
TRACE().p( TRACE_ERROR, "CServer::switchJTG() : Packet inconnu%T", this );
		cerr << endl << "Reception d'un paquet inconnu 3";
		break;
	}
TRACE().p( TRACE_RESEAU, "CServer::switchJTG() end%T", this );
}

bool CServer::ouvre( Uint16 port )
{
TRACE().p( TRACE_RESEAU, "CServer::ouvre(port=%d) begin%T", port, this );

	bool result = true;
	cout << endl << "Ouverture d'un serveur sur port " << port;

	if( !spaMaitre.open( port ) )
	{
TRACE().p( TRACE_ERROR, "CServer::ouvre() : %s %T", SDLNet_GetError(), this );
		result = false;
	}

	if( result )
	{
		socketSet = SDLNet_AllocSocketSet( 20 );	// Nombre maxi de sockets à écouter
		if( !socketSet )
		{
TRACE().p( TRACE_ERROR, "CServer::ouvre() : %s %T", SDLNet_GetError(), this );
cerr << endl << "SDLNet_AllocSocketSet: " << SDLNet_GetError();

			spaMaitre.close();

			result = false;
		}
	}

	if( result )
	{
		if( SDLNet_UDP_AddSocket( socketSet, spaMaitre.getSocket() )==-1 )
		{
TRACE().p( TRACE_ERROR, "CServer::ouvre() : %s %T", SDLNet_GetError(), this );
cerr << endl << "SDLNet_AddSocket : " << SDLNet_GetError();
			SDLNet_FreeSocketSet( socketSet );		// Libère le socket set du serveur
			socketSet = 0;

			spaMaitre.close();

			result = false;
		}
	}

TRACE().p( TRACE_RESEAU, "CServer::ouvre() -> %b end%T", result, this );
	return result;	// Ouverture serveur réussie
}

void CServer::emet()
{
	CPlayer *player, *player2;
	int curseur = -1, curseur2;

	while(Game.pTabIndexPlayer->Suivant(curseur)) {	// Envoie à chaque client-joueur
		player = GetPlayer( curseur );

		if(player) {
			player->spa.init();
			player->spa.addCode( SERVER_RECAP, SERVER_NULL );		// Code de récapitulation
			player->spa.add16( (Uint16)nbrPlayers() );				// Envoie le nombre de joueur

				// Envoie les infos concernant chaque joueur
			curseur2 = -1;

			while(Game.pTabIndexPlayer->Suivant(curseur2)) {
				player2 = GetPlayer( curseur2 );

				if(player2) {
					player->spa.add16( (Uint16)curseur2 );			// Envoie l'identifiant de ce proxy-joueur
					player->spa.addRecapFromServer( *player2 );
				}
			}

			player->spa.send();
		}
	}
}

}	// JTK_PACKAGE_RESEAU
