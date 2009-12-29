
#include "SDL.h"

#include "util/V3D.h"

#include "spatial/Particule.h"

namespace JKT_PACKAGE_MOTEUR3D
{

CParticule::CParticule()
{
	position.X = position.Y = position.Z = 0.0f;
	vitesse.X = vitesse.Y = vitesse.Z = 0.0f;
	acceleration.X = acceleration.Y = acceleration.Z = 0.0f;

	masse = 0.0f;
	date2naissance = SDL_GetTicks();
	duree2vie = 0;
	visible = false;
}

CParticule::CParticule( CV3D pos, CV3D vit, float m, unsigned int duree )
{
	position = pos;				// Position initiale de la particule
	vitesse = vit;				// Vitesse initiale
	acceleration.X = acceleration.Y = acceleration.Z = 0.0f;

	masse = m;					// Masse de la particule
	date2naissance = SDL_GetTicks();
	duree2vie = duree;
	visible = false;
}

void CParticule::Calcule()
{
		vitesse += acceleration;
		position += vitesse;		// Vitesse relative + vitesse absolue
}

bool CParticule::Vie( unsigned int temps )
{
	if( temps-date2naissance<duree2vie )	// Si la particule est trop vieille ou invisible
		return true;
	else
		return false;						// Elle doit disparaitre
}

void CParticule::Reset( CV3D pos, CV3D vit )	// Fait renaître la particule
{
	date2naissance = SDL_GetTicks();		// Date sa naissance
	visible = true;							// Rend-la visible
	position = pos;							// Donne lui sa position
	vitesse = vit;							// et sa vitesse
}

}	// JKT_PACKAGE_MOTEUR3D
