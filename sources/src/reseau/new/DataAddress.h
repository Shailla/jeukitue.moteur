/*
 * DataAddress.h
 *
 *  Created on: 25 juin 2013
 *      Author: Erwin
 */

#ifndef DATAADDRESS_H_
#define DATAADDRESS_H_

#include "SDL.h"
#include "SDL_net.h"

#include "util/types/Bytes.h"

class DataAddress {

	IPaddress _address;
	jkt::Bytes* _bytes;

public:
	DataAddress(char* data, int size);
	DataAddress(jkt::Bytes* bytes);
	DataAddress(const IPaddress& address, char* data, int size);
	DataAddress(const IPaddress& address, jkt::Bytes* bytes);
	virtual ~DataAddress();

	const IPaddress& getAddress() const;
	jkt::Bytes* getBytes();
};

#endif /* DATAADDRESS_H_ */
