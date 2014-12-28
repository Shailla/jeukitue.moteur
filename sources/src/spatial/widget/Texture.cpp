/*
 * Texture.cpp
 *
 *  Created on: 15 oct. 2012
 *      Author: Erwin
 */

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"
#include "util/V3D.h"

using namespace std;

#include "util/Trace.h"
#include "spatial/widget/Texture.h"

namespace JktMoteur {

Texture::Texture(const string& nomFichier, bool alpha, int hauteur, int largeur, Uint8* pixels) {
	_nomFichier = nomFichier;
	_alpha = alpha;
	_pixels = pixels;
	_hauteur = hauteur;
	_largeur = largeur;
	_glTexName = 0;
}

Texture::~Texture() {
	delete[] _pixels;
}

void Texture::initializeGraphicObject(void) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures(1, &_glTexName );
	glBindTexture(GL_TEXTURE_2D, _glTexName );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _largeur, _hauteur, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pixels);

	TRACE().info("Texture initialisee avec l'image '%s' sous la référence OpenGL '%d'", _nomFichier.c_str(), _glTexName);
}

void Texture::destructGraphicObject(void) {
	// TODO Détruire la texture OpenGL
}

unsigned int Texture::getGlTexName(void) {
	return _glTexName;
};

void Texture::afficheQuad(const CV3D& a, const CV3D& b, const CV3D& c, const CV3D& d) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, _glTexName);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(a.X, a.Y, a.Z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(b.X, b.Y, b.Z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(c.X, c.Y, c.Z);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(d.X, d.Y, d.Z);
	glEnd();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

} /* namespace JktMoteur */
