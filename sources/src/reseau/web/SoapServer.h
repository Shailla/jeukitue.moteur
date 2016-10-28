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
#include "SDL_net.h"

namespace jkt
{

class SoapRequest {
public:
	long timeSyncRequestSent;
	long timeSyncRequestResponse;

	long timeCallbackResponse;

	SoapRequest(long timeRequestSent);
	void serialize(ofstream& fichier);
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

	static const char* SIGNATURE_HEADER;
	static const char* SIGNATURE_RESPONSE;

	static const char* CALLBACK_HEADER;
	static const char* CALLBACK_RESPONSE;

	std::map<long, SoapRequest*> _requests;

	enum HTTP_EXCEPTION {
		RESOURCE_NOT_FOUND_EXCEPTION = 1
	};

	int _period;
	Uint16 _portClient;
	Uint16 _portServer;

	void* buildResponse();

	TCPsocket _socketClient;
	TCPsocket _socketServer;
	SDL_mutex* _mutex;

	bool _activated;

	long extractRequestId(string& response);
	void start();

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

	void open(int period, int portClient, int portServer);
	void stop();

	static int runServer(void* arg);
	static int runClientSender(void* arg);
	static int runClientReceiver(void* arg);

	void startServer();
	void startClientSender();
	void startClientReceiver();

	void addRequest(SoapRequest* request);
	void traceSynchronousResponse(long requestId);
	void traceCallback(long requestId);
};

}	// JktNet

#endif /* SRC_RESEAU_WEB_SOAPSERVER_H_ */
