#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/ClientDataTree.h"
#include "data/MarqueurDistant.h"
#include "reseau/new/Interlocutor2.h"
#include "main/Game.h"

#include "menu/DataTreeView.h"

extern CGame Game;

DataTreeView::DataTreeView(const AG_EventFn controllerCallback)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Arbre des donnees - null");

	AG_Box* box = AG_BoxNewHoriz(m_window, AG_BOX_HOMOGENOUS | AG_BOX_EXPAND);
	AG_Expand(box);

	drawWidgets(m_window, box);

    AG_SeparatorNewHoriz(m_window);

	// Boutons
	AG_Box* boxButtons = AG_BoxNewHoriz(m_window, 0);
	AG_ExpandHoriz(boxButtons);

    // Bouton retour
	AG_ButtonNewFn(boxButtons, 0, "Retour", controllerCallback, "%i", Controller::ShowDebugViewAction);

    // Bouton rafraîchir
    AG_ButtonNewFn(boxButtons, 0, "Rafraichir", DataTreeView::refresh, "%p", this);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_TC, 100, 50);
    hide();
}

DataTreeView::~DataTreeView(void) {
}

void DataTreeView::drawWidgets(AG_Window* window, AG_Box* box) {
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

	_dataList = dataList;
	_clientsTable = clientsTable;
}

void DataTreeView::refresh(AG_Event* event) {
	DataTreeView* thiz = (DataTreeView*)AG_PTR(1);
	thiz->refreshTree();
}

void DataTreeView::refreshTree() {
	/* ************************************
	 * Mise à jour de l'abre des données
	 * ***********************************/
	LocalDataTree* localDataTree = Game.getLocalDataTree();
	ServeurDataTree* serverDataTree = Game.getServerDataTree();
	ClientDataTree* clientDataTree = Game.getClientDataTree();

	if(localDataTree) {
		_dataTree = localDataTree;
		AG_WindowSetCaption(m_window, "Arbre des donnees - local");
	}
	else if(serverDataTree) {
		_dataTree = serverDataTree;
		AG_WindowSetCaption(m_window, "Arbre des donnees - serveur");
	}
	else if(clientDataTree) {
		_dataTree = clientDataTree;
		AG_WindowSetCaption(m_window, "Arbre des donnees - client");
	}
	else {
		_dataTree = 0;
		AG_WindowSetCaption(m_window, "Arbre des donnees - null");
	}

	AG_TlistBegin(_dataList);

	// Root
	if(_dataTree) {
		Branche* root = &_dataTree->getRoot();
		AG_TlistItem* item = AG_TlistAddPtr(_dataList, NULL, "Root", root);
		int depth = 0;
		item->depth = depth;
		drawBranche(this, root, depth+1);
	}

	AG_TlistEnd(_dataList);

	// Rafraichissement de la page
	AG_WindowUpdate(m_window);
}

void DataTreeView::show(void) {
	refreshTree();
	View::show();
}

void DataTreeView::drawBranche(DataTreeView* thiz, AbstractBranche* branche, int depth) {
	Branche* br = dynamic_cast<Branche*>(branche);

	if(br) {
		// Valeurs filles
		{
			vector<Valeur*>& valeurs = br->getValeurs();
			vector<Valeur*>::iterator itVa;

			for(itVa = valeurs.begin() ; itVa != valeurs.end() ; itVa++) {
				Valeur* valeur = *itVa;

				ostringstream txt;

				txt << "Valeur[" << valeur->getValeurId() << " r" << valeur->getRevision() << " '" << valeur->getValeurName() << "'] : '" << valeur->toString() << "'";

				AG_TlistItem* item = AG_TlistAddPtr(thiz->_dataList, NULL, txt.str().c_str(), valeur);
				item->depth = depth+1;
			}
		}

		// Branches filles
		{
			vector<Branche*>& subBranches = br->getSubBranches();
			vector<Branche*>::const_iterator itBr;

			for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
				Branche* subBranche = *itBr;

				ostringstream tete;
				tete << "Branche[" << subBranche->getBrancheId() << " r" << subBranche->getRevision() << " '" << subBranche->getBrancheName()  << "']";

				AG_TlistItem* item = AG_TlistAddPtr(thiz->_dataList, NULL, tete.str().c_str(), subBranche);
				item->depth = depth;

				drawBranche(thiz, subBranche, depth+1);
			}
		}
	}
}

