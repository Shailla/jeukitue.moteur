/*
 * TechnicalMessage.h
 *
 *  Created on: 20 juin 2013
 *      Author: vgdj7997
 */

#ifndef TECHNICALMESSAGE_H_
#define TECHNICALMESSAGE_H_

#include "util/types/Bytes.h"

class TechnicalMessage {
public:
	enum TECHNICAL_MESSAGE {
		/** When a client wants to initialize a connection to a server. */
		C2S_HELLO = 0,

		/** When a server accept the connection of a client. */
		S2C_CONNECTION_ACCEPTED,

		/** When a server refuse the connection of a client. */
		S2C_CONNECTION_REFUSED,

		/** When a client says to the server he is disconnecting from the server. */
		C2S_BYE,

		/** When the server says to a client that he will be disconnected. */
		S2C_DISCONNECTION,

		/** The message contains data. */
		DATA
	};
private:
	int _code;

public:
	TechnicalMessage(int code);
	virtual ~TechnicalMessage();

	static TechnicalMessage* traduct(jkt::Bytes* data);

	int getCode() const;
	virtual jkt::Bytes* toBytes() = 0;
};

#endif /* TECHNICALMESSAGE_H_ */
