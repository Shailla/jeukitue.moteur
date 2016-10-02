/*
 * UtilsTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <string>
#include <vector>

#include "ressource/RessourcesLoader.h"

#include "test/utils/RessourcesLoaderTest.h"

using namespace std;
using namespace jkt;

namespace jkt {

RessourcesLoaderTest::RessourcesLoaderTest() : Test("RessourcesLoaderTest") {
}

RessourcesLoaderTest::~RessourcesLoaderTest() {
}

string RessourcesLoaderTest::getDescription() {
	string description;

	description += "Tests réalisés :";
	description += "\n - Test de RessourcesLoader::getRessource";

	return description;
}

void RessourcesLoaderTest::test() {
	string directory, name;

	/* ********************************** */
	/* Tests sur getRessource             */
	/* ********************************** */

	// Cas où il n'y a pas de ressource
	bool result = RessourcesLoader::getRessource("Missile", directory, name);
	ASSERT_FALSE(result, "La ressource testée ne devrait pas être vue comme une ressource");

	// Cas limite, il manque un slash
	result = RessourcesLoader::getRessource("@Arme", directory, name);
	ASSERT_FALSE(result, "La ressource testée ne devrait pas être vue comme une ressource");

	// Cas nominal, c'est bien une ressource
	result = RessourcesLoader::getRessource("@Arme/Missile", directory, name);
	ASSERT_TRUE(result, "La ressource testée devrait être vue comme une ressource");
	ASSERT_EQUAL("./Ressources/Maps/Armes/Missile.map", directory, "Erreur");
	ASSERT_EQUAL("Missile.map.xml", name, "Erreur");

	// Cas limite, c'est bien une ressource mais avec un nom vide
	result = RessourcesLoader::getRessource("@Arme/", directory, name);
	ASSERT_TRUE(result, "La ressource testée devrait être vue comme une ressource");
	ASSERT_EQUAL("./Ressources/Maps/Armes/.map", directory, "Erreur");
	ASSERT_EQUAL(".map.xml", name, "Erreur");
}

} // JktTest
