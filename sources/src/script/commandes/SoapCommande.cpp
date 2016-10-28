/*
 * SoapCommande.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "script/exception/IllegalParameterException.h"

#include "script/commandes/SoapCommande.h"
#include "script/commandes/soapCommandes/StartSoapCommande.h"
#include "script/commandes/soapCommandes/StopSoapCommande.h"

using namespace jkt;

SoapCommande::SoapCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
	registerSubCommande("start", new StartSoapCommande(interpreter));
	registerSubCommande("stop", new StopSoapCommande(interpreter));
}

void SoapCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	string subCommande1 = StringUtils::findAndEraseFirstWord(ligne);

	Commande* subCommande = getSubCommande(subCommande1);	// Throws IllegalParameterException if the sub command is not found

	subCommande->executeIt(ligne, userOutput);
}

string SoapCommande::getShortHelp() const {
	return "Injecteur de signatures AE/MASP";
}

string SoapCommande::getHelp() const {
	return
"soap start <portServer> <period>"
"\nExemple : soap start 1000 18223 (envoi une signature toutes les secondes)";
}
