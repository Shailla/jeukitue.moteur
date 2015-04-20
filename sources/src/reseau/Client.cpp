
#include <fstream>
#include <set>
#include <map>

using namespace std;

#include "main/Fabrique.h"
#include "util/Erreur.h"
#include "util/V3D.h"
#include "util/TableauIndex.cpp"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "reseau/Client.h"
#include "main/RequeteProcess.h"
#include "util/Trace.h"
#include "spatial/contact.h"
#include "main/Clavier.h"
#include "main/Game.h"
#include "menu/MultijoueursClientView.h"

#include "enumReseau.h"
#include "reseau/Client.h"

extern CGame Game;

namespace JktNet
{

CClient::CClient() {
	_socketSet = 0;
	m_uNbrPlayers = 0;
	_statut = JKT_STATUT_CLIENT_INIT;
	m_pingClientServer = 0;
	m_timePingClientServer = 0;
	IDpersonnel = 0;
	_clientUdpInterlocutor = 0;

	_spaMaitre.openInServerMode(0);
}

CClient::~CClient() {
	TRACE().debug("CClient::~CClient() begin%T", this);
	disconnect();
	TRACE().debug("CClient::~CClient() end%T", this);
}

void CClient::setStatut( StatutClient statut ) {
	_statut = statut;
}

StatutClient CClient::getStatut() {
	return _statut;
}

void CClient::nbrPlayers(unsigned int nbr) {
	m_uNbrPlayers = nbr;
}

ClientUdpInterlocutor* CClient::getClientUdpInterlocutor() {
	return _clientUdpInterlocutor;
}

void CClient::decodeConnecte( Uint16 code1, Uint16 code2 ) {
	switch( code1 ) {
	case SERVER_RECAP:		// Réception d'une récapitulation de partie du serveur
		decodeRecap( code2 );
		break;

	case SERVER_ERROR:		// Réception d'un code d'erreur
		decodeError( code2 );
		break;

	default:
		cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet clientUDP inconnu 7";
		break;
	}
}

void CClient::decodeError( Uint16 code2 ) {
	switch( code2 ) {
	case UDP_ERROR_NOGAME:
		TRACE().debug("CClient::decodeError() Pas de partie en cours sur le serveur%T", this );
		cout << endl << "Pas de partie en cours sur le serveur";
		break;

	case UDP_ERROR_SATURE:
		TRACE().debug("CClient::decodeError() Serveur sature%T", this );
		cout << endl << "Serveur sature";
		break;

	default:
		TRACE().debug("CClient::decodeError() Reception d'un paquet ERROR inconnu%T", this );
		cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet ERROR inconnu";
		break;
	}
}

void CClient::decodeRecap( Uint16 code2 ) {
	switch( code2 ) {
	case SERVER_NULL:
		if( _statut==JKT_STATUT_CLIENT_PLAY ) {
			Uint16 var1;
			var1 = _spaMaitre.read16();	// Réception du nombre de joueurs

			if( m_uNbrPlayers!=var1 ) {
				TRACE().debug("CClient::switchRecepClient() Le nombre de joueurs a change%T", this );
				cerr << endl << __FILE__ << ":" << __LINE__ << " LE JEU N'EST PLUS PERTINENT, le nombre de joueurs a change !";
				nbrPlayers( var1 );
			}
			else {
				// Lit les données des joueurs
				CPlayer *player;
				int curseur = -1;
				while((player = Game.nextPlayer(curseur))) {
					var1 = _spaMaitre.read16();		// Identifiant du joueur

					if( var1!=curseur ) {
						TRACE().debug("CClient::switchRecepClient() Les joueurs ont change%T", this);
						cout << endl << "LE JEU N'EST PLUS PERTINENT, les joueurs ont change !";
					}

					_spaMaitre.readRecapFromServer( *player );
				}
			}
		}
		break;

	default:
		cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet RECAP inconnu";
		break;
	}
}

Interlocutor2* CClient::connect(const string& remAddress, Uint16 remPort, Uint16 remPortTree) {
	disconnect();

	cout << endl << " Ouverture client : " << remAddress << " port=" << remPort << " portArbre=" << remPortTree;

	bool result = true;		// Indique si au fur de la fonction si tout s'est bien passé
	Interlocutor2* interlocutor;

	if(!_spaMaitre.openInClientMode(remAddress, remPort))
		result = false;

	if(result) {
		_socketSet = SDLNet_AllocSocketSet( 20 );	// Nombre maxi de sockets à écouter
		if( !_socketSet ) {
			cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_AllocSocketSet: " << SDLNet_GetError();
			disconnect();
			result = false;
		}
	}

	if(result) {
		if( SDLNet_UDP_AddSocket( _socketSet, _spaMaitre.getSocket() )==-1 ) {
			cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_AddSocket : " << SDLNet_GetError();
			disconnect();
			result = false;
		}
	}

	/* ***********************************************************************
	 * Ouverture d'un client moderne
	 * **********************************************************************/

	if(result) {
		_clientUdpInterlocutor = new ClientUdpInterlocutor("Client", 0);
		interlocutor = _clientUdpInterlocutor->connect(remAddress, remPortTree);
	}

	if(interlocutor)
		setStatut( JKT_STATUT_CLIENT_READY );	// Indique que le client est prêt

	return interlocutor;
}

void CClient::disconnect() {
	if(_socketSet) {
		SDLNet_FreeSocketSet( _socketSet );		// Libère le socket set du serveur
		_socketSet = 0;
	}

	_spaMaitre.close();

	setStatut( JKT_STATUT_CLIENT_NULL );	// Indique que le client est prêt

	if(_clientUdpInterlocutor) {
		delete _clientUdpInterlocutor;
		_clientUdpInterlocutor = 0;
	}
}

bool CClient::sendNotConnectedRequestInfoToServer(const string& destinationAddress, Uint16 destinationPort) {
	bool result = true;
	IPaddress destination;

	if(SDLNet_ResolveHost(&destination, destinationAddress.c_str(), destinationPort)) {
		cerr << endl << __FILE__ << ":" << __LINE__ << SDLNet_GetError();
		result = false;
	}

	if(result) {
		m_InfoServer.Ready( false );	// Indique l'attente d'actualisation de ces infos

		_spaMaitre.init();
		_spaMaitre.addCode( CLIENT_INFO, CLIENT_NULL );

		cout << endl << "Sending info request to server '" << destinationAddress << ":" << destinationPort << "'";
		result = _spaMaitre.send(destination);
	}

	if(result) {
		cout << endl << "Info request sent to '" << destinationAddress << ":" << destinationPort << "'";
	}
	else {
		cerr << endl << "Error sending info request to '" << destinationAddress << ":" << destinationPort << "'";
	}

	return result;
}

bool CClient::sendNotConnectedRequestPingToServer(const string& destinationAddress, Uint16 destinationPort) {
	bool result = true;
	IPaddress destination;

	if(SDLNet_ResolveHost(&destination, destinationAddress.c_str(), destinationPort)) {
		cerr << endl << __FILE__ << ":" << __LINE__ << SDLNet_GetError();
		result = false;
	}

	if(result) {
		m_pingClientServer = -1;					// Initialisation de la durée du ping
		m_timePingClientServer = SDL_GetTicks();	// Temps à l'envoie du ping

		_spaMaitre.init();
		_spaMaitre.addCode( CLIENT_PING, CLIENT_NULL );
		_spaMaitre.add32( m_timePingClientServer );

		cout << endl << "Sending ping request to server '" << destinationAddress << ":" << destinationPort << "'";
		result = _spaMaitre.send(destination);
	}

	if(result) {
		cout << endl << "Ping request sent to '" << destinationAddress << ":" << destinationPort << "'";
	}
	else {
		cerr << endl << "Error sending ping request to '" << destinationAddress << ":" << destinationPort << "'";
	}

	return result;
}

void CClient::sendConnectedRequestJoinTheGame(const string& nomPlayer) {
	setStatut( JKT_STATUT_CLIENT_DEMJTG );	// Statut, demande de "Join The Game" envoyée

	_spaMaitre.init();
	_spaMaitre.addCode( CLIENT_JTG, CLIENT_NULL );	// Envoie la demande à joindre la partie
	_spaMaitre.add( nomPlayer );	// Envoie nom du joueur qui veut s'y joindre

	setStatut( JKT_STATUT_CLIENT_DEMJTG );	// Statut, demande de "Join The Game" envoyée,
	// en attente de réponse du serveur

	if( !_spaMaitre.send() ) {
		TRACE().debug("CClient::sendJoinTheGame() SDL_UDP_Send : %s%T", SDLNet_GetError(), this );
	}
}

void CClient::recoit() {
	int numReady;
	Uint16 code1, code2;

	numReady = SDLNet_CheckSockets( _socketSet, 0 );		// Nombre de sockets ayant une activité détectée

	if( numReady==-1 ) {
		cout << "SDLNet_CheckSockets: " << SDLNet_GetError();
	}
	else if( numReady ) {
		if( SDLNet_SocketReady( _spaMaitre.getSocket() ) ) {
			if( _spaMaitre.recoit() ) {
				_spaMaitre.init();
				_spaMaitre.readCode( code1, code2 );

				bool isNotConnectedMessage = decodeNonConnecte( code1, code2 );			// Tente de décoder les paquets en mode déconnecté

				if(!isNotConnectedMessage)
					if( getStatut()==JKT_STATUT_CLIENT_PLAY )	// Si une partie est en cours
						decodeConnecte( code1, code2 );			// Décode les paquest en mode connecté
			}
		}
	}
}

int CClient::getPingClientServeur() {
	int ping = m_pingClientServer;

	if( ping==-1 ) {		// Pas de valeur de ping à fournir
		if(m_timePingClientServer > 0) {	// Si un ping a été envoyé
			if( SDL_GetTicks()-m_timePingClientServer>9999 ) {	// S'il a été envoyé il y a trop longtemps
				m_pingClientServer = -1;	// Oublie ce ping
				ping = 9999;			// Signale un temps de ping long
			}
		}
	}
	else {
		m_pingClientServer = -1;
	}

	return ping;
}

bool CClient::decodeNonConnecte(Uint16 code1, Uint16 code2) {
	bool result = false;
	Uint16 code3;

	switch( code1 ) {
	case SERVER_INFO:
		switch( code2 ) {
		case SERVER_ACK:
			if( _spaMaitre.getPacketIn()->len>5 ) {
				char txt[50];
				TRACE().debug("CClient::decodeNonConnecte() : Info serveur%T", this);
				cout << endl << "Reponse a la demande d'info serveur recue";
				_spaMaitre.readChar( txt );				// Réception nom du serveur
				m_InfoServer.nom = txt;
				_spaMaitre.readChar( txt );				// Réception de la map en cours
				m_InfoServer.map = txt;
				m_InfoServer.Ready( true );			// Indique que les infos sont actualisées

				MultijoueursClientView* view = (MultijoueursClientView*)Fabrique::getAgarView()->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
				view->setServerName(m_InfoServer.nom);
				view->setActiveMap(m_InfoServer.map);
			}
			result = true;
			break;

		default:
			TRACE().debug("CClient::decodeNonConnecte() : Packet inconnu 5%s%T",
					_spaMaitre.debugToString().c_str(), this );
			result = false;
			break;
		}
		result = false;
		break;

		case SERVER_PING:
			switch( code2 ) {
			case SERVER_ACK:
				if( _spaMaitre.getPacketIn()->len==8 ) {
					TRACE().debug("CClient::decodeNonConnecte() : Reponse ping%T", this );
					cout << endl << "Reponse a un ping recue";
					if( m_timePingClientServer==_spaMaitre.read32() ) {
						m_pingClientServer = SDL_GetTicks() - m_timePingClientServer;

						MultijoueursClientView* view = (MultijoueursClientView*)Fabrique::getAgarView()->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
						view->setServerName(m_InfoServer.nom);
						view->setPing(m_pingClientServer);
					}
				}
				result = true;
				break;
			default:
				TRACE().debug("CClient::decodeNonConnecte() : Packet inconnu 4%s%T", _spaMaitre.debugToString().c_str(), this);
				result = false;
				break;
			}
			break;

			case SERVER_JTG:		// Ca concerne 'joindre la partie active du serveur'
				switch( code2 ) {
				case SERVER_ERROR:
					code3 = _spaMaitre.read16();
					switch( code3 ) {
					case UDP_ERROR_NOGAME:
						TRACE().debug("CClient::decodeNonConnecte() : Pas de partie en cours%T", this);
						cout << endl << "Pas de partie en cours sur le serveur";
						break;

					case UDP_ERROR_SATURE:
						TRACE().debug("CClient::decodeNonConnecte() : Serveur sature%T", this );
						cout << endl << "Serveur sature";
						break;

					default:
						TRACE().debug("CClient::decodeNonConnecte() : Packet inconnu 3%s%T", _spaMaitre.debugToString().c_str(), this);
						cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet ERROR inconnu";
						break;
					}
					result = true;
					break;

					case SERVER_ACK:	// Acceptation recue du serveur pour joindre la partie
						if( getStatut()==JKT_STATUT_CLIENT_DEMJTG ) {
							TRACE().debug("CClient::decodeNonConnecte() : Reponse JTG%T", this);
							cout << endl << "Acceptation recue du serveur pour joindre la partie";


							/* *************************************************************
							 * Sortie de la partie en cours
							 * ************************************************************/

							// Quitte la MAP en cours
							Game.quitCurrentMap();

							// Supprime les joueurs
							Game.Erwin(0);
							Game.deletePlayers();


							/* *************************************************************
							 * Récupération d'informations du serveur
							 * ************************************************************/

							// Détachement du socket du port principal du serveur
							SDLNet_UDP_Unbind( _spaMaitre.getSocket(), _spaMaitre.getPacketOut()->channel );

							// Attachement au port du serveur qui est réservé à ce client
							_spaMaitre.getPacketOut()->channel=SDLNet_UDP_Bind( _spaMaitre.getSocket(),-1,&_spaMaitre.getPacketIn()->address);
							_spaMaitre.getPacketIn()->channel = _spaMaitre.getPacketOut()->channel;
							if( _spaMaitre.getPacketOut()->channel==-1 )
								cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_UDP_Bind : " << SDLNet_GetError();

							Game.setStatutClient( JKT_STATUT_CLIENT_OUV );	// Indique l'ouverture en cours

							IDpersonnel = _spaMaitre.read16();			// Lecture de l'identifiant du joueur
							_spaMaitre.readString( nomMAP );			// Nom de la MAP en cours sur le serveur
							Game.createPlayerList(_spaMaitre.read16());	// Nombre max de joueurs sur le serveur
							nbrPlayers( _spaMaitre.read16() );			// Nombre de joeurs sur le serveur

							cout << endl;
							cout << endl << "Identifiant :\t\t" << IDpersonnel;
							cout << endl << "Nom de la MAP en cours :\t\t" << nomMAP;
							cout << endl << "Nombre de joueurs maxi :\t\t" << Game.getMaxPlayers();
							cout << endl << "Nombre de joueurs sur le serveur :\t" << m_uNbrPlayers;
							cout << endl;


							/* *************************************************************
							 * Construction de la nouvelle liste de joueurs
							 * ************************************************************/

							// Import de la nouvelle liste de joueurs
							CPlayer *player;

							for( unsigned int i=0 ; i<m_uNbrPlayers ; i++) {
								player = new CPlayer();

								// Identifiant du joueur sur le serveur
								Uint16 id = _spaMaitre.read16();
								Game.addPlayer(id, player);

								// Nom du joueur
								string nom_p;
								_spaMaitre.readString( nom_p );
								player->nom(nom_p);

								// Lecture des position, vitesse, Phi et Teta du joueur
								_spaMaitre.readRecapFromServer( *player );
							}

							// Affichage des infos des joueurs
							int curseur = -1;
							float pos[3];

							while((player = Game.nextPlayer(curseur))) {
								cout << endl << curseur;

								if( curseur==IDpersonnel )	// S'il s'agit du joueur de cet ordinateur
									cout << " *";

								cout << "\t" << player->nom();
								player->getPosition( pos );
								cout << "\t" << pos[0] << ";" << pos[1] << ";" << pos[2];
							}


							/* *************************************************************
							 * Attribution du joueur principal
							 * ************************************************************/

							Game.Erwin(Game.getPlayer(IDpersonnel));


							/* *************************************************************
							 * Lancement de l'ouverture de la MAP
							 * ************************************************************/

							Game.RequeteProcess.setOuvreMapClient( nomMAP );
						}
						else
						{
							TRACE().debug("CClient::decodeNonConnecte() : Reponse JKT sans demande%T", this);
							cerr << endl << __FILE__ << ":" << __LINE__ << " Acceptation JTG recue sans demande envoyée";
						}
						result = true;	// Le paquet a été utilisé
						break;

					default:
						TRACE().debug("CClient::decodeNonConnecte() : Packet inconnu %s%T", _spaMaitre.debugToString().c_str(), this);
						cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet inconnu 7";
						result = false;
						break;
				}
				break;

				default:
					result = false;
					break;
	}

	return result;
}

CClient::CInfoServer CClient::getInfoServer() {
	return m_InfoServer;
}

CClient::CInfoServer::CInfoServer() {
	m_bReady = false;
}

CClient::CInfoServer &CClient::CInfoServer::operator=( CInfoServer &infoServer ) {
	m_bReady = infoServer.Ready();	// Informe si ces infos ont été actualisées depuis la dernière requête d'infos
	nom = infoServer.nom;		// Nom du serveur
	map = infoServer.map;

	return *this;
}

void CClient::CInfoServer::Ready( bool ready ) {
	m_bReady = ready;
}

bool CClient::CInfoServer::Ready() {
	return m_bReady;
}

void CClient::emet( CPlayer &player ) {
	CClavier *clavier = player.getClavier();

	_spaMaitre.init();
	_spaMaitre.addCode( CLIENT_RECAP, CLIENT_NULL );		// Code de récapitulation

	// Déplacements
	Uint16 flags = 0;

	if( clavier->m_bIndic ) {
		flags |= 0x0001;	// Indique qu'il y a des requetes clavier à prendre en compte

		if( clavier->m_fAvance!=0.0f ) {
			flags |= 0x0010;	// Indique la présence d'une requête de mouvement vers l'avant
		}

		if( clavier->m_fDroite!=0.0f ) {
			flags |= 0x0100;	// Indique la présence d'une requête de mouvement vers la droite
		}

		if( clavier->m_fMonte!=0.0 ) {
			flags |= 0x1000;	// Indique la présence d'une requête de mouvement vers le haut
		}
	}

	_spaMaitre.add16(flags);

	if( clavier->m_bIndic ) {
		if( clavier->m_fAvance!=0.0f ) {
			_spaMaitre.add( clavier->m_fAvance );
		}

		if( clavier->m_fDroite!=0.0f ) {
			_spaMaitre.add( clavier->m_fDroite );
		}

		if( clavier->m_fMonte!=0.0 ) {
			_spaMaitre.add( clavier->m_fMonte );
		}
	}

	// Angles
	_spaMaitre.add( player.Phi() );
	_spaMaitre.add( player.Teta() );

	_spaMaitre.send();
}

}	// JktNet
