
#ifndef __JKT__FLOCON_H
#define __JKT__FLOCON_H

#include "util/V3D.h"

using namespace JktUtils;

namespace JktMoteur
{

class Flocon {
public:
	enum FloconState {
		FALLING,
		ON_THE_GROUND
	};

	/** Position actuelle de la particule. */
	JktUtils::CV3D _position;
	/** Vitesse actuelle de la particule. */
	JktUtils::CV3D _vitesse;
	/** Accélération actuelle de la particule. */
	JktUtils::CV3D _acceleration;

	FloconState _state;

	// Constructeurs
	Flocon();

	void changeState(FloconState state);
};

}	// JktMoteur

#endif
