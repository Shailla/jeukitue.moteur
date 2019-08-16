#ifndef __JKT__UDPCONNECTOR_H
#define __JKT__UDPCONNECTOR_H

#include "SDL_net.h"

#include "centralisateur/data/Data.h"

#include "exception/centralisateur/BindingUdpException.h"

class UdpController;

class UdpConnector
{
    IPaddress m_adresse;    // Adresse du serveur
    UDPsocket m_socket;
    SDLNet_SocketSet m_socketSet;
    UdpController* m_udpController;
    int m_channel;
    int m_portLocal;      // Port utilis� en local (donc c�t� client)
    bool m_isAlive;     // False when the connector has to stop listen
    SDL_Thread* m_thread;
    void decodePacket(UDPpacket* packet);
	char* m_ipDistant;
	int m_portDistant;
public:
    UdpConnector(UdpController* udpController, const int portLocal);
    ~UdpConnector(void);

    void connect(char* ip, int port) noexcept(false);
    void start();
    void stop();
    void send(UDPpacket* packet);
    int receive();

    static int runReceive(void* udpConnectorPtr);
};

#endif
