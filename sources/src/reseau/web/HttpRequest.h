/*
 * HttpRequest.h
 *
 *  Created on: 29 d�c. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_HTTPREQUEST_H_
#define SRC_RESEAU_WEB_HTTPREQUEST_H_

#include <string>

#include "reseau/web/json/JsonObject.h"
#include "reseau/web/json/MalformedJsonException.h"
#include "reseau/web/HttpException.h"
#include "reseau/web/HttpServer.h"
#include "reseau/tcp/TcpPacket.h"

namespace jkt {

class HttpRequest {
public:
	enum STATUS {
		PINCOMPLET = 0,
		PCOMPLET,
		PERROR
	};

private:
	std::string _header;
	std::string _body;

	HttpServer::HTTP_METHODS _method;
	std::string _url;
	std::string _endpoint;
	std::string _urlParameters;
	std::string _protocol;
	int _bodyContentLength;
	std::map<std::string, std::string> _httpParameters;

	STATUS _status;

public:
	HttpRequest(TcpPacket* tcpPacket) noexcept(false);
	virtual ~HttpRequest();

	void complete(TcpPacket* tcpPacket);

	HttpServer::HTTP_METHODS getMethod() const;
	const std::string& getEndpoint() const;

	STATUS getStatus() const;

	std::string getVerb() const;
	std::string getBodyText() const;
	JsonObject* getBodyJson() const noexcept(false);
};

}

#endif /* SRC_RESEAU_WEB_HTTPREQUEST_H_ */
