
#ifndef __JKT__MATH_VECTORIEL_H
#define __JKT__MATH_VECTORIEL_H

namespace jkt
{

float produitScalaire(const float *A, const float *B);
float norme(const float *A);
void normalise(float *A);
float normeCarre(const float *A);
void scale(const float *A, const float factor, float *result);
void scale(float *A, const float factor);
void produitVectoriel(const float *A, const float *B, float *result);			// C = A*B
void produitMatriciel(const float *row, const float *A, float *result);		// B = row * A

}	// JktUtils

#endif

