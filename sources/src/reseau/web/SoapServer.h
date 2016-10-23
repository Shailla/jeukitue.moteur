/*
 * SoapServer.h
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_SOAPSERVER_H_
#define SRC_RESEAU_WEB_SOAPSERVER_H_

#include <map>

#include "SDL.h"

namespace jkt
{

class SoapRequest {
public:
	long timeRequestSent;
	long timeRequestAck;

	long timeResponse;

	SoapRequest(long timeRequestSent) {
		this->timeRequestSent = timeRequestSent;

		timeRequestAck = 0;
		timeResponse = 0;
	}
};


class SoapServer {
	static const char* WEB_STATIC_RESOURCES_DIR;
	static const char* WEB_STATIC_JSON_DIR;

	static const char* WEB_CSS_DIR;
	static const char* WEB_IMAGE_DIR;
	static const char* WEB_HTML_DIR;
	static const char* WEB_JS_DIR;
	static const char* WEB_JSON_DIR;

	static const char* HTTP_INTERNAL_ERROR_CONTENT;

	static const char* DEMANDE_SIGNATURE_1;
	static const char* DEMANDE_SIGNATURE_2;
	static const char* DEMANDE_SIGNATURE_3;

	std::map<long, SoapRequest*> _requests;

	enum HTTP_EXCEPTION {
		RESOURCE_NOT_FOUND_EXCEPTION = 1
	};

	Uint16 _portClient;
	Uint16 _portServer;

	string buildStringResponse(const string& content, const string& contentType, const string& status);
	string buildResponseHeader(const string& contentType, long contentSize, const string& status);

	void* buildResponse();

	TCPsocket _socketClient;
	TCPsocket _socketServer;
	SDL_mutex* _mutex;

	bool _activated;

	long extractRequestId(string& response);

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

	SoapServer();
	virtual ~SoapServer();

	void open(int portClient, int portServer);
	void start();
	void stop();

	static int runServer(void* arg);
	static int runClientSender(void* arg);
	static int runClientReceiver(void* arg);

	void startServer();
	void startClientSender();
	void startClientReceiver();

	void addRequest(SoapRequest* request);
};

}	// JktNet

#endif /* SRC_RESEAU_WEB_SOAPSERVER_H_ */
