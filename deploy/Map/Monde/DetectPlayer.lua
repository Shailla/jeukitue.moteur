
function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	detector = Map:createPlayerZoneDetector("idCoucou", 2.0, 3.0, 0.0, 1.0, 0.0, 1.0);
	detector:setVisibility(true);
end

function eventManager(event)
	local type = event:getType();
		
	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		if actionId == 1100 then
			for index = 1, nbrSprites do
				updateVitPos(index);
			end
		end
	end
end
