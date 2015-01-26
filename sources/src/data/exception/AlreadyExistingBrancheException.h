/*
 * AlreadyExistingBrancheException.h
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#ifndef ALREADYEXISTINGBRANCHEEXCEPTION_H_
#define ALREADYEXISTINGBRANCHEEXCEPTION_H_

#include "exception/JktException.h"

class AlreadyExistingBrancheException : public JktException {
public:
	AlreadyExistingBrancheException(const std::string& message);
	virtual ~AlreadyExistingBrancheException();
};

#endif /* ALREADYEXISTINGBRANCHEEXCEPTION_H_ */
