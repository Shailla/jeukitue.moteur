/*
 * TcpServer.cpp
 *
 *  Created on: 16 juin 2017
 *      Author: Erwin
 */

#include "SDL.h"
#include "SDL_net.h"

#include "util/Trace.h"

#include "reseau/tcp/TcpServer.h"

using namespace std;

namespace jkt {

TcpServer::TcpServer(int port) {
	_serverPort = port;
	_serverSocket = 0;
	_socketSet = 0;
}

TcpServer::~TcpServer() {
}

void TcpServer::start() {
	// Creation de la socket serveur
	IPaddress adresse;

	if(SDLNet_ResolveHost(&adresse, NULL, _serverPort) == -1) {
		LOGERROR(("Resolve address error : %s", SDLNet_GetError()));
	}

	_serverSocket = SDLNet_TCP_Open(&adresse);

	if(!_serverSocket) {
		LOGERROR(("SDLNet_TCP_Open : %s", SDLNet_GetError()));
	}

	_socketSet = SDLNet_AllocSocketSet(1+ TCP_CLIENTS_SIZE);	// One for the server and one for each client

	if( !_socketSet ) {
		LOGERROR(("SDLNet_AllocSocketSet : %s", SDLNet_GetError()));
	}

	if(SDLNet_TCP_AddSocket(_socketSet, _serverSocket) == -1) {
		LOGERROR(("SDLNet_AddSocket: %s", SDLNet_GetError()));
	}
}

TcpPacket* TcpServer::receive() {
	TcpPacket* tcpPacket = 0;
	map<TCPsocket, TcpSession>::iterator itSession;

	while(tcpPacket == 0) {
		Uint32 time = SDL_GetTicks();

		if(SDLNet_CheckSockets(_socketSet, 100)) {
			// Server management
			if(SDLNet_SocketReady(_serverSocket)) {
				TCPsocket clientSocket = SDLNet_TCP_Accept(_serverSocket);

				if(_clientSockets.size() >=  TCP_CLIENTS_SIZE) {
					SDLNet_TCP_Close(clientSocket);
					LOGWARN(("Connexion TCP refussée car la pile de connexions est pleine"));
				}
				else if(clientSocket) {
					map<TCPsocket, TcpSession>::iterator it = _clientSockets.find(clientSocket);

					if(it != _clientSockets.end()) {
						pair<map<TCPsocket, TcpSession>::iterator, bool> ret = _clientSockets.insert(pair<TCPsocket, TcpSession>(clientSocket, TcpSession(clientSocket, time)));
						tcpPacket = receive(time, ret.first->second);
					}
					else {
						LOGWARN(("On ne devrait jamais être ici, le client est déjà connecté en TCP"));
					}
				}
				else {
					LOGWARN(("Socket ready mais rien à accepter"));
				}
			}

			// Clients management
			for(itSession = _clientSockets.begin() ; itSession != _clientSockets.end() ; itSession++) {
				if(SDLNet_SocketReady(itSession->first)) {
					tcpPacket = receive(time, itSession->second);
				}
			}
		}

		// Garbage old clients
		for(itSession = _clientSockets.begin() ; itSession != _clientSockets.end() ; itSession++) {
			TcpSession& session = itSession->second;

			if(time - session.getLastTime() > TCP_SOCKET_TIMEOUT) {
				SDLNet_TCP_DelSocket(_socketSet, session.getSocket());
				SDLNet_TCP_Close(session.getSocket());
				_clientSockets.erase(itSession);
			}
		}
	}

	return tcpPacket;
}

TcpPacket* TcpServer::receive(Uint32 time, TcpSession& session) {
	TcpPacket* packet = 0;

	char buffer[TCP_BUFFER_SIZE];
	int requestSize = SDLNet_TCP_Recv(session.getSocket(), buffer, TCP_BUFFER_SIZE); 				// Reception parametres connection du client

	// Réponse aux messages d'acknowledge
	if(requestSize == 0) {
		session.setLastTime(time);
		SDLNet_TCP_Send(session.getSocket(), "", 0); 		// Envoi du contenu de la page au client

		LOGDEBUG(("TCP acknowledge"));
	}
	else {
		packet = new TcpPacket(&session, buffer, requestSize);
	}

	return packet;
}

void TcpServer::send(TcpSession* session, void* data, int size) {
	SDLNet_TCP_Send(session->getSocket(), data, size); 		// Envoi du contenu de la page au client
}

} /* namespace jkt */
