
-- Variables globales
window = 0
checkboxSkinJoueurVisibility = 0;
checkboxOutlineJoueurVisibility = 0;
checkboxAxesMeterVisibility = 0;
checkboxCubicMeterVisibility = 0;

appliquerJoueurButton = 0;
reinitialiserJoueurButton = 0;
closeButton = 0;

function onLoad()
	log("Version Lua : " .. _VERSION)

	-- Ouverture fenêtre
	window = Window();
	window:setTitle("Configuration avancee");
	window:setSize(200, 200);
	window:setPosition(0, 0);
	local notebook = window:addNotebook();

	-- Onglet joueur
	local tabJoueur = notebook:addTab("Joueur");
	local boxJoueur = tabJoueur:addBoxVert();
	checkboxSkinJoueurVisibility = boxJoueur:addCheckbox("Voir le skin du joueur");
	checkboxOutlineJoueurVisibility = boxJoueur:addCheckbox("Voir les contours solides des joueurs");
	checkboxAxesMeterVisibility = boxJoueur:addCheckbox("Voir les axes");
	checkboxCubicMeterVisibility = boxJoueur:addCheckbox("Voir un cube d'un metre");
	
	loadJoueurConfiguration();	-- Chargement des valeurs courantes
	local boxBoutonsJoueur = boxJoueur:addBoxHoriz();
	appliquerJoueurButton = boxBoutonsJoueur:addButton("Appliquer");
	reinitialiserJoueurButton = boxBoutonsJoueur:addButton("Reinitialiser");
	
	closeButton = boxJoueur:addButton("Fermer");

	-- Onglet caméra
	local tabCamera = notebook:addTab("Camera");
	local boxCamera = tabCamera:addBoxVert();
	boxCamera:addNumeric("...", "pixels");
	boxCamera:addNumeric("...", "pixels");

	local drivers = getAvailableAudioDrivers();
	log("Liste des drivers :");
	for k,v in pairs(drivers) do
		log("  "..k.." : "..v);
	end
	
	window:hide();
end

-- Lit la configuration joueur par défaut
function loadJoueurConfiguration()
	checkboxSkinJoueurVisibility:setValue(getConfigurationParameter("SKIN_VISIBILITY"));
	checkboxOutlineJoueurVisibility:setValue(getConfigurationParameter("PLAYER_OUTLINE_VISIBILITY"));
	checkboxAxesMeterVisibility:setValue(getConfigurationParameter("CUBIC_METER_VISIBILITY"));
	checkboxCubicMeterVisibility:setValue(getConfigurationParameter("AXES_METER_VISIBILITY"));
end

-- Enregistre la configuration joueur
function appliqueJoueurConfiguration()
	setConfigurationParameter("SKIN_VISIBILITY", checkboxSkinJoueurVisibility:getValue());
	setConfigurationParameter("PLAYER_OUTLINE_VISIBILITY", checkboxOutlineJoueurVisibility:getValue());
	setConfigurationParameter("CUBIC_METER_VISIBILITY", checkboxAxesMeterVisibility:getValue());
	setConfigurationParameter("AXES_METER_VISIBILITY", checkboxCubicMeterVisibility:getValue());
end

function eventManager(event)
	local type = event:getType();

	-- Action event
	if type == 1 then
		local actionId = event:getActionId();
		
		log("Action event : "..actionId);
		
		if actionId == 1019 then		-- Show advanced configuration window
			log("SHOW");
			window:show();
		end
		
	-- Widget event
	elseif type == 3 then	
		local source = event:getSource();

		-- Enregistrement de la configuration du joueur
		if source == appliquerJoueurButton then
			log("Enregistrement de la configuration du joueur");
			appliqueJoueurConfiguration();
			saveConfiguration();
			
		-- Lecture de la configuration joueur par défaut
		elseif source == reinitialiserJoueurButton then
			log("Lecture de la configuration joueur par défaut");
			loadJoueurConfiguration();
			
		-- Fermer la fenêtre
		elseif source == closeButton then
			log("HIDE");
			window:hide();
			
		-- Evênement non-pris en compte
		else
			log("Evenement ignoré ("..event:__tostring()..")");
		end
	end
end
