#include <iostream>
#include <string>

#include "centralisateur/UdpConnector.h"
#include "centralisateur/data/SignalementData.h"
#include "centralisateur/data/SendChatMessageData.h"
#include "main/Fabrique.h"
#include "menu/Viewer.h"
#include "menu/CentralisateurView.h"
#include "menu/ConsoleView.h"

#include "centralisateur/UdpController.h"

UdpController::UdpController(void)
{
    m_udpConnector = 0;
}

UdpController::~UdpController(void)
{
}

void UdpController::connect(char* userName,
                            int portLocal,
                            char* ipCentralisateur,
                            int portCentralisateur)
{
    if(m_udpConnector) {
        m_udpConnector->stop();  // Kill the actual connector and wait he's dead
        delete m_udpConnector;
    }

    m_udpConnector = new UdpConnector(this, portLocal);
    m_udpConnector->connect(ipCentralisateur, portCentralisateur);
}

void UdpController::sendSignalement(const string& userName)
{
    SignalementData data;
    data.setUserName(userName);
    UDPpacket* packet = data.toPacket();
    m_udpConnector->send(packet);
}

void UdpController::sendMessageChat(const string& message, const string& userName)
{
    SendChatMessageData data;
    data.setMessage(message);
	data.setUserName(userName);
    UDPpacket* packet = data.toPacket();
    m_udpConnector->send(packet);
}

void UdpController::receive(UDPpacket* packet)
{
    Uint8* data = packet->data;
	Uint8* current = data;
    
	Uint32 code = SDLNet_Read32(data);
	current += 4;

    // Respond to an Ping request
    if(code == CODE_D_PingRequest) {
        // Remplace le code du paquet de requête par celui d'aquittement
		SDLNet_Write32((Uint32)UdpController::CODE_U_PingAck, data);
		// current += 4;
        m_udpConnector->send(packet);
    }

    // Receive the list of connected users
    else if(code == CODE_D_ListJoueurs) {
		// Nombre de joueurs
		Uint32 nombreJoueurs = SDLNet_Read32(current);
		current +=4;
		char** playerList = new char*[nombreJoueurs];

		for(unsigned int i=0 ; i<nombreJoueurs ; i++) {
		// Taille et nom du joueur qui a envoyé le message
			Uint32 taillePlayerName = SDLNet_Read32(current);
			current +=4;

			playerList[i] = new char[taillePlayerName + 1];
			strncpy( playerList[i], (char*)current, taillePlayerName );
			current += taillePlayerName;
			playerList[i][taillePlayerName] = '\0';
		}

		Viewer* viewer = Fabrique::getAgarView();
		CentralisateurView* view = viewer->getCentralisateurView();
		view->setPlayerList(playerList, nombreJoueurs);
	}

	// Receive a chat message
	else if(code == CODE_D_ChatMessage) {
		// Taille et nom du joueur qui a envoyé le message
		Uint32 taillePlayerName = SDLNet_Read32(current);
		current +=4;

		char* playerName = new char[taillePlayerName + 1];
		strncpy( playerName, (char*)current, taillePlayerName );
		current += taillePlayerName;
		playerName[taillePlayerName] = '\0';

		// Taille et contenu du message
		Uint32 tailleMessage = SDLNet_Read32(current);
		current +=4;

		char* message = new char[tailleMessage + 1];
		strncpy( message, (char*)current, tailleMessage );
		message[tailleMessage] = '\0';

		Viewer* viewer = Fabrique::getAgarView();

		// Show the chat message in the centralisateur window
		CentralisateurView* centralisateurView = viewer->getCentralisateurView();
		centralisateurView->addChatMessage(playerName, message);

		// Show the chat message in the console window
		ConsoleView* consoleView = viewer->getConsoleView();
		consoleView->addChatMessage(playerName, message);
	}

    // Unknown code
    else {
    }
}
