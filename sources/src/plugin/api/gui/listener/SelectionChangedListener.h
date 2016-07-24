/*
 * SelectionChangedListener.h
 *
 *  Created on: 19 mai 2012
 *      Author: Erwin
 */

#ifndef SELECTIONCHANGEDLISTENER_H_
#define SELECTIONCHANGEDLISTENER_H_

namespace jkt {

class SelectionChangedListener {
public:
	virtual ~SelectionChangedListener() {};

	virtual void selectionChangedEvent() = 0;
};

} /* namespace jkt */
#endif /* SELECTIONCHANGEDLISTENER_H_ */
