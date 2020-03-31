
#include "includes.h"

#include "util/V3D.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"

#include "main/Projectil.h"

CProjectil::CProjectil( CPlayer *player ) {
	m_Player = player;
}

CProjectil::~CProjectil() {
}
