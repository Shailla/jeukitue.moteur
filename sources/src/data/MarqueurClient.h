/*
 * MarqueurClient.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef MARQUEURCLIENT_H_
#define MARQUEURCLIENT_H_

#include "data/Branche.h"

class MarqueurClient {
	bool _isUpToDate;
	int _temporaryId;
public:
	MarqueurClient();
	virtual ~MarqueurClient();

	bool isUpToDate(void) const;

	int getTemporaryId() const;
	void setTemporaryId(int temporaryId);
};

#endif /* MARQUEURCLIENT_H_ */
