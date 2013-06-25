/*
 * S2CConnectionRefusedTechnicalMessage.h
 *
 *  Created on: 23 juin 2013
 *      Author: vgdj7997
 */

#ifndef S2CCONNECTIONREFUSEDTECHNICALMESSAGE_H_
#define S2CCONNECTIONREFUSEDTECHNICALMESSAGE_H_

#include "util/types/Bytes.h"

#include "reseau/new/message/C2SByeTechnicalMessage.h"

class S2CConnectionRefusedTechnicalMessage: public TechnicalMessage {
public:
	S2CConnectionRefusedTechnicalMessage();
	virtual ~S2CConnectionRefusedTechnicalMessage();

	JktUtils::Bytes* toBytes() const;
};

#endif /* S2CCONNECTIONREFUSEDTECHNICALMESSAGE_H_ */
