/*
 * NotExistingBrancheException.h
 *
 *  Created on: 7 mars 2013
 *      Author: vgdj7997
 */

#ifndef NOTEXISTINGVALEUREXCEPTION_H_
#define NOTEXISTINGVALEUREXCEPTION_H_

#include "exception/JktException.h"

class NotExistingValeurException : public JktException {
public:
	NotExistingValeurException(const std::string& message);
	virtual ~NotExistingValeurException();
};

#endif /* NOTEXISTINGVALEUREXCEPTION_H_ */
