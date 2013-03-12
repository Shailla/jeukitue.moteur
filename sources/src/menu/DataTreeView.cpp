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
	_serveurDataTree = AG_TlistNew(boxes[0], AG_TLIST_EXPAND | AG_TLIST_TREE);
	AG_TlistSetChangedFn(_serveurDataTree, selectionChanged, "%p", this);

	// Tableau des clients
	_serveurClientsTable = AG_TableNew(boxes[1], AG_TABLE_EXPAND);
	AG_TableAddCol(_serveurClientsTable, "Nom", "<XXXXXXXXXXXXXXX>", NULL);
	AG_TableAddCol(_serveurClientsTable, "Tmp ID", "<XXXXXXXXXXXXXXX>", NULL);
	AG_TableAddCol(_serveurClientsTable, "Sent revision", "<XXXXXXXXXXXXXXX>", NULL);
	AG_Expand(_serveurClientsTable);

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

DataTreeView::~DataTreeView(void) {
}

void DataTreeView::selectionChanged(AG_Event* event) {
	DataTreeView* This = (DataTreeView*)AG_PTR(1);
	refreshClientTable(This);
}

void DataTreeView::refreshClientTable(DataTreeView* This) {
	Donnee* selectedDonnee = (Donnee*)AG_TlistSelectedItemPtr(This->_serveurDataTree);


	/* **************************************
	 * Mise à jour du tableau des clients
	 * *************************************/

	AG_TableBegin(This->_serveurClientsTable);

	vector<Client*> clients = serveurDataTree.getClients();
	vector<Client*>::iterator iter;

	for(iter = clients.begin() ; iter != clients.end() ; iter++) {
		Client* client = *iter;

		if(selectedDonnee) {
			MarqueurClient* marqueur = client->getMarqueur(selectedDonnee);

			if(marqueur) {
				AG_TableAddRow(This->_serveurClientsTable, "%s:%d:%d", client->getDebugName().c_str(), marqueur->getTemporaryId(), marqueur->getSentRevision());
			}
			else {
				AG_TableAddRow(This->_serveurClientsTable, "%s:%s:%s", client->getDebugName().c_str(), "<Pas de marqueur>", "<Bug>");
			}
		}
		else {
			AG_TableAddRow(This->_serveurClientsTable, "%s:%s:%s", client->getDebugName().c_str(), "-", "-");
		}
	}

	AG_TableEnd(This->_serveurClientsTable);
}

void DataTreeView::refresh(void) {
	refreshClientTable(this);

	/* ************************************
	 * Mise à jour de l'abre des données
	 * ***********************************/

	AG_TlistBegin(_serveurDataTree);

	// Root
	Branche* root = &serveurDataTree.getRoot();
	AG_TlistItem* item = AG_TlistAddPtr(_serveurDataTree, NULL, "Root", root);
	int depth = 0;
	item->depth = depth;
	drawBranche(root, depth+1);

	AG_TlistEnd(_serveurDataTree);


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
		txt << "Valeur[" << vl->getValeurId() << ":" << vl->getValeurName() << "] : '" << vl->getValeur() << "'";
	}
	else {
		txt << "Valeur[" << valeur->getValeurId() << ":" << valeur->getValeurName() << "] : type inconnu";
	}

	return txt.str();
}

void DataTreeView::drawBranche(Branche* branche, int depth) {
	// Valeurs filles
	{
		map<int, Valeur*>& valeurs = branche->getValeurs();
		map<int, Valeur*>::iterator itVa;

		for(itVa = valeurs.begin() ; itVa != valeurs.end() ; itVa++) {
			Valeur* valeur = itVa->second;
			string txt = getValueString(valeur);

			AG_TlistItem* item = AG_TlistAddPtr(_serveurDataTree, NULL, txt.c_str(), valeur);
			item->depth = depth+1;
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

			AG_TlistItem* item = AG_TlistAddPtr(_serveurDataTree, NULL, tete.str().c_str(), subBranche);
			item->depth = depth;

			drawBranche(subBranche, depth+1);
		}
	}
}

