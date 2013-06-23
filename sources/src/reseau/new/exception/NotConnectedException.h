/*
 * NotConnectedException.h
 *
 *  Created on: 21 juin 2013
 *      Author: vgdj7997
 */

#ifndef NOTCONNECTEDEXCEPTION_H_
#define NOTCONNECTEDEXCEPTION_H_

#include "exception/JktException.h"

class NotConnectedException: public JktException {
public:
	NotConnectedException(void);
	NotConnectedException(const std::string& message);
	virtual ~NotConnectedException();
};

#endif /* CONNECTIONFAILEDEXCEPTION_H_ */
