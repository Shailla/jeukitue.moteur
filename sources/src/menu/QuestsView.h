/*
 * QuestView.h
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_MENU_QUESTSVIEW_H_
#define SRC_MENU_QUESTSVIEW_H_

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class QuestsView : public View {
public:
	QuestsView(const AG_EventFn controllerCallback);
	virtual ~QuestsView();
};

#endif /* SRC_MENU_QUESTSVIEW_H_ */
