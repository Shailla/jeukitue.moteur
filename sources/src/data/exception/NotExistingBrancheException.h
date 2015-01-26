/*
 * NotExistingBrancheException.h
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#ifndef NOTEXISTINGBRANCHEEXCEPTION_H_
#define NOTEXISTINGBRANCHEEXCEPTION_H_

#include "exception/JktException.h"

class NotExistingBrancheException : public JktException {
public:
	NotExistingBrancheException(const std::string& message);
	virtual ~NotExistingBrancheException();
};

#endif /* NOTEXISTINGBRANCHEEXCEPTION_H_ */
