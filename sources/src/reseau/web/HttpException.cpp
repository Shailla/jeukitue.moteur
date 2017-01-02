/*
 * HttpException.cpp
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/HttpException.h"

namespace jkt {

HttpException::HttpException(HTTP_EXCEPTION code, const std::string& message) : JktException("HttpException", message){
	_code = code;
}

HttpException::~HttpException() {
}

HttpException::HTTP_EXCEPTION HttpException::getCode() const {
	return _code;
}

} // namespace jkt
