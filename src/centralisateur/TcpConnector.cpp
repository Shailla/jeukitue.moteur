#include <iostream>
#include <fstream>

#include "SDL_net.h"

#include "reseau/TcpUtils.h"

#include "centralisateur/TcpConnector.h"

using namespace std;

namespace JktNet
{

vector<DownloadFileItem> TcpConnector::askDownloadFileList(const int port) throw(CentralisateurTcpException)
{
	// Open a TCP connection to the Centralisateur exchange files service
	IPaddress ip;
	TCPsocket socket;

	if(SDLNet_ResolveHost(&ip,"localhost", port)==-1) {
		throw CentralisateurTcpException("Resolve host error");
	}

	socket = SDLNet_TCP_Open(&ip);
	if(!socket) {
		throw CentralisateurTcpException("TCP open error");
	}

	// Send a TCP request to obtain the available download files list
	Uint8* data= new Uint8[4];
	SDLNet_Write32((Uint32)1, data);

	int result = SDLNet_TCP_Send(socket, data, 4);

	if(result < 4) {
		throw CentralisateurTcpException("Request downloadable list file error");
	}

	// Get data
	Uint32 nbrFiles = CTcpUtils::loadInteger(socket);

	vector<DownloadFileItem> list;

	for(Uint32 i = 0 ; i < nbrFiles ; i++) {
		DownloadFileItem item;

		// Identifier of the file
		item._identifier = CTcpUtils::loadLong(socket);

		// Name of the file (size of the name followed by the name itself)
		item._nom = CTcpUtils::loadString(socket);

		// Category of the file (MAP, MAP_PLAYER, ...)
		item._category = static_cast<CategoryDonwloadFile>(CTcpUtils::loadInteger(socket));

		// Size of the file (size of the name followed by the name itself)
		item._taille = CTcpUtils::loadInteger(socket);

		// Description of the file (size of the name followed by the name itself)
		item._description = CTcpUtils::loadString(socket);

		list.push_back(item);
	}

	// Fermeture de la socket
	SDLNet_TCP_Close(socket);

	return list;
}

struct DownloadOneFileThreadData
{
	int _port;
	int* _progress;
	char* _currentOperationMessage;
};

int downloadOneFileThread(void* threadData)
{
	int* progressValue = ((DownloadOneFileThreadData*) threadData)->_progress;
	int port = ((DownloadOneFileThreadData*) threadData)->_port;
	char* currentOperationMessage = ((DownloadOneFileThreadData*) threadData)->_currentOperationMessage;
	
	*progressValue = 0;

	// Open a TCP connection to the Centralisateur exchange files service
	IPaddress ip;
	TCPsocket socket;

	strcpy(currentOperationMessage, "Connection au centralisateur...");

	if(SDLNet_ResolveHost(&ip,"localhost", port) == -1) {
		throw CentralisateurTcpException("Resolve host error");
	}

	socket = SDLNet_TCP_Open(&ip);
	if(!socket) {
		throw CentralisateurTcpException("TCP open error");
	}

	// Send a TCP request to obtain the file
	strcpy(currentOperationMessage, "Envoi de la demande du fichier...");

	Uint8* dataCommand = new Uint8[4];
	SDLNet_Write32((Uint32)2, dataCommand);
	int result = SDLNet_TCP_Send(socket, dataCommand, 4);

	if(result < 4) {
		throw CentralisateurTcpException("Request downloadable list file error");
	}

	// Send the identifier of the file
	Uint8* dataIdentifier = new Uint8[8];
	JktNet_Write64((Uint64)2, dataIdentifier);
	result = SDLNet_TCP_Send(socket, dataIdentifier, 8);

	if(result < 8) {
		throw CentralisateurTcpException("Request file error");
	}


	/**************************************/
	/* Get file data                      */
	/**************************************/
	strcpy(currentOperationMessage, "Transfert du fichier...");

	DownloadFileItem item;

	// Identifier of the file
	item._identifier = CTcpUtils::loadLong(socket);

	// Name of the file (size of the name followed by the name itself)
	item._nom = CTcpUtils::loadString(socket);

	// Category of the file (MAP, MAP_PLAYER, ...)
	item._category = static_cast<CategoryDonwloadFile>(CTcpUtils::loadInteger(socket));

	// Size of the file (size of the name followed by the name itself)
	item._taille = CTcpUtils::loadInteger(socket);

	// Contenu du fichier
	ofstream file("c:/out.txt");

	if(!file)
	{
		cerr << "Erreur ouverture du fichier";
	}

	int accompli = 0;

	char byte;
	for(int i=0 ; i<item._taille ; i++) {
		SDLNet_TCP_Recv(socket, &byte, 1);
		file.put(byte);
		
		accompli++;
		*progressValue = (int) ((accompli * 100) / item._taille);
	}

	// Fermeture des flux
	strcpy(currentOperationMessage, "Fermeture du flux du fichier...");
	file.close();

	strcpy(currentOperationMessage, "Fermeture de la socket...");
	SDLNet_TCP_Close(socket);

	strcpy(currentOperationMessage, "Termine.");
}

void TcpConnector::downloadOneFile(const int port, const long fileId, int* progress, char* currentOperationMessage) throw(CentralisateurTcpException)
{
	DownloadOneFileThreadData* threadData = new DownloadOneFileThreadData();
	threadData->_port = port;
	threadData->_progress = progress;
	threadData->_currentOperationMessage = currentOperationMessage;

	SDL_CreateThread(downloadOneFileThread, (void*)threadData);
}

}