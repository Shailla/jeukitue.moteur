
#include <iostream>

#include "SDL.h"

#include "util/V3D.h"

#include "spatial/moteur/neige/mieux/Flocon.h"

namespace JktMoteur
{

Flocon::Flocon() {
	_position.X = _position.Y = _position.Z = 0.0f;
	_vitesse.X = _vitesse.Y = _vitesse.Z = 0.0f;
	_acceleration.X = _acceleration.Y = _acceleration.Z = 0.0f;

	_state = FloconState::FALLING;
}

void Flocon::changeState(FloconState state) {
		_state = state;
}

}	// JktMoteur
