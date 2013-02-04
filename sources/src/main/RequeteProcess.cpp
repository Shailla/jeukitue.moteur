
#include <iostream>

#include "main/RequeteProcess.h"

CRequeteProcess::CRequeteProcess()
{
	_bTakePicture = false;
	_mutex = SDL_CreateMutex();

	SDL_mutexP(_mutex);
	_bOpenMap = false;		// Aucune demande d'ouverture de MAP
	_openMapLocalEtape = OMLE_AUCUNE;		// Indique la présence d'une demande d'ouverture d'une MAP
	SDL_mutexV(_mutex);
}

CRequeteProcess::~CRequeteProcess()
{
	SDL_DestroyMutex(_mutex);
}

void CRequeteProcess::setOuvreMapLocal( string nomMAP ) {
	SDL_mutexP(_mutex);
	_openMapLocalEtape = OMLE_DEMANDE;		// Indique la présence d'une demande d'ouverture d'une MAP
	_MapAOuvrir = nomMAP;	// Nom de la MAP en question
	SDL_mutexV(_mutex);
}

void CRequeteProcess::setOuvreMap( string nomMAP ) {
	SDL_mutexP(_mutex);
	_bOpenMap = true;		// Indique la présence d'une demande d'ouverture d'une MAP
	_MapAOuvrir = nomMAP;	// Nom de la MAP en question
	SDL_mutexV(_mutex);
}

string CRequeteProcess::getOuvreMap() {
	SDL_mutexP(_mutex);
	string map = string(_MapAOuvrir);

	_bOpenMap = false;
	_MapAOuvrir.clear();
	SDL_mutexV(_mutex);

	return map;
}

bool CRequeteProcess::isOuvreMap()
{	return _bOpenMap;			}

void CRequeteProcess::setOuvreMapLocaleEtape(OUVRE_MAP_LOCALE_ETAPES etape) {
	SDL_mutexP(_mutex);
	_openMapLocalEtape = etape;
	SDL_mutexV(_mutex);
}

int CRequeteProcess::getOuvreMapLocaleEtape()
{
	SDL_mutexP(_mutex);
	OUVRE_MAP_LOCALE_ETAPES etape = _openMapLocalEtape;
	SDL_mutexV(_mutex);

	return etape;
}

void CRequeteProcess::setTakePicture()
{
	_bTakePicture = true;
}

bool CRequeteProcess::isTakePicture()
{
	bool result = false;

	SDL_mutexP(_mutex);
	if( _bTakePicture ) {
		_bTakePicture = false;
		result = true;
	}
	SDL_mutexV(_mutex);

	return result;
}
