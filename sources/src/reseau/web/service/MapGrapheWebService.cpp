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
#include "spatial/light/Light.h"


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

void MapGrapheWebService::jisonifyMap(CMap* map, JsonObject& mapGraphe) {
	mapGraphe.addString(TYPE, MAP);
	mapGraphe.addNumber(ID, map->getId());
	mapGraphe.addString(NAME, map->getName());

	JsonList& elements = mapGraphe.addList("elements");

	for(EntryPoint* ePt : map->getEntryPointsList()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, ENTRYPOINT);
		obj.addNumber(ID, ePt->getId());
		obj.addString(NAME, ePt->getName());

		obj.addList("elements");
		obj.addString("expanded", "false");
		obj.addString("checked", "false");
	}

	for(CLight* light : map->getLights()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, LIGHT);
		obj.addNumber(ID, light->getId());
		obj.addString(NAME, "lumiere");

		obj.addList("elements");
		obj.addString("expanded", "false");
		obj.addString("checked", "false");
	}

	for(MapObject* mObj : map->getMapObjects()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, GEO);
		obj.addNumber(ID, mObj->getId());
		obj.addString(NAME, mObj->getName());

		obj.addList("elements");
		obj.addString("expanded", "false");
		obj.addString("checked", "false");
	}

	for(auto& subMap : map->getSubMaps()) {
		JsonObject& obj = elements.addObject();
		jisonifyMap(subMap.second, obj);
	}

	mapGraphe.addString("expanded", "false");
	mapGraphe.addString("checked", "false");
}

WebServiceResult MapGrapheWebService::execute(const std::string& endpoint, const std::string& method) {
	JsonObject root;
	JsonObject& mapElement = root.addObject("mapElement");

	CMap* map = Game.getMap();

	if(map) {
		jisonifyMap(map, mapElement);
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
