/*
 * Fonte.h
 *
 *  Created on: 13 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_UTIL_FONTE_FONTE_H_
#define SRC_UTIL_FONTE_FONTE_H_

#include <GL/gl.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

class Fonte {
	GLuint _fonteTex;		// Texture atlas de la fonte

	float _atlasWidth;		// Largeur de la texture atlas en pixels
	float _atlasHeight;		// Hauteur de la texture atlas en pixels

	struct {
		float _texLeft;		// Bord gauche lettre dans la texture atlas
		float _texBottom;	// Bord bas de la lettre dans la texture atlas

		float _texRight;	// Bord droit de la lettre dans la texture atlas (= x1 + width)
		float _texTop;		// Bord haut de la lettre dans la texture atlas = (y1 + height)

		float _texWidth;	// Dimensions de la lettre dans la texture atlas
		float _texHeight;	// Dimensions de la lettre dans la texture atlas

		float _deltaX;		// Position de ce caractère par rapport au curseur
		float _deltaY;		// Position de ce caractère par rapport au curseur

		float _afterX;		// Mouvements pour la prochaine lettre
		float _afterY;		// Mouvements pour la prochaine lettre
	} lettres[128];

public:
	Fonte(FT_Face& face);
	virtual ~Fonte();

	void drawString(const std::string& text, float x, float y, float scalar, float color[]);
	void drawString(const std::string& text, float x, float y, float scalar);
};

#endif /* SRC_UTIL_FONTE_FONTE_H_ */
