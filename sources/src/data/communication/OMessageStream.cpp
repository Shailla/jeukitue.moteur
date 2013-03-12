/*
 * OMessageStream.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <vector>

using namespace std;

#include "data/communication/OMessageStream.h"

ostringstream& operator<<(ostringstream& out, const std::vector<int>& data) {
	out << data.size();

	vector<int>::iterator it;

	for(it = data.begin() ; it != data.end() ; it++) {
		out << *it;
	}

	return out;
}
