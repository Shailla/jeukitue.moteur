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

#include "SDL.h"
#include "SDL_net.h"

#include "util/StringUtils.h"
#include "util/Trace.h"
#include "util/FindFolder.h"
#include "reseau/web/HttpResponse.h"
#include "reseau/web/service/WebService.h"
#include "reseau/web/service/player/GetPlayersWS.h"
#include "reseau/web/service/map/GetMapElementWS.h"
#include "reseau/tcp/TcpServer.h"

#include "reseau/web/HttpServer.h"

using namespace std;

namespace jkt
{

TCPsocket HttpServer::_serveurSocket; // TODO A Virer

const char* HttpServer::WEB_CSS_DIR =				"./web/css";
const char* HttpServer::WEB_HTML_DIR =				"./web/html";
const char* HttpServer::WEB_IMAGE_DIR =				"./web/image";
const char* HttpServer::WEB_JS_DIR =				"./web/js";
const char* HttpServer::WEB_JSON_DIR =				"./web/json";

const char* HttpServer::HTTP_RETURN =				"\r\n";
const char* HttpServer::HTTP_HEAD = 				"HTTP/1.1";
const char* HttpServer::HTTP_RESPONSE_100 = 		"100 Continue";
const char* HttpServer::HTTP_RESPONSE_200 = 		"200 OK";
const char* HttpServer::HTTP_RESPONSE_201 = 		"201 Created";
const char* HttpServer::HTTP_RESPONSE_204 = 		"204 No Content";
const char* HttpServer::HTTP_RESPONSE_400 = 		"400 Bad Request";
const char* HttpServer::HTTP_RESPONSE_404 = 		"404 Not Found";
const char* HttpServer::HTTP_RESPONSE_500 = 		"500 Internal Error";
const char* HttpServer::HTTP_RESPONSE_501 = 		"501 Not Implemented";

const char* HttpServer::HTTP_PARAM_SEPARATOR = 		": ";

const char* HttpServer::HTTP_ACCESS_CONTROL_ALLOW_HEADERS = 	"Access-Control-Allow-Headers";
const char* HttpServer::HTTP_ACCESS_CONTROL_ALLOW_METHODS = 	"Access-Control-Allow-Methods";
const char* HttpServer::HTTP_ACCESS_CONTROL_ALLOW_ORIGIN = 		"Access-Control-Allow-Origin";
const char* HttpServer::HTTP_CACHE_CONTROL = 					"Cache-Control";
const char* HttpServer::HTTP_PRAGMA = 							"Pragma";
const char* HttpServer::HTTP_EXPIRES = 							"Expires";

const char* HttpServer::HTTP_CONTENT_LENGTH = 		"Content-Length";
const char* HttpServer::HTTP_CONTENT_TYPE = 		"Content-Type";

const char* HttpServer::HTTP_CONTENT_TYPE_HTML = 	"text/html; charset=utf-8";
const char* HttpServer::HTTP_CONTENT_TYPE_CSS = 	"text/css; charset=utf-8";
const char* HttpServer::HTTP_CONTENT_TYPE_IMAGE = 	"image";
const char* HttpServer::HTTP_CONTENT_TYPE_JS = 		"application/javascript";
const char* HttpServer::HTTP_CONTENT_TYPE_JSON = 	"application/json; charset=utf-8";

const char* HttpServer::HTTP_METHOD_OPTIONS =		"OPTIONS";
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

char* WebResource::getContent() {
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
		_content = (char*)malloc(_contentSize);

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

HttpServer::HttpServer(int port) : _tcpServer(port) {

	/* ************************************ */
	/* Réponse HTTP de base                 */
	/* ************************************ */

	_basicParameters.addParameter(HTTP_ACCESS_CONTROL_ALLOW_HEADERS, "Content-Type");
	_basicParameters.addParameter(HTTP_ACCESS_CONTROL_ALLOW_METHODS, "GET, HEAD, POST, PUT, OPTIONS, TRACE");
	_basicParameters.addParameter(HTTP_ACCESS_CONTROL_ALLOW_ORIGIN, "*");


	/* ************************************ */
	/* Déploiement des services web			*/
	/* ************************************ */

	_services["/rest/player-service/"] = new GetPlayersWS();
	_services["/rest/map-service/"] = new GetMapElementWS();


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
	else if(method == HTTP_METHOD_OPTIONS) {
		return HTTP_METHODS::HTTP_OPTIONS;
	}
	else {
		return HTTP_METHODS::HTTP_UNKNOWN;
	}
}

HttpSession* HttpServer::getHttpSession(TcpSession* tcpSession) {
	map<TcpSession*, HttpSession>::iterator it = _sessions.find(tcpSession);

	if(it != _sessions.end()) {
		return it->second;
	}
	else {
		return 0;
	}
}

void HttpServer::start() {
	HttpResponse tcpResponse;
	string header, methodStr, url, endpoint, params, protocol, path0;
	vector<string> paths;
	WebResource* resource;
	long contentSize;
	void* content;
	string contentType;
	string status;
	bool found;
	TcpPacket* packet;

	while(1) {

		/* ****************************************** */
		/* Attente des connexions client              */
		/* ****************************************** */

		packet = _tcpServer.receive();


		content = 0;
		found = false;
		tcpResponse.reset();

		try {

			/* ****************************************** */
			/* Réconciliation de la session HTTP          */
			/* ****************************************** */

			HttpSession* httpSession = getHttpSession(packet->getTcpSession());

			if(!httpSession) {
				httpSession = new HttpSession();
				_sessions[packet->getTcpSession()] = httpSession;
			}


			/* ****************************************** */
			/* Décodage de la requête HTTP                */
			/* ****************************************** */

			 HttpRequest* httpRequest = httpSession->getCurrentRequest();

			if(!httpRequest) {
				// New HTTP request
				httpRequest = new HttpRequest(packet);
			}
			else {
				// Continuing HTTP request
				httpRequest->complete(packet);
			}

			switch(httpRequest->getStatus()) {
			case HttpRequest::INCOMING:
				break;
			case HttpRequest::COMPLET:
				break;
			case HttpRequest::ERROR:
				break;
			default:
				LOGERROR(("On ne devrait jamais être ici %d", httpRequest->getStatus()));
				break;
			}

			endpoint = httpRequest->getEndpoint();

			if(Trace::instance().isLogLevelEnabled(TraceLevel::TRACE_LEVEL_DEBUG, __FILE__)) {
				LOGDEBUG(("\n\t* HTTP REQUEST full :\n\t********************************\n'%s'", packet->getData().c_str()));
			}
			else if(Trace::instance().isLogLevelEnabled(TraceLevel::TRACE_LEVEL_INFO, __FILE__)) {
				LOGINFO(("\n\t* HTTP RESPONSE synthesis :\n\t********************************\n'%s'\n'%s'", httpRequest->getVerb().c_str(), httpRequest->getBodyText().c_str()));
			}


			/* ****************************************** */
			/* Recherche du contenu demandé               */
			/* ****************************************** */

			// Search web service
			if(!found) {
				string baseEndpoint, serviceEndpoint;
				WebService* service = getService(httpRequest->getEndpoint(), baseEndpoint, serviceEndpoint);

				if(service) {
					if(httpRequest->getMethod() == HttpServer::HTTP_METHODS::HTTP_OPTIONS) {
						// Build response
						HttpParameters params(_basicParameters);
						params.addParameter("Access-Control-Max-Age", 86400);
						params.addParameter("Content-Length", 0);
						tcpResponse.update(HTTP_RESPONSE_204, params, "");
						found = true;
					}
					else {
						WebServiceResult result = service->execute(request, baseEndpoint, serviceEndpoint);

						contentType = result._contentType;
						contentSize = result._contentSize;
						content = result._content;
						status = result._status;

						// Build response
						buildResponse(tcpResponse, status, contentType, contentSize, content);
						found = true;
					}
				}
			}

			// Search static web resource
			if(!found) {
				resource = getResource(httpRequest->getEndpoint());	// Exception thrown if no resource found

				if(resource) {
					contentType = resource->getContentType();
					contentSize = resource->getContentSize();
					content = resource->getContent();
					status = HTTP_RESPONSE_200;

					// Build response
					buildResponse(tcpResponse, status, contentType, contentSize, content);
					found = true;
				}
			}

			// Nothing is found for the endpoint
			if(!found) {
				throw HttpException(HttpException::RESOURCE_NOT_FOUND_EXCEPTION, "No service or resource found");
			}
		}
		catch(HttpException& exception) {
			switch(exception.getCode()) {
			case HttpException::RESOURCE_NOT_FOUND_EXCEPTION:
				LOGWARN(("Endpoint introuvable : '%s'", endpoint.c_str()));

				resource = getResource("/resource_not_found.html");

				if(resource) {
					buildResponse(tcpResponse, HTTP_RESPONSE_404, *resource);
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant)"));
				}

				break;

			case HttpException::SERVICE_NOT_EXISTS:
				LOGWARN(("Service introuvable : '%s'", endpoint.c_str()));

				resource = getResource("/resource_not_found.html");

				if(resource) {
					buildResponse(tcpResponse, HTTP_RESPONSE_404, *resource);
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant)"));
				}
				break;

			case HttpException::MALFORMED_HTTP_REQUEST:
				LOGWARN(("Requête http malformée reçue sur le service : '%s'", endpoint.c_str()));

				resource = getResource("/bad_http_request.html");

				if(resource) {
					buildResponse(tcpResponse, HTTP_RESPONSE_400, *resource);
				}
				else {
					LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant)"));
				}
				break;

			default:
				LOGERROR(("On ne devrait jamais être ici (erreur non-maîtrisée) '%s' : %d", packet->getData().c_str(), exception.getCode()));
				break;
			}
		}
		catch(MalformedJsonException& exception) {
			LOGWARN(("La requête HTTP reçue contient un Json malformé sur le service : '%s'", endpoint.c_str()));

			resource = getResource("/bad_http_request.html");

			if(resource) {
				buildResponse(tcpResponse, HTTP_RESPONSE_400, *resource);
			}
			else {
				LOGERROR(("On ne devrait jamais être ici (fichier d'erreur HTML manquant) '%s' : %d", endpoint.c_str(), exception));
			}
		}
		catch(std::exception& exception) {
			LOGERROR(("Erreur interne (exception standard) sur '%s' : '%s'", packet->getData().c_str(), exception.what()));
		}
		catch(...) {
			LOGERROR(("Erreur interne inconnue sur '%s'", packet->getData().c_str()));
		}

