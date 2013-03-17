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

class FloatData : public Data {
	int _value;
public:
	FloatData(float value);
	virtual ~FloatData();

	void serialize(std::ostringstream& out);
	void unserialize(std::istringstream& in);

	int getValue() const;
};

}

#endif /* INTDATA_H_ */
