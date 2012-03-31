#ifndef __JKT__UDPCONTROLLER_H
#define __JKT__UDPCONTROLLER_H

#include <string>
using namespace std;

#include "SDL_net.h"

class UdpConnector;

class UdpController
{
    UdpConnector* m_udpConnector;
public:
	enum {
		// Messages descendant du serveur centralisateur vers le moteur (entrants)
		CODE_D_PingRequest = 100,	// Demande d'un ping du serveur centralisateur
		CODE_D_ListJoueurs = 101,	// Liste des joueurs connectés
		CODE_D_ChatMessage = 102,	// Message de chat descendant du serveur centralisateur vers le moteur

		// Messages remontant du moteur vers le serveur centralisateur (sortants)
		CODE_U_Signalement = 200,	// Signalement de présence au serveur centralisateur
		CODE_U_PingAck = 201,		// Réponse à une demande de ping du serveur centralisateur
		CODE_U_ListRequest = 202,	// Demande de la liste des connectés
		CODE_U_ChatMessage = 203	// Message de chat montant du moteur vers le serveur centralisateur
	};

    UdpController(void);
    ~UdpController(void);

    void connect(char* userName, int portLocal, char* ipCentralisateur, int portCentralisateur);
    void sendSignalement(const string& userName);
	void sendMessageChat(const string& message, const string& userName);
    void receive(UDPpacket* packet);
};

#endif
