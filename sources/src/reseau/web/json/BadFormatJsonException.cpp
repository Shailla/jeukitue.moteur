/*
 * BadFormatJsonException.cpp
 *
 *  Created on: 29 déc. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/BadFormatJsonException.h"

BadFormatJsonException::BadFormatJsonException(const std::string& message) : JktException("BadFormatJsonException", message){

}

BadFormatJsonException::~BadFormatJsonException() {
}

