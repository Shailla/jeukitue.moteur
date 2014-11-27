
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
	
	--mapDataTree = getMapDataTree();
	
	--posX = mapDataTree:allocFloat();
	--posY = mapDataTree:allocFloat();
	--posZ = mapDataTree:allocFloat();
	
	--vitX = mapDataTree:allocFloat();
	--vitY = mapDataTree:allocFloat();
	--vitZ = mapDataTree:allocFloat();
	
	subscribeEvents("refresh");
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		if actionId == 1100 then
			log("Refresh");
		end
	end
end
