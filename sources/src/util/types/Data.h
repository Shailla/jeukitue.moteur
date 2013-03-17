/*
 * Data.h
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#ifndef DATA_H_
#define DATA_H_

#include <sstream>

namespace JktUtils {

class Data {
public:
	Data();
	virtual ~Data();

	virtual void serialize(std::ostringstream& out) = 0;
	virtual void unserialize(std::istringstream& in) = 0;
};

};

#endif /* DATA_H_ */
