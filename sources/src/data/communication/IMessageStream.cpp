/*
 * IMessageStream.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <vector>
#include <string>
#include <sstream>

using namespace std;

#include "data/communication/IMessageStream.h"

istringstream& operator>>(istringstream& in, vector<int>& data) {
	char size, var;

	in >> size;

	for(int i=0 ; i<size ; i++) {
		in >> var;
		data.push_back(var);
	}

	return in;
}
