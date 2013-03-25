/*
 * Distant.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef DISTANT_H_
#define DISTANT_H_

#include <string>
#include <map>
#include <vector>
#include <sstream>

class MarqueurDistant;
class Donnee;
class Changement;

class Distant {
	std::string _debugName;
	std::map<Donnee*, MarqueurDistant*> _marqueurs;

	std::string* _dataReceived;
	std::string* _dataToSend;

public:
	Distant(const std::string& debugName);
	virtual ~Distant();

	MarqueurDistant* getMarqueur(Donnee* donnee);
	std::map<Donnee*, MarqueurDistant*>& getMarqueurs();
	std::string& getDebugName();
	MarqueurDistant* addMarqueur(Donnee* donnee, int donneeTmpId);
	void collecteChangementsInClientTree(std::vector<Changement*>& changements);
	void collecteChangementsInServerTree(std::vector<Changement*>& changements);

	void setDataReceived(std::string* data);
	std::string* getDataReceived();

	void setDataToSend(std::string* data);
	std::string* getDataToSend();
};

#endif /* DISTANT_H_ */
