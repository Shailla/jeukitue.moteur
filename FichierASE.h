#ifndef __JKT__CFICHIERASE_H
#define __JKT__CFICHIERASE_H

#include <string>

namespace JKT_PACKAGE_MOTEUR3D
{

class CFichierASE : private ifstream
{
	std::ofstream m_Trace;
	bool m_bAffiche;
	static const basic_string <char>::size_type npos = (basic_string <char>::size_type)-1;

	class FACES
	{
		// Ce classe ne doit pas gérer elle-même les new/delete sur les tableaux
	public:
		FACES(int num);
		~FACES();

		int *faces;
		int *texRef;
	};

	enum erreursFichier {
		JKT_ERREUR_FICHIER_EOF,					// Fin du fichier prématurée
		JKT_ERREUR_FICHIER_INVALIDE,			// Fichier ASE invalide 
		JKT_ERREUR_FICHIER_MARTERIAUMANQUE,		// Un ou plusieurs matériaux manquent
		JKT_ERREUR_FICHIER_MISSPARAM,			// Paramètre introuvable
		JKT_ERREUR_FICHIER_ACCOLADE,			// Accolade inattendue
	};

	CFichierASE(const std::string &nomFichier, CMap *pMap, bool bAffiche);
	~CFichierASE();

	CLight* litLightOmni();
	CLight* litLightTarget();
	FACES* LitFaces( unsigned int num );
	CLight* lightList();
	int* LitTexFaces(int num);
	float* LitVertex(unsigned int num);
	float* LitTexVertex(unsigned int num);
	float* LitNormaux(unsigned int num, const float *row);
	CMaterial* materiallist();
	CMaterialMulti* materialMulti( unsigned int uRef );
	CMaterial* materialStandard(unsigned int uRef);
	CGeo* geomobject(CMap *pMap, unsigned int nbr);
	void LitFichier(const string &nomFichier, CMap *pMap);


	bool operator!();
	ofstream& trace();	// Référence au fichier de trace de la lecture du fichier

	CFichierASE& find(const char *txt, int line);		// Trouve le texte 'txt' dans le fichier
	void findAccoladeO(int line);				// Trouve le prochain guillemet ouvrant du fichier
	void findAccoladeF(int line);				// Trouve le prochain guillemet ouvrant du fichier
	void get( string &mot, int line );
	void get( float &val, int line );
	void get( bool &val, int line );
	void get( unsigned int &val, int line );
	void get( int &val, int line );
	void get( char &ch, int line );

	CFichierASE& isNext(const char *txt, int line);
	bool isGet(string &mot, int line);

public:
	static bool LitFichierASE( const string &nomFichier, CMap *pMap, bool bAffiche );
};

}	//namespace JKT_PACKAGE_MOTEUR3D

#endif