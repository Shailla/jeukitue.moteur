/*
 * EchoCommand.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

using namespace std;

#include "util/StringUtils.h"

#include "script/commandes/EchoCommande.h"

EchoCommande::EchoCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void EchoCommande::executeIt(std::string ligne, bool userOutput) {
	JktUtils::StringUtils::trim(ligne);
	printStdLn(ligne, userOutput);
}

string EchoCommande::getHelp() const {
	return "echo <texte>\nAffiche le texte <texte> dans la console. Les espaces de debut et fin sont ignores";
}
