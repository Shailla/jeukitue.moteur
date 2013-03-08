/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

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
			string brancheName = StringUtils::findAndEraseFirstWord(ligne);
			vector<int> parentBrancheId = getIntParameters(ligne);

			try {
				dataTree.addBranche(parentBrancheId, brancheName);
			}
			catch(NotExistingBrancheException& exception) {
				printErrLn("La branche parent specifiee n'existe pas", userOutput);
			}
		}
		else if(subCommande2 == "valeur") {
			string subCommande3 = StringUtils::findAndEraseFirstWord(ligne);

			if(subCommande3 == "int") {
				string valeurName = StringUtils::findAndEraseFirstWord(ligne);
				vector<int> params = getIntParameters(ligne);

				int valeur = *params.begin();
				params.erase(params.begin());

				dataTree.addValeurInt(params, valeurName, valeur);
			}
			else {
				printErrLn("Syntaxe incorrecte", userOutput);
			}
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
	return "data add branche <brancheId> : Ajouter une branche dans l'arbre de donnees sous la branche <brancheId>.\nExemple : data add branche 0 3 2";
}
