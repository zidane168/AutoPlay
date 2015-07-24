VLocker = {
	bLockLeave = true,	
	bLockFight = true,
	bLockScoff = true,	
}


_VLocker = {
	bLeave = false,	
	dwLastID = 0,	
	dwPrevID = 0,	
	nLastFrame = 0,
	nLastSys = 0,
	tLocker = {},
	tSkillCache = {},
}

VLocker.OnFrameKeyDown = function()
	if VPlugins.KeyDown() == 1 then
		return 1
	end
	return 0
end

-- ***********************************************************************************
VLocker.OpenDlg = function()			
	-- open window
	local szIniFile = "interface\\VPlugins\\ui\\autotarget.ini"
	local _frame = Station.Lookup("Normal/VLocker") or Wnd.OpenWindow(szIniFile, "VLocker")
	_frame:SetPoint("CENTER", 0, 0, "CENTER", 0, 0);
	_frame:BringToTop();	 
		 	
	local _container = Station.Lookup("Normal/VLocker/Window_Main");
	if not _container then		return	end	 
			
	local _chkTargetByClicked = _container:Lookup("CheckBox_TargetByClicked");
	if not _chkTargetByClicked then	return  end
	
	--  Interface 
	UI.SetCheck(_chkTargetByClicked	, VLocker.bLockFight);		
			
	_chkTargetByClicked.OnCheckBoxCheck 	= function() 		
		VLocker.bLockFight = true;	 	
	end			
	_chkTargetByClicked.OnCheckBoxUncheck	 = function() 	 	
		VLocker.bLockFight = false;		
	end	
	
	local _btnTargetByClicked = _container:Lookup("Button_Close");
	if not _btnTargetByClicked then	return  end
		
	_btnTargetByClicked.OnLButtonClick = function()		
		-- OutputMessage("MSG_SYS","Đóng cửa sổ mục tiêu khi chiến đấu\n");
		Wnd.CloseWindow(_frame);				
	end		
end

-- set prev target
_VLocker.SetPrevTarget = function()
	local nFrame = GetLogicFrameCount() - _VLocker.nLastFrame
	if _VLocker.dwPrevID ~= 0 and nFrame >= 0 and nFrame < 16 then
		local tar = VLocker.GetTarget(_VLocker.dwPrevID)
		if tar and tar.nMoveState ~= MOVE_STATE.ON_DEATH then
			VLocker.SetTarget(tar.dwID)
			_VLocker.dwPrevID = tar.dwID
			return OutputMessage("MSG_SYS","Chọn lại mục tiêu trước: " .. tar.szName .. "\n");		
		end
	end
end

-- check to lock target
_VLocker.AddLocker = function(fnAction)
	table.insert(_VLocker.tLocker, fnAction)
end

-- check lock fight
_VLocker.CheckLockFight = function(dwCurID, dwLastID)
	local me = GetClientPlayer()
	if VLocker.bLockFight and dwCurID == 0 and me.bFightState and me.nMoveState ~= MOVE_STATE.ON_JUMP then
		local nFrame = GetLogicFrameCount()
		if VLocker.IsDps(me) then
			if IsEnemy(me.dwID, dwLastID) then
				if nFrame < _VLocker.nLastSys or (nFrame - _VLocker.nLastSys) > 12 then
					_VLocker.nLastSys = nFrame
					--OutputMessage("MSG_SYS","Giu muc tieu dang tang cong\n");		
				end
				return true
			end
		else
			if not IsEnemy(me.dwID, dwLastID) then
				if nFrame < _VLocker.nLastSys or (nFrame - _VLocker.nLastSys) > 12 then
					_VLocker.nLastSys = nFrame
					--OutputMessage("MSG_SYS","Giu muc tieu dang hoi mau\n");	
				end
				return true
			end
		end
	end
end

-------------------------------------
-- update target
_VLocker.OnUpdateTarget = function()
	if TargetPanel_GetOpenState() then
		return
	end
	local me = GetClientPlayer()
	local dwType, dwID = me.GetTarget()
	if _VLocker.dwLastID ~= dwID then
		-- always allowed to selectself
		if dwID ~= me.dwID and _VLocker.dwLastID ~= 0 then
			local tar0 = VLocker.GetTarget(_VLocker.dwLastID)
			if tar0 and tar0.nMoveState ~= MOVE_STATE.ON_DEATH then
				for _, v in ipairs(_VLocker.tLocker) do
					if v(dwID, _VLocker.dwLastID) then
						return VLocker.SetTarget(_VLocker.dwLastID)
					end
				end
			end
		end
		-- save new last
		if not _VLocker.bLeave or dwID ~= 0 then
			_VLocker.bLeave, _VLocker.nLastFrame = false, GetLogicFrameCount()
			_VLocker.dwPrevID, _VLocker.dwLastID = _VLocker.dwLastID, dwID
		end
	end
