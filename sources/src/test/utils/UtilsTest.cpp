/*
 * UtilsTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <string>
#include <vector>

#include "util/CollectionsUtils.h"
#include "util/StringUtils.h"
#include "util/TableauIndex.h"

#include "test/utils/UtilsTest.h"

using namespace std;
using namespace jkt;

namespace jkt {

UtilsTest::UtilsTest() : Test("UtilsTest") {
}

UtilsTest::~UtilsTest() {
}

string UtilsTest::getDescription() {
	string description;

	description += "Tests réalisés :";
	description += "\n - Test des méthodes de StringUtils";

	return description;
}

void UtilsTest::test() {
	stringUtilsTest();
	containsTest();
}

void UtilsTest::stringUtilsTest() {
	log("Test - 'StringUtils::isBlank'", __LINE__);
	ASSERT_TRUE(StringUtils::isBlank(""), "'isBlank' KO");
	ASSERT_TRUE(StringUtils::isBlank("  "), "'isBlank' KO");
	ASSERT_TRUE(StringUtils::isBlank("\t"), "'isBlank' KO");
	ASSERT_TRUE(StringUtils::isBlank("  \t"), "'isBlank' KO");
	ASSERT_FALSE(StringUtils::isBlank("  nn "), "'isBlank' KO");

	// -----------------------------------------------------
	log("Test - 'StringUtils::isBlank'", __LINE__);
	string ltrim = "  hello jkt    ";
	StringUtils::ltrim(ltrim);
	ASSERT_EQUAL("hello jkt    ", ltrim, "'ltrim' KO");

	string rtrim = "  hello jkt    ";
	StringUtils::rtrim(rtrim);
	ASSERT_EQUAL("  hello jkt", rtrim, "'rtrim' KO");

	string trim = "  hello jkt    ";
	StringUtils::trim(trim);
	ASSERT_EQUAL("hello jkt", trim, "'trim' KO");

	// -----------------------------------------------------
	log("Test - 'StringUtils::find...erase'", __LINE__);

	string faefw = "   Coucou        comment    tu    vas ";
	string faefwR = StringUtils::findAndEraseFirstWord(faefw);
	ASSERT_EQUAL("        comment    tu    vas ", faefw, "'findAndEraseFirstWord' KO");
	ASSERT_EQUAL("Coucou", faefwR, "'findAndEraseFirstWord' KO");

	string faefs = "   \"  Coucou \"        \"comment\"    \"tu    vas \" ";
	string faefsR = StringUtils::findAndEraseFirstString(faefs, StringUtils::isGuillemet);
	ASSERT_EQUAL("        \"comment\"    \"tu    vas \" ", faefs, "'findAndEraseFirstString' KO");
	ASSERT_EQUAL("  Coucou ", faefsR, "'findAndEraseFirstString' KO");

	ASSERT_EQUAL("hello", StringUtils::findFirstWord("   hello  you, how are you   "), "'findAndEraseFirstWord' KO");


	// -----------------------------------------------------
	log("Test - 'StringUtils::is...'", __LINE__);

	ASSERT_EQUAL(true, StringUtils::isFinishingWith("abcdefghijkl", "jkl"), "'isFinishingWith' KO");
	ASSERT_EQUAL(false, StringUtils::isFinishingWith("abc", "jkl"), "'isFinishingWith' KO");
	ASSERT_EQUAL(false, StringUtils::isFinishingWith("abcdefghijklm", "jkl"), "'isFinishingWith' KO");

	ASSERT_EQUAL(true, StringUtils::isBeginingWith("abcdefghijkl", "abcde"), "'isBeginingWith' KO");
	ASSERT_EQUAL(false, StringUtils::isBeginingWith("abc", "jkl"), "'isBeginingWith' KO");
	ASSERT_EQUAL(false, StringUtils::isBeginingWith("abcdefghijklm", "klm"), "'isBeginingWith' KO");

	// -----------------------------------------------------
	log("Test - 'StringUtils::split'", __LINE__);
	vector<string> split = StringUtils::split(" \"Coucou\"  \" comment tu \"\" vas \" ", StringUtils::isGuillemet);
	ASSERT_EQUAL(6, split.size(), "'split' KO");
	ASSERT_EQUAL(" ", split[0], "'split' KO");
	ASSERT_EQUAL("Coucou", split[1], "'split' KO");
	ASSERT_EQUAL("  ", split[2], "'split' KO");
	ASSERT_EQUAL(" comment tu ", split[3], "'split' KO");
	ASSERT_EQUAL(" vas ", split[4], "'split' KO");
	ASSERT_EQUAL(" ", split[5], "'split' KO");

	// -----------------------------------------------------
	log("Test - 'StringUtils::splitOnce'", __LINE__);
	string sodR, sofR;
	StringUtils::splitOnce("var=value=1", StringUtils::isEqual, sodR, sofR);
	ASSERT_EQUAL("var", sodR, "'splitOnce' KO");
	ASSERT_EQUAL("value=1", sofR, "'splitOnce' KO");

	StringUtils::splitOnce("var=", StringUtils::isEqual, sodR, sofR);
	ASSERT_EQUAL("var", sodR, "'splitOnce' KO");
	ASSERT_EQUAL("", sofR, "'splitOnce' KO");

	StringUtils::splitOnce("=value", StringUtils::isEqual, sodR, sofR);
	ASSERT_EQUAL("", sodR, "'splitOnce' KO");
	ASSERT_EQUAL("value", sofR, "'splitOnce' KO");

	StringUtils::splitOnce("xxx", StringUtils::isEqual, sodR, sofR);
	ASSERT_EQUAL("", sodR, "'splitOnce' KO");
	ASSERT_EQUAL("", sofR, "'splitOnce' KO");

	// -----------------------------------------------------
	log("Test - 'StringUtils::splitBySpaces'", __LINE__);
	vector<string> splitBySpaces = StringUtils::splitBySpaces(" Coucou   comment tu vas  ");
	ASSERT_EQUAL(4, splitBySpaces.size(), "'splitBySpaces' KO");
	ASSERT_EQUAL("Coucou", splitBySpaces[0], "'splitBySpaces' KO");
	ASSERT_EQUAL("comment", splitBySpaces[1], "'splitBySpaces' KO");
	ASSERT_EQUAL("tu", splitBySpaces[2], "'splitBySpaces' KO");
	ASSERT_EQUAL("vas", splitBySpaces[3], "'splitBySpaces' KO");

	// -----------------------------------------------------
	log("Test - 'StringUtils::splitByCarriageReturns'", __LINE__);
	vector<string> splitByCarriageReturns = StringUtils::splitByCarriageReturns("   Hello\n I am \n Speedy !");
	ASSERT_EQUAL(3, splitByCarriageReturns.size(), "'splitByCarriageReturns' KO");
	ASSERT_EQUAL("   Hello", splitByCarriageReturns[0], "'splitByCarriageReturns' KO");
	ASSERT_EQUAL(" I am ", splitByCarriageReturns[1], "'splitByCarriageReturns' KO");
	ASSERT_EQUAL(" Speedy !", splitByCarriageReturns[2], "'splitByCarriageReturns' KO");
}

void UtilsTest::containsTest() {
	log("Test - Template 'contains' s'apliquant aux vector", __LINE__);

	vector<string> list;
	list.push_back("aaa");
	list.push_back("bbb");
	list.push_back("1");
	list.push_back("2");
	list.push_back("3");
	list.push_back("coucou");

	ASSERT_TRUE(contains<string>(list, "aaa"), "'contains' KO");
	ASSERT_TRUE(contains<string>(list, "coucou"), "'contains' KO");
	ASSERT_TRUE(contains<string>(list, "3"), "'contains' KO");
	ASSERT_FALSE(contains<string>(list, "4"), "'contains' KO");
	ASSERT_FALSE(contains<string>(list, "aaaa"), "'contains' KO");
}
} // JktTest
