/*
 * DataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "data/MarqueurClient.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/OMessageStream.h"
#include "data/communication/IMessageStream.h"

#include "data/DataTree.h"

DataTree::DataTree() : _root(NULL, 0, "root") {
}

DataTree::~DataTree() {
}

Branche* DataTree::getBranche(vector<int>& brancheId) throw(NotExistingBrancheException) {
	vector<int>::iterator iter;

	Branche* branche = &_root;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		branche = branche->getSubBranche(*iter);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException();
	}

	return branche;
}

Valeur* DataTree::getValeur(vector<int>& valeurId) throw(NotExistingValeurException, NotExistingBrancheException) {
	vector<int>::iterator iter;

	Branche* branche = &_root;

	for(iter = valeurId.begin() ; (iter != valeurId.end() && branche != NULL) ; iter++) {
		branche = branche->getSubBranche(*iter);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException();
	}

	Valeur* valeur;

	if(iter != valeurId.end())  {
		valeur = branche->getValeur(*iter);
	}
	else {
		throw NotExistingValeurException();
	}

	return valeur;
}

Branche& DataTree::getRoot() {
	return _root;
}

Branche* DataTree::addBranche(vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheForClient(parentBrancheId, brancheName, 0, NULL);
}

Branche* DataTree::addBrancheForClient(vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Branche* branche = parentBranche->createSubBranche(brancheName);

	return (Branche*)addMarqueurForClient(client, branche, brancheClientTmpId);
}

Valeur* DataTree::addValeurInt(vector<int>& parentBrancheId, const string& valeurName, int valeur) {
	return addValeurIntForClient(parentBrancheId, valeurName, 0, valeur, NULL);
}

Valeur* DataTree::addValeurIntForClient(vector<int>& parentBrancheId, const string& valeurName, int valeurClientTmpId, int valeur, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Valeur* val = parentBranche->createValeurInt(valeurName, valeur);

	return (Valeur*)addMarqueurForClient(client, val, valeurClientTmpId);
}

Donnee* DataTree::addMarqueurForClient(Client* client, Donnee* donnee, int donneeClientTmpId) {
	vector<Client*>::iterator clIter;

	for(clIter = _clients.begin() ; clIter != _clients.end() ; clIter++) {
		Client* cl = *clIter;

		if(cl == client) {
			cl->addMarqueur(donnee, donneeClientTmpId);
		}
		else {
			cl->addMarqueur(donnee, 0);
		}
	}

	return donnee;
}

void DataTree::addClient(const string& clientName) {
	Client* client = new Client(clientName);

	// Init the marqueurs
	initBrancheClient(client, &_root);

	_clients.push_back(client);
}

void DataTree::initBrancheClient(Client* client, Branche* branche) {
	client->addMarqueur(branche, 0);

	// Init sub-branches
	{
		map<int, Branche*>& subBranches = branche->getSubBranches();
		map<int, Branche*>::iterator iterBr;

		for(iterBr = subBranches.begin() ; iterBr != subBranches.end() ; iterBr++) {
			initBrancheClient(client, iterBr->second);
		}
	}

	// Init values
	{
		map<int, Valeur*>& valeurs = branche->getValeurs();
		map<int, Valeur*>::iterator iterVl;

		for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
			client->addMarqueur(iterVl->second, 0);
		}
	}
}

vector<Client*>& DataTree::getClients() {
	return _clients;
}


void DataTree::diffuseChangements(void) {
	vector<Client*>::iterator clientIter;
	vector<Changement*> changements;

	for(clientIter = _clients.begin() ; clientIter != _clients.end() ; clientIter++) {
		Client* client = *clientIter;
		client->collecteChangements(changements);

		if(changements.size()) {
			OMessageStream out;
			DataSerializer::toStream(changements, out);
			client->sendData(out);
		}
	}
}

void DataTree::receiveChangements(void) {
	vector<Client*>::iterator clientIter;
	vector<Changement*> changements;

	for(clientIter = _clients.begin() ; clientIter != _clients.end() ; clientIter++) {
		Client* client = *clientIter;
		string data = client->receiveData();
		IMessageStream in(data);
		DataSerializer::fromStream(changements, in);

		vector<Changement*>::iterator itCh;

		for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
			(*itCh)->change(this);
		}
	}
}
