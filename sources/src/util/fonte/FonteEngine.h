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

	struct {
		int x;			// Position de la lettre dans la texture atlas
		int y;			// Position de la lettre dans la texture atlas

		int width;		// Dimensions de la lettre dans la texture atlas
		int height;		// Dimensions de la lettre dans la texture atlas

		int position_x;	// Position de ce caractère par rapport au curseur
		int position_y;	// Position de ce caractère par rapport au curseur

		int avance_x;	// Mouvements pour la prochaine lettre
		int avance_y;	// Mouvements pour la prochaine lettre
	} lettres[128];

public:
	FonteEngine();
	virtual ~FonteEngine();

	void init();
	void loadFonte(const string& fonte, int height);

	void drawString(const char *text, float x, float y, float sx, float sy);
};

#endif /* SRC_UTIL_FONTE_FONTEENGINE_H_ */
