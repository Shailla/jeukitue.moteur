
mapDataTree = 0;

-- Position du sprite
posX = 0;
posY = 0;
posZ = 0;

-- Vitesse du sprite
vitX = 0;
vitY = 0;
vitZ = 0;

function onLoad()
	log("Version Lua : " .. _VERSION)
	
	mapDataTree = getDataTree();
	
	rootBrancheId = mapDataTree:createBranche({}, "root");
	mapBrancheId = mapDataTree:createBranche(rootBrancheId, "map");
	spriteBrancheId = mapDataTree:createBranche(mapBrancheId, "sprite1");

	vitX = mapDataTree:createValeur(spriteBrancheId, "float", "vitX");
	vitY = mapDataTree:createValeur(spriteBrancheId, "float", "vitY");
	vitZ = mapDataTree:createValeur(spriteBrancheId, "float", "vitZ");

	posX = mapDataTree:createValeur(spriteBrancheId, "float", "posX");
	posY = mapDataTree:createValeur(spriteBrancheId, "float", "posY");
	posZ = mapDataTree:createValeur(spriteBrancheId, "float", "posZ");	
	
	traceIds();
	
	vitX:setValue(0.02);
	vitY:setValue(0);
	vitZ:setValue(0);

	posX:setValue(0);
	posY:setValue(0.5);
	posZ:setValue(0.5);	
	
	subscribeEvents("refresh");
end

function traceIds()
	traceId("root", rootBrancheId);
	traceId("mapBrancheId", mapBrancheId);
	traceId("spriteBrancheId", spriteBrancheId);
	traceId("vitXValeurId", vitX:getValeurFullId());
	traceId("vitYValeurId", vitY:getValeurFullId());
	traceId("vitZValeurId", vitZ:getValeurFullId());
	traceId("posXValeurId", posX:getValeurFullId());
	traceId("posYValeurId", posY:getValeurFullId());
	traceId("posZValeurId", posZ:getValeurFullId());
end

function traceId(object, id)
	msg = object .. " : ";
	for i = 1, #id do
		if i > 1 then
			msg = msg .. ",";
		end
		msg = msg .. id[i];
	end
	log(msg);
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		if actionId == 1100 then
			-- Vitesse du sprite
			if posX:getValue() < -1 then
				if vitX:getValue() < 0 then
					vitX:setValue(0.02);
				end
			elseif posX:getValue() > 1 then
				if vitX:getValue() > 0 then
					vitX:setValue(-0.02);
				end
			end
			
			vitY:setValue(0);
			vitZ:setValue(0);
			
			-- Position du sprite
			posX:setValue(posX:getValue() + vitX:getValue());
			posY:setValue(0.5);
			posZ:setValue(0.5);
		end
	end
end
