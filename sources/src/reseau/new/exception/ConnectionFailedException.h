/*
 * ConnectionFailedException.h
 *
 *  Created on: 21 juin 2013
 *      Author: vgdj7997
 */

#ifndef CONNECTIONFAILEDEXCEPTION_H_
#define CONNECTIONFAILEDEXCEPTION_H_

#include "exception/JktException.h"

class ConnectionFailedException: public JktException {
public:
	ConnectionFailedException(void);
	ConnectionFailedException(const std::string& message);
	virtual ~ConnectionFailedException();
};

#endif /* CONNECTIONFAILEDEXCEPTION_H_ */
