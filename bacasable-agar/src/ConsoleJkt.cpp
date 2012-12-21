/*
 * ConsoleJkt.cpp
 *
 *  Created on: 13 déc. 2012
 *      Author: Erwin
 */

#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <agar/config/have_opengl.h>
#include <agar/config/have_sdl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/cursors.h>
#include <agar/gui/sdl.h>
#include <agar/gui/opengl.h>
#include <string>

char _commandToExecute[200];
AG_Label* _mapOuverteLabel;

void initWidgets(AG_Window* win) {

	AG_Notebook* book = AG_NotebookNew(win, 0);
	AG_Expand(book);

	/******************************
	 *	Onglet principal
	 ******************************/

	AG_NotebookTab* tabMain = AG_NotebookAddTab(book, "Principal", AG_BOX_VERT);
	AG_ConsoleNew(tabMain, AG_CONSOLE_EXPAND);

	AG_Box* box = AG_BoxNewHoriz(win, AG_BOX_HFILL);
	AG_TextboxNew(box, AG_TEXTBOX_STATIC|AG_TEXTBOX_HFILL, "Input: ");
	AG_ButtonNewFn(box, 0, "OK", NULL, "%p", NULL);

	/******************************
			Onglet des scores
	 ******************************/
	AG_NotebookTab* tabScores = AG_NotebookAddTab(book, "Scores", AG_BOX_VERT);
	AG_LabelNew(tabScores, 0, "Scores 1");
	AG_LabelNew(tabScores, 0, "Scores 2");
	AG_LabelNew(tabScores, 0, "Scores 3");


	/******************************
			Onglet des info
	 ******************************/
	AG_NotebookTab* tabInfo = AG_NotebookAddTab(book, "Info", AG_BOX_VERT);
	AG_Scrollview* scrollInfo = AG_ScrollviewNew(tabInfo, AG_SCROLLVIEW_EXPAND | AG_SCROLLVIEW_NOPAN_X);
	_mapOuverteLabel = AG_LabelNew(scrollInfo, 0, "Map ouverte : ");


	// Disposition de la fenêtre
	AG_WidgetUpdate(book);
	AG_NotebookSelectTab (book, tabMain);
}

