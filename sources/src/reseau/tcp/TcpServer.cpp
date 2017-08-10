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
	_acknowledgementCount = 0;

	_tcpSocketTimeout = 180000;
	_tcpClientsSize = 20;
	_tcpBufferSize = 16384;
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

	_socketSet = SDLNet_AllocSocketSet(1+ _tcpClientsSize);	// One for the server and one for each client

	if( !_socketSet ) {
		LOGERROR(("SDLNet_AllocSocketSet : %s", SDLNet_GetError()));
	}

	if(SDLNet_TCP_AddSocket(_socketSet, _serverSocket) == -1) {
		LOGERROR(("SDLNet_AddSocket: %s", SDLNet_GetError()));
	}
}

void TcpServer::setTimeout(int tcpSocketTimeout) {
	_tcpSocketTimeout = tcpSocketTimeout;
}

void TcpServer::setClientsSize(int tcpClientsSize) {
	_tcpClientsSize = tcpClientsSize;
}

vector<TcpPacket*> TcpServer::receive(long maxTime) {
	map<TCPsocket, TcpSession>::iterator itSession;
	vector<TcpPacket*> tcpPackets;
	TcpPacket* tcpPacket;

	long startTime = SDL_GetTicks();
	long now = startTime;
	long last = startTime;

	do {
		int socketsReady = SDLNet_CheckSockets(_socketSet, maxTime - (last - startTime));
		LOGDEBUG(("Sockets ready : %d", socketsReady));

		if(socketsReady) {
			now = SDL_GetTicks();


			/* *************************** */
			/* Server management           */
			/* *************************** */

			if(SDLNet_SocketReady(_serverSocket)) {
				TCPsocket clientSocket;

				while((clientSocket = SDLNet_TCP_Accept(_serverSocket))) {
					if(_clientSockets.size() >=  (size_t)_tcpClientsSize) {
						LOGWARN(("Connexion TCP refusée (pile de connexions est pleine)"));

						SDLNet_TCP_Close(clientSocket);
					}
					else {
						map<TCPsocket, TcpSession>::iterator it = _clientSockets.find(clientSocket);

						if(it == _clientSockets.end()) {	// Si la socket n'est pas encore connectée
							// Connecte la socket au serveur
							LOGINFO(("Nouvelle connexion TCP"));

							_clientSockets.insert(pair<TCPsocket, TcpSession>(clientSocket, TcpSession(clientSocket, now)));
							SDLNet_TCP_AddSocket(_socketSet, clientSocket);
						}
						else {
							LOGERROR(("On ne devrait jamais être ici, le client est déjà connecté en TCP"));
						}
					}
				}
			}


			/* *************************** */
			/* Clients data reception      */
			/* *************************** */

			LOGDEBUG(("Client sockets size : %d", _clientSockets.size()));

			for(itSession = _clientSockets.begin() ; itSession != _clientSockets.end() ; itSession++) {
				if(SDLNet_SocketReady(itSession->first)) {
					tcpPacket = receive(now, itSession->second);

					if(tcpPacket) {
						tcpPackets.push_back(tcpPacket);
					}
				}
			}
		}


		/* *************************** */
		/* Garbage clients             */
		/* *************************** */

		for(itSession = _clientSockets.begin() ; itSession != _clientSockets.end() ; itSession++) {
			TcpSession& session = itSession->second;

			if(now - session.getLastTime() > (size_t)_tcpSocketTimeout) {
				LOGINFO(("Purge d'une session TCP en timeout"));

				SDLNet_TCP_DelSocket(_socketSet, session.getSocket());
				SDLNet_TCP_Close(session.getSocket());
				itSession = _clientSockets.erase(itSession);
			}
		}

		last = SDL_GetTicks();
	} while((last - startTime < maxTime) && (tcpPackets.size() == 0));

	return tcpPackets;
}

TcpPacket* TcpServer::receive(Uint32 now, TcpSession& session) {
	TcpPacket* packet = 0;

	char buffer[_tcpBufferSize];
	int requestSize = SDLNet_TCP_Recv(session.getSocket(), buffer, _tcpBufferSize); 				// Reception parametres connection du client

	// Réponse aux messages d'acknowledge
	/*if(requestSize == 0) {
		session.setLastTime(now);
		LOGINFO(("TCP ACK"));
		_acknowledgementCount++;
		SDLNet_TCP_Send(session.getSocket(), "", 0); 		// Envoi du contenu de la page au client
	}
	// Déconnexion client ou erreur socket
	else */if(requestSize <= 0) {
		LOGINFO(("Déconnexion d'une session TCP par le client"));

		SDLNet_TCP_DelSocket(_socketSet, session.getSocket());
		SDLNet_TCP_Close(session.getSocket());
		_clientSockets.erase(session.getSocket());
	}
	// Données métier
	else {
		LOGINFO(("Reception d'un paquet"));
		session.setLastTime(now);
		packet = new TcpPacket(&session, buffer, requestSize);
	}

	return packet;
}

long TcpServer::getAcknowledgementCount() const {
	return _acknowledgementCount;
}

void TcpServer::setTcpBufferSize(int tcpBufferSize) {
	_tcpBufferSize = tcpBufferSize;
}

void TcpServer::send(TcpSession* session, void* data, int size) {
	SDLNet_TCP_Send(session->getSocket(), data, size); 		// Envoi du contenu de la page au client
}

} /* namespace jkt */
