/*
 * MalformedJsonException.cpp
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#include <reseau/web/json/MalformedJsonException.h>

MalformedJsonException::MalformedJsonException(const std::string& message) : JktException("MalformedJsonException", message){

}

MalformedJsonException::~MalformedJsonException() {
}

