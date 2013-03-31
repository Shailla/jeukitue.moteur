
#include <iostream>
#include <sstream>
using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "util/Trace.h"
#include "util/Erreur.h"

#include "reseau/SPA.h"
#include "main/Player.h"
#include "main/Statistics.h"

namespace JktNet
{

int CSPA::m_BytesRec = 0;	// Nombre d'octets pour le comptage du débit en réception
Uint32 CSPA::m_TimeBytesRec = 0;
int CSPA::m_NombreRec = 0;

int CSPA::m_BytesEm = 0;	// Nombre d'octets pour le comptage du débit en émmission
Uint32 CSPA::m_TimeBytesEm = 0;
int CSPA::m_NombreEm = 0;

CSPA::CSPA() {
	if( m_TimeBytesRec == 0 ) {	// Si l'initialisation de classe n'a pas encore eu lieu
		m_TimeBytesRec = SDL_GetTicks();
		m_TimeBytesEm = m_TimeBytesRec;
	}

	m_Socket = 0;
	m_PacketIn = SDLNet_AllocPacket( 65535 );
	m_PacketOut = SDLNet_AllocPacket( 65535 );
	m_Ip.host = 0;
	m_Ip.port = 0;
	m_uPosition = 0;
}

bool CSPA::open( const string &remIp, Uint16 remPort ) {
TRACE().p( TRACE_RESEAU, "CSPA::open(lremIp=%s,remPort=%d) begin%T", remIp.c_str(), remPort, this );
	bool result = true;

	if( SDLNet_ResolveHost( &m_Ip, remIp.c_str(), remPort ) ) {
TRACE().p( TRACE_ERROR, "CSPA::open() SDLNet_ResolveHost : %s%T", SDLNet_GetError(), this );
		result = false;
	}

	if( result ) {
		result = open( m_Ip );
		if( !result )
			TRACE().p( TRACE_ERROR, "CSPA::open() open( m_Ip )%T", this );
	}

TRACE().p( TRACE_RESEAU, "CSPA::open() -> %b end%T", result, this );
	return result;
}

UDPsocket CSPA::getSocket() const
{
	return m_Socket;
}

bool CSPA::open( const IPaddress &address )
{
TRACE().p( TRACE_RESEAU, "CSPA::open(address=[%s,%d]) begin%T", SDLNet_ResolveIP( (IPaddress*)&address ), address.port, this );
	bool result = true;

	m_Ip = address;

	if(result) {
		m_Socket = SDLNet_UDP_Open( 0 );		// Ouverture d'un socket client sur un port quelconque

		if(!m_Socket) {
TRACE().p( TRACE_ERROR, "CSPA::open() : %s%T", SDLNet_GetError(), this );
			cerr << endl << __FILE__ << ":" << __LINE__ << " SDLnet_UDP_Open(0) : " << SDLNet_GetError();

			result = false;
		}
	}

	// Attachement du socket à l'IP de l'autre
	if(result) {
		m_PacketOut->channel = SDLNet_UDP_Bind( m_Socket, -1, &m_Ip );

		if(m_PacketOut->channel == -1) {
TRACE().p( TRACE_ERROR, "CSPA::open() : %s%T", SDLNet_GetError(), this );
			cerr << endl << __FILE__ << ":" << __LINE__ << " SDLNet_UDP_Bind : " << SDLNet_GetError();

			SDLNet_UDP_Close( m_Socket );		// Libération du socket
			m_Socket = 0;

			result = false;
		}

		m_PacketIn->channel = m_PacketOut->channel;
	}

TRACE().p( TRACE_RESEAU, "CSPA::open() -> %b end%T", result, this );
	return result;
}

bool CSPA::open(Uint16 locPort) {
TRACE().p( TRACE_RESEAU, "CSPA::open(locPort=%d) begin%T", locPort, this );
	bool result = true;

	SDLNet_ResolveHost( &m_Ip, 0, locPort );

	m_Socket = SDLNet_UDP_Open( locPort );		// Ouverture d'un socket UDP serveur
	if(!m_Socket) {		// Socket principale non-ouverte
TRACE().p( TRACE_ERROR, "CSPA::open() : %s%T", SDLNet_GetError(), this );
		cerr << endl << __FILE__ << ":" << __LINE__ << " SDLnet_UDP_Open(" << locPort << ") : " << SDLNet_GetError() << endl;
		result = false;	// Echec de l'ouverture du serveur
	}

TRACE().p( TRACE_RESEAU, "CSPA::open() -> %b end%T", result, this );
	return result;
}

void CSPA::close() {
TRACE().p( TRACE_RESEAU, "CSPA::close() begin%T", this );
	SDLNet_UDP_Unbind( m_Socket, m_PacketOut->channel );	// Libération du canal
	SDLNet_UDP_Close( m_Socket );	// Libération du socket principal
	m_Socket = 0;
TRACE().p( TRACE_RESEAU, "CSPA::close()end%T", this );
}

void CSPA::init() {	// Initialise la position dans le paquet
	m_uPosition = 0;
}

unsigned int CSPA::getPosition() const {
	return m_uPosition;
}

void CSPA::addCode(Uint16 code1, Uint16 code2) {
	SDLNet_Write16( code1, m_PacketOut->data+m_uPosition );
	SDLNet_Write16( code2, m_PacketOut->data+2+m_uPosition );
	m_uPosition += 4;
}

void CSPA::readCode(Uint16 &code1, Uint16 &code2) {
	code1 = SDLNet_Read16( m_PacketIn->data+m_uPosition );
	code2 = SDLNet_Read16( m_PacketIn->data+2+m_uPosition );
	m_uPosition += 4;
}

void CSPA::add16(Uint16 var) {
	SDLNet_Write16( var, m_PacketOut->data+m_uPosition );
	m_uPosition += 2;
}

void CSPA::add16InPosition( unsigned int position, Uint16 var )
{	SDLNet_Write16( var, m_PacketOut->data+position );	}

Uint16 CSPA::read16() {
	Uint16 var =  SDLNet_Read16( m_PacketIn->data+m_uPosition );
	m_uPosition += 2;
	return var;
}

void CSPA::add32(Uint32 var) {
	SDLNet_Write32( var, m_PacketOut->data+m_uPosition );
	m_uPosition += 4;
}

Uint32 CSPA::read32() {
	Uint32 var = SDLNet_Read32( m_PacketIn->data+m_uPosition );
	m_uPosition += 4;
	return var;
}

void CSPA::add( float var )	// Ajoute un flottant (c'est à dire 4 octets)
{
	*((char*)m_PacketOut->data+m_uPosition) = ((char*)&var)[0];
	*((char*)m_PacketOut->data+m_uPosition+1) = ((char*)&var)[1];
	*((char*)m_PacketOut->data+m_uPosition+2) = ((char*)&var)[2];
	*((char*)m_PacketOut->data+m_uPosition+3) = ((char*)&var)[3];

	m_uPosition += 4;
}

float CSPA::readf()	// Lit un flottant (c'est à dire 4 octets)
{
	float result;
	((char*)&result)[0] = *((char*)m_PacketIn->data+m_uPosition);
	((char*)&result)[1] = *((char*)m_PacketIn->data+m_uPosition+1);
	((char*)&result)[2] = *((char*)m_PacketIn->data+m_uPosition+2);
	((char*)&result)[3] = *((char*)m_PacketIn->data+m_uPosition+3);

	m_uPosition += 4;
	return result;
}

void CSPA::add3fv( float *var )	// Ajoute un tableau de 3 flottants
{
	*((char*)m_PacketOut->data+m_uPosition) = ((char*)var)[0];
	*((char*)m_PacketOut->data+m_uPosition+1) = ((char*)var)[1];
	*((char*)m_PacketOut->data+m_uPosition+2) = ((char*)var)[2];
	*((char*)m_PacketOut->data+m_uPosition+3) = ((char*)var)[3];

	*((char*)m_PacketOut->data+m_uPosition+4) = ((char*)var)[4];
	*((char*)m_PacketOut->data+m_uPosition+5) = ((char*)var)[5];
	*((char*)m_PacketOut->data+m_uPosition+6) = ((char*)var)[6];
	*((char*)m_PacketOut->data+m_uPosition+7) = ((char*)var)[7];

	*((char*)m_PacketOut->data+m_uPosition+8) = ((char*)var)[8];
	*((char*)m_PacketOut->data+m_uPosition+9) = ((char*)var)[9];
	*((char*)m_PacketOut->data+m_uPosition+10) = ((char*)var)[10];
	*((char*)m_PacketOut->data+m_uPosition+11) = ((char*)var)[11];

	m_uPosition += 12;
}

void CSPA::read3fv( float *var ) {	// Lit un tableau de 3 flottants
	((char*)var)[0] = *((char*)m_PacketIn->data+m_uPosition);
	((char*)var)[1] = *((char*)m_PacketIn->data+m_uPosition+1);
	((char*)var)[2] = *((char*)m_PacketIn->data+m_uPosition+2);
	((char*)var)[3] = *((char*)m_PacketIn->data+m_uPosition+3);

	((char*)var)[4] = *((char*)m_PacketIn->data+m_uPosition+4);
	((char*)var)[5] = *((char*)m_PacketIn->data+m_uPosition+5);
	((char*)var)[6] = *((char*)m_PacketIn->data+m_uPosition+6);
	((char*)var)[7] = *((char*)m_PacketIn->data+m_uPosition+7);

	((char*)var)[8] = *((char*)m_PacketIn->data+m_uPosition+8);
	((char*)var)[9] = *((char*)m_PacketIn->data+m_uPosition+9);
	((char*)var)[10] = *((char*)m_PacketIn->data+m_uPosition+10);
	((char*)var)[11] = *((char*)m_PacketIn->data+m_uPosition+11);

	m_uPosition += 12;
}

void CSPA::addChar( const char *txt ) {
	unsigned int len = (unsigned int)strlen( txt );
	strcpy( (char*)m_PacketOut->data+m_uPosition, txt );		// Copie le texte -> paquet
	m_uPosition += len + 1;								// Taille du txt + 2 octets pour len
}

void CSPA::add( const string &txt ) {
	unsigned int len = (unsigned int)txt.size();
	strcpy( (char*)m_PacketOut->data+m_uPosition, txt.c_str() );		// Copie le texte -> paquet
	m_uPosition += len + 1;								// Taille du txt + 2 octets pour len
}

void CSPA::readChar( char *txt ) {
	strcpy( txt, (char*)m_PacketIn->data+m_uPosition );
	m_uPosition += (unsigned int)strlen( txt ) + 1;
}

void CSPA::readString( string &mot ) {
	mot = (char*)m_PacketIn->data+m_uPosition;
	m_uPosition += (unsigned int)mot.length() + 1;
}

int CSPA::send() {
	m_BytesEm += m_uPosition;
	m_PacketOut->len = m_uPosition;
	m_NombreEm++;
	return SDLNet_UDP_Send( m_Socket, m_PacketOut->channel, m_PacketOut );
}

int CSPA::recoit() {
	int result = SDLNet_UDP_Recv( m_Socket, m_PacketIn );
	m_BytesRec += m_PacketIn->len;
	m_NombreRec++;
	return result;
}

string CSPA::iPInToString() const {
	stringstream str;
	Uint32 ip =  SDL_SwapBE32( m_PacketIn->address.host );
	Uint16 port = SDL_SwapBE16(  m_PacketIn->address.port );
	str << (ip>>24) << "." << ((ip>>16)&0xff) << "." << ((ip>>8)&0xff) << "." << ((ip)&0xff) << ":" << port;

	return str.str();
}

string CSPA::iPOutToString() const {
	stringstream str;
	Uint32 ip =  SDL_SwapBE32( m_PacketOut->address.host );
	Uint16 port = SDL_SwapBE16(  m_PacketOut->address.port );
	str << (ip>>24) << "." << ((ip>>16)&0xff) << "." << ((ip>>8)&0xff) << "." << ((ip)&0xff) << ":" << port;

	return str.str();
}

CSPA::~CSPA() {
	SDLNet_FreePacket( m_PacketIn );
	SDLNet_FreePacket( m_PacketOut );

	if( m_Socket )
	{
		SDLNet_UDP_Close( m_Socket );		// Libération du socket
		m_Socket = 0;
	}
}

UDPpacket *CSPA::getPacketIn() const {
	return m_PacketIn;
}

UDPpacket *CSPA::getPacketOut() const {
	return m_PacketOut;
}

string CSPA::debugToString() {
	stringstream str;

	str << "\nPaquet recu :\n";
	str << "\tIP packet  :\t" << SDLNet_ResolveIP( &m_PacketIn->address ) << endl;
	str << "\tIP CSPA :\t" << SDLNet_ResolveIP( &m_PacketIn->address ) << endl;
	str << "\tLength :\t" << m_PacketIn->len << endl;
	str << "\tPosition :\t" << m_uPosition << endl;
	str << "\tMaxLength :\t" << m_PacketIn->maxlen << endl;
	str << "\tChannel :\t" << m_PacketIn->channel << endl;
	str << "\tStatus :\t" << m_PacketIn->status << "\n\t";

	for( int i=0 ; i<m_PacketIn->len ; i=i+2 ) {
		if( (i%10) == 0 )
			str << "\n\t\t";
		else if( (i%5) == 0 )
			str << "\t";
		Uint16 nbr = SDLNet_Read16( m_PacketIn->data + i );
		str << nbr << ' ';
	}

	return str.str();
}

void CSPA::readRecapFromServer( const CPlayer &player ) {
	float vect[3];

	player.getPosition( vect );				// Position du joueur
	read3fv( vect );

	player.getVitesse( vect );				// Vitesse du joueur
	read3fv( vect );
}

void CSPA::addRecapFromServer( const CPlayer &player ) {
	float vect[3];

	player.getPosition( vect );		// Sa position
	add3fv( vect );

	player.getVitesse( vect );		// Sa vitesse
	add3fv( vect );
}

void CSPA::computeDebits(Uint32 currentTime) {
	float fDebitRec = 0, fTailleRec = 0, fDebitEm = 0, fTailleEm = 0;

	// Calcule du débit en réception
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

	// Calcule du débit en émission
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
