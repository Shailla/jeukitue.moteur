/*
 * TreeConstants.h
 *
 *  Created on: 15 mars 2013
 *      Author: Erwin
 */

#ifndef TREECONSTANTS_H_
#define TREECONSTANTS_H_

enum TREE_TYPE {
	TREE_LOCAL,
	TREE_SERVER,
	TREE_CLIENT
};

enum TREE_STATE {
	STATE_NOT_READY = 0,	// Data tree not ready to receive data
	STATE_READY				// Data tree ready to receive data
};

#endif /* TREECONSTANTS_H_ */
