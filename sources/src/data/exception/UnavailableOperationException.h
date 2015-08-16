/*
 * UnavailableOperationException.h
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#ifndef UNAVAILABLEOPERATIONEXCEPTION_H_
#define UNAVAILABLEOPERATIONEXCEPTION_H_

#include "exception/JktException.h"

class UnavailableOperationException : public JktException {
public:
	UnavailableOperationException(const std::string& message);
	virtual ~UnavailableOperationException();
};

#endif /* UNAVAILABLEOPERATIONEXCEPTION_H_ */
