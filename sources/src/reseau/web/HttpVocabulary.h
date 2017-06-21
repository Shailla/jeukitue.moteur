/*
 * HttpVocabulary.h
 *
 *  Created on: 20 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_WEB_HTTPVOCABULARY_H_
#define SRC_RESEAU_WEB_HTTPVOCABULARY_H_

namespace jkt {

class HttpVocabulary {
	HttpVocabulary();
public:
	virtual ~HttpVocabulary();

	static const char* HTTP_RETURN;
	static const char* HTTP_HEADER_BODY_SEPARATOR;
	static const char* HTTP_HEAD;
	static const char* HTTP_RESPONSE_100;
	static const char* HTTP_RESPONSE_200;
	static const char* HTTP_RESPONSE_201;
	static const char* HTTP_RESPONSE_204;
	static const char* HTTP_RESPONSE_400;
	static const char* HTTP_RESPONSE_404;
	static const char* HTTP_RESPONSE_500;
	static const char* HTTP_RESPONSE_501;

	static const char* HTTP_PARAM_SEPARATOR;

	static const char* HTTP_ACCESS_CONTROL_ALLOW_HEADERS;
	static const char* HTTP_ACCESS_CONTROL_ALLOW_METHODS;
	static const char* HTTP_ACCESS_CONTROL_ALLOW_ORIGIN;
	static const char* HTTP_CACHE_CONTROL;
	static const char* HTTP_PRAGMA;
	static const char* HTTP_EXPIRES;

	static const char* HTTP_CONTENT_TYPE;
	static const char* HTTP_CONTENT_TYPE_HTML;
	static const char* HTTP_CONTENT_TYPE_CSS;
	static const char* HTTP_CONTENT_TYPE_IMAGE;
	static const char* HTTP_CONTENT_TYPE_JS;
	static const char* HTTP_CONTENT_TYPE_JSON;
	static const char* HTTP_CONTENT_LENGTH;

	static const char* HTTP_METHOD_OPTIONS;
	static const char* HTTP_METHOD_GET;
	static const char* HTTP_METHOD_POST;
	static const char* HTTP_METHOD_PUT;
	static const char* HTTP_METHOD_PATCH;
	static const char* HTTP_METHOD_DELETE;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_HTTPVOCABULARY_H_ */
