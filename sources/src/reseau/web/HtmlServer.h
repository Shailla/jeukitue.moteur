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

class HtmlServer {
	static const char* WEB_STATIC_RESOURCES_DIR;
	static const char* HTTP_RETURN;
	static const char* HTTP_HEAD;
	static const char* HTTP_RESPONSE_OK;
	static const char* HTTP_CONTENT_HTML;
	static const char* HTTP_CONTENT_LENGTH;

	Uint16 _port;
	std::map<string, string> _pages;
public:
	HtmlServer(int port);
	virtual ~HtmlServer();

	void open();

	static int run(void* arg);

	void start();
	string getPage(string url);
};

}	// JktNet

#endif /* SRC_RESEAU_WEB_HTMLSERVER_H_ */
