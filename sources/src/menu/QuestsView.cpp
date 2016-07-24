/*
 * QuestView.cpp
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */
#include <sstream>
#include <iostream>

using namespace std;

#include "SDL.h"

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/QuestsView.h"

QuestsView::QuestsView(const AG_EventFn controllerCallback) : View(controllerCallback) {
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE|AG_WINDOW_MAXIMIZED);
	AG_WindowSetCaption(m_window, "Qu\u00e7te");
	AG_TextboxNew(m_window, AG_TEXTBOX_MULTILINE | AG_TEXTBOX_EXPAND,
			"Bonjour \u00e0 vous.\n"
			"\n"
			"vous \u00eates bien brave de venir ici.\n"
			"Peut-\u00eatre que...\n"
			"Certains maleurs se sont abattus sur cette contr\u00e9e\n"
			"et par les temps qui courrent on ne voit pas grand monde.");

	AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 70, 70);
}

QuestsView::~QuestsView() {
}

