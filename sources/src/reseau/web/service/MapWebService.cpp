/*
 * MapWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>
#include <boost/lexical_cast.hpp>

#include "util/StringUtils.h"
#include "util/Trace.h"
#include "reseau/web/HttpServer.h"
#include "reseau/web/json/JsonBoolean.h"
#include "reseau/web/json/JsonObject.h"
#include "service/MapService.h"
#include "service/dto/MapInformationDto.h"
#include "main/Game.h"
#include "spatial/Map.h"
#include "spatial/geo/EntryPoint.h"
#include "spatial/light/Light.h"
#include "reseau/web/HttpException.h"

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

std::regex MapWebService::RG_GET_MAPS("^maps$");
std::regex MapWebService::RG_GET_MAP_GRAPHE("^map-graphe$");
std::regex MapWebService::RG_GET_ELEMENT_SERVICE("^element/(\\d+)$");
std::regex MapWebService::RG_UPDATE_ELEMENT_SERVICE("^element/(\\d+)$");

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

	return WebServiceResult(root, HttpServer::HTTP_RESPONSE_200);
}

void MapWebService::jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe) {
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
		obj.addBoolean(SELECTED, mObj->isSelected());

		obj.addList("elements");
	}

	for(auto& subMap : map->getSubMaps()) {
		JsonObject& obj = elements.addObject();
		jisonifyMapGraphe(subMap.second, obj);
	}
}

WebServiceResult MapWebService::getCurrentMapGraphe() {
	JsonObject root;
	JsonObject& mapElement = root.addObject("mapElement");

	CMap* map = Game.getMap();

	if(map) {		jisonifyMapGraphe(map, mapElement);;
	}

	return WebServiceResult(root, HttpServer::HTTP_RESPONSE_200);
}

WebServiceResult MapWebService::getElement(int elementId) {
	JsonObject root;
	JsonObject& mapElement = root.addObject("mapElement");

	CMap* map = Game.getMap();

	if(!map) {
		return jsonErrorResponse(HttpServer::HTTP_RESPONSE_404, "There is no active Map");
	}

	MapObject* object = map->findMapObject(elementId);

	if(!object) {
		return jsonErrorResponse(HttpServer::HTTP_RESPONSE_404, "Object not found in the active Map");
	}

	mapElement.addString(TYPE, ENTRYPOINT);
	mapElement.addNumber(ID, object->getId());
	mapElement.addString(NAME, object->getName());
	mapElement.addBoolean(SELECTED, object->isSelected());

	return WebServiceResult(root, HttpServer::HTTP_RESPONSE_200);
}

WebServiceResult MapWebService::updateElement(HttpRequest& request, int elementId) {
	CMap* map = Game.getMap();

	if(!map) {
		return jsonErrorResponse(HttpServer::HTTP_RESPONSE_404, "There is no active Map");
	}

	MapObject* object = map->findMapObject(elementId);

	if(!object) {
		return jsonErrorResponse(HttpServer::HTTP_RESPONSE_404, "Object not found in the active Map");
	}

	JsonObject* newValues = request.getBodyJson();

	if(newValues) {
		for(JsonPair* pair : newValues->getPairs()) {
			const string& name = pair->getName();

			if(name == "selected") {
				const JsonValue* val = pair->getValue();

				if(!val) {
					return jsonErrorResponse(HttpServer::HTTP_RESPONSE_400, "Bad request format, 'selected' element has no value");
				}

				const JsonBoolean* value = val->isJsonBoolean();

				if(!value) {
					return jsonErrorResponse(HttpServer::HTTP_RESPONSE_400, "Bad request format, 'selected' element value should be boolean ('true' or 'false')");
				}

				object->select(value->getValue());
			}
		}
	}

	JsonObject root;
	return WebServiceResult(root, HttpServer::HTTP_RESPONSE_200);
}

WebServiceResult MapWebService::execute(HttpRequest& request, const string& baseEndpoint, const string& serviceEndpoint) {
	smatch match;

	if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_match(serviceEndpoint, RG_GET_MAPS)) {
		return getMapList();
	}
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_match(serviceEndpoint, RG_GET_MAP_GRAPHE)) {
		return getCurrentMapGraphe();
	}
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_search(serviceEndpoint, match, RG_GET_ELEMENT_SERVICE)) {
		int elementId = boost::lexical_cast<int>(match[1]);
		LOGINFO(("GET Element id='%d'", elementId));

		return getElement(elementId);
	}
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_PUT && regex_search(serviceEndpoint, match, RG_UPDATE_ELEMENT_SERVICE)) {
		int elementId = boost::lexical_cast<int>(match[1]);
		LOGINFO(("UPDATE Element id='%d'", elementId));

		return updateElement(request, elementId);
	}

	// Service not found
	throw HttpException(HttpException::SERVICE_NOT_EXISTS, "No map service found for the endpoint");
}

} /* namespace jkt */
