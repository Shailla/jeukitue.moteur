#include <string>
#include <iostream>

#include "SDL_net.h"

#include "BindingUdpException.h"
#include "OpenUdpSocketException.h"
#include "UdpController.h"

#include "UdpConnector.h"

using namespace std;

/**
    Constructeur
    portEcoute : numéro du port d'écoute de la facade
*/
UdpConnector::UdpConnector(UdpController* udpController,
                           const int portLocal)
{
    m_channel = -1;   // Uninitialized channel value
    m_udpController = udpController;

    m_socketSet = SDLNet_AllocSocketSet(16);
    if(!m_socketSet) {
        const char* message = SDLNet_GetError();
        cerr << "\nSDLNet_AllocSocketSet: " << message;
        throw OpenUdpSocketException(portLocal, message);
    }

    m_socket = SDLNet_UDP_Open(portLocal);

    if(!m_socket) {
        const char* message = SDLNet_GetError();
        cerr << "\nSDLNet_UDP_Open: " << message;
        throw OpenUdpSocketException(portLocal, message);
    }

    int numused = SDLNet_UDP_AddSocket(m_socketSet, m_socket);
    if(numused==-1)
    {
        const char* message = SDLNet_GetError();
        cerr << "\nSDLNet_UDP_AddSocket: " << message;
        throw OpenUdpSocketException(portLocal, message);
    }
}

void UdpConnector::connect(const char* ip, const int port) throw (BindingUdpException)
{
    SDLNet_ResolveHost(&m_adresse, ip, port);

    m_channel = SDLNet_UDP_Bind(m_socket, -1, &m_adresse);

    if(m_channel == -1) {
        const char* message = SDLNet_GetError();
        cerr << "\nSDLNet_UDP_Bind: " << message;
        throw BindingUdpException(message);
    }

    m_isAlive = true;
    m_thread = SDL_CreateThread(UdpConnector::runReceive, (void*)this);
}

UdpConnector::~UdpConnector(void)
{
    if(m_socket != 0) {
        SDLNet_UDP_Unbind(m_socket, 0);
        SDLNet_UDP_Close(m_socket);
    }
}

void UdpConnector::send(UDPpacket* packet)
{
    SDLNet_UDP_Send(m_socket, m_channel, packet);

    // TODO    delete packet->data;
    // TODO    delete packet;
}

void UdpConnector::stop() 
{
    m_isAlive = false;
    int threadStatus;
    SDL_WaitThread(m_thread, &threadStatus);
}

int UdpConnector::runReceive(void* udpConnectorPtr)
{
    UdpConnector* udpConnector = (UdpConnector*)udpConnectorPtr;
    udpConnector->receive();

    return 0;
}

int UdpConnector::receive()
{
    while(m_isAlive)
    {
        // Wait an UDP packet for 1 seconde
        int numReady = SDLNet_CheckSockets( m_socketSet, 1000 );	// Nombre de sockets ayant une activité détectée
    	
        if( numReady==-1 )
        {
	        cout << "SDLNet_CheckSockets: " << SDLNet_GetError();
        }
        else if( numReady )
        {
            UDPpacket packet;

            int numrecv = SDLNet_UDP_Recv(m_socket, &packet);
            if(numrecv) {
                m_udpController->receive(&packet);
            }
        }
    }

    return 0;
}

