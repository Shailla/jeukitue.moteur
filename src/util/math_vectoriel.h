
#ifndef __JKT__MATH_VECTORIEL_H
#define __JKT__MATH_VECTORIEL_H

namespace JKT_PACKAGE_UTILS
{

float produitScalaire(const float *A, const float *B);
float norme(float *A);
void normalise(float *A);
void produitVectoriel(const float *A, const float *B, float *C);			// C = A*B
void produitMatriciel(const float *row, const float *A, float *B);		// B = row * A

}	// JKT_PACKAGE_UTILS

#endif

