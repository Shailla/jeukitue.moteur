/*
 * MapWebService.cpp
 *
 *  Created on: 25 f�vr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>
#include <boost/lexical_cast.hpp>

#include "util/FileUtils.h"
#include "util/StringUtils.h"
#include "util/Trace.h"
#include "main/Fabrique.h"

#include "main/Game.h"
#include "spatial/Map.h"
#include "spatial/geo/EntryPoint.h"
#include "spatial/light/Light.h"

#include "reseau/web/HttpServer.h"
#include "reseau/web/json/JsonObject.h"
#include "reseau/web/json/JsonBoolean.h"
#include "reseau/web/json/JsonString.h"
#include "service/MapService.h"
#include "service/dto/MapInformationDto.h"
#include "menu/ConsoleView.h"

#include "reseau/web/HttpException.h"

#include "reseau/web/service/map/GetMapElementWS.h"

extern CGame Game;

namespace jkt {

string GetMapElementWS::ID = "id";
string GetMapElementWS::NAME = "name";
string GetMapElementWS::TYPE = "type";
string GetMapElementWS::HIGHLIGHTED = "highlighted";
string GetMapElementWS::HIDDEN = "hidden";

std::regex GetMapElementWS::RG_MAPS_SERVICE("^maps$");
std::regex GetMapElementWS::RG_MAP_SERVICE("^map$");
std::regex GetMapElementWS::RG_MAP_ELEMENT_SERVICE("^map/element/(\\d+)$");

GetMapElementWS::GetMapElementWS() {
}

GetMapElementWS::~GetMapElementWS() {
}

WebServiceResult GetMapElementWS::getMapList() {
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

void GetMapElementWS::jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe) {
	mapGraphe.addString(TYPE, map->getType());
	mapGraphe.addNumber(ID, map->getId());
	mapGraphe.addString(NAME, map->getName());
	mapGraphe.addString(HIGHLIGHTED, "false");			// TODO permettre de s�lectionner une sous-Map la Map
	mapGraphe.addString(HIDDEN, "false");			// TODO permettre de s�lectionner une sous-Map la Map

	JsonList& elements = mapGraphe.addList("elements");

	for(EntryPoint* ePt : map->getEntryPointsList()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, ePt->getType());
		obj.addNumber(ID, ePt->getId());
		obj.addString(NAME, ePt->getName());
		obj.addString(HIGHLIGHTED, "false");		// TODO permettre de mettre en surbrillance
		obj.addString(HIDDEN, "false");				// TODO permettre de masquer

		obj.addList("elements");
	}

	for(CLight* light : map->getLights()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, light->getType());
		obj.addNumber(ID, light->getId());
		obj.addString(NAME, "lumiere");
		obj.addString(HIGHLIGHTED, "false");		// TODO permettre de mettre en surbrillance
		obj.addString(HIDDEN, "false");				// TODO permettre de masquer

		obj.addList("elements");
	}

	for(MapObject* mObj : map->getMapObjects()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, mObj->getType());
		obj.addNumber(ID, mObj->getId());
		obj.addString(NAME, mObj->getName());
		obj.addBoolean(HIGHLIGHTED, mObj->isHighlighted());
		obj.addBoolean(HIDDEN, mObj->isHidden());

		obj.addList("elements");
	}

	for(auto& subMap : map->getSubMaps()) {
		JsonObject& obj = elements.addObject();
		jisonifyMapGraphe(subMap.second, obj);
	}
}

WebServiceResult GetMapElementWS::getCurrentMap() {
	JsonObject root;
	JsonObject& mapElement = root.addObject("mapElement");

	CMap* map = Game.getMap();

	if(map) {		jisonifyMapGraphe(map, mapElement);;
	}

	return WebServiceResult(root, HttpServer::HTTP_RESPONSE_200);
}

WebServiceResult GetMapElementWS::saveCurrentMap() {
	JsonObject root;

	CMap* map = Game.getMap();

	if(map) {
		string saveMapFilename = "map-" + FileUtils::horodatage();
		map->Save(saveMapFilename);

		ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));
		console->println(ConsoleView::COT_INFO, string("Map enregistr\u00e9e : ") + saveMapFilename);

		return WebServiceResult(root, HttpServer::HTTP_RESPONSE_201);
	}
	else {
		return WebServiceResult(root, HttpServer::HTTP_RESPONSE_204);
	}


}

WebServiceResult GetMapElementWS::getElement(int elementId) {
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


	/* ***************************************************** */
	/* Common parameters                                     */
	/* ***************************************************** */

	mapElement.addNumber(ID, object->getId());
	mapElement.addString(NAME, object->getName());
	mapElement.addBoolean(HIGHLIGHTED, object->isHighlighted());
	mapElement.addBoolean(HIDDEN, object->isHidden());
	mapElement.addString(TYPE, object->getType());

	return WebServiceResult(root, HttpServer::HTTP_RESPONSE_200);
}

