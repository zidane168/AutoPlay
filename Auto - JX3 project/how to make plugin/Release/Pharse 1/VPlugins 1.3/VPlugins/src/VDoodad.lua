_VDoodad = {
	tDoodad = {},
	bLoot = true,
	aDoodad = {},	-- new
}


VDoodad = {
	bLoot = true,				
	bLootFight = false,		
	bLootGray = false,		
	bLootWhite = true,		
	bLootGreen = true,	
	tLootFilter = {},		
	bLootOnly = false,	
	tLootOnly = {},			
	bQuest = true,			
	bInteract = true,		
	bLootCraft = true,		
	tCustom = {},		
	tNameColor = { 196, 64, 255 },	
	nLastFrame = 0,
}

VPlugins.RegisterCustomData("VDoodad")


VDoodad.OnFrameKeyDown = function()
	if VPlugins.KeyDown() == 1 then
		return 1
	end
	return 0
end

-- ***********************************************************************************
VDoodad.OpenDlg = function()
	 local szIniFile = "interface\\VPlugins\\ui\\autopickup.ini"
	 --local _frame = Station.Lookup("Normal/autopickup") or Wnd.OpenWindow(szIniFile, "autopickup")
	 
	 local _frame = Station.Lookup("Normal/VDoodad") or Wnd.OpenWindow(szIniFile, "VDoodad")
	 _frame:Show()
	 _frame:SetPoint("CENTER", 0, 0, "CENTER", 0, 0);
	 _frame:BringToTop()	

	 local _container = Station.Lookup("Normal/VDoodad/Window_Main");
	 if not _container then		return	end	 
			
	 local _chkAutoPick = _container:Lookup("CheckBox_AutoPickup");
	 if not _chkAutoPick then	return  end
	 
	 local _chkGarbage = _container:Lookup("CheckBox_Garbage");
	 if not _chkGarbage then	return  end
	 
	 local _chkWhiteItem = _container:Lookup("CheckBox_WhiteItem");
	 if not _chkWhiteItem then	return  end
	 
	 local _chkGreenItem = _container:Lookup("CheckBox_GreenItem");
	 if not _chkGreenItem then	return  end
	 
	 local _chkAutoMine = _container:Lookup("CheckBox_AutoMine");
	 if not _chkAutoMine then	return  end
	  
	 
	--  Interface 
	UI.SetCheck(_chkAutoPick	, VDoodad.bLoot);		
	UI.SetCheck(_chkGarbage	, 	not VDoodad.bLootGray);
	UI.SetCheck(_chkWhiteItem	, not VDoodad.bLootWhite);
	UI.SetCheck(_chkGreenItem	, not VDoodad.bLootGreen);
	UI.SetCheck(_chkAutoMine	, VDoodad.bLootCraft);
	
		-- Auto doodad
	_chkAutoPick.OnCheckBoxCheck 	= function()
		VDoodad.bLoot = true;		
		_VDoodad.Reload()
	end			
	
	_chkAutoPick.OnCheckBoxUncheck = function() 	 	
		VDoodad.bLoot = false;	
		_VDoodad.Reload()
	end	
	
	_chkGarbage.OnCheckBoxCheck 	= function()		
		VDoodad.bLootGray = false;
		_VDoodad.Reload()
	end			
	_chkGarbage.OnCheckBoxUncheck = function() 	 		
		VDoodad.bLootGray = true;	
		_VDoodad.Reload()
	end	
	
	_chkWhiteItem.OnCheckBoxCheck 	= function()		
		VDoodad.bLootWhite = false;	
		_VDoodad.Reload()
	end			
	_chkWhiteItem.OnCheckBoxUncheck = function() 	 	
		VDoodad.bLootWhite = true;
		_VDoodad.Reload()
	end	
	
	_chkGreenItem.OnCheckBoxCheck 	= function()		
		VDoodad.bLootGreen = false;	
		_VDoodad.Reload()
	end			
	_chkGreenItem.OnCheckBoxUncheck = function() 	 	
		VDoodad.bLootGreen = true;	
		_VDoodad.Reload()
	end	
	
	_chkAutoMine.OnCheckBoxCheck 	= function()		
	VDoodad.bLootCraft = true;	
	_VDoodad.Reload()
	end			
	_chkAutoMine.OnCheckBoxUncheck = function() 	 	
	VDoodad.bLootCraft = false;
	_VDoodad.Reload()	
	end	

	local _chkAutoMine = _container:Lookup("CheckBox_AutoMine");
	if not _chkAutoMine then	return  end
	
	local _btnClose = _container:Lookup("Button_Close");	
	if not _btnClose then 
		OutputMessage("MSG_SYS"," cant load Close\n");
		return
	end
	
	_btnClose.OnLButtonClick = function()		
		OutputMessage("MSG_SYS","Đóng cửa sổ thiết lập nhặt item\n");
		Wnd.CloseWindow(_frame);				
	end		
		
 end

