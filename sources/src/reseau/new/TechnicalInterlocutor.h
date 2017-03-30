/*
 * TechnicalInterlocutor.h
 *
 *  Created on: 17 juin 2013
 *      Author: vgdj7997
 */

#ifndef TECHNICALINTERLOCUTOR_H_
#define TECHNICALINTERLOCUTOR_H_

#include <string>

#include "reseau/new/Interlocutor2.h"

class TechnicalInterlocutor {
public:
	enum CONNEXION_STATUS {
		/** Initial status, can't connect, can't send or receive messages. */
		STOPPED = 0,

		/** Being connected. */
		CONNECTING,

		/** Available to receive and send messages. */
		CONNECTED,

		/** Being stopped. */
		STOPPING,
	};

private:
	std::string _name;
	Uint16 _localPort;
	CONNEXION_STATUS _status;
	SDL_Thread* _threadReceiving;
	SDL_Thread* _threadSending;
	SDL_Thread* _threadIntelligence;
	SDL_mutex* _mutexIntelligence;
	SDL_cond* _condIntelligence;

	static int startReceivingProcess(void* thiz);
	static int startSendingProcess(void* thiz);
	static int startIntelligenceProcess(void* thiz);

protected:
	void startProcesses();
	void waitProcessesFinished();

	virtual void receivingProcess() = 0;
	virtual void sendingProcess() = 0;
	virtual void intelligenceProcess() = 0;

	void setConnexionStatus(CONNEXION_STATUS status);

	SDL_mutex* getMutexIntelligence();
	SDL_cond* getCondIntelligence();

public:
	TechnicalInterlocutor(const std::string& name, Uint16 localPort);
	virtual ~TechnicalInterlocutor();

	Uint16 getLocalPort() const;
	CONNEXION_STATUS getConnexionStatus() const;
	virtual void close();

	// Log methods
	void log(int line, const char* sourceFile, const char* message);
	void log(int line, const char* sourceFile, const std::string& message);
	void log(int line, const char* sourceFile, const std::stringstream& message);
};

#endif /* TECHNICALINTERLOCUTOR_H_ */
