/*
 * Fonte.h
 *
 *  Created on: 13 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_UTIL_FONTE_FONTE_H_
#define SRC_UTIL_FONTE_FONTE_H_

#include <string>

#include <GL/gl.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#define INFOFONTESCALAR 0.3f

// Maximum texture width
#define MAXWIDTH 1024

// Plus grand charact�re ASCII inclus dans l'atlas
#define MAXASCIICHAR 255

class Fonte {
	FT_Library _ft;

	GLint _fonteTex;		// Texture atlas de la fonte

	float _atlasWidth;		// Largeur de la texture atlas en pixels
	float _atlasHeight;		// Hauteur de la texture atlas en pixels

	struct {
		float _texLeft;		// Bord gauche lettre dans la texture atlas
		float _texBottom;	// Bord bas de la lettre dans la texture atlas

		float _texRight;	// Bord droit de la lettre dans la texture atlas (= x1 + width)
		float _texTop;		// Bord haut de la lettre dans la texture atlas = (y1 + height)

		float _texWidth;	// Dimensions de la lettre dans la texture atlas
		float _texHeight;	// Dimensions de la lettre dans la texture atlas

		float _width;		// Width to draw when no scalar is applied
		float _height;		// Height to draw when no scalar is applied

		float _offsetX;		// Position de ce caract�re par rapport au curseur
		float _offsetY;		// Position de ce caract�re par rapport au curseur

		float _afterX;		// Mouvements pour la prochaine lettre
		float _afterY;		// Mouvements pour la prochaine lettre
	} lettres[MAXASCIICHAR];

public:
	enum JUSTIFY {JUSTIFY_LEFT = 0, JUSTIFY_CENTER, JUSTIFY_RIGHT};

	Fonte();
	virtual ~Fonte();

	void load(const std::string& fonte, int height);

	void drawString(const std::string& text, float x, float y, float scalar, JUSTIFY justify = JUSTIFY_LEFT);
	void drawString(const std::string& text, float x, float y, float scalar, float color[], JUSTIFY justify = JUSTIFY_LEFT);
	void getStringSize(const std::string& text, float scalar, float& stringWidth, float& stringHeight) const;
};

#endif /* SRC_UTIL_FONTE_FONTE_H_ */
