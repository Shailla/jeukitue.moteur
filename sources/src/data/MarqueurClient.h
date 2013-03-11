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
	int _sentRevision;
	int _temporaryId;

public:
	static const int MARQUEUR_REVISION_INIT = -1;

	MarqueurClient(Donnee* donnee, int donneeTmpId);
	virtual ~MarqueurClient();

	Donnee* getDonnee() const;

	int getSentRevision(void) const;
	void setSentRevision(int sentRevision);

	int getTemporaryId() const;
	void setTemporaryId(int temporaryId);
};

#endif /* MARQUEURCLIENT_H_ */
