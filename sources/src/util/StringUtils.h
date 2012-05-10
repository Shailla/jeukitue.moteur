/*
 * StringUtils.h
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <string>

namespace JktUtils {

class StringUtils {
	StringUtils();
public:
	virtual ~StringUtils();

	static char* toChars(const std::string& str);
};

} /* namespace JktUtils */
#endif /* STRINGUTILS_H_ */
