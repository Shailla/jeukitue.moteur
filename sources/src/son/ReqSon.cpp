

#ifdef WIN32				// Pour FMOD
#include <conio.h>			// Pour FMOD
#include <windows.h>		// Pour FMOD
#else						// Pour FMOD
#include "wincompat.h"		// Pour FMOD
#endif						// Pour FMOD
#include <vector>
#include <iostream>

#include "fmod.h"

#include "Son.h"
#include "ReqSon.h"

using namespace std;
using namespace jkt;

CReqSon::CReqSon(CSon* id) {
	_pause = false;			// Ne mets pas le son en pause
	_boucle = false;		// Ne joue pas le son en boucle
	_channel = -1;			// Pas de channel réservé
	_son = id;				// Associe la requéte au son demandé
	_volatil = false;		// Cette requête est non volatil (ne peut être supprimée que par l'appelant)
}

CReqSon::~CReqSon() {
	if(_channel != -1) {				// Si canal utilisé
		FSOUND_StopSound(_channel);		// Libère ce canal
	}
}

void CReqSon::pause(bool pause) {		// Met la requête en pause ou non
	_pause = pause;
}

void CReqSon::boucle(bool boucle) {	// Joue le son associé à la requête en boucle ou non
	_boucle = boucle;
}

void CReqSon::play() {
	if( _channel != -1 ) {
		FSOUND_PlaySound(_channel, _son->m_Sample);
	}
	else {
		_channel = FSOUND_PlaySound(FSOUND_FREE, _son->m_Sample);
	}
}

bool CReqSon::isPlaying() {
	return (_channel!=-1) && FSOUND_IsPlaying(_channel);
}

CReqSonMono::CReqSonMono(CSon *id) : CReqSon(id) {
}

CReqSonMono::~CReqSonMono() {
}

CReqSonStereo::CReqSonStereo(CSon *id) : CReqSon(id) {
}

CReqSonStereo::~CReqSonStereo() {
}

CReqSon3D::CReqSon3D(CSon *id) : CReqSon(id) {
	_position[0] = _position[1] = _position[2] = 0.0f;	// Position à l'origine
	_vitesse[0] = _vitesse[1] = _vitesse[2] = 0.0f;		// Vitesse nulle
	_doppler = false;									// Effet doppler désactivé
}

CReqSon3D::~CReqSon3D() {
}

void CReqSon3D::setPosition(const float pos[3]) {
	_position[0] = pos[0];
	_position[1] = pos[1];
	_position[2] = pos[2];
}
