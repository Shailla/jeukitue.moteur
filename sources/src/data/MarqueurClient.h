/*
 * MarqueurClient.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef MARQUEURCLIENT_H_
#define MARQUEURCLIENT_H_

#include "data/Donnee.h"

#include "data/Branche.h"

class MarqueurClient {
	Donnee* _donnee;
	bool _isUpToDate;
	int _temporaryId;

public:
	MarqueurClient(Donnee* donnee, int donneeTmpId, bool isUpToDate);
	virtual ~MarqueurClient();

	Donnee* getDonnee() const;

	bool isUpToDate(void) const;

	int getTemporaryId() const;
	void setTemporaryId(int temporaryId);
};

#endif /* MARQUEURCLIENT_H_ */
