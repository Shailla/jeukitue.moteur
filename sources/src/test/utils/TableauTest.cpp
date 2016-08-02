/*
 * TableauTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <string>
#include <set>

#include "util/CollectionsUtils.h"
#include "util/StringUtils.h"
#include "util/TableauIndex.cpp"

#include "test/utils/TableauTest.h"

using namespace std;
using namespace jkt;

namespace jkt {

TableauTest::TableauTest() : Test("TableauTest") {
}

TableauTest::~TableauTest() {
}

string TableauTest::getDescription() {
	string description;

	description += "Tests réalisés :";
	description += "\n - Test du tableau indexé (TableauIndex)";

	return description;
}

void TableauTest::test() {
	TableauIndexTest();
}

void TableauTest::TableauIndexTest() {
	string un = "Un";
	string deux = "Deux";
	string trois = "Trois";
	string quatre = "Quatre";
	string cinq = "Cinq";
	string six = "Six";

	// -----------------------------------------------------
	log("Test - 'TableauIndex'", __LINE__);
	TableauIndex<string> tab(5, false);

	ASSERT_EQUAL(5, tab.getMax(), "KO, le tableau devrait avoir une contenance de 5");
	ASSERT_EQUAL(0, tab.getNbr(), "KO, le tableau devrait être vide");

	// Ajout d'éléments
	int id1 = tab.add(&un);
	int id2 = tab.add(&deux);
	int id3 = tab.add(&trois);
	int id4 = tab.add(&quatre);
	int id5 = tab.add(&cinq);
	int id6 = tab.add(&six);

	ASSERT_TRUE(id1 >= 0, "KO");
	ASSERT_TRUE(id2 >= 0, "KO");
	ASSERT_TRUE(id3 >= 0, "KO");
	ASSERT_TRUE(id4 >= 0, "KO");
	ASSERT_TRUE(id5 >= 0, "KO");

	ASSERT_EQUAL(-1, id6, "KO, tableau size should be exceeding");
	ASSERT_EQUAL(5, tab.getNbr(), "KO, le tableau devrait contenir 5 éléments");

	ASSERT_FALSE(tab.Supprime(-1), "KO, remove out of bound should return false");
	ASSERT_FALSE(tab.Supprime(-1), "KO, remove out of bound should return false");

	set<int> unicIds;
	unicIds.insert(id1);
	unicIds.insert(id2);
	unicIds.insert(id3);
	unicIds.insert(id4);
	unicIds.insert(id5);
	ASSERT_EQUAL(5, unicIds.size(), "KO, chaque identifiant devrait être unique");

	ASSERT_EQUAL(un, *tab[id1], "KO");
	ASSERT_EQUAL(deux, *tab[id2], "KO");
	ASSERT_EQUAL(trois, *tab[id3], "KO");
	ASSERT_EQUAL(quatre, *tab[id4], "KO");
	ASSERT_EQUAL(cinq, *tab[id5], "KO");

	ASSERT_EQUAL(0, tab[-1], "KO, hors limite du tableau il doit répondre 0");
	ASSERT_EQUAL(0, tab[5], "KO, hors limite du tableau il doit répondre 0");

	// On vide le tableau élément par élément
	ASSERT_TRUE(tab.Supprime(id1), "KO");
	ASSERT_TRUE(tab.Supprime(id2), "KO");
	ASSERT_TRUE(tab.Supprime(id3), "KO");
	ASSERT_TRUE(tab.Supprime(id4), "KO");
	ASSERT_TRUE(tab.Supprime(id5), "KO");

	ASSERT_EQUAL(0, tab.getNbr(), "KO, le tableau devrait être vide");

	// Ajout d'éléments à une position
	ASSERT_TRUE(tab.add(1, &cinq), "KO");
	ASSERT_TRUE(tab.add(4, &trois), "KO");
	ASSERT_TRUE(tab.add(3, &un), "KO");

	ASSERT_EQUAL(0, 	tab[0], 	"KO, hors limite du tableau il doit répondre 0");
	ASSERT_EQUAL(cinq, 	*tab[1], 	"KO");
	ASSERT_EQUAL(0, 	tab[2], 	"KO, hors limite du tableau il doit répondre 0");
	ASSERT_EQUAL(un, 	*tab[3], 	"KO");
	ASSERT_EQUAL(trois,	*tab[4], 	"KO");
	ASSERT_EQUAL(0, 	tab[5], 	"KO, hors limite du tableau il doit répondre 0");

	// Ajout d'élément sur une position déjà prise
	ASSERT_FALSE(tab.add(3, &deux), "KO");

	// Parcours des éléments
	TableauIndex<int> tabInt(5, false);
	int var;
	int curseur = -1;

	int a=10, b=20, c=30;

	tabInt.add(1, &a);
	tabInt.add(3, &b);
	tabInt.add(4, &c);

	// Premier élément
	ASSERT_EQUAL(1, tab.IndexSuivant(curseur), "KO");
	ASSERT_TRUE(tab.Suivant(curseur), "KO");
	var = *tabInt[curseur];
	ASSERT_EQUAL(10, var, "KO");

	// Second élément
	ASSERT_EQUAL(3, tab.IndexSuivant(curseur), "KO");
	ASSERT_TRUE(tab.Suivant(curseur), "KO");
	var = *tabInt[curseur];
	ASSERT_EQUAL(20, var, "KO");

	// Second élément
	ASSERT_EQUAL(4, tab.IndexSuivant(curseur), "KO");
	ASSERT_TRUE(tab.Suivant(curseur), "KO");
	var = *tabInt[curseur];
	ASSERT_EQUAL(30, var, "KO");

	ASSERT_EQUAL(-1, tab.IndexSuivant(curseur), "KO");
	ASSERT_FALSE(tab.Suivant(curseur), "KO, end of tableau should be attempted");

	tabInt.clear();
	ASSERT_EQUAL(0, tabInt.getMax(), "KO");
	ASSERT_EQUAL(0, tabInt.getNbr(), "KO");
}

} // JktTest
