/*
 * MapWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <stdlib.h>
#include <boost/lexical_cast.hpp>

#include "SDL.h"
#include "SDL_net.h"

#include "util/FileUtils.h"
#include "util/StringUtils.h"
#include "util/Trace.h"
#include "main/Fabrique.h"

#include "main/Game.h"
#include "spatial/Map.h"
#include "spatial/geo/EntryPoint.h"
#include "spatial/light/Light.h"

#include "reseau/web/HttpVocabulary.h"
#include "reseau/web/HttpServer.h"
#include "reseau/web/json/JsonObject.h"
#include "reseau/web/json/JsonBoolean.h"
#include "reseau/web/json/JsonString.h"
#include "reseau/web/json/JsonNumber.h"
#include "service/MapService.h"
#include "service/dto/MapInformationDto.h"
#include "menu/ConsoleView.h"

#include "reseau/web/HttpException.h"

#include "reseau/web/service/map/GetMapElementWS.h"

using namespace std;

extern CGame Game;

namespace jkt {

const string GetMapElementWS::ID = 					"id";					// Identifiant unique d'un Map element
const string GetMapElementWS::NAME = 				"name";					// Nom d'un Map element
const string GetMapElementWS::TYPE = 				"type";					// Type d'un Map element
const string GetMapElementWS::MAPS = 				"maps";
const string GetMapElementWS::ELEMENTS = 			"elements";
const string GetMapElementWS::MAPELEMENT = 			"mapElement";
const string GetMapElementWS::MAPELEMENTS = 		"mapElements";

const string GetMapElementWS::CARACS =				"caracs";
const string GetMapElementWS::CARAC_UPDATABLE = 	"caracUpdatable";
const string GetMapElementWS::CARAC_VALUE = 		"caracValue";

const string GetMapElementWS::CARAC_TYPE = 			"caracType";
const string GetMapElementWS::CARAC_TYPE_BOOL = 	"bool";
const string GetMapElementWS::CARAC_TYPE_LONG = 	"long";
const string GetMapElementWS::CARAC_TYPE_STRING = 	"string";

const string GetMapElementWS::CARAC_GROUP = 		"caracGroup";
const string GetMapElementWS::CARAC_GROUP_MAIN = 	"main";

const GetMapElementWS::Caracteristic GetMapElementWS::HIGHLIGHTED("highlighted", GetMapElementWS::CARAC_TYPE_BOOL, "general", true);
const GetMapElementWS::Caracteristic GetMapElementWS::HIDDEN("hidden", GetMapElementWS::CARAC_TYPE_BOOL, "general", true);

std::regex GetMapElementWS::RG_MAPS_SERVICE("^maps$");
std::regex GetMapElementWS::RG_MAP_SERVICE("^map$");
std::regex GetMapElementWS::RG_MAP_ELEMENTS_SERVICE("^map/elements$");
std::regex GetMapElementWS::RG_MAP_ELEMENT_SERVICE("^map/element/(\\d+)$");

GetMapElementWS::Caracteristic::Caracteristic(const string& name, const string& type, const string& group,
		bool updatable) :_name(name), _type(type), _group(group), _updatable(updatable) {
}

GetMapElementWS::GetMapElementWS() {
}

GetMapElementWS::~GetMapElementWS() {
}

WebServiceResult GetMapElementWS::getMapList() {
	JsonObject root;
	JsonList& maps = root.addList(MAPS);

	vector<MapInformationDto> dtos;
	MapService::loadMapDirectoryContent(dtos);

	for(MapInformationDto dto : dtos) {
		JsonObject& jsonMap = maps.addObject();
		jsonMap.addString(NAME, dto.getMapFileMinimalName());
	}

	return WebServiceResult(root, HttpVocabulary::HTTP_RESPONSE_200);
}

void GetMapElementWS::jisonifyMapGraphe(CMap* map, JsonObject& mapGraphe) {
	mapGraphe.addString(TYPE, map->getType());
	mapGraphe.addNumber(ID, map->getId());
	mapGraphe.addString(NAME, map->getName());

	JsonObject& caracsGraphe = mapGraphe.addObject(CARACS);
	addCaracBoolean(caracsGraphe, HIGHLIGHTED, false);
	addCaracBoolean(caracsGraphe, HIDDEN, false);

	JsonList& elements = mapGraphe.addList(ELEMENTS);

	for(MapObject* mObj : map->getMapObjects()) {
		JsonObject& obj = elements.addObject();
		obj.addString(TYPE, mObj->getType());
		obj.addNumber(ID, mObj->getId());
		obj.addString(NAME, mObj->getName());

		JsonObject& caracs = obj.addObject(CARACS);
		addCaracBoolean(caracs, HIGHLIGHTED, mObj->isHighlighted());
		addCaracBoolean(caracs, HIDDEN, mObj->isHidden());

		obj.addList(ELEMENTS);
	}

	for(auto& subMap : map->getSubMaps()) {
		JsonObject& obj = elements.addObject();
		jisonifyMapGraphe(subMap.second, obj);
	}
}

WebServiceResult GetMapElementWS::getCurrentMapGraphe() {
	JsonObject root;
	JsonObject& mapElement = root.addObject(MAPELEMENT);

	CMap* map = Game.getMap();

	if(map) {		jisonifyMapGraphe(map, mapElement);;
	}

	return WebServiceResult(root, HttpVocabulary::HTTP_RESPONSE_200);
}

WebServiceResult GetMapElementWS::saveCurrentMap() {
	JsonObject root;

	CMap* map = Game.getMap();

	if(map) {
		string saveMapFilename = "map-" + FileUtils::horodatage();
		map->Save(saveMapFilename);

		ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));
		console->println(ConsoleView::COT_INFO, string("Map enregistr\u00e9e : ") + saveMapFilename);

		return WebServiceResult(root, HttpVocabulary::HTTP_RESPONSE_201);
	}
	else {
		return WebServiceResult(root, HttpVocabulary::HTTP_RESPONSE_204);
	}


}

void GetMapElementWS::getElement(int elementId, JsonObject& mapElement, vector<string>& errors) {
	CMap* map = Game.getMap();

	if(!map) {
		errors.push_back("There is no active Map");
		return;
	}

	MapObject* object = map->findMapObject(elementId);

	if(!object) {
		errors.push_back("Object not found in the active Map");
		return;
	}


	/* ***************************************************** */
	/* Common parameters                                     */
	/* ***************************************************** */

	mapElement.addNumber(ID, object->getId());
	mapElement.addString(TYPE, object->getType());
	mapElement.addString(NAME, object->getName());

	JsonObject& caracs = mapElement.addObject(CARACS);

	addCaracBoolean(caracs, HIGHLIGHTED, object->isHighlighted());
	addCaracBoolean(caracs, HIDDEN, object->isHidden());

	// Exemples
	addCaracBoolean(caracs, "booleanUpdatable", "Updatables", true, true);
	addCaracBoolean(caracs, "booleanNotUpdatable", "Not updatables", false, true);

	addCaracLong(caracs, "numberUpdatable", "Updatables", true, 56);
	addCaracLong(caracs, "numberNotUpdatable", "Not updatables", false, 56);

	addCaracString(caracs, "stringUpdatable", "Updatables", true, "coucou");
	addCaracString(caracs, "stringNotUpdatable", "Not updatables", false, "coucou");
}

