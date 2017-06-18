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
#include "SDL_net.h"

#include "reseau/web/HttpParameters.h"
#include "reseau/web/HttpException.h"
#include "reseau/tcp/TcpServer.h"

namespace jkt
{

class WebService;
class HttpResponse;

class WebResource {
	std::string _file;
	std::string _contentType;
	char* _content;
	long _contentSize;
public:
	WebResource();
	WebResource(const std::string& file, const std::string& contentType);
	~WebResource();

	void load();
	std::string getContentType();

	/** Get of the web resource, content is cached at first read */
	char* getContent();

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
		HTTP_DELETE,
		HTTP_OPTIONS
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

	TcpServer _tcpServer;
	HttpParameters _basicParameters;
	std::map<TcpSession*, HttpSession> _sessions;

	std::map<std::string, WebResource*> _resources;
	std::map<std::string, WebService*> _services;

	void collecteDir(const std::string& dirname, const std::string& endpoint, const std::string& contentType);

	void buildResponse(HttpResponse& tcpResponse, const std::string& status, const std::string& contentType, const std::string& content);
	void buildResponse(HttpResponse& tcpResponse, const std::string& status, const std::string& contentType, long contentSize, void* content);
	void buildResponse(HttpResponse& tcpResponse, const std::string& status, WebResource& webResource);
	void* buildResponse();

	HttpSession* getHttpSession(TcpSession* tcpSession);

public:
	static TCPsocket _serveurSocket;

	static const char* HTTP_RETURN;
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
