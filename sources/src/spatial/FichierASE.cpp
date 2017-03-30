
#include <string>
#include <fstream>
#include <sstream>

#include "util/math_vectoriel.h"
#include "spatial/light/LightOmni.h"
#include "spatial/light/LightTarget.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/geo/EntryPoint.h"
#include "spatial/Map.h"
#include "spatial/geo/ChanTex.h"

#include "spatial/FichierASE.h"

using namespace std;

namespace jkt
{
#define fScale (300.0f)

CFichierASE::FACES::FACES(int num) {
	faces = new int[3*num];
	texRef = new int[num];
}

CFichierASE::FACES::~FACES() {
	if(faces)
		delete[] faces;

	if(texRef)
		delete[] texRef;
}

CFichierASE::CFichierASE(const string &nomFichier, CMap *pMap, bool bAffiche) : ifstream(nomFichier.c_str(), ios::in) {
	try {
		m_bAffiche = bAffiche;
		m_Trace.open( "traceConvertASE.log", ios_base::out );

		LitFichier(nomFichier, pMap);
	}
	catch(int val) {	// Gestion des erreurs arriv�es pendant la lecture du fichier ASE
		trace() << "\nErreur : La lecture du fichier n'a pas abouti :\n\t";

		switch(val) {
		case JKT_ERREUR_FICHIER_EOF:
			trace() << "Erreur : Fin du fichier rencontree prematurement";
			break;

		case JKT_ERREUR_FICHIER_INVALIDE:
			trace() << "Erreur : Le fichier est invalide ou corrompu";
			break;

		case JKT_ERREUR_FICHIER_MARTERIAUMANQUE:
			trace() << "Erreur : Un ou plusieurs materiaux manquent";
			break;

		case JKT_ERREUR_FICHIER_MISSPARAM:
			trace() << "Erreur : Parametre introuvable";
			break;

		default:
			break;
		}
		throw (bool)false;
	}
}

CFichierASE::~CFichierASE(void) {
	if( m_Trace )
		m_Trace.close();
}

/**
 * Supprime le premier et le dernier caract�re d'une cha�ne de caract�res.
 */
string CFichierASE::extractGuillemetsDelimitedString(string str) {
	// Enl�ve le premier guillemet
	size_t pos = str.find_first_of( '"' );
	if(pos != npos)
		str.erase( pos, 1 );

	// Enl�ve le dernier guillemet
	pos = str.find_last_of( '"' );
	if(pos != npos)
		str.erase( pos, 1 );

	return str;
}

ofstream& CFichierASE::trace() {
	return m_Trace;
}

bool CFichierASE::operator!() {
	return !(*static_cast<ifstream *> (this));
}

CFichierASE& CFichierASE::find(const char *txt, int line) {
	string mot;

	do {
		if( !(*static_cast<ifstream *> (this) >>( mot)) )	// Recherche du nom de l'objet
		{
			m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
			throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
		}

		if( (mot=="{") || (mot=="}") )
		{
			m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Parametre introuvable";
			throw (int)JKT_ERREUR_FICHIER_MISSPARAM;	// Fin de fichier pr�matur�e
		}
	} while( mot != txt );

	return *this;
}

void CFichierASE::findAccoladeDebut(int line)	// Trouve le prochain guillemet ouvrant du fichier
{
	string mot;
	do
	{
		if( !(*static_cast<ifstream *> (this) >>( mot )) )	// Recherche du nom de l'objet
		{
			m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
			throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
		}
		if( mot == "}" )
		{
			m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Accolade inattendue";
			throw (int)JKT_ERREUR_FICHIER_ACCOLADE;	// Fin de fichier pr�matur�e
		}
	} while( mot != "{" );
}

void CFichierASE::findAccoladeFin(int line)	// Trouve le prochain guillemet ouvrant du fichier
{
	string mot;
	do {
		if( !(*static_cast<ifstream *> (this) >>( mot)) )	// Recherche du nom de l'objet
		{
			m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
			throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
		}
		if( mot == "{" )
		{
			m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Accolade inattendue";
			throw (int)JKT_ERREUR_FICHIER_ACCOLADE;	// Fin de fichier pr�matur�e
		}
	} while( mot != "}" );
}

CFichierASE& CFichierASE::isNext(const char *txt, int line)
{
	string mot;

	if( !(*static_cast<ifstream *> (this) >>( mot)) ) {		// Recherche du nom de l'objet
		m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
		throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
	}

	if( mot != txt ) {
		m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Parametre introuvable";
		throw (int)JKT_ERREUR_FICHIER_MISSPARAM;	// Fin de fichier pr�matur�e
	}

	return *this;
}

void CFichierASE::get(string &mot, int line) {
	if( !(*static_cast<ifstream *> (this) >>( mot )) )	// Recherche du nom de l'objet
	{
		m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
		throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
	}
}

void CFichierASE::get(char &ch, int line)
{
	if( !(*static_cast<ifstream *> (this) >>( ch )) )	// Recherche du nom de l'objet
	{
		m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
		throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
	}
}

bool CFichierASE::isGet(string &mot, int line) {
	if( !(*static_cast<ifstream *> (this) >>( mot )) )	// Recherche du nom de l'objet
	{
		m_Trace << "\nEnd of file";
		return false;
	}

	return true;
}

void CFichierASE::get(int &num, int line) {
	if( !(*static_cast<ifstream *> (this) >>( num )) )	// Recherche du nom de l'objet
	{
		m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
		throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
	}
}

void CFichierASE::get(unsigned int &num, int line) {
	if( !(*static_cast<ifstream *> (this) >>( num )) )	// Recherche du nom de l'objet
	{
		m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
		throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
	}
}

void CFichierASE::get(float &num, int line) {
	if( !(*static_cast<ifstream *> (this) >>( num )) )	// Recherche du nom de l'objet
	{
		m_Trace << "\nErreur (FichierASE.cpp::" << line << ") : Fin de fichier prematuree";
		throw (int)JKT_ERREUR_FICHIER_EOF;	// Fin de fichier pr�matur�e
	}
}

CFichierASE::FACES *CFichierASE::LitFaces(unsigned int num) {
	unsigned int index1, index2, index3, refTex;
	FACES *faces = new FACES(num);

	string mot;

	if( m_bAffiche )
		trace() << "\nTABLEAU D'INDEX :\n";

	findAccoladeFin(__LINE__);
	findAccoladeDebut(__LINE__);

	for( unsigned int i=0 ; i<num ; i++) {
		find("*MESH_FACE", __LINE__);

		get(mot, __LINE__);
		get(mot, __LINE__);

		get(index1, __LINE__);
		get(mot, __LINE__);
		get(index2, __LINE__);
		get(mot, __LINE__);
		get(index3, __LINE__);

		find("*MESH_MTLID", __LINE__);

		get(refTex, __LINE__);

		faces->faces[ (3*i) ] = index1;		// Face 1
		faces->faces[ (3*i)+1 ] = index2;	// Face 2
		faces->faces[ (3*i)+2 ] = index3;	// Face 3
		faces->texRef[ i ] = refTex;	// R�f�rence sous-mat�riaux

		if( m_bAffiche )
			trace() << "\n\t" << index1 << " " << index2 << " " << index3 << " " << " tex(" << refTex << ")";
	}

	return faces;
}

EntryPoint* CFichierASE::litEntryPoint(CMap* map) {
	trace() << "\n\tEntryPoint";
	EntryPoint *entryPoint = NULL;
	string mot;

	bool bPosition = false;		// Indique si la position a �t� trouv�e
	float position[ 3 ];

	findAccoladeDebut(__LINE__);
	find("*TM_POS", __LINE__);

	get(position[0],__LINE__);
	get(position[1],__LINE__);
	get(position[2],__LINE__);

	position[0] /= fScale;
	position[1] /= fScale;
	position[2] /= fScale;

	bPosition = true;

	if(bPosition) {
		entryPoint = new EntryPoint(map, position);
	}

	if( m_bAffiche ) {
		trace() << "\n\tPosition du point d'entree : ";
		trace() << position[0] << ' ' << position[1] << ' ' << position[2];
	}

	findAccoladeFin(__LINE__);

	return entryPoint;
}

CLight* CFichierASE::litLightOmni(CMap* map) {
	CLightOmni *pLightOmni;
	string mot;
//	int accolade = 0;

	bool bPosition = false;		// Indique si la position a �t� trouv�e
	float position[ 3 ];

	bool bColor = false;
	float color[4];

	findAccoladeDebut(__LINE__);
	find("*TM_POS", __LINE__);

	get(position[0],__LINE__);
	get(position[1],__LINE__);
	get(position[2],__LINE__);

	position[0] /= fScale;
	position[1] /= fScale;
	position[2] /= fScale;
	bPosition = true;

	if( m_bAffiche )
	{
		trace() << "\nPosition de la lumi�re : ";
		trace() << position[0] << ' ' << position[1] << ' ' << position[2];
	}

	findAccoladeFin(__LINE__);
	findAccoladeDebut(__LINE__);

	find("*LIGHT_COLOR", __LINE__);

	get(color[0],__LINE__);
	get(color[1],__LINE__);
	get(color[2],__LINE__);

	color[3] = 1.0f;
	bColor = true;

	if( m_bAffiche )
	{
		trace() << "\nCouleur de la lumiere : ";
		trace() << color[0] << ' ' << color[1] << ' ' << color[2];
	}

	findAccoladeFin(__LINE__);

	if( bPosition && bColor )
	{
		trace() << endl << "Creation de lumiere omni";
		pLightOmni = new CLightOmni(map);
		pLightOmni->SetPosition( position );
		pLightOmni->SetColor( color );
	}
	else
		pLightOmni = 0;

	return pLightOmni;
}

CLight *CFichierASE::litLightTarget(CMap* map) {
	string mot;
	float position[3], row[9], direction[3], color[4];
	float falloff;

	findAccoladeDebut(__LINE__);
	find("*TM_ROW0", __LINE__);

	get(row[0],__LINE__);
	get(row[1],__LINE__);
	get(row[2],__LINE__);

	if( m_bAffiche )
	{
		trace() << endl << "Row0 : ";
		trace() << row[0] << ' ' << row[1] << ' ' << row[2];
	}

	find("*TM_ROW1", __LINE__);
	get(row[3], __LINE__);
	get(row[4], __LINE__);
	get(row[5], __LINE__);

	if( m_bAffiche )
	{
		trace() << endl << "Row1 : ";
		trace() << row[3] << ' ' << row[4] << ' ' << row[5];
	}

	find("*TM_ROW2", __LINE__);
	get(row[6],__LINE__);
	get(row[7],__LINE__);
	get(row[8],__LINE__);

	if( m_bAffiche )
	{
		trace() << endl << "Row2 : ";
		trace() << row[6] << ' ' << row[7] << ' ' << row[8];
	}

	find("*TM_POS", __LINE__);	//Recherche la position de la lumi�re
	get(position[0],__LINE__);
	get(position[1],__LINE__);
	get(position[2],__LINE__);

	position[0] /= fScale;
	position[1] /= fScale;
	position[2] /= fScale;

	if( m_bAffiche )
	{
		trace() << "\nPosition de la lumi�re : ";
		trace() << position[0] << ' ' << position[1] << ' ' << position[2];
	}

	findAccoladeFin(__LINE__);
	findAccoladeDebut(__LINE__);

	find("*LIGHT_COLOR", __LINE__ );	// Recherche l'�ventuelle couleur de 'lobjet g�o

	get(color[0],__LINE__);
	get(color[1],__LINE__);
	get(color[2],__LINE__);

	color[3] = 1.0f;

	if( m_bAffiche )
	{
		trace() << "\nCouleur de la lumiere : ";
		trace() << color[0] << ' ' << color[1] << ' ' << color[2];
	}

	find("*LIGHT_FALLOFF", __LINE__);
	get(falloff,__LINE__);

	if( m_bAffiche )
		trace() << "\nFallOff : " << falloff;

	findAccoladeFin(__LINE__);

		// Calcul de la direction de la lumi�re
	float base[] = { 0.0f, 0.0f, -1.0f };
	jkt::produitMatriciel(row, base, direction );

		// Cr�ation et param�trage de la lumi�re
	CLightTarget *pLightTarget = new CLightTarget(map);
	pLightTarget->SetPosition( position );
	pLightTarget->SetDirection( direction );
	pLightTarget->SetColor( color );
	pLightTarget->SetFallOff( falloff );

	return pLightTarget;
}

CLight* CFichierASE::lightList(CMap* map)
{
	unsigned int accolade = 0;
	bool bSuite = false;

	string mot;

	CLight *pLight = 0;
	if( m_bAffiche )
		trace() << "\nLISTE DE LUMIERE(S)";

	do
	{
		get(mot,__LINE__);

		if( mot == "*LIGHT_TYPE" )
		{
			get(mot,__LINE__);
			if( mot == "Omni" )
			{
				pLight = litLightOmni(map);
				bSuite = true;	// On peut passer � la suite de la lecture du fichier
			}
			else if( mot == "Target" ) {
				pLight = litLightTarget(map);
				bSuite = true;	// On peut passer � la suite de la lecture du fichier
			}
			else {
				trace() << "\nErreur (FichierASE.cpp::" << __LINE__ << ") : Lumiere de type inconnu";
			}
		}
		else if( mot=="{" )
				accolade++;
		else if( mot=="}" )
				accolade--;
	}
	while( accolade!=0 && !bSuite );

	return pLight;
}

EntryPoint* CFichierASE::litHelper(CMap* map) {
	string mot;

	findAccoladeDebut(__LINE__);

	EntryPoint *entryPoint = NULL;

	if( m_bAffiche )
		trace() << "\nLECTURE D'UN HELPER";

	// Lecture du NODE NAME
	find("*NODE_NAME", __LINE__);
	get(mot,__LINE__);
	string nodeName = extractGuillemetsDelimitedString(mot);

	// Lecture du HELPER CLASS
	find("*HELPER_CLASS", __LINE__);
	get(mot,__LINE__);
	string helperClass = extractGuillemetsDelimitedString(mot);


	// Les helpers de type point dont le nom commence par EntryPoint d�finissent les points d'entr�e de la Map
	if(helperClass == "PointHelper" && nodeName.compare(0, strlen("EntryPoint") - 1, "EntryPoint")) {
		entryPoint = litEntryPoint(map);
	}
	else {		// Ignore le helper car il n'est pas d'un type reconnu par JKT
		findAccoladeDebut(__LINE__);
		findAccoladeFin(__LINE__);
	}

	findAccoladeFin(__LINE__);

	return entryPoint;
}

int* CFichierASE::LitTexFaces(int num)
{
	int index, tface1, tface2, tface3;
	string mot;

	int *tab = new int[3*num];

	findAccoladeDebut(__LINE__);

	if( m_bAffiche )
		trace() << "\nTABLEAU D'INDEX DE TEXTURES :\n";

	for( int i=0 ; i<num ; i++)
	{
		find("*MESH_TFACE", __LINE__);

		get(index, __LINE__);
		get(tface1, __LINE__);
		get(tface2, __LINE__);
		get(tface3, __LINE__);

		tab[ 3*i ] = tface1;
		tab[ (3*i)+1 ] = tface2;
		tab[ (3*i)+2 ] = tface3;

		if( m_bAffiche )
			trace()<<"\n\t" << index << ' ' << tab[ 3*i ] << ' ' << tab[ 3*i+1 ] << ' ' << tab[ 3*i+2 ];
	}

	return tab;
}

float* CFichierASE::LitVertex(unsigned int num)
{
	float *tab = new float[ 3*num ];

	string mot;

	unsigned int index;
	float vertex1, vertex2, vertex3;

	if( m_bAffiche )
		trace() << "\nTABLEAU DE SOMMETS :\n";

	findAccoladeDebut(__LINE__);

	for( unsigned int i=0 ; i<num ; i++)
	{
		find("*MESH_VERTEX", __LINE__);

		get(index, __LINE__);
		get(vertex1, __LINE__);
		get(vertex2, __LINE__);
		get(vertex3, __LINE__);

		tab[3*i] = vertex1/fScale;
		tab[(3*i)+1] = vertex2/fScale;
		tab[(3*i)+2] = vertex3/fScale;

		if( m_bAffiche )
			trace() <<"\n\t" <<index<<' '<<tab[3*i]<<' '<<tab[3*i+1]<<' '<< tab[3*i+2];
	}

	return tab;
}

float* CFichierASE::LitTexVertex(unsigned int num)	// num=nombre de sommets de texture
{
	string mot;
	float *tab = new float[2*num];
	float var;
	unsigned int index;

	if( m_bAffiche )
		trace() << "\nTABLEAU DE SOMMETS DE TEXTURES :\n";

	findAccoladeDebut(__LINE__);

	for( unsigned int i=0 ; i<num ; i++)
	{
		find("*MESH_TVERT", __LINE__);

		get(index, __LINE__);
		get(tab[(2*i)+1], __LINE__);
		get(tab[2*i], __LINE__);
		get(var, __LINE__);				// On ignore la 3� coordonn�e de texture

		if( m_bAffiche )
			trace()<<"\n\t"<<index<<' '<<tab[2*i]<<' '<<tab[(2*i)+1]<<' '<<var;
	}

	return tab;
}

float* CFichierASE::LitNormaux(unsigned int numFaces, const float *row)
{
	string mot;

	float *tab = new float[ 9*numFaces ];

	unsigned int index, index1, index2, index3;
	float normalFace1, normalFace2, normalFace3;
	float normal[3];

	float resultNormal[3];

	if( m_bAffiche )
		trace() << "\nTABLEAU DE VECTEURS NORMAUX : " << numFaces << " faces" << endl;

	findAccoladeDebut(__LINE__);

	for( unsigned int i=0 ; i<numFaces ; i++)
	{
		find("*MESH_FACENORMAL", __LINE__);

		get(index, __LINE__);
		get(normalFace1, __LINE__);
		get(normalFace2, __LINE__);
		get(normalFace3, __LINE__);		// Le vecteur normal � la face
										// n'est pas utilis�

		if( m_bAffiche )
			trace() << "\n" << index << normalFace1 << normalFace2 << normalFace3;

		get(mot, __LINE__);
		get(index1,__LINE__);
		get(normal[0], __LINE__);
		get(normal[1], __LINE__);
		get(normal[2], __LINE__);

		jkt::produitMatriciel(row, normal, resultNormal);
		tab[ 9*i + 0 ] = -resultNormal[0];
		tab[ 9*i + 1 ] = resultNormal[1];
		tab[ 9*i + 2 ] = resultNormal[2];

		get(mot, __LINE__);
		get(index2, __LINE__);
		get(normal[0], __LINE__);
		get(normal[1], __LINE__);
		get(normal[2], __LINE__);

		jkt::produitMatriciel(row, normal, resultNormal);
		tab[ 9*i + 3 ] = -resultNormal[0];
		tab[ 9*i + 4 ] = resultNormal[1];
		tab[ 9*i + 5 ] = resultNormal[2];

		get(mot, __LINE__);
		get(index3, __LINE__);
		get(normal[0], __LINE__);
		get(normal[1], __LINE__);
		get(normal[2], __LINE__);

		jkt::produitMatriciel(row, normal, resultNormal);
		tab[ 9*i + 6 ] = -resultNormal[0];
		tab[ 9*i + 7 ] = resultNormal[1];
		tab[ 9*i + 8 ] = resultNormal[2];

		if( m_bAffiche )
		{
			trace()<<"\n"<<index1<<' '<<tab[9*i]<<' '<<tab[9*i+1]<<' '<<tab[9*i+2];
			trace()<<"\n"<<index2<<' '<<tab[9*i+3]<<' '<<tab[9*i+4]<<' '<<tab[9*i+5];
			trace()<<"\n"<<index3<<' '<<tab[9*i+6]<<' '<<tab[9*i+7]<<' '<<tab[9*i+8];
		}
	}

	return tab;
}

CMaterial* CFichierASE::materiallist()
{
	string mot;
	CMaterial *pMaterial;
	unsigned int uRef;	// Variable locale de r�f�rence de l'objet mat�riau

	if( m_bAffiche )
		trace() << "\nLISTE DE MATOS";

	find("*MATERIAL", __LINE__);

	get(uRef,__LINE__);	// Recherche la r�f�rence de l'obet mat�riau

	findAccoladeDebut(__LINE__);

	find("*MATERIAL_CLASS", __LINE__);		// Recherche le type d'obet mat�riau

	get(mot,__LINE__);	// Recherche la r�f�rence de l'obet mat�riau

	if( m_bAffiche )
		trace() << "\nMateriau de type : " << mot << " (" << uRef << ")";

	if( mot=="\"Standard\"" ) {	// S'il s'agit d'un mat�riau de type standard
		trace() << "\nMateriau Standard";
		pMaterial = materialStandard( uRef );
	}
	else if((mot=="\"Multi/sous-objet\"") || (mot=="\"Multi/Sub-Object\"")) {	// S'il s'agit d'un mat�riau multiple
		trace() << "\nMateriau Multi";
		pMaterial = materialMulti( uRef );
	}
	else {
		trace() << "\nErreur (FichierASE.cpp::" << __LINE__ << ") : Type de materiau inconnu (" << mot << ")";
		throw (int)CFichierASE::JKT_ERREUR_FICHIER_INVALIDE;	// Fin de fichier pr�matur�e
	}

	return pMaterial;
}

CMaterialMulti* CFichierASE::materialMulti(unsigned int uRef)
{
	unsigned int accolade2 = 1;
	int indexTex = 0;				// Index du sous-mat�riau en cours de lecture
	string mot;

	CMaterialMulti *pMaterialMulti = new CMaterialMulti();
	pMaterialMulti->setRef(uRef);

	do {
		get(mot,__LINE__);

		if( mot=="*MATERIAL_AMBIENT" ) {			//Recherche l'�ventuelle couleur ambiante de l'objet geo
			get(pMaterialMulti->m_Ambient[0],__LINE__);
			get(pMaterialMulti->m_Ambient[1],__LINE__);
			get(pMaterialMulti->m_Ambient[2],__LINE__);

			if(m_bAffiche) {
				trace() << "\nCouleur ambiante (multi) : ";
				trace()<<pMaterialMulti->m_Ambient[0]<<' '<<pMaterialMulti->m_Ambient[1]<<' '<<pMaterialMulti->m_Ambient[2];
			}
		}
		else if( mot=="*MATERIAL_DIFFUSE" )	//Recherche l'�ventuelle couleur diffuse de l'objet g�o
		{
			get(pMaterialMulti->m_Diffuse[0],__LINE__);
			get(pMaterialMulti->m_Diffuse[1],__LINE__);
			get(pMaterialMulti->m_Diffuse[2],__LINE__);

			if(m_bAffiche) {
				trace() << "\nCouleur diffuse (multi) : ";
				trace()<<pMaterialMulti->m_Diffuse[0]<<' '<<pMaterialMulti->m_Diffuse[1]<<' '<<pMaterialMulti->m_Diffuse[2];
			}
		}
		else if( mot=="*MATERIAL_SPECULAR" )	//Recherche l'�ventuelle couleur sp�culaire de l'objet g�o
		{
			get(pMaterialMulti->m_Specular[0],__LINE__);
			get(pMaterialMulti->m_Specular[1],__LINE__);
			get(pMaterialMulti->m_Specular[2],__LINE__);

			if( m_bAffiche ) {
				trace() << "\nCouleur specular (multi) : ";
				trace()<<pMaterialMulti->m_Specular[0]<<' '<<pMaterialMulti->m_Specular[1]<<' '<<pMaterialMulti->m_Specular[2];
			}
		}
		else if( mot=="*NUMSUBMTLS" ) {
			int nbrTex;	// Nombre de sous-mat�riaux du mat�riau multiple
			get(nbrTex,__LINE__);

			pMaterialMulti->NbrTex( nbrTex );

			if( m_bAffiche )
				trace() << "\nNombre de sous-materiaux : " << nbrTex;
		}
		else if( mot=="*SUBMATERIAL" ) {
			unsigned int uRef;
			get(uRef,__LINE__);	// Recherche la r�f�rence de l'obet sous-mat�riau

			if( m_bAffiche )
				trace() << "\n\tSous-Materiau ref. : " << uRef;

			findAccoladeDebut(__LINE__);

			find("*MATERIAL_CLASS", __LINE__);	// Place toi � la d�finition du type de mat�riaux

			CMaterialTexture *pMat = (CMaterialTexture*)materialStandard(uRef );	// Lecture du sous-mat�riau

			pMaterialMulti->m_TabMat[ indexTex++ ] = pMat;
		}
		else if( mot=="{" )
			accolade2++;
		else if( mot=="}" )
			accolade2--;
	}
	while( accolade2!=0 );

	return pMaterialMulti;
}

CMaterial* CFichierASE::materialStandard(unsigned int uRef)
{
	unsigned int accolade2 = 1;
	string mot;
	bool indic;		// Indique si un canal de texture a �t� trouv�

	CMaterial *pMaterial = new CMaterial();
	pMaterial->setRef(uRef);
	string::iterator k;

	do {
		get(mot,__LINE__);

		if( mot=="*MAP_NAME" ) {	// Lecture du nom du mat�riau
			string canal;
			char lettre;

			get(lettre, __LINE__);	// Prends le guillemet d'ouverture

			if( lettre!='\"' ) {
				trace() << "\nErreur (fichier::materiallist) : Fichier ASE corrompu (\")";
				throw (int)CFichierASE::JKT_ERREUR_FICHIER_INVALIDE;	// Fin de fichier pr�matur�e
			}

			for( ;; ) {	// Prends les mots jusqu'� la fermeture des guillemets
				get(lettre,__LINE__);

				if( lettre!='\"' )
					canal += lettre;
				else
					break;
			}

				// Lecture de l'�ventuel canal de texture dans le nom du mat�riau
			//pMaterialTexture->m_CanalTex = 1;		// Canal par d�faut
			indic = false;

			for( string::iterator p=canal.begin() ; p+1<canal.end() ; p++ ) {
				// p+<canal.end() car la cha�ne doit contenir au moins un '*' et un chiffre
				if( p==canal.begin() )	// Si on en est pas au 1� caract�re de la cha�ne
				{
					if( *p==' ' )	// S'il y a un espace
						k = p+1;	// alors v�rifie si le caract�re suivant est un '*' (->passe � la suite avec p+1)
					else
						k = p;		// Sinon v�rifie si le caract�re actuel est un '*' (->passe � la suite avec p)
				}
				else
				{
					if( *p==' ' )	// S'il y a un espace
						k = p+1;	// alors v�rifie si le caract�re suivant est un '*' (->passe � la suite avec p+1)
					else
						continue;
				}

				if( k+1<canal.end() )	// V�rifie si la cha�ne est encore assez longue pour contenir un '*'
				{						// et au moins un chiffre
					if( (*k=='*') )
					{
						string crotte( k+1, canal.end() );	// Prend le reste de la cha�ne apr�s le '*'
						istringstream caca( crotte );
						int var;
						if( !!(caca>>var) )
						{		// Si un num�ro de canal a �t� trouv�, sorts de la boucle for
							//pMaterialTexture->m_CanalTex = var;
							indic = true;
							break;		// Sort de la boucle for
						}
					}
				}
			}

			if( m_bAffiche ) {
				//trace() << "\nCanal de texture : " << pMaterialTexture->m_CanalTex;

				if( !indic )	// Si aucun canal de texture explicite n'a �t� trouv�
					trace() << " (par defaut)";
			}
		}
		else if(mot=="*MAP_CLASS") {
			get(mot,__LINE__);

			CMaterialTexture *pMaterialTexture = 0;

			if(mot=="\"Bitmap\"") {
				pMaterialTexture = new CMaterialTexture( *pMaterial );
				delete pMaterial;
				pMaterial = pMaterialTexture;
			}
			else {
				pMaterialTexture = (CMaterialTexture*)pMaterial;
			}
		}
		else if( mot=="*MATERIAL_AMBIENT" ) {			//Recherche l'�ventuelle couleur ambiante de l'objet geo
			get(pMaterial->m_Ambient[0],__LINE__);
			get(pMaterial->m_Ambient[1],__LINE__);
			get(pMaterial->m_Ambient[2],__LINE__);

			if(m_bAffiche) {
				trace() << "\nCouleur ambiante : ";
				trace()<<pMaterial->m_Ambient[0]<<' '<<pMaterial->m_Ambient[1]<<' '<<pMaterial->m_Ambient[2];
			}
		}
		else if( mot=="*MATERIAL_DIFFUSE" )	//Recherche l'�ventuelle couleur diffuse de l'objet g�o
		{
			get(pMaterial->m_Diffuse[0],__LINE__);
			get(pMaterial->m_Diffuse[1],__LINE__);
			get(pMaterial->m_Diffuse[2],__LINE__);

			if(m_bAffiche) {
				trace() << "\nCouleur diffuse : ";
				trace()<<pMaterial->m_Diffuse[0]<<' '<<pMaterial->m_Diffuse[1]<<' '<<pMaterial->m_Diffuse[2];
			}
		}
		else if( mot=="*MATERIAL_SPECULAR" )	//Recherche l'�ventuelle couleur sp�culaire de l'objet g�o
		{
			get(pMaterial->m_Specular[0],__LINE__);
			get(pMaterial->m_Specular[1],__LINE__);
			get(pMaterial->m_Specular[2],__LINE__);

			if( m_bAffiche ) {
				trace() << "\nCouleur specular : ";
				trace()<<pMaterial->m_Specular[0]<<' '<<pMaterial->m_Specular[1]<<' '<<pMaterial->m_Specular[2];
			}
		}
		else if( mot=="*BITMAP" )	// Lecture du nom de fichier de texture
		{
			string nomFichier;
			CMaterialTexture *pMaterialTexture = 0;

			if( pMaterial->Type() != CMaterial::MAT_TYPE_TEXTURE ) {
				pMaterialTexture = new CMaterialTexture( *pMaterial );
				delete pMaterial;
				pMaterial = pMaterialTexture;
			}
			else {
				pMaterialTexture = (CMaterialTexture*)pMaterial;
			}

			get(nomFichier,__LINE__);

			// Enl�ve le premier guillemet
			size_t pos = nomFichier.find_first_of( '"' );
			if(pos != npos)
				nomFichier.erase( pos, 1 );

			// Enl�ve le dernier guillemet
			pos = nomFichier.find_last_of( '"' );
			if(pos != npos)
				nomFichier.erase( pos, 1 );

			pMaterialTexture->m_FichierTexture = nomFichier;

			if( m_bAffiche )
				trace() << "\nNom du fichier de texture : " << pMaterialTexture->m_FichierTexture;
		}
		else if( mot=="{" )
			accolade2++;
		else if( mot=="}" )
			accolade2--;
	}
	while( accolade2!=0 );

	return pMaterial;
}

MapObject* CFichierASE::litGeomobject(CMap *pMap, unsigned int nbr) {		//Lit un objet g�om�trique dans le
																			// fichier et le place en m�moire puis retourne son pointeur
	unsigned int accolade = 0;
	unsigned int numVertex=0, numFaces=0, numTexVertex=0, numTexFaces=0;
	float *tabV;
	string mot;
	float row[9];

	CGeoMaker *pGeoMaker = new CGeoMaker(pMap);

	if( m_bAffiche )
		trace() << "\f"; //Changement de page

	findAccoladeDebut(__LINE__);

	accolade = 2;

	isNext("*NODE_NAME", __LINE__);

	char ch;
	string nom;

	do	// Recherche du guillemet ouvrant
		get( ch, __LINE__);
	while( ch!='\"' );

	do {		// Lecture du nom de l'objet entre guillemets
		get(ch, __LINE__);

		if( ch=='\n' ) {
			trace() << "\nErreur (fichier::geomobject 2) : Fichier ASE corrompu";
			throw (int)CFichierASE::JKT_ERREUR_FICHIER_INVALIDE;		// Fichier invalide (il aurait d� y avoir une accolade ouvrante ici)
		}

		if( ch!='\"' )
			nom += ch;
	} while( ch!='\"' );

	pGeoMaker->setName( nom );

	if( m_bAffiche )
		trace() << "\nNom de l'objet geo : " << nom;

	findAccoladeDebut(__LINE__);

		// Lecture des param�tres de transformation pour les normales
	find("*TM_ROW0", __LINE__);
	get(row[0], __LINE__);
	get(row[1], __LINE__);
	get(row[2], __LINE__);
	if( m_bAffiche )
		trace() << "\nRow0 : " << row[0] << "\t" << row[1] << "\t" << row[2] << endl;

	find("*TM_ROW1", __LINE__);
	get(row[3],__LINE__);
	get(row[4],__LINE__);
	get(row [5],__LINE__);

	if( m_bAffiche )
		trace() << "\nRow1 : " << row[3] << "\t" << row[4] << "\t" << row[5] << endl;

	find("*TM_ROW2", __LINE__);
	get(row[6],__LINE__);
	get(row[7],__LINE__);
	get(row[8],__LINE__);

	if( m_bAffiche )
		trace() << "\nRow2 : " << row[6] << "\t" << row[7] << "\t" << row[8] << endl;

	while( accolade ) { // Recherche le nombre de sommets apres le mot "*MESH_NUMVERTEX"
		get(mot, __LINE__);

		if( mot=="*MESH_NUMVERTEX" ) {
			get(numVertex, __LINE__);

			if( numVertex!=0 ) {
				if( m_bAffiche )
					trace() << "\nIl y a " << numVertex << " sommets dans l'objet geometrique " << nbr;
				break;
			}
			else {		// Ignore l'objet s'il ne contient aucun sommet
				while( accolade ) {
					get(mot, __LINE__);

					if( mot=="{" )	// Compte une accolade
						accolade++;
					else if( mot=="}" )	// D�compte une accolade
						accolade--;
				}

				return 0;
			}
		}
		else if( mot=="{" )	// Compte une accolade
			accolade++;
		else if( mot=="}" )	// D�compte une accolade
			accolade--;
	}

	while( accolade ) {		//Recherche le nombre de faces apres le mot "*MESH_NUMFACES"
		get(mot, __LINE__);

		if( mot=="*MESH_NUMFACES" ) {
			get(numFaces, __LINE__);

			if( numFaces!=0 ) {
				if( m_bAffiche )
					trace() << "\nIl y a " << numFaces <<" faces dans l'objet geometrique " << nbr;
				break;
			}
			else {
				trace() << "\nNombre de faces non valide\n";
			}
		}
		else if( mot=="{" )	// Compte une accolade
			accolade++;
		else if( mot=="}" )	// D�compte une accolade
			accolade--;
	}

	if( m_bAffiche )
		trace() << "\nLe fichier contient " << numVertex << " sommets et " << numFaces << "faces";

	tabV = LitVertex( numVertex );			// Lit les sommets
	pGeoMaker->setVertex(numVertex, tabV);

	FACES *Faces = LitFaces( numFaces );	// Place les index en m�moire
	pGeoMaker->setFaces(numFaces, Faces->faces);
	Faces->faces = 0;

	accolade++;		//LitVertex et LitFaces ne comptent pas les accolades

	while( accolade ) {		 //Recherche la couleur de l'objet apres le mot "*WIREFRAME_COLOR"
		get(mot, __LINE__);

		if( mot=="*WIREFRAME_COLOR" ) {		//Recherche l'�ventuelle couleur de 'lobjet g�o
			float r,g,b;
			get(r, __LINE__);
			get(g, __LINE__);
			get(b, __LINE__);

			pGeoMaker->setColor3( r, g, b );

			if( m_bAffiche )
				trace() << "\nLa couleur est " <<' '<<r<<' '<<g<<' '<<b<<  " dans l'objet g�om�trique " << nbr;

		}
		else if( mot=="*MESH_NUMTVERTEX" ) {		// Recherche les �ventuels sommets de texture
			get(numTexVertex, __LINE__);

			if( numTexVertex ) {
//				pGeoObject->m_bMaterialTexture = true;
					// A FAIRE : Utiliser le canal 0 et voir si TabTex peut recevoir
					// un �l�ment nouveau en TabTex[1] comme ci-dessous
					// A FAIRE : canoniser CChanTex
				pGeoMaker->m_TabChanTex[1] = new CChanTex();

				float* texvertex = LitTexVertex(numTexVertex);	// Lecture des sommets de texture

				CChanTex* chantex = pGeoMaker->m_TabChanTex[1];
				chantex->setTexVertex(numTexVertex, texvertex);

				if( m_bAffiche )
					trace() << "\nIl y a " << numTexVertex << " coordonn�es de texture dans l'objet g�om�trique " << nbr;
				accolade++; //LitTexVertex ne compte pas les accolades
			}
		}
		else if( mot=="*MESH_NUMTVFACES" ) {		//Recherche les �ventuels index de sommets de texture
			get(numTexFaces, __LINE__);

			if( numTexFaces ) {
				if( pGeoMaker->m_TabChanTex.size()==0 ) {
					trace() << "\nErreur (fichier::geomobject) : Fichier ASE corrompu";
					throw (int)CFichierASE::JKT_ERREUR_FICHIER_INVALIDE;	// Fin de fichier pr�matur�e
				}

				int* texfaces = LitTexFaces(numTexFaces);// Lecture des index de sommets de texture

				CChanTex* chantex = pGeoMaker->m_TabChanTex[1];
				chantex->setTexFaces(numTexFaces, texfaces);

				if( m_bAffiche )
					trace() << "\nIl y a " << numTexFaces << " index de texture dans l'objet g�om�trique " << nbr;
				accolade++;		//LitTexFaces ne compte pas les accolades
			}
		}
		else if( mot=="*MESH_MAPPINGCHANNEL" ) {
			int canal, numTVertex, numTFaces, var;
			float vertex1, vertex2, vertex3;
			int face1, face2, face3;

				// Cr�ation d'un nouveau canal de texture
			CChanTex* chantex = new CChanTex();

				// Num�ro de canal
			get(canal, __LINE__);
			findAccoladeFin(__LINE__);

				// Nombre de sommets de texture
			find("*MESH_NUMTVERTEX",__LINE__);
			get(numTVertex,__LINE__);	// Nombre d'index de texture du canal

			isNext("*MESH_TVERTLIST",__LINE__);
			findAccoladeDebut(__LINE__);

			if( m_bAffiche )
				trace() << "\nVERTEX DU CANAL DE TEXTURE " << canal;

				// Lecture des sommets de texture
			float* texvertex = new float[ 2*numTVertex ];

			for( int i=0 ; i<numTVertex ; i++ ) {
				get(mot, __LINE__);
				get(var, __LINE__);
				get(vertex1, __LINE__);
				get(vertex2, __LINE__);
				get(vertex3, __LINE__);	// Nombre d'index de texture du canal

				if( m_bAffiche )
					trace() << endl << i << " : " << vertex1 << "\t" << vertex2 << "\t" << vertex3;

				texvertex[ 2*i ] = vertex1;
				texvertex[ 2*i+1 ] = vertex2;
			}

			chantex->setTexVertex(numTVertex, texvertex);

				// Nombre d'index de sommets de texture
			findAccoladeFin(__LINE__);
			isNext("*MESH_NUMTVFACES",__LINE__);

			get(numTFaces,__LINE__);	// Nombre d'index de texture du canal
			isNext("*MESH_TFACELIST",__LINE__);

			findAccoladeDebut(__LINE__);	// Lecture de l'accolade ouvrante

			if( m_bAffiche )
				trace() << "\nINDEX DU CANAL DE TEXTURE " << canal;

			int* texfaces = new int[ 3*numTFaces ];

			for( int i=0 ; i<numTFaces ; i++ ) {	// Lecture des index de texture
				get(mot,__LINE__);
				get(mot,__LINE__);
				get(face1,__LINE__);
				get(face2,__LINE__);
				get(face3,__LINE__);	// Nombre d'index de texture du canal

				if( m_bAffiche )
					trace() << endl << i << " : " << face1 << "\t" << face2 << "\t" << face3;

				texfaces[ 3*i ] = face1;
				texfaces[ 3*i+1 ] = face2;
				texfaces[ 3*i+2 ] = face3;
			}

			chantex->setTexFaces(numTFaces, texfaces);

			pGeoMaker->m_TabChanTex[ canal ] = chantex;

			get(mot,__LINE__);	// On passe les 2 accolades fermantes
			get(mot,__LINE__);	// On passe les 2 accolades fermantes
		}
		else if( mot=="*MESH_NORMALS" ) {		//Recherche les vecteurs normaux
			float *tab = LitNormaux( numFaces, row );	// Lecture des vecteurs normaux aux sommets
			pGeoMaker->setNormals(numFaces*9, tab);

			accolade++;
		}
		else if( mot=="*MATERIAL_REF" ) {		//Recherche l'�ventuelle r�f�rence � un mat�riau
			unsigned int matRef;
			get(matRef,__LINE__);
			pGeoMaker->setMaterialRef( matRef );	// Associe l'objet au mat�riau

			if( m_bAffiche )
				trace() << "\nReference au mat�riau " << matRef << " dans l'objet geometrique " << nbr;

			if( numTexFaces ) {	// S'il y a des coordonn�es de texture
								// associe l'objet au mat�riau, sinon ignore le mat�riau
				pGeoMaker->setSubMat( Faces->texRef );
				Faces->texRef = 0;
			}

		}
		else if( mot=="{" )
			accolade++;
		else if( mot=="}" )
			accolade--;
	}

	pGeoMaker->setOffsetMateriau(0);	// Mode lecture de fichier ASE
	MapObject* geo = pGeoMaker->makeNewGeoInstance();

	if(Faces)
		delete Faces;

	delete pGeoMaker;	// D�truit l'objet
	pGeoMaker = 0;

	return geo;
}

void CFichierASE::LitGroupe(const string &nomFichier, CMap *pMap, unsigned int &nbr_GeoObject) {
	string mot;

	// Lit le nom du groupe
	get(mot,__LINE__);
	string groupName = extractGuillemetsDelimitedString(mot);

	findAccoladeDebut(__LINE__);

	while(isGet(mot, __LINE__)) {
		if(mot == "*GEOMOBJECT") {		// Recherche des objets g�om�triques
			MapObject* object = litGeomobject(pMap, nbr_GeoObject + 1);

			if(object) {	// Si pas d'erreur mets le dans le tableau d'objets g�o
				pMap->add(object);
				nbr_GeoObject++;
			}
			else {
				trace() << "\nAttention : Un objet geometrique est corrompu => ignore";
			}
		}
	}
}

void CFichierASE::LitFichier(const string &nomFichier, CMap *pMap) {
	unsigned int nbr_GeoObject = 0;
	string mot;

	if( m_bAffiche )
		trace() << "\nLecture du fichier ASE : " << nomFichier << endl;

	while(isGet(mot, __LINE__)) {
		if(mot == "**GROUP") {
			LitGroupe(nomFichier, pMap, nbr_GeoObject);
		}
		else if(mot == "*GEOMOBJECT") {		// Recherche des objets g�om�triques
			MapObject* object = litGeomobject(pMap, nbr_GeoObject + 1);

			if(object) {	// Si pas d'erreur mets le dans le tableau d'objets g�o
				pMap->add( object );
				nbr_GeoObject++;
			}
			else {
				trace() << "\nAttention : Un objet geometrique est corrompu => ignore";
			}
		}
		else if(mot == "*LIGHTOBJECT") {	// Recherche des lumi�res
			CLight *pLight;

			pLight = lightList(pMap);

			if(pLight) {	//Si pas d'erreur mets le dans le tableau d'objets g�o
				pMap->add( pLight );
			}
			else {			//Sinon lib�re sa m�moire
				trace() << "\nErreur : Une lumiere est corrompue => ignore";
			}
		}
		else if(mot == "*MATERIAL_LIST") {	// Recherche des mat�riaux
			CMaterial *pMaterial;
			int nbr_materiau;	// Nombre de mat�riaux du fichier ASE

			findAccoladeDebut(__LINE__);
			find("*MATERIAL_COUNT", __LINE__);
			get(nbr_materiau,__LINE__);		// R�cup�re le nombre de mat�riaux

			if(m_bAffiche)
				trace() << "Nombre de mat�riaux : " << nbr_materiau << endl;

			while(nbr_materiau) {	// Lecture de tous les mat�riaux
				pMaterial = materiallist();

				if(pMaterial)
					pMap->add(pMaterial) ;
				else {
					trace() << "\nErreur (fichier::" << __LINE__ << ") : Fichier ASE corrompu";
					throw (int)CFichierASE::JKT_ERREUR_FICHIER_MARTERIAUMANQUE;
				}

				if(m_bAffiche)
					trace() << "\nMateriau " << nbr_materiau << " repertorie\n";
				nbr_materiau--;
			}
		}
		else if(mot == "*HELPEROBJECT") {
			EntryPoint* entryPoint = litHelper(pMap);

			if(entryPoint != NULL) {
				pMap->add(entryPoint);
			}
		}
	}
}


bool CFichierASE::LitFichierASE(const string nomFichier, CMap *pMap, bool bAffiche) {
	string nomFichierASE = "./ase/" + nomFichier + ".ase";	// Ajout de l'extension et du chemin

	try {		// Ouverture du fichier ASE
		CFichierASE from( nomFichierASE, pMap, bAffiche );
	}
	catch(bool) {
		return false;
	}

	return true;
}

}	//namespace jkt
