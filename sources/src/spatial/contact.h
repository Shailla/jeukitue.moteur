
#ifndef __CONTACT_H
#define __CONTACT_H

#include "util/TableauIndex.cpp"

namespace jkt
{

void contactPlayer(CPlayer *player, float *normal, float distanceW);
void contactSprite(CPlayer *sprite, float *normal, float distanceW);	// Fonction de rebond

}	// JktMoteur

#endif

