/*
 * MalformedJsonException.h
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_MALFORMEDJSONEXCEPTION_H_
#define SRC_RESEAU_WEB_JSON_MALFORMEDJSONEXCEPTION_H_

#include <string>

#include "exception/JktException.h"

class MalformedJsonException : public JktException  {
public:
	MalformedJsonException(const std::string& message);
	virtual ~MalformedJsonException();
};

#endif /* SRC_RESEAU_WEB_JSON_MALFORMEDJSONEXCEPTION_H_ */
