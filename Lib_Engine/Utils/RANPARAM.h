#pragma once
#include "d3dapp.h"
#include <string>

#define STRING_NUM_128	128
#define	QUICK_SLOT_NUM	6
#define	QUICK_SKILL_NUM	10
#define	QUICK_MENU_NUM	17
#define	QUICK_ALL_NUM	(QUICK_SLOT_NUM + QUICK_SKILL_NUM + QUICK_MENU_NUM)
#define CHAT_MACRO_NUM	10
#define BASICMENU_NUM	12

enum EMSERVICE_TYPE
{
	EMSERVICE_DEFAULT		= 0,
	EMSERVICE_KOREA			= 1,
	EMSERVICE_FEYA			= 2,
	EMSERVICE_MALAYSIA_CN	= 3,
	EMSERVICE_MALAYSIA_EN	= 4,
	EMSERVICE_JAPAN			= 5,
	EMSERVICE_THAILAND		= 6,
	EMSERVICE_CHINA			= 7,
	EMSERVICE_INDONESIA		= 8,
	EMSERVICE_PHILIPPINES	= 9,
	EMSERVICE_VIETNAM		= 10,
	EMSERVICE_GLOBAL		= 11,	//	旋煎弊 憮綠蝶
};

// SHOTCUT KEY
enum
{
	SHOTCUT_INVEN = 0,
	SHOTCUT_CHAR,
	SHOTCUT_SKILL,
	SHOTCUT_PARTY,
	SHOTCUT_QUEST,
	SHOTCUT_CLUB,
	SHOTCUT_FRIEND,
	SHOTCUT_MAP,
	SHOTCUT_ITEMBANK,
	SHOTCUT_RUN,
	SHOTCUT_HELP,
	SHOTCUT_PET,
	SHOTCUT_ATTACKMODE,
	SHOTCUT_PKMODE,
	SHOTCUT_ITEMSHOP,
	SHOTCUT_CHATMACRO,
	SHOTCUT_SUMMON
};

namespace RANPARAM
{
	enum
	{ 
		MAX_FTP = 24, 
		EMGAMEOPT_VER = 0x0112, 
		MAX_HTTP = 24,
		MAX_SERVER = 20,
		MAX_CHINA_REGION = 8
	};

	enum EMTEXTUREFILTER
	{
		EMTEXTUREFILTER_TRILINEAR		= 0,
		EMTEXTUREFILTER_ANISOTROPIC_2	= 1,
		EMTEXTUREFILTER_ANISOTROPIC_4	= 2,
		EMTEXTUREFILTER_ANISOTROPIC_8	= 3,
		EMTEXTUREFILTER_ANISOTROPIC_16	= 4,
		EMTEXTUREFILTER_NSIZE			= 5,
	};

	struct SERVER_SET
	{
		CString	strName;
		CString	LoginAddress;
		CString	HttpAddressTable[MAX_HTTP];
		CString	strNewsURL;
		CString	ServerName[MAX_SERVER];
		CString strItemShop;
		UINT	nLoginPort;
	};

	extern DWORD	dwPotsCount;

	extern EMSERVICE_TYPE	emSERVICE_TYPE;
	extern BOOL				bXML_USE;

	//	啪歜 撲薑.
	extern BOOL				bSAVE_USERID;

	extern BOOL				bDIS_CONFT;			//	'濠翕' 渠溼 剪睡.
	extern BOOL				bDIS_TRADE;			//	'濠翕' 剪楚 剪睡.
	extern BOOL				bDIS_PARTY;			//	'濠翕'  剪睡.
	extern BOOL				bDIS_RIDE;			//	'濠翕'  剪睡.
	extern BOOL				bDIS_FRIEND;
	extern BOOL				bSHOW_SIMPLEHP;
	extern BOOL				b3D_SOUND;			//	3離錳 餌遴萄
	extern BOOL				bSHOW_TIP;			//	 爾罹輿晦
	extern BOOL				bMOVABLE_ON_CHAT;	//	瓣璽 嬪縑憮 遺霜檜晦
	extern BOOL				bFORCED_ATTACK;		//	鬼薯 奢問
	extern BOOL				bNAME_DISPLAY;		//	檜葷 鼻 衛
	extern BOOL				bNON_Rebirth;		//	睡 蝶鑒 旎雖
	extern BOOL				bPrivateStats;		/*charinfoview , Juver, 2017/11/12 */

