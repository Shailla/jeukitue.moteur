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
	JktUtils::Bytes* _bytes;

public:
	DataAddress(char* data, int size);
	DataAddress(JktUtils::Bytes* bytes);
	DataAddress(const IPaddress& address, char* data, int size);
	DataAddress(const IPaddress& address, JktUtils::Bytes* bytes);
	virtual ~DataAddress();

	const IPaddress& getAddress() const;
	JktUtils::Bytes* getBytes();
};

#endif /* DATAADDRESS_H_ */
