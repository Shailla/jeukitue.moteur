/*
 * WebService.h
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SERVICE_WEBSERVICE_H_
#define SRC_RESEAU_WEB_SERVICE_WEBSERVICE_H_

#include <string>
#include <vector>

#include "reseau/web/json/JsonObject.h"
#include "reseau/web/HttpServer.h"
#include "reseau/web/HttpRequest.h"

namespace jkt {

class WebServiceResult {
public:
	WebServiceResult();
	WebServiceResult(JsonObject& root, const char* status);
	WebServiceResult(JsonObject& root, const std::vector<string>& errors, const char* status);

	std::string _status;
	std::string _contentType;
	void* _content;
	long _contentSize;
};


class WebService {
public:
	WebService();
	virtual ~WebService();

	virtual WebServiceResult execute(HttpRequest& request, const std::string& baseEndpoint, const std::string& serviceEndpoint) throw(HttpException) = 0;

	WebServiceResult jsonErrorResponse(const char* httpStatus, const std::string& errorDetail);
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_SERVICE_WEBSERVICE_H_ */
