/*
 * SoapServer.cpp
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#include <sstream>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "util/StringUtils.h"
#include "util/Trace.h"
#include "util/FindFolder.h"
#include "reseau/web/service/WebService.h"
#include "reseau/web/service/PlayersWebService.h"
#include "reseau/web/service/MapWebService.h"
#include "reseau/web/service/MapGrapheWebService.h"

#include "reseau/web/SoapServer.h"

namespace jkt
{

const char* SoapServer::WEB_CSS_DIR =		"./web/css";
const char* SoapServer::WEB_HTML_DIR =		"./web/html";
const char* SoapServer::WEB_IMAGE_DIR =		"./web/image";
const char* SoapServer::WEB_JS_DIR =		"./web/js";
const char* SoapServer::WEB_JSON_DIR =		"./web/json";

const char* SoapServer::HTTP_RETURN =				"\r\n";
const char* SoapServer::HTTP_HEAD = 				"HTTP/1.1";
const char* SoapServer::HTTP_RESPONSE_200 = 		"200 OK";
const char* SoapServer::HTTP_RESPONSE_404 = 		"404 NOT FOUND";
const char* SoapServer::HTTP_RESPONSE_500 = 		"500 INTERNAL ERROR";
const char* SoapServer::HTTP_CONTENT_TYPE_HTML = 	"Content-type: text/html; charset=utf-8";
const char* SoapServer::HTTP_CONTENT_TYPE_CSS = 	"Content-type: text/css; charset=utf-8";
const char* SoapServer::HTTP_CONTENT_TYPE_IMAGE = 	"Content-type: image";
const char* SoapServer::HTTP_CONTENT_TYPE_JS = 		"Content-type: application/javascript;";
const char* SoapServer::HTTP_CONTENT_TYPE_JSON = 	"Content-type: application/json; charset=utf-8";
const char* SoapServer::HTTP_CONTENT_LENGTH = 		"Content-Length: ";

SoapServer::SoapServer() {
	_activated = false;
	_mutex = SDL_CreateMutex();
	_portClient = 0;
	_portServer = 0;
	_socketClient = 0;
	_socketServer = 0;
}

SoapServer::~SoapServer() {
}

void SoapServer::open(int portClient, int portServer) {
	_portClient = portClient;
	_portServer = portServer;

	// Open client socket
	IPaddress adresseClient;

	if(SDLNet_ResolveHost(&adresseClient,"localhost", _portClient)==-1) {
		LOGERROR(("Resolve address error : %s", SDLNet_GetError()));
	}

	_socketClient = SDLNet_TCP_Open(&adresseClient);

	if(!_socketClient) {
		LOGERROR(("Open socket error : %s", SDLNet_GetError()));
	}


	// Open server socket
	IPaddress adresseServer;

	if(SDLNet_ResolveHost(&adresseServer, NULL, _portServer) == -1) {
		LOGERROR(("Resolve address error : %s", SDLNet_GetError()));
	}

	_socketServer = SDLNet_TCP_Open(&adresseServer);

	if(!_socketServer) {
		LOGERROR(("Open TCP server failed : %s", SDLNet_GetError()));
	}
}

void SoapServer::start() {
	stop();

	SDL_Delay(200);

	SDL_LockMutex(_mutex);
	_activated = false;
	SDL_UnlockMutex(_mutex);

	SDL_CreateThread(SoapServer::runServer, this);
	SDL_CreateThread(SoapServer::runClientReceiver, this);

	SDL_Delay(500);

	SDL_CreateThread(SoapServer::runClientSender, this);
}

void SoapServer::stop() {
	SDL_LockMutex(_mutex);
	_activated = false;
	SDL_UnlockMutex(_mutex);
}

void SoapServer::startClientSender() {
	long time;
	int result;
	long msisdn = 0;

	while(_activated) {
		if(msisdn > 90000) {
			msisdn = 0;
		}

		time = SDL_GetTicks();
		stringstream demande;
		demande << DEMANDE_SIGNATURE_1 << "yy" << time << "zz" << DEMANDE_SIGNATURE_2 << "+2126000" << msisdn++ << DEMANDE_SIGNATURE_3;

		addRequest(new SoapRequest(time));

		result = SDLNet_TCP_Send(_socketClient, demande.str().c_str(), demande.str().length());
		SDL_Delay(1000);
	}
}

void SoapServer::addRequest(SoapRequest* request) {
	SDL_LockMutex(_mutex);
	_requests[request->timeRequestSent] = request;
	SDL_UnlockMutex(_mutex);
}

void SoapServer::startClientReceiver() {
	char buffer[1024];
	int requestSize;
	string response;
	TCPsocket socket;

	while(_activated) {
		// Attente connexion client
		socket = 0;

		while(socket == 0 && _activated)
			socket = SDLNet_TCP_Accept(_socketClient);

		requestSize = SDLNet_TCP_Recv(socket, buffer, 1024); 	// Reception parametres connection du client
		buffer[requestSize] = 0;

		response = buffer;
		LOGINFO(("SOAP réponse synchrone reçue : '%s'", response.c_str()));

		// Extract requestID
		long requestId = extractRequestId(response);
	}
}

long SoapServer::extractRequestId(string& response) {
	string var;

	var = response.replace(response.find("yy"), response.find("yy")+2, "");
	var = response.replace(response.find("zz")+1, string::npos, "");

	long requestId = std::stol(var.c_str());

	return requestId;
}

int SoapServer::runClientSender(void* thiz) {
	((SoapServer*)thiz)->startClientSender();
	return 0;
}

int SoapServer::runClientReceiver(void* thiz) {
	((SoapServer*)thiz)->startClientReceiver();
	return 0;
}

int SoapServer::runServer(void* thiz) {
	((SoapServer*)thiz)->startServer();
	return 0;
}

void SoapServer::startServer() {
	TCPsocket clientSocket;
	string status;
	char* response;
	int responseSize;
	char buffer[1024];

	while(_activated) {
		// Attente connexion client
		clientSocket = 0;

		while(clientSocket == 0 && _activated)
			clientSocket = SDLNet_TCP_Accept(_socketServer);

		int requestSize = SDLNet_TCP_Recv(clientSocket, buffer, 1024); 	// Reception parametres connection du client
		buffer[requestSize] = 0;

		string request = buffer;
		LOGINFO(("SOAP requête callback reçue : '%s'", request.c_str()));


		// Recherche du contenu
		try {
			status = status = HTTP_RESPONSE_200;
		}
		catch(...) {
			LOGERROR(("Erreur interne inconnue"));
		}

		// Envoi réponse
		SDLNet_TCP_Send(clientSocket, response, responseSize); 		// Envoi du contenu de la page au client
		SDLNet_TCP_Close(clientSocket);

		free(response);
	}
}

const char* SoapServer::DEMANDE_SIGNATURE_1 = "<S:Envelope xmlns:S=\"http://www.w3.org/2003/05/soap-envelope\">"
   "<S:Body>"
   "   <ns7:MSS_Signature xmlns:ns4=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:ns5=\"http://uri.etsi.org/TS102204/v1.1.2#\" xmlns:ns6=\"http://www.w3.org/2001/04/xmlenc#\" xmlns:ns7=\"http://www.orange.com/mobileconnect/mssp/wsdl\">"
   "      <MSS_SignatureReq MessagingMode=\"asynchServerServer\" MajorVersion=\"1\" MinorVersion=\"1\">"
   "         <ns5:AP_Info AP_ID=\"SP_DEFAULT\" AP_TransID=\"xxIDyy\" AP_PWD=\"Administrateur!\" AP_URL=\"http://localhost:8089/notify\" Instant=\"2015-04-13T11:53:15.384+02:00\"/>"
   "         <ns5:MSSP_Info>"
   "             <ns5:MSSP_ID>"
   "                 <ns5:URI>test</ns5:URI>"
   "             </ns5:MSSP_ID>"
   "         </ns5:MSSP_Info>"
   "         <ns5:MobileUser>"
   "            <ns5:MSISDN>xxMSISDNyy</ns5:MSISDN>"
   "         </ns5:MobileUser>"
   "         <ns5:DataToBeSigned>test: Saisir votre code mobile connect a 4 chiffres</ns5:DataToBeSigned>"
   "         <ns5:SignatureProfile>"
   "            <ns5:mssURI>http://uri.gsma.com/mobileconnect/LoA2</ns5:mssURI>"
   "         </ns5:SignatureProfile>"
   "         <ns5:AdditionalServices>"
   "            <ns5:Service>"
   "               <ns5:Description>"
   "                  <ns5:mssURI>http://mobileConnect/TS102204/v1.0.0#antiSpamCode</ns5:mssURI>"
    "              </ns5:Description>"
    "              <xs:Code>aBcde</xs:Code>"
    "           </ns5:Service>"
    "        </ns5:AdditionalServices>"
    "     </MSS_SignatureReq>"
    "  </ns7:MSS_Signature>"
  " </S:Body>"
"</S:Envelope>";

}	// JktNet
