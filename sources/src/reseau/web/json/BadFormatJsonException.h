/*
 * BadFormatJsonException.h
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_BADFORMATJSONEXCEPTION_H_
#define SRC_RESEAU_WEB_JSON_BADFORMATJSONEXCEPTION_H_

#include <string>

#include "exception/JktException.h"

class BadFormatJsonException : public JktException  {
public:
	BadFormatJsonException(const std::string& message);
	virtual ~BadFormatJsonException();
};

#endif /* SRC_RESEAU_WEB_JSON_BADFORMATJSONEXCEPTION_H_ */
