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
	std::map<std::string, Commande*> _subCommandes;

protected:
	CommandeInterpreter* _interpreter;

	void printStdLn(const std::string& msg, bool userOutput);
	void printErrLn(const std::string& msg, bool userOutput);

	virtual void executeIt(std::string ligne, bool userOutput) = 0;

public:
	Commande(CommandeInterpreter* interpreter);
	virtual ~Commande();

	virtual std::string getHelp() const = 0;
	void execute(const std::string& ligne, bool userOutput);

	// Gestion des sous-commandes
	Commande* getSubCommande(const std::string& subCommandeName) throw(IllegalParameterException);
	void registerSubCommande(const std::string& subCommandeName, Commande* subCommande);

	static int getIntParameter(const string& word) throw(IllegalParameterException);
	static float getFloatParameter(const string& word) throw(IllegalParameterException);
	static std::vector<int> getIntParameters(const std::string& ligne) throw(IllegalParameterException);
};

#endif /* COMMANDE_H_ */
