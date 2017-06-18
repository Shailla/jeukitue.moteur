/*
 * HttpSession.h
 *
 *  Created on: 18 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_WEB_HTTPSESSION_H_
#define SRC_RESEAU_WEB_HTTPSESSION_H_

#include "reseau/web/HttpRequest.h"

namespace jkt {

class HttpSession {
	HttpRequest* _currentRequest;

public:
	HttpSession();
	virtual ~HttpSession();
};

} /* namespace jkt */

#endif /* SRC_RESEAU_WEB_HTTPSESSION_H_ */
