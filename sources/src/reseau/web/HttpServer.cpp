/*
 * HtmlServer.cpp
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#include <sstream>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "util/StringUtils.h"
#include "util/Trace.h"
#include "util/FindFolder.h"
#include "reseau/web/service/WebService.h"
#include "reseau/web/service/PlayersWebService.h"
#include "reseau/web/service/MapWebService.h"

#include "reseau/web/HttpServer.h"

namespace jkt
{

const char* HttpServer::WEB_CSS_DIR =		"./web/css";
const char* HttpServer::WEB_HTML_DIR =		"./web/html";
const char* HttpServer::WEB_IMAGE_DIR =		"./web/image";
const char* HttpServer::WEB_JS_DIR =		"./web/js";
const char* HttpServer::WEB_JSON_DIR =		"./web/json";

const char* HttpServer::HTTP_RETURN =				"\r\n";
const char* HttpServer::HTTP_HEAD = 				"HTTP/1.1";
const char* HttpServer::HTTP_RESPONSE_200 = 		"200 OK";
const char* HttpServer::HTTP_RESPONSE_404 = 		"404 NOT FOUND";
const char* HttpServer::HTTP_RESPONSE_500 = 		"500 INTERNAL ERROR";
const char* HttpServer::HTTP_CONTENT_TYPE_HTML = 	"Content-type: text/html; charset=utf-8";
const char* HttpServer::HTTP_CONTENT_TYPE_CSS = 	"Content-type: text/css; charset=utf-8";
const char* HttpServer::HTTP_CONTENT_TYPE_IMAGE = 	"Content-type: image";
const char* HttpServer::HTTP_CONTENT_TYPE_JS = 		"Content-type: application/javascript;";
const char* HttpServer::HTTP_CONTENT_TYPE_JSON = 	"Content-type: application/json; charset=utf-8";
const char* HttpServer::HTTP_CONTENT_LENGTH = 		"Content-Length: ";

const char* HttpServer::HTTP_METHOD_GET = 			"GET";
const char* HttpServer::HTTP_METHOD_POST = 			"POST";
const char* HttpServer::HTTP_METHOD_PUT = 			"PUT";
const char* HttpServer::HTTP_METHOD_PATCH = 		"PATCH";
const char* HttpServer::HTTP_METHOD_DELETE =		"DELETE";

WebResource::WebResource() {
	_content = 0;
	_contentSize = 0;
}

WebResource::WebResource(const string& file, const string& contentType) {
	_file = file;
	_contentType = contentType;
	_content = 0;
	_contentSize = 0;
}

WebResource::~WebResource() {
	if(_content) {
		free(_content);
	}
}

string WebResource::getContentType() {
	return _contentType;
}

void* WebResource::getContent() {
	if(!_content) {
		load();
	}

	return _content;
}

long WebResource::getContentSize() {
	if(!_content) {
		load();
	}

	return _contentSize;
}

void WebResource::load() {
	try {
		LOGINFO(("Chargement ressource Web file :'%s'", _file.c_str()));

		FILE* file = fopen(_file.c_str(), "rb");

		if(file==NULL) {
			LOGERROR(("Echec d'ouverture du fichier : '%s'", _file.c_str()));
			throw (int)1;
		}

		// Lecture taille du fichier
		fseek(file , 0 , SEEK_END);
		_contentSize = ftell(file);
		rewind(file);

		// Allocate memory to contain the whole file:
		_content = malloc(_contentSize);

		if(_contentSize == 0) {
			LOGERROR(("Echec d'allocation mémoire pour le fichier : '%s'", _file.c_str()));
			throw (int)1;
		}

		// Copy the file into the buffer
		size_t result = fread(_content, 1, _contentSize, file);

		if (result != (size_t)_contentSize) {
			LOGERROR(("Echec de lecture du fichier : '%s'", _file.c_str()));
			throw (int)1;
		}

		fclose (file);
	}
	catch(int& exception) {
		LOGERROR(("Echec de chargement du fichier : '%s'", _file.c_str()));

		if(_content) {
			free(_content);
		}
	}
}

// Contenu en dur en dernier recours (erreur interne) pour éviter de cumuler une erreur interne avec une ressource introuvable
const char* HttpServer::HTTP_INTERNAL_ERROR_CONTENT = "<html><center><h1>JKT Power</h1><font size=5 color=red>Erreur interne</font></center></html>";

HttpServer::HttpServer(int port) {
	_port = port;


	/* ************************************ */
	/* Déploiement des services web			*/
	/* ************************************ */

	_services["/rest/player-service/"] = new PlayersWebService();
	_services["/rest/map-service/"] = new MapWebService();


	/* ************************************ */
	/* Lecture des ressources disponibles	*/
	/* ************************************ */

	// Lecture des ressources web
	collecteDir(WEB_JS_DIR, "", HTTP_CONTENT_TYPE_JS);				// Scripts
	collecteDir(WEB_CSS_DIR, "", HTTP_CONTENT_TYPE_CSS);			// CSS
	collecteDir(WEB_JSON_DIR, "", HTTP_CONTENT_TYPE_JSON);			// JSON
	collecteDir(WEB_HTML_DIR, "", HTTP_CONTENT_TYPE_HTML);			// HTML
	collecteDir(WEB_IMAGE_DIR, "", HTTP_CONTENT_TYPE_IMAGE);		// Images
}

