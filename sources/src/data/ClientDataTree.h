/*
 * DataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef CLIENTDATATREE_H_
#define CLIENTDATATREE_H_

#include <string>
#include <map>
#include <vector>

using namespace std;

#include "data/DataTree.h"

class ClientDataTree : public DataTree {
	Branche _root;

public:
	ClientDataTree();
	virtual ~ClientDataTree();


	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void receiveChangements(const std::string& data);
};

#endif /* DATATREE_H_ */
