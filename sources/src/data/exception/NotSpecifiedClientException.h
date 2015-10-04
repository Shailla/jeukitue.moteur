/*
 * NotSpecifiedClientException.h
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#ifndef NOTSPECIFIEDCLIENTEXCEPTION_H_
#define NOTSPECIFIEDCLIENTEXCEPTION_H_

#include "exception/JktException.h"

class NotSpecifiedClientException : public JktException {
	NotSpecifiedClientException();
public:
	NotSpecifiedClientException(const std::string& message);
	virtual ~NotSpecifiedClientException();
};

#endif /* NOTSPECIFIEDCLIENTEXCEPTION_H_ */
