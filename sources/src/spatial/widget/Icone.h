/*
 * Icone.h
 *
 *  Created on: 15 oct. 2012
 *      Author: Erwin
 */

#ifndef ICONE_H_
#define ICONE_H_

#include "SDL.h"

#include "spatial/widget/GraphicObject.h"

namespace JktMoteur {

class Icone : public GraphicObject {
	GLvoid* _pixels;
	GLsizei _hauteur;
	GLsizei _largeur;
	unsigned int _glTexName;
public:
	Icone(GLsizei hauteur, GLsizei largeur, GLvoid *pixels);
	virtual ~Icone();

	void initializeGraphicObject(void);
	void destructGraphicObject(void);
	void affiche(float left, float right, float down, float up);
};

} /* namespace JktMoteur */
#endif /* ICONE_H_ */
