/*
 * HtmlServer.h
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_HTMLSERVER_H_
#define SRC_RESEAU_WEB_HTMLSERVER_H_

#include <map>

namespace JktNet
{

class WebResource {
public:
	string _file;
	string _contentType;
	string _content;

	WebResource();
	WebResource(const string& file, const string& contentType, const string& content);
};

class HtmlServer {
	static const char* WEB_STATIC_RESOURCES_DIR;
	static const char* WEB_STATIC_JSON_DIR;

	static const char* WEB_CSS_DIR;
	static const char* WEB_IMAGE_DIR;
	static const char* WEB_HTML_DIR;
	static const char* WEB_JS_DIR;
	static const char* WEB_JSON_DIR;

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

	static const char* HTTP_INTERNAL_ERROR_CONTENT;

	enum HTTP_EXCEPTION {
		RESOURCE_NOT_FOUND_EXCEPTION = 1
	};

	Uint16 _port;
	std::map<string, WebResource> _resources;

	void collecteDir(const string& dirname, const string& endpoint, const string& contentType);

	string buildResponse(const string& content, const string& contentType, const string& status);
	string buildResponse(const WebResource* resource, const string& status);
public:
	HtmlServer(int port);
	virtual ~HtmlServer();

	void open();

	static int run(void* arg);

	void start();

	WebResource* getResource(const string& endpoint) throw(int);
};

}	// JktNet

#endif /* SRC_RESEAU_WEB_HTMLSERVER_H_ */
