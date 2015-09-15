#pragma once
/*
//20140610_SangDH-Update Window Version:
	+ update function DetectOS()
	+ define window version
	+ Thêm hàm GetBaseAddress(): lấy base address của game khi game login
//20140328_SanghDH-Update game vn2.25 (auto v1.4)
//20140321_SangDH-Update game vn2.24 (auto v1.3)
*/

///Custom Message
#define MSG_MOVE													WM_USER+ 0x1
#define MSG_FIGHT													WM_USER+0x2
#define MSG_RIDE													WM_USER+0x3
#define MSG_SKILL													WM_USER+0x4
#define MSG_RIGHT_SKILL												WM_USER+0x5
#define MSG_RECORVER												WM_USER+0x6
#define MSG_KEYDOWN													WM_USER+0x7
#define MSG_CHAT													WM_USER+0x8
#define MSG_BACK_CITY												WM_USER+0x9
#define MSG_QUIT													WM_USER+0x10
#define MSG_CALLBACK												WM_USER+0x11
#define MSG_AUTOFIGHT												WM_USER+0x12
#define MSG_INJECT													WM_USER+0x13
#define MSG_MOVE_AWAY												WM_USER+0x14
#define MSG_MENU_CLICK												WM_USER+0x15
#define MSG_QUEST_ITEM_CLICK										WM_USER+0x16
#define MSG_QUEST_LINK_CLICK										WM_USER+0x17
#define MSG_AWARD_CLICK												WM_USER+0x18
#define MSG_OPEN_MENU												WM_USER+0x19
#define MSG_ITEM_CLICK												WM_USER+0x20

//=====NghiaLP-20141703========
#define MSG_CHECK_AUTO_PARTY										WM_USER+0x21
#define MSG_AGREE_PARTY												WM_USER+0x22
#define MSG_INVITE_LIST 											WM_USER+0x23
#define MSG_CHECK_AUTO_PARTY										WM_USER+0x24
//=====Van Tieu===============
#define MSG_CHOOSE_ADWARD											WM_USER+0x25
//========Dieu Doi==================
#define MSG_OPEN_QUEST												WM_USER+0x26
#define MSG_CHOOSE_QUEST											WM_USER+0x27
#define MSG_TEXT_QUEST												WM_USER+0x28

#define MSG_CHECK_FINISH											WM_USER+0x29
//#define MSG_INTER_PARTY											WM_USER+0x14
//#define MSG_CREATE_PARTY											WM_USER+0x15
//#define MSG_UnCheck_PARTY											WM_USER+0x17
//#define MSG_INVITE_LIST 											WM_USER+0x18
//=====Ket Thuc================



///Game Engine Info
#define EN_MAX_ACC													10
#define EN_PROC_NAME												"9k.exe"
#define EN_TITLE													L"Auto_9K"
#define NPC_MAX_SIZE												255				// độ lớn của mảng NPC trong game
#define NPC_NAME_SIZE												48//32
#define PARTY_SIZE													6				//Danh sách tổ đội
#define SIZE_TEXT													255

//Game engine Address
#define GAME_UPDATE													0x2338//V1.6.0_0x9f8

//
////21040610_Update new base address
//#define GAME_BASE_ADDRESS											0x7EFDF010//				Base address win64
//#define GAME_BASE_ADDRESS_32										0X7FFAE030//0X7FFAE030//0x7FFD3030//				Base address win32
//#define GAME_BASE_ADDRESS_XP_32										0X7FFAE030//0x7FFAF030//				Base address winXp32
//





