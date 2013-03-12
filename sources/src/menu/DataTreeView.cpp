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

	DataTreeDetails* details = &_serveurDetails;

	drawWidgets(details, box);

    AG_SeparatorNewHoriz(m_window);


	// Boutons
	AG_Box* boxButtons = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(boxButtons);

    // Bouton retour
	AG_ButtonNewFn(boxButtons, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);

    // Bouton rafraîchir
    AG_ButtonNewFn(boxButtons, 0, "Rafraichir", DataTreeView::refresh, "%p%p", m_window, details);

    // Bouton rafraîchir
    AG_ButtonNewFn(boxButtons, 0, "Ouvrir les fenetres clients", openClientsWindows, "%p", details);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_TC, 100, 50);
    hide();
}

DataTreeView::~DataTreeView(void) {
}

void DataTreeView::drawWidgets(DataTreeDetails* details, AG_Box* box) {
	AG_Box* boxes[2];
	boxes[0] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	boxes[1] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);

	// Liste des données
	details->_dataList = AG_TlistNew(boxes[0], AG_TLIST_EXPAND | AG_TLIST_TREE);
	AG_TlistSetChangedFn(details->_dataList, selectionChanged, "%p", details);

	// Tableau des clients
	details->_clientsTable = AG_TableNew(boxes[1], AG_TABLE_EXPAND);
	AG_TableAddCol(details->_clientsTable, "Nom", "<XXXXXXXXXXXXXXX>", NULL);
	AG_TableAddCol(details->_clientsTable, "Tmp ID", "<XXXXXXXXXXXXXXX>", NULL);
	AG_TableAddCol(details->_clientsTable, "Sent revision", "<XXXXXXXXXXXXXXX>", NULL);
	AG_Expand(details->_clientsTable);
}

void DataTreeView::selectionChanged(AG_Event* event) {
	DataTreeDetails* details = (DataTreeDetails*)AG_PTR(1);
	refreshClientTable(details);
}

void DataTreeView::openClientsWindows(AG_Event* event) {
	DataTreeDetails* details = (DataTreeDetails*)AG_PTR(1);

	AG_Window* window = AG_WindowNew(0);
	AG_WindowSetCaption(window, "Arbre de la MAP courante");

	AG_Box* box = AG_BoxNewHoriz(window, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	AG_Expand(box);

	drawWidgets(details, box);

    AG_SeparatorNewHoriz(window);


	// Boutons
	AG_Box* boxButtons = AG_BoxNewHoriz(window, 0);
	AG_ExpandHoriz(boxButtons);

    // Bouton rafraîchir
    AG_ButtonNewFn(boxButtons, 0, "Rafraichir", DataTreeView::refresh, "%p%p", window, details);

    AG_WindowSetGeometryAlignedPct(window, AG_WINDOW_TC, 100, 50);
	AG_WindowShow(window);
}

void DataTreeView::refreshClientTable(DataTreeDetails* details) {
	Donnee* selectedDonnee = (Donnee*)AG_TlistSelectedItemPtr(details->_dataList);


	/* **************************************
	 * Mise à jour du tableau des clients
	 * *************************************/

	AG_TableBegin(details->_clientsTable);

	vector<Client*> clients = serveurDataTree.getClients();
	vector<Client*>::iterator iter;

	for(iter = clients.begin() ; iter != clients.end() ; iter++) {
		Client* client = *iter;

		if(selectedDonnee) {
			MarqueurClient* marqueur = client->getMarqueur(selectedDonnee);

			if(marqueur) {
				AG_TableAddRow(details->_clientsTable, "%s:%d:%d", client->getDebugName().c_str(), marqueur->getTemporaryId(), marqueur->getSentRevision());
			}
			else {
				AG_TableAddRow(details->_clientsTable, "%s:%s:%s", client->getDebugName().c_str(), "<Pas de marqueur>", "<Bug>");
			}
		}
		else {
			AG_TableAddRow(details->_clientsTable, "%s:%s:%s", client->getDebugName().c_str(), "-", "-");
		}
	}

	AG_TableEnd(details->_clientsTable);
}

void DataTreeView::refresh(AG_Event* event) {
	AG_Window* window = (AG_Window*)AG_PTR(1);
	DataTreeDetails* details = (DataTreeDetails*)AG_PTR(2);

	refreshClientTable(details);
	refreshServeur(window, details);
}

void DataTreeView::refreshServeur(AG_Window* window, DataTreeDetails* details) {
	/* ************************************
	 * Mise à jour de l'abre des données
	 * ***********************************/

	AG_TlistBegin(details->_dataList);

	// Root
	Branche* root = &serveurDataTree.getRoot();
	AG_TlistItem* item = AG_TlistAddPtr(details->_dataList, NULL, "Root", root);
	int depth = 0;
	item->depth = depth;
	drawBranche(details, root, depth+1);

	AG_TlistEnd(details->_dataList);


	// Rafraichissement de la page
	AG_WindowUpdate(window);
}

void DataTreeView::show(void) {
	refreshClientTable(&_serveurDetails);
	refreshServeur(m_window, &_serveurDetails);

	View::show();
}

void DataTreeView::drawBranche(DataTreeDetails* details, Branche* branche, int depth) {
	// Valeurs filles
	{
		map<int, Valeur*>& valeurs = branche->getValeurs();
		map<int, Valeur*>::iterator itVa;

		for(itVa = valeurs.begin() ; itVa != valeurs.end() ; itVa++) {
			Valeur* valeur = itVa->second;

			ostringstream txt;

			if(dynamic_cast<ValeurInt*>(valeur) != 0) {
				ValeurInt* vl = (ValeurInt*)valeur;
				txt << "Valeur[" << vl->getValeurId() << ":" << vl->getValeurName() << "] : '" << vl->getValeur() << "'";
			}
			else {
				txt << "Valeur[" << valeur->getValeurId() << ":" << valeur->getValeurName() << "] : type inconnu";
			}

			AG_TlistItem* item = AG_TlistAddPtr(details->_dataList, NULL, txt.str().c_str(), valeur);
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

			AG_TlistItem* item = AG_TlistAddPtr(details->_dataList, NULL, tete.str().c_str(), subBranche);
			item->depth = depth;

			drawBranche(details, subBranche, depth+1);
		}
	}
}

