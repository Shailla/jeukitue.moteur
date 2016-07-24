/*
 * CheckPlayerInZone.h
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_
#define SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_

#include "spatial/Map.h"
#include "spatial/geo/Geo.h"

namespace JktMoteur
{

class CheckPlayerInZone : public CGeo {
public:
	CheckPlayerInZone(CMap *map);
	virtual ~CheckPlayerInZone();
};

}

#endif /* SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_ */
