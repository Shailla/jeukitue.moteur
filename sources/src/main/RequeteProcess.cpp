
#include <iostream>

#include "main/RequeteProcess.h"

CRequeteProcess::CRequeteProcess()
{
	m_bOpenMap = false;		// Aucune demande d'ouverture de MAP
}

CRequeteProcess::~CRequeteProcess()
{
}

void CRequeteProcess::setOuvreMapLocal( string nomMAP )
{
	m_bOpenMapLocal = true;		// Indique la présence d'une demande d'ouverture d'une MAP
	m_MapAOuvrir = nomMAP;	// Nom de la MAP en question
}

void CRequeteProcess::setOuvreMap( string nomMAP )
{
	m_bOpenMap = true;		// Indique la présence d'une demande d'ouverture d'une MAP
	m_MapAOuvrir = nomMAP;	// Nom de la MAP en question
}

string* CRequeteProcess::getOuvreMap()
{
	m_bOpenMap = false;
	m_bOpenMapLocal = false;

	string* nom = new string(m_MapAOuvrir);
	m_MapAOuvrir.clear();

	return nom;
}

bool CRequeteProcess::isOuvreMap()
{	return m_bOpenMap;			}

bool CRequeteProcess::isOuvreMapLocal()
{	return m_bOpenMapLocal;			}

void CRequeteProcess::setTakePicture()
{
	m_bTakePicture = true;
}

bool CRequeteProcess::isTakePicture()
{
	bool result = false;

	if( m_bTakePicture )
	{
		m_bTakePicture = false;
		result = true;
	}

	return result;
}
