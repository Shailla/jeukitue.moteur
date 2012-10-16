/*
 * GraphicObject.h
 *
 *  Created on: 15 oct. 2012
 *      Author: Erwin
 */

#ifndef GRAPHICOBJECT_H_
#define GRAPHICOBJECT_H_

namespace JktMoteur {

class GraphicObject {
public:
	GraphicObject();
	virtual ~GraphicObject();

	virtual void initializeGraphicObject(void) = 0;
	virtual void destructGraphicObject(void) = 0;
};

} /* namespace JktMoteur */
#endif /* GRAPHICOBJECT_H_ */
