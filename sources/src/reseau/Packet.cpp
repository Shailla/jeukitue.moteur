/*
 * Packet.cpp
 *
 *  Created on: 19 août 2013
 *      Author: vgdj7997
 */

#include "reseau/Packet.h"

using namespace std;

namespace jkt
{

Packet::Packet() {
	_packet = SDLNet_AllocPacket(65535);
	_positionRead = 0;
	_positionWrite = 0;
}

Packet::~Packet() {
	SDLNet_FreePacket(_packet);
}

void Packet::init() {
	_positionRead = 0;
	_positionWrite = 0;
}

void Packet::prepareToSend() {
	_packet->len = _positionWrite;
}

UDPpacket* Packet::getPacket() {
	return _packet;
}

void Packet::setChannel(int channel) {
	_packet->channel = channel;
}

int Packet::getChannel() const {
	return _packet->channel;
}

unsigned int Packet::getPositionRead() const {
	return _positionRead;
}

unsigned int Packet::getPositionWrite() const {
	return _positionWrite;
}

void Packet::addCode(Uint16 code1, Uint16 code2) {
	SDLNet_Write16( code1, _packet->data+_positionWrite );
	SDLNet_Write16( code2, _packet->data+2+_positionWrite );
	_positionWrite += 4;
}

void Packet::readCode(Uint16& code1, Uint16& code2) {
	code1 = SDLNet_Read16( _packet->data+_positionRead );
	code2 = SDLNet_Read16( _packet->data+2+_positionRead );
	_positionRead += 4;
}

void Packet::add16(Uint16 var) {
	SDLNet_Write16( var, _packet->data+_positionWrite );
	_positionWrite += 2;
}

void Packet::add16InPosition( unsigned int position, Uint16 var )
{	SDLNet_Write16( var, _packet->data+position );	}

Uint16 Packet::read16() {
	Uint16 var =  SDLNet_Read16( _packet->data+_positionRead );
	_positionRead += 2;
	return var;
}

void Packet::add32(Uint32 var) {
	SDLNet_Write32( var, _packet->data+_positionWrite );
	_positionWrite += 4;
}

Uint32 Packet::read32() {
	Uint32 var = SDLNet_Read32( _packet->data+_positionRead );
	_positionRead += 4;
	return var;
}

void Packet::add( float var )	// Ajoute un flottant (c'est à dire 4 octets)
{
	*((char*)_packet->data+_positionWrite) = ((char*)&var)[0];
	*((char*)_packet->data+_positionWrite+1) = ((char*)&var)[1];
	*((char*)_packet->data+_positionWrite+2) = ((char*)&var)[2];
	*((char*)_packet->data+_positionWrite+3) = ((char*)&var)[3];

	_positionWrite += 4;
}

float Packet::readf()	// Lit un flottant (c'est à dire 4 octets)
{
	float result;
	((char*)&result)[0] = *((char*)_packet->data+_positionRead);
	((char*)&result)[1] = *((char*)_packet->data+_positionRead+1);
	((char*)&result)[2] = *((char*)_packet->data+_positionRead+2);
	((char*)&result)[3] = *((char*)_packet->data+_positionRead+3);

	_positionRead += 4;
	return result;
}

void Packet::add3fv(const float *var )	// Ajoute un tableau de 3 flottants
{
	*((char*)_packet->data+_positionWrite) = ((char*)var)[0];
	*((char*)_packet->data+_positionWrite+1) = ((char*)var)[1];
	*((char*)_packet->data+_positionWrite+2) = ((char*)var)[2];
	*((char*)_packet->data+_positionWrite+3) = ((char*)var)[3];

	*((char*)_packet->data+_positionWrite+4) = ((char*)var)[4];
	*((char*)_packet->data+_positionWrite+5) = ((char*)var)[5];
	*((char*)_packet->data+_positionWrite+6) = ((char*)var)[6];
	*((char*)_packet->data+_positionWrite+7) = ((char*)var)[7];

	*((char*)_packet->data+_positionWrite+8) = ((char*)var)[8];
	*((char*)_packet->data+_positionWrite+9) = ((char*)var)[9];
	*((char*)_packet->data+_positionWrite+10) = ((char*)var)[10];
	*((char*)_packet->data+_positionWrite+11) = ((char*)var)[11];

	_positionWrite += 12;
}

void Packet::read3fv( float *var ) {	// Lit un tableau de 3 flottants
	((char*)var)[0] = *((char*)_packet->data+_positionRead);
	((char*)var)[1] = *((char*)_packet->data+_positionRead+1);
	((char*)var)[2] = *((char*)_packet->data+_positionRead+2);
	((char*)var)[3] = *((char*)_packet->data+_positionRead+3);

	((char*)var)[4] = *((char*)_packet->data+_positionRead+4);
	((char*)var)[5] = *((char*)_packet->data+_positionRead+5);
	((char*)var)[6] = *((char*)_packet->data+_positionRead+6);
	((char*)var)[7] = *((char*)_packet->data+_positionRead+7);

	((char*)var)[8] = *((char*)_packet->data+_positionRead+8);
	((char*)var)[9] = *((char*)_packet->data+_positionRead+9);
	((char*)var)[10] = *((char*)_packet->data+_positionRead+10);
	((char*)var)[11] = *((char*)_packet->data+_positionRead+11);

	_positionRead += 12;
}

void Packet::addChar( const char *txt ) {
	unsigned int len = (unsigned int)strlen( txt );
	strcpy( (char*)_packet->data+_positionWrite, txt );		// Copie le texte -> paquet
	_positionWrite += len + 1;								// Taille du txt + 2 octets pour len
}

void Packet::add( const string &txt ) {
	unsigned int len = (unsigned int)txt.size();
	strcpy( (char*)_packet->data+_positionWrite, txt.c_str() );		// Copie le texte -> paquet
	_positionWrite += len + 1;								// Taille du txt + 2 octets pour len
}

void Packet::readChar( char *txt ) {
	strcpy( txt, (char*)_packet->data+_positionRead );
	_positionRead += (unsigned int)strlen( txt ) + 1;
}

void Packet::readString( string &mot ) {
	mot = (char*)_packet->data+_positionRead;
	_positionRead += (unsigned int)mot.length() + 1;
}

} /* namespace jkt */
