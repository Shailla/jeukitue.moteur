/*
 * Branche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

#include "util/types/IntData.h"
#include "util/types/FloatData.h"
#include "util/types/StringData.h"
#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/MarqueurDistant.h"

#include "data/Branche.h"

using namespace JktUtils;

Branche::Branche(Branche* parent, int brancheId, const string& brancheName, int revision, int brancheTmpId) : Donnee(revision) {
	_parent = parent;
	_brancheId = brancheId;
	_brancheName = brancheName;
	_brancheTmpId = brancheTmpId;
}

Branche::~Branche() {
}

void Branche::setBrancheId(int brancheId) {
	_brancheId = brancheId;
}

Branche* Branche::getSubBranche(int brancheId) const {
	Branche* branche = NULL;

	try {
		branche = _subBranches.at(brancheId);
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* Branche::getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheTmpId) throw(NotExistingBrancheException) {
	map<int, Branche*>::iterator it;
	MarqueurDistant* marqueur;
	Branche* subBranche = NULL;

	for(it = _subBranches.begin() ; (it!=_subBranches.end() && subBranche==NULL) ; it++) {
		subBranche = it->second;
		marqueur = subBranche->getMarqueur(distant);

		if(marqueur->getTemporaryId() != brancheTmpId) {
			subBranche = NULL;
		}
	}

	if(!subBranche) {
		throw NotExistingBrancheException();
	}

	return subBranche;
}

Valeur* Branche::getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException) {
	map<int, Valeur*>::iterator it;
	MarqueurDistant* marqueur;
	Valeur* valeur = NULL;

	for(it = _valeurs.begin() ; (it!=_valeurs.end() && valeur==NULL) ; it++) {
		valeur = it->second;
		marqueur = valeur->getMarqueur(distant);

		if(marqueur->getTemporaryId() != valeurTmpId) {
			valeur = NULL;
		}
	}

	if(!valeur) {
		throw NotExistingValeurException();
	}

	return valeur;
}

Branche* Branche::getSubBrancheByTmpId(int brancheTmpId) const {
	Branche* branche = NULL;

	try {
		branche = _subBranches.at( -brancheTmpId );
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Valeur* Branche::getValeurByTmpId(int valeurTmpId) const {
	Valeur* valeur = NULL;

	try {
		valeur = _valeurs.at( -valeurTmpId );
	}
	catch(out_of_range& exception) {
		valeur = NULL;
	}

	return valeur;
}

Branche* Branche::createSubBrancheForClient(const string& brancheName, int revision) {
	// Alloue une r�f�rence pour la nouvelle branche
	int tmpRef = _brancheTmpRefGenerator.genRef();		// On d�marre � 1

	// Cr�e la nouvelle branche
	Branche* newBranche = new Branche(this, -1, brancheName, revision, tmpRef);
	_subBranches[-tmpRef] = newBranche;		// Moins 1 pour �viter les collisions avec les ID non temporaires

	return newBranche;
}


Branche* Branche::createSubBrancheForServer(const string& brancheName, int revision) {
	// Alloue une r�f�rence pour la nouvelle branche
	int ref = _brancheRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants d�marrent � 0

	// Cr�e la nouvelle branche
	Branche* newBranche = new Branche(this, ref, brancheName, revision, -1);
	_subBranches[ref] = newBranche;

	return newBranche;
}

Branche* Branche::addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision) {
	if(_subBranches.find(brancheId) != _subBranches.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " La branche existe deja";
	}

	// Cr�e la nouvelle branche
	Branche* newBranche = new Branche(this, brancheId, brancheName, brancheRevision, -1);
	_subBranches[brancheId] = newBranche;

	return newBranche;
}

Branche* Branche::acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException) {
	Branche* branche = getSubBrancheByTmpId(brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException();
	}

	_subBranches.erase( -brancheTmpId );
	_subBranches[brancheId] = branche;
	branche->setBrancheId(brancheId);
	branche->setRevision(brancheRevision);

	return branche;
}

Valeur* Branche::acceptTmpValeur(int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException) {
	Valeur* valeur = getValeurByTmpId(valeurTmpId);

	if(!valeur) {
		throw NotExistingBrancheException();
	}

	_valeurs.erase( -valeurTmpId );
	_valeurs[valeurId] = valeur;
	valeur->setValeurId(valeurId);
	valeur->setRevision(valeurRevision);

	return valeur;
}

Valeur* Branche::createValeurForClient(const string& valeurName, int revision, const Data* valeur) {
	// Alloue une r�f�rence pour la nouvelle branche
	int tmpRef = _valeurTmpRefGenerator.genRef();		// On d�marre � 1

	// Cr�e la nouvelle branche
	Valeur* newValeur = NULL;

	if(const IntData* value = dynamic_cast<const IntData*>(valeur)) {
		newValeur = new ValeurInt(this, -1, valeurName, tmpRef, revision, value->getValue());
	}
	else if(const FloatData* value = dynamic_cast<const FloatData*>(valeur)) {
		newValeur = new ValeurFloat(this, -1, valeurName, tmpRef, revision, value->getValue());
	}
	else if(const StringData* value = dynamic_cast<const StringData*>(valeur)) {
		newValeur = new ValeurString(this, -1, valeurName, tmpRef, revision, value->getValue());
	}

	if(newValeur) {
		_valeurs[-tmpRef] = newValeur;
	}

	return newValeur;
}

Valeur* Branche::createValeurForServeur(const string& valeurName, int revision, const Data* valeur) {
	// Alloue une r�f�rence pour la nouvelle branche
	int ref = _valeurRefGenerator.genRef() - 1;		// On soustrait 1 pour que les identifiants d�marrent � 0

	// Cr�e la nouvelle branche
	Valeur* newValeur = NULL;

	if(const IntData* value = dynamic_cast<const IntData*>(valeur)) {
		newValeur = new ValeurInt(this, ref, valeurName, -1, revision, value->getValue());
	}
	else if(const FloatData* value = dynamic_cast<const FloatData*>(valeur)) {
		newValeur = new ValeurFloat(this, ref, valeurName, -1, revision, value->getValue());
	}
	else if(const StringData* value = dynamic_cast<const StringData*>(valeur)) {
		newValeur = new ValeurString(this, ref, valeurName, -1, revision, value->getValue());
	}

	if(newValeur) {
		_valeurs[ref] = newValeur;
	}

	return newValeur;
}

const Valeur* Branche::addValeur(int valeurId, const string& valeurName, int valeurRevision, const JktUtils::Data* valeur) {
	Valeur* newValeur = NULL;

	if(_valeurs.find(valeurId) != _valeurs.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " La valeur existe deja";
	}

	// Cr�e la nouvelle valeur
	if(const JktUtils::IntData* intData = dynamic_cast<const JktUtils::IntData*>(valeur)) {
		newValeur = new ValeurInt(this, valeurId, valeurName, -1, valeurRevision, intData->getValue());
		_valeurs[valeurId] = newValeur;
	}
	else if(const JktUtils::FloatData* floatData = dynamic_cast<const JktUtils::FloatData*>(valeur)) {
		newValeur = new ValeurFloat(this, valeurId, valeurName, -1, valeurRevision, floatData->getValue());
		_valeurs[valeurId] = newValeur;
	}
	else if(const JktUtils::StringData* stringData = dynamic_cast<const JktUtils::StringData*>(valeur)) {
		newValeur = new ValeurString(this, valeurId, valeurName, -1, valeurRevision, stringData->getValue());
		_valeurs[valeurId] = newValeur;
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Type de valeur inconnu";
	}

	return newValeur;
}

map<int, Branche*>& Branche::getSubBranches() {
	return _subBranches;
}

map<int, Valeur*>& Branche::getValeurs() {
	return _valeurs;
}

Valeur* Branche::getValeur(int valeurId) {
	try {
		return _valeurs.at(valeurId);
	}
	catch(std::out_of_range& exception) {
		return 0;
	}
}

string Branche::getBrancheName() const {
	return _brancheName;
}

int Branche::getBrancheId() const {
	return _brancheId;
}

int Branche::getBrancheTmpId() const {
	return _brancheTmpId;
}


vector<int> Branche::getParentBrancheId(void) const {
	vector<int> parentBrancheId;

	if(_parent) {
		parentBrancheId = _parent->getBrancheFullId();
	}
	else {
		// Do nothing, return empty id
	}

	return parentBrancheId;
}

void Branche::getBrancheFullId(vector<int>& id) const {
	if(_parent) {
		_parent->getBrancheFullId(id);
		id.push_back(_brancheId);
	}
}

vector<int> Branche::getBrancheFullId() const {
	vector<int> id;
	getBrancheFullId(id);

	return id;
}

void Branche::print(ostringstream& out, int indentation) {
	int i;

	// Affiche la branche
	out << endl;

	for(i = 0 ; i < indentation ; i++) {
		out << "   ";
	}

	out << "+ [" << _brancheId << ":" << _brancheTmpId << ":" << getRevision() << "] '" << _brancheName << "'";
	map<int, Valeur*>::iterator valIt;

	// Affiche les valeurs de la branche
	for(valIt = _valeurs.begin() ; valIt != _valeurs.end() ; ++valIt) {
		Valeur* valeur = valIt->second;

		out << endl;

		for(i = 0 ; i < indentation ; i++) {
				out << "   ";
		}

		out << "- [" << valeur->getValeurId() << ":" << valeur->getValeurTmpId() << ":" << valeur->getRevision() << "] '" << valeur->getValeurName() << "'";
	}

	// Affiche les branches de la branche
	map<int, Branche*>::iterator brIt;

	for(brIt = _subBranches.begin() ; brIt != _subBranches.end() ; ++brIt) {
		Branche* branche = brIt->second;
		branche->print(out, indentation+1);
	}
}
