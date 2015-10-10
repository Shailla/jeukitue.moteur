/*
 * DataTreeUtils.h
 *
 *  Created on: 19 déc. 2014
 *      Author: VGDJ7997
 */

#ifndef SRC_DATA_DATATREEUTILS_H_
#define SRC_DATA_DATATREEUTILS_H_

#include "data/DataTree.h"

class DataTreeUtils {
	DataTreeUtils();
public:
	static const char* TREE_VID_BRANCHE_NAME;
	static const char* TREE_CONTROL_BRANCHE_NAME;

	virtual ~DataTreeUtils();

	static void formatGameServerDataTree(DataTree* dataTree);
};

#endif /* SRC_DATA_DATATREEUTILS_H_ */
