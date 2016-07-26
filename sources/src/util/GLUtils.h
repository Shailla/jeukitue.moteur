/*
 * GLUtils.h
 *
 *  Created on: 21 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_UTIL_GLUTILS_H_
#define SRC_UTIL_GLUTILS_H_

#include <GL/glu.h>

class GLUtils {
	GLUquadric* _quadricSphere;

public:
	GLUtils();
	virtual ~GLUtils();

	void drawSphere(float radius, int slices, int stacks);
	void drawCube(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
};

#endif /* SRC_UTIL_GLUTILS_H_ */
