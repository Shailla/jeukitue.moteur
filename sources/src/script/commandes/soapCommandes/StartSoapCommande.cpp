/*
 * StartSoapCommande.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "script/exception/IllegalParameterException.h"
#include "reseau/web/SoapServer.h"

#include "script/commandes/soapCommandes/StartSoapCommande.h"

using namespace jkt;

extern SoapServer soapServer;

StartSoapCommande::StartSoapCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void StartSoapCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	// Period
	string periodStr = StringUtils::findAndEraseFirstWord(ligne);
	int period = getIntParameter(periodStr);

	// Port client
	string portClientStr = StringUtils::findAndEraseFirstWord(ligne);
	int portClient = getIntParameter(portClientStr);

	// Port server
	string portServerStr = StringUtils::findAndEraseFirstWord(ligne);
	int portServer = getIntParameter(portServerStr);

	soapServer.open(period, portClient, portServer);

	printStdLn("Injection soap demarrée.", userOutput);
}

string StartSoapCommande::getShortHelp() const {
	return "Démarre l'injecteur Soap";
}

string StartSoapCommande::getHelp() const {
	return "TODO";
}
