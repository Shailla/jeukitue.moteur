/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

#include "util/StringUtils.h"
#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"
#include "script/exception/IllegalParameterException.h"
#include "main/Game.h"

#include "script/commandes/dataCommandes/UpdateDataCommande.h"

using namespace std;
using namespace jkt;

extern CGame Game;

UpdateDataCommande::UpdateDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void UpdateDataCommande::executeIt(std::string ligne, bool userOutput) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "valeur") {
		DataTree* dataTree = Game.getDataTree();

		if(dataTree == 0) {
			printErrLn("Client et serveur tous deux inactifs", userOutput);
		}
		else {
			// Valeur de la valeur
			string valueStr = StringUtils::findAndEraseFirstWord(ligne);

			// Coordonn�es de la branche sur laquelle la valeur doit �tre ajout�e
			vector<int> valeurFullId = getIntParameters(ligne);

			if(valeurFullId.size() > 0) {
				vector<int> brancheId = valeurFullId;
				brancheId.erase(brancheId.end() - 1);
				int valeurId = *(valeurFullId.end() - 1);

				Valeur* valeur = dataTree->getValeur(0, brancheId, valeurId);

				if(valeur) {
					bool result = true;

					try {
						if(dynamic_cast<ValeurInt*>(valeur)) {
							// Valeur de la valeur
							string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
							((ValeurInt*)valeur)->updateValeur(getIntParameter(valeurStr));
						}
						else if(dynamic_cast<ValeurFloat*>(valeur)) {
							// Valeur de la valeur
							string valeurStr = StringUtils::findAndEraseFirstWord(ligne);
							((ValeurFloat*)valeur)->updateValeur(getFloatParameter(valeurStr));
						}
						else if(dynamic_cast<ValeurString*>(valeur)) {
							// Valeur de la valeur
							((ValeurString*)valeur)->updateValeur(StringUtils::findAndEraseFirstString(ligne));
						}
					}
					catch(IllegalParameterException& exception) {
						printErrLn("Valeur incompatible avec le type de la valeur", userOutput);
						result = false;
					}

					if(result) {
						ostringstream result;
						result << "Valeur mise � jour. Nouvelle revision " << valeur->getRevision() << ".";
						printStdLn(result.str().c_str(), userOutput);
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
	}
	else {
		printErrLn("Syntaxe incorrecte", userOutput);
	}
}

string UpdateDataCommande::getShortHelp() const {
	return "Met � jour une donn�e de l'arbre";
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
