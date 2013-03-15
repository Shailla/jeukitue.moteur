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

#include "util/StreamUtils.h"

void StreamUtils::read(istringstream& in, int& data) {
	in.read((char*)&data, sizeof(int));;
}

void StreamUtils::write(ostringstream& out, int data) {
	out.write((char*)&data, sizeof(int));
}

void StreamUtils::read(istringstream& in, vector<int>& data) {
	int size, var;

	in.read((char*)&size, sizeof(int));;

	for(int i=0 ; i<size ; i++) {
		in.read((char*)&var, sizeof(int));
		data.push_back(var);
	}
}

void StreamUtils::write(ostringstream& out, const vector<int>& data) {
	int var = data.size();
	out.write((char*)&var, sizeof(int));

	vector<int>::const_iterator it;

	for(it = data.begin() ; it != data.end() ; it++) {
		out.write((char*)&(*it), sizeof(int));
	}
}

void StreamUtils::read(istringstream& in, string& data) {
	int size;

	in.read((char*)&size, sizeof(int));

	char* var = new char[size];
	in.read(var, data.size());
	data = var;
	delete[] var;
}

void StreamUtils::write(ostringstream& out, const string& data) {
	int var = data.size();
	out.write((char*)&var, sizeof(int));

	vector<int>::iterator it;
	out.write(data.c_str(), data.size());
}