void HttpServer::collecteDir(const string& dirname, const string& endpoint, const string& contentType) {
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
				WebResource* res = new WebResource(resFile, contentType);
				_resources[resEndpoint] = res;
			}
		}
	}

	closedir(dir);
}

HttpServer::~HttpServer() {
	for(std::pair<string, WebResource*> res : _resources) {
		delete res.second;
	}
}

void HttpServer::open() {
	SDL_CreateThread(HttpServer::run, this);
}

int HttpServer::run(void* thiz) {
	((HttpServer*)thiz)->start();
	return 0;
}

HttpServer::HTTP_METHODS HttpServer::resolveHttpMethod(const string& method) {
	if(method == HTTP_METHOD_GET) {
		return HTTP_METHODS::HTTP_GET;
	}
	else if(method == HTTP_METHOD_POST) {
		return HTTP_METHODS::HTTP_POST;
	}
	else if(method == HTTP_METHOD_PUT) {
		return HTTP_METHODS::HTTP_PUT;
	}
	else if(method == HTTP_METHOD_PATCH) {
		return HTTP_METHODS::HTTP_PATCH;
	}
	else if(method == HTTP_METHOD_DELETE) {
		return HTTP_METHODS::HTTP_DELETE;
	}
	else {
		return HTTP_METHODS::HTTP_UNKNOWN;
	}
}

