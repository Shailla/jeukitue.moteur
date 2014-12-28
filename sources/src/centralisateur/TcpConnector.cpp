#include <fstream>

#include "SDL_net.h"
#include "ZipArchive.h"

#include "util/Trace.h"
#include "reseau/TcpUtils.h"
#include "util/FindFolder.h"
#include "ressource/RessourceConstantes.h"
#include "menu/ProgressBarView.h"

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

		TRACE().info("Download id='%l', nom='%s', taille='%d'", item._identifier, item._nom.c_str(), item._taille);
	}

	// Fermeture de la socket
	SDLNet_TCP_Close(socket);

	return list;
}

struct DownloadOneFileThreadData
{
	int _port;
	long _downloadId;
	ProgressBarView* _progressView;
};

int downloadOneFileThread(void* threadData)
{
	int port = ((DownloadOneFileThreadData*) threadData)->_port;
	long downloadId = ((DownloadOneFileThreadData*) threadData)->_downloadId;
	ProgressBarView* progressView = ((DownloadOneFileThreadData*) threadData)->_progressView;

	// Initialisation de la barre de progression
	progressView->setProgressPercentage(0);
	progressView->setCurrentOperationMessage("Connection au centralisateur...");

	// Open a TCP connection to the Centralisateur exchange files service
	IPaddress ip;
	TCPsocket socket;

	if(SDLNet_ResolveHost(&ip,"localhost", port) == -1) {
		throw CentralisateurTcpException("Resolve host error");
	}

	socket = SDLNet_TCP_Open(&ip);
	if(!socket) {
		throw CentralisateurTcpException("TCP open error");
	}

	// Send a TCP request to obtain the file
	progressView->setCurrentOperationMessage("Envoi de la demande du fichier...");

	Uint8* dataCommand = new Uint8[4];
	SDLNet_Write32((Uint32)2, dataCommand);
	int result = SDLNet_TCP_Send(socket, dataCommand, 4);

	if(result < 4) {
		throw CentralisateurTcpException("Request downloadable list file error");
	}

	// Send the identifier of the file
	Uint8* dataIdentifier = new Uint8[8];
	JktNet_Write64((Uint64)downloadId, dataIdentifier);
	result = SDLNet_TCP_Send(socket, dataIdentifier, 8);

	if(result < 8) {
		throw CentralisateurTcpException("Request file error");
	}


	/**************************************/
	/* Receive the file data              */
	/**************************************/
	progressView->setCurrentOperationMessage("Transfert du fichier...");

	DownloadFileItem item;

	int responseCode = CTcpUtils::loadInteger(socket);

	if(responseCode == 10) {
		// Identifier of the file
		item._identifier = CTcpUtils::loadLong(socket);

		// Name of the file (size of the name followed by the name itself)
		item._nom = CTcpUtils::loadString(socket);

		// Category of the file (MAP, MAP_PLAYER, ...)
		item._category = static_cast<CategoryDonwloadFile>(CTcpUtils::loadInteger(socket));

		// Size of the file (size of the name followed by the name itself)
		item._taille = CTcpUtils::loadInteger(socket);

		// Création d'un fichier temporaire
		string tmpFilename = TEMPORARY_DIRECTORY;
		tmpFilename.append(item._nom);
		tmpFilename.append(".tmp");

		ofstream tmpFile(tmpFilename.c_str(), ios_base::trunc | ios_base::binary);	// Toute ancienne donnée dans le fichier est écrasée

		if(!tmpFile) {
			TRACE().error("Erreur ouverture du fichier");
		}

		int accompli = 0;

		char byte;
		for(int i=0 ; i<item._taille ; i++) {
			SDLNet_TCP_Recv(socket, &byte, 1);
			tmpFile.put(byte);
			
			accompli++;

			// Mise à jour de la barre de progression
			progressView->setProgressPercentage((accompli * 100) / item._taille);
		}

		// Fermeture des flux
		progressView->setCurrentOperationMessage("Fermeture du flux du fichier...");
		tmpFile.flush();
		tmpFile.close();

		progressView->setCurrentOperationMessage("Fermeture du flux du fichier...");
		SDLNet_TCP_Close(socket);

		// Création du répertoire de la Map
		string destDirectory = MAP_DIRECTORY;
		destDirectory.append(item._nom).append("/");

		if(CFindFolder::mkdir(destDirectory.c_str()) != 0) {
			TRACE().error(" Le repertoire '%s' existe deja", destDirectory.c_str());
		}

		// Décompression de l'archive
		progressView->setCurrentOperationMessage("Ouverture de l'archive...");

		CZipArchive zip;
		zip.Open((LPCTSTR)tmpFilename.c_str());

		for(int i = 0 ; i < zip.GetCount() ; i++) {
			CZipFileHeader* fileHeader = zip.GetFileInfo(i);
			CZipString currentFileName = fileHeader->GetFileName();
			string destFileName(destDirectory);
			destFileName.append((stdbs&)currentFileName);

			zip.ExtractFile(i, (LPCTSTR)destFileName.c_str());
		}

		zip.Close();

		// Suppression du fichier temporaire
		progressView->setCurrentOperationMessage("Suppression du fichier temporaire...");
		remove(tmpFilename.c_str());

		// Fin
		progressView->setCurrentOperationMessage("Termine.");
	}
	else {
		// Fin avec erreur
		progressView->setCurrentOperationMessage("Erreur : Le telechargement n'existe pas.");
	}

	return 0;
}

void TcpConnector::downloadOneFile(const int port, const long downloadId, ProgressBarView* progressView) throw(CentralisateurTcpException)
{
	DownloadOneFileThreadData* threadData = new DownloadOneFileThreadData();
	threadData->_port = port;
	threadData->_downloadId = downloadId;
	threadData->_progressView = progressView;

	SDL_CreateThread(downloadOneFileThread, (void*)threadData);
}

}
