
#include <string>
#include <fstream>
#include <set>
#include <cmath>
#include <map>
#include <sstream>

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/GLU.H>

class CGame;

extern int JKT_RenderMode;

#include "spatial/XmlVocabulaire.h"
#include "util/Tableau.cpp"
#include "util/math_vectoriel.h"
#include "util/V3D.h"
#include "spatial/materiau/Material.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"
#include <spatial/basic/Geometrical.h>
#include <spatial/basic/Refreshable.h>
#include "spatial/materiau/MaterialTexture.h"
#include "spatial/materiau/MaterialMulti.h"
#include "spatial/geo/ChanTex.h"
#include "spatial/light/Light.h"
#include "spatial/Map.h"
#include "son/DemonSons.h"
#include "reseau/SPA.h"
#include "main/Player.h"
#include "spatial/contact.h"
#include "spatial/Couleurs.h"
#include "main/Cfg.h"

#include "GeoObject.h"

using namespace std;
using namespace jkt;

extern CCfg Config;

namespace jkt
{
class CGeoObject;
class CPorte;
class CNavette;

const char* CGeoObject::identifier = "GeoObject";

		//CONSTRUCTEURS
CGeoObject::CGeoObject(CMap *map, unsigned int nbrVertex, unsigned int nbrFaces) : MapObject(map, MapObject::GEO) {
	m_Color[0] = 1.0;	//Couleur blanche par défaut
	m_Color[1] = 1.0;
	m_Color[2] = 1.0;

	m_NumVertex = nbrVertex;
	m_NumFaces = nbrFaces;
	m_NumTexVertex = 0;
	m_NumTexFaces = 0;

	m_bMaterialTexture = false;			//Pas de référence à un matériau à l'initialisation
	m_OffsetMateriau = -1;
	m_TypeMateriau = CMaterial::MAT_TYPE_SIMPLE;
	m_Material = 0;
	m_MaterialTexture = 0;

	m_TabVertex = new float[3*nbrVertex];
	m_TabFaces = new int[3*nbrFaces];
	m_TabVectNormaux = 0;
	m_pNormalTriangle = 0;
	m_TabSubMat = 0;
	m_bSolid = true;		// Objet solide par défaut
	m_Rayon = 0.0f;
	maxX = maxY = maxZ = minX = minY = minZ = 0.0f;
}

CGeoObject::CGeoObject(CMap *map) : MapObject(map, MapObject::GEO) {
	m_Color[0] = 1.0;	//Couleur blanche par défaut
	m_Color[1] = 1.0;
	m_Color[2] = 1.0;

	m_NumVertex = 0;
	m_NumFaces = 0;
	m_NumTexVertex = 0;
	m_NumTexFaces = 0;

	m_TabVertex = 0;
	m_TabFaces = 0;
	m_TabSubMat = 0;
	m_TabVectNormaux = 0;
	m_pNormalTriangle = 0;

	m_bMaterialTexture = false;	//Pas de référence à un matériau à l'initialisation
	m_OffsetMateriau = -1;
	m_TypeMateriau = CMaterial::MAT_TYPE_SIMPLE;
	m_Material = 0;
	m_MaterialTexture = 0;

	m_bSolid = true;		// Objet solide par défaut
	m_Rayon = 0.0f;
	maxX = maxY = maxZ = minX = minY = minZ = 0.0f;
}

void CGeoObject::setVertex( float *tab, int num ) {
	if( m_TabVertex )	// Destruction de l'ancien tableau de sommets s'il existe
		delete[] m_TabVertex;

	m_NumVertex = num;	// Nombre de sommets

	m_TabVertex = new float[ 3*m_NumVertex ];	// Création du tableau de sommets

	for( int i=0 ; i< num ; i++ )	// Copie des données du tableau
		for( int j=0 ; j<3 ; j++ )
			m_TabVertex[ (i*3)+j ] = tab[ (i*3)+j ];
}

void CGeoObject::setFaces( int *tab, int num ) {
	if( m_TabFaces )	// Destruction de l'ancien tableau d'indices de sommets s'il existe
		delete[] m_TabFaces;

	m_NumFaces = num;	// Nombre d'indices de sommets

	m_TabFaces = new int[ 3*num ];	// Création du tableau d'indices de sommets

	for( int i=0 ; i< num ; i++ )	// Copie des données du tableau
	{
		for( int j=0 ; j<3 ; j++ )	// Copie des faces
		{
			m_TabFaces[ (i*3)+j ] = tab[ (i*3)+j ];
		}
	}
}

void CGeoObject::setSubMat( int *tab )
{
	if( m_TabSubMat )
		delete[] m_TabSubMat;

	m_TabSubMat = new int[ m_NumFaces ];	// Création du tableau des références
											// sous-matériaux

	for( int i=0 ; i<m_NumFaces ; i++ )
		m_TabSubMat[ i ] = tab[ i ];	// Copie des références sous-matériaux
}

void CGeoObject::setNormalVertex( float *tab )
{
	m_TabVectNormaux = new float[ m_NumFaces*9 ];

	for( int i=0 ; i<m_NumFaces ; i++)
	{
		for( int j=0 ; j<9 ; j++ )
			m_TabVectNormaux[ (i*9)+j ] = tab[ (i*9)+j ];
	}
}

void CGeoObject::AfficheWithMaterialSimple(CMaterial *mat) {
	glDisable(GL_TEXTURE_2D);

	vector<CLight*>& tabLight = getMap()->getLights();
	vector<CLight*>::iterator iterLight;

	if(tabLight.size()) {
		glEnable( GL_LIGHTING );

		for( iterLight=tabLight.begin() ; iterLight!=tabLight.end() ; iterLight++ )
			(*iterLight)->Active();
	}
	else {
		glDisable(GL_LIGHTING);
		glColor3fv(mat->m_Diffuse);
		cout << endl << m_Color[0] << " " << m_Color[1] << " " << m_Color[2];
	}

	glLineWidth( 1 );

	mat->Active();

	if( m_TabVectNormaux ) {
		glNormalPointer(GL_FLOAT, 0, m_TabVectNormaux);
		glEnableClientState(GL_NORMAL_ARRAY);
	}

	glDrawElements(JKT_RenderMode,3*m_NumFaces,GL_UNSIGNED_INT,m_TabFaces);

	if( m_TabVectNormaux ) {
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	mat->Desactive();

	if(tabLight.size()) {
		for( iterLight=tabLight.begin() ; iterLight!=tabLight.end() ; iterLight++ )
			(*iterLight)->Desactive();
	}

		//AFFICHAGE DES VECTEURS NORMAUX
	if( Config.Debug.bAfficheNormaux ) {
		float pos[3], bout[ 3 ];
		float facteur = 0.05f;

		if( m_TabVectNormaux ) {
			glBegin( GL_LINES );
			for( int i=0; i < m_NumFaces ; i++ ) //dessine toutes les faces de la map
			{
				glColor3f( 1.0f, 0.0f, 0.0f );
				pos[ 0 ] = m_TabVertex[ m_TabFaces[3*i]*3 ];
				pos[ 1 ] = m_TabVertex[ m_TabFaces[3*i]*3 + 1 ];
				pos[ 2 ] = m_TabVertex[ m_TabFaces[3*i]*3 + 2 ];

				bout[ 0 ] = (m_TabVectNormaux[ (9*i)+0 ]*facteur) + pos[ 0 ];
				bout[ 1 ] = (m_TabVectNormaux[ (9*i)+1 ]*facteur) + pos[ 1 ];
				bout[ 2 ] = (m_TabVectNormaux[ (9*i)+2 ]*facteur) + pos[ 2 ];

				glVertex3fv( pos );
				glVertex3fv( bout );

				glColor3f( 0.0f, 1.0f, 0.0f );
				pos[ 0 ] = m_TabVertex[ m_TabFaces[3*i+1]*3 ];
				pos[ 1 ] = m_TabVertex[ m_TabFaces[3*i+1]*3 + 1 ];
				pos[ 2 ] = m_TabVertex[ m_TabFaces[3*i+1]*3 + 2 ];

				bout[ 0 ] = (m_TabVectNormaux[ (9*i)+3 ]*facteur) + pos[ 0 ];
				bout[ 1 ] = (m_TabVectNormaux[ (9*i)+4 ]*facteur) + pos[ 1 ];
				bout[ 2 ] = (m_TabVectNormaux[ (9*i)+5 ]*facteur) + pos[ 2 ];

				glVertex3fv( pos );
				glVertex3fv( bout );

				glColor3f( 0.0f, 0.0f, 1.0f );
				pos[ 0 ] = m_TabVertex[ m_TabFaces[3*i+2]*3 ];
				pos[ 1 ] = m_TabVertex[ m_TabFaces[3*i+2]*3 + 1 ];
				pos[ 2 ] = m_TabVertex[ m_TabFaces[3*i+2]*3 + 2 ];

				bout[ 0 ] = (m_TabVectNormaux[ (9*i)+6 ]*facteur) + pos[ 0 ];
				bout[ 1 ] = (m_TabVectNormaux[ (9*i)+7 ]*facteur) + pos[ 1 ];
				bout[ 2 ] = (m_TabVectNormaux[ (9*i)+8 ]*facteur) + pos[ 2 ];

				glVertex3fv( pos );
				glVertex3fv( bout );
			}

			glEnd();
		}
	}

	glDisable(GL_LIGHTING);
}

void CGeoObject::AfficheHighlighted(float r,float v,float b) {
	glVertexPointer( 3, GL_FLOAT, 0, m_TabVertex );	//Initialisation du tableau de sommets

	glColor3f(r, v, b); // Définit la couleur de l'objet géo. sélectionné
	glDrawElements(GL_TRIANGLES, 3*m_NumFaces, GL_UNSIGNED_INT, m_TabFaces);

	//AFFICHAGE DES VECTEURS NORMAUX
	if( Config.Debug.bAfficheNormaux )
		AfficheNormals();
}

const char* CGeoObject::toString() {
	ostringstream ttt;
	ttt << identifier << " Nom=" << getName() << " Mat=";

	if(m_Material)
		ttt << m_Material->toString();

	tostring = ttt.str();

	return tostring.c_str();
}

void CGeoObject::AfficheWithMaterialTexture(CMaterialTexture *mat, int canal)
{
		// Initialisation pour la texture avec canal par défaut
	float *texVertex = TabTex[ canal ]->getTexVertex();
	int *texFaces = TabTex[ canal ]->getTexFaces();

	mat->Active();

	glLineWidth( 1 );

	glBegin( JKT_RenderMode );		//AVOIR : mettre les coord de tex dans glArrayElement
	for( int i=0; i < m_NumFaces ; i++ ) //dessine toutes les faces de la map
	{
		glTexCoord2fv( &texVertex[3*texFaces[3*i]] );
		glArrayElement( m_TabFaces[3*i] );

		glTexCoord2fv( &texVertex[3*texFaces[(3*i)+1]] );
		glArrayElement( m_TabFaces[(3*i)+1] );

		glTexCoord2fv( &texVertex[3*texFaces[(3*i)+2]] );
		glArrayElement( m_TabFaces[(3*i)+2] );
	}
	glEnd();

	mat->Desactive();
}

void CGeoObject::AfficheWithMaterialMultiTexture(CMaterialMulti *mat)
{
	for( int i=0; i < m_NumTexFaces ; i++ )	//dessine toutes les faces de la map
	{
		int canal = m_TabSubMat[i];
		if( mat->m_TabMat[ canal ]->Type() == CMaterial::MAT_TYPE_TEXTURE )
		{
			CMaterialTexture *matTex = (CMaterialTexture*)mat->m_TabMat[ canal ];
			float *texVertex = TabTex[ canal ]->getTexVertex();
			int *texFaces = TabTex[ canal ]->getTexFaces();

			matTex->Active();

			glLineWidth( 1 );

			glBegin( JKT_RenderMode );	//AVOIR : mettre les coord de tex dans glArrayElement
				glTexCoord2fv( &texVertex[3*texFaces[3*i]] );
				glArrayElement( m_TabFaces[3*i] );

				glTexCoord2fv( &texVertex[3*texFaces[(3*i)+1]] );
				glArrayElement( m_TabFaces[(3*i)+1] );

				glTexCoord2fv( &texVertex[3*texFaces[(3*i)+2]] );
				glArrayElement( m_TabFaces[(3*i)+2] );
			glEnd();

			matTex->Desactive();
		}
		else if( mat->m_TabMat[ m_TabSubMat[i] ]->Type() == CMaterial::MAT_TYPE_SIMPLE )
		{
/*			CMaterial *matSimple = mat->m_TabMat[ m_TabSubMat[i] ];
			float *texVertex = TabTex[ matSimple->m_CanalTex ]->TexVertex;
			int *texFaces = TabTex[ matSimple->m_CanalTex ]->TexFaces;

			glLineWidth( 1 );

			glBegin( JKT_RenderMode );	//AVOIR : mettre les coord de tex dans glArrayElement
				glArrayElement( m_TabFaces[3*i] );
				glArrayElement( m_TabFaces[(3*i)+1] );
				glArrayElement( m_TabFaces[(3*i)+2] );
			glEnd();*/
		}
	}

	glDisable( GL_TEXTURE_2D );
}

void CGeoObject::Affiche() {
	glVertexPointer( 3, GL_FLOAT, 0, m_TabVertex );	//Initialisation du tableau de sommets

	if( m_bMaterialTexture && m_Material )	// Y a-t-il un matériau associé
	{
		CMaterial *m = m_Material;

		if( m->Type() == CMaterial::MAT_TYPE_SIMPLE )			// Un matériau minimaliste est associé (propriétés diffuses, ambiantes et spéculaires)
			AfficheWithMaterialSimple( m );
		else if( m->Type() == CMaterial::MAT_TYPE_TEXTURE )		// Matériau associé == texture
			AfficheWithMaterialTexture( (CMaterialTexture*)m, 1 );
		else if( m->Type() == CMaterial::MAT_TYPE_MULTI )		// Matériau associé == texture multiple
			AfficheWithMaterialMultiTexture( (CMaterialMulti*)m );
		else
			cerr << endl << __FILE__ << ":" << __LINE__ << " CGeoObject::Affiche (name=" << this->getName() << "): Materiau de type inconnu";
	}
	else	// Pas de texture associée
	{
		cout << endl << "Nothing : " << getName();
		glColor3fv( m_Color ); // Définit la couleur de l'objet géo.
		glDrawElements(GL_TRIANGLES, 3*m_NumFaces, GL_UNSIGNED_INT, m_TabFaces);
	}

	//AFFICHAGE DES VECTEURS NORMAUX
	if( Config.Debug.bAfficheNormaux )
		AfficheNormals();
}

void CGeoObject::AfficheNormals() {
	float pos[3], bout[ 3 ];
	float facteur = 0.05f;

	if( m_TabVectNormaux ) {
		glBegin( GL_LINES );

		for( int i=0; i < m_NumVertex ; i++ ) //dessine toutes les faces de la map
		{
			glColor3f( 1.0f, 0.0f, 0.0f );
			pos[ 0 ] = m_TabVertex[ i*3 ];
			pos[ 1 ] = m_TabVertex[ i*3 + 1 ];
			pos[ 2 ] = m_TabVertex[ i*3 + 2 ];

			bout[ 0 ] = (m_TabVectNormaux[ (9*i)+0 ]*facteur) + pos[ 0 ];
			bout[ 1 ] = (m_TabVectNormaux[ (9*i)+1 ]*facteur) + pos[ 1 ];
			bout[ 2 ] = (m_TabVectNormaux[ (9*i)+2 ]*facteur) + pos[ 2 ];

			glVertex3fv( pos );
			glVertex3fv( bout );
		}
		glEnd();
	}
}

void CGeoObject::MinMax()
{
	minX = maxX = m_TabVertex[ 0 ];
	minY = maxY = m_TabVertex[ 1 ];
	minZ = maxZ = m_TabVertex[ 2 ];

	for( int i=1 ; i<m_NumVertex ; i++ )
	{
		if( m_TabVertex[3*i] < minX )		//récupération des coordonnées du pavé englobant
			minX = m_TabVertex[3*i];		//l'objet géo
		if( m_TabVertex[(3*i)+1] < minY )
			minY = m_TabVertex[(3*i)+1];
		if( m_TabVertex[(3*i)+2] < minZ )
			minZ = m_TabVertex[(3*i)+2];

		if( m_TabVertex[3*i] > maxX )
			maxX = m_TabVertex[3*i];
		if( m_TabVertex[(3*i)+1] > maxY )
			maxY = m_TabVertex[(3*i)+1];
		if( m_TabVertex[(3*i)+2] > maxZ )
			maxZ = m_TabVertex[(3*i)+2];
	}
}

void CGeoObject::Bulle()
{
	float r0, r1, r2;
		// Calcul du centre de la sphère à partir des valeurs min/max
	m_Centre[0] = (minX+maxX)/2.0f;
	m_Centre[1] = (minY+maxY)/2.0f;
	m_Centre[2] = (minZ+maxZ)/2.0f;

		// Recherche du rayon de la sphère
	r0 = fabsf( minX-maxX );
	r1 = fabsf( minY-maxY );
	r2 = fabsf( minZ-maxZ );

	m_Rayon = sqrtf( (r0*r0) + (r1*r1) + (r2*r2) );
}

void CGeoObject::ConstruitBase()
{
	float X[3], Y[3];

	if( m_pNormalTriangle )
		delete[] m_pNormalTriangle;

	m_pNormalTriangle = new float[ 3*m_NumFaces ];

	for( int i=0; i<m_NumFaces; i++ ) //pour chaque triangle de l'objet géo.
	{
			// X = vecteur coté AB
		X[0] = m_TabVertex[ 3*m_TabFaces[(3*i)+1]] - m_TabVertex[ 3*m_TabFaces[3*i] ];
		X[1] = m_TabVertex[ (3*m_TabFaces[(3*i)+1])+1 ] - m_TabVertex[ (3*m_TabFaces[3*i])+1 ];
		X[2] = m_TabVertex[ (3*m_TabFaces[(3*i)+1])+2 ] - m_TabVertex[ (3*m_TabFaces[3*i])+2 ];

			// Y = vecteur coté AC
		Y[0] = m_TabVertex[3*m_TabFaces[(3*i)+2]] - m_TabVertex[3*m_TabFaces[3*i]];
		Y[1] = m_TabVertex[(3*m_TabFaces[(3*i)+2])+1] - m_TabVertex[(3*m_TabFaces[3*i])+1];
		Y[2] = m_TabVertex[(3*m_TabFaces[(3*i)+2])+2] - m_TabVertex[(3*m_TabFaces[3*i])+2];

		produitVectoriel(X, Y, &m_pNormalTriangle[3*i]);	// calcul du vecteur normal au plan
															// du triangle
		normalise( &m_pNormalTriangle[3*i] );		// normalise ce vecteur
	}
}

		//DESTRUCTEUR
CGeoObject::~CGeoObject()
{
	if( m_TabVertex )
	{
		delete[] m_TabVertex;
		m_TabVertex = 0;
	}

	if( m_TabFaces )
	{
		delete[] m_TabFaces;
		m_TabFaces = 0;
	}

	if( m_TabSubMat )
	{
		delete[] m_TabSubMat;			// Index des éventuels sous-matériau
		m_TabSubMat = 0;
	}

	if( m_pNormalTriangle )
	{
		delete[] m_pNormalTriangle;	// Pointeur sur le tableau des vecteurs orthogonaux aux surfaces des triangles (calculs préliminaires à la gestion des contacts)
		m_pNormalTriangle = 0;
	}

	if( m_TabVectNormaux )
	{
		delete[] m_TabVectNormaux;	// Pointeur sur le tableau des vecteurs normaux
		m_TabVectNormaux = 0;
	}

	for( map<int,CChanTex*>::iterator p=TabTex.begin() ; p!=TabTex.end() ; p++ )
	{
		delete (*p).second;
	}
}

void CGeoObject::EchangeXY()	// Echange les axes X et Y de l'objet
{
	float varX, varY;

	for( int i=0 ; i<m_NumVertex ; i++ )
	{
		varX = m_TabVertex[ 3*i ];
		varY = m_TabVertex[ (3*i)+1 ];

		m_TabVertex[ 3*i ] = varY;
		m_TabVertex[ (3*i)+1 ] = varX;
	}

	if( m_TabVectNormaux )
	{
		for( int i=0 ; i<(m_NumFaces*3) ; i++ )
		{
			varX = m_TabVectNormaux[ 3*i ];
			varY = m_TabVectNormaux[ (3*i)+1 ];

			m_TabVectNormaux[ 3*i ] = varY;
			m_TabVectNormaux[ (3*i)+1 ] = varX;
		}
	}
}

void CGeoObject::EchangeXZ()	// Echange les axes X et Y de l'objet
{
	float varX, varZ;

	for( int i=0 ; i<m_NumVertex ; i++ )
	{
		varX = m_TabVertex[ 3*i ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ 3*i ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varX;
	}

	if( m_TabVectNormaux )
	{
		for( int i=0 ; i<(m_NumFaces*3) ; i++ )
		{
			varX = m_TabVectNormaux[ 3*i ];
			varZ = m_TabVectNormaux[ (3*i)+2 ];

			m_TabVectNormaux[ 3*i ] = varZ;
			m_TabVectNormaux[ (3*i)+2 ] = varX;
		}
	}
}

void CGeoObject::EchangeYZ()	// Echange les axes X et Y de l'objet
{
	float varY, varZ;

	for( int i=0 ; i<m_NumVertex ; i++ )
	{
		varY = m_TabVertex[ (3*i)+1 ];
		varZ = m_TabVertex[ (3*i)+2 ];

		m_TabVertex[ (3*i)+1 ] = varZ;
		m_TabVertex[ (3*i)+2 ] =varY;
	}

	if( m_TabVectNormaux )
	{
		for( int j=0 ; j<(m_NumFaces*3) ; j++ )
		{
			varZ = m_TabVectNormaux[ (3*j)+2 ];
			varY = m_TabVectNormaux[ (3*j)+1 ];

			m_TabVectNormaux[ (3*j)+2 ] = varY;
			m_TabVectNormaux[ (3*j)+1 ] = varZ;
		}
	}
}

void CGeoObject::Scale( float scaleX, float scaleY, float scaleZ ) {
	for( int i=0 ; i<m_NumVertex ; i++ ) {
		m_TabVertex[ (3*i)+0 ] *= scaleX;
		m_TabVertex[ (3*i)+1 ] *= scaleY;
		m_TabVertex[ (3*i)+2 ] *= scaleZ;
	}

	if( m_TabVectNormaux ) {
		for( int i=0 ; i<(m_NumFaces*3) ; i++ ) {
			m_TabVectNormaux[ (3*i)+0 ] *= scaleX;
			m_TabVectNormaux[ (3*i)+1 ] *= scaleY;
			m_TabVectNormaux[ (3*i)+2 ] *= scaleZ;

			normalise( &m_TabVectNormaux[ 3*i ] );
		}
	}
}

void CGeoObject::translate( float x, float y, float z ) {
	for( int i=0 ; i<m_NumVertex ; i++ ) {
		m_TabVertex[ (3*i)+0 ] += x;
		m_TabVertex[ (3*i)+1 ] += y;
		m_TabVertex[ (3*i)+2 ] += z;
	}
}

int CGeoObject::getOffsetMateriau() throw(CErreur)
{
	if(m_OffsetMateriau < 0) {
		throw CErreur("m_OffsetMateriau non initialisé");
	}

	return m_OffsetMateriau;
}

void CGeoObject::setMaterial(int matRef)
{
	int nbrMat = matRef + getOffsetMateriau();	// Décalage de la référence matériau de l'offset demandé

		// Vérification du type de matériau
	CMap *map = getMap();

	if( nbrMat >= (int)map->_materials.size() ) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CGeoObject::setMaterial) : Materiau introuvable 1 " << nbrMat << endl;
	}
	else {
		CMaterial *mat = map->_materials[ nbrMat ];
		if( mat != 0 ) {
			m_bMaterialTexture = true;		// Indique la présence d'un matériau associé à l'objet
			m_Material = mat;
			m_MaterialTexture = nbrMat;
			m_TypeMateriau = m_Material->Type();
		}
		else {
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur (CGeoObject::setMaterial) : Materiau introuvable2 " << nbrMat << endl;
		}
	}
}

void CGeoObject::Color( float r, float g, float b ) {
	m_Color[0] = r;
	m_Color[1] = g;
	m_Color[2] = b;
}

bool CGeoObject::Save(TiXmlElement* element) {
	int i=0;

		// Sauve les données générales
	TiXmlElement* elGeo = new TiXmlElement("Geo");
	elGeo->SetAttribute("Ref", getId());
	elGeo->SetAttribute("Nom", getName());
	elGeo->SetAttribute("Type", "Simple material");
	element->LinkEndChild(elGeo);

		// Solidité
	CGeoMaker::SaveSolidite(elGeo, m_bSolid);

		// Présence matériau
	if(m_bMaterialTexture) {
		TiXmlElement* elMat = new TiXmlElement("Materiau");
		elMat->SetAttribute("Ref", m_MaterialTexture);
		elGeo->LinkEndChild(elMat);
	}

		// Couleur
	CGeoMaker::SaveCouleur3fv(elGeo, Xml::COULEUR, m_Color);

		// Sommets
	CGeoMaker::SaveVertex(elGeo, m_NumVertex, m_TabVertex);

		// Index de sommets
	CGeoMaker::SaveFaces(elGeo, m_NumFaces, m_TabFaces);

		// Index de sommets et références de sous-matériaux
	TiXmlElement* elFac = new TiXmlElement("IndexDeSommets");
	elGeo->LinkEndChild(elFac);

	for( i=0 ; i<m_NumFaces ; i++ )
	{
		TiXmlElement* el = new TiXmlElement("Face");
		el->SetAttribute("S1", m_TabFaces[3*i + 0]);
		el->SetAttribute("S2", m_TabFaces[3*i + 1]);
		el->SetAttribute("S3", m_TabFaces[3*i + 2]);

		if( m_TypeMateriau==CMaterial::MAT_TYPE_MULTI )		// Sauvegarde des références de texture en cas de
		{
			el->SetAttribute("SubMat", m_TabSubMat[i]);	// texture multiple
		}

		elFac->LinkEndChild(el);
	}

		// Canaux de texture
	TiXmlElement* elCan = new TiXmlElement("Canaux");
	elCan->SetAttribute("Nbr", (int)TabTex.size());
	for( map<int,CChanTex*>::iterator p=TabTex.begin() ; p!=TabTex.end() ; p++ )
	{
		TiXmlElement* el = new TiXmlElement("Canal");
		el->SetAttribute("Id",(*p).first);

		float *texVertex = (*p).second->getTexVertex();
		int *texFaces = (*p).second->getTexFaces();

			// Sommets de texture
		CGeoMaker::SaveTexVertex(el, m_NumTexVertex, texVertex);

			// Index des sommets de texture
		CGeoMaker::SaveTexIndex(el, m_NumTexFaces*3, texFaces);
	}

	if( m_TabVectNormaux )
		CGeoMaker::SaveVecteursNormaux(elGeo, m_NumFaces*3, m_TabVectNormaux);

	return true;
}

void CGeoObject::init() throw(CErreur) {
	MinMax();			// Mesure les minimums et maximums de l'objet géo
	Bulle();			// Mesure le centre et le rayon de la sphère englobant l'objet géo
	ConstruitBase();	// Construit la table des vecteurs normaux
}


float CGeoObject::testContactTriangle( unsigned int i, const float posPlayer[3], float dist ) {		// Test s'il y a contact entre un point P et le i° triangle de l'objet géo
	int *indices = &m_TabFaces[3*i];
	float *normal = &m_pNormalTriangle[3*i];
	float A[3], B[3], C[3], F[3], G[3], X[3], Y[3], Z[3];
	float distanceW;

		// Z = vecteur AP, P personnage
	Z[0] =  posPlayer[0] - m_TabVertex[ (3*indices[0])+0 ];
	Z[1] =  posPlayer[1] - m_TabVertex[ (3*indices[0])+1 ];
	Z[2] = -posPlayer[2] - m_TabVertex[ (3*indices[0])+2 ];

	distanceW = produitScalaire(normal, Z); //distance entre le plan du triangle et le joueur
	if( distanceW >= dist )
		return 1000.0f;		// Y'a pas contact

	if( distanceW <= - dist )
		return 1000.0f;		// Y'a pas contact

	X[0] = m_TabVertex[ (3*indices[1])+0 ] - m_TabVertex[ (3*indices[0])+0 ];
	X[1] = m_TabVertex[ (3*indices[1])+1 ] - m_TabVertex[ (3*indices[0])+1 ];
	X[2] = m_TabVertex[ (3*indices[1])+2 ] - m_TabVertex[ (3*indices[0])+2 ];

	produitVectoriel( X, normal, A );
	normalise( A );
		// test position P / droite AB
	if( produitScalaire(A, Z) >= dist )	// avant c'était 0.0
		return 1000.0f;

	Y[0] = m_TabVertex[ (3*indices[2])+0 ] - m_TabVertex[ (3*indices[0])+0 ];
	Y[1] = m_TabVertex[ (3*indices[2])+1 ] - m_TabVertex[ (3*indices[0])+1 ];
	Y[2] = m_TabVertex[ (3*indices[2])+2 ] - m_TabVertex[ (3*indices[0])+2 ];

	produitVectoriel(Y, normal, B);
	normalise(B);
		// test position P / droite AC
	if( produitScalaire(B, Z) <= -dist )	// avant c'était 0.0
		return 1000.0f;

		// calcul de F = vecteur BC
	F[0] = Y[0] - X[0];
	F[1] = Y[1] - X[1];
	F[2] = Y[2] - X[2];

		// G = vecteur BP
	G[0] = Z[0]-X[0];
	G[1] = Z[1]-X[1];
	G[2] = Z[2]-X[2];

	produitVectoriel(F, normal, C);
	normalise(C);
		// test position P / droite BC
	if( produitScalaire(C, G)>= dist )
		return 1000.0f;

	return distanceW;
}

