/*
 * C2SHelloTechnicalMessage.h
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#ifndef C2SHELLOTECHNICALMESSAGE_H_
#define C2SHELLOTECHNICALMESSAGE_H_

#include "util/types/Bytes.h"

#include "reseau/new/message/TechnicalMessage.h"

class C2SHelloTechnicalMessage: public TechnicalMessage {
public:
	C2SHelloTechnicalMessage();
	virtual ~C2SHelloTechnicalMessage();

	jkt::Bytes* toBytes();
};

#endif /* C2SHELLOTECHNICALMESSAGE_H_ */
