/*
 * CommandeInterpreter.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#include <vector>
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

using namespace jkt;

#include "script/CommandeInterpreter.h"

CommandeInterpreter::CommandeInterpreter(Viewer* viewer) {
	_viewer = viewer;

	_commandes["data"] = new DataCommande(this);
	_commandes["echo"] = new EchoCommande(this);
	_commandes["help"] = new HelpCommande(this);
}

CommandeInterpreter::~CommandeInterpreter() {
}

const map<string, Commande*>& CommandeInterpreter::getCommandes() const {
	return _commandes;
}

Commande* CommandeInterpreter::getCommande(const string& commandeName) {
	std::map<std::string, Commande*>::iterator it = _commandes.find(commandeName);
	return (it==_commandes.end())?NULL:it->second;
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

		vector<string> lines = StringUtils::splitByCarriageReturns(msg);
		vector<string>::iterator it;

		for(it = lines.begin() ; it != lines.end() ; it++) {
			console->println(ConsoleView::COT_COMMAND_RESULT, *it);
		}
	}
}

void CommandeInterpreter::printErrLn(const std::string& msg, bool userOutput) {
	if(userOutput) {
		ConsoleView* console = (ConsoleView*)_viewer->getView(Viewer::CONSOLE_VIEW);

		vector<string> lines = StringUtils::splitByCarriageReturns(msg);
		vector<string>::iterator it;

		for(it = lines.begin() ; it != lines.end() ; it++) {
			console->println(ConsoleView::COT_ERROR, *it);
		}
	}
}

