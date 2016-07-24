/*
 * HtmlServer.h
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_HTMLSERVER_H_
#define SRC_RESEAU_WEB_HTMLSERVER_H_

#include <map>

#include "SDL.h"

namespace jkt
{

class WebService;

class WebResource {
	string _file;
	string _contentType;
	void* _content;
	long _contentSize;
public:
	WebResource();
	WebResource(const string& file, const string& contentType);
	~WebResource();

	void load();
	string getContentType();

	/** Get of the web resource, content is cached at first read */
	void* getContent();

	/** Get size of the web resource, content is cached at first read */
	long getContentSize();
};

class HtmlServer {
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
	std::map<string, WebResource*> _resources;
	std::map<string, WebService*> _services;

	void collecteDir(const string& dirname, const string& endpoint, const string& contentType);

	string buildStringResponse(const string& content, const string& contentType, const string& status);
	string buildResponseHeader(const string& contentType, long contentSize, const string& status);

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

	HtmlServer(int port);
	virtual ~HtmlServer();

	void open();

	static int run(void* arg);

	void start();

	WebService* getService(const string& endpoint);
	WebResource* getResource(const string& endpoint) throw(int);
};

}	// JktNet

#endif /* SRC_RESEAU_WEB_HTMLSERVER_H_ */
