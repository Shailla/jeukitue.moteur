
#include <cmath>
#include <iostream>

using namespace std;

#include "V3D.h"

namespace JktUtils
{

CV3D::CV3D()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

CV3D::CV3D(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

CV3D::CV3D( const float b[3] )
{
	X = b[0];
	Y = b[1];
	Z = b[2];
}

bool CV3D::operator ==( CV3D b )
{
	if( X==b.X )
		if( Y==b.Y )
			if( Z==b.Z )
				return true;
	
	return false;
}

bool CV3D::operator !=( CV3D b )
{
	if( X!=b.X )
		return true;
	
	if( Y!=b.Y )
		return true;
	
	if( Z!=b.Z )
		return true;

	return false;
}

void CV3D::operator =( CV3D b )
{
	X = b.X;
	Y = b.Y;
	Z = b.Z;
}

CV3D CV3D::operator *( CV3D b )	// Multiplication vectorielle
{
	return CV3D( (Y*b.Z)-(Z*b.Y), (Z*b.X)-(X*b.Z), (X*b.Y)-(Y*b.X) );
}

float CV3D::operator ^( CV3D b )	// Multiplication scalaire
{
	return (X*b.X) + (Y*b.Y) + (Z*b.Z);
}

CV3D CV3D::operator *( float sca )	// Multiplication par un scalaire
{
	return CV3D( sca*X, sca*Y, sca*Z );
}

CV3D CV3D::operator +( CV3D b )
{
	return CV3D( X+b.X, Y+b.Y, Z+b.Z );
}

CV3D CV3D::operator -( CV3D b )
{
	return CV3D( X-b.X, Y-b.Y, Z-b.Z );
}

CV3D CV3D::operator -( const float *b )
{
	return CV3D( X-b[0], Y-b[1], Z-b[2] );
}

void CV3D::operator +=( CV3D b )
{
	X += b.X;
	Y += b.Y;
	Z += b.Z;
}

CV3D CV3D::operator +()
{
	return CV3D( +X, +Y, +Z );
}

void CV3D::operator -=( CV3D b )
{
	X -= b.X;
	Y -= b.Y;
	Z -= b.Z;
}

void CV3D::operator -( float *b )
{
	X -= b[ 0 ];
	Y -= b[ 1 ];
	Z -= b[ 2 ];
}

CV3D CV3D::operator -()
{
	return CV3D( -X, -Y, -Z );
}

void CV3D::operator =( const float b[3] )
{
	X = b[ 0 ];
	Y = b[ 1 ];
	Z = b[ 2 ];
}

float CV3D::norme()
{
	return sqrtf( (X*X) + (Y*Y) + (Z*Z) );
}

CV3D CV3D::directeur()
{
	float n = norme();
	if( n==0.0f )
		cerr << "ATTENTION, DIVISION PAR 0 (math3D.cpp)\n";
	
	return CV3D( X/n, Y/n, Z/n );
}

void CV3D::setNull()
{
	X = Y = Z = 0.0f;
}

void CV3D::get( float vec[3] )
{
	vec[0] = X;
	vec[1] = Y;
	vec[2] = Z;
}

}	// JktUtils
