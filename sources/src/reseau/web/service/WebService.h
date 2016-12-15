/*
 * WebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_WEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_WEBSERVICE_H_

#include <string>

#include "reseau/web/HttpServer.h"

namespace jkt {

class WebServiceResult {
public:
	std::string _status;
	std::string _contentType;
	void* _content;
	long _contentSize;
};


class WebService {
public:
	WebService();
	virtual ~WebService();

	virtual WebServiceResult execute(HttpServer::HTTP_METHODS method, const std::string& fullEndpoint, const std::string& baseEndpoint, const std::string& serviceEndpoint, const std::string& params) = 0;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_WEBSERVICE_H_ */
