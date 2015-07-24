-- *********************************************************************************
-- VPlugins Interface
-- *********************************************************************************

local WINDOW_NOT_FOUND 		= 1;
local BUTTON_NOT_FOUND 		= 2;
local CHECKBOX_NOT_FOUND 	= 3;
local CLOSE_FORM 			= 50;
local REPAIR				= 100;
local REPAIR_FINISHED		= 101;


_VPlugins= {
	tDelayCall = {},
	tEvent = {},
}

-- *********************************************************************************
VPlugins = 
{		
	szMessage 		=
	{
		[WINDOW_NOT_FOUND] 		= "Không tìm thấy window: ",
		[BUTTON_NOT_FOUND] 		= "Không tìm thấy button: ",
		[CHECKBOX_NOT_FOUND] 	= "Không tìm thấy checkbox: ",
		[CLOSE_FORM]			= "Đóng cửa sổ: ",
		[REPAIR] 				= "Sửa đồ thành công",
		[REPAIR_FINISHED]		= "Bạn không có đồ gì cần sửa",
	},	
	tBreatheCall = {},
}

-- ***********************************************************************************
VPlugins.CloseAllWindow = function()
	local _tframe = {};
	
	local szPath = {
		[1] = "Normal/autorepair",
		[2] = "Normal/autopickup",
		[3] = "Normal/autotarget",
	};
	
	for i=1, table.getn(szPath) do
		local _frame = Station.Lookup(szPath[i]);
		if (_frame) then
			table.insert(_tframe, _frame);
		end
	end

	for iIndex, hHandle in pairs(_tframe) do
		Wnd.CloseWindow(hHandle);
	end
end

-- ***********************************************************************************
VPlugins.LoadingEnd = function()			

	-- open window
	local _frm, szIniFile = nil, "interface\\VPlugins\\ui\\main.ini"
	local _frame = Station.Lookup("Normal/VPlugins") or Wnd.OpenWindow(szIniFile, "VPlugins")
	_frame:Show()
	_frame:BringToTop()			 
		 	
	_frm = Station.Lookup("Normal/VPlugins/Window_Main");
	if not _frm then	return	end	 
		
	local _btnPickup = _frm:Lookup("Button_Pickup");	
	if not _btnPickup then	return	end
	
	local _btnRepair = _frm:Lookup("Button_Repair");
	if not _btnRepair then	return		end
	
	local _btnShowTarget = _frm:Lookup("Button_ShowTarget");
	if not _btnShowTarget then	return		end
	
	local _btnTarget = _frm:Lookup("Button_Target");
	if not _btnTarget then	return		end
	
	-- local _btnReload = _frm:Lookup("Button_Reload");
		
	_btnRepair.OnLButtonClick = function()		
		VPlugins.CloseAllWindow();
		VAutoRepair.OpenDlg();				
	end
		
	_btnPickup.OnLButtonClick = function()	
		VPlugins.CloseAllWindow();
		VDoodad.OpenDlg();					
	end
		
	_btnShowTarget.OnLButtonClick = function()			
		VPlugins.CloseAllWindow();	
		TargetList.Open();
	end			
		
	_btnTarget.OnLButtonClick = function()	
		VPlugins.CloseAllWindow();
		VLocker.OpenDlg();
	end			
		
	OutputMessage("MSG_SYS","**** Tải VPlugins 0.8 thành công ****\n");
	
end

--************************************************************************
-- On frame
-- handle event
_VPlugins.EventHandler = function(szEvent)
	local tEvent = _VPlugins.tEvent[szEvent]
	if tEvent then
		for k, v in pairs(tEvent) do
			local res, err = pcall(v)
			if not res then
				OutputMessage("MSG_SYS","Plugin Event Handler error..... \n");
			end
		end
	end
end

VPlugins.BreatheCall = function(szKey, fnAction, nTime)
	local key = StringLowerW(szKey)
	if type(fnAction) == "function" then
		local nFrame = 1
		if nTime and nTime > 0 then
			nFrame = math.ceil(nTime / 62.5)
		end
		VPlugins.tBreatheCall[key] = { 
			fnAction = fnAction, 
			nNext = GetLogicFrameCount() + 1, 
			nFrame = nFrame 
		}
	else
		VPlugins.tBreatheCall[key] = nil
	end
end

VPlugins.BreatheCallDelay = function(szKey, nTime)
	local t = VPlugins.tBreatheCall[StringLowerW(szKey)]
	if t then
		t.nFrame = math.ceil(nTime / 62.5)
		t.nNext = GetLogicFrameCount() + t.nFrame
	end
end

VPlugins.BreatheCallDelayOnce = function(szKey, nTime)
	local t = VPlugins.tBreatheCall[StringLowerW(szKey)]
	if t then
		t.nNext = GetLogicFrameCount() + math.ceil(nTime / 62.5)
	end
end

VPlugins.DelayCall = function(nDelay, fnAction)
	local nTime = nDelay + GetTime() -- GetTime() -> lấy thời gian hiện tại
	table.insert(_VPlugins.tDelayCall, { nTime = nTime, fnAction = fnAction })
end

-- ***********************************************************************************
VPlugins.RegisterEvent = function(szEvent, fnAction)
	local szKey = nil
	local nPos = StringFindW(szEvent, ".")
	if nPos then
		szKey = string.sub(szEvent, nPos + 1)
		szEvent = string.sub(szEvent, 1, nPos - 1)
	end
	if not _VPlugins.tEvent[szEvent] then
		_VPlugins.tEvent[szEvent] = {}
		RegisterEvent(
			szEvent, 
			function() 
				_VPlugins.EventHandler(szEvent) 
			end)
	end
	local tEvent = _VPlugins.tEvent[szEvent]
	if fnAction then
		if not szKey then
			table.insert(tEvent, fnAction)
		else
			tEvent[szKey] = fnAction
		end
	else
		if not szKey then
			_VPlugins.tEvent[szEvent] = {}
		else
			tEvent[szKey] = nil
		end
	end
end

VPlugins.UnRegisterEvent = function(szEvent)
	VPlugins.RegisterEvent(szEvent, nil)
end

VPlugins.RegisterCustomData = function(szVarPath)
	if _G and type(_G[szVarPath]) == "table" then
		for k, _ in pairs(_G[szVarPath]) do
			RegisterCustomData(szVarPath .. "." .. k)
		end
	else
		RegisterCustomData(szVarPath)
	end
end
RegisterEvent("LOADING_END",VPlugins.LoadingEnd);	-- load completed

VPlugins.RegisterEvent("DOODAD_ENTER_SCENE", function() _VDoodad.aDoodad[arg0] = true end)
VPlugins.RegisterEvent("DOODAD_LEAVE_SCENE", function() _VDoodad.aDoodad[arg0] = nil end)

VPlugins.OnFrameBreathe = function()	
	-- run breathe calls
	local nFrame = GetLogicFrameCount()
	for k, v in pairs(VPlugins.tBreatheCall) do
		if nFrame >= v.nNext then
			v.nNext = nFrame + v.nFrame
			local res, err = pcall(v.fnAction)
			if not res then
				OutputMessage("MSG_SYS","BreatheCall error..... \n");
			end
		end
	end
	-- run delay calls
	local nTime = GetTime()
	for k = #_VPlugins.tDelayCall, 1, -1 do
		local v = _VPlugins.tDelayCall[k]
		if v.nTime <= nTime then
			local res, err = pcall(v.fnAction)
			if not res then
				OutputMessage("MSG_SYS","DelayCall error..... \n");
			end
			table.remove(_VPlugins.tDelayCall, k)
		end
	end
end