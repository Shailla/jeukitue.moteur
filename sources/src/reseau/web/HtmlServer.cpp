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

#include "util/StringUtils.h"
#include "util/Trace.h"

#include "reseau/web/HtmlServer.h"

namespace JktNet
{

const char* HtmlServer::WEB_STATIC_RESOURCES_DIR =	"./web/static/";

const char* HtmlServer::HTTP_RETURN =			"\r\n";
const char* HtmlServer::HTTP_HEAD = 			"HTTP/1.1";
const char* HtmlServer::HTTP_RESPONSE_200 = 	"200 OK";
const char* HtmlServer::HTTP_RESPONSE_404 = 	"404 NOT FOUND";
const char* HtmlServer::HTTP_RESPONSE_500 = 	"500 INTERNAL ERROR";
const char* HtmlServer::HTTP_CONTENT_HTML = 	"Content-type: text/html; charset=utf-8";
const char* HtmlServer::HTTP_CONTENT_LENGTH = 	"Content-Length: ";

// Contenu en dur en dernier recours (erreur interne) pour éviter de cumuler une erreur interne avec une ressource introuvable
const char* HtmlServer::HTTP_INTERNAL_ERROR_CONTENT = "<html><center><h1>JKT Power</h1><font size=5 color=red>Erreur interne</font></center></html>";

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
	string response, content, method, endpoint, protocol;

	while(1) {
		// Attente connexion client
		clientSocket = 0;

		while(clientSocket == 0)
			clientSocket = SDLNet_TCP_Accept(serveurSocket);

		char buffer[1024];

		int requestSize = SDLNet_TCP_Recv(clientSocket, buffer, 1024); 				// Reception parametres connection du client
		buffer[requestSize] = 0;

		string request = buffer;
		LOGDEBUG(("HTTP requête reçue : '%s'", request.c_str()));

		method = JktUtils::StringUtils::findAndEraseFirstWord(request);
		endpoint = JktUtils::StringUtils::findAndEraseFirstWord(request);
		protocol = JktUtils::StringUtils::findAndEraseFirstWord(request);
		LOGDEBUG((" - Méthode : '%s'", method.c_str()));
		LOGDEBUG((" - Ressource demandée : '%s'", endpoint.c_str()));
		LOGDEBUG((" - Protocol : '%s'", protocol.c_str()));

		// Recherche du contenu
		try {
			content = getPage(endpoint);
			response = buildResponse(content, HTTP_RESPONSE_200);
		}
		catch(int exception) {
			switch(exception) {
			case RESOURCE_NOT_FOUND_EXCEPTION:
				content = getPage("resource_not_found.html");
				response = buildResponse(content, HTTP_RESPONSE_404);
				LOGWARN(("Endpoint introuvable : '%s'", endpoint));
				break;
			default:
				response = buildResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_RESPONSE_500);
				LOGERROR(("On ne devrait jamais être ici '%s' : %d", endpoint.c_str(), exception));
				break;
			}
		}
		catch(std::exception& exception) {
			response = buildResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_RESPONSE_500);
			LOGERROR(("Erreur interne (exception standard) sur '%s' : '%s'", endpoint.c_str(), exception.what()));
		}
		catch(...) {
			response = buildResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_RESPONSE_500);
			LOGERROR(("Erreur interne inconnue sur '%s'", endpoint.c_str()));
		}

		// Envoi réponse
		LOGDEBUG(("HTTP response : '%s'", response.c_str()));

		SDLNet_TCP_Send(clientSocket, response.c_str(), response.size()+1); 		// Envoi du contenu de la page au client
		SDLNet_TCP_Close(clientSocket);
	}
}

string HtmlServer::buildResponse(const string& content, const string& status) {
	stringstream response;

	// Entête de réponse
	response << HTTP_HEAD << " " << status << HTTP_RETURN;
	response << HTTP_CONTENT_HTML << HTTP_RETURN;
	response << HTTP_CONTENT_LENGTH << content.size() << HTTP_RETURN;
	response << HTTP_RETURN;

	response << content;

	return response.str();
}

string HtmlServer::getPage(const string& endpoint) throw (int) {
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
		throw (int)RESOURCE_NOT_FOUND_EXCEPTION;
	}

	return content;
}

}	// JktNet
