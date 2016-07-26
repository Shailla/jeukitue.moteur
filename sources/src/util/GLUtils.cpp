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

void GLUtils::drawCube(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
	glBegin(GL_QUADS);

	// Sur xMin
	glVertex3f(xMin,  yMin, zMin);
	glVertex3f(xMin,  yMax, zMin);
	glVertex3f(xMin,  yMax, zMax);
	glVertex3f(xMin,  yMin, zMax);

	// Sur xMax
	glVertex3f(xMax,  yMin, zMin);
	glVertex3f(xMax,  yMax, zMin);
	glVertex3f(xMax,  yMax, zMax);
	glVertex3f(xMax,  yMin, zMax);

	// Sur yMin
	glVertex3f(xMin,  yMin, zMin);
	glVertex3f(xMax,  yMin, zMin);
	glVertex3f(xMax,  yMin, zMax);
	glVertex3f(xMin,  yMin, zMax);

	// Sur yMax
	glVertex3f(xMin,  yMax, zMin);
	glVertex3f(xMax,  yMax, zMin);
	glVertex3f(xMax,  yMax, zMax);
	glVertex3f(xMin,  yMax, zMax);

	// Sur zMin
	glVertex3f(xMin,  yMin, zMin);
	glVertex3f(xMax,  yMin, zMin);
	glVertex3f(xMax,  yMax, zMin);
	glVertex3f(xMin,  yMax, zMin);

	// Sur zMaxn
	glVertex3f(xMin,  yMin, zMax);
	glVertex3f(xMax,  yMin, zMax);
	glVertex3f(xMax,  yMax, zMax);
	glVertex3f(xMin,  yMax, zMax);

	glEnd();
}