#define GAME_OFFSET_32												0x00000008//				offset start win32
#define GAME_OFFSET_1												0x0000ecd8//0x0000EE08//				(offset start)
#define GAME_OFFSET_INDEX											0x0016dca0//0x00166D90 + GAME_UPDATE//v1.5.5_0x00166D90//v1.5_0x00166CCC//v1.4_0x00166CBC//v1.3_0x00166B3C//v1.2_0x00166AEC//0x00165AEC//0x00165ABC//0x00165A9C		(offset index)					
#define GAME_OFFSET_NAME											0x0016DD78//0x00166E6C + GAME_UPDATE//V1.5.5_0x00166E68//V1.5_0x00166DA4//V1.4_0x00166D94//V1.3_0x00166C14//v1.2_0x00166BC4//0x00165BC4//0x00165B94//				(offset tên cố định trong game)
#define GAME_OFFSET_PARTY											0x00000000//0x00166DF8 + GAME_UPDATE//V1.5.5_0x00166DF4//V1.5_0x00166D30//V1.4_0x00166D20//V1.3_0x00166BA0//v1.2_0x00166B50//		(Get party list)
#define GAME_OFFSET_INFO											0x0016dd10//0x00166E04 + GAME_UPDATE//V1.5.5_0x00166E00//V1.5_0x00166D3C//V1.4_0x00166D2C//V1.3_0x00166BAC//v1.2_0x00166B5C//0x00165B5C//0x00165B2C//0x00165B0C		(offset info)
#define GAME_OFFSET_MAPID											0x0016dcb4//0x00166DA4 + GAME_UPDATE//V1.5.5_0x00166DA4//V1.5_0x00166CE0//V1.4_0x00166CD0//V1.3_0x00166B50//v1.2_0x00166B00//0x00165B00//0x00165AD0//0x00165AB0		(offset map pos)
#define GAME_SERVER_OFFSET											0x0004E8D4


#define GAME_OFFSET_MAP_X											0x0016dcf8//0x015a90c8//V1.8.1_0x015a90A0//V1.8.0_0x015a9070//V1.6.0_0x0158c710 + GAME_UPDATE//V1.5.5_0x0158c710//V1.5_0x0158c648//V1.4_0x0158c638//V1.3_0x0158C4A0//v1.2_0x0158C448//0x0158b448//0x0158b418//				(offset map star X pos)
#define GAME_OFFSET_MAP_Y											0x0016dcfc//0x015a90cc//V1.8.1_0x015a90A4//V1.8.0_0x015a9074//V1.6.0_0x0158c714 + GAME_UPDATE//V1.5.5_0x0158c714	//V1.5_0x0158c64c	//V1.4_0x0158c63c	//V1.3_0x0158C4A4	//v1.2_0x0158C44C	//0x0158b44C	//	0x0158b41C	//				(offset map star Y pos)
#define GAME_OFFSET_DATASIZE										0x000012b4//0x000012A8//V1.0.8.1_0x00001290//V1.6.0_0x0000127C//				(offset datasize)
#define GAME_OFFSET_MOUSE_X											0x0016D7E8//0x001668D8 + GAME_UPDATE//V1.5.5_0x001668D8//V1.5_0x00166814//V1.4_0x00166804//V1.3_0x00166684//v1.2_0x00166634//0x00165634//0x00165604//				(tọa độ x khi click chuột)
#define GAME_OFFSET_MOUSE_Y											0x0016D7EC//0x001668DC + GAME_UPDATE//V1.5.5_0x001668DC//V1.5_0x00166818//V1.4_0x00166808//V1.3_0x00166688//v1.2_0x00166638//0x00165638//0x00165608//				(tọa độ y khi click chuột)
//Skill offset
#define GAME_OFFSET_SKILL											0x0016e194//0x0016727C + GAME_UPDATE//V1.5.5_0x0016727C//V1.5_0x001671B4//V1.4_0x001671A4//V1.3_0x00167024//v1.2_0x00166FD4//0x00165FD4//0x00165FA4//				(offset ô skill)
#define GAME_OFFSET_SKILL_1											0x00000324//n				
#define GAME_OFFSET_SKILL_2											0x00000328//n				
#define GAME_OFFSET_SKILL_3											0x000003E4//n				
#define GAME_OFFSET_SKILL_4											0x00000004//n
#define GAME_OFFSET_SKILL_5											0x00000008//n
#define GAME_OFFSET_SKILL_6											0x00000014//n
#define GAME_OFFSET_SKILL_7											0x00000138//n				//right skill Delay
#define GAME_OFFSET_SKILL_8											0x000001D8//n				//left skill Delay (right - left)

