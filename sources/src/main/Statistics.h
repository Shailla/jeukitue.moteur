/*
 * Statistics.h
 *
 *  Created on: 7 févr. 2013
 *      Author: vgdj7997
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <agar/core.h>

class Statistics {
	static Statistics* _instance;
	Statistics();
	AG_Mutex _mutex;
public:
	virtual ~Statistics();

	static Statistics* getInstance();
	void lock();
	void unlock();
	AG_Mutex* getMutex();

	float _fDebitEm;
	float _fTailleEm;
	float _fDebitRec;
	float _fTailleRec;
};

#endif /* STATISTICS_H_ */
