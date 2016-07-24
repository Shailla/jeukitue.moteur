/*
 * PluginGuiController.h
 *
 *  Created on: 19 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINGUICONTROLLER_H_
#define PLUGINGUICONTROLLER_H_

#include <agar/core.h>
#include <agar/gui.h>

namespace jkt {

class PluginGuiController {
public:
	PluginGuiController();
	virtual ~PluginGuiController();

	static void catchButtonEvent(AG_Event* event);
	static void catchComboListEvent(AG_Event* event);
};

} /* namespace jkt */
#endif /* PLUGINGUICONTROLLER_H_ */