	// Renvoie la distance entre le point de position 'pos' et le plus proche triangle de l'objet
	// géo. N'effectue cette mesure que pour des distances inférieures à 'dist'
bool CGeoObject::checkContact( const float pos[3], float dist )
{
	float distanceW;
	if( TestContactPave( pos, dist ) )	// Teste proximité 'joueur / l'objet géo'
	{
		for( int i=0; i<m_NumFaces; i++)		//pour chaque triangle de l'objet géo.
		{
			distanceW = testContactTriangle( i, pos, dist );
			if( distanceW <= dist )
				return true;
		}
	}

	return false;
}

void CGeoObject::GereContactPlayer(float positionPlayer[3], CPlayer *player) {
	float dist = player->getRayon();	// Rayon de la sphère représentant le volume du joueur
	float distanceW;

	if( m_bSolid )	// Si l'objet est solide
		if( TestContactPave( positionPlayer, dist ) )	// Teste proximité 'joueur / l'objet géo'
			for( int i=0; i<m_NumFaces; i++) {		//pour chaque triangle de l'objet géo.
				distanceW = testContactTriangle( i, positionPlayer, dist );

				if( distanceW<500.0f ) // Teste le contact avec le joueur (1.0f = valeur arbitraire mais grande)
					player->exeContactFunc( &m_pNormalTriangle[3*i], distanceW );	// On a contact !
			}
}

bool CGeoObject::TestContactPave( const float pos[3], float dist )
{
	// Teste si le point qui a pour position 'pos' se trouve ou non à une distance inférieure à 'dist'
	// du pavé englobant l'objet

	if( pos[0] < maxX+dist )
		if( pos[1] < maxY+dist )
			if( -pos[2] < maxZ+dist )
				if( pos[0] > minX-dist )
					if( pos[1] > minY-dist )
						if( -pos[2] > minZ-dist )
							return true;	// Le point 'pos' est à une distance inférieure

	return false;	// Le point 'pos' se trouve à une distance supérieure
}

void CGeoObject::setOffsetMateriau(int offset)
{
	m_OffsetMateriau = offset;
}

float CGeoObject::GereLaserPlayer(float pos[3], CV3D &Dir, float dist)
{
	float distanceVar;
	float *vertex, *normal;
	int *indices;
	CV3D AP, N, AB, AC, U, V, W, var;

	if( !m_bSolid )	// Si l'objet n'est pas solide, il ne peut être touché par le laser
		return dist;	// => on sort en renvoyant 'dist'

		// Vérifie si le laser passe à proxomité de l'objet géo
	CV3D CP;	// Vecteur allant du point origine du laser au centre de l'objet géo (=centre de la Bulle qui l'englobe)
	CP.X = m_Centre[0] - pos[0];
	CP.Y = m_Centre[1] - pos[1];
	CP.Z = m_Centre[2] + pos[2];

	float a = CP^Dir;				// Dir est normalisé
	float bCarre = CP^CP;			// b = norme du veteur CP au carré
	float hCarre = bCarre - (a*a);	// b²=bCarre = h² + a² => h²=hCarre = b² - a²
	float rCarre = (0.001f+m_Rayon)*(0.001f+m_Rayon);
	if( hCarre > rCarre )			// Si distance (laser-centre de la sphère) > rayon de la sphère
		return dist;				// Alors le rayon laser ne touche pas l'objet

		// Vérifie si le laser touche une face de la map
	vertex = m_TabVertex;
	for( int i=0; i<m_NumFaces; i++) //pour chaque triangle de l'objet géo.
	{
		indices = &m_TabFaces[3*i];
		normal = &m_pNormalTriangle[3*i];

			// vecteur AP, P personnage, A point du triangle
		AP.X = pos[0]-vertex[ 3*indices[0] ];			//horizontal
		AP.Y = pos[1]-vertex[ (3*indices[0]) + 1 ];		//vertical
		AP.Z = -pos[2]-vertex[ (3*indices[0]) + 2 ];	//horizontal

			// normale au triangle
		N.X = normal[0];
		N.Y = normal[1];
		N.Z = normal[2];

		distanceVar = -(N^AP)/(N^Dir);

		if( distanceVar < 0.0f )	// Si la distance trouvée pour ce triangle est négative
			continue;	// Passe au triangle suivant
		if( distanceVar > dist )	// Si elle est supérieure à une autre déjà trouvée
			continue;	// Passe au triangle suivant

		AB.X = vertex[ 3*indices[1] ] - vertex[ 3*indices[0] ];
		AB.Y = vertex[ (3*indices[1])+1 ] - vertex[ (3*indices[0])+1 ];
		AB.Z = vertex[ (3*indices[1])+2 ] - vertex[ (3*indices[0])+2 ];

		V = (Dir*distanceVar)+AP;
		var = AB*V;

		if( (N^var)<0.0f )	// Si le laser passe du mauvais côté du premier segment du triangle
			continue;	// Passe au triangle suivant

		AC.X = vertex[ 3*indices[2] ] - vertex[ 3*indices[0] ];
		AC.Y = vertex[ (3*indices[2])+1 ] - vertex[ (3*indices[0])+1 ];
		AC.Z = vertex[ (3*indices[2])+2 ] - vertex[ (3*indices[0])+2 ];

		var = AC*V;

		if( (N^var)>0.0f )	// S'il passe du mauvais côté du second segment du triangle
			continue;	// Passe au triangle suivant

			// Ici on est passé du bon côté du second segment du triangle
		U = AC - AB;
		W = V - AB;

		var = W*U;

		if( (N^var)<=0.0f )	// S'il passe du bon côté du dernier segment du triangle
			dist = distanceVar;	// C'est qu'il le touche
	}

	return dist;	// Renvoie la distance trouvée
}

}	// JktMoteur
