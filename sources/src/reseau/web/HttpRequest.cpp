/*
 * HttpRequest.cpp
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#include "util/Trace.h"
#include "util/StringUtils.h"

#include "reseau/web/HttpRequest.h"

using namespace std;

namespace jkt {

const char* HttpRequest::SPLIT_HTTP_HEADER_BODY = 	"\r\n\r\n";

HttpRequest::HttpRequest(TcpPacket* packet) throw(HttpException) {

	/* ************************************************************** */
	/* Sépare les paramètres HTTP de son body                         */
	/* ************************************************************** */

	LOGDEBUG(("\n\t* TCP REQUEST (%d bytes) :\n\t********************************'%s'", packet->getData().c_str()));

	std::size_t splitHttpHeaderBody = packet->getData().find(SPLIT_HTTP_HEADER_BODY);

	if(splitHttpHeaderBody == string::npos) {
		throw HttpException(HttpException::MALFORMED_HTTP_REQUEST, "Cannot split http header and body");
	}

	_header = packet->getData().substr(0, splitHttpHeaderBody);
	_body = packet->getData().substr(splitHttpHeaderBody + 4);


	/* ************************************************************** */
	/** Extrait les info du header HTTP                               */
	/* ************************************************************** */

	// Méthode HTTP
	string methodStr = jkt::StringUtils::findAndEraseFirstWord(_header);

	_method = HttpServer::resolveHttpMethod(methodStr);

	// Endpoint et paramètres
	_url = jkt::StringUtils::findAndEraseFirstWord(_header);

	size_t bp = _url.find("?");
	_endpoint = _url.substr(0, bp);

	if(bp != string::npos) {
		_parameters = _url.substr(bp+1);
	}
	else {
		_parameters = "";	// Pas de paramètres sur la requête
	}

	// Protocole
	_protocol = jkt::StringUtils::findAndEraseFirstWord(_header);

	if(Trace::instance().isLogLevelEnabled(TraceLevel::TRACE_LEVEL_DEBUG, __FILE__)) {
		stringstream log;

		log << "\n\t- Méthode :  '" << methodStr 	<< "' (" << _method << ")";
		log << "\n\t- Endpoint : '" << _endpoint 	<< "'";
		log << "\n\t- Params:    '" << _parameters 	<< "'";
		log << "\n\t- Protocol : '" << _protocol 	<< "'";
		log << "\n\t- Protocol : '" << _protocol 	<< "'";

		LOGDEBUG(("http paquet received synthesis :%s", log.str().c_str()));
	}
}

HttpRequest::~HttpRequest() {
}

HttpServer::HTTP_METHODS HttpRequest::getMethod() const {
	return _method;
}

const string& HttpRequest::getEndpoint() const {
	return _endpoint;
}

const string& HttpRequest::toString() const {
//	return _request;
}

string HttpRequest::getVerb() const {
	stringstream verb;

	switch(_method) {
	case HttpServer::HTTP_UNKNOWN:
		verb << "Méthode HTTP inconnue";
		break;
	case HttpServer::HTTP_GET:
		verb << "GET";
		break;
	case HttpServer::HTTP_POST:
		verb << "POST";
		break;
	case HttpServer::HTTP_PUT:
		verb << "PUT";
		break;
	case HttpServer::HTTP_PATCH:
		verb << "PACTH";
		break;
	case HttpServer::HTTP_DELETE:
		verb << "DELETE";
		break;
	case HttpServer::HTTP_OPTIONS:
		verb << "OPTIONS";
		break;
	default:
		verb << "Méthode HTTP inconnue";
		LOGERROR(("On ne devrait jamais être ici"));
		break;
	}

	verb << " " << _endpoint;

	return verb.str();
}

string HttpRequest::getBodyText() const {
	return _body;
}

JsonObject* HttpRequest::getBodyJson() const throw(MalformedJsonException) {
	return JsonObject::fromJson(_body);
}

} // namespace jkt
