/*
 * StringUtils.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <iostream>

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

} /* namespace JktUtils */