    extern BOOL             bCheckUsableItemMarketPage; // NPC 鼻薄 ui縑憮 餌辨 陛棟 嬴檜蠱 羹觼.
    extern BOOL             bCheckBuyableItemMarketPage; // NPC 鼻薄 ui縑憮 掘殮 陛棟 嬴檜蠱 羹觼.
    extern BOOL             bCheckRebuySuppliesRebuyPage; // NPC 營衙殮 ui縑憮 模賅 嬴檜蠱 羹觼.
    extern BOOL             bCheckAllDelItemsRebuyPage; // NPC 營衙殮 ui縑憮 蕾樓謙猿 衛 營衙殮 嬴檜蠱 檣.
    extern BOOL             bCheckSellItemRButton; // NPC 鼻薄 ui縑憮 葆辦蝶 辦贗葛戲煎 嬴檜蠱 衙 羹觼.
    extern BOOL             bCheckConfirmSellItem; // NPC 鼻薄 ui縑憮 嬴檜蠱 衙 衛 檣.

	extern BOOL				bCONFT_BOUND;		//	渠溼衛 艙羲 薯 罹睡.
	extern WORD				wCONFT_RECOVER;		//	渠溼衛 犒擒 薯.
	extern float			fCONFT_HP_RATE;		//	渠溼衛 羹溘 渠 綠徽.
	extern float			fCONFT_TAR_HP_RATE;	//	渠溼衛 羹溘 渠 綠徽 [ 鼻渠寞(偃檣渠溼衛虜) ].

	extern DWORD			dwPARTY_GET_ITEM;	//	褫暮 嬴檜蠱 碟寡 敘罌.
	extern DWORD			dwPARTY_GET_MONEY;	//	褫暮 絲 碟寡 敘罌.
	
	extern int				QuickSlot[QUICK_SLOT_NUM]; // 襬蝸煜
	extern int				SkillSlot[QUICK_SKILL_NUM]; // 襬蝶鑒
	extern int				MenuShotcut[QUICK_MENU_NUM]; // 詭景欽蹴酈
	extern int				DefaultShotcutKey[QUICK_ALL_NUM]; // 蛤 酈 顫殮
	extern int				KeySettingToRanparam[QUICK_MENU_NUM];
	extern int				BasicMenuToRanparam[BASICMENU_NUM];

	extern CString			ChatMacro[CHAT_MACRO_NUM];

	//	蛤蝶溯檜 撲薑.
	extern BOOL				bCHECKWHQL;
	extern DWORD			dwScrWidth;
	extern DWORD			dwScrHeight;
	extern EMSCREEN_FORMAT	emScrFormat;
	extern UINT				uScrRefreshHz;
	extern BOOL				bScrWindowed;
	extern BOOL				bScrWndHalfSize;
	extern BOOL				bScrWndFullSize;
	extern BOOL				bGameCursor;

	extern bool				bDISP_CONSOLE;
	extern bool				bDISP_FPS;

	//	斜楚 撲薑.
	extern DWORD			dwVideoLevel;
	extern DWORD			dwShadowChar;	//	0~4
	extern BOOL				bBuff;
	extern BOOL				bShadowLand;
	extern BOOL				bRealReflect;
	extern BOOL				bRefract;
	extern BOOL				bGlow;
	extern BOOL				bPost;
	extern BOOL				bFrameLimit;
	extern DWORD			dwSkinDetail;	// 0~2
	extern DWORD			dwTextureFilter;

	extern BOOL				bBUFF_SFX;
	extern BOOL				bEMOTICON;

	extern BOOL				bClickEffect;
	extern BOOL				bTargetEffect;
	extern BOOL				bMineEffect;

	
	extern BOOL				bCharacterSimple;	/*character simple, Juver, 2017/10/01 */
	extern BOOL				bHideSkillEffect;	/*skill effect setting, Juver, 2017/10/01 */

	extern BOOL				bHideDamageDisplay;
	extern BOOL				bCharacterMisc;
	extern BOOL				bSpecialName;
	extern BOOL				bClassicNameDisplay;
	extern BOOL				bKillFeedDisplay;