JsonObject& GetMapElementWS::addCaracString(JsonObject& jsonCharistics, const Caracteristic& carac, const bool value) {
	return addCaracBoolean(jsonCharistics, carac._name, carac._group, carac._updatable, value);
}

JsonObject& GetMapElementWS::addCaracLong(JsonObject& jsonCharistics, const Caracteristic& carac, const bool value) {
	return addCaracBoolean(jsonCharistics, carac._name, carac._group, carac._updatable, value);
}

JsonObject& GetMapElementWS::addCaracBoolean(JsonObject& jsonCharistics, const Caracteristic& carac, const bool value) {
	return addCaracBoolean(jsonCharistics, carac._name, carac._group, carac._updatable, value);
}

JsonObject& GetMapElementWS::addCaracString(JsonObject& jsonCharistics, const string& name, const string& group, bool updatable, const string& value) {
	JsonObject& charistic = jsonCharistics.addObject(name);
	charistic.addString(CARAC_TYPE, CARAC_TYPE_STRING);
	charistic.addString(CARAC_GROUP, group);
	charistic.addBoolean(CARAC_UPDATABLE, updatable);
	charistic.addString(CARAC_VALUE, value);

	return charistic;
}

JsonObject& GetMapElementWS::addCaracLong(JsonObject& jsonCharistics, const string& name, const string& group, bool updatable, long value) {
	JsonObject& charistic = jsonCharistics.addObject(name);
	charistic.addString(CARAC_TYPE, CARAC_TYPE_LONG);
	charistic.addString(CARAC_GROUP, group);
	charistic.addBoolean(CARAC_UPDATABLE, updatable);
	charistic.addNumber(CARAC_VALUE, value);

	return charistic;
}

