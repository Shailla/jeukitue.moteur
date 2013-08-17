
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
	TRACE().p( TRACE_RESEAU, "CClient::CClient() begin%T", this );
	socketSet = 0;
	m_uNbrPlayers = 0;
	m_Statut = JKT_STATUT_CLIENT_INIT;
	m_pingClientServer = 0;
	m_timePingClientServer = 0;
	IDpersonnel = 0;

	_clientUdpInterlocutor = 0;
	TRACE().p( TRACE_RESEAU, "CClient::CClient() end%T", this );
}

CClient::~CClient() {
	TRACE().p( TRACE_RESEAU, "CClient::~CClient() begin%T", this );
	ferme();
	TRACE().p( TRACE_RESEAU, "CClient::~CClient() end%T", this );
}

void CClient::setStatut( StatutClient statut ) {
	m_Statut = statut;
}

StatutClient CClient::getStatut() {
	return m_Statut;
}

void CClient::nbrPlayers(unsigned int nbr)
{	m_uNbrPlayers = nbr;	}

void CClient::decodeConnecte( Uint16 code1, Uint16 code2 ) {
	switch( code1 ) {
	case SERVER_RECAP:		// Réception d'une récapitulation de partie du serveur
		decodeRecap( code2 );
		break;

	case SERVER_TREE_DATA_UPDATE:
		//		updateTreeData();
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
		TRACE().p( TRACE_INFO, "CClient::decodeError() Pas de partie en cours sur le serveur%T", this );
		cout << endl << "Pas de partie en cours sur le serveur";
		break;

	case UDP_ERROR_SATURE:
		TRACE().p( TRACE_INFO, "CClient::decodeError() Serveur sature%T", this );
		cout << endl << "Serveur sature";
		break;

	default:
		TRACE().p( TRACE_ERROR, "CClient::decodeError() Reception d'un paquet ERROR inconnu%T", this );
		cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet ERROR inconnu";
		break;
	}
}

void CClient::decodeRecap( Uint16 code2 ) {
	switch( code2 ) {
	case SERVER_NULL:
		if( m_Statut==JKT_STATUT_CLIENT_PLAY ) {
			Uint16 var1;
			var1 = spaMaitre.read16();	// Réception du nombre de joueurs

			if( m_uNbrPlayers!=var1 ) {
				TRACE().p( TRACE_ERROR, "CClient::switchRecepClient() Le nombre de joueurs a change%T", this );
				cerr << endl << __FILE__ << ":" << __LINE__ << " LE JEU N'EST PLUS PERTINENT, le nombre de joueurs a change !";
				nbrPlayers( var1 );
			}
			else {
				// Lit les données des joueurs
				CPlayer *player;
				int curseur = -1;
				while(Game._pTabIndexPlayer->Suivant(curseur)) {
					var1 = spaMaitre.read16();		// Identifiant du joueur

					if( var1!=curseur ) {
						TRACE().p( TRACE_ERROR, "CClient::switchRecepClient() Les joueurs ont change%T", this );
						cout << endl << "LE JEU N'EST PLUS PERTINENT, les joueurs ont change !";
					}

					player = Game._pTabIndexPlayer->operator [](curseur);
					spaMaitre.readRecapFromServer( *player );
				}
			}
		}
		break;

	default:
		TRACE().p( TRACE_ERROR, "CClient::switchRecepClient() Reception d'un paquet RECAP inconnu : %s%T",
				spaMaitre.debugToString().c_str(), this );
		cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet RECAP inconnu";
		break;
	}
}

