/*
 * MapWebService.cpp
 *
 *  Created on: 25 févr. 2016
 *      Author: VGDJ7997
 */

#include <reseau/web/HttpServer.h>
#include <stdlib.h>

#include "reseau/web/json/JsonObject.h"
#include "service/MapService.h"
#include "service/dto/MapInformationDto.h"

#include "reseau/web/service/MapWebService.h"

namespace jkt {

MapWebService::MapWebService() {
}

MapWebService::~MapWebService() {
}

WebServiceResult MapWebService::execute(const std::string& method, const std::string& serviceEndpoint, const std::string& params) {
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

} /* namespace jkt */
