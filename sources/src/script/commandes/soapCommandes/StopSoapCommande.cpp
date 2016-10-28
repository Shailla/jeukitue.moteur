/*
 * StopSoapCommande.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "script/exception/IllegalParameterException.h"
#include "reseau/web/SoapServer.h"

#include "script/commandes/soapCommandes/StopSoapCommande.h"

using namespace jkt;

extern SoapServer soapServer;

StopSoapCommande::StopSoapCommande(CommandeInterpreter* interpreter) : Commande(interpreter) {
}

void StopSoapCommande::executeIt(std::string ligne, bool userOutput) throw(IllegalParameterException) {
	soapServer.stop();

	printStdLn("Injection soap arr�t�e.", userOutput);
}

string StopSoapCommande::getShortHelp() const {
	return "Arr�te l'injecteur Soap";
}

string StopSoapCommande::getHelp() const {
	return "TODO";
}
