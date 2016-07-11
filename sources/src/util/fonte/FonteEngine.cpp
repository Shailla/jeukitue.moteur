/*
 * FonteEngine.cpp
 *
 *  Created on: 11 juil. 2016
 *      Author: Erwin
 */
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "main/Extensions.h"

#include "util/Trace.h"

#include "util/fonte/FonteEngine.h"

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

void FonteEngine::loadFonte(const string& fonte) {
	FT_Face face;

	if(FT_New_Face(*_ft, fonte.c_str(), 0, &face)) {
		LOGERROR(("Echec d'ouverture de la fonte : %s", fonte.c_str()));
	}
	else {
		LOGINFO(("Fonte chargée : %s", fonte.c_str()));
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if(FT_Load_Char(face, 'k', FT_LOAD_RENDER)) {
		LOGERROR(("Echec de lecture de X"));
	}
	else {
		LOGINFO(("X lu"));
	}

	// Création de la texture de la fonte
	glGenTextures(1, &_fonteTex);
	glBindTexture(GL_TEXTURE_2D, _fonteTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	FT_GlyphSlot g = face->glyph;

	glTexImage2D(	GL_TEXTURE_2D,
			0,
			GL_RGB,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_LUMINANCE_ALPHA,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer);
}

void FonteEngine::affiche() {
	glBindTexture(GL_TEXTURE_2D, _fonteTex);

	glEnable( GL_TEXTURE_2D );

	glColor3f( 1.0f, 1.0f, 1.0f );

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(100.0, 200.0, 0.0);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(200.0, 200.0, 0.0);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(200.0, 300.0, 0.0);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(100.0, 300.0, 0.0);
	glEnd();
}
