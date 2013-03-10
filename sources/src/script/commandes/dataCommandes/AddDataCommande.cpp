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

#include "script/commandes/dataCommandes/AddDataCommande.h"

using namespace JktUtils;

extern DataTree dataTree;

AddDataCommande::AddDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void AddDataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "client") {
		string clientName = StringUtils::findAndEraseFirstWord(ligne);

		dataTree.addClient(clientName);
	}
	else if(subCommande1 == "branche") {
		string brancheName = StringUtils::findAndEraseFirstWord(ligne);
		vector<int> parentBrancheId = getIntParameters(ligne);

		try {
			dataTree.addBranche(parentBrancheId, brancheName);
		}
		catch(NotExistingBrancheException& exception) {
			printErrLn("La branche parent specifiee n'existe pas", userOutput);
		}
	}
	else if(subCommande1 == "valeur") {
		string valeurType = StringUtils::findAndEraseFirstWord(ligne);

		if(valeurType == "int") {
			// Nom de la valeur
			string valeurName = StringUtils::findAndEraseFirstWord(ligne);

			// Valeur de la valeur
			string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
			int valeur = getIntParameter(valeurStr);

			// Coordonnées de la branche sur laquelle la valeur doit être ajoutée
			vector<int> params = getIntParameters(ligne);

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

string AddDataCommande::getHelp() {
	return
"data add branche <brancheName> <brancheId> : Ajouter une branche nommee <brancheName> dans l'arbre de donnees sous la branche <brancheId>.\
\nExemple : data add branche ggg 0 3 2\
\n---\
\ndata add valeur <type> <valeurName> <valeur> <brancheId> : Ajouter une valeur nommee <valeurName> de type <type> de valeur <valeur> a la branche <brancheId>\
\n<type> = {int}\
\nExemple : data add valeur int 22 0 3 2";
}
