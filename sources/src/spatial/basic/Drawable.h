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

	virtual void affiche() = 0;										// Affiche l'objet g�om�trique
	virtual void afficheHighlighted(float r,float v,float b) = 0;	// Affiche l'objet g�om�trique en couleur unique

	bool isHidden() const;											// Indique si l'objet est cach�
	void hide(bool highlighted);									// Cache ou non l'objet

	bool isHighlighted() const;										// Indique si l'objet est s�lectionn�
	void highlight(bool highlighted);								// S�lectionne l'objet (il sera affich� diff�remment dans sa Map)

	virtual void initGL() = 0;										// Initialisation de l'objet g�om�trique
	virtual void freeGL() = 0;										// Lib�ration des ressources de l'objet dans le contexte OpenGL
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_DRAWABLE_H_ */
