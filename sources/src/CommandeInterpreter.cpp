/*
 * CommandeInterpreter.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#include <string>

using namespace std;

#include "util/StringUtils.h"
#include "main/Fabrique.h"
#include "Commande.h"
#include "menu/ConsoleView.h"

using namespace JktUtils;

#include "CommandeInterpreter.h"

CommandeInterpreter::CommandeInterpreter() {
}

CommandeInterpreter::~CommandeInterpreter() {
}

void CommandeInterpreter::interpreteCommande(string ligne, bool userOutput) {
	string commandeStr = StringUtils::findFirstWord(ligne);

	Commande* commande = _commandes[commandeStr];

	if(commande != NULL) {
		commande.execute(ligne, userOutput);
	}
	else {
		if(userOutput) {
			Fabrique.
			ConsoleView* view = (ConsoleView*)m_agarView->getView(Viewer::CONSOLE_VIEW);
			view->println("Commande inconnue");
		}
	}
}