void HttpServer::start() {
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
	string header, methodStr, url, endpoint, params, protocol, path0;
	vector<string> paths;
	WebResource* resource;
	long contentSize;
	void* content;
	string contentType;
	string status;
	char* response;
	long responseSize;
	bool found;

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

		methodStr = jkt::StringUtils::findAndEraseFirstWord(request);

		HTTP_METHODS method = resolveHttpMethod(methodStr);

		url = jkt::StringUtils::findAndEraseFirstWord(request);
		protocol = jkt::StringUtils::findAndEraseFirstWord(request);

		// Extrait les paramètres de la requête s'il y en a
		size_t bp = url.find("?");
		endpoint = url.substr(0, bp);

		if(bp != string::npos) {
			params = url.substr(bp+1);
		}
		else {
			params = "";	// Pas de paramètres sur la requête
		}

		LOGDEBUG((" - Méthode :  '%s (%d)'", methodStr.c_str(), (int)method));
		LOGDEBUG((" - Endpoint : '%s'", endpoint.c_str()));
		LOGDEBUG((" - Params:    '%s'", params.c_str()));
		LOGDEBUG((" - Protocol : '%s'", protocol.c_str()));

		// Recherche du contenu
		try {
			// Init
			content = 0;
			found = false;

			// Search web service
			if(!found) {
				string baseEndpoint, serviceEndpoint;
				WebService* service = getService(endpoint, baseEndpoint, serviceEndpoint);

				if(service) {
					WebServiceResult result = service->execute(method, endpoint, baseEndpoint, serviceEndpoint, params);

					contentType = result._contentType;
					contentSize = result._contentSize;
					content = result._content;
					status = result._status;

					found = true;
				}
			}

			// Search static web resource
			if(!found) {
				resource = getResource(endpoint);	// Exception thrown if no resource found

				if(resource) {
					contentType = resource->getContentType();
					contentSize = resource->getContentSize();
					content = resource->getContent();
					status = HTTP_RESPONSE_200;

					found = true;
				}
			}

			// Nothing is found for the endpoint
			if(!found) {
				throw (int)RESOURCE_NOT_FOUND_EXCEPTION;
			}

			header = buildResponseHeader(contentType, contentSize, status);

			responseSize = header.size() + contentSize;
			response = (char*)malloc(responseSize);
			header.copy(response, header.size());
			memcpy(response + (size_t)header.size(), content, contentSize);
		}
		catch(int exception) {
			switch(exception) {
			case RESOURCE_NOT_FOUND_EXCEPTION:
				LOGWARN(("Endpoint introuvable : '%s'", endpoint.c_str()));

				resource = getResource("/resource_not_found.html");

				if(resource) {
					header = buildResponseHeader(resource->getContentType(), resource->getContentSize(), HTTP_RESPONSE_404);

					responseSize = header.size() + resource->getContentSize();
					response = (char*)malloc(responseSize);
					header.copy(response, header.size());
					memcpy(response + header.size(), resource->getContent(), resource->getContentSize());
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant) '%s' : %d", endpoint.c_str(), exception));
					string str = buildStringResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
					responseSize = str.size();
					response = (char*)malloc(responseSize);
					str.copy(response, responseSize);
				}

				break;
			case SERVICE_NOT_EXISTS:
				LOGWARN(("Service introuvable : '%s'", endpoint.c_str()));

				resource = getResource("/resource_not_found.html");

				if(resource) {
					header = buildResponseHeader(resource->getContentType(), resource->getContentSize(), HTTP_RESPONSE_404);

					responseSize = header.size() + resource->getContentSize();
					response = (char*)malloc(responseSize);
					header.copy(response, header.size());
					memcpy(response + header.size(), resource->getContent(), resource->getContentSize());
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant) '%s' : %d", endpoint.c_str(), exception));
					string str = buildStringResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
					responseSize = str.size();
					response = (char*)malloc(responseSize);
					str.copy(response, responseSize);
				}
				break;
			default:
				LOGERROR(("On ne devrait jamais être ici (erreur non-maîtrisée) '%s' : %d", endpoint.c_str(), exception));
				string str = buildStringResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
				responseSize = str.size();
				response = (char*)malloc(responseSize);
				str.copy(response, responseSize);
				break;
			}
		}
		catch(std::exception& exception) {
			LOGERROR(("Erreur interne (exception standard) sur '%s' : '%s'", endpoint.c_str(), exception.what()));
			string str = buildStringResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
			responseSize = str.size();
			response = (char*)malloc(responseSize);
			str.copy(response, responseSize);
		}
		catch(...) {
			LOGERROR(("Erreur interne inconnue sur '%s'", endpoint.c_str()));
			string str = buildStringResponse(HTTP_INTERNAL_ERROR_CONTENT, HTTP_CONTENT_TYPE_HTML, HTTP_RESPONSE_500);
			responseSize = str.size();
			response = (char*)malloc(responseSize);
			str.copy(response, responseSize);
		}

		// Envoi réponse
		LOGDEBUG(("HTTP response : '%s'", response));

		SDLNet_TCP_Send(clientSocket, response, responseSize); 		// Envoi du contenu de la page au client
		SDLNet_TCP_Close(clientSocket);

		free(response);
	}
}

WebService* HttpServer::getService(const string& fullEndpoint, string& baseEndpoint, string& serviceEndpoint) {

	for(auto& iter : _services) {
		if(StringUtils::isBeginingWith(fullEndpoint, iter.first)) {
			baseEndpoint = iter.first;
			serviceEndpoint = fullEndpoint.substr(baseEndpoint.size());

			return iter.second;
		}
	}

	return 0;
}

WebResource* HttpServer::getResource(const string& endpoint) throw(int) {
	try {
		return _resources.at(endpoint);
	}
	catch(out_of_range& exception) {
		return 0;
	}
}

string HttpServer::buildStringResponse(const string& content, const string& contentType, const string& status) {
	stringstream response;

	// Entête de réponse
	response << HTTP_HEAD << " " << status << HTTP_RETURN;
	response << "Access-Control-Allow-Origin: *" << HTTP_RETURN;	// TODO A supprimer, sert juste aux tests avec nodejs
	response << contentType << HTTP_RETURN;
	response << HTTP_CONTENT_LENGTH << content.size() << HTTP_RETURN;
	response << HTTP_RETURN;

	response << content;

	return response.str();
}

string HttpServer::buildResponseHeader(const string& contentType, long contentSize, const string& status) {
	stringstream header;

	// Entête de réponse
	header << HTTP_HEAD << " " << status << HTTP_RETURN;
	header << "Access-Control-Allow-Origin: *" << HTTP_RETURN;	// TODO A supprimer, sert juste aux tests avec nodejs
	header << contentType << HTTP_RETURN;
	header << HTTP_CONTENT_LENGTH << contentSize << HTTP_RETURN;
	header << HTTP_RETURN;

	return header.str();
}

}	// JktNet
