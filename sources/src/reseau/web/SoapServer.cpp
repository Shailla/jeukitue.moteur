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
#include "util/IpUtils.h"
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

SoapRequest::SoapRequest(long timeRequestSent) {
	this->timeSyncRequestSent = timeRequestSent;

	timeSyncRequestResponse = 0;
	timeCallbackResponse = 0;
}

void SoapRequest::serialize(ofstream& fichier) {
	stringstream ligne;
	ligne << timeSyncRequestSent << "\t" << timeSyncRequestResponse << "\t" << timeCallbackResponse;

	fichier << ligne.str() << "\n";
}


SoapServer::SoapServer() {
	_activated = false;
	_mutex = SDL_CreateMutex();
	_period = 0;
	_portClient = 0;
	_portServer = 0;
	_socketClient = 0;
	_socketServer = 0;
}

SoapServer::~SoapServer() {
}

void SoapServer::open(int period, int portClient, int portServer) {
	_period = period;
	_portClient = portClient;
	_portServer = portServer;

	// Open client socket
	IPaddress adresseClient;

	if(SDLNet_ResolveHost(&adresseClient,"mcoamea-ae-prp.dmz.cloud.mbs", _portClient)==-1) {
		LOGERROR(("Resolve address error : %s", SDLNet_GetError()));
	}

	LOGINFO(("RESOLVE CLIENT '%s'", IpUtils::translateAddress(adresseClient).c_str()));

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

	_requests.clear();

	start();
}

void SoapServer::start() {
	stop();

	SDL_LockMutex(_mutex);
	_activated = true;
	SDL_UnlockMutex(_mutex);

	SDL_CreateThread(SoapServer::runServer, this);
	SDL_CreateThread(SoapServer::runClientReceiver, this);

	SDL_Delay(500);

	SDL_CreateThread(SoapServer::runClientSender, this);
}

