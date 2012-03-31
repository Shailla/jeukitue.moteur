
#include "util/V3D.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"

#include "main/Projectil.h"

CProjectil::CProjectil( CPlayer *player )
{
	m_Player = player;
}

CProjectil::~CProjectil()
{
}
