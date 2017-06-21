/*
 * HttpParameters.cpp
 *
 *  Created on: 11 juin 2017
 *      Author: Erwin
 */

#include <string>
#include <sstream>
#include <map>

using namespace std;

#include "reseau/web/HttpVocabulary.h"
#include "reseau/web/HttpParameters.h"

namespace jkt {

HttpParameters::HttpParameters() {
	_head = "";
	_modified = true;
}

HttpParameters::HttpParameters(const HttpParameters& basicParameters) : HttpParameters() {
	for(pair<string, string> parameter : basicParameters._parameters) {
		_parameters[parameter.first] = parameter.second;
	}
}

HttpParameters::~HttpParameters() {
}

void HttpParameters::addParameter(const char* name, const char* value) {
	_parameters[name] = value;
	_modified = true;
}

void HttpParameters::addParameter(const char* name, const string& value) {
	_parameters[name] = value;
	_modified = true;
}

void HttpParameters::addParameter(const char* name, int value) {
	stringstream var;
	var << value;
	_parameters[name] = var.str();
	_modified = true;
}

void HttpParameters::addParameter(const char* name, long value) {
	stringstream var;
	var << value;
	_parameters[name] = var.str();
	_modified = true;
}

void HttpParameters::addParameter(const char* name, size_t value) {
	stringstream var;
	var << value;
	_parameters[name] = var.str();
	_modified = true;
}

void HttpParameters::clear() {
	_parameters.clear();
}

bool HttpParameters::isEmpty() const {
	return _parameters.empty();
}

const string& HttpParameters::str() {
	stringstream head;

	if(_modified) {
		_head = "";
		_modified = false;
	}

	for(pair<string, string> parameter : _parameters) {
		head << parameter.first << HttpVocabulary::HTTP_PARAM_SEPARATOR << parameter.second << HttpVocabulary::HTTP_RETURN;
	}

	_head = head.str();

	return _head;
}

} /* namespace jkt */
