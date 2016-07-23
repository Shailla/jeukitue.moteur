
#include <iostream>
#include <string>

using namespace std;

#include "util/Erreur.h"
#include "util/StringUtils.h"
#include "main/Cfg.h"
#include "util/Trace.h"
#include "spatial/AseImporter.h"
#include "util/FindFolder.h"
#include "spatial/Map.h"
#include "util/FindFolder.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/FichierASE.h"
#include "menu/ConsoleAvancementView.h"

extern CCfg Config;

using namespace JktUtils;

namespace JktMoteur
{

bool AseImporter::supprimeRepertoire(const string& repName) {
	LOGINFO(("Suppression du répertoire : '%s'", repName.c_str()));

	return CFindFolder::rmdir(repName.c_str());
}

void AseImporter::lanceImportAse(const string& aseFilename, ConsoleAvancementView* console)  {
	void** arg = new void*[2];

	arg[0] = StringUtils::toChars(aseFilename);
	arg[1] = console;

	SDL_CreateThread(importAse, arg);
}

int AseImporter::importAse(void* arg) {
	// Récupération et désallocation des paramètres du thread
	string aseFileName = (char*)((void**)arg)[0];
	ConsoleAvancementView* console = (ConsoleAvancementView*)((void**)arg)[1];

	delete[] (char*)((void**)arg)[0];
	delete[] (void**)arg;


	CMap* pMapASE = NULL;

	try {
		console->addMsg("Lancement import ASE...");

		pMapASE = new CMap(0);		// Crée une classe pour recevoir les données de la map

		// Conversion fichier ASE -> fichier Map
		if(	!CFichierASE::LitFichierASE(aseFileName, pMapASE, Config.Debug.bAfficheFichier)) {
			LOGINFO(("Erreur : Lecture du fichier ASE impossible ou fichier corrompu 1" ));
			throw CErreur("Lecture du fichier ASE impossible ou fichier corrompu");
		}

		console->addMsg("Ajustement dimensions / orientation...");
		pMapASE->EchangeYZ();						// Inversion des coordonnées Y et Z
		pMapASE->Scale( -1.0f, 1.0f, 1.0f );

		// Transfert des fichiers de texture dans le répertoire du fichier Map
		string nomRep = string("./Map/") + aseFileName;

		console->addMsg("Enregistrement fichier Map...");
		pMapASE->Save(aseFileName.c_str());

		CMaterial *mat;
		vector<CMaterial*>::iterator iter;

		for( iter=pMapASE->m_TabMaterial.begin() ; iter!=pMapASE->m_TabMaterial.end() ; iter++ ) {
			mat = *iter;

			if( mat->Type()==CMaterial::MAT_TYPE_TEXTURE ) {
				CMaterialTexture *matRef = (CMaterialTexture*)mat;
				copieTexture(matRef, pMapASE, nomRep, console);
			}
			else if( mat->Type()==CMaterial::MAT_TYPE_MULTI ) {
				CMaterialMulti *matMulti = (CMaterialMulti *)mat;

				for( int i=0 ; i<matMulti->NbrTex() ; i++ ) {
					if( matMulti->m_TabMat[i]->Type()==CMaterial::MAT_TYPE_TEXTURE ) {
						copieTexture((CMaterialTexture*)matMulti->m_TabMat[i], pMapASE, nomRep, console);
					}
				}
			}
		}

		console->addMsg("Lib\u00e9ration des ressources...");
		delete pMapASE;
		console->addMsg("Import termin\u00e9.");
		console->enableOkButton();
	}
	catch(CErreur& erreur) {
		stringstream consoleMsg;
		consoleMsg << "Echec de l'import : " << erreur.what();
		console->addMsgError(consoleMsg.str());

		if(pMapASE != NULL) {
			delete pMapASE;
		}

		console->enableOkButton();
	}

	return 0;
}

void AseImporter::copieTexture( CMaterialTexture *mat,
								CMap *pMapASE,
								string &nomRep,
								ConsoleAvancementView* console) {
	basic_string <char>::size_type index, npos = (basic_string <char>::size_type)-1;
	char ch;

	LOGDEBUG(("copieTexture(mat=%x,pMapASE=%x,nomRep=%s)",	mat, pMapASE, nomRep.c_str() ));
	LOGDEBUG(("copieTexture() Fichier de texture=%s", mat->m_FichierTexture.c_str() ));

	fstream from;	// Fichier source pour la copie du fichier de texture
	fstream to;		// Fichier destination pour la copie du fichier de texture

	string& nom = mat->m_FichierTexture;

	stringstream consoleTexte;
	consoleTexte << "Enregistrement texture '" << nom << "'...";
	console->addMsg(consoleTexte.str());

	from.open( nom.c_str(), ios_base::binary|ios_base::in );	// Ouvre le fichier texture d'origine

	if( from.fail() ) {
		stringstream texte;
		texte << "Echec d'ouverture du fichier de texture '" << nom << "'";
		LOGINFO((texte.str().c_str() ));
		throw CErreur(texte);
	}
		// Récupération du nom du fichier sans son chemin
	index = nom.find_last_of( "/" );		// Recherche du dernier slach
	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien à faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'à lui

		// Récupération du nom du fichier sans son chemin
	index = nom.find_last_of( "\\" );		// Recherche du dernier anti-slach
	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien à faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'à lui

		// Nouveau nom avec chemin du fichier de texture
	nom = nomRep + "/" + nom;

	to.open( nom.c_str(), ios_base::binary|ios_base::out );	// Ouvre fichier destination texture
	if( to.fail() ) {
		stringstream texte;
		texte << "Echec de cr\u00e9ation du fichier de texture '" << nom << "'";
		LOGINFO((texte.str().c_str() ));
		throw CErreur(texte);
	}

		// Copie du fichier proprement dite
	while( from.get( ch ) )
		to.put( ch );

		// Vérifie si la copie s'est bien passé
	if( !from.eof() ) {
		stringstream texte;
		texte << "Echec de copie du fichier de texture '" << nom << "'";
		LOGINFO((texte.str().c_str() ));
		throw CErreur(texte);
	}
}

}	// JktMoteur
