
#ifndef __JKT__FLOCON_H
#define __JKT__FLOCON_H

#include "util/V3D.h"

using namespace jkt;

namespace jkt
{

class Flocon {
public:
	enum FloconState {
		FALLING,
		ON_THE_GROUND
	};

	/** Position actuelle de la particule. */
	jkt::CV3D _position;
	/** Vitesse actuelle de la particule. */
	jkt::CV3D _vitesse;
	/** Acc�l�ration actuelle de la particule. */
	jkt::CV3D _acceleration;

	FloconState _state;

	// Constructeurs
	Flocon();

	void changeState(FloconState state);
};

}	// JktMoteur

#endif