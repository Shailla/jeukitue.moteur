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
#include "data/ServeurDataTree.h"
#include "script/exception/IllegalParameterException.h"

#include "script/commandes/dataCommandes/UpdateDataCommande.h"

using namespace JktUtils;

extern ServeurDataTree serveurDataTree;

UpdateDataCommande::UpdateDataCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void UpdateDataCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	if(subCommande1 == "valeur") {
		// Nouvelle valeur de la valeur
		string valeurStr = StringUtils::findAndEraseFirstWord(ligne);

		// Coordonnées de la branche sur laquelle la valeur doit être ajoutée
		vector<int> brancheId = getIntParameters(ligne);

		if(brancheId.size() > 0) {
			int valeurId = brancheId[brancheId.size() - 1];
			brancheId.erase(brancheId.end() - 1);

			Valeur* vl = serveurDataTree.getValeur(brancheId, valeurId);
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
			printErrLn("Identifiant de valeur manquant", userOutput);
		}
	}
	else {
		printErrLn("Syntaxe incorrecte", userOutput);
	}
}

string UpdateDataCommande::getHelp() const {
	return "data update valeur <newValeur> <valeurId> : Valorise la donnée de type valeur identifiée par <valeurId> avec la valeur <newValeur>.\
\n<newValeur> depend du type de valeur qui est detecte automatiquement (entier, chaine de caracteres, ...)"\
"\nExemple : data update valeur 88 0 3 2";
}
