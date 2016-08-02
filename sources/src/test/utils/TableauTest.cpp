/*
 * TableauTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <string>
#include <vector>

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
	// -----------------------------------------------------
	log("Test - 'TableauIndex'", __LINE__);
//	CTableauIndex<string> tab(5);
//	tab.Ajoute("Un");
//	tab.Ajoute("Deux");
//	tab.Ajoute("Trois");
//	tab.Ajoute("Quatre");
//	tab.Ajoute("Cinq");
//	tab.Ajoute("Six");

}

} // JktTest
