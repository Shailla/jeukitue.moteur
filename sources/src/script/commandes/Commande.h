/*
 * Commande.h
 *
 *  Created on: 6 mars 2013
 *      Author: vgdj7997
 */

#ifndef COMMANDE_H_
#define COMMANDE_H_

#include <string>
#include <map>

#include "script/exception/IllegalParameterException.h"

class CommandeInterpreter;

class Commande {
	friend class DataCommande;

protected:
	CommandeInterpreter* _interpreter;
	std::map<std::string, Commande*> _subCommandes;

	void printStdLn(const std::string& msg, bool userOutput);
	void printErrLn(const std::string& msg, bool userOutput);
	virtual void executeIt(std::string ligne, bool userOutput) = 0;

public:
	Commande(CommandeInterpreter* interpreter);
	virtual ~Commande();

	virtual std::string getHelp() = 0;
	void execute(const std::string& ligne, bool userOutput);

	static int getIntParameter(const string& word) throw(IllegalParameterException);
	static std::vector<int> getIntParameters(const std::string& ligne) throw(IllegalParameterException);
};

#endif /* COMMANDE_H_ */
