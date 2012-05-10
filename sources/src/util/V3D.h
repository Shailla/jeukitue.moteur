
#ifndef __JKT__V3D_H
#define __JKT__V3D_H

namespace JktUtils
{

class CV3D
{
public:
	float X;
	float Y;
	float Z;

		// Constructeurs
	CV3D();
	CV3D(const float x, const float y, const float z);
	CV3D(const float b[3]);

		// Opérateurs
	void operator=(CV3D b);				// Affectation
	bool operator!=(CV3D b);				// Comparaison
	bool operator==(CV3D b);				// Comparaison
	CV3D operator*(CV3D b);				// Multiplication vectorielle
	float operator^(CV3D b);				// Multiplication scalaire
	CV3D operator *(float sca);			// Multiplication par un scalaire
	CV3D operator+(CV3D b);				// Addition vectorielle
	CV3D operator+();						// 
	CV3D operator -(const float *b);
	CV3D operator-(CV3D b);				// Soustraction vectorielle
	CV3D operator-();						//
	void operator+=(CV3D b);
	void operator-=(CV3D b);

		// Opérateurs hybride fonctionnant avec les vecteurs de la forme 'float[3]'
	void operator =(const float[3]);
	void operator-(float *b);				// Soustraction vectorielle

		// Fonctions
	void get(float vec[3]);	// Revoie x, y et z sous forme langage C
	float norme();				// Calcule la norme du vecteur
	CV3D directeur();			// Renvoie un veteur normalisé de celui-ci
	void setNull();				// Donne une valeur nulle au vecteur
};

}	// JktUtils

#endif 

