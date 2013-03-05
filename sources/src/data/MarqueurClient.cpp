/*
 * MarqueurClient.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/MarqueurClient.h"

MarqueurClient::MarqueurClient() {
	_isUpToDate = false;
	_temporaryId = 0;
}

MarqueurClient::~MarqueurClient() {
}

bool MarqueurClient::isUpToDate(void) const {
	return _isUpToDate;
}

void MarqueurClient::setTemporaryId(int temporaryId) {
	_temporaryId = temporaryId;
}
