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
#include <stdexcept>

#include "util/StringUtils.h"
#include "script/CommandeInterpreter.h"

#include "script/commandes/Commande.h"

using namespace std;
using namespace jkt;

Commande::Commande(CommandeInterpreter* interpreter) {
	_interpreter = interpreter;
}

Commande::~Commande() {
}

Commande* Commande::getSubCommande(const string& subCommandeName)  throw(IllegalParameterException) {
	Commande* commande;

	try {
		commande = _subCommandes.at(subCommandeName);
	}
	catch(out_of_range& exception) {
		throw IllegalParameterException();
	}

	return commande;
}

void Commande::registerSubCommande(const std::string& subCommandeName, Commande* subCommande) {
	_subCommandes[subCommandeName] = subCommande;
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

int Commande::getIntParameter(const string& word) throw(IllegalParameterException) {
	int result;

	if(!(istringstream(word) >> result)) {
		throw IllegalParameterException();
	}

	return result;
}

float Commande::getFloatParameter(const string& word) throw(IllegalParameterException) {
	float result;

	if(!(istringstream(word) >> result)) {
		throw IllegalParameterException();
	}

	return result;
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