JsonObject& GetMapElementWS::addCaracBoolean(JsonObject& jsonCharistics, const string& name, const string& group, bool updatable, const bool value) {
	JsonObject& charistic = jsonCharistics.addObject(name);
	charistic.addString(CARAC_TYPE, CARAC_TYPE_BOOL);
	charistic.addString(CARAC_GROUP, group);
	charistic.addBoolean(CARAC_UPDATABLE, updatable);
	charistic.addBoolean(CARAC_VALUE, value);

	return charistic;
}

WebServiceResult GetMapElementWS::updateElement(HttpRequest& request, int elementId) {
	CMap* map = Game.getMap();

	// Is there an active Map
	if(!map) {
		return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_404, "There is no active Map");
	}

	// Do the object exist
	MapObject* object = map->findMapObject(elementId);

	if(!object) {
		return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_404, "Object not found in the active Map");
	}

	JsonObject* newValues = request.getBodyJson();

	if(newValues) {
		JsonObject* mapElement = newValues->getObject(MAPELEMENT, false);

		if(mapElement) {
			// Update the object
			try {
				updateElement(mapElement, object);
			}
			catch(BadFormatJsonException& exception) {
				return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_400, exception.getMessage());
			}
		}
		else {
			return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_400, "Bad request format, root object 'mapElement' element does not exists");
		}
	}
	else {
		JsonObject root;
		return WebServiceResult(HttpVocabulary::HTTP_RESPONSE_200);
//		return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_400, "Bad request format, no Json content");
	}

	vector<string> errors;
	JsonObject root;
	JsonObject& mapElement = root.addObject(MAPELEMENT);
	getElement(elementId, mapElement, errors);
	return WebServiceResult(root, HttpVocabulary::HTTP_RESPONSE_200);
}

bool GetMapElementWS::updateCaracBoolean(JsonObject& jsonCharistics, const Caracteristic& carac, bool& newValue) {
	bool updated = false;

	if(carac._updatable) {
		JsonObject* caracObject = jsonCharistics.getObject(carac._name, false);

		if(caracObject) {
			const JsonBoolean* value = caracObject->getBoolean(CARAC_VALUE, true);
			newValue = value->getValue();
			updated = true;
		}
	}

	return updated;
}

bool GetMapElementWS::updateCaracLong(JsonObject& jsonCharistics, const Caracteristic& carac, long& newValue) {
	bool updated = false;

	if(carac._updatable) {
		JsonObject* caracObject = jsonCharistics.getObject(carac._name, false);

		if(caracObject) {
			const JsonNumber* value = caracObject->getNumber(CARAC_VALUE, true);
			newValue = value->getValue();
			updated = true;
		}
	}

	return updated;
}

bool GetMapElementWS::updateCaracString(JsonObject& jsonCharistics, const Caracteristic& carac, string& newValue) {
	bool updated = false;

	if(carac._updatable) {
		JsonObject* caracObject = jsonCharistics.getObject(carac._name, false);

		if(caracObject) {
			const JsonString* value = caracObject->getString(CARAC_VALUE, true);
			newValue = value->getValue();
			updated = true;
		}
	}

	return updated;
}

void GetMapElementWS::updateElement(JsonObject* jsonObject, MapObject* object) throw(BadFormatJsonException) {

	// Update Name
	const JsonString* name = jsonObject->getString(NAME, false);

	if(name) {
		object->setName(name->getValue());
	}


	/* ********************************************* */
	/* Characteristics update                        */
	/* ********************************************* */

	JsonObject& caracs = *jsonObject->getObject(CARACS, true);
	bool newValue;

	// HIGHLIGHTED characteristic
	if(updateCaracBoolean(caracs, HIGHLIGHTED, newValue)) {
		object->highlight(newValue);
	}

	// HIDDEN characteristic
	if(updateCaracBoolean(caracs, HIDDEN, newValue)) {
		object->hide(newValue);
	}
}

