VAutoRepair = 
{
	bIsRepair 		= true,
	bIsAutoSell		= true,
	bIsGrey			= true,
	bIsWhite		= true,
	bIsGreen 		= false,
	bIsBlue			= false,
	bIsBook			= false,
	bIsMaterial		= false,
};

-- *********************************************************************************
-- Interface --
VAutoRepair.OpenDlg = function()
	local _szIniFile = "interface\\VPlugins\\ui\\autorepair.ini"
	local _frame = Station.Lookup("Normal/autorepair") or Wnd.OpenWindow(_szIniFile, "autorepair")
	_frame:Show();	
	_frame:SetPoint("CENTER", 0, 0, "CENTER", 0, 0);
	_frame:BringToTop();
	
	local _container = Station.Lookup("Normal/autorepair/Window_Main");
	if not _container then		return	end	 
	
	local _btnClose = _container:Lookup("Button_Close");	
	if not _btnClose then 		return 	end
		
	local _chkRepairAll = _container:Lookup("CheckBox_RepairAll");
	if not _chkRepairAll then	return  end
	
	local _chkAutoSell = _container:Lookup("CheckBox_SellAll");
	if not _chkAutoSell then 	return 	end
	
	local _chkGrey = _container:Lookup("CheckBox_Grey");
	if not _chkGrey 	then	return	end
	
	local _chkWhite = _container:Lookup("CheckBox_White");
	if not _chkWhite 	then	return	end
	
	local _chkBlue = _container:Lookup("CheckBox_Blue");
	if not _chkBlue 	then	return	end
	
	local _chkGreen = _container:Lookup("CheckBox_Green");
	if not _chkGreen 	then	return	end
	
	local _chkBook = _container:Lookup("CheckBox_Book");
	if not _chkBook 	then	return	end
	
	local _chkMaterial = _container:Lookup("CheckBox_Material");
	if not _chkMaterial 	then	return	end
	
	--  Interface 
	UI.SetCheck(_chkRepairAll	, VAutoRepair.bIsRepair);		
	UI.SetCheck(_chkAutoSell	, VAutoRepair.bIsAutoSell);		
	UI.SetCheck(_chkGrey		, VAutoRepair.bIsGrey);	
	UI.SetCheck(_chkWhite		, VAutoRepair.bIsWhite);	
	UI.SetCheck(_chkGreen		, VAutoRepair.bIsGreen);
	UI.SetCheck(_chkBlue		, VAutoRepair.bIsBlue);	
	UI.SetCheck(_chkBook		, VAutoRepair.bIsBook);
	UI.SetCheck(_chkMaterial	, VAutoRepair.bIsMaterial);			
		
	-- sự kiện check vào checkbox	
	_chkRepairAll.OnCheckBoxCheck 	= function() 		VAutoRepair.bIsRepair = true;	 	end			
	_chkRepairAll.OnCheckBoxUncheck = function() 	 	VAutoRepair.bIsRepair = false;		end	
	
	_chkAutoSell.OnCheckBoxCheck 	= function() 	
		VAutoRepair.bIsAutoSell 	= true; 	
		VAutoRepair.bIsGrey 		= true; 	
		VAutoRepair.bIsWhite 		= true; 			
		UI.SetCheck(_chkGrey	, VAutoRepair.bIsGrey);	
		UI.SetCheck(_chkWhite	, VAutoRepair.bIsWhite);	
	end			
	
	_chkAutoSell.OnCheckBoxUncheck 	= function() 	
		VAutoRepair.bIsAutoSell 	= false;
		VAutoRepair.bIsGrey 		= false; 	
		VAutoRepair.bIsWhite	 	= false; 			
		VAutoRepair.bIsGreen	 	= false; 	
		VAutoRepair.bIsBlue 		= false; 			
		VAutoRepair.bIsBook			= false; 	
		VAutoRepair.bIsMaterial		= false; 		
		UI.SetCheck(_chkGrey	, VAutoRepair.bIsGrey);	
		UI.SetCheck(_chkWhite	, VAutoRepair.bIsWhite);		
		UI.SetCheck(_chkGreen 	, VAutoRepair.bIsGreen);	
		UI.SetCheck(_chkBlue	, VAutoRepair.bIsBlue);
		UI.SetCheck(_chkBook 	, VAutoRepair.bIsBook);	
		UI.SetCheck(_chkMaterial, VAutoRepair.bIsMaterial);
	end	
	
	_chkGrey.OnCheckBoxCheck 		= function() 	VAutoRepair.bIsGrey = true;		end			
	_chkGrey.OnCheckBoxUncheck 		= function() 	VAutoRepair.bIsGrey = false;	end	
	
	_chkWhite.OnCheckBoxCheck 		= function() 	VAutoRepair.bIsWhite = true;	end			
	_chkWhite.OnCheckBoxUncheck 	= function() 	VAutoRepair.bIsWhite = false;	end		
	
	_chkGreen.OnCheckBoxCheck 		= function() 	VAutoRepair.bIsGreen = true;	end			
	_chkGreen.OnCheckBoxUncheck 	= function() 	VAutoRepair.bIsGreen = false;	end		
	
	_chkBlue.OnCheckBoxCheck 		= function() 	VAutoRepair.bIsBlue = true;		end			
	_chkBlue.OnCheckBoxUncheck 		= function() 	VAutoRepair.bIsBlue = false;	end
	
	_chkBook.OnCheckBoxCheck 		= function() 	VAutoRepair.bIsBook = true;		end			
	_chkBook.OnCheckBoxUncheck 		= function() 	VAutoRepair.bIsBook = false;	end
		
	_chkMaterial.OnCheckBoxCheck 		= function() 	VAutoRepair.bIsMaterial = true;		end			
	_chkMaterial.OnCheckBoxUncheck 		= function() 	VAutoRepair.bIsMaterial = false;	end
		
	-- Close
	_btnClose.OnLButtonClick = function()		
		-- OutputMessage("MSG_SYS","Đóng cửa sổ thiết lập sửa trang bị, bán vật phẩm\n");
		Wnd.CloseWindow(_frame);				
	end		
