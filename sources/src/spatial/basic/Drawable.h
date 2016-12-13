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

	virtual void Affiche() = 0;									// Affiche l'objet géométrique
	virtual void AfficheSelection(float r,float v,float b) = 0;	// Affiche l'objet géométrique en couleur unique

	bool isSelected() const;												// Indique si l'objet est sélectionné
	void select(bool selected);												// Sélectionne l'objet (il sera affiché différemment dans sa Map)

	virtual void initGL() = 0;									// Initialisation de l'objet géométrique
	virtual void freeGL() = 0;									// Libération des ressources de l'objet dans le contexte OpenGL
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_DRAWABLE_H_ */
