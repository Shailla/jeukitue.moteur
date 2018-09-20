
#include <string>
#include <fstream>
#include <iostream>

#include "util/GenRef.h"
#include "spatial/IfstreamMap.h"

#include <spatial/basic/Geometrical.h>

using namespace std;

namespace jkt
{

Geometrical::Geometrical() {
	_minX = _maxX = _minY = _maxY = _minZ = _maxZ = 0.0f;
	_rayon = 0.0f;
}

Geometrical::Geometrical(const Geometrical& other) {
	// Bulle
	_minX = other._minX;
	_minY = other._minY;
	_minZ = other._minZ;
	_maxX = other._maxX;
	_maxY = other._maxY;
	_maxZ = other._maxZ;

	// Centre
	_centre[0] = other._centre[0];
	_centre[1] = other._centre[1];
	_centre[2] = other._centre[2];

	// Rayon
	_rayon = other._rayon;
}

Geometrical::~Geometrical() {
}

}	// JktMoteur
