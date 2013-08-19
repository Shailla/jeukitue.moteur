
#include <iostream>

#include "main/RequeteProcess.h"

CRequeteProcess::CRequeteProcess()
{
	_bTakePicture = false;
	_mutex = SDL_CreateMutex();

	SDL_mutexP(_mutex);
	_bOpenMap = false;		// Aucune demande d'ouverture de MAP
	_openMapLocalEtape = OMLE_AUCUNE;		// Indique la présence d'une demande d'ouverture d'une MAP en mode de jeu local
	_openMapServerEtape = OMSE_AUCUNE;		// Indique la présence d'une demande d'ouverture d'une MAP en mode de jeu serveur
	_openMapClientEtape = OMCE_AUCUNE;		// Indique la présence d'une demande d'ouverture d'une MAP en mode de jeu client
	SDL_mutexV(_mutex);
}

CRequeteProcess::~CRequeteProcess()
{
	SDL_DestroyMutex(_mutex);
}

void CRequeteProcess::setOuvreMapLocal(const string& nomMAP ) {
	SDL_mutexP(_mutex);

	_openMapLocalEtape = OMLE_DEMANDE;		// Indique la présence d'une demande d'ouverture d'une MAP
	_MapAOuvrir = nomMAP;					// Nom de la MAP en question

	SDL_mutexV(_mutex);
}

void CRequeteProcess::setOuvreMapServer(const string& nomMAP ) {
	SDL_mutexP(_mutex);

	_openMapServerEtape = OMSE_DEMANDE;		// Indique la présence d'une demande d'ouverture d'une MAP
	_MapAOuvrir = nomMAP;					// Nom de la MAP en question

	SDL_mutexV(_mutex);
}

void CRequeteProcess::setOuvreMapClient(const string& nomMAP ) {
	SDL_mutexP(_mutex);

	_openMapClientEtape = OMCE_DEMANDE;		// Indique la présence d'une demande d'ouverture d'une MAP
	_bOpenMap = true;						// Indique la présence d'une demande d'ouverture d'une MAP
	_MapAOuvrir = nomMAP;					// Nom de la MAP en question

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

void CRequeteProcess::setOuvreMapLocaleEtape(OUVRE_MAP_LOCALE_ETAPES etape) {
	SDL_mutexP(_mutex);
	_openMapLocalEtape = etape;
	SDL_mutexV(_mutex);
}

void CRequeteProcess::setOuvreMapServerEtape(OUVRE_MAP_SERVER_ETAPES etape) {
	SDL_mutexP(_mutex);
	_openMapServerEtape = etape;
	SDL_mutexV(_mutex);
}

void CRequeteProcess::setOuvreMapClientEtape(OUVRE_MAP_CLIENT_ETAPES etape) {
	SDL_mutexP(_mutex);
	_openMapClientEtape = etape;
	SDL_mutexV(_mutex);
}

int CRequeteProcess::getOuvreMapLocaleEtape() {
	SDL_mutexP(_mutex);
	OUVRE_MAP_LOCALE_ETAPES etape = _openMapLocalEtape;
	SDL_mutexV(_mutex);

	return etape;
}

int CRequeteProcess::getOuvreMapServerEtape() {
	SDL_mutexP(_mutex);
	OUVRE_MAP_SERVER_ETAPES etape = _openMapServerEtape;
	SDL_mutexV(_mutex);

	return etape;
}

int CRequeteProcess::getOuvreMapClientEtape() {
	SDL_mutexP(_mutex);
	OUVRE_MAP_CLIENT_ETAPES etape = _openMapClientEtape;
	SDL_mutexV(_mutex);

	return etape;
}

void CRequeteProcess::setTakePicture() {
	_bTakePicture = true;
}

bool CRequeteProcess::isTakePicture() {
	bool result = false;

	SDL_mutexP(_mutex);
	if( _bTakePicture ) {
		_bTakePicture = false;
		result = true;
	}
	SDL_mutexV(_mutex);

	return result;
}
