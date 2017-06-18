/*
 * HttpResponse.h
 *
 *  Created on: 29 mai 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_WEB_HTTPRESPONSE_H_
#define SRC_RESEAU_WEB_HTTPRESPONSE_H_

#include <string>
#include <sstream>

#include "reseau/web/HttpParameters.h"

namespace jkt {

class HttpResponse {
	std::string _status;

	HttpParameters _parameters;

	std::string _body;

	long _responseSize;
	void* _responseContent;

	void generateResponse();

public:
	HttpResponse();
	~HttpResponse();

	void updateToEmptyPaquet();
	void update(const std::string& status, const HttpParameters& parameters, const std::string& body);
	void update(const std::string& status, const HttpParameters& parameters, const void* bodyContent, long bodySize);
	void reset();

	bool isEmpty() const;
	long getResponseSize() const;
	std::string getResponseString() const;
	void* getResponseContent() const;


	std::string getStatus() const;
	std::string getHeader();
	std::string getBody() const;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_HTTPRESPONSE_H_ */
