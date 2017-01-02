/*
 * HttpServer.h
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_HTTPSERVER_H_
#define SRC_RESEAU_WEB_HTTPSERVER_H_

#include <regex>
#include <string>
#include <map>

#include "SDL.h"

#include "reseau/web/HttpException.h"

namespace jkt
{

class WebService;

class HttpTcpResponse {
	long _size;
	char* _content;
public:
	HttpTcpResponse();
	~HttpTcpResponse();

	void update(char* content, long size);
	void reset();

	long getSize() const;
	char* getContent() const;
};

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
public:
	enum HTTP_METHODS {
		HTTP_UNKNOWN,
		HTTP_GET,
		HTTP_POST,
		HTTP_PUT,
		HTTP_PATCH,
		HTTP_DELETE
	};
private:
	static const char* WEB_STATIC_RESOURCES_DIR;
	static const char* WEB_STATIC_JSON_DIR;

	static const char* WEB_CSS_DIR;
	static const char* WEB_IMAGE_DIR;
	static const char* WEB_HTML_DIR;
	static const char* WEB_JS_DIR;
	static const char* WEB_JSON_DIR;

	static const char* HTTP_INTERNAL_ERROR_CONTENT;

	Uint16 _port;
	std::map<std::string, WebResource*> _resources;
	std::map<std::string, WebService*> _services;

	void collecteDir(const std::string& dirname, const std::string& endpoint, const std::string& contentType);

	void buildResponse(HttpTcpResponse& tcpResponse, const std::string& contentType, const std::string& content, const std::string& status);
	void buildResponse(HttpTcpResponse& tcpResponse, const std::string& contentType, void* content, long contentSize, const std::string& status);
	void buildResponse(HttpTcpResponse& tcpResponse, WebResource& webResource, const std::string& status);


	void* buildResponse();

public:
	static const char* HTTP_RETURN;
	static const char* HTTP_HEAD;
	static const char* HTTP_RESPONSE_200;
	static const char* HTTP_RESPONSE_400;
	static const char* HTTP_RESPONSE_404;
	static const char* HTTP_RESPONSE_500;
	static const char* HTTP_CONTENT_TYPE_HTML;
	static const char* HTTP_CONTENT_TYPE_CSS;
	static const char* HTTP_CONTENT_TYPE_IMAGE;
	static const char* HTTP_CONTENT_TYPE_JS;
	static const char* HTTP_CONTENT_TYPE_JSON;
	static const char* HTTP_CONTENT_LENGTH;

	static const char* HTTP_METHOD_GET;
	static const char* HTTP_METHOD_POST;
	static const char* HTTP_METHOD_PUT;
	static const char* HTTP_METHOD_PATCH;
	static const char* HTTP_METHOD_DELETE;

	static HTTP_METHODS resolveHttpMethod(const std::string& method);

	HttpServer(int port);
	virtual ~HttpServer();

	void open();

	static int run(void* arg);

	void start();

	WebService* getService(const std::string& fullEndpoint, std::string& baseEndpoint, std::string& serviceEndpoint);
	WebResource* getResource(const std::string& endpoint) throw(int);
};

}	// jkt

#endif /* SRC_RESEAU_WEB_HTTPSERVER_H_ */
