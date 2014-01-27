/*
 * StringData.h
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
	StringData(const char* value);
	StringData(const std::string& value);
	StringData(std::istringstream& in);
	virtual ~StringData();

	void serialize(std::ostringstream& out);
	void toString(std::ostringstream& out);

	const std::string& getValue() const;
};

}

#endif /* STRINGDATA_H_ */
