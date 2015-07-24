UI = {};

-- Interface
UI.SetCheck = function(szControl, bChecked)
	if (szControl:GetType() == "WndCheckBox") then
		szControl:Check(bChecked == true)
	end	
end

-- Message, event, not
UI.SetValue = function(szControl, bvar, bValue)	
	if (szControl:GetType() == "WndCheckBox") then	
		szControl.OnCheckBoxCheck = function()
			OutputMessage("MSG_SYS","Repair Checkbox Checked\n");
			bvar = bValue
		end		
	end	
end