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
	_atlasWidth = 0;
	_atlasHeight = 0;
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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _fonteTex);

	glBegin(GL_QUADS);

	int tx1, tx2, ty1, ty2;

	for(int i=0 ; text[i] ; i++) {
		char lettre = text[i];

		tx1 = lettres[(int)lettre].x;
		tx2 = lettres[(int)lettre].x + lettres[(int)lettre].width;
		ty1 = lettres[(int)lettre].y;
		ty2 = lettres[(int)lettre].y + lettres[(int)lettre].height;

		LOGINFO(("Lettre %c %d %d %d %d", lettre, tx1, tx2, ty1, ty2));

//		glTexCoord2f( 0, 0 );
		glTexCoord2i( tx1, ty1 );
		glVertex3f( x,		y, 		0.0 );

//		glTexCoord2f( 1.0, 0 );
		glTexCoord2i( tx2, ty1 );
		glVertex3f( x+sx, 	y, 		0.0 );

//		glTexCoord2f( 1.0, 1.0 );
		glTexCoord2i( tx2, ty2 );
		glVertex3f( x+sx, 	y+sy, 	0.0 );

//		glTexCoord2f( 0, 1.0 );
		glTexCoord2i( tx1, ty2 );
		glVertex3f( x, 		y+sy, 	0.0 );
	}

	glEnd();

	glDisable( GL_TEXTURE_2D );
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

	int rowWidth = 0;
	int rowHeight = 0;

	_atlasWidth = 0;
	_atlasHeight = 0;

	memset(lettres, 0, sizeof(lettres));

	/* *********************************************** */
	/* Calcule la taille minimale de la texture atlas  */
	/* *********************************************** */

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			LOGINFO(("Loading character %d failed!", i));
			continue;
		}

		if (rowWidth + g->bitmap.width + 1 >= MAXWIDTH) {	// Si on est au bout de la ligne on passe à la suivante
			_atlasWidth = std::max(_atlasWidth, rowWidth);
			_atlasHeight += rowHeight;

			rowWidth = 0;
			rowHeight = 0;
		}

		rowWidth += g->bitmap.width + 1;
		rowHeight = std::max(rowHeight, (int)g->bitmap.rows);
	}

	// Taille de la texture atlas
	_atlasWidth = std::max(_atlasWidth, rowWidth);
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
	int x = 0;
	int y = 0;
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
		}

		// Position de la lettre dans la texture atlas
		lettres[i].x = x;
		lettres[i].y = y;

		// Dimensions de la lettre dans la texture atlas
		lettres[i].width = g->bitmap.width;
		lettres[i].height = g->bitmap.rows;

		// Position de ce caractère par rapport au curseur
		lettres[i].position_x = g->bitmap_left;
		lettres[i].position_y = g->bitmap_top;

		// Mouvements pour la prochaine lettre
		lettres[i].avance_x = g->advance.x;
		lettres[i].avance_y = g->advance.y;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		rowHeight = std::max(rowHeight, (int)g->bitmap.rows);
		x += g->bitmap.width + 1;
	}

	LOGINFO(("Generated a %dx%d (%d kb) atlas texture with %d characters", _atlasWidth, _atlasHeight, _atlasWidth * _atlasHeight / 1024, nbrCharacters));
}
