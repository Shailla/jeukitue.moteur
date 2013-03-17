/*
 * IntData.h
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#ifndef STRINGDATA_H_
#define STRINGDATA_H_

#include <string>

#include "util/types/Data.h"

namespace JktUtils {

class StringData : public Data {
	std::string _value;
public:
	StringData(const std::string& value);
	virtual ~StringData();

	void serialize(std::ostringstream& out);
	void unserialize(std::istringstream& in);
};

}

#endif /* INTDATA_H_ */
