
#include "util/V3D.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "Player.h"

#include "Projectil.h"

CProjectil::CProjectil( CPlayer *player )
{
	m_Player = player;
}

CProjectil::~CProjectil()
{
}