		if(tcpResponse.isEmpty()) {	// Si aucune réponse n'a été obtenue ici c'est qu'il y a une couille dans le potage
			LOGERROR(("On ne devrait jamais être ici '%s'", packet->getData().c_str()));
			buildResponse(tcpResponse, HTTP_RESPONSE_500, HTTP_CONTENT_TYPE_HTML, HTTP_INTERNAL_ERROR_CONTENT);
		}

		// Envoi réponse
		if(Trace::instance().isLogLevelEnabled(TraceLevel::TRACE_LEVEL_DEBUG, __FILE__)) {
			LOGDEBUG(("\n\tHTTP RESPONSE full :\n\t********************************\n'%s'", tcpResponse.getResponseString().c_str()));
		}
		else if(Trace::instance().isLogLevelEnabled(TraceLevel::TRACE_LEVEL_INFO, __FILE__)) {
			LOGINFO(("\n\tHTTP RESPONSE header :\n\t********************************\n'%s'", tcpResponse.getHeader().c_str()));
		}

		_tcpServer.send(packegetTcpSessionession(), tcpResponse.getResponseContent(), tcpResponse.getResponseSize());		// Envoi du contenu de la page au client
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

void HttpServer::buildResponse(HttpResponse& tcpResponse, const string& status, const string& contentType, const string& content) {
	HttpParameters params(_basicParameters);
	params.addParameter(HTTP_CONTENT_TYPE, contentType);
	params.addParameter(HTTP_CONTENT_LENGTH, content.size());

	tcpResponse.update(status, params, content);
}

void HttpServer::buildResponse(HttpResponse& tcpResponse, const string& status, const string& contentType, long contentSize, void* content) {
	// Paramètres
	HttpParameters params(_basicParameters);
	params.addParameter(HTTP_CONTENT_TYPE, contentType);
	params.addParameter(HTTP_CONTENT_LENGTH, contentSize);

	tcpResponse.update(status, params, content, contentSize);
}

void HttpServer::buildResponse(HttpResponse& tcpResponse, const string& status, WebResource& webResource) {
	buildResponse(tcpResponse, status, webResource.getContentType(), webResource.getContentSize(), webResource.getContent());
}

}	// JktNet
