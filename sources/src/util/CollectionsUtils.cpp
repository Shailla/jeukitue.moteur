/*
 * StringUtils.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "util/CollectionsUtils.h"

using namespace std;

namespace jkt {

CollectionsUtils::CollectionsUtils() {
}

CollectionsUtils::~CollectionsUtils() {
}

string CollectionsUtils::toString(const vector<int>& data) {
	ostringstream str;

	str << "[";

	bool first = true;
	vector<int>::const_iterator iter;

	for(iter = data.begin() ; iter != data.end() ; iter++) {
		if(!first) {
			str << ", ";
		}

		first = false;
		str << *iter;
	}

	str << "]";

	return str.str();
}


} /* namespace jkt */
