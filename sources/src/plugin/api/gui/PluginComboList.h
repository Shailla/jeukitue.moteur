/*
 * PluginComboList.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCOMBOLIST_H_
#define PLUGINCOMBOLIST_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/api/gui/listener/SelectionChangedListener.h"
#include "plugin/PluginContext.h"

namespace jkt {

class PluginComboList {
	SelectionChangedListener* _selectionChangedListener;
	AG_Combo* _combo;
public:
	PluginComboList(AG_Widget* parent, const std::string& title, SelectionChangedListener* selectionChangedListener);
	virtual ~PluginComboList();

	/** Ajout un item associé à un identifiant "userId" au choix de l'utilisateur */
	void addItem(const std::string& itemName, int userId);

	/** Définit la taille de la combo list à partir d'un masque de caractères */
	void setComboSize(const std::string& mask);

	void selectionChangedEvent(AG_Event* event);

	void selectItemByUserId(int userId);
	int getSelectedItemUserId();

	void expand();
	void expandHoriz();
	void expandVert();
};

} /* namespace jkt */
#endif /* PLUGINCOMBOLIST_H_ */
