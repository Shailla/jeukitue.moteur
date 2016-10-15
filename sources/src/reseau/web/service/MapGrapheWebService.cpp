/*
 * MapGrapheWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>

#include "reseau/web/json/JsonObject.h"
#include "reseau/web/HtmlServer.h"
#include "main/Game.h"
#include "spatial/Map.h"
#include "spatial/geo/EntryPoint.h"

#include "reseau/web/service/MapGrapheWebService.h"

extern CGame Game;

namespace jkt {

string MapGrapheWebService::ID = "id";
string MapGrapheWebService::NAME = "name";
string MapGrapheWebService::TYPE = "type";

string MapGrapheWebService::MAP = "map";
string MapGrapheWebService::GEO = "geo";
string MapGrapheWebService::LIGHT = "light";
string MapGrapheWebService::ENTRYPOINT = "entryPoint";

MapGrapheWebService::MapGrapheWebService() {
}

MapGrapheWebService::~MapGrapheWebService() {
}

WebServiceResult MapGrapheWebService::execute(const std::string& endpoint, const std::string& method) {
	JsonObject root;
	JsonList& mapGraphe = root.addList("map-graphe");

	CMap* map = Game.getMap();

	if(map) {
		for(CLight* light : map->getLights()) {
			JsonObject& obj = mapGraphe.addObject();
			obj.addString(TYPE, LIGHT);
			obj.addString(ID, "TODO");
			obj.addString(NAME, "lumiere");
		}

		for(MapObject* mObj : map->getMapObjects()) {
			JsonObject& obj = mapGraphe.addObject();
			obj.addString(TYPE, GEO);
			obj.addString(ID, "TODO");
			obj.addString(NAME, mObj->getName());
		}

		for(EntryPoint* ePt : map->getEntryPointsList()) {
			JsonObject& obj = mapGraphe.addObject();
			obj.addString(TYPE, ENTRYPOINT);
			obj.addString(ID, "TODO");
			obj.addString(NAME, ePt->getName());
		}
	}

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
