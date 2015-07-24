TargetList = {
	szIniFile =  "interface\\VPlugins\\ui\\TargetList.ini",
	tNpc = {},
	tPlayer = {},
	tListMode = {"NPC","Player",},
	tListTitle = {"NPC","Player",},
	nListMode = 2;
}

TargetList.Open = function()
	local frame = Wnd.OpenWindow(TargetList.szIniFile, "TargetList")
	frame:Show()
	frame:BringToTop()
	return frame
end

TargetList.UpdateMode = function()
	if  TargetList.frame then
	
		local win = TargetList.frame:Lookup("Wnd_Mode")
		local ttm = win:Lookup("","Text_Mode")
		local szMode = TargetList.tListMode[TargetList.nListMode]
		ttm:SetText(szMode)
		TargetList.frame:Lookup("","Text_Title"):SetText(TargetList.tListTitle[TargetList.nListMode])
		local hnd = TargetList.frame:Lookup("","Handle_List")
		if not hnd then
			OutputMessage("MSG_SYS","not handle list");
		else
			TargetList.UpdateListItem(hnd)
			hnd:FormatAllItemPos()
		end
		
		if TargetList.nListMode > 1 then
			win:Lookup("Btn_Left"):Enable(1)
		else
			win:Lookup("Btn_Left"):Enable(0)
		end
		if TargetList.nListMode < #TargetList.tListMode then
			win:Lookup("Btn_Right"):Enable(1)
		else
			win:Lookup("Btn_Right"):Enable(0)
		end
	end
end

TargetList.OnFrameCreate = function()
	TargetList.frame = this
	TargetList.UpdateMode();
	
end

TargetList.OnFrameBreathe= function()
	 local hnd = TargetList.frame:Lookup("","Handle_List")
	 TargetList.UpdateListItem(hnd)
	 hnd:FormatAllItemPos()
end

TargetList.OnLButtonClick = function()
	local szName = this:GetName()
	if szName == "Btn_Left" then
		if TargetList.nListMode > 1 then
			TargetList.nListMode = TargetList.nListMode - 1
			TargetList.UpdateMode()
		end
	elseif szName == "Btn_Right" then
		if TargetList.nListMode < #TargetList.tListMode then
			TargetList.nListMode = TargetList.nListMode + 1
			TargetList.UpdateMode()
		end
	elseif szName == "Button_Close" then
		Wnd.CloseWindow(this:GetParent());
	end
end


-- TargetList.OnCheckBoxCheck = function()
	-- local szName = this:GetName()
	-- if szName == "Check_Minimize" then
		-- TargetList.bCollapse = true
		-- TargetList.UpdateSize()
	-- end
-- end
-- TargetList.OnCheckBoxUncheck = function()
	-- local szName = this:GetName()
	-- if szName == "Check_Minimize" then
		-- TargetList.bCollapse = false
		-- TargetList.UpdateSize()
	-- end
-- end


TargetList.UpdateSize = function()
	local frame = TargetList.frame
	local wList = frame:Lookup("Wnd_Mode")
	if TargetList.bCollapse  then
		wList:Hide()
		frame:Lookup("", "Image_Bg"):SetSize(240, 30)
		frame:SetSize(240, 30)
	else
		frame:Lookup("", "Image_Bg"):SetSize(240, 385)
		frame:SetSize(240, 420)
		wList:Show()
	end
end
TargetList.ListItemCompare = function(a, b)
	if not a or not b then
		return true
	end
	
	return a.nDis < b.nDis
