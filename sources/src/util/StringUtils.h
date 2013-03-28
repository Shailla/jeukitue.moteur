/*
 * StringUtils.h
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <vector>
#include <string>

namespace JktUtils {

class StringUtils {
	StringUtils();
public:
	virtual ~StringUtils();

	static char* toChars(const std::string& str);
	static bool isBlank(const std::string& str);

	static std::string findAndEraseFirstWord(std::string& s);
	static std::string findAndEraseFirstString(std::string& s);
	static std::string findFirstWord(std::string& s);

	static std::vector<std::string> splitBySpaces(std::string s);

	// trim from start
	static void ltrim(std::string& s);

	// trim from end
	static void rtrim(std::string& s);

	// trim from both ends
	static void trim(std::string& s);
};

} /* namespace JktUtils */
#endif /* STRINGUTILS_H_ */
