/*
 * HttpSession.cpp
 *
 *  Created on: 18 juin 2017
 *      Author: Erwin
 */

#include "reseau/web/HttpSession.h"

namespace jkt {

HttpSession::HttpSession() {
	_currentRequest = 0;
}

HttpSession::~HttpSession() {
}

HttpRequest* HttpSession::getCurrentRequest() {
	return _currentRequest;
}

} /* namespace jkt */
