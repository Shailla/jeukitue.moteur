/*
 * HttpRequest.cpp
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#include "util/Trace.h"
#include "util/StringUtils.h"
#include "reseau/web/HttpVocabulary.h"

#include "reseau/web/HttpRequest.h"

using namespace std;

namespace jkt {

HttpRequest::HttpRequest(TcpPacket* packet) throw(HttpException) {

	_bodyContentLength = 0;


	/* ************************************************************** */
	/* Sépare les paramètres HTTP de son body                         */
	/* ************************************************************** */

	LOGDEBUG(("\n\t* TCP REQUEST (%d bytes) :\n\t********************************'%s'", packet->getData().size(), packet->getData().c_str()));

	std::size_t splitHttpHeaderBody = packet->getData().find(HttpVocabulary::HTTP_HEADER_BODY_SEPARATOR);

	if(splitHttpHeaderBody == string::npos) {
		throw HttpException(HttpException::MALFORMED_HTTP_REQUEST, "Cannot split http header and body");
	}

	_header = packet->getData().substr(0, splitHttpHeaderBody);


	/* ************************************************************** */
	/** Extrait les info du header HTTP                               */
	/* ************************************************************** */

	vector<string> headers = StringUtils::split(_header, HttpVocabulary::HTTP_RETURN);

	if(headers.size() == 0) {
		throw HttpException(HttpException::MALFORMED_HTTP_REQUEST, "Header is empty !");
	}

	string firstLine = headers[0];
	headers.erase(headers.begin());	// Remove first line


	// ************************************************************************************
	// Lecture de la première ligne (méthode, url/paramètres et protocole)

	// Méthode HTTP
	string methodStr = StringUtils::findAndEraseFirstWord(firstLine);

	_method = HttpServer::resolveHttpMethod(methodStr);

	// Endpoint et paramètres
	_url = jkt::StringUtils::findAndEraseFirstWord(firstLine);

	size_t bp = _url.find("?");
	_endpoint = _url.substr(0, bp);

	if(bp != string::npos) {
		_urlParameters = _url.substr(bp+1);
	}
	else {
		_urlParameters = "";	// Pas de paramètres sur la requête
	}

	// Protocole
	_protocol = jkt::StringUtils::findAndEraseFirstWord(firstLine);


	// ************************************************************************************
	// Lecture des autres paramètres

	string name, value;

	for(string line : headers) {
		std::size_t splitLine = line.find(HttpVocabulary::HTTP_PARAM_SEPARATOR);

		if(splitLine == string::npos) {
			LOGWARN(("Ligne de paramètre http malformée '%s'", line.c_str()));
		}
		else {
			name = line.substr(0, splitLine);
			value = line.substr(splitLine + 1);

			StringUtils::trim(name);
			StringUtils::toLower(name);	// Http headers are insensitive case (RFC-2616)

			StringUtils::trim(value);

			if(name.size() > 0) {
				_httpParameters[name] = value;
			}
			else {
				LOGWARN(("Ligne de paramètre http malformée '%s'", line.c_str()));
			}
		}
	}

	map<string, string>::iterator contentLength = _httpParameters.find(HttpVocabulary::HTTP_CONTENT_LENGTH);

	if(contentLength != _httpParameters.end()) {
		char* temp;
		long val = strtol(contentLength->second.c_str(), &temp, 0);

		if(*temp == '\0') {
			_bodyContentLength = val;
		}
		else {
			throw HttpException(HttpException::MALFORMED_HTTP_REQUEST, "Malformed content-length !");
		}
	}

	_status = PCOMPLET;


	/* ************************************************************** */
	/** Lecture du body                                               */
	/* ************************************************************** */

	if(_bodyContentLength > 0) {	// S'il y a un body
		// Si le body est complet prends contentLength octets
		if(packet->getData().size() >= splitHttpHeaderBody + 4 + _bodyContentLength) {
			_body = packet->getData().substr(splitHttpHeaderBody + 4, _bodyContentLength);
			_status = PCOMPLET;
		}
		// Sinon prends ce qui est présent, on complètera avec le paquet TCP suivant
		else {
			_body = packet->getData().substr(splitHttpHeaderBody + 4);
			_status = PINCOMPLET;
		}
	}


	/* ************************************************************** */
	/** Log                                                           */
	/* ************************************************************** */

	if(Trace::instance().isLogLevelEnabled(TraceLevel::TRACE_LEVEL_DEBUG, __FILE__)) {
		stringstream log;

		log << "\n\t- Méthode :        '" << methodStr 	<< "' (" << _method << ")";
		log << "\n\t- Endpoint :       '" << _endpoint 	<< "'";
		log << "\n\t- Params:          '" << _urlParameters 	<< "'";
		log << "\n\t- Protocol :       '" << _protocol 	<< "'";
		log << "\n\t- Content-Length : '" << _bodyContentLength 	<< "'";
		log << "\n\t- Taille body :    '" << _body.size() << "'";

		switch(_status) {
		case PINCOMPLET:
			log << "\n\t- Statut :         'Incomplet'";
			break;
		case PCOMPLET:
			log << "\n\t- Statut :         'Complet'";
			break;
		case PERROR:
			log << "\n\t- Statut :         'Erreur'";
			break;
		default:
			log << "\n\t- Statut :         'Inconnu'";
			break;
		}

		LOGDEBUG(("http paquet received synthesis :%s", log.str().c_str()));
	}
}

HttpRequest::~HttpRequest() {
}

void HttpRequest::complete(TcpPacket* packet) {
	if(_status != PINCOMPLET) {
		LOGERROR(("On ne devrait jamais être ici"));
		_status = PERROR;
	}
	else {
		size_t missingData = _bodyContentLength - _body.size();

		if(missingData > packet->getData().size()) {	// Complète déjà avec ce qui est présent, on complètera avec le paquet TCP suivant
			_body += packet->getData();
			_status = PINCOMPLET;	// We wait stil more data
		}
		else {
			_body += packet->getData().substr(0, missingData);	// Prends tout ce qui manque et le paquet est complet
			_status = PCOMPLET;	// Http request is complete
		}
	}
}

HttpServer::HTTP_METHODS HttpRequest::getMethod() const {
	return _method;
}

const string& HttpRequest::getEndpoint() const {
	return _endpoint;
}

HttpRequest::STATUS HttpRequest::getStatus() const {
	return _status;
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
