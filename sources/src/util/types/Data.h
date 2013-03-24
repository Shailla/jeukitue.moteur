/*
 * Data.h
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#ifndef DATA_H_
#define DATA_H_

#include <sstream>

#include "util/exception/UnserializeException.h"

namespace JktUtils {

class Data {

protected:
	enum DataType {
		INT_DATA_TYPE = 1,
		FLOAT_DATA_TYPE,
		STRING_DATA_TYPE
	};

public:
	Data();
	virtual ~Data();

	static Data* unserialize(std::istringstream& in) throw(UnserializeException);
	virtual void serialize(std::ostringstream& out) = 0;
	virtual void serializeHumanReadable(std::ostringstream& out) = 0;
};

};

#endif /* DATA_H_ */
