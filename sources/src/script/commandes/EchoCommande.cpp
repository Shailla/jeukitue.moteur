/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include "util/StringUtils.h"

#include "script/commandes/EchoCommande.h"

using namespace std;

EchoCommande::EchoCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void EchoCommande::executeIt(std::string ligne, bool userOutput) {
	jkt::StringUtils::trim(ligne);
	printStdLn(ligne, userOutput);
}

string EchoCommande::getShortHelp() const {
	return "Affiche un texte dans la console";
}

string EchoCommande::getHelp() const {
	return "echo <texte>\nAffiche le texte <texte> dans la console. Les espaces de debut et fin sont ignores";
}
