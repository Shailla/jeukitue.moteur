
#ifndef __JKT__NETWORKMANAGER_H
#define __JKT__NETWORKMANAGER_H

#include <string>

#include "enumReseau.h"
#include "reseau/new/Interlocutor2.h"
#include "reseau/new/NotConnectedInterlocutor2.h"

/* Remarque, le protocole est comme suit par exemple :
	- UDP_INFO, NULL : demande les infos serveur
	- UDP_ACK, UDP_INFO  : acquitte et transmet les infos du serveur
	- UDP_ERROR, code d'erreur, "texte explicatif" : informe d'une erreur
	- UDP_PING, NULL : Demande un ping
	- UDP_PING, UDP_ACK : Réponse à un ping
*/

namespace JktNet
{

class CServer;
class CClient;

class NetworkManager {
		// Variables membres 
	CServer *_server;
	CClient *_client;

	bool _on;						// Indique si le réseau peut être utilisé ou non

public:

		// Constructeurs / Destructeur
	NetworkManager();		
	~NetworkManager();

	void setOn( bool on );					// Indique si le réseau peut être utilisé ou non
	bool getOn();							// Est-ce que le réseau peut être utilisé ou non ?


	/* ********************************************************
	 * Gestion réseau pour le serveur
	 * *******************************************************/

	CServer *getServer();

	NotConnectedInterlocutor2* ouvreServer(Uint16 serverPort, Uint16 serverTreePort);	// Connecte le serveur
	void fermeServer();							// Déconnecte le serveur
	void recoitServer();						// Recoit tous les paquets

	/* ********************************************************
	 * Gestion réseau pour le client
	 * *******************************************************/

	CClient *getClient();

	Interlocutor2* ouvreClient(const std::string& serverIp, Uint16 serverPort, Uint16 serverTreePort);			// Connecte le client au serveur
	void fermeClient();						// Déconnecte le client du serveur
};

}	// JktNet

#endif

