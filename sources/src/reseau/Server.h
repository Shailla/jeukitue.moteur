
#ifndef __JKT__SERVER_H
#define __JKT__SERVER_H

#include <string>
#include <map>

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/Interlocutor2.h"
#include "reseau/new/ServerUdpInterlocutor.h"

class CPlayer;

namespace jkt
{

class CServer {									// Contient les donn�es courantes du serveur (nombre de joueur, partie en cours,...)
	StatutServer m_Statut;						// Etat du serveur (d�connect�, connect�, partie en cours, ...)
	std::map<IPaddress, CPlayer*> _addressPlayer;	// Mapping entre adresses IP/Port et joueurs connect�s
	std::map<Uint16, CPlayer*> _jetonPlayer;	// Mapping entre adresses IP/Port et joueurs connect�s
public:
	CSPA spaMaitre;
	SDLNet_SocketSet socketSet;		// Les sockets � �couter

	std::string nom;						// Nom du serveur
	std::string nameMap;					// Nom de la MAP ouverte en mode multi-joueurs
	
	bool bGame;						// true si une partie est en cours
	std::string nomMAP;					// Nom du fichier MAP de la partie en cours

	// Serveur moderne
	ServerUdpInterlocutor* _serverUdpInterlocutor;

		// Constructeurs / Destructeur
	CServer();	
	~CServer();

	CPlayer* getPlayerByJeton(const Uint16 jeton);
	void registerPlayerByAddress(const IPaddress& address, CPlayer* player);
	CPlayer* getPlayerByAddress(const IPaddress& address);

	int AjoutePlayer(CPlayer *player);			// Ajoute un proxy-joueur � la partie en cours
	void partieEnCours(bool indic);				// Donne une valeur � bPartieEnCours
	int nbrPlayers();							// Donne le nombre de joueurs sur le serveur
	void setStatut( StatutServer statut);
	StatutServer getStatut();

	void decodeServerUDP( CSPA *spa );						// D�code les paquets ne provenant pas des joeurs actuels
	void decodeProxyPlayer(CPlayer *player, CSPA* spa); 	// D�code les paquets provenant des diff�rents joueurs
	bool acceptPlayer( CSPA *spa );							// Accepte un nouveau joueur dans la partie

	NotConnectedInterlocutor2* connect(Uint16 port, Uint16 portTree);
	void disconnect();
	void emet();

private:
	void switchRecapServer(CPlayer *player,Uint16 code2, CSPA* spa);
	void switchInfo(CSPA *spa,Uint16 code2);
	void switchPing(CSPA *spa,Uint16 code2);
	void switchJTG(CSPA *spa,Uint16 code2);
};

}	// JktNet

#endif

