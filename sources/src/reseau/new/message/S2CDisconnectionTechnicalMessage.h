/*
 * S2CConnectionRefusedTechnicalMessage.h
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#ifndef S2DISCONNECTIONTECHNICALMESSAGE_H_
#define S2DISCONNECTIONTECHNICALMESSAGE_H_

#include "reseau/new/message/TechnicalMessage.h"

class S2DisconnectionTechnicalMessage: public TechnicalMessage {
public:
	S2DisconnectionTechnicalMessage();
	virtual ~S2DisconnectionTechnicalMessage();

	JktUtils::Bytes* toBytes();
};

#endif /* S2DISCONNECTIONTECHNICALMESSAGE_H_ */
