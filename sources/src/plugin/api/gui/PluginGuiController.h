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

namespace JktPlugin {

class PluginGuiController {
public:
	PluginGuiController();
	virtual ~PluginGuiController();

	static void catchButtonEvents(AG_Event* event);
};

} /* namespace JktPlugin */
#endif /* PLUGINGUICONTROLLER_H_ */
