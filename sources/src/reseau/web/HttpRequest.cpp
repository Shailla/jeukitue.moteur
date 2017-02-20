/*
 * HttpRequest.cpp
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

using namespace std;

#include "util/Trace.h"
#include "util/StringUtils.h"

#include "reseau/web/HttpRequest.h"

namespace jkt {

const char* HttpRequest::SPLIT_HTTP_HEADER_BODY = 	"\r\n\r\n";

HttpRequest::HttpRequest(const char* request, int requestSize) throw(HttpException) : _request(request, requestSize) {

	/* ************************************************************** */
	/* Sépare les paramètres HTTP de son body                         */
	/* ************************************************************** */

	LOGINFO(("REQUETE:%s", _request.c_str()));

	std::size_t splitHttpHeaderBody = _request.find(SPLIT_HTTP_HEADER_BODY);

	if(splitHttpHeaderBody == string::npos) {
		throw HttpException(HttpException::MALFORMED_HTTP_REQUEST, "Cannot split http header and body");
	}

	_header = _request.substr(0, splitHttpHeaderBody);
	_body = _request.substr(splitHttpHeaderBody + 4);


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

	LOGDEBUG((" - Méthode :  '%s (%d)'", methodStr.c_str(), (int)_method));
	LOGDEBUG((" - Endpoint : '%s'", _endpoint.c_str()));
	LOGDEBUG((" - Params:    '%s'", _parameters.c_str()));
	LOGDEBUG((" - Protocol : '%s'", _protocol.c_str()));
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
	return _request;
}

string HttpRequest::getBodyText() const {
	return _body;
}

JsonObject* HttpRequest::getBodyJson() const throw(MalformedJsonException) {
	return JsonObject::fromJson(_body);
}

} // namespace jkt
