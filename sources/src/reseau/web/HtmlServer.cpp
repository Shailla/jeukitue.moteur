/*
 * HtmlServer.cpp
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#include <sstream>
#include <fstream>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "util/FindFolder.h"
#include "util/Trace.h"

#include "reseau/web/HtmlServer.h"

namespace JktNet
{

const char* HtmlServer::WEB_STATIC_RESOURCES_DIR =	"./web/static/";

const char* HtmlServer::HTTP_RETURN =			"\r\n";
const char* HtmlServer::HTTP_HEAD = 			"HTTP/1.1";
const char* HtmlServer::HTTP_RESPONSE_OK = 		"200 OK";
const char* HtmlServer::HTTP_CONTENT_HTML = 	"Content-type: text/html; charset=utf-8";
const char* HtmlServer::HTTP_CONTENT_LENGTH = 	"Content-Length: ";

HtmlServer::HtmlServer(int port) {
	_port = port;
}

HtmlServer::~HtmlServer() {
}

void HtmlServer::open() {
	SDL_CreateThread(HtmlServer::run, this);
}

int HtmlServer::run(void* thiz) {
	((HtmlServer*)thiz)->start();
	return 0;
}

void HtmlServer::start() {
	// Creation de la socket serveur
	IPaddress adresse;

	if(SDLNet_ResolveHost(&adresse, NULL, _port) == -1) {
		LOGERROR(("Resolve address error : %s", SDLNet_GetError()));
	}

	TCPsocket serveurSocket = SDLNet_TCP_Open(&adresse);

	if(!serveurSocket) {
		LOGERROR(("Open TCP server failed : %s", SDLNet_GetError()));
	}

	TCPsocket clientSocket;

	while(1) {
		// Attente connexion client
		clientSocket = 0;

		while(clientSocket == 0)
			clientSocket = SDLNet_TCP_Accept(serveurSocket);

		char request[1024];

		int requestSize = SDLNet_TCP_Recv(clientSocket, request, 1024); 				// Reception parametres connection du client
		request[requestSize] = 0;
		LOGINFO(("HTTP requête reçue : %s", request));

		// Recherche du contenu
		string content = getPage("index.html");

		// Entête de réponse
		stringstream response;
		response << HTTP_HEAD << " " << HTTP_RESPONSE_OK << HTTP_RETURN;
		response << HTTP_CONTENT_HTML << HTTP_RETURN;
		response << HTTP_CONTENT_LENGTH << content.size() << HTTP_RETURN;
		response << HTTP_RETURN;

		response << content;
		string responseStr = response.str();

		// Envoi réponse
		LOGINFO(("HTTP response : %s", responseStr.c_str()));

		SDLNet_TCP_Send(clientSocket, responseStr.c_str(), responseStr.size()+1); 		// Envoi du contenu de la page au client
		SDLNet_TCP_Close(clientSocket);
	}
}

string HtmlServer::getPage(string endpoint) {
	string content;

	string file = WEB_STATIC_RESOURCES_DIR + endpoint;
	LOGINFO(("Ressource de la page %s : %s", endpoint.c_str(), file.c_str()));

	// Lecture fichier contenant la page à afficher au client
	ifstream pageFile(file);
	stringstream page;

	// Lit le fichier en entier
	if(pageFile) {
		page << pageFile.rdbuf();
		pageFile.close();
		content = page.str();
	}
	else {
		content = "";
	}

	return content;
}

}	// JktNet
