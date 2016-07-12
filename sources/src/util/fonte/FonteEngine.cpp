/*
 * FonteEngine.cpp
 *
 *  Created on: 11 juil. 2016
 *      Author: Erwin
 */

#include <algorithm>

#include <GL/glew.h>

#include "util/Trace.h"

#include "util/fonte/FonteEngine.h"

// Maximum texture width
#define MAXWIDTH 1024

FonteEngine::FonteEngine() {
	_ft = new FT_Library();
	_fonteTex = -1;
}

FonteEngine::~FonteEngine() {
}

void FonteEngine::init() {
	if(FT_Init_FreeType(_ft)) {
		LOGERROR(("Librairie FreeType introuvable"));
	}
	else {
		LOGINFO(("Librairie FreeType initialisée"));
	}
}

void FonteEngine::drawString(const char *text, float x, float y, float sx, float sy) {
	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	} coords[6 * strlen(text)];

	int n = 0;

	for(const char* txtIndex = text; *txtIndex; txtIndex++) {
		int p = *txtIndex;
		float x2 =  x + lettre[p].bl * sx;
		float y2 = -y - lettre[p].bt * sy;
		float w = lettre[p].bw * sx;
		float h = lettre[p].bh * sy;

		/* Advance the cursor to the start of the next character */
		x += lettre[p].ax * sx;
		y += lettre[p].ay * sy;

		/* Skip glyphs that have no pixels */
		if(!w || !h)
			continue;

		coords[n++] = (point){x2,     -y2    , lettre[p].tx,                                            0};
		coords[n++] = (point){x2 + w, -y2    , lettre[p].tx + lettre[p].bw / _atlasWidth,   0};
		coords[n++] = (point){x2,     -y2 - h, lettre[p].tx,                                          lettre[p].bh / _atlasHeight}; //remember: each glyph occupies a different amount of vertical space
		coords[n++] = (point){x2 + w, -y2    , lettre[p].tx + lettre[p].bw / _atlasWidth,   0};
		coords[n++] = (point){x2,     -y2 - h, lettre[p].tx,                                          lettre[p].bh / _atlasHeight};
		coords[n++] = (point){x2 + w, -y2 - h, lettre[p].tx + lettre[p].bw / _atlasWidth, lettre[p].bh / _atlasHeight};
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, n);
}


void FonteEngine::loadFonte(const string& fonte, int height) {
	FT_Face face;

	/* ************************************** */
	/* Charge la fonte                        */
	/* ************************************** */

	if(FT_New_Face(*_ft, fonte.c_str(), 0, &face)) {
		LOGERROR(("Echec d'ouverture de la fonte : %s", fonte.c_str()));
	}
	else {
		LOGINFO(("Fonte chargée : %s", fonte.c_str()));
	}

	FT_Set_Pixel_Sizes(face, 0, height);

	FT_GlyphSlot g = face->glyph;

	int roww = 0;
	int rowh = 0;
	_atlasWidth = 0;
	_atlasHeight = 0;

	memset(lettre, 0, sizeof lettre);

	/* Find minimum size for a texture holding all visible ASCII characters */
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}
		if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
			_atlasWidth = std::max(_atlasWidth, roww);
			_atlasHeight += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + 1;
		rowh = std::max(rowh, (int)g->bitmap.rows);
	}

	_atlasWidth = std::max(_atlasWidth, roww);
	_atlasHeight += rowh;

	/* Create a texture that will be used to hold all ASCII glyphs */
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_fonteTex);
	glBindTexture(GL_TEXTURE_2D, _fonteTex);
	//	glUniform1i(uniform_tex, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _atlasWidth, _atlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Paste all glyph bitmaps into the texture, remembering the offset */
	int ox = 0;
	int oy = 0;
	int nbrCharacters = 0;
	rowh = 0;

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		nbrCharacters++;

		if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		lettre[i].ax = g->advance.x >> 6;
		lettre[i].ay = g->advance.y >> 6;

		lettre[i].bw = g->bitmap.width;
		lettre[i].bh = g->bitmap.rows;

		lettre[i].bl = g->bitmap_left;
		lettre[i].bt = g->bitmap_top;

		lettre[i].tx = ox / (float)_atlasWidth;
		lettre[i].ty = oy / (float)_atlasHeight;

		rowh = std::max(rowh, (int)g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}

	LOGINFO(("Generated a %dx%d (%d kb) atlas texture with %d characters", _atlasWidth, _atlasHeight, _atlasWidth * _atlasHeight / 1024, nbrCharacters));
}
