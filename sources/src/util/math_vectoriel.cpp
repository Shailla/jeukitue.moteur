
#include <cmath>
#include <iostream>

#include "math_vectoriel.h"

namespace jkt
{

float produitScalaire(const float *A, const float *B) {
	return A[0]*B[0] + A[1]*B[1] + A[2]*B[2];
}

float norme(float *A) {
	return sqrtf((A[0]*A[0]) + (A[1]*A[1]) + (A[2]*A[2]) );
}

void normalise(float *A) {
	float n = norme(A);

	A[0] /= n;
	A[1] /= n;
	A[2] /= n;
}

void produitVectoriel(const float *A, const float *B, float *C) {		//C = A*B
	C[0] = A[1]*B[2]-A[2]*B[1];
	C[1] = A[2]*B[0]-A[0]*B[2];
	C[2] = A[0]*B[1]-A[1]*B[0];
}

void produitMatriciel(const float *row, const float *A, float *B) {		// B = row * A
	B[0] = (row[0]*A[0]) + (row[3]*A[1]) + (row[6]*A[2]);
	B[1] = (row[1]*A[0]) + (row[4]*A[1]) + (row[7]*A[2]);
	B[2] = (row[2]*A[0]) + (row[5]*A[1]) + (row[8]*A[2]);
}

}	// JktUtils