-- ***********************************************************************************
_VDoodad.TryAdd = function(dwID, bDelay)
	local d = GetDoodad(dwID)
	if d then
		local data, me = nil, GetClientPlayer()
		if d.nKind == DOODAD_KIND.CORPSE or d.nKind == DOODAD_KIND.NPCDROP then
			if bDelay then
				return VPlugins.DelayCall(500, function() _VDoodad.TryAdd(dwID) end)
			end
			if VDoodad.bLoot and d.CanLoot(me.dwID) then
				data = { loot = true }
			elseif VDoodad.bLootCraft and GetDoodadTemplate(d.dwTemplateID).dwCraftID == 3 then --and VDoodad.tCustom[d.szName]
				data = { craft = true }
			end
		elseif VDoodad.bLootCraft then -- VDoodad.tCraft[d.szName] or (VDoodad.bCustom then and VDoodad.tCustom[d.szName]) then
			data = { craft = true }
		elseif d.HaveQuest(me.dwID) then	-- quest
			if VDoodad.bQuest then
				data = { quest = true }
			end
		end
		if data then
			_VDoodad.tDoodad[dwID] = data
		end
	end
end

-- ***********************************************************************************
-- remove doodad
_VDoodad.Remove = function(dwID)
	local data = _VDoodad.tDoodad[dwID]
	if data then
		_VDoodad.tDoodad[dwID] = nil
	end
end

_VDoodad.GetOpenDoodadID = function()
	local dwID = _VDoodad.dwOpenID
	if dwID then
		_VDoodad.dwOpenID = nil
	elseif VDoodad.bManualFilter2 then
		local tObject = Scene_SelectObject("all") or {}
		for _, v in pairs(tObject) do
			if v["Type"] == TARGET.DOODAD and IsCorpseAndCanLoot(v["ID"]) then
				dwID = v["ID"]
				break
			end
		end
	end
	return dwID	
end

_VDoodad.OnAutoDoodad = function()
		local hL = Station.Lookup("Normal/LootList", "Handle_LootList")
		if hL and _VDoodad.bhLClear then
			local nFrame = GetLogicFrameCount() - _VDoodad.nLastFrame
			if nFrame >= 0 and nFrame < 16 then
				hL:Clear()
				_VDoodad.bhLClear = false
			end
		end
	local me = GetClientPlayer()
	if not me or me.GetOTActionState() ~= 0
		or (me.nMoveState ~= MOVE_STATE.ON_STAND and me.nMoveState ~= MOVE_STATE.ON_FLOAT)
		or IsDialoguePanelOpened()
	then
		return
	end
	for k, v in pairs(_VDoodad.tDoodad) do
		local d, bKeep, bIntr = GetDoodad(k), false, false
		if not d or not d.CanDialog(me) then
			bKeep = d ~= nil
		elseif v.loot then	
			bKeep = true	
			bIntr = not me.bFightState or VDoodad.bLootFight
			if bIntr then
				_VDoodad.dwOpenID = k
			end
		elseif (v.craft or d.HaveQuest(me.dwID)) and VDoodad.bLootCraft then	
			bIntr = not me.bFightState and not me.bOnHorse and VDoodad.bInteract
			if d.nKind == DOODAD_KIND.QUEST then
				_VDoodad.dwOpenID = k
			end
			if d.nKind == DOODAD_KIND.READ then
				bKeep = false
			else
				bKeep = true
			end
			
		end
		if not bKeep then
			_VDoodad.Remove(k)
		end
		if bIntr then
			VPlugins.BreatheCallDelayOnce("AutoDoodad", 500)
			return InteractDoodad(k)
		end
	end
