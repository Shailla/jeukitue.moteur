
#ifndef __JKT__REQUETEPROCESS_H
#define __JKT__REQUETEPROCESS_H

#include <string>
using namespace std;

class CRequeteProcess
{
	bool m_bOpenMap;		// Indique une demande d'ouvrir la MAP nommée dans m_MapAOuvrir
	string m_MapAOuvrir;	// Indique le nom du fichier Map à ouvrir

	bool m_bTakePicture;	// Indique une demande de prise de photo de la scène
public:
	CRequeteProcess();
	~CRequeteProcess();

	void setOuvreMap( string nomMAP );	// Inscrit une demande d'ouverture de MAP
	bool isOuvreMap();					// Intéroge sur la présence d'une demande d'ouverture de MAP
	string getOuvreMap();				// Renvoie le nom de la MAP à ouvrir et détruit la demande

	void setTakePicture();				// Inscrit une demande de prise de photo de la scène
	bool isTakePicture();				// Intéroge sur la présence d'un demande de prise de photo
};

#endif