end

-- target levave
 _VLocker.OnLeave = function()
	if VLocker.bLockLeave and _VLocker.dwLastID == arg0 then
		_VLocker.bLeave = true
	end
end

-- target enter
_VLocker.OnEnter = function()
	if VLocker.bLockLeave and _VLocker.dwLastID == arg0 then
		_VLocker.bLeave = false
		VLocker.SetTarget(_VLocker.dwLastID)
	end
end

_VLocker.OnSkillHit = function(dwCaster, dwTarget, dwID, dwLevel)
	
	if VLocker.bLockScoff and dwTarget == GetClientPlayer().dwID then
		if dwID == 234 or dwID == 236 or dwID == 402 or dwID == 1665
			or dwID == 6719 or dwID == 5692 or dwID == 5919 or dwID == 5976 or dwID == 5990
			or dwID == 6009 or dwID == 6518 or dwID == 6719
			or dwID == 2476 or dwID == 2589 or dwID == 3982 or dwID == 3985 or dwID == 3971
		then
			local _, tarID = GetClientPlayer().GetTarget()
			if tarID == dwCaster then
				_VLocker.SetPrevTarget()
			end
		end
	end
end

-- register locker
_VLocker.AddLocker(_VLocker.CheckLockFight)

---------------------------------------------------------------------
VPlugins.RegisterEvent("UPDATE_SELECT_TARGET",  _VLocker.OnUpdateTarget)
VPlugins.RegisterEvent("NPC_ENTER_SCENE", _VLocker.OnEnter)
VPlugins.RegisterEvent("NPC_LEAVE_SCENE", _VLocker.OnLeave)
VPlugins.RegisterEvent("PLAYER_ENTER_SCENE", _VLocker.OnEnter)
VPlugins.RegisterEvent("PLAYER_LEAVE_SCENE", _VLocker.OnLeave)

VPlugins.RegisterEvent("SYS_MSG", function()
	if arg0 == "UI_OME_SKILL_HIT_LOG" and arg3 == SKILL_EFFECT_TYPE.SKILL then
		_VLocker.OnSkillHit(arg1, arg2, arg4, arg5)
	elseif arg0 == "UI_OME_SKILL_EFFECT_LOG" and arg4 == SKILL_EFFECT_TYPE.SKILL then
		_VLocker.OnSkillHit(arg1, arg2, arg5, arg6)
	end
end)


VLocker.GetTarget = function(dwType, dwID)
	if not dwType then
		local me = GetClientPlayer()
		if me then
			dwType, dwID = me.GetTarget()
		else
			dwType, dwID = TARGET.NO_TARGET, 0
		end
	elseif not dwID then
		dwID, dwType = dwType, TARGET.NPC
		if IsPlayer(dwID) then
			dwType = TARGET.PLAYER
		end
	end
	if dwID <= 0 or dwType == TARGET.NO_TARGET then
		return nil, TARGET.NO_TARGET
	elseif dwType == TARGET.PLAYER then
		return GetPlayer(dwID), TARGET.PLAYER
	elseif dwType == TARGET.DOODAD then
		return GetDoodad(dwID), TARGET.DOODAD
	else
		return GetNpc(dwID), TARGET.NPC
	end
end

VLocker.SetTarget = function(dwType, dwID)
	if not dwType or dwType <= 0 then
		dwType, dwID = TARGET.NO_TARGET, 0
	elseif not dwID then
		dwID, dwType = dwType, TARGET.NPC
		if IsPlayer(dwID) then
			dwType = TARGET.PLAYER
		end
	end
	SetTarget(dwType, dwID)
end

VLocker.IsDps = function(tar)
	tar = tar or GetClientPlayer()
	local mnt = tar.GetKungfuMount()
	return not mnt or (mnt.dwSkillID ~= 10080 and mnt.dwSkillID ~= 10028 and mnt.dwSkillID ~= 10176)
end
