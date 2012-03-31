#include <iostream>
#include <sstream>
#include "SDL_net.h"

#include "reseau/TcpUtils.h"

namespace JktNet
{

int CTcpUtils::loadInteger(const TCPsocket socket) throw(CentralisateurTcpException)
{
	// Read the number size of the string
	const int SIZE = 4;

	Uint8 data[SIZE];
	int num = SDLNet_TCP_Recv(socket, data, SIZE);

	if(SIZE != num) {
		std::ostringstream message;
		message << "Bad data received size, expected " << SIZE << " but received " << num;
		throw CentralisateurTcpException(message.str());
	}

    return (int) SDLNet_Read32(data);
}

long CTcpUtils::loadLong(const TCPsocket socket) throw(CentralisateurTcpException)
{
	// Read the number size of the string
	const int SIZE = 8;

	Uint8 data[SIZE];
	int num = SDLNet_TCP_Recv(socket, data, SIZE);

	if(SIZE != num) {
		std::ostringstream message;
		message << "Bad data received size, expected " << SIZE << " but received " << num;
		throw CentralisateurTcpException(message.str());
	}

    return (long) JktNet_Read64(data);
}

char* CTcpUtils::loadString(const TCPsocket socket) throw(CentralisateurTcpException)
{
	// Read the number size of the string
	int strSize = loadInteger(socket);

	// Read the string itself
	char* data = new char[strSize + 1];
	int num = SDLNet_TCP_Recv(socket, data, strSize);

	if(strSize != num) {
		std::ostringstream message;
		message << "Bad data received size, expected " << strSize << " but received " << num;
		throw CentralisateurTcpException(message.str());
	}

	data[strSize] = '\0';

    return data;
}

char* CTcpUtils::loadCharsBySize(const TCPsocket socket, const int size) throw(CentralisateurTcpException)
{
	// Read the string itself
	char* data = new char[size];
	int num = SDLNet_TCP_Recv(socket, data, size);

	if(size != num) {
		std::ostringstream message;
		message << "Bad data received size, expected " << size << " but received " << num;
		throw CentralisateurTcpException(message.str());
	}

    return data;
}

}