	extern BOOL				bHideDeadName;
	extern BOOL				bHideDeadBody;
	extern BOOL				bPartyDisplay;
	extern BOOL				bPartyBuffDisplay;
	//	衛撿
	extern DWORD			dwFogRange;

	extern LONG				nSndSfx;
	extern LONG				nSndMap;
	extern LONG				nSndMusic;
	extern DWORD			dw3DAlgorithm;
	extern DWORD			dwSndBufferSize;

	extern BOOL				bSndMuteSfx;
	extern BOOL				bSndMuteMap;
	extern BOOL				bSndMuteMusic;	

	//	憮幗 撲薑.
	extern TCHAR			LoginAddress[STRING_NUM_128];
	extern UINT				nLoginPort; /*login port, Juver, 2017/11/16 */
	extern TCHAR			FtpAddressTable[MAX_FTP][STRING_NUM_128];
	extern TCHAR			HttpAddressTable[MAX_HTTP][STRING_NUM_128];	
	extern CString			strNewsURL;
	extern CString			strConnectionURL;
	extern BOOL				bUsePassiveDN;
	extern UINT				nChinaRegion; // 醞措 - 纂憮幗 雖羲(MAX_CHINA_REGION:僭橫爾晦, 0~3:摹鷗雖羲)
	
	// 醞措 雖羲縑 評艇 憮幗 撲薑
	extern SERVER_SET		China_Region[MAX_CHINA_REGION];

	//	樹橫  撲薑.
	extern DWORD			dwLangSet;
	extern CString			strGDIFont;
	extern CString			strFontType;

	//	晦獄 撲薑 橾. ( 樹橫滌 txt )
	extern CString			strCountry;
	extern CString			strLauncherText;
	extern CString			strGameInText;
	extern CString			strGameExText;
	extern CString			strGameWord;
	extern CString			strServerText;
	//xml translation
	extern CString			strGameWordNpc;
	extern CString			strGameWordQuest;
	
	extern CString			strLangPlacer;

	// 紫遺蜓 壎 檜雖 輿模 ( 遽 )
	extern TCHAR			HelpAddress[STRING_NUM_128];
	extern TCHAR			ItemShopAddress[STRING_NUM_128];
	extern TCHAR			RPFDataPath[STRING_NUM_128];
	extern TCHAR			ClientWindowTitle[STRING_NUM_128];


	//temp variable
	extern BOOL bNPCShopConfirm; /*npc shop, Juver, 2017/07/27 */
	extern BOOL bItemPreviewStance; /*item preview, Juver, 2017/07/28 */
	extern BOOL	bVehicleWindowRun; /*vehicle system, Juver, 2017/08/08 */


	void SETIDPARAM ( std::string strPARAM );
	void SETIDPARAM2 ( std::string strPARAM );
	void SETIDPARAM3 ( std::string strPARAM );
	void SETJAPANUUID( std::string strUUID );

	const TCHAR* GETIDPARAM ();
	const TCHAR* GETIDPARAM2 ();
	const TCHAR* GETIDPARAM3 ();
	const TCHAR* GETJAPANUUID();

	/* anti-zoomout cheat fix, SeiferXIII 2020/05/17 */
	float GETZOOMOUTVALUE();
	void SETZOOMOUTVALUE( float fZoomOutValue );

	bool VALIDIDPARAM ();

	BOOL SETUSERID ( const TCHAR* szUSERID );
	BOOL SETCHARID ( const TCHAR* szCHARID );
	std::string GETCHARID();
	std::string GETUSERID_ENC ();
	std::string GETUSERID_DEC ();


	BOOL LOAD ( const TCHAR *szRootPath, bool bServer = false );
	BOOL LOAD_PARAM( const TCHAR *szRootPath );	

	//game options
	BOOL LOAD_GAMEOPTION( const TCHAR *szRootPath, bool bServer = false );	
	BOOL SAVE_GAMEOPTION ();

	//player profile
	void INIT_PROFILEDATA();
	BOOL SAVE_PLAYERPROFILE ();
	BOOL LOAD_PLAYERPROFILE ();

	//texture filter
	DWORD GetTextureFilter();
	DWORD GetTextureAF();

