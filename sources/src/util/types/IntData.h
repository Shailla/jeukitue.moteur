/*
 * IntData.h
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#ifndef INTDATA_H_
#define INTDATA_H_

#include "util/types/Data.h"

namespace JktUtils {

class IntData : public Data {
	int _value;
public:
	IntData(int value);
	IntData(std::istringstream& in);
	virtual ~IntData();

	void serialize(std::ostringstream& out);
	void serializeHumanReadable(std::ostringstream& out);

	int getValue() const;
};

}

#endif /* INTDATA_H_ */
