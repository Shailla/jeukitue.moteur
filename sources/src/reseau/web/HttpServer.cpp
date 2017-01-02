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

const char* HttpServer::WEB_CSS_DIR =				"./web/css";
const char* HttpServer::WEB_HTML_DIR =				"./web/html";
const char* HttpServer::WEB_IMAGE_DIR =				"./web/image";
const char* HttpServer::WEB_JS_DIR =				"./web/js";
const char* HttpServer::WEB_JSON_DIR =				"./web/json";

const char* HttpServer::HTTP_RETURN =				"\r\n";
const char* HttpServer::HTTP_HEAD = 				"HTTP/1.1";
const char* HttpServer::HTTP_RESPONSE_200 = 		"200 OK";
const char* HttpServer::HTTP_RESPONSE_400 = 		"400 Bad Request";
const char* HttpServer::HTTP_RESPONSE_404 = 		"404 Not Found";
const char* HttpServer::HTTP_RESPONSE_500 = 		"500 Internal Error";
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


HttpTcpResponse::HttpTcpResponse() {
	_size = 0;
	_content = 0;
}

void HttpTcpResponse::update(char* content, long size) {
	if(_content) {
		free(_content);
	}

	_size = size;
	_content = content;
}

void HttpTcpResponse::reset() {
	if(_content) {
		free(_content);
	}

	_size = 0;
	_content = 0;
}

long HttpTcpResponse::getSize() const {
	return _size;
}

char* HttpTcpResponse::getContent() const {
	return _content;
}

HttpTcpResponse::~HttpTcpResponse() {
	if(_content) {
		free(_content);
	}
}

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
	HttpTcpResponse tcpResponse;
	string header, methodStr, url, endpoint, params, protocol, path0;
	vector<string> paths;
	WebResource* resource;
	long contentSize;
	void* content;
	string contentType;
	string status;
	bool found;
	char buffer[1024];

	while(1) {

		/* ****************************************** */
		/* Attente des connexions client              */
		/* ****************************************** */

		clientSocket = 0;

		while(clientSocket == 0)
			clientSocket = SDLNet_TCP_Accept(serveurSocket);

		int requestSize = SDLNet_TCP_Recv(clientSocket, buffer, 1024); 				// Reception parametres connection du client

		try {

			/* ****************************************** */
			/* Décodage de la requête HTTP                */
			/* ****************************************** */

			HttpRequest request(buffer, requestSize);
			LOGDEBUG(("HTTP requête reçue : '%s'", request.toString().c_str()));


			/* ****************************************** */
			/* Recherche du contenu demandé               */
			/* ****************************************** */

			content = 0;
			found = false;
			tcpResponse.reset();

			// Search web service
			if(!found) {
				string baseEndpoint, serviceEndpoint;
				WebService* service = getService(request.getEndpoint(), baseEndpoint, serviceEndpoint);

				if(service) {
					WebServiceResult result = service->execute(request, baseEndpoint, serviceEndpoint);

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
				throw HttpException(HttpException::RESOURCE_NOT_FOUND_EXCEPTION, "No service or resource found");
			}
			else {
				buildResponse(tcpResponse, contentType, content, contentSize, status);
			}
		}
		catch(HttpException& exception) {
			switch(exception.getCode()) {
			case HttpException::RESOURCE_NOT_FOUND_EXCEPTION:
				LOGWARN(("Endpoint introuvable : '%s'", endpoint.c_str()));

				resource = getResource("/resource_not_found.html");

				if(resource) {
					buildResponse(tcpResponse, *resource, HTTP_RESPONSE_404);
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant) '%s' : %d", endpoint.c_str(), exception));
				}

				break;
			case HttpException::SERVICE_NOT_EXISTS:
				LOGWARN(("Service introuvable : '%s'", endpoint.c_str()));

				resource = getResource("/resource_not_found.html");

				if(resource) {
					buildResponse(tcpResponse, *resource, HTTP_RESPONSE_404);
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant) '%s' : %d", endpoint.c_str(), exception));
				}
				break;
			case HttpException::MALFORMED_HTTP_REQUEST:
				LOGWARN(("Requête http malformée reçue sur le service : '%s'", endpoint.c_str()));

				resource = getResource("/bad_http_request.html");

				if(resource) {
					buildResponse(tcpResponse, *resource, HTTP_RESPONSE_400);
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant) '%s' : %d", endpoint.c_str(), exception));
				}
				break;
			default:
				LOGERROR(("On ne devrait jamais être ici (erreur non-maîtrisée) '%s' : %d", endpoint.c_str(), exception));
				break;
			}
		}
		catch(MalformedJsonException& exception) {
			LOGWARN(("La requête HTTP reçue contient un Json malformé sur le service : '%s'", endpoint.c_str()));

			resource = getResource("/bad_http_request.html");

			if(resource) {
				buildResponse(tcpResponse, *resource, HTTP_RESPONSE_400);
			}
			else {
				LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant) '%s' : %d", endpoint.c_str(), exception));
			}
		}
		catch(std::exception& exception) {
			LOGERROR(("Erreur interne (exception standard) sur '%s' : '%s'", endpoint.c_str(), exception.what()));
		}
		catch(...) {
			LOGERROR(("Erreur interne inconnue sur '%s'", endpoint.c_str()));
		}

		if(tcpResponse.getSize() <= 0) {	// Si aucune réponse n'a été obtenue ici c'est qu'il y a une couille dans le potage
			buildResponse(tcpResponse, HTTP_CONTENT_TYPE_HTML, HTTP_INTERNAL_ERROR_CONTENT, HTTP_RESPONSE_500);
		}

		// Envoi réponse
		LOGDEBUG(("HTTP response : '%s'", tcpResponse.getContent()));

		SDLNet_TCP_Send(clientSocket, tcpResponse.getContent(), tcpResponse.getSize()); 		// Envoi du contenu de la page au client
		SDLNet_TCP_Close(clientSocket);
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

