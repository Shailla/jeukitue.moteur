/*
 * FonteEngine.h
 *
 *  Created on: 11 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_UTIL_FONTE_FONTEENGINE_H_
#define SRC_UTIL_FONTE_FONTEENGINE_H_

#include <string>

#include <GL/gl.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include "util/fonte/Fonte.h"

class FonteEngine {
	FT_Library _ft;

public:
	FonteEngine();
	virtual ~FonteEngine();

	Fonte* loadFonte(const std::string& fonteFile, int height);
};

#endif /* SRC_UTIL_FONTE_FONTEENGINE_H_ */
