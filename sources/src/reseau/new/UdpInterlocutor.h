/*
 * UdpInterlocutor.h
 *
 *  Created on: 18 juin 2013
 *      Author: vgdj7997
 */

#ifndef UDPINTERLOCUTOR_H_
#define UDPINTERLOCUTOR_H_

#include "reseau/new/TechnicalInterlocutor.h"

class UdpInterlocutor : TechnicalInterlocutor {
	void boucle();

public:
	UdpInterlocutor();
	virtual ~UdpInterlocutor();

	void connect();
};

#endif /* UDPINTERLOCUTOR_H_ */
