/*
 * HttpVocabulary.cpp
 *
 *  Created on: 20 juin 2017
 *      Author: Erwin
 */

#include "reseau/web/HttpVocabulary.h"

namespace jkt {

const char* HttpVocabulary::HTTP_RETURN =				"\r\n";
const char* HttpVocabulary::HTTP_HEADER_BODY_SEPARATOR ="\r\n\r\n";

const char* HttpVocabulary::HTTP_HEAD = 				"HTTP/1.1";
const char* HttpVocabulary::HTTP_RESPONSE_100 = 		"100 Continue";
const char* HttpVocabulary::HTTP_RESPONSE_200 = 		"200 OK";
const char* HttpVocabulary::HTTP_RESPONSE_201 = 		"201 Created";
const char* HttpVocabulary::HTTP_RESPONSE_204 = 		"204 No Content";
const char* HttpVocabulary::HTTP_RESPONSE_400 = 		"400 Bad Request";
const char* HttpVocabulary::HTTP_RESPONSE_404 = 		"404 Not Found";
const char* HttpVocabulary::HTTP_RESPONSE_500 = 		"500 Internal Error";
const char* HttpVocabulary::HTTP_RESPONSE_501 = 		"501 Not Implemented";

const char* HttpVocabulary::HTTP_PARAM_SEPARATOR = 		":";

const char* HttpVocabulary::HTTP_ACCESS_CONTROL_ALLOW_HEADERS = 	"access-control-allow-headers";
const char* HttpVocabulary::HTTP_ACCESS_CONTROL_ALLOW_METHODS = 	"access-control-allow-methods";
const char* HttpVocabulary::HTTP_ACCESS_CONTROL_ALLOW_ORIGIN = 		"access-control-allow-origin";
const char* HttpVocabulary::HTTP_CACHE_CONTROL = 					"cache-control";
const char* HttpVocabulary::HTTP_PRAGMA = 							"pragma";
const char* HttpVocabulary::HTTP_EXPIRES = 							"expires";

const char* HttpVocabulary::HTTP_CONTENT_LENGTH = 		"content-length";
const char* HttpVocabulary::HTTP_CONTENT_TYPE = 		"content-type";

const char* HttpVocabulary::HTTP_CONTENT_TYPE_HTML = 	"text/html; charset=utf-8";
const char* HttpVocabulary::HTTP_CONTENT_TYPE_CSS = 	"text/css; charset=utf-8";
const char* HttpVocabulary::HTTP_CONTENT_TYPE_IMAGE = 	"image";
const char* HttpVocabulary::HTTP_CONTENT_TYPE_JS = 		"application/javascript";
const char* HttpVocabulary::HTTP_CONTENT_TYPE_JSON = 	"application/json; charset=utf-8";

const char* HttpVocabulary::HTTP_METHOD_OPTIONS =		"OPTIONS";
const char* HttpVocabulary::HTTP_METHOD_GET = 			"GET";
const char* HttpVocabulary::HTTP_METHOD_POST = 			"POST";
const char* HttpVocabulary::HTTP_METHOD_PUT = 			"PUT";
const char* HttpVocabulary::HTTP_METHOD_PATCH = 		"PATCH";
const char* HttpVocabulary::HTTP_METHOD_DELETE =		"DELETE";

HttpVocabulary::HttpVocabulary() {
}

HttpVocabulary::~HttpVocabulary() {
}

} /* namespace jkt */
