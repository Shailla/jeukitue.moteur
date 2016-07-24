/*
 * GraphicObject.h
 *
 *  Created on: 15 oct. 2012
 *      Author: Erwin
 */

#ifndef GRAPHICOBJECT_H_
#define GRAPHICOBJECT_H_

namespace jkt {

class GraphicObject {
public:
	GraphicObject();
	virtual ~GraphicObject();

	/** Alloue les ressources OpenGL de l'objet graphique. */
	virtual void initializeGraphicObject(void) = 0;

	/** Lib�re les ressources OpenGL de l'objet graphique. */
	virtual void destructGraphicObject(void) = 0;
};

} /* namespace jkt */
#endif /* GRAPHICOBJECT_H_ */