#define GAME_LOGIN													0x00169CBC+ GAME_UPDATE//				//Game login (1) | logout (0)
#define NPC_STATEFIGHT												0x00165238					//1: tu dong chien dau, 0: ko hien dau


#define NPC_MAP														0x00000018
//NPC address offset
#define NPC_INDEX													0x0000602C//V1.5.5_0x0000602C//V1.5_0x000059D4//V1.3_0x000058F4//V1.2_0x000058E4
#define NPC_NAME													0x0004e90c//0x0004EF88//				(tên không thay đổi)

#define NPC_MAX_HP													0x00000a8c//a68//0x00000A5C
#define NPC_CURRENT_HP												0x00000a90//a6c//0x00000A60
#define NPC_MAX_MP													0x00000abc//a98//0x00000A8C
#define NPC_CURRENT_MP												0x00000ac0//a9c//0x00000A90
#define NPC_LEVEL													0x00000c30//c0c//0x00000C00
#define NPC_TYPE													0x00000c34//c10//0x00000C04//0: Quái | 1: Player | 3: NPC | 5:vòng xoáy
#define NPC_SEX														0x00000cc8//ca0//0x00000C94
#define NPC_STATE													0x00000ce4//cb8//0x00000CAC //1: Stand, 2: Run, 3: Sit, 5:Kinh công 7: Attack, 15:Die
#define NPC_NAME_COLOR												0x00000//CB0//0x00000CA4 //
#define NPC_ACTIVE_SKILL											0x00000cec//cec//0x00000CB4	//0x00000D0C	//
#define NPC_PK_TARGET												0x00000d1c//e68//cf0//0x00000CE4	//Index đối tượng đang bị đánh
#define NPC_TARGET													0x00000d34//d08//0x00000CFC
#define NPC_SAFE													0x00000d2c//d00//0x00000CF4//0: an toàn | 1: chiến đấu
#define NPC_PARTY													0x00000E04//dd8//0x00000DCC//0: khác party | 1: cung party
#define NPC_ID														0x00000///DE8//0x00000DD8//ID npc
#define NPC_FACTION													0x00000cdc//dec//0x00000DDC  //1:Thien Vuong | 2:Thieu Lam | 3:Tieu Giao | 4:Hoa Son | 5:Con Luan | 6:Vo Dang | 7:Dao Hoa Dao | 8:Nga My | 9: Cai Bang | 10:Duong Mon
#define NPC_FAMILY													0x00000e28//e00//0x00000DF0 //V1.6.0_0x00000DFC // 
#define NPC_PK_STATE												0x00000e68//e40//0x00000E30  //0: luyện công | 1: Trận doanh | 2: Gia tộc, bang | 3: Đồ sát
#define NPC_TITLE													0x00000ea0//e8C//0x00000E7C//V1.6.0_0x00000E68  //danh hiệu của người chơi, thay đổi khi vào chiến trường
#define NPC_RIDE_HORSE												0x00000f40//f2c//0x00000F1C//V1.6.0_0x00000F08
#define NPC_COOR_X													0x00000f6c//F60//0x00000F6C//V1.6.0_0x00000F58
#define NPC_COOR_Y													0x00000f70//F64//0x00000F70//V1.6.0_0x00000F5C
#define NPC_ELEMENT													0x00000fc0//fd4//0x00000FD0 // 1: Metal, 2: Wood, 3: Water, 4: Fire, 5: Earth
#define NPC_ELEMENT_2												0x00001088//105c//0x0000104C // 1: Metal, 2: Wood, 3: Water, 4: Fire, 5: Earth
#define NPC_NAMESTATE												0x00000000//c1c//0x00000C10 //Bang


//Party list
#define PARTY_CAPTAIN_ID											0x00000004 // party leader

