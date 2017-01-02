/*
 * HttpException.h
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_JSON_HTTPEXCEPTION_H_
#define SRC_RESEAU_WEB_JSON_HTTPEXCEPTION_H_

#include <string>

#include "exception/JktException.h"

namespace jkt {

class HttpException : public JktException  {
public:
	enum HTTP_EXCEPTION {
		RESOURCE_NOT_FOUND_EXCEPTION = 1,
		SERVICE_NOT_EXISTS = 2,
		MALFORMED_HTTP_REQUEST = 3
	};

private:
	HTTP_EXCEPTION _code;

public:
	HttpException(HTTP_EXCEPTION code, const std::string& message);
	virtual ~HttpException();

	HTTP_EXCEPTION getCode() const;
};

} // namespace jkt

#endif /* SRC_RESEAU_WEB_JSON_HTTPEXCEPTION_H_ */
