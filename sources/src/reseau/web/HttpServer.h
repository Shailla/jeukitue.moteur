/*
 * HttpServer.h
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_HTTPSERVER_H_
#define SRC_RESEAU_WEB_HTTPSERVER_H_

#include <string>
#include <map>

#include "SDL.h"

namespace jkt
{

class WebService;

class WebResource {
	std::string _file;
	std::string _contentType;
	void* _content;
	long _contentSize;
public:
	WebResource();
	WebResource(const std::string& file, const std::string& contentType);
	~WebResource();

	void load();
	std::string getContentType();

	/** Get of the web resource, content is cached at first read */
	void* getContent();

	/** Get size of the web resource, content is cached at first read */
	long getContentSize();
};

class HttpServer {
	static const char* WEB_STATIC_RESOURCES_DIR;
	static const char* WEB_STATIC_JSON_DIR;

	static const char* WEB_CSS_DIR;
	static const char* WEB_IMAGE_DIR;
	static const char* WEB_HTML_DIR;
	static const char* WEB_JS_DIR;
	static const char* WEB_JSON_DIR;

	static const char* HTTP_INTERNAL_ERROR_CONTENT;

	enum HTTP_EXCEPTION {
		RESOURCE_NOT_FOUND_EXCEPTION = 1
	};

	Uint16 _port;
	std::map<std::string, WebResource*> _resources;
	std::map<std::string, WebService*> _services;

	void collecteDir(const std::string& dirname, const std::string& endpoint, const std::string& contentType);

	std::string buildStringResponse(const std::string& content, const std::string& contentType, const std::string& status);
	std::string buildResponseHeader(const std::string& contentType, long contentSize, const std::string& status);

	void* buildResponse();
public:
	static const char* HTTP_RETURN;
	static const char* HTTP_HEAD;
	static const char* HTTP_RESPONSE_200;
	static const char* HTTP_RESPONSE_404;
	static const char* HTTP_RESPONSE_500;
	static const char* HTTP_CONTENT_TYPE_HTML;
	static const char* HTTP_CONTENT_TYPE_CSS;
	static const char* HTTP_CONTENT_TYPE_IMAGE;
	static const char* HTTP_CONTENT_TYPE_JS;
	static const char* HTTP_CONTENT_TYPE_JSON;
	static const char* HTTP_CONTENT_LENGTH;

	HttpServer(int port);
	virtual ~HttpServer();

	void open();

	static int run(void* arg);

	void start();

	WebService* getService(const std::string& fullEndpoint, std::string& baseEndpoint, std::string& serviceEndpoint);
	WebResource* getResource(const std::string& endpoint) throw(int);
};

}	// JktNet

#endif /* SRC_RESEAU_WEB_HTTPSERVER_H_ */