WebServiceResult GetMapElementWS::updateElements(HttpRequest& request) {
	vector<string> errors;

	CMap* map = Game.getMap();

	if(!map) {
		return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_404, "There is no active Map");
	}

	JsonObject* body = request.getBodyJson();

	if(!body) {
		return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_400, "Bad http request, there is no body");
	}

	JsonValue* mapElements = body->getValue(MAPELEMENTS, false);

	if(!mapElements) {
		return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_400, "Bad request format, root 'mapElements' element does not exist");
	}

	JsonList* mapElementList = mapElements->isJsonList();

	if(!mapElementList) {
		return jsonErrorResponse(HttpVocabulary::HTTP_RESPONSE_400, "Bad request format, root 'mapElements' must be a list");
	}

	const vector<JsonValue*>& list = mapElementList->getValue();

	JsonObject rootResponse;
	JsonList& mapElementsResponse = rootResponse.addList(MAPELEMENTS);

	for(JsonValue* mapElementList : list) {
		JsonObject* mapElement = mapElementList->isJsonObject();

		if(!mapElement) {
			errors.push_back("Bad request format, mapElements must be Json objects");
			continue;
		}

		JsonValue* val = mapElement->getValue(ID, false);

		if(!val) {
			errors.push_back("'id' must exist in each mapElement");
			continue;
		}

		JsonNumber* elementIdVl = val->isJsonNumber();

		if(!elementIdVl) {
			errors.push_back("Bad request format, 'id' must be a number");
			continue;
		}

		int elementId = elementIdVl->getValue();

		MapObject* object = map->findMapObject(elementId);

		if(!object) {
			stringstream msg;
			msg << "Object whith id" << elementId << "not found in the active Map";
			errors.push_back(msg.str());
			continue;
		}

		try {
			updateElement(mapElement->isJsonObject(), object);
		}
		catch(BadFormatJsonException& exception) {
			errors.push_back(exception.getMessage());
		}

		JsonObject& mapElementResponse = mapElementsResponse.addObject();
		getElement(elementId, mapElementResponse, errors);
	}

	if(errors.size() == 0) {
		return WebServiceResult(rootResponse, HttpVocabulary::HTTP_RESPONSE_200);
	}
	else {
		return WebServiceResult(rootResponse, errors, HttpVocabulary::HTTP_RESPONSE_400);
	}
}

WebServiceResult GetMapElementWS::execute(HttpRequest& request, const string& baseEndpoint, const string& serviceEndpoint) throw(HttpException) {
	smatch match;

	// Get Map list
	if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_match(serviceEndpoint, RG_MAPS_SERVICE)) {
		return getMapList();
	}
	// Get current Map
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_match(serviceEndpoint, RG_MAP_SERVICE)) {
		return getCurrentMapGraphe();
	}
	// Save current Map
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_POST && regex_match(serviceEndpoint, RG_MAP_SERVICE)) {
		return saveCurrentMap();
	}
	// Update current Map elements
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_PUT && regex_search(serviceEndpoint, match, RG_MAP_ELEMENTS_SERVICE)) {
		return updateElements(request);
	}
	// Get one Map element
	else if(request.getMethod() == HttpServer::HTTP_METHODS::HTTP_GET && regex_search(serviceEndpoint, match, RG_MAP_ELEMENT_SERVICE)) {
		int elementId = boost::lexical_cast<int>(match[1]);
		LOGINFO(("GET Element id='%d'", elementId));

		vector<string> errors;
		JsonObject root;
		JsonObject& mapElement = root.addObject(MAPELEMENT);
		getElement(elementId, mapElement, errors);

		if(errors.size() == 0) {
			return WebServiceResult(root, HttpVocabulary::HTTP_RESPONSE_200);
		}
		else {
			return WebServiceResult(root, errors, HttpVocabulary::HTTP_RESPONSE_400);
		}
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
