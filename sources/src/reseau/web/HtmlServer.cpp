/*
 * HtmlServer.cpp
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/web/HtmlServer.h"

namespace JktNet
{

HtmlServer::HtmlServer() {
}

HtmlServer::~HtmlServer() {
}

void HtmlServer::open() {
	const char* entete = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
	TCPsocket serveur,socket;
	char buffer[1024], page[4096];

	for(int i=0 ; i<1024 ; i++)
		buffer[i]=0;

	// Creation de la socket serveur
	IPaddress adresse;
	SDLNet_ResolveHost(&adresse, NULL, 10000);
	serveur = SDLNet_TCP_Open(&adresse);


	// Lecture fichier contenant la page à afficher au client
	FILE* fpage=fopen("page.html", "r");
	int i = 0;

	while(!feof(fpage)&& i<4096) {
		fscanf(fpage, "%c", &page[i++]);
	}

	fclose(fpage);

	page[i] = 0;

	// Traitement d'une connection
	while(1) {
		socket = 0;
		while(socket == 0)
			socket = SDLNet_TCP_Accept(serveur); 			// Attente client

		SDLNet_TCP_Recv(socket, buffer, 1024); 				// Reception parametres connection du client
		SDLNet_TCP_Send(socket, entete, strlen(entete)); 	// Envoi de l'entete page au client
		SDLNet_TCP_Send(socket, page, strlen(page)+1); 		// Envoi du contenu de la page au client
		SDLNet_TCP_Close(socket);
	}
}

}	// JktNet
