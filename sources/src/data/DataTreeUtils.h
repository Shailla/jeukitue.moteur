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
	virtual ~DataTreeUtils();

	static void formatGameDataTree(DataTree* dataTree);
};

#endif /* SRC_DATA_DATATREEUTILS_H_ */
