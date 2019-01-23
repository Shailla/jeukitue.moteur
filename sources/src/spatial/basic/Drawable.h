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
	bool _hidden;
	bool _highlighted;
public:
	Drawable();
	Drawable(const Drawable& other);
	virtual ~Drawable();

	virtual void affiche() = 0;										// Affiche l'objet géométrique
	virtual void afficheHighlighted(float r, float v, float b) = 0;	// Affiche l'objet géométrique en couleur unique

	bool isHidden() const;											// Indique si l'objet est caché
	void hide(bool highlighted);									// Cache ou non l'objet

	bool isHighlighted() const;										// Indique si l'objet est sélectionné
	void highlight(bool highlighted);								// Sélectionne l'objet (il sera affiché différemment dans sa Map)

	virtual void initGL() = 0;										// Initialisation de l'objet géométrique
	virtual void freeGL() = 0;										// Libération des ressources de l'objet dans le contexte OpenGL
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_DRAWABLE_H_ */