WebServiceResult GetMapElementWS::updateElement(HttpRequest& request, int elementId) {
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
		JsonValue* mapElement = newValues->getValue("mapElement");

		if(mapElement && mapElement->isJsonObject()) {
			JsonObject* jsonObject = (JsonObject*)mapElement;

			// Name
			JsonValue* val;

			if((val = jsonObject->getValue(NAME))) {
				const JsonString* value = val->isJsonString();

				if(!value) {
					return jsonErrorResponse(HttpServer::HTTP_RESPONSE_400, "Bad request format, 'name' element value must be a string");
				}

				object->setName(value->getValue());
			}

			// Highlighted state
			if((val = jsonObject->getValue(HIGHLIGHTED))) {
				const JsonBoolean* value = val->isJsonBoolean();

				if(!value) {
					return jsonErrorResponse(HttpServer::HTTP_RESPONSE_400, "Bad request format, 'highlighted' element value must be boolean ('true' or 'false')");
				}

				object->highlight(value->getValue());
			}

			// Hidden state
			if((val = jsonObject->getValue(HIDDEN))) {
				const JsonBoolean* value = val->isJsonBoolean();

				if(!value) {
					return jsonErrorResponse(HttpServer::HTTP_RESPONSE_400, "Bad request format, 'hidden' element value must be boolean ('true' or 'false')");
				}

				object->hide(value->getValue());
			}
		}
	}
	else {
		return jsonErrorResponse(HttpServer::HTTP_RESPONSE_400, "Bad request format, root 'mapElement' element does not exists");
	}

	return getElement(elementId);
}

WebServiceResult GetMapElementWS::execute(HttpRequest& request, const string& baseEndpoint, const string& serviceEndpoint) throw(HttpException) {
	smatch match;

	// Get Map list
	if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_match(serviceEndpoint, RG_MAPS_SERVICE)) {
		return getMapList();
	}
	// Get current Map
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_match(serviceEndpoint, RG_MAP_SERVICE)) {
		return getCurrentMap();
	}
	// Save current Map
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_POST && regex_match(serviceEndpoint, RG_MAP_SERVICE)) {
		return saveCurrentMap();
	}
	// Get one Map element
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_search(serviceEndpoint, match, RG_MAP_ELEMENT_SERVICE)) {
		int elementId = boost::lexical_cast<int>(match[1]);
		LOGINFO(("GET Element id='%d'", elementId));

		return getElement(elementId);
	}
	// Update one Map element
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_PUT && regex_search(serviceEndpoint, match, RG_MAP_ELEMENT_SERVICE)) {
		int elementId = boost::lexical_cast<int>(match[1]);
		LOGINFO(("UPDATE Element id='%d'", elementId));

		return updateElement(request, elementId);
	}

	// Service not found
	throw HttpException(HttpException::SERVICE_NOT_EXISTS, "No map service found for the endpoint");
}

} /* namespace jkt */