	//game feature
	BOOL LOAD_FEATURE( const TCHAR *szRootPath );	

	extern BOOL				bShowRankName; //MMR Rank
	extern BOOL				bShowRankMark; //MMR Rank

	/*product item, Juver, 2017/10/18 */
	extern BOOL bFeatureProduct;

	/*quick slot delay, Juver, 2017/10/29 */
	extern BOOL bFeatureQuickSlotDelay;
	extern DWORD dwFeatureQuickSlotDelay;

	/*student record ui, Juver, 2017/10/30 */
	extern BOOL bFeatureStudentRecord;

	/*activity system, Juver, 2017/11/04 */
	extern BOOL bFeatureActivity;

	extern BOOL bFeatureCTF;
	extern BOOL bFeaturePVPInterface;
	extern BOOL bFeatureSW;

	extern float fFeatureSkillRangeClientMax;

	/*quest ui, Juver, 2017/11/06 */
	extern BOOL	bFeatureModernQuestWindow;
	//轉生
	extern BOOL bzhuqansheng ;
	//洗點
	extern BOOL bxidian;
	//洗紅
	extern BOOL bxihong;
	//商城
	extern BOOL bshangcheng;
	//轉校
	extern BOOL bzhuanxiao;
	//換名
	extern BOOL bhuanming;
	//身高
	extern BOOL bshengao;
	//轉性
	extern BOOL bzhuanxing;
	//收藏是否銷毀道具
	extern BOOL bShouchang;
	//顯示道具編碼
	extern BOOL bitemcode;

	extern BOOL EATBANANA;

	
	

	/*charinfoview , Juver, 2017/11/11 */
	extern BOOL bFeatureViewCharInfo;

	/*register page, Juver, 2017/11/18 */
	extern BOOL bFeatureRegister;
	extern BOOL bFeatureRegisterUseMD5;

	/*character delete, Juver, 2017/11/18 */	
	extern BOOL bFeatureCharacterDelete;
	extern BOOL bFeatureCharacterDeleteMD5;
	extern BOOL bCreateClass[8];
	/*recovery exp setting, Juver, 2017/11/18 */
	extern BOOL bFeatureUseRecoveryEXP;

	/*hide gm info, Juver, 2017/11/26 */
	extern BOOL bFeatureHideGMInfo;

	/*skill validity check, Juver, 2017/11/27 */
	extern WORD wFeatureSkillRangeCheckAdd;

	/*pet skill info display, Juver, 2017/12/01 */
	extern BOOL bFeaturePetSkilDisplay;

	/*regional chat, Juver, 2017/12/06 */
	extern BOOL bFeatureRegionalChatColor;
	extern BYTE nFeatureRegionalChatColorR;
	extern BYTE nFeatureRegionalChatColorG;
	extern BYTE nFeatureRegionalChatColorB;
	extern DWORD dwFeatureRegionalChatCost;
	extern DWORD dwpaodian;
	extern DWORD dwjifen;

	/*block back damage, Juver, 2017/12/12 */
	extern BOOL bFeatureBlockDamageBack;
	extern float fFeatureBlockDamageBackTimer;

	/*clubwar verify non push or pull, Juver, 2017/12/13 */
	extern BOOL bFeatureClubWarVerifyDisablePushPull;

	/*staff name in chat, Juver, 2017/12/26 */
	extern BOOL bFeatureChatShowStaffName;

	/*change school card, Juver, 2018/01/12 */
	extern BOOL bFeatureChangeSchoolReqCleanQuest;
	extern BOOL bFeatureChangeSchoolReqCleanGuild;
	extern BOOL bFeatureChangeSchoolReqMaxLevel;
	extern BOOL bFeatureChangeSchoolWipeQuest;

	/*item transfer card, Juver, 2018/01/18 */
	extern DWORD dwFeatureItemTransferCost;

    extern BOOL bFeatureDisplayCP;
	//打開槍手

	extern BOOL bgunner ;

	extern BOOL brenzhe;

	extern BOOL bzhongji;
	extern BOOL Pandora;
	extern BOOL Codex;
	extern BOOL PartyFinder;
	extern BOOL MovingTab;
	extern BOOL learnskillnoscroll;
	extern BOOL bpaodian;
	extern BOOL bjifen;

	
};
