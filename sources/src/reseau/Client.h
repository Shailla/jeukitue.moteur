

#ifndef __JKT__CLIENT_H
#define __JKT__CLIENT_H

#include "SDL.h"
#include "SDL_net.h"
#include "reseau/enumReseau.h"
#include "reseau/SPA.h"
#include "reseau/new/ClientUdpInterlocutor.h"

class CPlayer;

namespace JktNet
{

class CClient {
public:
	class CInfoServer {
		bool m_bReady;						// Informe si ces infos ont �t� actualis�es depuis la derni�re requ�te d'infos
	public:
		string nom;						// Nom du serveur
		string map;						// Nom de la MAP jou�e sur le serveur
		CInfoServer();
		CInfoServer &operator=( CInfoServer &infoServer );
		void Ready( bool ready );
		bool Ready();
	};

private:
	Uint32 m_timePingClientServer;	// Temps � l'envoie d'un ping du client vers le serveur
	int m_pingClientServer;			// Temps qu'� mis le dernier ping client->serveur

	CInfoServer m_InfoServer;		// Map en cours, nom du serveur, ...

	unsigned int m_uNbrPlayers;				// Nombre de joueurs sur le serveur

	// Serveur moderne
	ClientUdpInterlocutor* _clientUdpInterlocutor;

public:
	SDLNet_SocketSet _socketSet;		// Les sockets � �couter
	CSPA _spaMaitre;
	StatutClient _statut;				// Etat du client (demande de connexion envoy�e, connect�, ...)
	int IDpersonnel;					// Identifiant du joueur (sur ce PC)
	Uint16 _jeton;

	string nomMAP;						// Nom de la MAP en cours

		// Constructeurs/destructeurs
	CClient();
	~CClient();

	Interlocutor2* connect(const string &address, Uint16 port, Uint16 portTree);
	void disconnect();

	bool sendNotConnectedRequestInfoToServer(const string& destinationAddress, Uint16 destinationPort);

	bool sendNotConnectedRequestPingToServer(const string& destinationAddress, Uint16 destinationPort);

	void sendConnectedRequestJoinTheGame(const string& nomPlayer );

	ClientUdpInterlocutor* getClientUdpInterlocutor();

	void setStatut(StatutClient statut);		// Renseigne l'�tat du client (d�connect�,connect�, partie en cours...)
	StatutClient getStatut();					// Retourne l'�tat du client

	unsigned int nbrPlayers();					// Donne le nombre de joueurs sur la MAP en cours
	void nbrPlayers(unsigned int nbr);			// Impl�mente le nombre de joueurs sur la MAP en cours

	CInfoServer getInfoServer();
	int getPingClientServeur();

	void emet( CPlayer &player );
	void recoit();
	void decodeConnecte( Uint16, Uint16 ); 		//D�code les paquets recus du r�seau
	bool decodeNonConnecte( Uint16 code1, Uint16 code2 );

private:
	void decodeRecap( Uint16 code2 );		// Gestion des msg de r�capitulation de partie
	void decodeError( Uint16 code2 );		// Gestion des msg d'erreur
};

}	// JktNet

#endif

