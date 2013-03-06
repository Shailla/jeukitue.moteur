/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

using namespace std;

#include "util/StringUtils.h"
#include "data/DataTree.h"
#include "script/exception/IllegalParameterException.h"

#include "script/commandes/DataCommande.h"

using namespace JktUtils;

extern DataTree dataTree;

DataCommande::DataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void DataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "add") {
		string subCommande2 = StringUtils::findAndEraseFirstWord(ligne);

		if(subCommande2 == "branche") {
			vector<int> parentBrancheId = getIntParameters(ligne);
			dataTree.addBranche(parentBrancheId);
		}
		else {
			printErrLn("Syntaxe incorrecte", userOutput);
		}
	}
	else {
		printErrLn("Syntaxe incorrecte", userOutput);
	}
}

string DataCommande::getHelp() {
	return "TODO";
}
