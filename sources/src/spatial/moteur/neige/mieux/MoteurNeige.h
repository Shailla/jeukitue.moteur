#ifndef __JKT__MOTEURNEIGE_H
#define __JKT__MOTEURNEIGE_H

#include <queue>

#include "util/V3D.h"

#include "spatial/moteur/MoteurParticules.h"
#include "spatial/moteur/neige/mieux/Flocon.h"

class CGame;

namespace jkt
{

class MoteurNeige : public CMoteurParticules {
	int _nbrParticules;					// Nombre total de flocons
	int _nbrParticulesOnGround;			// Nombre de flocons au sole
	Flocon* _flocons;					// Liste de tous les flocons
	std::queue<Flocon*> _floconsOnGround;	// Liste des flocons actuellement au sol
	unsigned int _texName;				// Texture du flocon

	jkt::CV3D _centre;
	jkt::CV3D _taille;
	jkt::CV3D _vitesse;

	void GenereTextureParticule();

	/** Calcule une position al�atoire pour une nouvelle particule. */
	void randomPosition(Flocon& flocon);
	void randomVelocity(Flocon& flocon);

public:
	/**
	 * Constructeur.
	 *
	 * @param nbrParticules nombre de particules dans le moteur
	 * @param position position du centre du moteur de particules dans le sc�ne OpenGL
	 * @param taille vecteur dimension du parall�l�pip�de dans lequel sont affich�es les particules
	 */
	MoteurNeige(CMap* parent);
	~MoteurNeige();

	MapObject* clone() override;
	void init() noexcept(false) override;

	bool Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) noexcept(false) override;	// Lit l'objet depuis un fichier Map
	bool Save(TiXmlElement* element) noexcept(false) override;					// Sauve l'objet dans un fichier Map

	// Fonctions d'intialisation et libération
	void initGL() override;			// Initialisation de l'objet géométrique
	void freeGL() override;			// Libération des ressources de l'objet dans le contexte OpenGL

	/** Affiche la neige. */
	void affiche() override;								// Affiche l'ensemble des particules
	void afficheHighlighted(float r,float v,float b) override;
	void refresh(CGame* game) override;
};

}	// JktMoteur

#endif
