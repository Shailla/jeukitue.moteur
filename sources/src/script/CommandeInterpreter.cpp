/*
 * CommandeInterpreter.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#include <string>
#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "main/Fabrique.h"
#include "menu/ConsoleView.h"
#include "script/commandes/Commande.h"

#include "script/commandes/DataCommande.h"
#include "script/commandes/EchoCommande.h"
#include "script/commandes/HelpCommande.h"

using namespace JktUtils;

#include "script/CommandeInterpreter.h"

CommandeInterpreter::CommandeInterpreter(Viewer* viewer) {
	_viewer = viewer;

	_commandes["data"] = new DataCommande(this);
	_commandes["echo"] = new EchoCommande(this);
	_commandes["help"] = new HelpCommande(this);
}

CommandeInterpreter::~CommandeInterpreter() {
}

Commande* CommandeInterpreter::getCommande(const string& commandeName) {
	return _commandes[commandeName];
}

void CommandeInterpreter::interpreteCommande(string ligne, bool userOutput) {
	if(userOutput) {
		ConsoleView* console = (ConsoleView*)_viewer->getView(Viewer::CONSOLE_VIEW);
		console->println(ConsoleView::COT_ECHO, ligne);
	}

	string commandeName = StringUtils::findAndEraseFirstWord(ligne);

	Commande* commande = getCommande(commandeName);

	if(commande != NULL) {
		commande->execute(ligne, userOutput);
	}
	else {
		printErrLn("Commande inconnue", userOutput);
	}
}

void CommandeInterpreter::printStdLn(const std::string& msg, bool userOutput) {
	if(userOutput) {
		ConsoleView* console = (ConsoleView*)_viewer->getView(Viewer::CONSOLE_VIEW);
		console->println(ConsoleView::COT_COMMAND_RESULT, msg);
	}
}

void CommandeInterpreter::printErrLn(const std::string& msg, bool userOutput) {
	if(userOutput) {
		ConsoleView* console = (ConsoleView*)_viewer->getView(Viewer::CONSOLE_VIEW);
		console->println(ConsoleView::COT_COMMAND_ERROR, msg);
	}
}

