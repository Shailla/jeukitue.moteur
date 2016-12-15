/*
 * MapWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>

#include "reseau/web/HttpServer.h"
#include "reseau/web/json/JsonObject.h"
#include "service/MapService.h"
#include "service/dto/MapInformationDto.h"
#include "main/Game.h"
#include "spatial/Map.h"
#include "spatial/geo/EntryPoint.h"
#include "spatial/light/Light.h"

#include "reseau/web/service/MapWebService.h"

extern CGame Game;

namespace jkt {

string MapWebService::ID = "id";
string MapWebService::NAME = "name";
string MapWebService::TYPE = "type";
string MapWebService::SELECTED = "selected";

string MapWebService::MAP = "map";
string MapWebService::GEO = "geo";
string MapWebService::LIGHT = "light";
string MapWebService::ENTRYPOINT = "entryPoint";

MapWebService::MapWebService() {
}

MapWebService::~MapWebService() {
}

WebServiceResult MapWebService::getMapList() {
	JsonObject root;
	JsonList& maps = root.addList("maps");

	vector<MapInformationDto> dtos;
	MapService::loadMapDirectoryContent(dtos);

	for(MapInformationDto dto : dtos) {
		JsonObject& jsonMap = maps.addObject();
		jsonMap.addString("name", dto.getMapFileMinimalName());
	}

	string json = root.toString();

	WebServiceResult result;
	result._contentSize = json.size();
	result._content = malloc(json.size());
	json.copy((char*)result._content, json.size());
	result._contentType = HttpServer::HTTP_CONTENT_TYPE_JSON;
	result._status = HttpServer::HTTP_RESPONSE_200;

	return result;
}

void MapWebService::jisonifyMap(CMap* map, JsonObject& mapGraphe) {
	mapGraphe.addString(TYPE, MAP);
	mapGraphe.addNumber(ID, map->getId());
	mapGraphe.addString(NAME, map->getName());
	mapGraphe.addString(SELECTED, "false");			// TODO permettre de sélectionner une sous-Map la Map

	JsonList& elements = mapGraphe.addList("elements");

	for(EntryPoint* ePt : map->getEntryPointsList()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, ENTRYPOINT);
		obj.addNumber(ID, ePt->getId());
		obj.addString(NAME, ePt->getName());
		obj.addString(SELECTED, "false");		// TODO permettre de sélectionner un entry point dans la Map

		obj.addList("elements");
	}

	for(CLight* light : map->getLights()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, LIGHT);
		obj.addNumber(ID, light->getId());
		obj.addString(NAME, "lumiere");
		obj.addString(SELECTED, "false");		// TODO permettre de sélectionner une lumière dans la Map

		obj.addList("elements");
	}

	for(MapObject* mObj : map->getMapObjects()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, GEO);
		obj.addNumber(ID, mObj->getId());
		obj.addString(NAME, mObj->getName());
		obj.addString(SELECTED, mObj->isSelected()?"true":"false");

		obj.addList("elements");
	}

	for(auto& subMap : map->getSubMaps()) {
		JsonObject& obj = elements.addObject();
		jisonifyMap(subMap.second, obj);
	}
}

WebServiceResult MapWebService::getCurrentMap() {
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
	result._contentType = HttpServer::HTTP_CONTENT_TYPE_JSON;
	result._status = HttpServer::HTTP_RESPONSE_200;

	return result;
}


WebServiceResult MapWebService::execute(HttpServer::HTTP_METHODS method, const string& fullEndpoint, const string& baseEndpoint, const string& serviceEndpoint, const std::string& params) {
	if(method == HttpServer::HTTP_METHODS::HTTP_GET && serviceEndpoint == "maps") {
		return getMapList();
	}
	else if(method == HttpServer::HTTP_METHODS::HTTP_GET && serviceEndpoint == "map") {
		return getCurrentMap();
	}
	else {
		// TODO return unexisting requested service
	}
}

} /* namespace jkt */
