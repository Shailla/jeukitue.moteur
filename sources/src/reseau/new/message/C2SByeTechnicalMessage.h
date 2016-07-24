/*
 * C2SByeTechnicalMessage.h
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#ifndef C2SBYETECHNICALMESSAGE_H_
#define C2SBYETECHNICALMESSAGE_H_

#include "reseau/new/message/TechnicalMessage.h"

class C2SByeTechnicalMessage: public TechnicalMessage {
public:
	C2SByeTechnicalMessage();
	virtual ~C2SByeTechnicalMessage();

	jkt::Bytes* toBytes();
};

#endif /* C2SBYETECHNICALMESSAGE_H_ */
