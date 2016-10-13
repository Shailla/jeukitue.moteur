/*
 * MapGrapheWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>

#include "reseau/web/json/JsonObject.h"
#include "reseau/web/HtmlServer.h"

#include "reseau/web/service/MapGrapheWebService.h"

namespace jkt {

MapGrapheWebService::MapGrapheWebService() {
}

MapGrapheWebService::~MapGrapheWebService() {
}

WebServiceResult MapGrapheWebService::execute(const std::string& endpoint, const std::string& method) {
	JsonObject root;
	JsonList& mapGraphe = root.addList("map-graphe");

	JsonObject& jsonGeo1 = mapGraphe.addObject();
	jsonGeo1.addString("type", "geo");
	jsonGeo1.addString("name", "geo1");

	JsonObject& jsonGeo2 = mapGraphe.addObject();
	jsonGeo2.addString("type", "geo");
	jsonGeo2.addString("name", "geo2");

	JsonObject& jsonGeo3 = mapGraphe.addObject();
	jsonGeo3.addString("type", "geo");
	jsonGeo3.addString("name", "geo3");

	JsonObject& jsonLight1 = mapGraphe.addObject();
	jsonLight1.addString("type", "light");
	jsonLight1.addString("name", "light1");

	JsonObject& jsonLight2 = mapGraphe.addObject();
	jsonLight2.addString("type", "light");
	jsonLight2.addString("name", "light3");

	JsonObject& jsonEntryPoint1 = mapGraphe.addObject();
	jsonEntryPoint1.addString("type", "entryPoint");
	jsonEntryPoint1.addString("name", "entryPoint1");

	JsonObject& jsonEntryPoint2 = mapGraphe.addObject();
	jsonEntryPoint2.addString("type", "entryPoint");
	jsonEntryPoint2.addString("name", "entryPoint2");

	string json = root.toString();

	WebServiceResult result;
	result._contentSize = json.size();
	result._content = malloc(json.size());
	json.copy((char*)result._content, json.size());
	result._contentType = HtmlServer::HTTP_CONTENT_TYPE_JSON;
	result._status = HtmlServer::HTTP_RESPONSE_200;

	return result;
}

} /* namespace jkt */
