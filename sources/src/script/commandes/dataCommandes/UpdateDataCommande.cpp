/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "data/ValeurInt.h"
#include "data/DataTree.h"
#include "script/exception/IllegalParameterException.h"

#include "script/commandes/dataCommandes/UpdateDataCommande.h"

using namespace JktUtils;

extern DataTree dataTree;

UpdateDataCommande::UpdateDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void UpdateDataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "valeur") {
		// Nouvelle valeur de la valeur
		string valeurStr = StringUtils::findAndEraseFirstWord(ligne);

		// Coordonnées de la branche sur laquelle la valeur doit être ajoutée
		vector<int> valeurId = getIntParameters(ligne);

		Valeur* vl = dataTree.getValeur(valeurId);
		ValeurInt* vlInt = dynamic_cast<ValeurInt*>(vl);

		if(vlInt != 0) {
			int valeur = getIntParameter(valeurStr);
			vlInt->updateValeur(valeur);
		}
		else {
			printErrLn("Type de valeur non-pris en compte", userOutput);
		}
	}
	else {
		printErrLn("Syntaxe incorrecte", userOutput);
	}
}

string UpdateDataCommande::getHelp() {
	return
"data add branche <brancheName> <brancheId> : Ajouter une branche nommee <brancheName> dans l'arbre de donnees sous la branche <brancheId>.\
\nExemple : data add branche ggg 0 3 2\
\n---\
\ndata add valeur <type> <valeurName> <valeur> <brancheId> : Ajouter une valeur nommee <valeurName> de type <type> de valeur <valeur> a la branche <brancheId>\
\n<type> = {int}\
\nExemple : data add valeur int 22 0 3 2";
}
