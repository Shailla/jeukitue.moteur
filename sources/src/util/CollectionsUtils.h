/*
 * StringUtils.h
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#ifndef COLLECTIONSUTILS_H_
#define COLLECTIONSUTILS_H_

#include <algorithm>
#include <vector>
#include <string>

namespace JktUtils {

template<class T>
bool contains(std::vector<T>& list, T element) {
	return std::find(list.begin(), list.end(), element) != list.end();
}

class CollectionsUtils {
	CollectionsUtils();
public:
	virtual ~CollectionsUtils();

	static std::string toString(const std::vector<int>& data);
};



} /* namespace JktUtils */
#endif /* STRINGUTILS_H_ */
