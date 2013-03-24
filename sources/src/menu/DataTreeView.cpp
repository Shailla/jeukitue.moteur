#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/ClientDataTree.h"
#include "menu/DataTreeView.h"

DataTreeView::DataTreeView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Serveur");

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	AG_Expand(box);

	_serveurDetails = drawWidgets(box, &serveurDataTree);

    AG_SeparatorNewHoriz(m_window);


	// Boutons
	AG_Box* boxButtons = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(boxButtons);

    // Bouton retour
	AG_ButtonNewFn(boxButtons, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);

    // Bouton rafraîchir
    AG_ButtonNewFn(boxButtons, 0, "Rafraichir", DataTreeView::refresh, "%p%p", m_window, _serveurDetails);

    // Bouton rafraîchir
    AG_ButtonNewFn(boxButtons, 0, "Ouvrir les fenetres clients", openClientsWindows, "%p", this);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_TC, 100, 50);
    hide();
}

DataTreeView::~DataTreeView(void) {
}

DataTreeDetails* DataTreeView::drawWidgets(AG_Box* box, DataTree* tree) {
	AG_Box* boxes[2];
	boxes[0] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	boxes[1] = AG_BoxNewVert(box, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);

	// Liste des données
	AG_Tlist* dataList = AG_TlistNew(boxes[0], AG_TLIST_EXPAND | AG_TLIST_TREE);


	// Tableau des clients
	AG_Table* clientsTable = AG_TableNew(boxes[1], AG_TABLE_EXPAND);
	AG_TableAddCol(clientsTable, "Nom", "<XXXXXXXXXXXX>", NULL);
	AG_TableAddCol(clientsTable, "TmpId", "<XXXXXXXXXXXX>", NULL);
	AG_TableAddCol(clientsTable, "Sent rev.", "<XXXXXXXXXXXX>", NULL);
	AG_TableAddCol(clientsTable, "Conf. rev.", "<XXXXXXXXXXXX>", NULL);
	AG_Expand(clientsTable);

	DataTreeDetails* details = new DataTreeDetails(dataList, clientsTable, tree);
	AG_TlistSetChangedFn(dataList, selectionChanged, "%p", details);

	return details;
}

void DataTreeView::selectionChanged(AG_Event* event) {
	DataTreeDetails* details = (DataTreeDetails*)AG_PTR(1);
	refreshClientTable(details);
}

void DataTreeView::openClientsWindows(AG_Event* event) {
	DataTreeView* This = (DataTreeView*)AG_PTR(1);

	map<ClientDataTree*, Distant*>::iterator it;

	for(it = dataRouter.begin() ; it != dataRouter.end() ; it++) {
		ClientDataTree* tree = it->first;
		DataTreeDetails* details = NULL;

		try {
			details = This->_clientDataTrees.at(tree);
		}
		catch(out_of_range& exception) {
			details = NULL;
		}

		if(!details) {
			AG_Window* window = AG_WindowNew(0);
			AG_WindowSetCaption(window, tree->getClientName().c_str());

			AG_Box* box = AG_BoxNewHoriz(window, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
			AG_Expand(box);

			AG_SeparatorNewHoriz(window);

			details = drawWidgets(box, tree);
			This->_clientDataTrees[tree] = details;

			// Boutons
			AG_Box* boxButtons = AG_BoxNewHoriz(window, 0);
			AG_ExpandHoriz(boxButtons);

			// Bouton rafraîchir
			AG_ButtonNewFn(boxButtons, 0, "Rafraichir", DataTreeView::refresh, "%p%p", window, details);

			refreshClientTable(details);
			refreshServeur(window, details);

			AG_WindowSetGeometryAlignedPct(window, AG_WINDOW_TC, 100, 50);
			AG_WindowShow(window);
		}
	}
}

void DataTreeView::refreshClientTable(DataTreeDetails* details) {
	Donnee* selectedDonnee = (Donnee*)AG_TlistSelectedItemPtr(details->getDataList());


	/* **************************************
	 * Mise à jour du tableau des clients
	 * *************************************/

	AG_TableBegin(details->getClientsTable());

	vector<Distant*> distantsToShow;

	if(ServeurDataTree* serveur = dynamic_cast<ServeurDataTree*>(details->getDataTree())) {
		distantsToShow = serveur->getDistants();
	}
	else if(ClientDataTree* client = dynamic_cast<ClientDataTree*>(details->getDataTree())) {
		distantsToShow.push_back(client->getDistantServer());
	}

	vector<Distant*>::iterator iter;

	for(iter = distantsToShow.begin() ; iter != distantsToShow.end() ; iter++) {
		Distant* client = *iter;

		if(selectedDonnee) {
			MarqueurDistant* marqueur = client->getMarqueur(selectedDonnee);

			if(marqueur) {
				AG_TableAddRow(details->getClientsTable(), "%s:%d:%d:%d", client->getDebugName().c_str(), marqueur->getTemporaryId(), marqueur->getSentRevision(), marqueur->getConfirmedRevision());
			}
			else {
				AG_TableAddRow(details->getClientsTable(), "%s:%s:%s:%s", client->getDebugName().c_str(), "<Pas de marqueur>", "<Bug>", "<Bug>");
			}
		}
		else {
			cout << endl << "1:" << details->getClientsTable();
			cout << endl << "2:" << client->getDebugName().c_str();
			cout << flush;
			AG_TableAddRow(details->getClientsTable(), "%s:%s:%s:%s", client->getDebugName().c_str(), "-", "-", "-");
		}
	}

	AG_TableEnd(details->getClientsTable());
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

	AG_TlistBegin(details->getDataList());

	// Root
	Branche* root = &details->getDataTree()->getRoot();
	AG_TlistItem* item = AG_TlistAddPtr(details->getDataList(), NULL, "Root", root);
	int depth = 0;
	item->depth = depth;
	drawBranche(details, root, depth+1);

	AG_TlistEnd(details->getDataList());


	// Rafraichissement de la page
	AG_WindowUpdate(window);
}

void DataTreeView::show(void) {
	refreshClientTable(_serveurDetails);
	refreshServeur(m_window, _serveurDetails);

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

			if(ValeurInt* vl = dynamic_cast<ValeurInt*>(valeur)) {
				txt << "Valeur[" << vl->getValeurId() << ":" << vl->getValeurName() << "] : '" << vl->getValeur() << "'";
			}
			else {
				txt << "Valeur[" << valeur->getValeurId() << ":" << valeur->getValeurName() << "] : type inconnu";
			}

			AG_TlistItem* item = AG_TlistAddPtr(details->getDataList(), NULL, txt.str().c_str(), valeur);
			item->depth = depth+1;
		}
	}

	// Branches filles
	{
		map<int, Branche*>& subBranches = branche->getSubBranches();
		map<int, Branche*>::const_iterator itBr;

		for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
			Branche* subBranche = itBr->second;

			ostringstream tete;
			tete << "Branche[" << subBranche->getBrancheId() << ":" << subBranche->getBrancheName()  << "]";

			AG_TlistItem* item = AG_TlistAddPtr(details->getDataList(), NULL, tete.str().c_str(), subBranche);
			item->depth = depth;

			drawBranche(details, subBranche, depth+1);
		}
	}
}

