/*
 * IMessageStream.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <vector>
#include <string>

using namespace std;

#include "data/communication/IMessageStream.h"

IMessageStream::IMessageStream(const string& str) : istringstream(str) {

}

IMessageStream& IMessageStream::operator>>(vector<int>& data) {
	char size, var;

	*this >> size;

	for(int i=0 ; i<size ; i++) {
		*this >> var;
		data.push_back(var);
	}

	return *this;
}