#define PARTY_DATA_SIZE												0x0000003D // party block
#define PARTY_MEM_ID												0x00000008 // ID
#define PARTY_MEM_NAME												0x0000000E // Name
#define PARTY_MEM_MAP												0x00000032 //0x00000032 // Map
#define PARTY_MEM_X													0x0000003D // X
#define PARTY_MEM_Y													0x00000041 // Y
#define OFFSET_FUNTION_INVITE										0x00017540 //v1.0.8.1_0x00017980	//v1.0.8.0_0x00017A90 //v1.6.0.1_0x00017470  
#define PARTY_CHECK_BASE_ADDRESS									0x001706f8//0x00169744 + GAME_UPDATE //V.1.0.8.0_0x00169714 + GAME_UPDATE


//Follow quest
#define QUEST_BASE_ADDRESS											0x0016e194//0x0016727C + GAME_UPDATE //v1.5.5_0x0016727C //v1.5_0x001671B4 //v1.4_0x001671A4 //v1.3_0x00167024 // Base address của bảng theo dõi nhiệm vụ
//20140512_Thay đổi đại chỉ block text
//#define QUEST_DATA_SIZE												V1.6.0.1_0x000003C8 // data block size
#define QUEST_TEXT_OFFSET											0x00000398
//win xp64
//20140512_Thay đổi đại chỉ block text (dùng cho xp và win7)
//#define QUEST_TEXT_0												0x00001C88 // data block text 1
//#define QUEST_TEXT_1												0x000003C8 // data block text 2 và block 5 của win khác
//#define QUEST_TEXT_2												0x00006670 // data block text 3
//#define QUEST_TEXT_3												0x00007F50 // data block text 4
//#define QUEST_TEXT_4												0x00009818 // data block text 5


//QUEST AWARD
#define QUEST_AWARD_BASE_ADDRESS									0x00167F48 + GAME_UPDATE //v1.5.5_0x00167F48 //v1.5_0x00167E80 //v1.4_0x00167E70 //v1.3_0x00167CF0 // Base address của bảng nhận thưởng
#define QUEST_AWARD_OPENED											0x0016EE74//0x00167F5C  + GAME_UPDATE//v1.5.5_0x00167F5C //v1.5_0x00167E94 //v1.4_0x00167E84 //v1.3_0x00167D04 // Menu chọn phần thưởng opened (0: closing,1 opening);
#define QUEST_AWARD_SIZE											0x00000488 //data size of menu award
#define QUEST_AWARD_TEXT_SIZE										0x00000048	//data size of text award
//Winxp64
#define QUEST_AWARD_TEXT_SIZE_XP									0x00000028	//data size of text award
#define QUEST_AWARD_TEXT_1											0x000014F8	//data size of text award
#define QUEST_AWARD_TEXT_2											0x00002698	//data size of text award
#define QUEST_AWARD_TEXT_3											0x000037A8	//data size of text award




#define QUEST_LINK_FUNCTION											0x0012A82C//V1.0.8.1_0x0012A82C//V1.0.8.0_0x0012A7BC	//V1.6.0_0x00129794	//V1.5.5_0x00129794	//V1.5_0x00129784	//data size of text award
#define QUEST_LINK_FUNCTION1										0x00057630//V1.0.8.1_0x000576A0  //V1.0.8.0_0x00057A30//V1.6.0.2_0x000579D0//V1.6.0.1_0x00057B90//V1.6.0_0x00057310//V1.5.5.5_0x000573C0//V1.5.5_0x000573C0//V1.5_0x00056FC0//V1.4_0x00057030	//data size of text award





//Function address
#define CENTER_X_OFFSET												0x001DE764//0x001D78AC + GAME_UPDATE//V1.5.5_0x001d7824//V1.5_0x001d775C//V1.4_0x001d774C//V1.3_0x001d75B4//v1.2_0x001d7564//0x001d6564//0x001d6534//0x001d6514
#define CENTER_Y_OFFSET												0x001DE768//0x001D78B0 + GAME_UPDATE//V1.5.5_0x001d7828//V1.5_0x001d7760//V1.4_0x001d7750//V1.3_0x001d75B8//v1.2_0x001d7568//0x001d6568//0x001d6538//0x001d6518
//#define FUNC_MOVE_OFFSET											0x0006CEE0//V1.5_0x0006CAF0//V1.4_0x0006CB80//V1.3_0x0006D080//v1.2_0x0006DA10//0x0006D210//0x0006CC80//0x0006D520
#define FUNC_MOVE_OFFSET											0x0016dca0//0x00166D90 + GAME_UPDATE //20140418-Thay ham di chuyen moi
#define FUNC_MOVE_FUNC												0x0008DEA0//0x0008D1E0//v1.0.8.1_0x0008C5C0 //v1.0.8.0_0x0008D020 //V1.6.0.2_0x0008D1C0 //V1.6.0.1_0x0008D2A0 //V1.6.0_0x0008C540 //V1.5.5_0x0008C650 //20140418-Thay ham di chuyen moi

