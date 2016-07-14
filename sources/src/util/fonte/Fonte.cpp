/*
 * Fonte.cpp
 *
 *  Created on: 13 juil. 2016
 *      Author: Erwin
 */

#include <algorithm>

#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

using namespace std;

#include "util/Trace.h"

#include "util/fonte/Fonte.h"

// Maximum texture width
#define MAXWIDTH 1024

Fonte::Fonte() :_ft() {
	_atlasWidth = 0;
	_atlasHeight = 0;
	_fonteTex = -1;

	if(FT_Init_FreeType(&_ft)) {
		LOGERROR(("Librairie FreeType introuvable"));
	}
	else {
		LOGINFO(("Librairie FreeType initialisée"));
	}
}

void Fonte::load(const string& fonte, int height) {
	FT_Face face;

	/* ************************************** */
	/* Charge la fonte                        */
	/* ************************************** */

	if(FT_New_Face(_ft, fonte.c_str(), 0, &face)) {
		LOGERROR(("Echec d'ouverture de la fonte : %s", fonte.c_str()));
		return;
	}
	else {
		LOGINFO(("Fonte chargée : %s", fonte.c_str()));
	}

	FT_Set_Pixel_Sizes(face, 0, height);



	memset(lettres, 0, sizeof(lettres));

	FT_GlyphSlot g = face->glyph;

	int rowWidth = 0;
	int rowHeight = 0;


	/* *********************************************** */
	/* Calcule la taille minimale de la texture atlas  */
	/* *********************************************** */

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			continue;
		}

		if (rowWidth + g->bitmap.width + 1 >= MAXWIDTH) {	// Si on est au bout de la ligne on passe à la suivante
			_atlasWidth = std::max((int)_atlasWidth, rowWidth);
			_atlasHeight += rowHeight;

			rowWidth = 0;
			rowHeight = 0;
		}

		rowWidth += g->bitmap.width + 1;
		rowHeight = std::max(rowHeight, (int)g->bitmap.rows);
	}

	// Taille de la texture atlas
	_atlasWidth = std::max((int)_atlasWidth, rowWidth);
	_atlasHeight += rowHeight;


	/* *********************************************** */
	/* Crée la texture atlas                           */
	/* *********************************************** */

	/* Create a texture that will be used to hold all ASCII glyphs */
	glGenTextures(1, &_fonteTex);
	glBindTexture(GL_TEXTURE_2D, _fonteTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _atlasWidth, _atlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/* *********************************************** */
	/* Remplie la texture atlas                        */
	/* *********************************************** */

	int nbrCharacters = 0;
	float x = 0;
	float y = 0;
	rowHeight = 0;

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			LOGINFO(("Loading character %d failed!", i));
			continue;
		}

		nbrCharacters++;

		if (x + g->bitmap.width + 1 >= MAXWIDTH) {
			x = 0;
			y += rowHeight;
			rowHeight = 0;
		}

		// Position de la lettre dans la texture atlas
		lettres[i]._texLeft = x / _atlasWidth;
		lettres[i]._texBottom = y / _atlasHeight;

		lettres[i]._texRight = (x + (float)g->bitmap.width) / _atlasWidth;
		lettres[i]._texTop = (y + (float)g->bitmap.rows) / _atlasHeight;

		// Dimensions de la lettre dans la texture atlas
		lettres[i]._texWidth = float(g->bitmap.width) / _atlasWidth;
		lettres[i]._texHeight = float(g->bitmap.rows) / _atlasHeight;

		// Position de ce caractère par rapport au curseur
		lettres[i]._deltaX = float(g->bitmap_left) / _atlasWidth;
		lettres[i]._deltaY = float(g->bitmap_top) / _atlasHeight;

		// Mouvements pour la prochaine lettre
		lettres[i]._afterX = float(g->advance.x >> 6) / _atlasWidth;
		lettres[i]._afterY = float(g->advance.y >> 6) / _atlasHeight;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		rowHeight = std::max(rowHeight, (int)g->bitmap.rows);
		x += g->bitmap.width +1;
	}

	LOGINFO(("Atlas texture generated : %fx%f (%f kb) with %d characters", _atlasWidth, _atlasHeight, _atlasWidth * _atlasHeight / 1024.0f, nbrCharacters));
}

Fonte::~Fonte() {
	if(_fonteTex != -1) {
		glDeleteTextures(1, &_fonteTex);
	}
}

void Fonte::drawString(const string& text, float x, float y, float scalar, float color[]) {
	glColor3f(color[0], color[1], color[2]);
	drawString(text, x, y, scalar);
}

void Fonte::drawString(const string& text, float x, float y, float scalar) {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, _fonteTex);

	glBegin(GL_QUADS);

	float tx1, tx2, ty1, ty2, width, height, deltaX, deltaY;

	for(int i=0 ; text[i] ; i++) {
		int lettre = text[i];

		tx1 = lettres[lettre]._texLeft;
		tx2 = lettres[lettre]._texRight;
		ty1 = lettres[lettre]._texBottom;
		ty2 = lettres[lettre]._texTop;

		width = lettres[lettre]._texWidth * scalar;
		height = lettres[lettre]._texHeight * scalar;

		deltaX = lettres[lettre]._deltaX * scalar;
		deltaY = lettres[lettre]._deltaY * scalar;

		glTexCoord2f( tx1, ty2 );
		glVertex3f( x + deltaX,				y + deltaY - height,	0.0 );

		glTexCoord2f( tx2, ty2 );
		glVertex3f( x + deltaX + width, 	y + deltaY - height,	0.0 );

		glTexCoord2f( tx2, ty1 );
		glVertex3f( x + deltaX + width, 	y + deltaY,				0.0 );

		glTexCoord2f( tx1, ty1 );
		glVertex3f( x + deltaX, 			y + deltaY,				0.0 );

		x += lettres[lettre]._afterX * scalar;
		y += lettres[lettre]._afterY * scalar;
	}

	glEnd();

	glDisable( GL_TEXTURE_2D );
}
