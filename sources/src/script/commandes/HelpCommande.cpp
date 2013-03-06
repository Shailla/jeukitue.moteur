/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

using namespace std;

#include "util/StringUtils.h"
#include "script/CommandeInterpreter.h"

#include "script/commandes/HelpCommande.h"

HelpCommande::HelpCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void HelpCommande::executeIt(std::string ligne, bool userOutput) {
	JktUtils::StringUtils::trim(ligne);
	Commande* commande = _interpreter->getCommande(ligne);

	if(commande == NULL) {
		printErrLn("Cette commande n'existe pas", userOutput);
	}
	else {
		printStdLn(commande->getHelp(), userOutput);
	}
}

string HelpCommande::getHelp() {
	return "help <commande>\nAffiche l'aide concernant la commande <commande>";
}
