/*
 * Packet.h
 *
 *  Created on: 19 août 2013
 *      Author: vgdj7997
 */

#ifndef PACKET_H_
#define PACKET_H_

#include <string>

#include "SDL_net.h"

namespace jkt
{

class Packet {
	UDPpacket* _packet;
	unsigned int _positionRead;
	unsigned int _positionWrite;

public:
	Packet();
	virtual ~Packet();

	void init();

	void setChannel(int channel);
	int getChannel() const;

	void prepareToSend();

	UDPpacket* getPacket();

	unsigned int getPositionRead() const;
	unsigned int getPositionWrite() const;

	void addCode(Uint16 code1, Uint16 code2);
	void readCode(Uint16 &code1, Uint16 &code2);

	void add16(Uint16 var);
	void add16InPosition(unsigned int position, Uint16 var);
	Uint16 read16();

	void add32(Uint32 var);
	Uint32 read32();

	void add(float var);
	float readf();
	void add3fv(const float *var);
	void read3fv(float *var);
	void addChar(const char *txt);
	void add(const std::string &txt );
	void readChar(char *txt);
	void readString(std::string &mot);
};

} /* namespace jkt */
#endif /* PACKET_H_ */
