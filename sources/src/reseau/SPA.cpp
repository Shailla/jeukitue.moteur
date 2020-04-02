
#include <iostream>
#include <sstream>

#include <SDL.h>
#include <SDL_net.h>

#include "util/Trace.h"
#include "util/Erreur.h"

#include "reseau/SPA.h"
#include "reseau/Packet.h"
#include "main/Player.h"
#include "main/Statistics.h"

using namespace std;

namespace jkt
{

int CSPA::m_BytesRec = 0;	// Nombre d'octets pour le comptage du d�bit en r�ception
Uint32 CSPA::m_TimeBytesRec = 0;
int CSPA::m_NombreRec = 0;

int CSPA::m_BytesEm = 0;	// Nombre d'octets pour le comptage du d�bit en �mmission
Uint32 CSPA::m_TimeBytesEm = 0;
int CSPA::m_NombreEm = 0;

CSPA::CSPA() {
	if( m_TimeBytesRec == 0 ) {	// Si l'initialisation de classe n'a pas encore eu lieu
		m_TimeBytesRec = SDL_GetTicks();
		m_TimeBytesEm = m_TimeBytesRec;
	}

	m_Socket = 0;
	m_Ip.host = 0;
	m_Ip.port = 0;
	_localPort = 0;
}

bool CSPA::openInClientMode(const string& remIp, Uint16 remPort) {
	bool result = true;

	if( SDLNet_ResolveHost( &m_Ip, remIp.c_str(), remPort ) ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << SDLNet_GetError();
		result = false;
	}

	if( result ) {
		result = openInClientMode( m_Ip );
	}

	return result;
}

bool CSPA::openInClientMode(const IPaddress &address) {
	bool result = true;

	m_Ip = address;

	if(result) {
		result = openInServerMode(0);
	}

	// Attachement du socket � l'IP de l'autre
	int channel = 0;

	if(result) {
		channel = SDLNet_UDP_Bind(m_Socket, -1, &m_Ip);

		if(channel == -1) {
			cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_UDP_Bind : " << SDLNet_GetError();
			close();
			result = false;
		}
	}

	if(result) {
		_packetOut.setChannel(channel);
		_packetIn.setChannel(channel);
	}

	return result;
}

bool CSPA::openInServerMode(Uint16 localPort) {
	bool result = true;
	_localPort = localPort;

	close();

	m_Socket = SDLNet_UDP_Open( _localPort );		// Ouverture d'un socket UDP serveur

	if(!m_Socket) {		// Socket principale non-ouverte
		cerr << endl << __FILE__ << ":" << __LINE__ << " SDLnet_UDP_Open(" << _localPort << ") : " << SDLNet_GetError() << endl;
		result = false;	// Echec de l'ouverture du serveur
	}

	return result;
}

UDPsocket CSPA::getSocket() const {
	return m_Socket;
}

void CSPA::close() {
	if(m_Socket) {
		SDLNet_UDP_Unbind(m_Socket, _packetOut.getChannel());	// Lib�ration du canal
		SDLNet_UDP_Close(m_Socket);								// Lib�ration du socket principal
		m_Socket = 0;
	}
}

void CSPA::init() {	// Initialise la position dans le paquet
	_packetIn.init();
	_packetOut.init();
}



int CSPA::send() {
	m_BytesEm += _packetOut.getPositionWrite();
	_packetOut.prepareToSend();
	m_NombreEm++;

	UDPpacket* packet = _packetOut.getPacket();

	// Le channel est sp�cifi� donc l'adresse "m_PacketOut->address" est ignor�e pour le choix de la desitination
	return SDLNet_UDP_Send(m_Socket, packet->channel, packet);
}

int CSPA::send(const IPaddress &destination) {
	m_BytesEm += _packetOut.getPositionWrite();
	_packetOut.prepareToSend();
	m_NombreEm++;

	UDPpacket* packet = _packetOut.getPacket();
	packet->address = destination;

	// Le channel n'est pas sp�cifi� (-1) donc l'adresse "m_PacketOut->address" est utilis�e comme desitination
	return SDLNet_UDP_Send(m_Socket, -1, packet);
}

int CSPA::recoit() {
	UDPpacket* packet = _packetIn.getPacket();

	int result = SDLNet_UDP_Recv( m_Socket, packet );
	m_BytesRec += packet->len;
	m_NombreRec++;

	return result;
}

string CSPA::iPInToString() {
	UDPpacket* packet = _packetIn.getPacket();

	stringstream str;
	Uint32 ip =  SDL_SwapBE32( packet->address.host );
	Uint16 port = SDL_SwapBE16(  packet->address.port );
	str << (ip>>24) << "." << ((ip>>16)&0xff) << "." << ((ip>>8)&0xff) << "." << ((ip)&0xff) << ":" << port;

	return str.str();
}

string CSPA::iPOutToString() {
	UDPpacket* packet = _packetIn.getPacket();

	stringstream str;
	Uint32 ip =  SDL_SwapBE32( packet->address.host );
	Uint16 port = SDL_SwapBE16(  packet->address.port );
	str << (ip>>24) << "." << ((ip>>16)&0xff) << "." << ((ip>>8)&0xff) << "." << ((ip)&0xff) << ":" << port;

	return str.str();
}

CSPA::~CSPA() {
	if(m_Socket) {
		SDLNet_UDP_Close( m_Socket );		// Lib�ration du socket
		m_Socket = 0;
	}
}

UDPpacket* CSPA::getPacketIn() {
	return _packetIn.getPacket();
}

UDPpacket* CSPA::getPacketOut() {
	return _packetOut.getPacket();
}

string CSPA::debugToString() {
	UDPpacket* packet = _packetIn.getPacket();

	stringstream str;
	str << "\nPaquet recu :\n";
	str << "\tIP packet  :\t" << SDLNet_ResolveIP( &packet->address ) << endl;
	str << "\tIP CSPA :\t" << SDLNet_ResolveIP( &packet->address ) << endl;
	str << "\tLength :\t" << packet->len << endl;
	str << "\tPosition :\t" << _packetIn.getPositionRead() << endl;
	str << "\tMaxLength :\t" << packet->maxlen << endl;
	str << "\tChannel :\t" << packet->channel << endl;
	str << "\tStatus :\t" << packet->status << "\n\t";

	for( int i=0 ; i<packet->len ; i=i+2 ) {
		if( (i%10) == 0 )
			str << "\n\t\t";
		else if( (i%5) == 0 )
			str << "\t";
		Uint16 nbr = SDLNet_Read16( packet->data + i );
		str << nbr << ' ';
	}

	return str.str();
}

void CSPA::addCode(Uint16 code1, Uint16 code2) {
	_packetOut.addCode(code1, code2);
}

void CSPA::readCode(Uint16& code1, Uint16& code2) {
	_packetIn.readCode(code1, code2);
}

void CSPA::add16(Uint16 var) {
	_packetOut.add16(var);
}

Uint16 CSPA::read16() {
	return _packetIn.read16();
}

void CSPA::add32(Uint32 var) {
	_packetOut.add32(var);
}

Uint32 CSPA::read32() {
	return _packetIn.read32();
}

void CSPA::add( float var )	// Ajoute un flottant (c'est � dire 4 octets)
{
	_packetOut.add(var);
}

float CSPA::readf()	// Lit un flottant (c'est � dire 4 octets)
{
	return _packetIn.readf();
}

void CSPA::add3fv(const float *var)	// Ajoute un tableau de 3 flottants
{
	_packetOut.add3fv(var);
}

void CSPA::read3fv(float *var) {	// Lit un tableau de 3 flottants
	_packetIn.read3fv(var);
}

void CSPA::addChar(const char *txt) {
	_packetOut.addChar(txt);
}

void CSPA::add( const string& txt ) {
	_packetOut.add(txt);
}

void CSPA::readChar(char* txt) {
	_packetIn.readChar(txt);
}

void CSPA::readString( string &mot ) {
	_packetIn.readString(mot);
}

void CSPA::readRecapFromServer( const CPlayer &player ) {
	float vect[3];

	player.getPosition( vect );				// Position du joueur
	_packetIn.read3fv( vect );

	player.getVitesse( vect );				// Vitesse du joueur
	_packetIn.read3fv( vect );
}

void CSPA::addRecapFromServer( const CPlayer &player ) {
	float vect[3];

	player.getPosition( vect );		// Sa position
	_packetOut.add3fv( vect );

	player.getVitesse( vect );		// Sa vitesse
	_packetOut.add3fv( vect );
}

void CSPA::computeDebits(Uint32 currentTime) {
	float fDebitRec = 0, fTailleRec = 0, fDebitEm = 0, fTailleEm = 0;

	// Calcule du d�bit en r�ception
	if( currentTime - m_TimeBytesRec > 1000 ) {
		fDebitRec = (float)m_BytesRec / ( currentTime - m_TimeBytesRec );

		if(m_NombreRec)
			fTailleRec = (float)m_BytesRec / m_NombreRec;
		else
			fTailleRec = 0;

		m_TimeBytesRec = currentTime;
		m_BytesRec = 0;
		m_NombreRec = 0;
	}

	// Calcule du d�bit en �mission
	if(currentTime - m_TimeBytesEm > 1000) {
		fDebitEm = (float)m_BytesEm / ( currentTime - m_TimeBytesEm );

		if(m_NombreEm)
			fTailleEm = (float)m_BytesEm / m_NombreEm;
		else
			fTailleEm = 0;

		m_TimeBytesEm = currentTime;
		m_BytesEm = 0;
		m_NombreEm = 0;
	}

	Statistics::getInstance()->lock();
	Statistics::getInstance()->_fDebitRec = fDebitRec;
	Statistics::getInstance()->_fTailleRec = fTailleRec;
	Statistics::getInstance()->_fDebitEm = fDebitEm;
	Statistics::getInstance()->_fTailleEm = fTailleEm;
	Statistics::getInstance()->unlock();
}

}	// JktNet