end

-- ***********************************************************************************

VAutoRepair.OnSell = function(nShopID, nShopType, nValidPageCount, nCanRepair, nNpcID)
	local player = GetClientPlayer()
	if not player then return end  		
	
	
	if (VAutoRepair.bIsGrey == false and VAutoRepair.bIsWhite == false and 
		VAutoRepair.bIsGreen == false and VAutoRepair.bIsBlue == false and
		VAutoRepair.bIsBook == false and VAutoRepair.bIsMaterial == false ) then return end
	
	local i,j;
	for i = 1, 5 do
		for j = 0, player.GetBoxSize(i) - 1 do
			local item = player.GetItem(i,j)	
				
			if item and item.bCanTrade then				
				
				local bSell = false;
				
				
				if VAutoRepair.bIsMaterial then
					if (item.nGenre == ITEM_GENRE.MATERIAL) then bSell = true end
				end
				
				if VAutoRepair.bIsGrey then					
					if (item.nQuality == 0) then bSell = true end
				end
												
				if (item.nGenre ~= ITEM_GENRE.BOOK 		and item.nGenre ~= ITEM_GENRE.MATERIAL 	and 
					item.nGenre ~= ITEM_GENRE.TASK_ITEM and item.nGenre ~= ITEM_GENRE.POTION 	and 
					item.nGenre ~= ITEM_GENRE.DIAMOND 	and item.nGenre ~= ITEM_GENRE.COLOR_DIAMOND and
					item.nGenre ~= ITEM_GENRE.BOX_KEY 	and item.nGenre ~= ITEM_GENRE.BOX) then
						
					if VAutoRepair.bIsWhite then
						if (item.nQuality == 1) then bSell = true end
					end
					
					if VAutoRepair.bIsGreen then
						if (item.nQuality == 2) then bSell = true end				
					end
					
					if VAutoRepair.bIsBlue then
						if (item.nQuality == 3) then bSell = true end				
					end
				end
				
				if VAutoRepair.bIsBook then
					if (item.nGenre == ITEM_GENRE.BOOK) then				
						if item.nBookID == nil then
							if IsBookMemorized(GlobelRecipeID2BookID(item.nBookID)) then
								bSell = true;						
							end
						end
					end
				end
				
				if (bSell == true) then
					local nCount = 1
					if item.nGenre == ITEM_GENRE.EQUIPMENT and item.nSub == EQUIPMENT_SUB.ARROW then
						nCount = item.nCurrentDurability
					elseif item.bCanStack then
						nCount = item.nStackNum
					end
					SellItem(nNpcID, nShopID, i, j, nCount)
					OutputMessage("MSG_SYS", "Bán [" .. item.szName.. "] x " .. nCount .. "\n");	
				end	
							
			end
		end
	end		
			
	if VAutoRepair.bIsRepair == true then	
		if GetRepairAllItemsPrice(nNpcID, nShopID) > 0 then
			RepairAllItems(nNpcID, nShopID)
			OutputMessage("MSG_SYS",VAutoRepair.szMessage[VAutoRepair.REPAIR]);
		end	
	end
end

-- ***********************************************************************************
VAutoRepair.OpenShop = function()	
	OutputMessage("MSG_SYS","kích hoạt mở shop\n");
	VAutoRepair.OnSell(arg0, arg1, arg2, arg3, arg4)
end

RegisterEvent("SHOP_OPENSHOP", VAutoRepair.OpenShop);	
