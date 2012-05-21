/*
 * ButtonPressedListener.h
 *
 *  Created on: 19 mai 2012
 *      Author: Erwin
 */

#ifndef BUTTONPRESSEDLISTENER_H_
#define BUTTONPRESSEDLISTENER_H_

namespace JktPlugin {

class ButtonPressedListener {
public:
	virtual ~ButtonPressedListener() {};

	virtual void buttonPressedEvent() = 0;
};

} /* namespace JktPlugin */
#endif /* BUTTONPRESSEDLISTENER_H_ */