#define FUNC_RIGHT_CLICK											0x000E3590//0x000E4FF0//0x000E2F40//v1.0.8.1_0x000E32F0//v1.0.8.0_0x000E3C10//V1.6.0.2_0x000E3260//V1.6.0.1_0x000E3220//V1.6.0_0x000E2E70//V1.5.5_0x000E2FC0//V1.5_0x000E2EA0//V1.4_0x000E2E70//V1.3_0x000E2380//v1.2_0x000E2900//0x000E2450//0x000E2470//0x000E24E0 //Hàm 1 khi right click
#define FUNC_RIGHT_CLICK_1											0x000AA4E0//0x000A9300///v1.0.8.1_0x000A8E30//v1.0.8.0_0x000AA050//V1.6.0.2_0x000A9650//V1.6.0.1_0x000A96F0//V1.6.0_0x000A8D70//V1.5.5_0x000A8DA0//V1.5_0x000A8D10//V1.5_0x000A8B20//V1.4_0x000A8B40//V1.3_0x000A8310//v1.2_0x000A9160 //0x000A83C0 //0x000A8580 //0x000A8E70 //Hàm 2 gọi khi right click
#define GAME_CLICK_FUNC												0x00002E60//2CD0//v1.0.8.0_0x00002CE0//V1.6.2_0x00002D00//V1.6.0_0x00002CC0//V1.5_0x00002CD0//V1.3_0x00002D10//v1.2_0x00002CC0//0x00002CE0//0x00002CF0//Menu Back to City function address


//Button Offset
#define GAME_BUTTON_BASE_ADDRESS									0x0016e194//0x0016727C + GAME_UPDATE//V1.5.5_0x0016727C//V1.5_0x001671B4//V1.4_0x001671A4//V1.3_0x00167024//v1.2_0x00166FD4//0x00165FD4//0x001655c4//Menu Back to City base address
#define GAME_MAP_BASE_ADDRESS										0x001706f8//0x001706f8//0x00169744 + GAME_UPDATE //V.1.0.8.0_0x00169714 + GAME_UPDATE0x00169714 + GAME_UPDATE//V1.5.5_0x00169714//V1.5_0x0016964C//V1.4_0x0016963C//V1.3_0x001694A4//v1.2_0x00169454//0x00165FD4//0x001655c4//Map base address
//#define GAME_RIDE_HOURSE_BUTTON										0x0016727C + GAME_UPDATE



//MENU
#define MENU_OPENED													0x00162374//	0x0015E384//V1.0.8.10x0015E374 // V1.6.0_0x0015D374 // V1.5.5_0x0015D374 // V1.5_0x0015D364 // V1.3_0x0015D34C // Menu opened (0: closing,1 opening);
#define MENU_BASE_ADDRESS											0x001706f8//0x00167F3C + GAME_UPDATE//V1.5.5_0x00167F3C//V1.5_0x00167E74//V1.4_0x00167E64//V1.3_0x00167CE4 //Menu_base
#define MENU_DATA_SIZE												0x00000448//



#define GAME_MENU_TOTAL_LINE										0x000003B8//
#define GAME_MENU_OFFSET_1											0x00000324//n				
#define GAME_MENU_OFFSET_2											0x00000328//n	
#define GAME_MAP_CITY_1												0x00000890//n	Map city offset
#define GAME_MAP_CITY_2												0x00000448//n	Map city offset
//#define GAME_MENU_OFFSET											0x0000032C//Menu button offset

