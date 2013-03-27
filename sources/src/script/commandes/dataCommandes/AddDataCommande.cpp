/*
 * AddDataCommande.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "data/ServeurDataTree.h"
#include "data/Distant.h"
#include "data/ClientDataTree.h"
#include "script/exception/IllegalParameterException.h"

#include "script/commandes/dataCommandes/AddDataCommande.h"

using namespace JktUtils;

extern ServeurDataTree serveurDataTree;
extern std::map<ClientDataTree*, Distant*> dataRouter;

AddDataCommande::AddDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void AddDataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "client") {
		string clientName = StringUtils::findAndEraseFirstWord(ligne);

		Distant* client = serveurDataTree.addDistant(clientName);
		ClientDataTree* clientDataTree = new ClientDataTree(new Distant("serveur"), clientName);
		dataRouter[clientDataTree] = client;
		printStdLn("Client cree.", userOutput);
	}
	else if(subCommande1 == "branche") {
		string serverOrClientMode = StringUtils::findAndEraseFirstWord(ligne);

		DataTree* tree = NULL;

		if(serverOrClientMode == "server") {
			tree = &serveurDataTree;
		}
		else if(serverOrClientMode == "client") {
			string clientName = StringUtils::findAndEraseFirstWord(ligne);

			// Recherche du client
			map<ClientDataTree*, Distant*>::iterator it;
			ClientDataTree* clientTree;

			for(it = dataRouter.begin() ; (it != dataRouter.end() && !tree) ; it++) {
				clientTree = it->first;

				if(clientTree->getClientName() == clientName) {
					tree = clientTree;
				}
			}
		}
		else {
			printErrLn("Syntaxe incorrecte", userOutput);
		}

		if(tree) {
			string brancheName = StringUtils::findAndEraseFirstWord(ligne);
			vector<int> parentBrancheId = getIntParameters(ligne);

			try {
				tree->createBranche(parentBrancheId, brancheName, 0);
				printStdLn("Branche creee.", userOutput);
			}
			catch(NotExistingBrancheException& exception) {
				printErrLn("La branche parent specifiee n'existe pas", userOutput);
			}
		}
		else {
			printErrLn("Syntaxe incorrecte (arbre inconnu)", userOutput);
		}
	}
	else if(subCommande1 == "valeur") {
		string serverOrClientMode = StringUtils::findAndEraseFirstWord(ligne);

		DataTree* tree = NULL;

		if(serverOrClientMode == "server") {
			tree = &serveurDataTree;
		}
		else if(serverOrClientMode == "client") {
			string clientName = StringUtils::findAndEraseFirstWord(ligne);

			// Recherche du client
			map<ClientDataTree*, Distant*>::iterator it;
			ClientDataTree* clientTree;

			for(it = dataRouter.begin() ; (it != dataRouter.end() && !tree) ; it++) {
				clientTree = it->first;

				if(clientTree->getClientName() == clientName) {
					tree = clientTree;
				}
			}
		}

		if(tree) {
			string valeurType = StringUtils::findAndEraseFirstWord(ligne);

			if(valeurType == "int") {
				// Nom de la valeur
				string valeurName = StringUtils::findAndEraseFirstWord(ligne);

				// Valeur de la valeur
				string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
				int valeur = getIntParameter(valeurStr);

				// Coordonnées de la branche sur laquelle la valeur doit être ajoutée
				vector<int> brancheId = getIntParameters(ligne);

				tree->createValeurInt(brancheId, valeurName, 0, valeur);

				printStdLn("Valeur creee.", userOutput);
			}
			else {
				printErrLn("Syntaxe incorrecte (type de donnee inconnu)", userOutput);
			}
		}
		else {
			printErrLn("Syntaxe incorrecte (arbre inconnu)", userOutput);
		}
	}
	else {
		printErrLn("Syntaxe incorrecte", userOutput);
	}
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
