/*
 * FonteEngine.h
 *
 *  Created on: 11 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_UTIL_FONTE_FONTEENGINE_H_
#define SRC_UTIL_FONTE_FONTEENGINE_H_

#include "GL/gl.h"

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H


class FonteEngine {
	FT_Library* _ft;
	GLuint _fonteTex;

public:
	FonteEngine();
	virtual ~FonteEngine();

	void init();
	void loadFonte(const string& fonte);

	void affiche();
};

#endif /* SRC_UTIL_FONTE_FONTEENGINE_H_ */