//#define GAME_MENU_FUNC												0x000348D9//0x00034309//Menu jump address

//========ITEM==============

#define ITEM_OFFSET_ADDRESS											0x00166654+ GAME_UPDATE//V1.6.1_0x00A13CFC
#define ITEM_OFFSET_ADDRESS_1										0x00000004//V1.6.1_0x00044FC0
#define ITEM_NAME_OFFSET											0x000004D4
#define ITEM_DATA_SIZE												0x000002D8

//Get Item index in package's cell
#define PACKAGE_CELL_ADDRESS										0x0016dca0//0x00166D90 + GAME_UPDATE
#define PACKAGE_OFFSET_1											0x00005D2C
#define PACKAGE_OFFSET_2											0x0000003C
#define PACKAGE_OFFSET_3											0x00000084

//Get package base address
#define PACKAGE_BASE_ADDRESS										0x001706f8//0x00169744 + GAME_UPDATE //V.1.0.8.0_0x00169714 + GAME_UPDATE//0x00169714 + GAME_UPDATE




//==============NghiaLP=========
#define OFFSET_PICK_BAG												0x001655E8//0x001614F8//V1.0.6.0_0x001604F8//V1.0.5.5_0x001605BC//V1.0.5_0x001604F8//V1.0.4_0x001604E8
#define OFSET_OPEN_DIALOG_PARTY										0x00169F44//0x00167B44 + GAME_UPDATE//mở bảng mời tổ đội
#define GAME_CHECK_PARTY											0x001706f8//0x00169744 + GAME_UPDATE //V.1.0.8.0_0x00169714 + GAME_UPDATE

#define OFFSET_NAME_PARTY											0x000003F8
#define OPEN_DIALOG_PARTY											0x0000000F

//#define OFFSET_CHOSE_MEMBEM											0x00169648
#define OFFSET_SAVE_NAME											0x000000F0   //Dia chi bat ki save ten
//=============Dieu Doi============
#define OFFSET_BUTTON_ACCEPT										0x0016A280
#define	OFFSET_GET_TEXT												0x00000498//0x00005108
#define	OFFSET_GET_TEXT_2											0x00001DA8
///=============ket thuc=============

///=============ket thuc=============



//Not change

//#define PLAYER_ITEM_TARGET											0x00001418
//
//#define ITEM_BASE_ADDRESS											0x013B8A08
//#define ITEM_DATA_SIZE												0x000003D4
//#define ITEM_NAME													0x00000060
//#define ITEM_COOR_X													0x00000294
//#define ITEM_COOR_Y													0x00000298
//#define ITEM_MAX_SIZE												255				// độ lớn của mảng ITEM trong game
//#define ITEM_NAME_SIZE												32
//
//
//#define KEY_SHORTCUT_BASE											0x006394FC
//#define KEY_SHORTCUT_OFSET											0x00000FF8
//#define KEY_SHORTCUT_SIZE											0x00000164
//
//#define FUNC_FIGHT													0x0058c7f0
//
//class player{
//public:
//	char	name[32];
//};
//
//
//
//
//class npc{
//public:
//	int	target;
//};


///Window version

#define WIN_XP_32				5132
#define WIN_XP_64				5164
#define WIN_7_32				6132
#define WIN_7_64				6164

class sys{
public:
	static BOOL Is64BitOS()
	{
	   BOOL bIs64BitOS = FALSE;

	   // We check if the OS is 64 Bit
	   typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL); 

	   LPFN_ISWOW64PROCESS
		  fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress (
		  GetModuleHandleA((LPCSTR)"kernel32"),"IsWow64Process");
	 
