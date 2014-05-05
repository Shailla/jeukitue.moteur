/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/types/IntData.h"
#include "util/types/FloatData.h"
#include "util/types/StringData.h"
#include "util/StringUtils.h"
#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"
#include "script/exception/IllegalParameterException.h"
#include "main/Game.h"

#include "script/commandes/dataCommandes/UpdateDataCommande.h"

using namespace JktUtils;

extern CGame Game;

UpdateDataCommande::UpdateDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void UpdateDataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	ServeurDataTree* serverDataTree = Game.getServerDataTree();
	ClientDataTree* clientDataTree = Game.getClientDataTree();

	if(subCommande1 == "valeur") {
		DataTree* tree = NULL;

		if(serverDataTree != 0) {
			tree = serverDataTree;
		}
		else if(clientDataTree != 0) {
			tree = clientDataTree;
		}
		else {
			printErrLn("Client et serveur tous deux inactifs", userOutput);
		}

		if(tree) {
			// Valeur de la valeur
			string valueStr = StringUtils::findAndEraseFirstWord(ligne);

			// Coordonnées de la branche sur laquelle la valeur doit être ajoutée
			vector<int> valeurFullId = getIntParameters(ligne);

			if(valeurFullId.size() > 0) {
				vector<int> brancheId = valeurFullId;
				brancheId.erase(brancheId.end() - 1);
				int valeurId = *(valeurFullId.end() - 1);

				Valeur* valeur = tree->getValeur(brancheId, valeurId);

				if(valeur) {
					Data* value = NULL;

					if(dynamic_cast<ValeurInt*>(valeur)) {
						// Valeur de la valeur
						string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
						value = new IntData(getIntParameter(valeurStr));
					}
					else if(dynamic_cast<ValeurFloat*>(valeur)) {
						// Valeur de la valeur
						string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
						value = new FloatData(getFloatParameter(valeurStr));
					}
					else if(dynamic_cast<ValeurString*>(valeur)) {
						// Valeur de la valeur
						value = new StringData(StringUtils::findAndEraseFirstString(ligne));
					}

					if(value) {
						valeur->updateValeur(value);

						ostringstream result;
						result << "Valeur mise à jour. Nouvelle revision " << valeur->getRevision() << ".";
						printStdLn(result.str().c_str(), userOutput);
					}
					else {
						printErrLn("Type de valeur inconnu", userOutput);
					}
				}
				else {
					printErrLn("Valeur introuvable", userOutput);
				}
			}
			else {
				printErrLn("Syntaxe incorrecte (identifiant de la valeur incomplet)", userOutput);
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

string UpdateDataCommande::getShortHelp() const {
	return "Met à jour une donnée de l'arbre";
}

string UpdateDataCommande::getHelp() const {
	return
"data update valeur server <valeur> <brancheId> <valeurId> : Met a jour dans les donnees serveur la valeur nommee identifiee <valeurId> sur la branche <brancheId> avec la valeur <valeur>.\
\nLe type de valeur (entier, flottant, ...) est detecte automatiquement.\
\nExemple : data update valeur client Erwin 22 0 3 2\
\n---\
\ndata update valeur client <clientName> <valeur> <brancheId> <valeurId> : Met a jour dans les donnees du client <clientName> la valeur nommee identifiee <valeurId> sur la branche <brancheId> avec la valeur <valeur>\
\nExemple : data update valeur client Erwin 22 0 3 2";
}