end

-- open doodad (loot)
_VDoodad.OnOpenDoodad = function(dwID)
	_VDoodad.Remove(dwID)
	local d = GetDoodad(dwID)
	if d then
		local bP, bClear, me = false, true, GetClientPlayer()
		if VDoodad.bInteract and VDoodad.bLootCraft and GetDoodadTemplate(d.dwTemplateID).dwCraftID == 3 and d.nKind == DOODAD_KIND.QUEST 
			--and VDoodad.tCustom[d.szName] and GetDoodadTemplate(d.dwTemplateID).dwCraftID == 3
		then
			OutputMessage("MSG_SYS","bP = true\n");
			_VDoodad.tDoodad[dwID] = { craft = true }
			bP = true
		end
		-- money
		local nM = d.GetLootMoney() or 0
		if nM > 0 then
			LootMoney(d.dwID)
		end
		-- items
		for i = 0, 31 do
			local it, bRoll, bDist = d.GetLootItem(i, me)
			if it then
				if bDist and bClear then
					bClear = false
					if bP then
						_VDoodad.tDoodad[dwID] = nil
						bP = false
					end
				end
				local bLoot, szName = true, GetItemNameByItem(it)
				if bP then
					bLoot = true
				elseif (it.nQuality == 0 and not VDoodad.bLootGray)
					or (it.nQuality == 1 and not VDoodad.bLootWhite)
					or (it.nQuality == 2 and not VDoodad.bLootGreen)
					--or VDoodad.tLootFilter[szName] == true
				then
					bLoot = false
					if it.nGenre == ITEM_GENRE.TASK_ITEM then
						bLoot = true
					end
				end
				if bLoot then
					LootItem(d.dwID, it.dwID)
				end
			end
		end
		if bClear then
			local hL = Station.Lookup("Normal/LootList", "Handle_LootList")
			if hL then
				hL:Clear()
				_VDoodad.bhLClear = true
				_VDoodad.nLastFrame = GetLogicFrameCount()
			end
		end
	end
end

_VDoodad.Reload = function()
	_VDoodad.tDoodad = {}
	for k, _ in pairs(_VDoodad.GetAllDoodadID()) do
		_VDoodad.TryAdd(k)
	end
end

_VDoodad.GetAllDoodadID = function()
	return _VDoodad.aDoodad
end

VPlugins.RegisterEvent("DOODAD_ENTER_SCENE", function() _VDoodad.TryAdd(arg0, true) end)
VPlugins.RegisterEvent("DOODAD_LEAVE_SCENE", function() _VDoodad.Remove(arg0) end)

-- VPlugins.RegisterEvent("OPEN_DOODAD", _VDoodad.OnLootDoodad)

VPlugins.RegisterEvent("HELP_EVENT", function()
	if arg0 == "OnOpenpanel" and arg1 == "LOOT" and VDoodad.bLoot then
		local dwOpenID =  _VDoodad.GetOpenDoodadID()
		if dwOpenID then
			_VDoodad.OnOpenDoodad(dwOpenID)
		end
	end
end)

VPlugins.BreatheCall("AutoDoodad", _VDoodad.OnAutoDoodad)

-- get item name by item
if not GetItemNameByItem then
	function GetItemNameByItem(item)
		if item.nGenre == ITEM_GENRE.BOOK then
			local nBookID, nSegID = GlobelRecipeID2BookID(item.nBookID)
			return Table_GetSegmentName(nBookID, nSegID) or g_tStrings.BOOK
		else
			return Table_GetItemName(item.nUiId)
		end
	end
end

-- dialogue panel
if not IsDialoguePanelOpened then
	function IsDialoguePanelOpened()
		local frame = Station.Lookup("Normal/DialoguePanel")
		if frame and frame:IsVisible() then
			return true
		end
		return false
	end
end