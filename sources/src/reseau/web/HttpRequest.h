/*
 * HttpRequest.h
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_HTTPREQUEST_H_
#define SRC_RESEAU_WEB_HTTPREQUEST_H_

#include <string>

#include "reseau/web/json/JsonObject.h"
#include "reseau/web/json/MalformedJsonException.h"
#include "reseau/web/HttpException.h"
#include "reseau/web/HttpServer.h"

namespace jkt {

class HttpRequest {
	static const char* SPLIT_HTTP_HEADER_BODY;

	std::string _request;

	HttpServer::HTTP_METHODS _method;
	std::string _url;
	std::string _endpoint;
	std::string _parameters;
	std::string _protocol;

	std::string _header;
	std::string _body;

public:
	HttpRequest(const char* request, int requestSize) throw(HttpException);
	virtual ~HttpRequest();

	HttpServer::HTTP_METHODS getMethod() const;
	const string& getEndpoint() const;

	std::string getBodyText() const;
	JsonObject* getBodyJson() const throw(MalformedJsonException);

	const std::string& toString() const;
};

}

#endif /* SRC_RESEAU_WEB_HTTPREQUEST_H_ */
