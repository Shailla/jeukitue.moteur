/*
 * WebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <string>

using namespace std;

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

} /* namespace jkt */
