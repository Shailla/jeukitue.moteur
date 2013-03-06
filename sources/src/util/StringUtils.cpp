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
 * Attention, cette méthode n'est pas finie, pour l'instant elle vérifie uniquement si la chaîne n'est pas vide !
 * TODO Finir l'implémentation de la présente méthode
 * ""		: true
 * "     "	: true
 * "-"		: false
 * "  - "	: false
 */
bool StringUtils::isBlank(const char* str) {
	if(strlen(str) == 0) {
		return true;
	}

	return false;
}

string StringUtils::findFirstWord(string& s) {
	string result;

	string::iterator debutMot = s.begin();

	debutMot = find_if(debutMot, s.end(), not1(ptr_fun<int, int>(isspace)));

	if(debutMot != s.end()) {
		string::iterator finMot = find_if(debutMot, s.end(), not1(ptr_fun<int, int>(isspace)));
		result = string(debutMot, finMot);
	}

	return result;
}

vector<string> StringUtils::splitBySpaces(string& s) {
	vector<string> result;

	string::iterator debutMot = s.begin();

	do {
		debutMot = find_if(debutMot, s.end(), not1(ptr_fun<int, int>(isspace)));

		if(debutMot != s.end()) {
			string::iterator finMot = find_if(debutMot, s.end(), not1(ptr_fun<int, int>(isspace)));
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
