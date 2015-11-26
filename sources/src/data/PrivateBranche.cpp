/*
 * PrivateBranche.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

#include "util/CollectionsUtils.h"
#include "data/Branche.h"
#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/MarqueurDistant.h"
#include "data/exception/UnavailableOperationException.h"
#include "data/exception/NotSpecifiedClientException.h"

#include "data/PrivateBranche.h"

using namespace JktUtils;

PrivateBranche::PrivateBranche(AbstractBranche* parent, int brancheId, const string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId) : Branche(parent, brancheId, brancheName, brancheType, revision, brancheTmpId) {
}

PrivateBranche::~PrivateBranche() {
}

map<DistantTreeProxy*, PrivateBranche::DistantPrivateBranche>& PrivateBranche::getDistants() {
	return _distants;
}

PrivateBranche::DistantPrivateBranche* PrivateBranche::addDistant(DistantTreeProxy* distant) {
	DistantPrivateBranche var;
	_distants[distant] = var;

	return &_distants[distant];
}

PrivateBranche::DistantPrivateBranche* PrivateBranche::getDistant(DistantTreeProxy* distant) {
	DistantPrivateBranche* dpb = NULL;

	if(distant) {
		try {
			dpb = &_distants.at(distant);
		}
		catch(out_of_range& exception) {
			dpb = NULL;
		}
	}
	else {
		dpb = NULL;
	}

	return dpb;
}

Branche* PrivateBranche::getSubBrancheByName(DistantTreeProxy* distant, const string& brancheName) {
	Branche* branche = NULL;

	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByName' need to know the client on private branche");
	}

	try {
		branche = getDistant(distant)->_subBranchesByName.at(brancheName);
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* PrivateBranche::getSubBrancheByIdOrTmpId(DistantTreeProxy* distant, int brancheId) {
	Branche* branche = NULL;

	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* prBr = getDistant(distant);

	try {
		if(brancheId >= 0) {
			branche = prBr->_subBranchesById.at(brancheId);
		}
		else {
			branche = prBr->_subBranchesByTmpId.at(brancheId);
		}
	}
	catch(out_of_range& exception) {
		branche = NULL;
	}

	return branche;
}

Branche* PrivateBranche::getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	if(brancheId >= 0) {
		return getSubBrancheByIdOrTmpId(distant, brancheId);
	}
	else {
		return getSubBrancheByDistantTmpId(distant, brancheId);
	}
}

Branche* PrivateBranche::getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<Branche*>::iterator it;
	MarqueurDistant* marqueur;
	Branche* subBranche = NULL;

	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* prBr = getDistant(distant);

	for(it = prBr->_subBranches.begin() ; (it!=prBr->_subBranches.end() && subBranche==NULL) ; it++) {
		subBranche = *it;
		marqueur = subBranche->getMarqueur(distant);

		if(marqueur->getTemporaryId() != brancheTmpId) {
			subBranche = NULL;
		}
	}

	if(!subBranche) {
		throw NotExistingBrancheException("PrivateBranche::getSubBrancheByDistantTmpId 1");
	}

	return subBranche;
}

Valeur* PrivateBranche::getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException) {
	vector<Valeur*>::iterator it;
	MarqueurDistant* marqueur;
	Valeur* valeur = NULL;

	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* prBr = getDistant(distant);

	for(it = prBr->_valeurs.begin() ; (it!=prBr->_valeurs.end() && valeur==NULL) ; it++) {
		valeur = *it;
		marqueur = valeur->getMarqueur(distant);

		if(marqueur->getTemporaryId() != valeurTmpId) {
			valeur = NULL;
		}
	}

	if(!valeur) {
		throw NotExistingValeurException("PrivateBranche::getValeurByDistantTmpId 2");
	}

	return valeur;
}

Valeur* PrivateBranche::getValeurByIdOrTmpId(DistantTreeProxy* distant, int valeurId) {
	Valeur* valeur = NULL;

	try {
		if(!distant) {
			throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
		}

		PrivateBranche::DistantPrivateBranche* prBr = getDistant(distant);

		if(valeurId >= 0) {
			valeur = prBr->_valeursById.at(valeurId);
		}
		else {
			valeur = prBr->_valeursByTmpId.at(valeurId);
		}
	}
	catch(out_of_range& exception) {
		valeur = NULL;
	}

	return valeur;
}

Branche* PrivateBranche::createSubBrancheForClient(DistantTreeProxy* distant, const string& brancheName, int revision) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int tmpRef = -dst->_brancheTmpRefGenerator.genRef();		// On démarre à -1

	// Crée la nouvelle branche
	Branche* newBranche = new Branche(this, -1, brancheName, getDonneeType(), revision, tmpRef);
	dst->_subBranchesByTmpId[tmpRef] = newBranche;		// Moins 1 pour éviter les collisions avec les ID non temporaires
	dst->_subBranches.push_back(newBranche);

	return newBranche;
}

Branche* PrivateBranche::createSubBrancheForServer(DistantTreeProxy* distant, const string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int ref = dst->_brancheRefGenerator.genRef();	// On démarre à 1

	// Vérifie si la branche n'existe pas déjà
	bool brancheExisting = true;

	try {
		dst->_subBranchesByName.at(brancheName);
	}
	catch(out_of_range& exception) {
		brancheExisting = false;
	}

	if(brancheExisting) {
		stringstream message;
		message << "La branche nommée '" << brancheName << "' de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
		throw AlreadyExistingBrancheException(message.str());
	}

	// Crée la nouvelle branche
	if(type != DONNEE_DEFAULT && type != DONNEE_PRIVATE_SUB) {
		cerr << endl << "Une branche privée ne peut créer que des sous-branches sous-privées";
	}

	Branche* newBranche = new Branche(this, ref, brancheName, DONNEE_PRIVATE_SUB, revision, -1);

	dst->_subBranchesById[ref] = newBranche;
	dst->_subBranchesByName[brancheName] = newBranche;
	dst->_subBranches.push_back(newBranche);

	return newBranche;
}

Branche* PrivateBranche::addSubBranche(DistantTreeProxy* distant, int brancheId, DONNEE_TYPE type, const std::string& brancheName, int brancheRevision) {
	Branche* newBranche;

	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	if(type != DONNEE_DEFAULT && type != DONNEE_PRIVATE_SUB) {
		cerr << endl << "Une branche privée ne peut créer que des sous-branches sous-privées";
	}

	PrivateBranche::DistantPrivateBranche* prBr = getDistant(distant);

	map<int, Branche*>::iterator iter = prBr->_subBranchesById.find(brancheId);

	if(iter != prBr->_subBranchesById.end()) {
		cerr << endl << __FILE__ << ":" << __LINE__ << "La branche " << brancheId << " (" << brancheName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
		newBranche = iter->second;
	}
	else {
		// Crée la nouvelle branche
		newBranche = new Branche(this, brancheId, brancheName, DONNEE_PRIVATE_SUB, brancheRevision, -1);
		prBr->_subBranchesById[brancheId] = newBranche;
		prBr->_subBranchesByName[brancheName] = newBranche;
		prBr->_subBranches.push_back(newBranche);
	}

	return newBranche;
}

Branche* PrivateBranche::acceptTmpSubBranche(DistantTreeProxy* distant, int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* prBr = getDistant(distant);

	Branche* branche = getSubBrancheByIdOrTmpId(distant, brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException("PrivateBranche::acceptTmpSubBranche");
	}

	prBr->_subBranchesById[brancheId] = branche;
	prBr->_subBranchesByName[branche->getBrancheName()] = branche;
	branche->setBrancheId(brancheId);
	branche->setRevision(brancheRevision);

	return branche;
}

Valeur* PrivateBranche::acceptTmpValeur(DistantTreeProxy* distant, int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* prBr = getDistant(distant);

	Valeur* valeur = getValeurByIdOrTmpId(distant, valeurTmpId);

	if(!valeur) {
		throw NotExistingBrancheException("PrivateBranche::acceptTmpValeur");
	}

	prBr->_valeursById[valeurId] = valeur;
	valeur->setValeurId(valeurId);
	valeur->setRevision(valeurRevision);

	return valeur;
}

Valeur* PrivateBranche::createValeurForClient(DistantTreeProxy* distant, UPDATE_MODE updateMode, const string& valeurName, int revision, const AnyData& valeur) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int tmpRef = -dst->_valeurTmpRefGenerator.genRef();		// On démarre à -1

	// Crée la nouvelle branche
	Valeur* newValeur = NULL;

	if(valeur.isInt()) {
		newValeur = new ValeurInt(this, -1, valeurName, DONNEE_PRIVATE_SUB, updateMode, tmpRef, revision, valeur.getValueInt());
	}
	else if(valeur.isFloat()) {
		newValeur = new ValeurFloat(this, -1, valeurName, DONNEE_PRIVATE_SUB, updateMode, tmpRef, revision, valeur.getValueFloat());
	}
	else if(valeur.isString()) {
		newValeur = new ValeurString(this, -1, valeurName, DONNEE_PRIVATE_SUB, updateMode, tmpRef, revision, valeur.getValueString());
	}

	if(newValeur) {
		dst->_valeursByTmpId[tmpRef] = newValeur;
		dst->_valeurs.push_back(newValeur);
	}

	return newValeur;
}

Valeur* PrivateBranche::createValeurForServeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const string& valeurName, int revision, const AnyData& valeur) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* dst = getDistant(distant);

	// Alloue une référence pour la nouvelle branche
	int ref = dst->_valeurRefGenerator.genRef();		// On démarre à 1

	// Crée la nouvelle branche
	Valeur* newValeur = NULL;

	if(valeur.isInt()) {
		newValeur = new ValeurInt(this, ref, valeurName, DONNEE_PRIVATE_SUB, updateMode, -1, revision, valeur.getValueInt());
	}
	else if(valeur.isFloat()) {
		newValeur = new ValeurFloat(this, ref, valeurName, DONNEE_PRIVATE_SUB, updateMode, -1, revision, valeur.getValueFloat());
	}
	else if(valeur.isString()) {
		newValeur = new ValeurString(this, ref, valeurName, DONNEE_PRIVATE_SUB, updateMode, -1, revision, valeur.getValueString());
	}

	if(newValeur) {
		dst->_valeursById[ref] = newValeur;
		dst->_valeurs.push_back(newValeur);
	}

	return newValeur;
}

Valeur* PrivateBranche::addValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, int valeurId, const string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur) {
	if(!distant) {
		throw NotSpecifiedClientException("'getSubBrancheByIdOrDistantTmpId' need to know the client on private branche");
	}

	PrivateBranche::DistantPrivateBranche* dst = getDistant(distant);
	Valeur* newValeur = getValeurByIdOrTmpId(distant, valeurId);

	if(newValeur) {
		cout << endl << __FILE__ << ":" << __LINE__ << "La valeur " << valeurId << " (" << valeurName << ") de parent " << CollectionsUtils::toString(getBrancheFullId()) << " existe déjà";
	}
	else {
		// Crée la nouvelle valeur
		if(valeur.isInt()) {
			newValeur = new ValeurInt(this, valeurId, valeurName, DONNEE_PRIVATE_SUB, updateMode, -1, valeurRevision, valeur.getValueInt());
			dst->_valeursById[valeurId] = newValeur;
			dst->_valeurs.push_back(newValeur);
		}
		else if(valeur.isFloat()) {
			newValeur = new ValeurFloat(this, valeurId, valeurName, DONNEE_PRIVATE_SUB, updateMode, -1, valeurRevision, valeur.getValueFloat());
			dst->_valeursById[valeurId] = newValeur;
			dst->_valeurs.push_back(newValeur);
		}
		else if(valeur.isString()) {
			newValeur = new ValeurString(this, valeurId, valeurName, DONNEE_PRIVATE_SUB, updateMode, -1, valeurRevision, valeur.getValueString());
			dst->_valeursById[valeurId] = newValeur;
			dst->_valeurs.push_back(newValeur);
		}
		else {
			cerr << endl << __FILE__ << ":" << __LINE__ << " Type de valeur inconnu";
		}
	}

	return newValeur;
}

vector<Branche*>* PrivateBranche::getSubBranches(DistantTreeProxy* distant) {
	PrivateBranche::DistantPrivateBranche* dd = getDistant(distant);

	return dd?(&dd->_subBranches):0;
}

vector<Valeur*>& PrivateBranche::getValeurs(DistantTreeProxy* distant) {
	return getDistant(distant)->_valeurs;
}

void PrivateBranche::print(ostringstream& out, DistantTreeProxy* distant, bool details, int indentation) {
	int i;

	out << endl;

	// Affiche la branche
	switch(getDonneeType()) {
	case DONNEE_DEFAULT:
		out << "+err_d";	// Une donnée ne devrait jamais être de ce type, elle est créée avec lui pour recevoir automatiquement son vrai type
		break;
	case DONNEE_PUBLIC:
		out << "+pub";
		break;
	case DONNEE_PRIVATE:
		out << "+pri";
		break;
	case DONNEE_PRIVATE_SUB:
		out << "+prs";
		break;
	case DONNEE_LOCAL:
		out << "+loc";
		break;
	default:
		out << "+err";	// Type non-reconnu
		break;
	}

	out << "\t";

	for(i = 0 ; i < indentation ; i++) {
		out << "+";
	}

	out << "+ [id=" << _brancheId;

	if(details) {
		out << " tmpId=" << _brancheTmpId;
	}

	out << " rv=" << getRevision() << "] '" << _brancheName << "'";

	std::map<DistantTreeProxy*, DistantPrivateBranche>::iterator itDt;

	for(itDt = _distants.begin() ; itDt != _distants.end() ; itDt++) {
		if(itDt->first) {	// Ignore le distant bouchon, voir dans le constructeur de PrivateBranche
			if(!distant || itDt->first == distant) {
				// Affiche le distant uniquement si celui-ci n'a pas été explicitement spécifié
				if(!distant) {
					out << endl << "\t" << "[[ Distant '" << itDt->first->getInterlocutor()->getName() <<"' ]]";
				}

				// Affiche les valeurs de la branche de manière ordonnée
				vector<Valeur*>& valeurs = getDistant(itDt->first)->_valeurs;

				sort(valeurs.begin(), valeurs.end(), Valeur::highestId);

				vector<Valeur*>::iterator valIt;

				for(valIt = valeurs.begin() ; valIt != valeurs.end() ; ++valIt) {
					Valeur* valeur = *valIt;

					out << endl;

					switch(valeur->getDonneeType()) {
					case DONNEE_DEFAULT:
						out << ".err_d";	// Une donnée ne devrait jamais être de ce type, elle est créée avec pour recevoir automatiquement son vrai type
						break;
					case DONNEE_PUBLIC:
						out << ".pub";
						break;
					case DONNEE_PRIVATE:
						out << ".pri";
						break;
					case DONNEE_PRIVATE_SUB:
						out << ".prs";
						break;
					case DONNEE_LOCAL:
						out << ".loc";
						break;
					default:
						out << ".err";
						break;
					}

					out << "\t";

					for(i = 0 ; i < indentation+1 ; i++) {
						out << ".";
					}

					out << ". [id=" << valeur->getValeurId();

					if(details) {
						out << " tmpId=" << valeur->getValeurTmpId();
					}

					out << " rv=" << valeur->getRevision() << "] '" << valeur->getValeurName() << "' <" << valeur->toString() << ">";
				}
			}
		}

		// Affiche les branches de la branche
		vector<Branche*>& branches = getDistant(itDt->first)->_subBranches;
		sort(branches.begin(), branches.end(), Branche::highestId);

		vector<Branche*>::iterator brIt;

		for(brIt = branches.begin() ; brIt != branches.end() ; ++brIt) {
			Branche* branche = *brIt;
			branche->print(out, distant, details, indentation+1);
		}
	}
}
