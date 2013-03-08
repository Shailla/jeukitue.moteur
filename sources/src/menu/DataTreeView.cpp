
#include <sstream>
#include <iostream>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "data/Valeur.h"
#include "data/ValeurInt.h"

#include "menu/DataTreeView.h"

DataTreeView::DataTreeView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Arbre de la MAP courante");

	_tree = AG_TreetblNew(m_window, AG_TREETBL_EXPAND, NULL, NULL);

	// Bouton retour
    AG_SeparatorNewHoriz(m_window);

	AG_Button* _buttonRetour = AG_ButtonNewFn(m_window, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);
    AG_ExpandHoriz(_buttonRetour);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_TC, 100, 50);
    hide();
}

DataTreeView::~DataTreeView(void)
{
}

void DataTreeView::show(void) {
	AG_TreetblClearRows(_tree);

	// Arbre des données du serveur
	AG_TreetblAddCol(_tree, 0, NULL, "");

	int rowId = 0;

	// Root
	Branche& root = dataTree.getRoot();

	ostringstream tete;
	tete << "ROOT";
	AG_TreetblRow* rootRow = AG_TreetblAddRow(_tree, NULL, rowId++, "%d%s", 0, tete.str().c_str());
	AG_TreetblExpandRow (_tree, rootRow);

	drawBranche(&root, _tree, rootRow, rowId);

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);

	View::show();
}

string getValueString(Valeur* valeur) {
	ostringstream txt;

	if(dynamic_cast<ValeurInt*>(valeur) != 0) {
		ValeurInt* vl = (ValeurInt*)valeur;
		txt << "Valeur[" << vl->getValeurId() << ":" << vl->getValeurName() << "] : '" << vl->getInt() << "'";
	}
	else {
			txt << "Valeur[" << valeur->getValeurId() << ":" << valeur->getValeurName() << "] : type inconnu";
		}

	return txt.str();
}

void DataTreeView::drawBranche(Branche* branche, AG_Treetbl* tree, AG_TreetblRow* parentRow, int& rowId) {

	// Valeurs filles
	{
		map<int, Valeur*>& subBranches = branche->getValeurs();
		map<int, Valeur*>::iterator itVa;

		for(itVa = subBranches.begin() ; itVa != subBranches.end() ; itVa++) {
			Valeur* valeur = itVa->second;
			string txt = getValueString(valeur);
			AG_TreetblAddRow(tree, parentRow, rowId++, "%d%s", 0, txt.c_str());
		}
	}

	// Branches filles
	{
		map<int, Branche*> subBranches = branche->getSubBranches();
		map<int, Branche*>::iterator itBr;

		for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
			// Tête de la branche
			ostringstream tete;
			tete << "Branche[" << branche->getBrancheId() << ":" << branche->getBrancheName()  << "]";
			AG_TreetblRow* subRow = AG_TreetblAddRow(tree, parentRow, rowId++, "%d%s", 0, tete.str().c_str());
			drawBranche(itBr->second, _tree, subRow, rowId);
		}
	}

	AG_TreetblExpandRow (tree, parentRow);
}

