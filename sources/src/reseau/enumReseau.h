
#ifndef __JKT__ENUM_H
#define __JKT__ENUM_H

namespace JktNet
{

enum enteteServer {
	SERVER_NULL = 0,
	SERVER_ACK,					// Acquisition
	SERVER_INFO,				// Demande les informations du serveur
	SERVER_ERROR,				// Erreur
	SERVER_PING,				// Ping
	SERVER_VERSION,				// Version du programme
	SERVER_JTG,					// Demande à joindre la partie acive sur le serveur
	SERVER_RECAP				// Récapitulation de partie
};

enum errorUDP {
	UDP_ERROR_NOGAME,	// Pas de partie en cours sur le serveur
	UDP_ERROR_SATURE,	// Serveur saturé
	UDP_ERROR_VERSION,	// Version du programme du client trop ancienne
};

enum StatutServer {			// Etats du serveur
	JKT_STATUT_SERVER_NULL = 0,
	JKT_STATUT_SERVER_INIT,			// Indique que le serveur est en cours d'initialisation
	JKT_STATUT_SERVER_READY,		// Indique que le serveur est prêt (donc connecté)
	JKT_STATUT_SERVER_PLAY,			// Une partie est en cours sur le serveur
};


enum enteteClient {
	CLIENT_NULL = 0,
	CLIENT_ACK,			// Acquisition
	CLIENT_INFO,		// Demande les informations du serveur
	CLIENT_ERROR,		// Erreur
	CLIENT_PING,		// Ping
	CLIENT_VERSION,		// Version du programme
	CLIENT_JTG,			// Demande à joindre la partie acive sur le serveur
	CLIENT_RECAP,		// Récapitulation de partie
};

enum StatutClient {		// Etats du client
	JKT_STATUT_CLIENT_NULL = 0,
	JKT_STATUT_CLIENT_INIT,			// Le client est en cours d'initialisation
	JKT_STATUT_CLIENT_READY,		// Indique que le serveur est prêt
	JKT_STATUT_CLIENT_DEMJTG,		// Une demande de 'Join The Game' a été envoyée
	JKT_STATUT_CLIENT_OUV,			// Indique que la partie est en cours d'ouverture
	JKT_STATUT_CLIENT_PLAY,			// Indique que la partie est en cours
};

}	// JktNet

#endif

