/*
 * FloatData.h
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#ifndef FLOATDATA_H_
#define FLOATDATA_H_

#include "util/types/Data.h"

namespace JktUtils {

class FloatData : public Data {
	float _value;
public:
	FloatData(float value);
	FloatData(std::istringstream& in);
	virtual ~FloatData();

	void serialize(std::ostringstream& out);
	void toString(std::ostringstream& out) const;

	float getValue() const;
};

}

#endif /* FLOATDATA_H_ */
