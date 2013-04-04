
#ifndef __JKT__CRESEAU_H
#define __JKT__CRESEAU_H

#include "enumReseau.h"

/* Remarque, le protocole est comme suit par exemple :
	- UDP_INFO, NULL : demande les infos serveur
	- UDP_ACK, UDP_INFO  : acquitte et transmet les infos du serveur
	- UDP_ERROR, code d'erreur, "texte explicatif" : informe d'une erreur
	- UDP_PING, NULL : Demande un ping
	- UDP_PING, UDP_ACK : Réponse à un ping
*/

namespace JktNet
{

class CReseau {
		// Variables membres 
	CServer *m_Server;
	CClient *m_Client;

	bool m_On;						// Indique si le réseau peut être utilisé ou non

public:

		// Constructeurs / Destructeur
	CReseau();		
	~CReseau();

	void setOn( bool on );					// Indique si le réseau peut être utilisé ou non
	bool getOn();							// Est-ce que le réseau peut être utilisé ou non ?


	/* ********************************************************
	 * Gestion réseau pour le serveur
	 * *******************************************************/

	CServer *getServer();

	bool ouvreServer( Uint16 port );		// Connecte le serveur
	void fermeServer();						// Déconnecte le serveur

	void recoitServer();					// Recoit tous les paquets


	/* ********************************************************
	 * Gestion réseau pour le client
	 * *******************************************************/

	CClient *getClient();

	bool ouvreClient();						// Connecte le client au serveur
	void fermeClient();						// Déconnecte le client du serveur

	void setStatutClient( StatutClient statut );
	StatutClient getStatutClient() const;

	void recoitClient();					// Recoit tous les paquets


	void sendJoinTheGameFromClient();					// Joins la partie en cours sur le serveur
	void sendPingClientServer();						// Envoie d'un ping du client vers le serveur
	int getPingClientServer();							// Récupère la valeur du dernier ping puis efface celle-ci
	void sendRequestInfoServer();						// Demande ses infos au serveur

	CClient::CInfoServer getInfoServer();				// Récupère les dervières info obtenues du serveur
};

}	// JktNet

#endif

