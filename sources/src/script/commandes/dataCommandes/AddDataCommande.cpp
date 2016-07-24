/*
 * AddDataCommande.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "data/DataTree.h"
#include "data/DistantTreeProxy.h"
#include "script/exception/IllegalParameterException.h"
#include "main/Game.h"

#include "script/commandes/dataCommandes/AddDataCommande.h"

using namespace jkt;

extern CGame Game;

AddDataCommande::AddDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void AddDataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "branche") {
		DataTree* dataTree = Game.getDataTree();

		if(!dataTree) {
			printErrLn("Client et serveur tous deux inactifs", userOutput);
		}
		else {
			string brancheName = StringUtils::findAndEraseFirstWord(ligne);
			vector<int> parentBrancheId = getIntParameters(ligne);

			try {
				dataTree->createBranche(0, parentBrancheId, brancheName);
				printStdLn("Branche creee.", userOutput);
			}
			catch(NotExistingBrancheException& exception) {
				printErrLn("La branche parent specifiee n'existe pas", userOutput);
			}
		}
	}
	else if(subCommande1 == "valeur") {
		DataTree* dataTree = Game.getDataTree();

		if(!dataTree) {
			printErrLn("Client et serveur tous deux inactifs", userOutput);
		}
		else {
			string valeurType = StringUtils::findAndEraseFirstWord(ligne);

			// Nom de la valeur
			string valeurName = StringUtils::findAndEraseFirstWord(ligne);

			AnyData valeur;
			bool valued = true;

			if(valeurType == "int") {
				// Valeur de la valeur
				string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
				valeur.update(getIntParameter(valeurStr));
			}
			else if(valeurType == "float") {
				// Valeur de la valeur
				string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
				valeur.update(getFloatParameter(valeurStr));
			}
			else if(valeurType == "string") {
				// Valeur de la valeur
				cout << endl << "LIGNE'" << ligne << "'";
				const string& valeurStr = StringUtils::findAndEraseFirstString(ligne);
				valeur.update(valeurStr);
				cout << endl << "LIGNE'" << ligne << "'";
				cout << endl << "RES'" << valeurStr << "'";
			}
			else {
				valued = false;
			}

			if(valued) {
				// Coordonn�es de la branche sur laquelle la valeur doit �tre ajout�e
				vector<int> brancheId = getIntParameters(ligne);

				dataTree->createValeur(0, ANY, brancheId, valeurName, valeur);

				printStdLn("Valeur creee.", userOutput);
			}
			else {
				printErrLn("Syntaxe incorrecte (type de donnee inconnu)", userOutput);
			}
		}
	}
	else {
		printErrLn("Syntaxe incorrecte", userOutput);
	}
}

string AddDataCommande::getShortHelp() const {
	return "Ajoute un donn�e sur un arbre";
}

string AddDataCommande::getHelp() const {
	return
"data add branche server <brancheName> : Ajoute aux donnees du serveur une branche nommee <brancheName> dans l'arbre de donnees sous la branche <brancheId>.\
\nExemple : data add branche server ggg 0 3 2\
\n---\
\ndata add branche client <clientName> <brancheName> : Ajoute aux donnees du client <clientName> une branche nommee <brancheName> sous la branche <brancheId>.\
\nExemple : data add branche client Erwin ggg 0 3 2\
\n---\
\ndata add valeur server <type> <valeurName> <valeur> <brancheId> : Ajoute aux donnees du serveur une valeur nommee <valeurName> de type <type> et de valeur <valeur> a la branche <brancheId>\
\n<type> = {int}\
\nExemple : data add valeur server int 22 0 3 2\
\ndata add valeur client <clientName> <type> <valeurName> <valeur> <brancheId> : Ajoute aux donnees du client <clientName> une valeur nommee <valeurName> de type <type> et de valeur <valeur> a la branche <brancheId>\
\n<type> = {int}\
\nExemple : data add valeur client Erwin int 22 0 3 2";
}
