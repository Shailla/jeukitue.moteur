
-- Constantes
windowWeight = 400
windowHeight = 400

-- Variables globales
window = 0

backButton = 0


function onLoad()
	log("Version Lua : " .. _VERSION)

	-- ----------------------------------------
	-- Creation IHM
	-- ----------------------------------------

	-- Ouverture fenêtre
	window = Window();
	window:setTitle("A propos");
	
	local compilationLabel = window:addLabel("Date de compilation : "..getConstant("COMPILATION_DATE"));
	compilationLabel:expandHoriz();
	
	local openglLabel = window:addLabel("Version OpenGL : "..getConstant("OPENGL_VERSION"));
	openglLabel:expandHoriz();
	
	local gluLabel = window:addLabel("Version GLU : "..getConstant("GLU_VERSION"));
	gluLabel:expandHoriz();
	
	local fmodLabel = window:addLabel("Version FMOD : "..getConstant("FMOD_VERSION"));
	fmodLabel:expandHoriz();
	
	local agarLabel = window:addLabel("Version Agar : "..getConstant("AGAR_VERSION"));
	agarLabel:expandHoriz();
	
	local creatorLabel = window:addLabel("JKT a été developpé par Jean-Claude Vogel, ahuut@yahoo.fr");
	creatorLabel:expandHoriz();
	
	-- Boutons
	window:addSeparator();
	
	local boxBoutonsAudio = window:addBoxHoriz();
	boxBoutonsAudio:expandHoriz();
	backButton = boxBoutonsAudio:addButton("Retour");
		
	-- Affichage de la fenêtre
	window:setSize(windowWeight, windowHeight);
	
	local screenSizeWeight, screenSizeHeight = getScreenSize();
	window:setPosition((screenSizeWeight - windowWeight) / 2, (screenSizeHeight - windowHeight) / 2);
	
	window:hide();
end

function eventManager(event)
	local actionId = event:getActionId();
	log("Event actionId=" .. actionId);
	
	if actionId == 10001 then		-- Show about
		window:show();

	elseif actionId == 1001 then	-- Hide menu
		window:hide();

	elseif actionId == 1004 then		-- Widget event
		local source = event:getSource();
		
		-- Enregistrement de la configuration du joueur
		if source == backButton then
			window:hide();
			pushEvent(1002);	-- Show main menu
		
		-- Evênement non-pris en compte
		else
			log("Evenement ignoré ("..event:__tostring()..")");
		end
	end
end