Interlocutor2* CClient::ouvre(const string &address, Uint16 port, Uint16 portTree) {
	TRACE().p( TRACE_RESEAU, "CClient::ouvre(address=%s,port=%d) begin%T", address.c_str(), port, this );

	cout << endl << __FILE__ << ":" << __LINE__ << " Ouverture client : " << address << ":" << port;

	bool result = true;		// Indique si au fur de la fonction si tout s'est bien passé
	Interlocutor2* interlocutor;

	if(!spaMaitre.open(address, port))
		result = false;

	if(result) {
		socketSet = SDLNet_AllocSocketSet( 20 );	// Nombre maxi de sockets à écouter
		if( !socketSet ) {
			TRACE().p( TRACE_ERROR, "CClient::ouvre() : %s%T", SDLNet_GetError(), this );
			cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_AllocSocketSet: " << SDLNet_GetError();

			ferme();

			result = false;
		}
	}

	if(result) {
		if( SDLNet_UDP_AddSocket( socketSet, spaMaitre.getSocket() )==-1 ) {
			TRACE().p( TRACE_ERROR, "CClient::ouvre() : %s%T", SDLNet_GetError(), this );
			cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_AddSocket : " << SDLNet_GetError();

			ferme();

			result = false;
		}
	}

	/* ***********************************************************************
	 * Ouverture d'un client moderne
	 * **********************************************************************/

	if(result) {
		_clientUdpInterlocutor = new ClientUdpInterlocutor(0);
		interlocutor = _clientUdpInterlocutor->connect(address, portTree);
	}

	if(interlocutor)
		setStatut( JKT_STATUT_CLIENT_READY );	// Indique que le client est prêt

	TRACE().p( TRACE_RESEAU, "CClient::ouvre() -> %b end%T", result, this );
	return interlocutor;
}

void CClient::ferme() {
	if(socketSet) {
		SDLNet_FreeSocketSet( socketSet );		// Libère le socket set du serveur
		socketSet = 0;
	}

	spaMaitre.close();

	setStatut( JKT_STATUT_CLIENT_NULL );	// Indique que le client est prêt

	if(_clientUdpInterlocutor) {
		delete _clientUdpInterlocutor;
		_clientUdpInterlocutor = 0;
	}
}

void CClient::sendRequestInfoToServer() {
	m_InfoServer.Ready( false );	// Indique l'attente d'actualisation de ces infos

	spaMaitre.init();
	spaMaitre.addCode( CLIENT_INFO, CLIENT_NULL );

	if( !spaMaitre.send() )	{	// Acquittement de la réception du packet
		cout << endl << "SDL_UDP_Send : ", SDLNet_GetError();
	}
	else {
		cout << endl << "Demande d'info envoyee au serveur";
	}
}

void CClient::sendPingToServer() {
	m_pingClientServer = -1;	// Initialisation de la durée du ping
	m_timePingClientServer = SDL_GetTicks();	// Temps à l'envoie du ping

	spaMaitre.init();
	spaMaitre.addCode( CLIENT_PING, CLIENT_NULL );
	spaMaitre.add32( m_timePingClientServer );
	if( !spaMaitre.send() )		// Emmission de la demande de ping
		cout << endl << "SDL_UDP_Send : ",SDLNet_GetError();
	else
		cout << endl << "Ping envoye au serveur";
}

void CClient::sendJoinTheGame(string &nomPlayer) {
	TRACE().p( TRACE_RESEAU, "CClient::sendJoinTheGame(nomPlayer=%s) begin%T", nomPlayer.c_str(), this );

	setStatut( JKT_STATUT_CLIENT_DEMJTG );	// Statut, demande de "Join The Game" envoyée

	spaMaitre.init();
	spaMaitre.addCode( CLIENT_JTG, CLIENT_NULL );	// Envoie la demande à joindre la partie
	spaMaitre.add( nomPlayer );	// Envoie nom du joueur qui veut s'y joindre

	setStatut( JKT_STATUT_CLIENT_DEMJTG );	// Statut, demande de "Join The Game" envoyée,
	// en attente de réponse du serveur

	if( !spaMaitre.send() ) {
		TRACE().p( TRACE_ERROR, "CClient::sendJoinTheGame() SDL_UDP_Send : %s%T", SDLNet_GetError(), this );
	}

	TRACE().p( TRACE_RESEAU, "CClient::sendJoinTheGame() end%T", this );
}

