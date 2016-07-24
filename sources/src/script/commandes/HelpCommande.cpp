/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <sstream>

using namespace std;

#include "util/StringUtils.h"
#include "script/CommandeInterpreter.h"

#include "script/commandes/HelpCommande.h"

HelpCommande::HelpCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void HelpCommande::executeIt(std::string ligne, bool userOutput) {
	jkt::StringUtils::trim(ligne);

	if(ligne.size() == 0) {
		ostringstream ss;
		ss << "Liste des commandes :";

		map<string, Commande*>::const_iterator cIt;
		const map<string, Commande*>& commandes = _interpreter->getCommandes();

		for(cIt = commandes.begin() ; cIt != commandes.end() ; cIt++) {
			ss << endl << "  " << cIt->first << " : " << cIt->second->getShortHelp();
		}

		printStdLn(ss.str(), userOutput);
	}
	else {
		Commande* commande = _interpreter->getCommande(ligne);

		if(commande == NULL) {
			printErrLn("Cette commande n'existe pas", userOutput);
		}
		else {
			printStdLn(commande->getHelp(), userOutput);
		}
	}
}

std::string HelpCommande::getShortHelp() const {
	return "Affiche l'aide";
}

std::string HelpCommande::getHelp() const {
	return "help <commande>\nAffiche l'aide concernant la commande <commande>";
}
