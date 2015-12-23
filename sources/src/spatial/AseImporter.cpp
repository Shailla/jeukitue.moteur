
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

bool AseImporter::supprimeRepertoire(const string& repName)
{
	cout << "\n\nPATH\t : " << repName;
	string var;
//	struct _finddata_t fileinfo;
//	intptr_t hFile;
	string fichier;

	CFindFolder folder(repName.c_str(), 0, 0);

	while(folder.findNext(fichier)) {
		if(!deleteOnlyFiles( fichier, repName)) {
			stringstream texte;
			texte << "Erreur deleteOnlyFiles(" << fichier << "," << repName << ")";
			LOGDEBUG((texte.str().c_str() ));
			cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
			throw CErreur(texte);
		}
	}

		// Effacement de tous les fichiers du r�pertoire
/*	var = path + "*.*";
	hFile = _findfirst( var.c_str(), &fileinfo );
	if( hFile!=-1L )
	{
		if( !deleteOnlyFiles( fileinfo, path ) )
			return false;

		while( _findnext( hFile, &fileinfo )!=-1L )
		{
			if( !deleteOnlyFiles( fileinfo, path ) )
				return false;
		}
	}*/

	folder.reset();

	while(folder.findNext(fichier)) {
		if((fichier!="..") && (fichier!=".")) {	// Si ni "..", ni "."
			var = repName + fichier;
			var += '/';

			if(!delDirectory(var)) {
				stringstream texte;
				texte << "Erreur delDirectory(" << var << ")" << endl;
				LOGDEBUG((texte.str().c_str() ));
				cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
				throw CErreur(texte);
			}

			if(!CFindFolder::chmod( var.c_str(), true, true)) {
				stringstream texte;
				texte << "Erreur CFindFolder::chmod(" << repName << ")" << endl;
				LOGDEBUG((texte.str().c_str() ));
				cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
				throw CErreur(texte);
			}

			if( CFindFolder::rmdir( var.c_str() ) ) {
				stringstream texte;
				texte << "Erreur CFindFolder::rmdir( " << var << ")";
				LOGDEBUG((texte.str().c_str() ));
				cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
				throw CErreur(texte);
			}
		}
	}

	return true;
}

bool AseImporter::delDirectory(const string &path) {
	cout << "\n\nPATH\t : " << path;
	string var;
//	struct _finddata_t fileinfo;
//	intptr_t hFile;
	string fichier;

	CFindFolder folder( path.c_str(), 0, 0 );
	while( folder.findNext(fichier) )
	{
		if( !deleteOnlyFiles( fichier, path ) )
		{
			stringstream texte;
			texte << "Erreur deleteOnlyFiles(" << fichier << "," << path << ")";
			LOGDEBUG((texte.str().c_str() ));
			cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
			throw CErreur(texte);
		}
	}

		// Effacement de tous les fichiers du r�pertoire
/*	var = path + "*.*";
	hFile = _findfirst( var.c_str(), &fileinfo );
	if( hFile!=-1L )
	{
		if( !deleteOnlyFiles( fileinfo, path ) )
			return false;

		while( _findnext( hFile, &fileinfo )!=-1L )
		{
			if( !deleteOnlyFiles( fileinfo, path ) )
				return false;
		}
	}*/

	folder.reset();
	while( folder.findNext(fichier) )
	{
		if( (fichier!="..") && (fichier!=".") )	// Si ni "..", ni "."
		{
			var = path + fichier;
			var += '/';

			if(!delDirectory( var )) {
				stringstream texte;
				texte << "Erreur delDirectory(" << var << ")";
				LOGDEBUG((texte.str().c_str() ));
				cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
				throw CErreur(texte);
			}

			if(!CFindFolder::chmod( var.c_str(), true, true )) {
				stringstream texte;
				texte << endl << "MOpenASE::delDirectory(" << path << ")" << endl;
				LOGDEBUG((texte.str().c_str() ));
				cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
				throw CErreur(texte);
			}

			if(CFindFolder::rmdir(var.c_str())) {
				stringstream texte;
				texte << "\nErreur _rmdir : " << var;
				LOGDEBUG((texte.str().c_str() ));
				cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
				throw CErreur(texte);
			}
		}
	}

	return true;
}

bool AseImporter::deleteOnlyFiles(string &fichier, const string& path )
{
	string var;
	if( (fichier!="..") && (fichier!=".") )	{	// Si ni "..", ni "."
		var = path + fichier;
		if(!CFindFolder::isFolder(var))	{	// Si ce n'est pas un r�pertoire (=>fichier)
			cout << "\nDeleting file : " << var;

			if(remove( var.c_str())) {
				stringstream texte;
				texte << "Erreur remove(" << var << ")";
				LOGDEBUG((texte.str().c_str() ));
				cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str();
				throw CErreur(texte);
			}
		}
	}

	return true;
}

void AseImporter::lanceImportAse(const string& aseFilename, ConsoleAvancementView* console) 
{
	void** arg = new void*[2];

	arg[0] = StringUtils::toChars(aseFilename);
	arg[1] = console;

	SDL_CreateThread(importAse, arg);
}

