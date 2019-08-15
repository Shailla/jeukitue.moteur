/*
 * DataCommande.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

#include "util/StringUtils.h"
#include "data/ServeurDataTree.h"
#include "script/exception/IllegalParameterException.h"

#include "script/commandes/DataCommande.h"
#include "script/commandes/dataCommandes/AddDataCommande.h"
#include "script/commandes/dataCommandes/UpdateDataCommande.h"

using namespace std;
using namespace jkt;

DataCommande::DataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
	registerSubCommande("add", new AddDataCommande(interpreter));
	registerSubCommande("update", new UpdateDataCommande(interpreter));
}

void DataCommande::executeIt(std::string ligne, bool userOutput) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	Commande* subCommande = getSubCommande(subCommande1);	// Throws IllegalParameterException if the sub command is not found

	try {
		subCommande->executeIt(ligne, userOutput);
	}
	catch(NotExistingValeurException& exception) {
		printErrLn("La branche n'existe pas", userOutput);
	}
	catch(NotExistingBrancheException& exception) {
		printErrLn("La valeur n'existe pas", userOutput);
	}
}

string DataCommande::getShortHelp() const {
	return "Manipule les arbres de donn�es";
}

string DataCommande::getHelp() const {
	return
"data add branche <brancheName> <brancheId> : Ajouter une branche nommee <brancheName> dans l'arbre de donnees sous la branche <brancheId>.\
\nExemple : data add branche ggg 0 3 2\
\n---\
\ndata add valeur <type> <valeurName> <valeur> <brancheId> : Ajouter une valeur nommee <valeurName> de type <type> de valeur <valeur> a la branche <brancheId>\
\n<type> = {int}\
\nExemple : data add valeur int 22 0 3 2";
}
