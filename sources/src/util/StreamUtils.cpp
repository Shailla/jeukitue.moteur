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

JktUtils::AnyData StreamUtils::readData(istringstream& in) {
	return JktUtils::AnyData(in);
}

void StreamUtils::write(ostringstream& out, const JktUtils::AnyData& data) {
	data.serialize(out);
}

void StreamUtils::writeHumanReadable(ostringstream& out, const JktUtils::AnyData& data) {
	data.toString(out);
}

void StreamUtils::read(istringstream& in, int& data) {
	in.read((char*)(&data), sizeof(int));
}

void StreamUtils::write(ostringstream& out, const int& data) {
	out.write((const char*)(&data), sizeof(int));
}

void StreamUtils::read(istringstream& in, float& data) {
	in.read((char*)(&data), sizeof(float));
}

void StreamUtils::write(ostringstream& out, const float& data) {
	out.write((const char*)(&data), sizeof(float));
}

void StreamUtils::read(istringstream& in, vector<int>& data) {
	int size, var;

	in.read((char*)(&size), sizeof(int));

	for(int i=0 ; i<size ; i++) {
		in.read((char*)(&var), sizeof(int));
		data.push_back(var);
	}
}

void StreamUtils::writeHumanReadable(ostringstream& out, const vector<int>& data) {
	vector<int>::const_iterator it;
	bool first = true;

	out << "[";

	for(it = data.begin() ; it != data.end() ; it++) {
		if(!first) {
			out << ",";
		}

		first = false;
		out << *it;
	}

	out << "]";
}

void StreamUtils::write(ostringstream& out, const vector<int>& data) {
	int size = data.size();
	out.write((const char*)(&size), sizeof(int));

	vector<int>::const_iterator it;

	for(it = data.begin() ; it != data.end() ; it++) {
		out.write((const char*)(&(*it)), sizeof(int));
	}
}

void StreamUtils::read(istringstream& in, string& data) {
	int size;

	in.read((char*)(&size), sizeof(int));

	char* var = new char[size + 1];
	in.read(var, size);
	var[size] = '\0';
	data = var;
	delete[] var;
}

void StreamUtils::write(ostringstream& out, const string& data) {
	int size = data.size();
	out.write((const char*)(&size), sizeof(int));

	out.write(data.c_str(), size);
}
