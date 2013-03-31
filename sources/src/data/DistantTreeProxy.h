/*
 * DistantTreeProxy.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef DISTANTTREEPROXY_H_
#define DISTANTTREEPROXY_H_

#include <string>
#include <map>
#include <vector>
#include <sstream>

class Interlocutor;
class MarqueurDistant;
class Donnee;
class Changement;

class DistantTreeProxy {
	Interlocutor* _interlocutor;
	std::map<Donnee*, MarqueurDistant*> _marqueurs;

public:
	DistantTreeProxy(Interlocutor* interlocutor);
	virtual ~DistantTreeProxy();

	Interlocutor* getInterlocutor() const;
	MarqueurDistant* getMarqueur(Donnee* donnee);
	std::map<Donnee*, MarqueurDistant*>& getMarqueurs();
	MarqueurDistant* addMarqueur(Donnee* donnee, int donneeTmpId);
	void collecteChangementsInClientTree(std::vector<Changement*>& changements);
	void collecteChangementsInServerTree(std::vector<Changement*>& changements);
};

#endif /* DISTANTTREEPROXY_H_ */
