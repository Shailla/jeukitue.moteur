
#include "main/RequeteProcess.h"

CRequeteProcess::CRequeteProcess()
{
	m_bOpenMap = false;		// Aucune demande d'ouverture de MAP
}

CRequeteProcess::~CRequeteProcess()
{
}

void CRequeteProcess::setOuvreMap( string nomMAP )
{
	m_bOpenMap = true;			// Indique la présence d'une demande d'ouverture d'une MAP
	m_MapAOuvrir = nomMAP;	// Nom de la MAP en question
}

string CRequeteProcess::getOuvreMap()
{
	string nom = m_MapAOuvrir;
	m_MapAOuvrir.clear();
	m_bOpenMap = false;

	return nom;
}

bool CRequeteProcess::isOuvreMap()
{	return m_bOpenMap;			}

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
