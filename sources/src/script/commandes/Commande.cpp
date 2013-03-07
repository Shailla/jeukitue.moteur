/*
 * Commande.cpp
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

#include "util/StringUtils.h"
#include "script/CommandeInterpreter.h"

#include "script/commandes/Commande.h"

using namespace JktUtils;

Commande::Commande(CommandeInterpreter* interpreter) {
	_interpreter = interpreter;
}

Commande::~Commande() {
}


void Commande::execute(const std::string& ligne, bool userOutput) {
	try {
		executeIt(ligne, userOutput);
	}
	catch(IllegalParameterException& exception) {
		printErrLn("Erreur de syntaxe", userOutput);
	}
}

void Commande::printStdLn(const std::string& msg, bool userOutput) {
	_interpreter->printStdLn(msg, userOutput);
}

void Commande::printErrLn(const std::string& msg, bool userOutput) {
	_interpreter->printErrLn(msg, userOutput);
}

std::vector<int> Commande::getIntParameters(const string& ligne) throw(IllegalParameterException) {
	vector<int> result;
	vector<string> parameters = StringUtils::splitBySpaces(ligne);
	vector<string>::iterator iter;
	int var;

	for(iter = parameters.begin() ; iter != parameters.end() ; iter++) {
		string p = *iter;

		if(!(istringstream(p) >> var)) {
			cout << endl << "EXCEPTION" << flush;
			throw IllegalParameterException();
		}

		result.push_back(var);
	}

	return result;
}
