
-- Constantes
windowWeight = 400
windowHeight = 400

-- Variables globales
window = 0

backButton = 0
appliquerAudioButton = 0
reinitialiserAudioButton = 0

audioDriverCombo = 0
audioRecordDriverCombo = 0
audioOutputCombo = 0

function onLoad()
	log("Version Lua : " .. _VERSION)

	-- ----------------------------------------
	-- Creation IHM
	-- ----------------------------------------

	-- Ouverture fenêtre
	window = Window();
	window:setTitle("Configuration audio");
	
	-- Combos
	audioDriverCombo = window:addComboList("Pilote audio :");
	audioDriverCombo:expandHoriz();
	audioDriverCombo:setItemSize("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	
	audioRecordDriverCombo = window:addComboList("Pilote micro :");
	audioRecordDriverCombo:expandHoriz();
	audioRecordDriverCombo:setItemSize("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	
	audioOutputCombo = window:addComboList("Sortie son :");
	audioOutputCombo:expandHoriz();
	audioOutputCombo:setItemSize("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	
	-- Boutons
	window:addSeparator();
	
	local boxBoutonsAudio = window:addBoxHoriz();
	boxBoutonsAudio:expandHoriz();
	backButton = boxBoutonsAudio:addButton("Retour");
	appliquerAudioButton = boxBoutonsAudio:addButton("Appliquer");
	reinitialiserAudioButton = boxBoutonsAudio:addButton("Reinitialiser");

	-- ----------------------------------------
	-- Chargement des informations
	-- ----------------------------------------
	
	-- Information sur les pilotes audio
	local audioDrivers = getAvailableAudioDrivers();
	log("Liste des pilotes disponibles :");
	
	for id,name in pairs(audioDrivers) do
		log("  "..id.." : "..name);
		audioDriverCombo:addItem(name, id);
	end
	
	-- Information sur les pilotes d'enregistrement audio	
	local audioRecordDrivers = getAvailableAudioRecordDrivers();
	log("Liste des pilotes d'enregistrement disponibles :");
	
	for id,name in pairs(audioRecordDrivers) do
		log("  "..id.." : "..name);
		audioRecordDriverCombo:addItem(name, id);
	end
	
	-- Information sur les sorties audio	
	local audioOutputs = getAvailableAudioOutputs();
	log("Liste des sorties audio disponibles :");
	
	for id,name in pairs(audioOutputs) do
		log("  "..id.." : "..name);
		audioOutputCombo:addItem(name, id);
	end
		
	-- Chargement des valeurs courantes
	loadAudioConfiguration();
		
	-- Affichage de la fenêtre
	window:setSize(windowWeight, windowHeight);
	
	local screenSizeWeight, screenSizeHeight = getScreenSize();
	window:setPosition((screenSizeWeight - windowWeight) / 2, (screenSizeHeight - windowHeight) / 2);
	
	window:hide();
end

--
-- Lit la configuration joueur par défaut
--
function loadAudioConfiguration()
	-- Lit driver audio courant
	local currentAudioDriver = getConfigurationParameter("AUDIO_DRIVER");
	audioDriverCombo:selectItemByUserId(currentAudioDriver);
	
	-- Lit driver d'enregistrement audio courant
	local currentAudioRecordDriver = getConfigurationParameter("AUDIO_RECORD_DRIVER");
	audioRecordDriverCombo:selectItemByUserId(currentAudioRecordDriver);
	
	-- Lit sortie audio courante
	local currentAudioOutput = getConfigurationParameter("AUDIO_OUTPUT");
	audioOutputCombo:selectItemByUserId(currentAudioOutput);
end

--
-- Enregistre la configuration joueur
--
function appliqueAudioConfiguration()
	initAudio();
end

function eventManager(event)
	local actionId = event:getActionId();
	log("Event actionId=" .. actionId);
	
	if actionId == 10000 then		-- Show configuration audio
		log("SHOW");
		window:show();

	elseif actionId == 1001 then	-- Hide menu
		log("HIDE");
		window:hide();
		
	elseif actionId == 1004 then		-- Widget event
		local source = event:getSource();
		
		-- Enregistrement de la configuration du joueur
		if source == backButton then
			window:hide();
			pushEvent(1104);	-- Show configuration menu action
		
		-- Enregistrement de la configuration du joueur
		elseif source == appliquerAudioButton then
			log("Activation de la nouvelle configuration audio");
			appliqueAudioConfiguration();
			
		-- Lecture de la configuration joueur par défaut
		elseif source == reinitialiserAudioButton then
			log("Lecture de la configuration audio actuelle");
			loadAudioConfiguration();
			
		-- Evênement non-pris en compte
		else
			log("Evenement ignoré ("..event:__tostring()..")");
		end
	end
end
