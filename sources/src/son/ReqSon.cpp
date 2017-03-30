

#ifdef WIN32				// Pour FMOD
  #include <conio.h>		// Pour FMOD
  #include <windows.h>		// Pour FMOD
#else						// Pour FMOD
  #include "wincompat.h"	// Pour FMOD
#endif						// Pour FMOD
#include <vector>
#include <iostream>

#include "fmod.h"

#include "Son.h"
#include "ReqSon.h"

using namespace std;
using namespace jkt;

CReqSon::CReqSon( CSon *id )
{
	m_bPause = false;		// Ne mets pas le son en pause
	m_bBoucle = false;		// Ne joue pas le son en boucle
	channel = -1;			// Pas de channel r�serv�
	m_IdSon = id;			// Associe la requ�te au son demand�
	m_bVolatil = false;		// Cette requ�te est non volatil (ne peut �tre supprim�e que par l'appelant)
}

CReqSon::~CReqSon()
{
	if( channel!=-1 )						// Si canal utilis�
		FSOUND_StopSound( channel );		// Lib�re ce canal
}

void CReqSon::Pause( bool indic )		// Met la requ�te en pause ou non
{	m_bPause = indic;		}

void CReqSon::Boucle( bool indic )	// Joue le son associ� � la requ�te en boucle ou non
{	m_bBoucle = indic;	}

void CReqSon::Play()
{
	if( channel != -1 )
		FSOUND_PlaySound( channel, m_IdSon->m_Sample );
	else
		channel = FSOUND_PlaySound( FSOUND_FREE, m_IdSon->m_Sample );
}

bool CReqSon::IsPlaying()
{
	if( channel!=-1 )
		if( FSOUND_IsPlaying( channel ) )
			return true;
	
	return false;
}

CReqSonMono::CReqSonMono( CSon *id )
	:CReqSon( id )
{
}

CReqSonMono::~CReqSonMono()
{
}

CReqSonStereo::CReqSonStereo( CSon *id )
	:CReqSon( id )
{
}

CReqSonStereo::~CReqSonStereo()
{
}

CReqSon3D::CReqSon3D( CSon *id )
	:CReqSon( id )
{
	m_Position[0] = m_Position[1] = m_Position[2] = 0.0f;	// Opsition � l'origine
	m_Vitesse[0] = m_Vitesse[1] = m_Vitesse[2] = 0.0f;		// Vitesse nulle
	m_bDoppler = false;										// Effet doppler d�sactiv�
}

CReqSon3D::~CReqSon3D()
{
}

void CReqSon3D::SetPosition(float pos[3])
{
	m_Position[0] = pos[0];
	m_Position[1] = pos[1];
	m_Position[2] = pos[2];
}
