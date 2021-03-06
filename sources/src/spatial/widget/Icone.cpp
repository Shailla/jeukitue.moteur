/*
 * Icone.cpp
 *
 *  Created on: 15 oct. 2012
 *      Author: Erwin
 */

#include <iostream>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"

using namespace std;

#include "spatial/widget/Icone.h"

namespace JktMoteur {

Icone::Icone(const string& nomFichier, bool alpha, int hauteur, int largeur, Uint8* pixels) {
	_nomFichier = nomFichier;
	_alpha = alpha;
	_pixels = pixels;
	_hauteur = hauteur;
	_largeur = largeur;
	_glTexName = 0;
}

Icone::~Icone() {
	delete[] _pixels;
}

void Icone::initializeGraphicObject(void) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures(1, &_glTexName );
	glBindTexture(GL_TEXTURE_2D, _glTexName );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _largeur, _hauteur, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels);

	cout << endl << "Icone initialisee avec l'image '" << _nomFichier << "' sous la reference OpenGL " << _glTexName;
}

void Icone::destructGraphicObject(void) {
	// TODO D�truire la texture OpenGL
}

void Icone::affiche(float left, float right, float down, float up) {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glBindTexture(GL_TEXTURE_2D, _glTexName);
	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	if(_alpha) {
		glEnable(GL_BLEND);
	}

	glBegin( GL_QUADS );
		glTexCoord2f( 1.0f, 0.0f );
		glVertex2f(left, up);

		glTexCoord2f( 1.0f, 1.0f );
		glVertex2f(right, up);

		glTexCoord2f( 0.0f, 1.0f );
		glVertex2f(right, down);

		glTexCoord2f( 0.0f, 0.0f );
		glVertex2f(left, down);
	glEnd();
}

} /* namespace JktMoteur */