void CClient::recoit() {
	int numReady;
	Uint16 code1, code2;

	numReady = SDLNet_CheckSockets( socketSet, 0 );	// Nombre de sockets ayant une activité détectée

	if( numReady==-1 ) {
		cout << "SDLNet_CheckSockets: " << SDLNet_GetError();
	}
	else if( numReady ) {
		if( SDLNet_SocketReady( spaMaitre.getSocket() ) ) {
			if( spaMaitre.recoit() ) {
				spaMaitre.init();
				spaMaitre.readCode( code1, code2 );

				if( !decodeNonConnecte( code1, code2 ) )	// Décode les paquets en mode non connecté
					if( getStatut()==JKT_STATUT_CLIENT_PLAY )	// Si une partie est en cours
						decodeConnecte( code1, code2 );	// Décode les paquest en mode connecté
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
			if( spaMaitre.getPacketIn()->len>5 ) {
				char txt[50];
				TRACE().p( TRACE_INFO, "CClient::decodeNonConnecte() : Info serveur%T", this );
				cout << endl << "Reponse a la demande d'info serveur recue";
				spaMaitre.readChar( txt );				// Réception nom du serveur
				m_InfoServer.nom = txt;
				spaMaitre.readChar( txt );				// Réception de la map en cours
				m_InfoServer.map = txt;
				m_InfoServer.Ready( true );			// Indique que les infos sont actualisées

				MultijoueursClientView* view = (MultijoueursClientView*)Fabrique::getAgarView()->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
				view->setServerName(m_InfoServer.nom);
				view->setActiveMap(m_InfoServer.map);
			}
			result = true;
			break;

		default:
			TRACE().p( TRACE_ERROR, "CClient::decodeNonConnecte() : Packet inconnu 5%s%T",
					spaMaitre.debugToString().c_str(), this );
			result = false;
			break;
		}
		result = false;
		break;

		case SERVER_PING:
			switch( code2 ) {
			case SERVER_ACK:
				if( spaMaitre.getPacketIn()->len==8 ) {
					TRACE().p( TRACE_INFO, "CClient::decodeNonConnecte() : Reponse ping%T", this );
					cout << endl << "Reponse a un ping recue";
					if( m_timePingClientServer==spaMaitre.read32() ) {
						m_pingClientServer = SDL_GetTicks() - m_timePingClientServer;

						MultijoueursClientView* view = (MultijoueursClientView*)Fabrique::getAgarView()->getView(Viewer::MULTIJOUEURS_CLIENT_VIEW);
						view->setServerName(m_InfoServer.nom);
						view->setPing(m_pingClientServer);
					}
				}
				result = true;
				break;
			default:
				TRACE().p( TRACE_ERROR, "CClient::decodeNonConnecte() : Packet inconnu 4%s%T", spaMaitre.debugToString().c_str(), this );
				result = false;
				break;
			}
			break;

			case SERVER_JTG:		// Ca concerne 'joindre la partie active du serveur'
				switch( code2 ) {
				case SERVER_ERROR:
					code3 = spaMaitre.read16();
					switch( code3 ) {
					case UDP_ERROR_NOGAME:
						TRACE().p( TRACE_INFO, "CClient::decodeNonConnecte() : Pas de partie en cours%T", this );
						cout << endl << "Pas de partie en cours sur le serveur";
						break;

					case UDP_ERROR_SATURE:
						TRACE().p( TRACE_INFO, "CClient::decodeNonConnecte() : Serveur sature%T", this );
						cout << endl << "Serveur sature";
						break;

					default:
						TRACE().p( TRACE_ERROR, "CClient::decodeNonConnecte() : Packet inconnu 3%s%T", spaMaitre.debugToString().c_str(), this );
						cerr << endl << __FILE__ << ":" << __LINE__ << " Reception d'un paquet ERROR inconnu";
						break;
					}
					result = true;
					break;

					case SERVER_ACK:	// Acceptation recue du serveur pour joindre la partie
						if( getStatut()==JKT_STATUT_CLIENT_DEMJTG )
						{
							TRACE().p( TRACE_INFO, "CClient::decodeNonConnecte() : Reponse JTG%T", this );
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
							SDLNet_UDP_Unbind( spaMaitre.getSocket(), spaMaitre.getPacketOut()->channel );

							// Attachement au port du serveur qui est réservé à ce client
							spaMaitre.getPacketOut()->channel=SDLNet_UDP_Bind( spaMaitre.getSocket(),-1,&spaMaitre.getPacketIn()->address);
							spaMaitre.getPacketIn()->channel = spaMaitre.getPacketOut()->channel;
							if( spaMaitre.getPacketOut()->channel==-1 )
								cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_UDP_Bind : " << SDLNet_GetError();

							Game.setStatutClient( JKT_STATUT_CLIENT_OUV );	// Indique l'ouverture en cours

							IDpersonnel = spaMaitre.read16();			// Lecture de l'identifiant du joueur
							spaMaitre.readString( nomMAP );				// Nom de la MAP en cours sur le serveur
							Game.setPlayerList(spaMaitre.read16());		// Nombre max de joueurs sur le serveur
							nbrPlayers( spaMaitre.read16() );			// Nombre de joeurs sur le serveur

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
								Uint16 id = spaMaitre.read16();
								Game._pTabIndexPlayer->Ajoute( id, player );

								// Nom du joueur
								string nom_p;
								spaMaitre.readString( nom_p );
								player->nom(nom_p);

								// Lecture des position, vitesse, Phi et Teta du joueur
								spaMaitre.readRecapFromServer( *player );
							}

							// Affichage des infos des joueurs
							int curseur = -1;
							float pos[3];

							while(Game._pTabIndexPlayer->Suivant(curseur)) {
								player = Game._pTabIndexPlayer->operator []( curseur );
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

							Game.Erwin( Game._pTabIndexPlayer->operator []( IDpersonnel ) );


							/* *************************************************************
							 * Lancement de l'ouverture de la MAP
							 * ************************************************************/

							Game.RequeteProcess.setOuvreMap( nomMAP );
						}
						else
						{
							TRACE().p( TRACE_INFO, "CClient::decodeNonConnecte() : Reponse JKT sans demande%T", this );
							cerr << endl << __FILE__ << ":" << __LINE__ << " Acceptation JTG recue sans demande envoyée";
						}
						result = true;	// Le paquet a été utilisé
						break;

					default:
						TRACE().p( TRACE_ERROR, "CClient::decodeNonConnecte() : Packet inconnu %s%T", spaMaitre.debugToString().c_str(), this );
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
	unsigned int position;
	Uint16 flags = 0;

	CClavier *clavier = player.getClavier();

	spaMaitre.init();
	spaMaitre.addCode( CLIENT_RECAP, CLIENT_NULL );		// Code de récapitulation

	position = spaMaitre.getPosition();
	spaMaitre.add16( 0 );	// Ca sert juste à avancer de 16 bits dans le paquet pour laisser
	// une place libre pour la variable flags (voir fin de emetClient)

	if( clavier->m_bIndic ) {
		flags |= 0x0001;	// Indique qu'il y a des requetes clavier à prendre en compte

		if( clavier->m_fAvance!=0.0f ) {
			flags |= 0x0010;	// Indique la présence d'une requête de mouvement vers l'avant
			spaMaitre.add( clavier->m_fAvance );
		}

		if( clavier->m_fDroite!=0.0f ) {
			flags |= 0x0100;	// Indique la présence d'une requête de mouvement vers la droite
			spaMaitre.add( clavier->m_fDroite );
		}

		if( clavier->m_fMonte!=0.0 ) {
			flags |= 0x1000;	// Indique la présence d'une requête de mouvement vers le haut
			spaMaitre.add( clavier->m_fMonte );
		}
	}

	spaMaitre.add( player.Phi() );
	spaMaitre.add( player.Teta() );

	spaMaitre.add16InPosition( position, flags );	// Envoie le filtre des requêtes à prendre en compte

	spaMaitre.send();
}

}	// JktNet
