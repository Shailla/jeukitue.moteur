/*
 * StringUtils.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <vector>
#include <iostream>
#include <string.h>
#include <algorithm>

using namespace std;

#include "ctype.h"

#include "util/StringUtils.h"

namespace JktUtils {

StringUtils::StringUtils() {
}

StringUtils::~StringUtils() {
}

/**
 * Copy a string instance in a char* and :
 *    - allocate the memory for the char*
 *    - terminate the char* with the termination character ('\0')
 */
char* StringUtils::toChars(const std::string& str) {
	char* chars = new char[str.length() + 1]();
	str.copy(chars, str.length());
	chars[str.length()] = '\0';

	return chars;
}

/**
 * Copy a string instance in a char* and :
 *    - allocate the memory for the char*
 *    - terminate the char* with the termination character ('\0')
 *    - limit the size to maxLength bytes
 */
void StringUtils::toChars(const std::string& str, char* out, int maxLength) {
	int length = (str.length()<maxLength)?str.length():maxLength;

	str.copy(out, length);
	out[length] = '\0';
}

/**
 * Attention, cette méthode n'est pas finie, pour l'instant elle vérifie uniquement si la chaîne n'est pas vide !
 * ""		: true
 * "     "	: true
 * "-"		: false
 * "  - "	: false
 */
bool StringUtils::isBlank(const string& s) {
	return find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))) == s.end();
}

string StringUtils::findAndEraseFirstWord(string& s) {
	string result;

	string::iterator debutMot = find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)));

	if(debutMot != s.end()) {
		string::iterator finMot = find_if(debutMot, s.end(), ptr_fun<int, int>(isspace));
		result = string(debutMot, finMot);

		// Erase the first word
		s.erase(s.begin(), finMot);
	}

	return result;
}

int StringUtils::isGuillemet(int c) {
	return c == '"';
}

string StringUtils::findAndEraseFirstString(string& s) {
	string result;

	string::iterator debutString = find_if(s.begin(), s.end(), ptr_fun<int, int>(isGuillemet));
	string::iterator finString = find_if(debutString + 2, s.end(), ptr_fun<int, int>(isGuillemet));

	if(finString < s.end() && debutString + 1 < finString) {
		result = string(debutString + 1, finString);

		// Erase the first word
		s.erase(s.begin(), finString + 1);
	}

	return result;
}

string StringUtils::findFirstWord(string& s) {
	string result;

	string::iterator debutMot = find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)));

	if(debutMot != s.end()) {
		string::iterator finMot = find_if(debutMot, s.end(), ptr_fun<int, int>(isspace));
		result = string(debutMot, finMot);
	}

	return result;
}

vector<string> StringUtils::splitBySpaces(string s) {
	return split(s, isspace);
}

vector<string> StringUtils::split(string s, int (isSeparator)(int)) {
	vector<string> result;

	string::iterator finMot = s.begin();
	string::iterator debutMot;

	do {
		debutMot = find_if(finMot, s.end(), not1(ptr_fun<int, int>(isSeparator)));

		if(debutMot != s.end()) {
			finMot = find_if(debutMot, s.end(), ptr_fun<int, int>(isSeparator));
			result.push_back(string(debutMot, finMot));
		}
	} while(debutMot != s.end());

	return result;
}

// trim from start
void StringUtils::ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}

// trim from end
void StringUtils::rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
}

// trim from both ends
void StringUtils::trim(string &s) {
	rtrim(s);
	ltrim(s);
}

} /* namespace JktUtils */
