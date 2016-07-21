/*
 * GLUtils.cpp
 *
 *  Created on: 21 juil. 2016
 *      Author: Erwin
 */

#include "util/GLUtils.h"

GLUtils::GLUtils() {
	_quadricSphere = gluNewQuadric();
}

GLUtils::~GLUtils() {
	gluDeleteQuadric(_quadricSphere);
}

void GLUtils::drawSphere(float radius, int slices, int stacks) {
	gluSphere(_quadricSphere, radius, slices, stacks);
}
