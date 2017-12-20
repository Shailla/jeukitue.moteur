/*
 * MathUtils.cpp
 *
 *  Created on: 7 oct. 2017
 *      Author: Erwin
 */

#include <util/MathUtils.h>

namespace jkt {

MathUtils::MathUtils() {
}

MathUtils::~MathUtils() {
}

float MathUtils::determinantMatrix4x4(float var[16]) {
	float a11 = var[0], 	a12 = var[1], 	a13 = var[2],	a14 = var[3];
	float a21 = var[4], 	a22 = var[5], 	a23 = var[6],	a24 = var[7];
	float a31 = var[8], 	a32 = var[9], 	a33 = var[10], 	a34 = var[11];
	float a41 = var[12], 	a42 = var[13], 	a43 = var[14], 	a44 = var[15];

	float D11 = a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42;
	float D12 = a21*a33*a44 + a23*a34*a41 + a24*a31*a43 - a21*a34*a43 - a24*a33*a41 - a23*a31*a44;
	float D13 = a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a24*a32*a41 - a22*a31*a44;
	float D14 = a21*a32*a43 + a22*a33*a41 + a23*a31*a42 - a21*a33*a42 - a23*a32*a41 - a22*a31*a43;

	return a11*D11 - a12*D12 + a13*D13 - a14*D14;
}

void MathUtils::multMatrix4x4(float var1[16], float var2[16], float result[16]) {
	result[0] = 	var1[0]*var2[0] 	+ var1[1]*var2[4] 	+ var1[2]*var2[8] 	+ var1[3]*var2[12];
	result[1] = 	var1[0]*var2[1] 	+ var1[1]*var2[5] 	+ var1[2]*var2[9] 	+ var1[3]*var2[13];
	result[2] = 	var1[0]*var2[2] 	+ var1[1]*var2[6] 	+ var1[2]*var2[10] 	+ var1[3]*var2[14];
	result[3] = 	var1[0]*var2[3] 	+ var1[1]*var2[7] 	+ var1[2]*var2[11] 	+ var1[3]*var2[15];

	result[4] = 	var1[4]*var2[0] 	+ var1[5]*var2[4] 	+ var1[6]*var2[8] 	+ var1[7]*var2[12];
	result[5] = 	var1[4]*var2[1] 	+ var1[5]*var2[5] 	+ var1[6]*var2[9] 	+ var1[7]*var2[13];
	result[6] = 	var1[4]*var2[2] 	+ var1[5]*var2[6] 	+ var1[6]*var2[10] 	+ var1[7]*var2[14];
	result[7] = 	var1[4]*var2[3] 	+ var1[5]*var2[7] 	+ var1[6]*var2[11] 	+ var1[7]*var2[15];

	result[8] = 	var1[8]*var2[0] 	+ var1[9]*var2[4] 	+ var1[10]*var2[8] 	+ var1[11]*var2[12];
	result[9] = 	var1[8]*var2[1] 	+ var1[9]*var2[5] 	+ var1[10]*var2[9] 	+ var1[11]*var2[13];
	result[10] = 	var1[8]*var2[2] 	+ var1[9]*var2[6] 	+ var1[10]*var2[10] + var1[11]*var2[14];
	result[11] = 	var1[8]*var2[3] 	+ var1[9]*var2[7] 	+ var1[10]*var2[11] + var1[11]*var2[15];

	result[12] = 	var1[12]*var2[0] 	+ var1[13]*var2[4] 	+ var1[14]*var2[8] 	+ var1[15]*var2[12];
	result[13] = 	var1[12]*var2[1] 	+ var1[13]*var2[5] 	+ var1[14]*var2[9] 	+ var1[15]*var2[13];
	result[14] = 	var1[12]*var2[2] 	+ var1[13]*var2[6] 	+ var1[14]*var2[10] + var1[15]*var2[14];
	result[15] = 	var1[12]*var2[3] 	+ var1[13]*var2[7] 	+ var1[14]*var2[11] + var1[15]*var2[15];
}

void MathUtils::multMatrixVector4x4(const float matrix[16], const float vector[4], float result[4]) {
	result[0] = 	matrix[0]*vector[0] 	+ matrix[1]*vector[1] 	+ matrix[2]*vector[2] 	+ matrix[3]*vector[3];
	result[1] = 	matrix[4]*vector[0] 	+ matrix[5]*vector[1] 	+ matrix[6]*vector[2] 	+ matrix[7]*vector[3];
	result[2] = 	matrix[8]*vector[0] 	+ matrix[9]*vector[1] 	+ matrix[10]*vector[2] 	+ matrix[11]*vector[3];
	result[3] = 	matrix[12]*vector[0] 	+ matrix[13]*vector[1] 	+ matrix[14]*vector[2] 	+ matrix[15]*vector[3];
}

bool MathUtils::invertMatrix4x4(float var[16], float result[16]) {
	// Calcule du déterminant
	float a11 = var[0], 	a12 = var[1], 	a13 = var[2],	a14 = var[3];
	float a21 = var[4], 	a22 = var[5], 	a23 = var[6],	a24 = var[7];
	float a31 = var[8], 	a32 = var[9], 	a33 = var[10], 	a34 = var[11];
	float a41 = var[12], 	a42 = var[13], 	a43 = var[14], 	a44 = var[15];

	float D11 = a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42;
	float D12 = a21*a33*a44 + a23*a34*a41 + a24*a31*a43 - a21*a34*a43 - a24*a33*a41 - a23*a31*a44;
	float D13 = a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a24*a32*a41 - a22*a31*a44;
	float D14 = a21*a32*a43 + a22*a33*a41 + a23*a31*a42 - a21*a33*a42 - a23*a32*a41 - a22*a31*a43;

	float determinant = a11*D11 - a12*D12 + a13*D13 - a14*D14;

	// Vérification de la bijectivité
	if(!determinant) {
		return false;
	}

	// Calcule de la matrice inverse
	result[0] =		+ (a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42) / determinant;
	result[4] =		- (a21*a33*a44 + a23*a34*a41 + a24*a31*a43 - a21*a34*a43 - a23*a31*a44 - a24*a33*a41) / determinant;
	result[8] =		+ (a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41) / determinant;
	result[12] =	- (a21*a32*a43 + a22*a33*a41 + a23*a31*a42 - a21*a33*a42 - a22*a31*a43 - a23*a32*a41) / determinant;

	result[1] =		- (a12*a33*a44 + a13*a34*a42 + a14*a32*a43 - a12*a34*a43 - a13*a32*a44 - a14*a33*a42) / determinant;
	result[5] =		+ (a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41) / determinant;
	result[9] =		- (a11*a32*a44 + a12*a34*a41 + a14*a31*a42 - a11*a34*a42 - a12*a31*a44 - a14*a32*a41) / determinant;
	result[13] =	+ (a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41) / determinant;

	result[2] =		+ (a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42) / determinant;
	result[6] =		- (a11*a23*a44 + a13*a24*a41 + a14*a21*a43 - a11*a24*a43 - a13*a21*a44 - a14*a23*a41) / determinant;
	result[10] =	+ (a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41) / determinant;
	result[14] =	- (a11*a22*a43 + a12*a23*a41 + a13*a21*a42 - a11*a23*a42 - a12*a21*a43 - a13*a22*a41) / determinant;

	result[3] =		- (a12*a23*a34 + a13*a24*a32 + a14*a22*a33 - a12*a24*a33 - a13*a22*a34 - a14*a23*a32) / determinant;
	result[7] =		+ (a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31) / determinant;
	result[11] =	- (a11*a22*a34 + a12*a24*a31 + a14*a21*a32 - a11*a24*a32 - a12*a21*a34 - a14*a22*a31) / determinant;
	result[15] =	+ (a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31) / determinant;

	return true;
}

} /* namespace jkt */
