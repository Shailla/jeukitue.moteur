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

namespace jkt {

class StringUtils {
	StringUtils();
public:
	virtual ~StringUtils();

	static char* toChars(const std::string& str);
	static void toChars(const std::string& str, char* out, unsigned int maxLength);

	static bool isBlank(const std::string& str);
	static bool isNotBlank(const std::string& str);

	static std::string findAndEraseFirstWord(std::string& s, int (isSeparator)(int));
	static std::string findAndEraseFirstWord(std::string& s);
	static std::string findAndEraseFirstString(std::string& s, int (isSeparator)(int));
	static std::string findAndEraseFirstString(std::string& s);
	static std::string findFirstWord(const std::string& s);

	static bool isFinishingWith(const std::string& txt, const std::string& suffix);
	static bool isBeginingWith(const std::string& txt, const std::string& prefix);

	static std::vector<std::string> splitByCarriageReturns(const std::string& s);
	static std::vector<std::string> splitBySpaces(const std::string& s);
	static std::vector<std::string> split(const std::string& s, int (isSeparator)(int));
	static std::vector<std::string> split(const std::string& s, const std::string& delimiter);
	static void splitOnce(const std::string& s, int (isSeparator)(int), std::string& debutRes, std::string& finRes);

	// trim from start
	static void ltrim(std::string& s);

	// trim from end
	static void rtrim(std::string& s);

	// trim from both ends
	static void trim(std::string& s);

	static int isCarriageReturn(int c);
	static int isGuillemet(int c);
	static int isComma(int c);
	static int isEqual(int c);
	static int isSlash(int c);
};

} /* namespace jkt */
#endif /* STRINGUTILS_H_ */
