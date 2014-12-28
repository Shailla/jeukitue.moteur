#include <string>
#include <iostream>

#include "SDL_net.h"

#include "exception/centralisateur/BindingUdpException.h"
#include "exception/centralisateur/OpenUdpSocketException.h"
#include "centralisateur/UdpController.h"
#include "util/Trace.h"
#include "centralisateur/UdpConnector.h"

using namespace std;

/**
    Constructeur
    portLocal : numéro du port d'écoute de la facade
*/
UdpConnector::UdpConnector(UdpController* udpController, const int portLocal) : m_socketSet() {
    m_channel = -1;   // Uninitialized channel value
    m_udpController = udpController;
	m_portLocal = portLocal;
	m_isAlive = false;
	m_portDistant = 0;
	m_thread = NULL;
	m_ipDistant = NULL;
	m_socket = NULL;
}

/**
	Connection à une entité distante
	ip: adresse IP de l'entité distante
	port: port de l'entité distante
*/
void UdpConnector::connect(char* ipDistant, int portDistant) throw (BindingUdpException) {
	m_ipDistant = ipDistant;
	m_portDistant = portDistant;
    m_isAlive = true;
    m_thread = SDL_CreateThread(UdpConnector::runReceive, (void*)this);
	SDL_Delay(1000);
}

UdpConnector::~UdpConnector(void) {
    if(m_socket != 0) {
        SDLNet_UDP_Unbind(m_socket, 0);
        SDLNet_UDP_Close(m_socket);
    }
}

void UdpConnector::send(UDPpacket* packet) {
    SDLNet_UDP_Send(m_socket, m_channel, packet);

    // TODO    delete packet->data;
    // TODO    delete packet;
}

void UdpConnector::stop() {
    m_isAlive = false;
    int threadStatus;
    SDL_WaitThread(m_thread, &threadStatus);
}

int UdpConnector::runReceive(void* udpConnectorPtr) {
    UdpConnector* udpConnector = (UdpConnector*)udpConnectorPtr;
    udpConnector->receive();

    return 0;
}

int UdpConnector::receive() {
	// Ouverture de la socket
	m_socket = SDLNet_UDP_Open(m_portLocal);

    if(!m_socket) {
        const char* message = SDLNet_GetError();
		TRACE().error("SDLNet_UDP_Open: '%s'", message );
        throw OpenUdpSocketException(m_portLocal, message);
    }

	// Gestion de la socket dans un pool de sockets
	m_socketSet = SDLNet_AllocSocketSet(16);

    if(!m_socketSet) {
        const char* message = SDLNet_GetError();
		TRACE().error("SDLNet_AllocSocketSet: '%s'", message );
        throw OpenUdpSocketException(m_portLocal, message);
    }

    int numused = SDLNet_UDP_AddSocket(m_socketSet, m_socket);

    if(numused==-1) {
        const char* message = SDLNet_GetError();
		TRACE().error("SDLNet_UDP_AddSocket: '%s'", message );
        throw OpenUdpSocketException(m_portLocal, message);
    }

    SDLNet_ResolveHost(&m_adresse, m_ipDistant, m_portDistant);

    m_channel = SDLNet_UDP_Bind(m_socket, -1, &m_adresse);

    if(m_channel == -1) {
        const char* message = SDLNet_GetError();
		TRACE().error("SDLNet_UDP_Bind: '%s'", message );
        throw BindingUdpException(message);
    }

    while(m_isAlive) {
        // Wait an UDP packet for 1 seconde
		int numReady = SDLNet_CheckSockets( m_socketSet, 1000 );	// Nombre de sockets ayant une activité détectée

        if( numReady==-1 ) {
			TRACE().error("SDLNet_CheckSockets: '%s'", SDLNet_GetError());
			perror("SDLNet_CheckSockets");
        }
        else if( numReady ) {
            UDPpacket *packet = SDLNet_AllocPacket( 65535 );
			int numrecv;

			if( SDLNet_SocketReady( m_socket ) ) {
				numrecv = SDLNet_UDP_Recv(m_socket, packet);
			}

            if(numrecv) {
                m_udpController->receive(packet);
            }
        }
    }

    return 0;
}

