/*
 * TechnicalInterlocutor.h
 *
 *  Created on: 17 juin 2013
 *      Author: vgdj7997
 */

#ifndef TECHNICALINTERLOCUTOR_H_
#define TECHNICALINTERLOCUTOR_H_

#include <string>

using namespace std;

#include "reseau/new/Interlocutor.h"

class TechnicalInterlocutor {

	enum CONNEXION_STATUS {
		/** Initial status, can't connect, can't send or receive messages. */
		STOPPED = 0,

		/** Everything is ready to launch a connect() operation. */
		READY_TO_CONNECT,

		/** Available to receive and send messages. */
		CONNECTED,

		/** Abnormal status, the interlocutor can't be restarted. */
		DEAD
	};

	string _ip;
	Uint16 _port;
	Interlocutor& _interlocutor;
	CONNEXION_STATUS _status;

	int startBoucle(void* thiz);

protected:
	virtual int boucle();
	const string& getIp() const;
	Uint16 getPort() const;
	Interlocutor& getInterlocutor();
	void setConnexionStatus(CONNEXION_STATUS status);

public:
	TechnicalInterlocutor(const string& ip, Uint16 port, Interlocutor& interlocutor);
	virtual ~TechnicalInterlocutor();

	CONNEXION_STATUS getConnexionStatus() const;

	virtual bool connect();
	virtual void close();
};

#endif /* TECHNICALINTERLOCUTOR_H_ */
