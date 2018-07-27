
#include <iostream>
#include <string>
#include <set>

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"
using namespace boost::filesystem;

#include "util/Erreur.h"
#include "util/StringUtils.h"
#include "main/Cfg.h"
#include "util/Trace.h"
#include "util/FindFolder.h"
#include "spatial/Map.h"
#include "util/FindFolder.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/FichierASE.h"
#include "menu/ConsoleView.h"
#include "ressource/RessourcesLoader.h"

#include "spatial/AseImporter.h"

extern CCfg Config;

using namespace std;
using namespace jkt;

namespace jkt
{

void AseImporter::lanceImportAse(const string& aseFilename, ConsoleView* console)  {
	void** arg = new void*[2];

	arg[0] = StringUtils::toChars(aseFilename);
	arg[1] = console;

	SDL_CreateThread(importAse, arg);
}

int AseImporter::importAse(void* arg) {
	// R�cup�ration et d�sallocation des param�tres du thread
	string aseFileName = (char*)((void**)arg)[0];
	ConsoleView* console = (ConsoleView*)((void**)arg)[1];

	delete[] (char*)((void**)arg)[0];
	delete[] (void**)arg;


	CMap* pMapASE = NULL;

	try {
		path mapDir = string(MAP_DIRECTORY) + aseFileName;
		console->println(ConsoleView::ConsoleOutputType::COT_INFO, "Import ASE : Lancement...");


		/* ************************* */
		/* Import du fichier Map     */
		/* ************************* */

		console->println(ConsoleView::ConsoleOutputType::COT_INFO, "Import ASE : Suppression du r\u00e9pertoire existant...");
		remove_all(mapDir);

		if(exists(mapDir)) {
			string msg("Suppression du r\u00e9pertoire '");
			msg += mapDir.string();
			msg += "' impossible";
			LOGINFO((msg));
			throw CErreur(msg);
		}


		/* ************************* */
		/* Import du fichier Map     */
		/* ************************* */

		// Conversion fichier ASE -> fichier Map
		pMapASE = new CMap(0);

		if(	!CFichierASE::LitFichierASE(aseFileName, pMapASE, Config.Debug.bAfficheFichier)) {
			LOGINFO(("Erreur : Lecture du fichier ASE impossible ou fichier corrompu" ));
			throw CErreur("Lecture du fichier ASE impossible ou fichier corrompu");
		}

		// Ajustements spatiaux
		console->println(ConsoleView::ConsoleOutputType::COT_INFO, "Import ASE : Ajustement dimensions / orientation...");
		pMapASE->echangeYZ();						// Inversion des coordonn�es Y et Z
		pMapASE->scale( -1.0f, 1.0f, 1.0f );

		pMapASE->init();

		// Enregistre fichier Map
		console->println(ConsoleView::ConsoleOutputType::COT_INFO, "Import ASE : Enregistrement fichier Map...");
		pMapASE->Save(aseFileName.c_str());


		/* ************************* */
		/* Import des textures       */
		/* ************************* */

		// Cr�e r�pertoire de la Map
		if(!create_directory(mapDir)) {
			string msg("Cr\u00e9ation du r\u00e9pertoire '");
			msg += mapDir.string();
			msg += "' impossible";
			LOGINFO((msg));
			throw CErreur(msg);
		}

		// Collecte la liste des textures
		CMaterial *mat;
		vector<CMaterial*>::iterator iter;

		std::set<string> texturesToCopy;
		string texture;

		for( iter=pMapASE->_materials.begin() ; iter!=pMapASE->_materials.end() ; iter++ ) {
			mat = *iter;

			if( mat->Type()==CMaterial::MAT_TYPE_TEXTURE ) {
				CMaterialTexture *matRef = (CMaterialTexture*)mat;

				texture = matRef->m_FichierTexture;
				jkt::RessourcesLoader::getFileRessource(texture);
				texturesToCopy.insert(texture);
			}
			else if( mat->Type()==CMaterial::MAT_TYPE_MULTI ) {
				CMaterialMulti *matMulti = (CMaterialMulti *)mat;

				for( int i=0 ; i<matMulti->NbrTex() ; i++ ) {
					if( matMulti->m_TabMat[i]->Type()==CMaterial::MAT_TYPE_TEXTURE ) {
						texture = ((CMaterialTexture*)matMulti->m_TabMat[i])->m_FichierTexture;
						jkt::RessourcesLoader::getFileRessource(texture);
						texturesToCopy.insert(texture);
					}
				}
			}
		}

		// Importe les textures
		for(const string& texture : texturesToCopy) {
			copieTexture(texture, pMapASE, mapDir, console);
		}


		/* ************************* */
		/* Finalisation              */
		/* ************************* */

		console->println(ConsoleView::ConsoleOutputType::COT_INFO, "Import ASE : Lib\u00e9ration des ressources...");
		delete pMapASE;
		console->println(ConsoleView::ConsoleOutputType::COT_COMMAND_RESULT, "Import ASE : Import termin\u00e9 en succ\u00e8s.");
	}
	catch(const CErreur& erreur) {
		stringstream consoleMsg;
		consoleMsg << "Import ASE : " << erreur.getMessage();
		console->println(ConsoleView::ConsoleOutputType::COT_ERROR, consoleMsg.str());

		if(pMapASE != NULL) {
			delete pMapASE;
		}
	}

	return 0;
}

void AseImporter::copieTexture(const string& fromFile, CMap *pMapASE, const path &mapDir, ConsoleView* console) throw(CErreur) {
	stringstream consoleTexte;
	consoleTexte << "Import ASE : Copie texture '" << fromFile << "'...";
	console->println(ConsoleView::ConsoleOutputType::COT_INFO, consoleTexte.str());

	path from(fromFile);		// Fichier source pour la copie du fichier de texture
	path to = mapDir / from.leaf();

	LOGINFO(("Copie de '%s' vers '%s'", from.string().c_str(), to.string().c_str()));

	try {
		boost::filesystem::copy_file(from, to);
	}
	catch(const boost::filesystem::filesystem_error& exception) {
		LOGWARN(("Echec copie fichier texture '%s', erreur boost : '%s'", fromFile.c_str(), exception.what()));
		string msg = "Echec copie fichier texture '";
		msg += fromFile + "' '";
		msg += exception.what();
		msg += "'";
		throw CErreur(msg);
	}
}

}	// JktMoteur
