
mapDataTree = 0
nbrSprites = 100
vitesses = {}
positions = {}
colorsR = {}
colorsG = {}
colorsB = {}

function onLoad()
	log("Version Lua A : " .. _VERSION)
	
	if (isModeServer() or isModeLocal()) then
		log("Jeu en mode serveur ou local => initialisation des sprites");
		
		mapDataTree = getDataTree();

		dirigeablesBrId = mapDataTree:getBranche( {"map", "sprites", "dirigeables"} );
		traceId("ID DIRIGEABLE", dirigeablesBrId);
		
		for index = 1, nbrSprites do
			createSprite(dirigeablesBrId, index);
		end
			
		subscribeEvents("refresh");
	else
		log("Jeu en mode client => pas initialisation des sprites (tâche laissée au serveur)");
	end
end

function createSprite(dirigeablesBrId, index)
	monSpriteBrId = mapDataTree:createBranche(dirigeablesBrId, "monSprite"..index);
	
	vitX = mapDataTree:createValeur(monSpriteBrId, "float", "vitX");
	vitY = mapDataTree:createValeur(monSpriteBrId, "float", "vitY");
	vitZ = mapDataTree:createValeur(monSpriteBrId, "float", "vitZ");

	posX = mapDataTree:createValeur(monSpriteBrId, "float", "posX");
	posY = mapDataTree:createValeur(monSpriteBrId, "float", "posY");
	posZ = mapDataTree:createValeur(monSpriteBrId, "float", "posZ");

	colorR = mapDataTree:createValeur(monSpriteBrId, "float", "colorRed");
	colorG = mapDataTree:createValeur(monSpriteBrId, "float", "colorGreen");
	colorB = mapDataTree:createValeur(monSpriteBrId, "float", "colorBlue");	
	
	vitX:setValue(0.02);
	vitY:setValue(0.005+index*0.05*math.random());
	vitZ:setValue(0.005+index*0.05*math.random());

	posX:setValue(2*math.random());
	posY:setValue(2*math.random());
	posZ:setValue(0.5+2*math.random());	
	
	colorR:setValue(math.random());
	colorG:setValue(math.random());
	colorB:setValue(math.random());
	
	vitesses[index] = vitX;
	positions[index] = posX;
	colorsR[index] = colorR;
	colorsG[index] = colorG;
	colorsB[index] = colorB;
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
			for index = 1, nbrSprites do
				updateVitPos(index);
			end
		end
	end
end

function updateVitPos(index)
	vitX = vitesses[index];
	posX = positions[index];
	colorR = colorsR[index];
	colorG = colorsG[index];
	colorB = colorsB[index];
	
	-- Vitesse du sprite
	if posX:getValue() < -1 then
		if vitX:getValue() < 0 then
			vitX:setValue(0.001+0.05*math.random());
			colorR:setValue(math.random());
			colorG:setValue(math.random());
			colorB:setValue(math.random());
		end
	elseif posX:getValue() > 1 then
		if vitX:getValue() > 0 then
			vitX:setValue(-0.001-0.05*math.random());
			colorR:setValue(math.random());
			colorG:setValue(math.random());
			colorB:setValue(math.random());
		end
	end
	
	-- Position du sprite
	posX:setValue(posX:getValue() + vitX:getValue());
end