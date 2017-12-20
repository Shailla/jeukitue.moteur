/*
 * MathUtils.h
 *
 *  Created on: 7 oct. 2017
 *      Author: Erwin
 */

#ifndef SRC_UTIL_MATHUTILS_H_
#define SRC_UTIL_MATHUTILS_H_

namespace jkt {

class MathUtils {
public:
	MathUtils();
	virtual ~MathUtils();

	static float determinantMatrix4x4(float var[16]);
	static void multMatrix4x4(float var1[16], float var2[16], float result[16]);
	static void multMatrixVector4x4(const float matrix[16], const float vector[4], float result[4]);
	static bool invertMatrix4x4(float var[16], float result[16]);
};

} /* namespace jkt */

#endif /* SRC_UTIL_MATHUTILS_H_ */
