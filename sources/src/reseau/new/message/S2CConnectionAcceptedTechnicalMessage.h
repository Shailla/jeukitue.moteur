/*
 * S2CConnectionAcceptedTechnicalMessage.h
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#ifndef S2CCONNECTIONACCEPTEDTECHNICALMESSAGE_H_
#define S2CCONNECTIONACCEPTEDTECHNICALMESSAGE_H_

#include "reseau/new/message/TechnicalMessage.h"

class S2CConnectionAcceptedTechnicalMessage: public TechnicalMessage {
	Uint16 _port;

public:
	S2CConnectionAcceptedTechnicalMessage(Uint16 port);
	virtual ~S2CConnectionAcceptedTechnicalMessage();

	int getPort() const;
	JktUtils::Bytes* toBytes();
};

#endif /* S2CCONNECTIONACCEPTEDTECHNICALMESSAGE_H_ */
