
#include <string>
#include <fstream>
#include <set>
#include <cmath>
#include <map>
#include <sstream>

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/GL.H>
#include <GL/GLU.H>

using namespace std;

class CGame;

extern int JKT_RenderMode;

#include "XmlVocabulaire.h"
#include "Tableau.cpp"
#include "math_vectoriel.h"
#include "V3D.h"
#include "Material.h"
#include "IfstreamMap.h"
#include "GeoMaker.h"
#include "Geo.h"
#include "Mouve.h"
#include "MaterialTexture.h"
#include "MaterialMulti.h"
#include "ChanTex.h"
#include "Light.h"
#include "Map.h"
#include "DemonSons.h"
#include "SPA.h"
#include "Player.h"
#include "contact.h"
#include "Couleurs.h"
#include "Cfg.h"

#include "GeoObject.h"

using namespace JKT_PACKAGE_UTILS;

extern CCfg Config;


void initTemps();
void setTemps(const string& note);

namespace JKT_PACKAGE_MOTEUR3D
{
class CGeoObject;
class CPorte;
class CNavette;

const char* CGeoObject::identifier = "GeoObject";

		//CONSTRUCTEURS
CGeoObject::CGeoObject( CMap *map, unsigned int nbrVertex, unsigned int nbrFaces )
	:CGeo( map )
{
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
}

CGeoObject::CGeoObject( CMap *map )
	:CGeo( map )
{
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
}

void CGeoObject::setVertex( float *tab, int num )
{
	if( m_TabVertex )	// Destruction de l'ancien tableau de sommets s'il existe
		delete[] m_TabVertex;

	m_NumVertex = num;	// Nombre de sommets

	m_TabVertex = new float[ 3*m_NumVertex ];	// Création du tableau de sommets
	
	for( int i=0 ; i< num ; i++ )	// Copie des données du tableau 
		for( int j=0 ; j<3 ; j++ )
			m_TabVertex[ (i*3)+j ] = tab[ (i*3)+j ];
}

void CGeoObject::setFaces( int *tab, int num )
{
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

void CGeoObject::AfficheWithMaterialSimple(CMaterial *mat)
{
	glDisable(GL_TEXTURE_2D);
	
	vector<CLight*> &tabLight = getMap()->m_TabLight;
	vector<CLight*>::iterator iterLight;
	
	if(tabLight.size())
	{
		glEnable( GL_LIGHTING );
		for( iterLight=tabLight.begin() ; iterLight!=tabLight.end() ; iterLight++ )
			(*iterLight)->Active();
	}
	else
	{
		glDisable(GL_LIGHTING);
		glColor3fv(mat->m_Diffuse);
		cout << endl << m_Color[0] << " " << m_Color[1] << " " << m_Color[2];
	}
	
	glLineWidth( 1 );
	
	mat->Active();

	if( m_TabVectNormaux )
	{
		glNormalPointer(GL_FLOAT, 0, m_TabVectNormaux);
		glEnableClientState(GL_NORMAL_ARRAY);
	}

	glDrawElements(JKT_RenderMode,3*m_NumFaces,GL_UNSIGNED_INT,m_TabFaces);

	if( m_TabVectNormaux )
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	mat->Desactive();

	if(tabLight.size())
	{
		for( iterLight=tabLight.begin() ; iterLight!=tabLight.end() ; iterLight++ )
			(*iterLight)->Desactive();
	}

		//AFFICHAGE DES VECTEURS NORMAUX
	if( Config.Debug.bAfficheNormaux )
	{
		float pos[3], bout[ 3 ];
		float facteur = 0.05f;
		if( m_TabVectNormaux )
		{	
			glBegin( GL_LINES );			
			for( int i=0; i < m_NumFaces ; i++ ) //dessine toutes les faces de la map
			{
				glColor3f( 1.0f, 0.0f, 0.0f );
				pos[ 0 ] = m_TabVertex[ m_TabFaces[3*i]*3 ];
				pos[ 1 ] = m_TabVertex[ m_TabFaces[3*i]*3 + 1 ];
				pos[ 2 ] = m_TabVertex[ m_TabFaces[3*i]*3 + 2 ];

				bout[ 0 ] = (m_TabVectNormaux[ (9*i)+0   ]*facteur) + pos[ 0 ];
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
}

void CGeoObject::AfficheSelection(float r,float v,float b)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glColor3f(r,v,b);
	
	glLineWidth( 1 );
	
	glVertexPointer( 3, GL_FLOAT, 0, m_TabVertex );	//Initialisation du tableau de sommets	
	glDrawElements(JKT_RenderMode,3*m_NumFaces,GL_UNSIGNED_INT,m_TabFaces);
}

const char* CGeoObject::toString() 
{
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
			cout << "\nAOIZODIJZDOIJZOAJDOJZ";
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

void CGeoObject::Affiche()
{
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
			cerr << "\nCGeoObject::Affiche (name=" << this->getName() << "): Materiau de type inconnu";
	}
	else	// Pas de texture associée
	{
		cout << endl << "Nothing : " << getName();
		glColor3fv( m_Color ); // Définit la couleur de l'objet géo.
		glDrawElements(GL_TRIANGLES, 3*m_NumFaces, GL_UNSIGNED_INT, m_TabFaces);
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

void CGeoObject::Scale( float scaleX, float scaleY, float scaleZ )
{
	for( int i=0 ; i<m_NumVertex ; i++ )
	{
		m_TabVertex[ (3*i) ] *= scaleX;
		m_TabVertex[ (3*i)+1 ] *= scaleY;
		m_TabVertex[ (3*i)+2 ] *= scaleZ;
	}

	if( m_TabVectNormaux )
	{ 	
		for( int i=0 ; i<(m_NumFaces*3) ; i++ )
		{
			m_TabVectNormaux[ (3*i)] *= scaleX;
			m_TabVectNormaux[ (3*i)+1] *= scaleY;
			m_TabVectNormaux[ (3*i)+2] *= scaleZ;

			normalise( &m_TabVectNormaux[ 3*i ] );
		}
	}
}

int CGeoObject::getOffsetMateriau() throw(CErreur)
{
	if(m_OffsetMateriau < 0) {
		throw CErreur(0,"m_OffsetMateriau non initialisé");
	}

	return m_OffsetMateriau;
}

void CGeoObject::setMaterial(int matRef)
{
	int nbrMat = matRef + getOffsetMateriau();	// Décalage de la référence matériau de l'offset demandé

		// Vérification du type de matériau
	CMap *map = getMap();
	
	if( nbrMat >= (int)map->m_TabMaterial.size() )
	{
		cerr << endl << "Erreur (CGeoObject::setMaterial) : Materiau introuvable 1 " << nbrMat << endl;
	}
	else 
	{
		CMaterial *mat = map->m_TabMaterial[ nbrMat ];
		if( mat != 0 )
		{
			m_bMaterialTexture = true;		// Indique la présence d'un matériau associé à l'objet	
			m_Material = mat;
			m_MaterialTexture = nbrMat;
			m_TypeMateriau = m_Material->Type();
		}
		else
		{
			cerr << endl << "Erreur (CGeoObject::setMaterial) : Materiau introuvable2 " << nbrMat << endl;
		}
	}
}

void CGeoObject::Color( float r, float g, float b )
{
	m_Color[0] = r;
	m_Color[1] = g;
	m_Color[2] = b;
}

/*bool CGeoObject::LitFichier( CIfstreamMap &fichier )
{
	return LitFichierGeoObject(fichier);
}*/

/*bool CGeoObject::LitFichierGeoObject( CIfstreamMap &fichier )
{
	string mot, nom;
	int i, nbrCanaux;
	char ch;

		// Lecture de la référence de l'object
	fichier >> mot;	
	if( mot!="Reference" )
	{
		cerr << __LINE__;
		return false;
	}
	unsigned int reference;
	fichier >> reference;

		// Lecture du nom de l'objet géo entre guillemets
	fichier >> mot;	
	if( mot!="Nom" )
	{
		cerr << "001";
		return false;
	}
	
	fichier >> ch;		// Recherche du guillemet ouvrant
	if( ch!='\"' )
	{
		cerr << "002";
		return false;
	}

	do
	{
		fichier.get( ch );
		if( ch=='\n' )
		{
			cerr << "003";
			return false;
		}

		if( ch!='\"' )
			nom += ch;
	}while( ch!='\"' );
		
	setName( nom );

	fichier >> mot;		// Présence matériau
	if( mot!="PresenceMateriau" )
	{
		cerr << "004";
		return false;
	}

	fichier >> m_bMaterialTexture;

	if( m_bMaterialTexture )	// Référence matériau
	{
		fichier >> mot;		
		if( mot!="ReferenceMateriau" )
		{
			cerr << "005";
			return false;
		}

		fichier >> m_MaterialTexture;
		setMaterial( m_MaterialTexture );
	}
	else
	{
		m_TypeMateriau = CMaterial::MAT_TYPE_SIMPLE;
	}

	fichier >> mot;		// Couleur
	if( mot!="Couleur" )
	{
		cerr << "006";
		return false;
	}

	fichier >> m_Color[0] >> m_Color[1] >> m_Color[2];	
	
	fichier >> mot;		// "Nombre de sommets"
	if( mot!="NombreDeSommets" )
	{
		cerr << "007";
		return false;
	}

	fichier >> m_NumVertex;

	fichier >> mot;		// "Nombre d'index de sommets"
	if( mot!="NombreDIndexDeSommets" )
	{
		cerr << "008";
		return false;
	}

	fichier >> m_NumFaces;

	fichier >> mot;
	if( mot!="NombreDeSommetsDeTexture" )
	{
		cerr << "009";
		return false;
	}

	fichier >> m_NumTexVertex;

	fichier >> mot;		// "Nombre d'index de sommets de texture"
	if( mot!="NombreDIndexDeSommetsDeTexture" )
	{
		cerr << "010";
		return false;
	}

	fichier >> m_NumTexFaces;

	fichier >> mot;
	if( mot!="NbrCanauxDeTexture" )
	{
		cerr << "011";
		return false;
	}

	fichier >> nbrCanaux;

	fichier >> mot;			// Solidité
	if( mot!="Solidité" )
	{
		cerr << "012";
		return false;
	}

	fichier >> m_bSolid;

	fichier >> mot;			// "Sommets"
	if( mot!="Sommets" )
	{
		cerr << "013";
		return false;
	}

	m_TabVertex = new float[ 3*m_NumVertex ];

	for( i=0 ; i<m_NumVertex ; i++ )	// Lecture des sommets
		fichier >> m_TabVertex[ 3*i ] >> m_TabVertex[ (3*i)+1 ] >> m_TabVertex[ (3*i)+2 ];

	fichier >> mot;	// "Index de sommets"
	if( mot!="IndexDeSommets" )
	{
		cerr << "014";
		return false;
	}
	
	m_TabFaces = new int[ 3*m_NumFaces ];
	
	if( m_TypeMateriau==CMaterial::MAT_TYPE_MULTI )	// Si le matériau associé est un matériau multiple
		m_TabSubMat = new int[ m_NumFaces ];

	for( i=0 ; i<m_NumFaces ; i++ )
	{
		fichier >> m_TabFaces[3*i] >> m_TabFaces[(3*i)+1] >> m_TabFaces[(3*i)+2];
		
		if( m_TypeMateriau==CMaterial::MAT_TYPE_MULTI )	// Si le matériau associé est un matériau multiple
		{
			fichier >> mot;
			if( mot!="*" )
			{
				cerr << "015";
				return false;
			}

			fichier >> m_TabSubMat[ i ];	// Lecture de la référence de sous-matériau
		}
	}

	int canal;
	TabTex.clear();	// Vide le tableau des canaux de texture (pas nécessaire normalement

	for( int k=0 ; k<nbrCanaux ; k++ )
	{
		fichier >> mot;		// Numéro de canal de texture
		if( mot!="CanalDeTexture" )
		{
			cerr << "016";
			return false;
		}

		fichier >> canal;	

		TabTex[ canal ] = new CChanTex();

		fichier >> mot;	// "Sommets de texture"
		if( mot!="SommetsDeTexture" )
		{
			cerr << "017";
			return false;
		}
		
		TabTex[ canal ]->setTexVertex(3*m_NumTexVertex, new float[3*m_NumTexVertex]);
		float *texVertex = TabTex[ canal ]->getTexVertex();

		for( i=0 ; i<m_NumTexVertex ; i++ )
			fichier >> texVertex[3*i] >> texVertex[(3*i)+1] >> texVertex[(3*i)+2];

		fichier >> mot;	// "Index de sommets de texture"
		if( mot!="IndexDeSommetsDeTexture" )
		{
			cerr << "018";
			return false;
		}

		TabTex[ canal ]->setTexFaces(3*m_NumTexFaces, new int[ 3*m_NumTexFaces ]);
		int *texFaces = TabTex[ canal ]->getTexFaces();

		for( i=0 ; i<m_NumTexFaces ; i++ )
			fichier >> texFaces[3*i] >> texFaces[(3*i)+1] >> texFaces[(3*i)+2];
	}

	fichier >> mot;
	if( mot!="VecteursNormaux" )
	{
		cerr << "019";
		return false;
	}

	m_TabVectNormaux = new float[ m_NumFaces * 9 ];

	for( int m=0 ; m < 9*m_NumFaces ; m++ )
		fichier >> m_TabVectNormaux[ m ];

	return true;
}*/

/*bool CGeoObject::SaveNameType( ofstream &fichier )
{
	fichier << "\n\nGeoObject\n";	// Cette classe est un GeoObject
	return true;
}*/

/*bool CGeoObject::SaveFichierMap( ofstream &fichier )
{	
	int i=0;
	
	fichier << "\tReference\t\t" << getReference();

	fichier << "\tNom\t\t\t\t\"" << getName() << '\"';	// Sauvegarde du nom de l'objet

	fichier << "\n\tPresenceMateriau\t\t" << m_bMaterialTexture;	// Materiau (présence ou non)
	if( m_bMaterialTexture )	// Référence matériau s'il existe
		fichier << "\n\tReferenceMateriau\t\t" <<	m_MaterialTexture;	

	fichier << "\n\tCouleur\t\t\t\t" << m_Color[0] << "\t" << m_Color[1] << "\t" << m_Color[2]; // Couleur
	
	fichier << "\n\tNombreDeSommets\t\t\t" << m_NumVertex;					// Nombre de sommets
	fichier << "\n\tNombreDIndexDeSommets\t\t" << m_NumFaces;				// Nombre d'index de sommets
	fichier << "\n\tNombreDeSommetsDeTexture\t" << m_NumTexVertex;		// Nombre de sommets de texture	
	fichier << "\n\tNombreDIndexDeSommetsDeTexture\t" << m_NumTexFaces;	// Nombre d'index de sommets de texture
	fichier << "\n\tNbrCanauxDeTexture\t\t" << (unsigned int)TabTex.size();	// Nombre de canaux de texture

	fichier << "\n\tSolidité\t\t\t" << m_bSolid;						// Solidité
	
		// Sommets
	fichier << "\n\n\tSommets";		
	for( i=0 ; i<m_NumVertex ; i++ )
		fichier << "\n\t\t" << m_TabVertex[3*i] << "\t" << m_TabVertex[(3*i)+1] << "\t" << m_TabVertex[(3*i)+2];

		// Index de sommets et références de sous-matériaux
	fichier << "\n\n\tIndexDeSommets";
	
	for( i=0 ; i<m_NumFaces ; i++ )
	{
		fichier << "\n\t\t" << m_TabFaces[3*i] << "\t" << m_TabFaces[(3*i)+1] << "\t" << m_TabFaces[(3*i)+2];

		if( m_TypeMateriau==CMaterial::MAT_TYPE_MULTI )		// Sauvegarde des références de texture en cas de
		{
			cout << endl << "Geo avec matMulti : " << getName() << endl;
			fichier << "\t* " << m_TabSubMat[i];	// texture multiple
		}
	}

		// Sauvegarde des canaux de texture
	for( map<int,CChanTex*>::iterator p=TabTex.begin() ; p!=TabTex.end() ; p++ )
	{
		fichier << "\n\n\tCanalDeTexture\t" << (*p).first;
		
		float *texVertex = (*p).second->getTexVertex();
		int *texFaces = (*p).second->getTexFaces();

			// Sommets de texture
		fichier << "\n\n\tSommetsDeTexture";
		for( i=0 ; i<m_NumTexVertex ; i++ )
			fichier << "\n\t\t" << texVertex[3*i] << "\t" << texVertex[(3*i)+1] << "\t" << texVertex[(3*i)+2];

			// Index des sommets de texture
		fichier << "\n\n\tIndexDeSommetsDeTexture";
		for( i=0 ; i<m_NumTexFaces ; i++ )
			fichier << "\n\t\t" << texFaces[3*i] << "\t" << texFaces[(3*i)+1] << "\t" << texFaces[(3*i)+2];
	}

	if( m_TabVectNormaux )
	{
		fichier << "\n\n\tVecteursNormaux";
		for( int m=0 ; m<m_NumFaces*3 ; m++ )
			fichier << "\n\t\t" <<  m_TabVectNormaux[m*3] << "\t" << m_TabVectNormaux[m*3+1] << "\t" << m_TabVectNormaux[m*3+2];
	}

	return true;
}*/

bool CGeoObject::Save(TiXmlElement* element)
{	
	int i=0;
	
		// Sauve les données générales
	TiXmlElement* elGeo = new TiXmlElement("Geo");
	elGeo->SetAttribute("Ref", getReference());
	elGeo->SetAttribute("Nom", getName());
	elGeo->SetAttribute("Type", "Simple material");
	element->LinkEndChild(elGeo);

		// Solidité
	CGeoMaker::SaveSolidite(elGeo, m_bSolid);

		// Présence matériau
	if(m_bMaterialTexture)
	{
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

void CGeoObject::Init()
{
	MinMax();			// Mesure les minimums et maximums de l'objet géo
	Bulle();			// Mesure le centre et le rayon de la sphère englobant l'objet géo
	ConstruitBase();	// Construit la table des vecteurs normaux
}


float CGeoObject::testContactTriangle( unsigned int i, const float posPlayer[3], float dist ) 
{		// Test s'il y a contact entre un point P et le i° triangle de l'objet géo
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
bool CGeoObject::Contact( const float pos[3], float dist )
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

void CGeoObject::GereContactPlayer( const float pos[3], CPlayer *player )
{	
	float dist = 0.1f;	// Rayon de la sphère représentant le volume du joueur
	float distanceW;
	if( m_bSolid )	// Si l'objet est solide
		if( TestContactPave( pos, 0.05f+dist ) )	// Teste proximité 'joueur / l'objet géo'
			for( int i=0; i<m_NumFaces; i++)		//pour chaque triangle de l'objet géo.
			{
				distanceW = testContactTriangle( i, pos, dist );

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

float CGeoObject::GereLaser(float pos[3], CV3D &Dir, float dist)
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

}	// JKT_PACKAGE_MOTEUR3D
