
#ifndef __CONTACT_H
#define __CONTACT_H

#include "TableauIndex.cpp"

namespace JKT_PACKAGE_MOTEUR3D
{

void contactPlayer(CPlayer *player, float *normal, float distanceW);
void contactSprite(CPlayer *sprite, float *normal, float distanceW);	//fonction de rebond sur

}	// JKT_PACKAGE_MOTEUR3D

#endif