int AseImporter::importAse(void* arg)
{
	// R�cup�ration et d�sallocation des param�tres du thread
	string aseFileName = (char*)((void**)arg)[0];
	ConsoleAvancementView* console = (ConsoleAvancementView*)((void**)arg)[1];

	delete[] (char*)((void**)arg)[0];
	delete[] (void**)arg;


	CMap* pMapASE = NULL;

	try {
		console->addMsg("Lancement import ASE...");

		pMapASE = new CMap(0);		// Cr�e une classe pour recevoir les donn�es de la map

			// Conversion fichier ASE -> fichier Map
		if(	!CFichierASE::LitFichierASE(aseFileName, pMapASE, Config.Debug.bAfficheFichier))	// Lit le fichier ASE de la map
		{
			LOGDEBUG(("Erreur : Lecture du fichier ASE impossible ou fichier corrompu 1" ));
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur : Lecture du fichier ASE impossible ou fichier corrompu 1";
			throw CErreur("Erreur : Lecture du fichier ASE impossible ou fichier corrompu 1");
		}

		console->addMsg("Ajustement dimensions / orientation...");
		pMapASE->EchangeYZ();						// Inversion des coordonn�es Y et Z
		pMapASE->Scale( -1.0f, 1.0f, 1.0f );

			// Transfert des fichiers de texture dans le r�pertoire du fichier Map
		string nomRep = string("./Map/") + aseFileName;

		console->addMsg("Enregistrement...");
		pMapASE->Save(aseFileName.c_str());

		CMaterial *mat;
		vector<CMaterial*>::iterator iter;
		for( iter=pMapASE->m_TabMaterial.begin() ; iter!=pMapASE->m_TabMaterial.end() ; iter++ )
		{
			mat = *iter;
			if( mat->Type()==CMaterial::MAT_TYPE_TEXTURE )
			{
				CMaterialTexture *matRef = (CMaterialTexture*)mat;
				if( !copieTexture( matRef, pMapASE, nomRep, console ) )
				{
					LOGDEBUG(("Erreur a la copie d'un fichier de texture" ));
					cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur a la copie d'un fichier de texture";
					throw CErreur("Erreur a la copie d'un fichier de texture");
				}
			}
			else if( mat->Type()==CMaterial::MAT_TYPE_MULTI )
			{
				CMaterialMulti *matMulti = (CMaterialMulti *)mat;
				for( int i=0 ; i<matMulti->NbrTex() ; i++ )
				{
					if( matMulti->m_TabMat[i]->Type()==CMaterial::MAT_TYPE_TEXTURE )
					{
						if( !copieTexture( (CMaterialTexture*)matMulti->m_TabMat[i], pMapASE, nomRep, console ) )
						{
							LOGDEBUG(("Erreur a la copie d'un fichier de texture" ));
							cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur a la copie d'un fichier de texture";
							throw CErreur("Erreur a la copie d'un fichier de texture");
						}
					}
				}
			}
		}

		console->addMsg("Liberation des ressources...");
		delete pMapASE;
		console->addMsg("Import ASE termine.");
		console->enableOkButton();
	}
	catch(CErreur& erreur)
	{
		if(pMapASE != NULL) {
			delete pMapASE;
		}

		stringstream consoleMsg;
		consoleMsg << "Erreur : " << erreur.what();
		console->addMsg(consoleMsg.str());

		console->addMsg("Import interrompu.");
	}

	return 0;
}

bool AseImporter::copieTexture( CMaterialTexture *mat,
								CMap *pMapASE,
								string &nomRep,
								ConsoleAvancementView* console)
{
	basic_string <char>::size_type index, npos = (basic_string <char>::size_type)-1;
	char ch;

	LOGDEBUG(("copieTexture(mat=%x,pMapASE=%x,nomRep=%s)",
	mat, pMapASE, nomRep.c_str() ));
	LOGDEBUG(("copieTexture() Fichier de texture=%s",
	mat->m_FichierTexture.c_str() ));

	fstream from;	// Fichier source pour la copie du fichier de texture
	fstream to;		// Fichier destination pour la copie du fichier de texture

	string& nom = mat->m_FichierTexture;
	cout << "\nCopie du fichier de texture : " << nom << " (materiau ref " << mat->getRef() << ")";

	stringstream consoleTexte;
	consoleTexte << "Enregistrement texture '" << nom << "'...";
	console->addMsg(consoleTexte.str());

	from.open( nom.c_str(), ios_base::binary|ios_base::in );	// Ouvre le fichier texture d'origine
	if( from.fail() )
	{
		stringstream texte;
		texte << "\nErreur : Echec d'ouverture du fichier de texture (" << nom << ")";
		LOGDEBUG((texte.str().c_str() ));
		cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str().c_str();
		throw CErreur(texte);
	}
		// R�cup�ration du nom du fichier sans son chemin
	index = nom.find_last_of( "/" );		// Recherche du dernier slach
	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien � faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'� lui

		// R�cup�ration du nom du fichier sans son chemin
	index = nom.find_last_of( "\\" );		// Recherche du dernier anti-slach
	if( index!=npos )	// S'il n'y a pas d'anti-slach, y'a rien � faire au nom du fichier
		nom.erase( 0, index+1 );	// Sinon supprime tout jusqu'� lui

		// Nouveau nom avec chemin du fichier de texture
	nom = nomRep + "/" + nom;

	to.open( nom.c_str(), ios_base::binary|ios_base::out );	// Ouvre fichier destination texture
	if( to.fail() )
	{
		stringstream texte;
		texte << "Erreur : Echec de creation du fichier de texture ( " << nom << " )";
		LOGDEBUG((texte.str().c_str() ));
		cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str().c_str();
		throw CErreur(texte);
	}

		// Copie du fichier proprement dite
	while( from.get( ch ) )
		to.put( ch );

		// V�rifie si la copie s'est bien pass�
	if( !from.eof() ) {
		stringstream texte;
		texte << "Echec de copie du fichier de texture (" << nom << ")";
		LOGDEBUG((texte.str().c_str() ));
		cerr << endl << __FILE__ << ":" << __LINE__ << " " << texte.str().c_str();
		throw CErreur(texte);
	}

LOGDEBUG(("copieTexture() Ok" ));

	return true;
}

}	// JktMoteur
