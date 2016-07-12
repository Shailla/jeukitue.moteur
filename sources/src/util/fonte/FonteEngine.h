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
	int _atlasWidth, _atlasHeight;

	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};

	struct {
		float ax;	// advance.x
		float ay;	// advance.y

		float bw;	// bitmap.width;
		float bh;	// bitmap.height;

		float bl;	// bitmap_left;
		float bt;	// bitmap_top;

		float tx;	// x offset of glyph in texture coordinates
		float ty;	// y offset of glyph in texture coordinates
	} lettre[128];		// character information

public:
	FonteEngine();
	virtual ~FonteEngine();

	void init();
	void loadFonte(const string& fonte, int height);

	void drawString(const char *text, float x, float y, float sx, float sy);
};

#endif /* SRC_UTIL_FONTE_FONTEENGINE_H_ */
