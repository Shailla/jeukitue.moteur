
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


	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	AG_Expand(box);

	AG_Box* boxes[2];
	boxes[0] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	boxes[1] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);

	// Arbre des données
	_dataTree = AG_TreetblNew(boxes[0], AG_TREETBL_EXPAND  | AG_TREETBL_POLLED, NULL, NULL);

	// Tableau des clients
	_clientsTable = AG_TableNew(boxes[1], AG_TABLE_EXPAND);
	AG_TableAddCol(_clientsTable, "Nom", "<xxxxxxxxxxxxxxx>", NULL);
	AG_TableAddCol(_clientsTable, "Tmp ID", "<xxxxxxxxxxxxxxx>", NULL);
	AG_TableAddCol(_clientsTable, "Uptodate", "<xxxxxxxxxxxxxxx>", NULL);
	AG_Expand(_clientsTable);

    AG_SeparatorNewHoriz(m_window);


	// Boutons
	AG_Box* boxButtons = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(boxButtons);

    // Bouton retour
	AG_ButtonNewFn(boxButtons, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);

    // Bouton rafraîchir
    AG_ButtonNewFn(boxButtons, 0, "Rafraichir", controllerCallback, "%i%p", Controller::Refresh, this);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_TC, 100, 50);
    hide();
}

DataTreeView::~DataTreeView(void)
{
}

void DataTreeView::refresh(void) {
	AG_TreetblRow* selectedTreeRow = AG_TreetblSelectedRow(_dataTree);
	Branche* selectedDonnee = NULL;

	if(selectedTreeRow) {
		selectedDonnee = _treeRows.at(selectedTreeRow);
	}


	/* **************************************
	 * Mise à jour du tableau des clients
	 * *************************************/

	AG_TableBegin(_clientsTable);

	vector<Client*> clients = dataTree.getClients();
	vector<Client*>::iterator iter;

	for(iter = clients.begin() ; iter != clients.end() ; iter++) {
		Client* client = *iter;

		if(selectedDonnee) {
			MarqueurClient* marqueur = client->getMarqueur(selectedDonnee);

			if(marqueur) {
				AG_TableAddRow(_clientsTable, "%s:%d:%b", client->getDebugName().c_str(), marqueur->getTemporaryId(), marqueur->isUpToDate());
			}
			else {
				AG_TableAddRow(_clientsTable, "%s:%s:%s", client->getDebugName().c_str(), "<Pas de marqueur>", "<Bug>");
			}
		}
		else {
			AG_TableAddRow(_clientsTable, "%s:%s:%s", client->getDebugName().c_str(), "-", "-");
		}
	}

	AG_TableEnd(_clientsTable);


	/* ************************************
	 * Mise à jour de l'abre des données
	 * ***********************************/

	AG_TreetblBegin(_dataTree);
	_treeRows.clear();

	// Arbre des données du serveur
	AG_TreetblAddCol(_dataTree, 0, NULL, "");

	int rowId = 0;

	// Root
	Branche& root = dataTree.getRoot();

	ostringstream tete;
	tete << "ROOT";
	AG_TreetblRow* rootRow = AG_TreetblAddRow(_dataTree, NULL, rowId++, "%d%s", 0, tete.str().c_str());
	_treeRows[rootRow] = &root;
	AG_TreetblExpandRow (_dataTree, rootRow);

	drawBranche(&root, _dataTree, rootRow, rowId);

	AG_TreetblEnd(_dataTree);


	// Rafraichissement de la page
	AG_WindowUpdate(m_window);
}

void DataTreeView::show(void) {
	refresh();
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
		map<int, Valeur*>& valeurs = branche->getValeurs();
		map<int, Valeur*>::iterator itVa;

		for(itVa = valeurs.begin() ; itVa != valeurs.end() ; itVa++) {
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
			Branche* subBranche = itBr->second;

			ostringstream tete;
			tete << "Branche[" << subBranche->getBrancheId() << ":" << subBranche->getBrancheName()  << "]";
			AG_TreetblRow* subRow = AG_TreetblAddRow(tree, parentRow, rowId++, "%d%s", 0, tete.str().c_str());
			_treeRows[subRow] = subBranche;
			drawBranche(subBranche, tree, subRow, rowId);
		}
	}

	AG_TreetblExpandRow (tree, parentRow);
}

