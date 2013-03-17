/*
 * NotExistingBrancheException.h
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#ifndef DATACOMMUNICATIONEXCEPTION_H_
#define DATACOMMUNICATIONEXCEPTION_H_

#include <string>

#include "exception/JktException.h"

class DataCommunicationException : public JktException {
public:
	DataCommunicationException();
	DataCommunicationException(const std::string& message);
	virtual ~DataCommunicationException();
};

#endif /* NOTEXISTINGBRANCHEEXCEPTION_H_ */
