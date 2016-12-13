/*
 * Drawable.h
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_SPATIAL_BASIC_DRAWABLE_H_
#define SRC_SPATIAL_BASIC_DRAWABLE_H_

#include "spatial/basic/Object.h"

namespace jkt {

class Drawable {
	bool _selected;
public:
	Drawable();
	Drawable(const Drawable& other);
	virtual ~Drawable();

	virtual void Affiche() = 0;									// Affiche l'objet g�om�trique
	virtual void AfficheSelection(float r,float v,float b) = 0;	// Affiche l'objet g�om�trique en couleur unique

	bool isSelected() const;												// Indique si l'objet est s�lectionn�
	void select(bool selected);												// S�lectionne l'objet (il sera affich� diff�remment dans sa Map)

	virtual void initGL() = 0;									// Initialisation de l'objet g�om�trique
	virtual void freeGL() = 0;									// Lib�ration des ressources de l'objet dans le contexte OpenGL
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_DRAWABLE_H_ */
