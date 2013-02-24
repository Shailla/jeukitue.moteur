/*
 * Statistics.cpp
 *
 *  Created on: 7 févr. 2013
 *      Author: vgdj7997
 */

#include "main/Statistics.h"

Statistics* Statistics::_instance = NULL;

Statistics::Statistics() {
	AG_MutexInit(&_mutex);

	_fDebitEm = 0;
	_fTailleEm = 0;
	_fDebitRec = 0;
	_fTailleRec = 0;
}

Statistics::~Statistics() {
}

Statistics* Statistics::getInstance() {
	if(!_instance) {
		_instance = new Statistics();
	}

	return _instance;
}

void Statistics::lock() {
	AG_MutexLock(&_mutex);
}

void Statistics::unlock() {
	AG_MutexUnlock(&_mutex);
}

AG_Mutex* Statistics::getMutex() {
	return &_mutex;
}
