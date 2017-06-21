/*
 * HttpResponse.cpp
 *
 *  Created on: 29 mai 2017
 *      Author: Erwin
 */

#include <string>
#include <stdlib.h>

#include "reseau/web/HttpVocabulary.h"

#include "reseau/web/HttpResponse.h"

using namespace std;

namespace jkt {

HttpResponse::HttpResponse() : _parameters() {
	_responseSize = 0;
	_responseContent = 0;
}

void HttpResponse::updateToEmptyPaquet() {
	_status.clear();
	_parameters.clear();
	_body.clear();

	_responseSize = 0;
	_responseContent = malloc(0);
}

void HttpResponse::update(const string& status, const HttpParameters& parameters, const std::string& body) {
	_status = status;
	_parameters = parameters;
	_body = body;

	generateResponse();
}

void HttpResponse::update(const string& status, const HttpParameters& parameters, const void* bodyContent, long bodySize) {
	_status = status;
	_parameters = parameters;
	_body.assign((char*)bodyContent, (size_t)bodySize);

	generateResponse();
}

void HttpResponse::reset() {
	_status.clear();
	_parameters.clear();
	_body.clear();

	_responseSize = 0;
	_responseContent = 0;
}

bool HttpResponse::isEmpty() const {
	return !_responseContent;
}

long HttpResponse::getResponseSize() const {
	return _responseSize;
}

void* HttpResponse::getResponseContent() const {
	return _responseContent;
}

void HttpResponse::generateResponse() {
	stringstream response;
	response << HttpVocabulary::HTTP_HEAD << " " << _status << HttpVocabulary::HTTP_RETURN;
	response << _parameters.str();
	response << HttpVocabulary::HTTP_RETURN;
	response << _body;

	if(_responseContent) {
		free(_responseContent);
	}

	_responseSize = response.str().size();
	_responseContent = malloc(_responseSize);
	response.str().copy((char*)_responseContent, _responseSize);
}

string HttpResponse::getStatus() const {
	stringstream status;
	status << HttpVocabulary::HTTP_HEAD << " " << _status;

	return status.str();
}

string HttpResponse::getHeader() {
	stringstream header;

	header << HttpVocabulary::HTTP_HEAD << " " << _status << HttpVocabulary::HTTP_RETURN;
	header << _parameters.str();

	return header.str();
}

string HttpResponse::getResponseString() const {
	return string((char*)_responseContent, _responseSize);
}

HttpResponse::~HttpResponse() {
}

} /* namespace jkt */