void SoapServer::stop() {
	// Termine les threads
	SDL_LockMutex(_mutex);
	_activated = false;
	SDL_UnlockMutex(_mutex);

	// Laisse finir les trucs en cours
	SDL_Delay(200);

	ofstream fichier("soap.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if(!fichier)
		LOGERROR(("Erreur à l'ouverture en écriture du fichier des résultats SOAP!"));


	for(pair<long, SoapRequest*> var : _requests) {
		var.second->serialize(fichier);
	}

	fichier.flush();
	fichier.close();
}

void SoapServer::startClientSender() {
	long time;
	int result;
	long msisdn = 10000;

	while(_activated) {
		if(msisdn > 99999) {
			msisdn = 10000;
		}

		// RequestID
		time = SDL_GetTicks();
		stringstream requestId;
		requestId << "yy" << time << "zz";

		string demande = SIGNATURE_RESPONSE;
		demande = demande.replace(demande.find("${ID}"), 5, requestId.str());

		// MSISDN
		stringstream numero;
		numero << "+2126000" << msisdn++;
		demande = demande.replace(demande.find("${MSISDN}"), 9, numero.str());


		string header = SIGNATURE_HEADER;
		stringstream size;
		size << demande.length();
		header = header.replace(header.find("${SIZE}"), 7, size.str());

		demande = header + demande;

		addRequest(new SoapRequest(time));

		LOGERROR(("Demande signature à envoyer '%s'", demande.c_str()));

		result = SDLNet_TCP_Send(_socketClient, demande.c_str(), demande.length());

		LOGERROR(("Demande signature envoyée"));

		SDL_Delay(_period);
	}
}

void SoapServer::addRequest(SoapRequest* request) {
	SDL_LockMutex(_mutex);
	_requests[request->timeSyncRequestSent] = request;
	SDL_UnlockMutex(_mutex);
}

void SoapServer::startClientReceiver() {
	char buffer[1024];
	int requestSize;
	string response;

	while(_activated) {
		// Attente connexion client
		requestSize = 0;

		while(requestSize == 0 && _activated)
			requestSize = SDLNet_TCP_Recv(_socketClient, buffer, 1024); 	// Reception parametres connection du client

		buffer[requestSize] = 0;

		response = buffer;
		LOGINFO(("SOAP réponse synchrone reçue : '%s'", response.c_str()));

		// Extract requestID
		long requestId = extractRequestId(response);
		traceSynchronousResponse(requestId);
	}
}

void SoapServer::traceSynchronousResponse(long requestId) {
	SoapRequest* request = _requests[requestId];
	request->timeSyncRequestResponse = SDL_GetTicks();
}

void SoapServer::traceCallback(long requestId) {
	SoapRequest* request = _requests[requestId];
	request->timeCallbackResponse = SDL_GetTicks();
}

long SoapServer::extractRequestId(string& response) {
	string var;
	long requestId;

	if(response.find("yy") != string::npos && response.find("zz") != string::npos) {
		var = response.replace(response.find("yy"), 2, "");
		var = response.replace(response.find("zz")+1, string::npos, "");
		requestId = std::stol(var.c_str());
	}
	else {
		requestId = 0;
	}

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
	string response;
	int responseSize;
	char buffer[1024];

	while(_activated) {
		// Attente connexion client
		clientSocket = 0;

		while(clientSocket == 0 && _activated)
			clientSocket = SDLNet_TCP_Accept(_socketServer);

		int requestSize = SDLNet_TCP_Recv(clientSocket, buffer, 1024); 	// Reception parametres connection du client
		buffer[requestSize] = 0;

		string callback = buffer;
		LOGINFO(("SOAP requête callback reçue : '%s'", callback.c_str()));

		// Extract requestID
		long requestId = extractRequestId(callback);


		if(requestId) {
			traceCallback(requestId);

			response = CALLBACK_RESPONSE;
			stringstream requestIdStr;
			requestIdStr << requestId;
			response = response.replace(response.find("${ID}"), 5, requestIdStr.str());

			stringstream size;
			size << response.length();
			string header = CALLBACK_HEADER;
			header = header.replace(header.find("${SIZE}"), 7, size.str());

			response = header + response;

			// Envoi réponse
			SDLNet_TCP_Send(clientSocket, response.c_str(), response.length()); 		// Envoi du contenu de la page au client
			SDLNet_TCP_Close(clientSocket);
		}
		else {
			LOGERROR(("Pas de requestId"));
		}
	}
}

const char* SoapServer::SIGNATURE_HEADER =
		"POST /services/MSSSignatureService HTTP/1.1\r\n"
		"Content-Type: application/soap+xml;charset=UTF-8;action=\"tns:MSS_Signature\"\r\n"
		"Content-Length: ${SIZE}\r\n"
		"Host: mcoamea-ae-prp.dmz.cloud.mbs\r\n"
		"Connection: Keep-Alive\r\n"
		"User-Agent: Apache-HttpClient/4.1.1 (java 1.5)\r\n"
		"\r\n";

const char* SoapServer::SIGNATURE_RESPONSE =
		"<S:Envelope xmlns:S=\"http://www.w3.org/2003/05/soap-envelope\">\r\n"
		"<S:Body>\r\n"
		"   <ns7:MSS_Signature xmlns:ns4=\"http://www.w3.org/2000/09/xmldsig#\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:ns5=\"http://uri.etsi.org/TS102204/v1.1.2#\" xmlns:ns6=\"http://www.w3.org/2001/04/xmlenc#\" xmlns:ns7=\"http://www.orange.com/mobileconnect/mssp/wsdl\">\r\n"
		"      <MSS_SignatureReq MessagingMode=\"asynchServerServer\" MajorVersion=\"1\" MinorVersion=\"1\">\r\n"
		"         <ns5:AP_Info AP_ID=\"SP_DEFAULT\" AP_TransID=\"${ID}\" AP_PWD=\"Administrateur!\" AP_URL=\"http://localhost:8089/notify\" Instant=\"2015-04-13T11:53:15.384+02:00\"/>\r\n"
		"         <ns5:MSSP_Info>\r\n"
		"             <ns5:MSSP_ID>\r\n"
		"                 <ns5:URI>test</ns5:URI>\r\n"
		"             </ns5:MSSP_ID>\r\n"
		"         </ns5:MSSP_Info>\r\n"
		"         <ns5:MobileUser>\r\n"
		"            <ns5:MSISDN>${MSISDN}</ns5:MSISDN>\r\n"
		"         </ns5:MobileUser>\r\n"
		"         <ns5:DataToBeSigned>test: Saisir votre code mobile connect a 4 chiffres</ns5:DataToBeSigned>\r\n"
		"         <ns5:SignatureProfile>\r\n"
		"            <ns5:mssURI>http://uri.gsma.com/mobileconnect/LoA2</ns5:mssURI>\r\n"
		"         </ns5:SignatureProfile>\r\n"
		"         <ns5:AdditionalServices>\r\n"
		"            <ns5:Service>\r\n"
		"               <ns5:Description>\r\n"
		"                  <ns5:mssURI>http://mobileConnect/TS102204/v1.0.0#antiSpamCode</ns5:mssURI>\r\n"
		"              </ns5:Description>\r\n"
		"              <xs:Code>aBcde</xs:Code>\r\n"
		"           </ns5:Service>\r\n"
		"        </ns5:AdditionalServices>\r\n"
		"     </MSS_SignatureReq>\r\n"
		"  </ns7:MSS_Signature>\r\n"
		" </S:Body>\r\n"
		"</S:Envelope>\r\n";

const char* SoapServer::CALLBACK_HEADER =
		"HTTP/1.1 200 OK\r\n"
		"Date: Sun, 23 Oct 2016 16:22:16 GMT\r\n"
		"Server: Apache\r\n"
		"Content-Length: ${SIZE}\r\n"
		"Content-Type: application/soap+xml;charset=UTF-8\r\n"
		"Set-Cookie: MCOAMEA_AE=wasae01; path=/\r\n"
		"\r\n";

const char* SoapServer::CALLBACK_RESPONSE =
		"<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsdl=\"http://www.orange.com/mobileconnect/mssp/wsdl\" xmlns:v1=\"http://uri.etsi.org/TS102204/v1.1.2#\" xmlns:xd=\"http://www.w3.org/2000/09/xmldsig#\">\r\n"
		"<soap:Header/>\r\n"
		"<soap:Body>\r\n"
		"   <wsdl:MSS_NotificationResponse>\r\n"
		"      <MSS_ReceiptReq MajorVersion=\"1\" MinorVersion=\"1\" MSSP_TransID=\"${ID}\">\r\n"
		"         <v1:AP_Info AP_ID=\"SP_DEFAULT\" AP_TransID=\"12345\" AP_PWD=\"spPassword\" AP_URL=\"http://ITL-5MQ1ZX1:8090/notify\"/>\r\n"
		"         <v1:MSSP_Info Instant=\"1234567\">\r\n"
		"            <v1:MSSP_ID>\r\n"
		"            </v1:MSSP_ID>\r\n"
		"         </v1:MSSP_Info>\r\n"
		"         <v1:MobileUser>\r\n"
		"         </v1:MobileUser>\r\n"
		"      </MSS_ReceiptReq>\r\n"
		"   </wsdl:MSS_NotificationResponse>\r\n"
		"</soap:Body>\r\n"
		"</soap:Envelope>\r\n";

}	// JktNet
