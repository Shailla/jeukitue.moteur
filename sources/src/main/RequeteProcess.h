
#ifndef __JKT__REQUETEPROCESS_H
#define __JKT__REQUETEPROCESS_H

#include <string>

#include <SDL.h>

class CRequeteProcess
{
public:
	enum OUVRE_MAP_LOCALE_ETAPES {
		OMLE_AUCUNE,					// Aucune ouverture de MAP locale n'est en cours
		OMLE_DEMANDE,					// L'ouverture d'une MAP locale a été demandée ==> Fermeture MAP en cours et lancement ouverture de la nouvelle MAP
		OMLE_OUVERTURE_EN_COURS,		// Chargement en cours des données de la nouvelle MAP
		OMLE_OUVERTURE					// Finalisation ouverture de la nouvelle MAP
	};

	enum OUVRE_MAP_SERVER_ETAPES {
		OMSE_AUCUNE,					// Aucune ouverture de MAP serveur n'est en cours
		OMSE_DEMANDE,					// L'ouverture d'une MAP serveur a été demandée ==> Fermeture MAP en cours et lancement ouverture de la nouvelle MAP
		OMSE_OUVERTURE_EN_COURS,		// Chargement en cours des données de la nouvelle MAP
		OMSE_OUVERTURE					// Finalisation ouverture de la nouvelle MAP
	};

	enum OUVRE_MAP_CLIENT_ETAPES {
		OMCE_AUCUNE,					// Aucune ouverture de MAP serveur n'est en cours
		OMCE_DEMANDE,					// L'ouverture d'une MAP serveur a été demandée ==> Fermeture MAP en cours et lancement ouverture de la nouvelle MAP
		OMCE_OUVERTURE_EN_COURS,		// Chargement en cours des données de la nouvelle MAP
		OMCE_OUVERTURE					// Finalisation ouverture de la nouvelle MAP
	};

private:
	bool _bOpenMap;									// Indique une demande d'ouvrir la MAP nommée dans _MapAOuvrir
	OUVRE_MAP_LOCALE_ETAPES _openMapLocalEtape;		// Indique une demande d'ouvrir la MAP nommée dans _MapAOuvrir
	OUVRE_MAP_SERVER_ETAPES _openMapServerEtape;	// Indique une demande d'ouvrir la MAP nommée dans _MapAOuvrir
	OUVRE_MAP_CLIENT_ETAPES _openMapClientEtape;	// Indique une demande d'ouvrir la MAP nommée dans _MapAOuvrir
	std::string _MapAOuvrir;						// Indique le nom du fichier Map à ouvrir
	SDL_mutex *_mutex;
	bool _bTakePicture;								// Indique une demande de prise de photo de la scène

public:
	CRequeteProcess();
	~CRequeteProcess();

	void setOuvreMapLocal(const std::string& nomMAP );				// Inscrit une demande d'ouverture de MAP en mode de jeu local
	void setOuvreMapClient(const std::string& nomMAP );				// Inscrit une demande d'ouverture de MAP en mode de jeu client
	void setOuvreMapServer(const std::string& nomMAP );				// Inscrit une demande d'ouverture de MAP en mode de jeu serveur

	void setOuvreMapLocaleEtape(OUVRE_MAP_LOCALE_ETAPES etape);
	void setOuvreMapServerEtape(OUVRE_MAP_SERVER_ETAPES etape);
	void setOuvreMapClientEtape(OUVRE_MAP_CLIENT_ETAPES etape);

	int getOuvreMapLocaleEtape();									// Int�roge sur la pr�sence d'une demande d'ouverture de MAP en jeu local
	int getOuvreMapClientEtape();									// Int�roge sur la pr�sence d'une demande d'ouverture de MAP en jeu client
	int getOuvreMapServerEtape();									// Int�roge sur la pr�sence d'une demande d'ouverture de MAP en jeu serveur

	std::string getOuvreMap();										// Renvoie le nom de la MAP à ouvrir et détruit la demande

	void setTakePicture();											// Inscrit une demande de prise de photo de la scène
	bool isTakePicture();											// Interroge sur la présence d'un demande de prise de photo
};

#endif

