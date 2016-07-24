
#ifndef __JKT__SPA_H
#define __JKT__SPA_H

#include <iostream>
#include <string>
using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/Packet.h"

class CPlayer;

namespace jkt
{

class CSPA
{
private: 
	static int m_BytesRec;			// Nombre d'octets pour le comptage du débit en réception
	static Uint32 m_TimeBytesRec;
	static int m_NombreRec;

	static int m_BytesEm;			// Nombre d'octets pour le comptage du débit en émmission
	static Uint32 m_TimeBytesEm;
	static int m_NombreEm;

	Packet _packetIn;				// Paquet recevant les données
	Packet _packetOut;				// Paquet pour l'envoie des données
	IPaddress m_Ip;					// Adresse du distant
	Uint16 _localPort;				// Port local d'écoute
	UDPsocket m_Socket;				// Ben ça c'est la socket :)

public:
	CSPA();
	~CSPA();

	bool openInClientMode(const IPaddress &address);				// Ouverture en mode client
	bool openInClientMode(const string &remIp, Uint16 remPort);	// Ouverture en mode client
	bool openInServerMode(Uint16 localPort);						// Ouverture en mode serveur
	void close();

	void init();
	UDPpacket *getPacketIn();
	UDPpacket *getPacketOut();
	UDPsocket getSocket() const;

	unsigned int getPosition() const;		// Retourne la position dans le paquet
	
	void addCode( Uint16 code1, Uint16 code2 );		// Ajoute un doublet de codes à la fin du paquet
	void addBool( bool var );						// Ajoute un booléen à la fin du paquet
	void add16( Uint16 var );						// Ajoute un mot de 16 bits à la fin du paquet
	void add32( Uint32 var );						// Ajoute un mot de 32 bits à la fin du paquet
	void add( float var);							// Ajoute un tableau de 3 flottants
	void add3fv(const float *var);						// Ajoute un tableau de 3 flottants
	void addChar( const char *txt );				// Ajoute du texte à la fin du paquet
	void add( const string &txt );					// Ajoute du texte à la fin du paquet
	void addRecapFromServer( const CPlayer &player );

	void readCode(Uint16& code1,Uint16& code2);		// Lit le doublet de code pointé
	bool readBool();								// Lit le booléen pointé
	Uint16 read16();								// Lit le mot de 16 bits pointé
	Uint32 read32();								// Lit le mot de 32 bits pointé
	float readf();									// Lit un tableau de 3 flottants
	void read3fv( float *var);						// Lit un tableau de 3 flottants
	void readChar( char *txt );						// Lit le texte pointé
	void readString( string &mot );					// Lit le texte pointé vers un string
	void readRecapFromServer( const CPlayer &player );

	string iPInToString();					// Renvoie l'IP in en string
	string iPOutToString();					// Renvoie l'IP out en string
	string debugToString();							// Renvoie un résumé du paquet sous forme de string
	int send();										// Envoie le CSPA vers l'adresse ouverte
	int send(const IPaddress& destination);				// Envoie le CSPA vers une adresse spécifique
	int recoit();									// Recoit les paquets du CSPA->packetIn
	static void computeDebits(Uint32 currentTime);	// Calcule les débit en émission et en réception
};

}	// JktNet

#endif