void HttpServer::buildResponse(HttpTcpResponse& tcpResponse, const string& contentType, const string& content, const string& status) {
	stringstream response;

	// Entête de réponse
	response << HTTP_HEAD << " " << status << HTTP_RETURN;
	response << "Access-Control-Allow-Origin: *" << HTTP_RETURN;	// TODO A supprimer, sert juste aux tests avec nodejs
	response << contentType << HTTP_RETURN;
	response << HTTP_CONTENT_LENGTH << content.size() << HTTP_RETURN;
	response << HTTP_RETURN;

	response << content;
	string str = response.str();

	tcpResponse.update( (char*)malloc(str.size()+1), str.size());
	str.copy(tcpResponse.getContent(), str.size());
	tcpResponse.getContent()[str.size()] = '\0';
}

void HttpServer::buildResponse(HttpTcpResponse& tcpResponse, const string& contentType, void* content, long contentSize, const string& status) {
	stringstream header;

	// Entête de réponse
	header << HTTP_HEAD << " " << status << HTTP_RETURN;
	header << "Access-Control-Allow-Origin: *" << HTTP_RETURN;	// TODO A supprimer, sert juste aux tests avec nodejs
	header << contentType << HTTP_RETURN;
	header << HTTP_CONTENT_LENGTH << contentSize << HTTP_RETURN;
	header << HTTP_RETURN;

	string headerStr = header.str();

	long responseSize = headerStr.size() + contentSize;
	tcpResponse.update((char*)malloc(responseSize+1), responseSize);

	headerStr.copy(tcpResponse.getContent(), headerStr.size());
	memcpy(tcpResponse.getContent() + (size_t)headerStr.size(), content, contentSize);
	tcpResponse.getContent()[responseSize] = '\0';
}

void HttpServer::buildResponse(HttpTcpResponse& tcpResponse, WebResource& webResource, const string& status) {
	buildResponse(tcpResponse, webResource.getContentType(), webResource.getContent(), webResource.getContentSize(), status);
}

}	// JktNet
