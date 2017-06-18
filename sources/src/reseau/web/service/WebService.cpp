/*
 * WebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <string>

#include "reseau/web/json/JsonObject.h"

#include "reseau/web/service/WebService.h"

using namespace std;

namespace jkt {

WebServiceResult::WebServiceResult(const char* status) {
	_contentType = HttpServer::HTTP_CONTENT_TYPE_JSON;
	_status = status;

	string body = "";

	_contentSize = body.size();
	_content = malloc(body.size());
	body.copy((char*)_content, body.size());
}

WebServiceResult::WebServiceResult(JsonObject& root, const char* status) {
	_contentType = HttpServer::HTTP_CONTENT_TYPE_JSON;
	_status = status;

	string body = root.toString();

	_contentSize = body.size();
	_content = malloc(body.size());
	body.copy((char*)_content, body.size());
}

WebServiceResult::WebServiceResult(JsonObject& root, const vector<string>& errors, const char* status) {
	_contentType = HttpServer::HTTP_CONTENT_TYPE_JSON;
	_status = status;

	JsonList& errorList = root.addList("errors");

	for(const string error : errors) {
		errorList.addString(error);
	}

	string body = root.toString();

	_contentSize = body.size();
	_content = malloc(body.size());
	body.copy((char*)_content, body.size());
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