end
TargetList.UpdateListItem = function(hnd)

	local aList = {}
	local aItem,me = {}, GetClientPlayer()
	local nCount = hnd:GetItemCount()
	if TargetList.nListMode == 2 then
		aList = TargetList.GetAllPlayer()
	else
		aList = TargetList.GetAllNpc()
	end
	for _, v in ipairs(aList) do
		local item = {
			dwID = v.dwID, nMoveState = v.nMoveState,
			szName = v.szName, nLevel = v.nLevel,
			dwForceID = v.dwForceID,
		}
		item.nIndex = #aItem + 1

		item.nDis = TargetList.GetDistance(v)
		item.nAngle = TargetList.GetRelAngle(me, v)

		table.insert(aItem, item)
	end
	if #aItem > 1 then
		table.sort(aItem, TargetList.ListItemCompare)
	end
	for k, v in ipairs(aItem) do
		if k > 10 then
			break
		end
		local h = nil
		if k <= nCount then
			h = hnd:Lookup(k - 1)
		else
			h = hnd:AppendItemFromIni(TargetList.szIniFile,"Handle_Lister","List_"..k)
			h:Show()
		end
		h.dwID, h.szName = v.dwID, v.szName
		h:Lookup("Text_Player"):SetText(h.szName)
		
		
		local imgItem = h:Lookup("Image_Circle")
		if not imgItem then
			OutputMessage("MSG_SYS","not image item");
		end
		imgItem:SetSize(25,25);
		
		local imgTar = h:Lookup("Image_Target")
		if not imgTar then
			OutputMessage("MSG_SYS","not image item");
		end
		imgTar:SetSize(12,12);
		imgTar:SetRelPos(5,-4);
		
		local imgArrow = h:Lookup("Image_Arrow")
		if not imgArrow then
			OutputMessage("MSG_SYS","not image item");
		end
		imgArrow:SetSize(12,12);
		imgArrow:SetRelPos(4.5,5);
		imgArrow:SetRotate((v.nAngle-90) * (math.pi / 180));
		
		local txtDis = h:Lookup("Text_Distance")
		if not txtDis then
			OutputMessage("MSG_SYS","not text");
		end
		txtDis:SetText(v.nDis)
		h:SetRelPos(10,(k-1)*35);
		
	end
	for i = nCount - 1, #aItem, -1 do
		hnd:RemoveItem(i)
	end
end
TargetList.GetDistance = function(nX, nY, nZ)
	local me = GetClientPlayer()
	if not nY and not nZ then
		local tar = nX
		nX, nY, nZ = tar.nX, tar.nY, tar.nZ
	elseif not nZ then
		return math.floor(math.floor(((me.nX - nX) ^ 2 + (me.nY - nY) ^ 2) ^ 0.5)/64)
	end
	return math.floor(math.floor(((me.nX - nX) ^ 2 + (me.nY - nY) ^ 2 + (me.nZ/8 - nZ/8) ^ 2) ^ 0.5)/64)
end
TargetList.GetRelAngle = function(me, tar)
	local nX, nY = tar.nX - me.nX, tar.nY - me.nY
	
	local nDeg, nFace =  0, me.nFaceDirection / 256 * 360
	if nY == 0 then
		if nX < 0 then
			nDeg = 180
		end
	elseif nX == 0 then
		if nY > 0 then
			nDeg = 90
		else
			nDeg = 270
		end
	else
		nDeg = math.deg(math.atan(nY / nX))
		if nX < 0 then
			nDeg = 180 + nDeg
		elseif nY < 0 then
			nDeg = 360 + nDeg
		end
	end
	local nAngle = nFace - nDeg
	-- if nAngle < -180 then
		-- nAngle = nAngle + 360
	-- elseif nAngle > 180 then
		-- nAngle = nAngle - 360
	-- end
	
	return nAngle
end

TargetList.GetAllPlayer = function(nLimit)
	local aPlayer = {}
	local me = GetClientPlayer()
	for k, _ in pairs(TargetList.tPlayer) do
		local p = GetPlayer(k)
		if not p then
			TargetList.tPlayer[k] = nil
		elseif p.szName ~= "" and p.szName~=me.szName then
			table.insert(aPlayer, p)
			if nLimit and #aPlayer == nLimit then
				break
			end
		end
	end
	return aPlayer
end


TargetList.GetAllNpc = function(nLimit)
	local aNpc = {}
	for k, _ in pairs(TargetList.tNpc) do
		local p = GetNpc(k)
		if not p then
			TargetList.tNpc[k] = nil
		else
			table.insert(aNpc, p)
			if nLimit and #aNpc == nLimit then
				break
			end
		end
	end
	return aNpc
end
RegisterEvent("PLAYER_ENTER_SCENE", function() TargetList.tPlayer[arg0] = true end)
RegisterEvent("PLAYER_LEAVE_SCENE", function() TargetList.tPlayer[arg0] = nil end)
RegisterEvent("NPC_ENTER_SCENE", function() TargetList.tNpc[arg0] = true end)
RegisterEvent("NPC_LEAVE_SCENE", function() TargetList.tNpc[arg0] = nil end)
