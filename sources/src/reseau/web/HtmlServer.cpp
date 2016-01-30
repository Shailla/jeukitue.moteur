/*
 * HtmlServer.cpp
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#include <sstream>
#include <fstream>
#include <dirent.h>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "util/StringUtils.h"
#include "util/Trace.h"
#include "util/FindFolder.h"

#include "reseau/web/HtmlServer.h"

namespace JktNet
{

const char* HtmlServer::WEB_CSS_DIR =		"./web/css";
const char* HtmlServer::WEB_HTML_DIR =		"./web/html";
const char* HtmlServer::WEB_IMAGE_DIR =		"./web/image";
const char* HtmlServer::WEB_JS_DIR =		"./web/js";
const char* HtmlServer::WEB_JSON_DIR =		"./web/json";

const char* HtmlServer::HTTP_RETURN =				"\r\n";
const char* HtmlServer::HTTP_HEAD = 				"HTTP/1.1";
const char* HtmlServer::HTTP_RESPONSE_200 = 		"200 OK";
const char* HtmlServer::HTTP_RESPONSE_404 = 		"404 NOT FOUND";
const char* HtmlServer::HTTP_RESPONSE_500 = 		"500 INTERNAL ERROR";
const char* HtmlServer::HTTP_CONTENT_TYPE_HTML = 	"Content-type: text/html; charset=utf-8";
const char* HtmlServer::HTTP_CONTENT_TYPE_CSS = 	"Content-type: text/css; charset=utf-8";
const char* HtmlServer::HTTP_CONTENT_TYPE_IMAGE = 	"Content-type: image";
const char* HtmlServer::HTTP_CONTENT_TYPE_JS = 		"Content-type: application/javascript;";
const char* HtmlServer::HTTP_CONTENT_TYPE_JSON = 	"Content-type: application/json; charset=utf-8";
const char* HtmlServer::HTTP_CONTENT_LENGTH = 		"Content-Length: ";

WebResource::WebResource() {
}

WebResource::WebResource(const string& file, const string& contentType, const string& content) {
	_file = file;
	_contentType = contentType;
	_content = content;
}


// Contenu en dur en dernier recours (erreur interne) pour éviter de cumuler une erreur interne avec une ressource introuvable
const char* HtmlServer::HTTP_INTERNAL_ERROR_CONTENT = "<html><center><h1>JKT Power</h1><font size=5 color=red>Erreur interne</font></center></html>";

HtmlServer::HtmlServer(int port) {
	_port = port;


	/* ************************************ */
	/* Lecture des ressources disponibles	*/
	/* ************************************ */

	// Lecture des ressources web
	collecteDir(WEB_CSS_DIR, "", HTTP_CONTENT_TYPE_CSS);			// CSS
	collecteDir(WEB_HTML_DIR, "", HTTP_CONTENT_TYPE_HTML);			// HTML
	collecteDir(WEB_IMAGE_DIR, "", HTTP_CONTENT_TYPE_IMAGE);		// Images
	collecteDir(WEB_JS_DIR, "", HTTP_CONTENT_TYPE_JS);				// Scripts
	collecteDir(WEB_JSON_DIR, "", HTTP_CONTENT_TYPE_JSON);			// JSON
}

void HtmlServer::collecteDir(const string& dirname, const string& endpoint, const string& contentType) {
	DIR* dir = opendir(dirname.c_str());
	struct dirent* file;
	bool isDir;
	string filename;

	while((file = readdir(dir))) {
		filename = file->d_name;

		string resEndpoint = endpoint + "/" + filename;
		string resFile = dirname + "/" + filename;

		if(filename != "." && filename != "..") {	// Exclude "." and ".." directories
			isDir = CFindFolder::isFolder(resFile);

			if(isDir) {
				// Parcours le sous-répertoire
				collecteDir(resFile, resEndpoint, contentType);
			}
			else {
				LOGINFO(("Chargement ressource Web : endpoint:'%s' file:'%s'", resEndpoint.c_str(), resFile.c_str()));

				ifstream pageFile(resFile);
				stringstream content;

				if(pageFile) {
					// Chargement du contenu du fichier
					content << pageFile.rdbuf();
					pageFile.close();

					// Enregistrement en cache
					WebResource res(resFile, contentType, content.str());
					_resources[resEndpoint] = res;
				}
				else {
					LOGERROR(("Fichier introuvable : %s", resFile.c_str()));
				}
			}
		}
	}

	closedir(dir);
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
	string response, method, endpoint, protocol, path0;
	vector<string> paths;
	WebResource* resource;

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
			resource = getResource(endpoint);
			response = buildResponse(resource, HTTP_RESPONSE_200);
		}
		catch(int exception) {
			switch(exception) {
			case RESOURCE_NOT_FOUND_EXCEPTION:
				resource = getResource("/resource_not_found.html");
				response = buildResponse(resource, HTTP_RESPONSE_404);
				LOGWARN(("Endpoint introuvable : '%s'", endpoint.c_str()));
				break;
			default:
				response = buildResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
				LOGERROR(("On ne devrait jamais être ici '%s' : %d", endpoint.c_str(), exception));
				break;
			}
		}
		catch(std::exception& exception) {
			response = buildResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
			LOGERROR(("Erreur interne (exception standard) sur '%s' : '%s'", endpoint.c_str(), exception.what()));
		}
		catch(...) {
			response = buildResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
			LOGERROR(("Erreur interne inconnue sur '%s'", endpoint.c_str()));
		}

		// Envoi réponse
		LOGDEBUG(("HTTP response : '%s'", response.c_str()));

		SDLNet_TCP_Send(clientSocket, response.c_str(), response.size()+1); 		// Envoi du contenu de la page au client
		SDLNet_TCP_Close(clientSocket);
	}
}

WebResource* HtmlServer::getResource(const string& endpoint) throw(int) {
	try {
		return &_resources.at(endpoint);
	}
	catch(out_of_range& exception) {
		throw (int)RESOURCE_NOT_FOUND_EXCEPTION;
	}
}

string HtmlServer::buildResponse(const string& content, const string& contentType, const string& status) {
	stringstream response;

	// Entête de réponse
	response << HTTP_HEAD << " " << status << HTTP_RETURN;
	response << contentType << HTTP_RETURN;
	response << HTTP_CONTENT_LENGTH << content.size() << HTTP_RETURN;
	response << HTTP_RETURN;

	response << content;

	return response.str();
}

string HtmlServer::buildResponse(const WebResource* resource, const string& status) {
	stringstream response;

	// Entête de réponse
	response << HTTP_HEAD << " " << status << HTTP_RETURN;
	response << resource->_contentType << HTTP_RETURN;
	response << HTTP_CONTENT_LENGTH << resource->_content.size() << HTTP_RETURN;
	response << HTTP_RETURN;

	response << resource->_content;

	return response.str();
}

}	// JktNet
