#ifndef __JKT__CFICHIERASE_H
#define __JKT__CFICHIERASE_H

#include <string>

namespace jkt
{

class CFichierASE : private std::ifstream
{
	std::ofstream m_Trace;
	bool m_bAffiche;
	static const std::basic_string <char>::size_type npos = (std::basic_string <char>::size_type)-1;

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

	EntryPoint* litEntryPoint(CMap* map);
	CLight* litLightOmni(CMap* map);
	CLight* litLightTarget(CMap* map);
	FACES* LitFaces( unsigned int num );
	CLight* lightList(CMap* map);
	EntryPoint* litHelper(CMap* map);
	int* LitTexFaces(int num);
	float* LitVertex(unsigned int num);
	float* LitTexVertex(unsigned int num);
	float* LitNormaux(unsigned int num, const float *row);
	CMaterial* materiallist();
	CMaterialMulti* materialMulti( unsigned int uRef );
	CMaterial* materialStandard(unsigned int uRef);
	MapObject* litGeomobject(CMap *pMap, unsigned int nbr);
	void LitFichier(const string &nomFichier, CMap *pMap);
	void LitGroupe(const string &nomFichier, CMap *pMap, unsigned int& nbr_GeoObject);

	string extractGuillemetsDelimitedString(const string str);


	bool operator!();
	std::ofstream& trace();	// Référence au fichier de trace de la lecture du fichier

	CFichierASE& find(const char *txt, int line);		// Trouve le texte 'txt' dans le fichier
	void findAccoladeDebut(int line);					// Trouve le prochain guillemet ouvrant du fichier
	void findAccoladeFin(int line);						// Trouve le prochain guillemet ouvrant du fichier

	void get( string &mot, int line );
	void get( float &val, int line );
	void get( bool &val, int line );
	void get( unsigned int &val, int line );
	void get( int &val, int line );
	void get( char &ch, int line );

	CFichierASE& isNext(const char *txt, int line);
	bool isGet(std::string &mot, int line);

public:
	static bool LitFichierASE(const std::string nomFichier, CMap *pMap, bool bAffiche );
};

}	//namespace jkt

#endif
