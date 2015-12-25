
#ifndef __JKT__ENUM_H
#define __JKT__ENUM_H

namespace JktNet
{

enum enteteServer {
	// Ent�tes g�n�raux
	GLOBAL_NULL = 0,
	GLOBAL_JETON,

	// Ent�tes serveur
	SERVER_ACK,			// Acquisition
	SERVER_INFO,		// Demande les informations du serveur
	SERVER_ERROR,		// Erreur
	SERVER_PING,		// Ping
	SERVER_VERSION,		// Version du programme
	SERVER_JTG,			// Demande � joindre la partie acive sur le serveur
	SERVER_RECAP,		// R�capitulation de partie

	// Ent�te client
	CLIENT_ACK,			// Acquisition
	CLIENT_INFO,		// Demande les informations du serveur
	CLIENT_ERROR,		// Erreur
	CLIENT_PING,		// Ping
	CLIENT_VERSION,		// Version du programme
	CLIENT_JTG,			// Demande � joindre la partie acive sur le serveur
	CLIENT_RECAP,		// R�capitulation de partie
};

enum errorUDP {
	UDP_ERROR_NOGAME,	// Pas de partie en cours sur le serveur
	UDP_ERROR_SATURE,	// Serveur satur�
	UDP_ERROR_VERSION,	// Version du programme du client trop ancienne
};

enum StatutServer {			// Etats du serveur
	JKT_STATUT_SERVER_NULL = 0,
	JKT_STATUT_SERVER_INIT,			// Indique que le serveur est en cours d'initialisation
	JKT_STATUT_SERVER_READY,		// Indique que le serveur est pr�t (donc connect�)
	JKT_STATUT_SERVER_PLAY,			// Une partie est en cours sur le serveur
};

enum StatutClient {		// Etats du client
	JKT_STATUT_CLIENT_NULL = 0,
	JKT_STATUT_CLIENT_INIT,			// Le client est en cours d'initialisation
	JKT_STATUT_CLIENT_READY,		// Indique que le serveur est pr�t
	JKT_STATUT_CLIENT_DEMJTG,		// Une demande de 'Join The Game' a �t� envoy�e
	JKT_STATUT_CLIENT_OUV,			// Indique que la partie est en cours d'ouverture
	JKT_STATUT_CLIENT_PLAY,			// Indique que la partie est en cours
};

}	// JktNet

#endif

