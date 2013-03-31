/*
 * Interlocutor.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef INTERLOCUTOR_H_
#define INTERLOCUTOR_H_

#include <string>
#include <queue>

class Interlocutor {
	std::string _name;
	std::queue<std::string*> _dataReceived;
	std::queue<std::string*> _dataToSend;

public:
	Interlocutor(const std::string& name);
	virtual ~Interlocutor();

	const std::string& getName() const;

	void addDataReceived(std::string* data);
	std::string* getDataReceived();

	void addDataToSend(std::string* data);
	std::string* getDataToSend();
};

#endif /* INTERLOCUTOR_H_ */
