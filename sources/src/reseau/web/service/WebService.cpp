/*
 * WebService.cpp
 *
 *  Created on: 25 f�vr. 2016
 *      Author: VGDJ7997
 */

#include <string>

using namespace std;

#include "reseau/web/json/JsonObject.h"

#include "reseau/web/service/WebService.h"

namespace jkt {

WebServiceResult::WebServiceResult(JsonObject& root, const char* status) {
	_contentType = HttpServer::HTTP_CONTENT_TYPE_JSON;
	_status = status;

	string json = root.toString();

	_contentSize = json.size();
	_content = malloc(json.size());
	json.copy((char*)_content, json.size());
}

WebService::WebService() {
}

WebService::~WebService() {
}

WebServiceResult WebService::jsonErrorResponse(const char* httpStatus, const string& errorDetail) {
	JsonObject root;
	JsonObject& error = root.addObject("error");
	error.addString("detail", errorDetail);

	return WebServiceResult(root, httpStatus);
}


} /* namespace jkt */