	   if (NULL != fnIsWow64Process)
	   {
		  if (!fnIsWow64Process(GetCurrentProcess(),&bIs64BitOS))
		  {
			 //error
		  }
	   }
	   return bIs64BitOS;
	}
	static int DetectOS(){
		OSVERSIONINFO OS;
		byte iBit = 32;
		OS.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		::GetVersionEx(&OS);
		if(Is64BitOS())
			iBit= 64;

		CString Version;
		Version.Format(L"%d%d%d", OS.dwMajorVersion , OS.dwMinorVersion,iBit );
//		return _ttoi(Version);
		int ver = _ttoi(Version);
		if(ver == 5264)
			ver = 5164;
		return ver;
	}
	static bool SeDebugPrivileges (){
		bool result = false;
		HANDLE hToken = NULL;
	   TOKEN_PRIVILEGES tokenPriv;
	   LUID luidDebug;
	   if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) != FALSE) 
	   {
		  if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidDebug) != FALSE)
		  {
			 tokenPriv.PrivilegeCount           = 1;
			 tokenPriv.Privileges[0].Luid       = luidDebug;
			 tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			 if(AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, 0, NULL, NULL) != FALSE)
			 {
				// Always successful, even in the cases which lead to OpenProcess failure
				//cout << "SUCCESSFULLY CHANGED TOKEN PRIVILEGES" << endl;
				result = true;
			 }
		  }
	   }
	   CloseHandle(hToken);
	   return result;
	}

	static DWORD GetBaseAddress (HANDLE m_hHandle,CString &sName){
		DWORD BASEADDRESS[] =		{
				0x7EFDF010,
				0X7FFAE030,
				0x7FFD3030,
				0x7FFAF030
			};

		DWORD dwGameBaseAddress;
		int m_iOS = DetectOS();

		//Dò từng base address đọc thông tin
		for(int i=0;i<4;i++){

			dwGameBaseAddress = BASEADDRESS[i];
			ReadProcessMemory(m_hHandle, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
			//ReadProcessMemory(m_hHandle, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
			//Get Base Address win32
			if(m_iOS==WIN_XP_32||m_iOS==WIN_7_32){
				dwGameBaseAddress = dwGameBaseAddress + GAME_OFFSET_32;
				ReadProcessMemory(m_hHandle, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);
			}
			dwGameBaseAddress = dwGameBaseAddress + GAME_OFFSET_1;
			ReadProcessMemory(m_hHandle, (void*)dwGameBaseAddress, &dwGameBaseAddress, (DWORD)sizeof(dwGameBaseAddress), 0);

//16BFB4: base login | logout
			bool bLogin;
			DWORD dwPlayerInfo = dwGameBaseAddress + GAME_LOGIN;	
			ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &bLogin, (DWORD)sizeof(bLogin), 0);
			/*if(!bLogin)
				return NULL;*/
			


			dwPlayerInfo = dwGameBaseAddress + GAME_OFFSET_NAME;	
			ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &dwPlayerInfo, (DWORD)sizeof(dwPlayerInfo), 0);
			dwPlayerInfo = dwPlayerInfo + NPC_NAME;
			
			char sValue[NPC_NAME_SIZE];
			memset(sValue, 0x0, sizeof(sValue));
			ReadProcessMemory(m_hHandle, (void*)dwPlayerInfo, &sValue, (DWORD)sizeof(sValue), 0);
			sName = (CString)sValue;
			if(sName.GetLength()>0)
				return dwGameBaseAddress;


		}
		return NULL;
	}
};




//
//
//enum SENDCMD
//{
//	cmd_none,
//	cmd_autohp,		// Tu dong bom Mau
//	cmd_automp,		// Tu dong bom Mana
//	cmd_autoam,		// Tu dong TDP
//	cmd_autohit,	// Tu dong danh quai
//	cmd_hitlimit,	// Danh quanh diem	
//	cmd_hitclose,	// Danh tiep can
//	cmd_minhp,		// Mau toi thieu de bom
//	cmd_minmp,		// Mana toi thieu de bom
//	cmd_minam,		// Mau toi thieu dung TDP
//	cmd_searchrange,// Gioi han tim kiem
//	cmd_hitrange,	// Gioi han danh
//	cmd_skillradius,// Ban kinh chieu thuc
//	cmd_collect,	// Che do nhat do
//	cmd_sendhwnd,	// Thong bao cua so dieu khien
//	cmd_amulet,		// Thong bao da TDP
//};