#pragma once

#include "GLCharDefine.h"
#include "GLCharData.h"
#include "GLogicData.h"
#include "GLogicEx.h"
#include "GLCOPY.h"

#include "GLContrlMsg.h"
#include "gltradeclient.h"
#include "GLPrivateMarket.h"
#include "GLCLUBMAN.h"

#include "../../Lib_Engine/NaviMesh/Actor.h"
#include "DxSkinChar.h"
#include "s_NetGlobal.h"

#include "../NpcTalk/NpcTalkData.h"

/*skill summon, Juver, 2017/10/08 */
#include "GLSummonData.h"

/*skill activated effect, Juver, 2017/12/29 */
class DxEffSingleGroup;

/*game stats, Juver, 2017/06/21 */
#define PING_PACKET_INTERVAL	1.0f
#define FPS_CALC_INTERVAL		1.0f

/* pet fix add call delay Juver 2017/06/27 */
#define PET_USE_DELAY			3.0f

/*npc shop, Juver, 2017/07/27 */
#define NPC_SHOP_DELAY			2.0f

/*item exchange, Juver, 2017/10/13 */
#define NPC_ITEM_EXCHANGE_DELAY	1.0f


enum EMREACTION
{
	REACT_MOB		= 0,
	REACT_NPC		= 1,
	REACT_P2P		= 2,
	REACT_PVP		= 3,
	REACT_ITEM		= 4,
	REACT_SKILL		= 5,
	REACT_GATHERING	= 6,
	REACT_TOGGLE_VEHICLE	= 7,
	REACT_SWAP_WEAPON		= 8,
	REACT_SIZE		= 9
};

namespace COMMENT
{
	extern std::string szEMREACTION[REACT_SIZE+1];
};

struct SREACTION
{
	EMREACTION		emREACTION;	//	¿¹¾àÇàÀ§.

	bool			bCONTINUE;	//	¿¬¼Ó ¼öÇà.

	D3DXVECTOR3		vMARK_POS;	//	¼±ÇàÀ§Ä¡.

	STARGETID		sTARID;		//	ÇÇÇàÀ§ÀÚ.

	SREACTION () :
		emREACTION(REACT_SIZE),
		bCONTINUE(false),

		vMARK_POS(FLT_MAX,FLT_MAX,FLT_MAX)
	{
	}

	void RESET ()
	{
		vMARK_POS = D3DXVECTOR3(FLT_MAX,FLT_MAX,FLT_MAX);
		emREACTION = REACT_SIZE;
	}

	bool ISVALID ()		{ return (emREACTION!=REACT_SIZE); }
};

//	Note : ¿¢¼Ç ÇàÀ§ Å¸°Ùµé.. ( ¿ì¼±¼øÀ§ ¼øÀ¸·Î Á¤·Ä. )
//
enum EMACTIONTAR
{
	EMACTAR_NULL	= 0,
	
	EMACTAR_PC_PVP	= 1,
	EMACTAR_PC_P2P	= 2,
	
	EMACTAR_MOB		= 3,
	EMACTAR_NPC		= 4,
	
	EMACTAR_PC_OUR	= 5,
	EMACTAR_PC_ANY	= 6,

	EMACTAR_ITEM	= 7,

	EMACTAR_SUMMON_ATTACK	= 8,
	EMACTAR_MATERIAL	= 9, 
};

enum EMFIND_TAR
{
	ENFIND_TAR_NULL		= 0,
	EMFIND_TAR_ANY		= 1,
	EMFIND_TAR_ENEMY	= 2,
	EMFIND_TAR_OUR		= 3,
	EMFIND_TAR_OUR_DIE	= 4,
	EMFIND_TAR_ANY_DIE	= 5,
};

struct SACTIONTAR
{
	EMACTIONTAR		emACTAR;
	STARGETID		sTARID;

	SACTIONTAR () :
		emACTAR(EMACTAR_NULL)
	{
	}
};

class GLCharClient;

typedef std::vector<USER_ATTEND_INFO>		VECATTEND;

class GLCharacter : public GLCHARLOGIC, public GLCOPY
{

protected:
	const static float	m_fELAPS_MOVE;

	enum { EMWAIT_COUNT = 10 };

protected:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	Actor				m_actorMove;
	DxSkinChar*			m_pSkinChar;

	D3DXVECTOR3			m_vDir;
	D3DXVECTOR3			m_vDirOrig;
	D3DXVECTOR3			m_vPos;

	//dmk14 | 11-1-16 | AI - auto pilot
	D3DXVECTOR3			m_vBotPos;
	bool				m_bRunBot;
	bool				m_bBotSupport;
	bool				m_bAutoLoot;
	bool				m_bBotPK;
	bool				m_bResuPtLeader;
	bool				m_bBotFLeader;
	bool				m_bBotLockSpot;

	bool				m_bRunAutoPots;
	float				m_fBotPotsDelay;
	D3DXVECTOR3			m_vBotTargetPt;
	SNATIVEID			m_sBotLastSkill;

	D3DXMATRIX			m_matTrans;

	D3DXVECTOR3			m_vServerPos;

	//	Note : Çàµ¿.
	//
protected:
	EMACTIONTYPE		m_Action;				//	ÇöÁ¦ ¾×¼Ç.
	DWORD				m_dwActState;			//	Çàµ¿ »óÅÂ.
	
	STARGETID			m_sTargetID;
	
	WORD				m_wTARNUM;
	D3DXVECTOR3			m_vTARPOS;
	STARID				m_sTARIDS[EMTARGET_NET];

	TARGETMAP			m_vecTAR_TEMP;

	SNATIVEID			m_sRunSkill;				// ¼±ÅÃÇÑ ½ºÅ³
	SNATIVEID			m_sActiveSkill;				// ±¸µ¿ÁßÀÎ ½ºÅ³

	DWORD				m_dwANISUBCOUNT;
	DWORD				m_dwANISUBSELECT;

	DWORD				m_dwANISUBGESTURE;

	EMITEM_QUESTION		m_emOldQuestionType;	//	È¿°ú »ý¼º/Á¾·á¿ëÀ¸·Î Á÷ÀüÀÇ Å¸ÀÔÀ» ÀúÀåÇÏ°í ÀÖÀ½.

public:
	BOOL				m_bAutoPots;  // 9/13/2016 - Anti  Auto Pots System - Eduj
	BOOL				m_bUsePots;  // 9/13/2016 - Anti  Auto Pots System - Eduj
	BOOL				m_bDisablePots;  // 9/13/2016 - Anti  Auto Pots System - Eduj
	BOOL				m_bDetectOnce;  // 9/13/2016 - Anti  Auto Pots System - Eduj
	float				m_fTickDelay;
	DWORD				m_dwTickCount;

	D3DXVECTOR3			m_vSAVEPOSGM;
	D3DXVECTOR3			m_vSAVETARPOS;

protected:
	float				m_fLastMsgMoveSend;
	GLMSG::SNETPC_GOTO	m_sLastMsgMove;
	GLMSG::SNETPC_GOTO	m_sLastMsgMoveSend;

	SREACTION			m_sREACTION;

protected:
	float				m_fIdleTime;
	float				m_fattTIMER;
	int					m_nattSTEP;

	DWORD				m_dwWAIT;

public:
	DWORD				m_dwGaeaID;

protected:
	DWORD				m_dwNumStorageItem[EMSTORAGE_CHANNEL];	//	Ã¢°í¿¡ µé¾îÀÖ´Â ¾ÆÀÌÅÛ °¹¼ö.
	SCONFTING_CLT		m_sCONFTING;							//	ÁøÇà ´ë·Ã Á¤º¸.

public:
	WORD				m_wPMPosX;								//	°³ÀÎ»óÁ¡ °³¼³ Çã°¡±Ç.
	WORD				m_wPMPosY;
	GLPrivateMarket		m_sPMarket;								//	°³ÀÎ »óÁ¡.

	std::vector< DWORD > m_vecMarketClick;						//	Å¬¸¯ÇÑ °³ÀÎ»óÁ¡ ¸®½ºÆ®

	BOOL				m_bCLUB_CERTIFY;						//	¼±µµ Å¬·´ °áÁ¤Àü ÀÎÁõ À¯¹«.
	float				m_fCLUB_CERTIFY_TIMER;					//	¼±µµ Å¬·´ °áÁ¤Àü ÀÎÁõ ½Ã°£.
	GLCLUB				m_sCLUB;								//	Å¬·´ Á¤º¸.

public:
	WORD				m_wInvenPosX1;				// Temp
	WORD				m_wInvenPosY1;
	WORD				m_wInvenPosX2;				// Temp
	WORD				m_wInvenPosY2;

	/*item wrapper, Juver, 2018/01/12 */
	WORD				m_wInvenPosX3;				// Temp
	WORD				m_wInvenPosY3;

	//used by locker connect from inventory
	WORD				m_wInvenPosX4;	
	WORD				m_wInvenPosY4;

	SITEMCUSTOM			m_sTempItemCustom;

	/*skill summon, Juver, 2017/10/08 */
	DWORD				m_dwSummonGUID[SKILL_SUMMON_MAX_CLIENT_NUM];					//  ¼ÒÈ¯¼ö ID

	HMODULE m_hCheckStrDLL;
	BOOL (_stdcall *m_pCheckString)(CString);

	// Note : Çì¾î˜Þ¿¡¼­ ¸Ó¸®¸¦ ¹Ù²Ü °æ¿ì ÀÓ½Ã·Î µ¥ÀÌÅÍ¸¦ °¡Áö°í ÀÖÀ½.
private:
	BOOL			m_bEnableHairSytle;
	BOOL			m_bEnableHairColor;
	BOOL			m_bEnableFaceStyle;
	WORD			m_wHairColorTEMP;
	WORD			m_wHairStyleTEMP;
	WORD			m_wFaceStyleTEMP;

	BOOL			m_bAttackable;							// °ø°Ý°¡´ÉÀ¯¹«
	BOOL			m_bMiniGameOpen;

	/*npc shop, Juver, 2017/07/27 */
	SNATIVEID		m_sNPCShopCrow;
	SNATIVEID		m_sNPCShopItem;
	WORD			m_wNPCShopType;
	float			m_fNPCShopDelay;

	/*item exchange, Juver, 2017/10/13 */
	float			m_fNPCItemExchangeDelay;

public:
	/*product item, Juver, 2017/10/18 */
	DWORD			m_dwProductID;
	int				m_nProductCount;
	BOOL			m_bItemCompoundTask;
	DWORD			m_dwItemCompoundTaskID;
	float			m_fItemCompoundTaskTime;
	float			m_fItemCompoundTaskTimer;

	/*activity system, Juver, 2017/11/05 */
	float			m_fCharTitleDelay;

	/*pvp capture the flag, Juver, 2018/02/07 */
	float			m_fCaptureTheFlagHoldFlagTimer;
	BOOL			m_bCaptureTheFlagLocatorOn;

	/*PartySearch - Added By Bubu*/
	VECPARTYSEARCH					m_vecPartySearch;

	//dmk14 | 11-1-16 | AI - auto pilot
	HRESULT RunAutoPots( float fTime, float fElapsedTime );
	HRESULT FrameMoveBot( float fTime, float fElapsedTime );

	//dmk14 | 11-1-16 | AI - auto pilot
	HRESULT SetSkillRunSet ( const WORD wSLOT );
	HRESULT SetSelectSkillTarget ( const WORD wSLOT, D3DXVECTOR3 &vTargetPt, SACTIONTAR &sACTAR, bool &bForceBuff );

	//dmk14 | 11-1-16 | AI - auto pilot
	HRESULT ReqToggleRunBot();
	HRESULT ReqToggleRunAutoPots();
	HRESULT SetBotSettings( bool bSupport, bool bLoot, bool bPk, bool bResuPtLeader, bool bFollowLead, bool bLockSpot );

	//dmk14 | 11-1-16 | AI - auto pilot
	SACTIONTAR FindNearTarget( D3DXVECTOR3 &vTargetPt, const EMFIND_TAR emFINDTAR, bool bUseSkill );
	SACTIONTAR FindNearTarget( D3DXVECTOR3 &vTargetPt, const EMFIND_TAR emFINDTAR, SNATIVEID sSkillID );
	SACTIONTAR FindNearItem( bool &bLoot );

	void BotSkillReaction ( const STARGETID &sTargetID, const DWORD dwMR, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );



public:
	BOOL IsOpenMiniGame()			{ return m_bMiniGameOpen; }
	VOID OpenMiniGame()				{ m_bMiniGameOpen = TRUE; }
	VOID CloseMiniGame()			{ m_bMiniGameOpen = FALSE; }

public:
	void HairStyleInitData();
	void HairStyleChange( WORD wStyle );
	void HairStyleEnd()						{ m_bEnableHairSytle = FALSE; }
	
	void HairColorInitData();
	void HairColorChange( WORD wColor );
	void HairColorEnd()						{ m_bEnableHairColor = FALSE; }

	void FaceStyleInitData();
	void FaceStyleChange( WORD wStyle );
	void FaceStyleEnd()						{ m_bEnableFaceStyle = FALSE; }

private:
	void HairStyleUpdate();
	void HairColorUpdate();
	void FaceStyleUpdate();

public:
	BOOL IsMarketClick( DWORD dwGaeaID );
	void InsertMarketClick( DWORD dwGaeaID );

public:
	//	Note : AABB
	//
	D3DXVECTOR3		m_vMaxOrg;
	D3DXVECTOR3		m_vMinOrg;

public:
	D3DXVECTOR3		m_vMax;
	D3DXVECTOR3		m_vMin;
	float			m_fHeight;

protected:
	//	¼öµ¿Àû ÇàÀ§ ¹Ýº¹ Ã¼Å© º¯¼ö,
	//		¼öµ¿ÀûÇàÀ§¸¦ Çß´Â°¡? BOOL
	//		ÃÖ±Ù¿¡ ¼öµ¿Àû ÇàÀ§¸¦ ÇÑµÚ °æ°ú½Ã°£.
	//		¼öµ¿Àû ÇàÀ§ ÃÑ °æ°ú ½Ã°£.
	//
	BOOL				m_bPASSIVITY_ACTION;
	float				m_fPASSIVITY_ACTION_ELAPS;
	float				m_fPASSIVITY_ACTION_TOTAL;
	BOOL				m_bPASSIVITY_ACTION_CHECK;
	float				m_fPASSIVITY_ACTION_CHECK;

	float				m_fKeyDownCheckTime;
	bool				m_bOneHourNotInputKey;

public:
	DWORD				m_dwNPCID;								//  °¡Àå ¸¶Áö¸·¿¡ Å¬¸¯ÇÑ NPC ID

public:
	// PET
	LONGLONG		m_llPetCardGenNum;							// È°¼ºÈ­µÈ ÆÖÄ«µåÀÇ GenNum
	SNATIVEID		m_sPetCardNativeID;							// È°¼ºÈ­µÈ ÆÖÄ«µåÀÇ Nativeid
	BYTE			m_cPetCardGenType;							// È°¼ºÈ­µÈ ÆÖÄ«µåÀÇ GenType

	PETCARDINFO_MAP			m_mapPETCardInfo;					// ÆÖÄ«µå Á¤º¸ (ÆÖÀÇ »óÅÂ°¡ º¯°æµÉ¶§¸¶´Ù °°ÀÌ º¯°æÇØÁà¾ßÇÔ)
	PETCARDINFO_MAP			m_mapPETCardInfoTemp;				// ÆÖÄ«µå Á¤º¸ (°Å·¡½Ã »ç¿ë)
	PETREVIVEINFO_MAP		m_mapPETReviveInfo;					// ºÎÈ°½ÃÅ°±â À§ÇÑ ÆÖÀÇ Á¤º¸

	BOOL					m_bRecievedPetCardInfo;				// ÆÖÄ«µå Á¤º¸ ¼ö½Å¿©ºÎ
	BOOL					m_bIsPetActive;						// ¸ÊÀ» ÀÌµ¿ÇÒ¶§ ÆÖÀÌ È°µ¿ÁßÀÌ¾ú´ÂÁö ¿©ºÎ

public:
	VEHICLEITEMINFO_MAP			m_mapVEHICLEItemInfo;					// Å»°Í ¾ÆÀÌÅÛÀÇ Á¤º¸ ( Å»°Í »óÅÂ º¯°æ )
	VEHICLEITEMINFO_MAP			m_mapVEHICLEItemInfoTemp;				// Å»°Í ¾ÆÀÌÅÛÀÇ Á¤º¸ ( °Å·¡½Ã »ç¿ë )

	GLVEHICLE		m_sVehicle;				// Å»°Í ±¸Á¶Ã¼
	BOOL			m_bReqVehicle;			// º¸µå Å¾½Â ¹× ÇØÁ¦ ÁßÀÌ¶ó¸é...
	BOOL			m_bReqDoneVehi;
	BOOL			m_bIsVehicleActive;		// ¸Ê ÀÌµ¿Áß Å»°Í Å¸°í ÀÖ¾ú´Ù¸é...
	BOOL			m_bRecivedVehicleItemInfo;  // Å»°Í Á¤º¸ ¼ö½Å ¿©ºÎ

	bool			m_bOldVisibleTracingUI[8];
	bool			m_bNewVisibleTracingUI[8];

	float			m_fPrintProcessInfoMsgGap;


	BOOL			m_bDisTrade;
	BOOL			m_bDisTradeOLD;

	bool			m_bGarbageOpen;
	SINVEN_POS		m_sGarbageItem;			// ÈÞÁöÅë¿¡ µî·ÏµÈ ¾ÆÀÌÅÛ

	bool			m_bItemShopOpen;		// ItemShopOpen ( Japan ) 
	
	bool			m_bReqAttendList;		// Ãâ¼® ¸®½ºÆ®
	VECATTEND		m_vecAttend;			// Ãâ¼® ¸®½ºÆ®
	DWORD			m_dwComboAttend;		// ¿¬¼Ó Ãâ¼® ÀÏ¼ö
	__time64_t		m_tAttendLogin;			// ·Î±×ÀÎ½Ã°£ ( Ãâ¼®ºÎ¿¡¸¸ »ç¿ë ) 
	DWORD			m_dwAttendTime;			// Ãâ¼® °¡´ÉÇÑ °ÔÀÓ Á¢¼Ó ½Ã°£

	CString			m_strRecordChat;
	bool			m_bRecordChat;
	CTime			m_recordStartTime;

	SINVEN_POS		m_sItemMixPos[ITEMMIX_ITEMNUM];

	/*dash skill logic, Juver, 2017/06/17 */
	WORD			m_wActionAnim;

	/*game stats, Juver, 2017/06/21 */
	float			m_fPacketInterValTimer;
	clock_t			m_clock_Packet;
	float			m_fFPSInterValTimer;
	unsigned int	m_uNumFramesSinceLastCalc;

	/*rightclick wear/unwear, Juver, 2017/06/23 */
	bool			m_bAccessorySlotFirst;
	bool			m_bRingSlotFirst;
	bool			m_bEarringSlotFirst;
	float			m_fWearDelay;

	/* pet fix add call delay Juver 2017/06/27 */
	float			m_fPetUseDelay;

public:
	float			m_fHPCheckTimer; //hpcheck
	WORD			m_wLastHPCheck;//hpcheck
	WORD			m_wLastHPCounter;//hpcheck
	DWORD			m_dwMismatchCount;

	/*charinfoview , Juver, 2017/11/11 */
	float			m_fReqCharInfoTimer;

	/*bike color , Juver, 2017/11/16 */
	WORD			m_wBikeColor[BIKE_COLOR_SLOT_PART_SIZE];
	BOOL			m_bEnableBikeColor;

	/*change scale card, Juver, 2018/01/03 */
	float			m_fActiveCharScale;
	BOOL			m_bChangeCharScale;

	/*item color, Juver, 2018/01/10 */
	WORD			m_wItemColor1;
	WORD			m_wItemColor2;
	BOOL			m_bEnableItemColor;
	EMSLOT			m_emSlotItemColor;

	/*item transfer card, Juver, 2018/01/18 */
	SINVEN_POS		m_sItemTransferPosOld;
	SINVEN_POS		m_sItemTransferPosNew;

	/* car, cart color, Juver, 2018/02/14 */
	WORD			m_wCarColorA;
	WORD			m_wCarColorB;
	BOOL			m_bEnableCarColor;


public:
	void DoPASSIVITY ( BOOL bPASSIVITY );
	bool CheckPASSIVITY ( float fElaps );
	void ResetPASSIVITY ();
	bool IsBlockPASSIVITY ();

	void UpdateSpecialSkill();
public:
	BOOL ISCONFRONT_TAR ( const STARGETID &sTargetID );
	void DELCONFRONT_MEMBER ( DWORD dwGaeaID );

public:
	BOOL IsReActionable ( const STARGETID &sTargetID, BOOL bENEMY=TRUE );

public:
	BOOL IsSTATE ( DWORD dwState )		 			{ return m_dwActState&dwState; }
	
	virtual BOOL IsACTION ( EMACTIONTYPE emCur )	{ return m_Action == emCur; }

public:
	void SetSTATE ( DWORD dwState )					{ m_dwActState |= dwState; }
	void ReSetSTATE ( DWORD dwState )				{ m_dwActState &= ~dwState; }

public:
	BOOL IsCtrlBlockBody ();
	BOOL IsValidBody ();
	BOOL IsDie ();
	BOOL IsRunning ()								{ return IsACTION(GLAT_MOVE) && IsSTATE(EM_ACT_RUN); };
	BOOL IsRunMode ()								{ return IsSTATE(EM_ACT_RUN); };

	virtual BOOL IsPartyMaster ();
	BOOL IsPartyMem ();

	BOOL IsClubMaster ();
	BOOL IsCDCertify ();

protected:
	void TurnAction ( EMACTIONTYPE toAction );

public:
	void ResetAction();

public:
	float GetMoveVelo ();
	BOOL ActionMoveTo ( float fTime, D3DXVECTOR3 vFromPt, D3DXVECTOR3 vTargetPt, BOOL bContinue=FALSE, BOOL bREACT=FALSE );
	BOOL LargeMapMoveTo( D3DXVECTOR3* pPos );

public:
	Actor& GetActorMove ()						{ return m_actorMove; }
	DxSkinChar* GetSkinChar ()					{ return m_pSkinChar; }

public:
	HRESULT SetPosition ( D3DXVECTOR3 vPos );
	virtual const D3DXVECTOR3 &GetPosition ()	{ return m_vPos; }
	float GetDirection ();
	D3DXVECTOR3 GetDirectionVector () { return m_vDir; }

public:
	SCHARDATA2& GetCharData ()			{ return (*this); }
	GLCHARLOGIC& GetCharLogic ()		{ return (*this); }
	GLTradeClient& GetTradeClient ()	{ return GLTradeClient::GetInstance(); }
	SNATIVEID GetskillRunSlot ()		{ return m_sRunSkill; }
	SNATIVEID GetActiveSkillRun()		{ return m_sActiveSkill; }
	SCONFTING_CLT& GetConfting ()		{ return m_sCONFTING; }
	float GetBuyRate ();
	float GetSaleRate ();

public:
	const SITEMCUSTOM& GET_PRETRADE_ITEM ();
	BOOL IsVALID_STORAGE ( DWORD dwChannel );

	STARGETID GetTargetID () { return m_sTargetID; }


public:
	DWORD DetectGate ();
	CString DetectGateToMapName ();

	HRESULT UpdateSuit( BOOL bChangeHair = TRUE, BOOL bChangeFace = TRUE );

public:
	virtual EMCROW GetCrow () const			{ return CROW_PC; }
	virtual DWORD GetCtrlID () const		{ return m_dwGaeaID; }
	virtual DWORD GetCharID () const		{ return m_dwCharID; }

	virtual const char* GetName () const	{ return m_szName; }
	virtual GLDWDATA GetHp () const			{ return m_sHP; }

	virtual WORD GetSchool() const		    { return m_wSchool; }
	virtual WORD GetBodyRadius ();
	virtual float GetBodyHeight ()			{ return m_fHeight; }
	virtual D3DXVECTOR3 GetPosBodyHeight ();
	virtual void GetSkinAABB( D3DXVECTOR3& vMin, D3DXVECTOR3& vMax );
	virtual float GetSkinScale();
	virtual float GetSkinHeight();

	virtual bool IsSafeZone () const { return m_bSafeZone; }

	bool IsSafeTime () const { return m_bSafeTime; }

protected:
	BOOL RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, const DWORD dwSELECT, WORD _wCasterCrow, DWORD _dwCasterID );

	virtual void ReceiveDamage ( DWORD dwDamage, DWORD dwDamageFlag, STARGETID sACTOR );
	virtual void ReceiveAVoid ();
	virtual void ReceiveSwing ();
	void ReceiveShock ();

	void PrintTracingUserWindowInfo();
	void PrintTracingUserProcessInfo( float fElapsedTime );

public:
	virtual void MsgProcess( NET_MSG_GENERIC* nmg );


	void MsgProcessMiniGame( NET_MSG_GENERIC* nmg );
	void MsgProcessAlliance( NET_MSG_GENERIC* nmg );
	void MsgProcessClub( NET_MSG_GENERIC* nmg );
	void MsgProcessQuest( NET_MSG_GENERIC* nmg );
	void MsgProcessConfront( NET_MSG_GENERIC* nmg );

	void MsgProcessClubBattle( NET_MSG_GENERIC* nmg );
	void MsgProcessAllianceBattle( NET_MSG_GENERIC* nmg );

	void MsgDefenseSkillActive( GLMSG::SNETPC_DEFENSE_SKILL_ACTIVE* nmg );

	bool IsInsertToInven ( PITEMCLIENTDROP pItemDrop );

public:
	DWORD GetAmountActionQ ( WORD wSLOT );

public:
	bool IsInvenSplitItem ( WORD wPosX, WORD wPosY, bool bVietnamInven = FALSE );
	bool IsStorageSplitItem ( DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsClubStorageSplitItem ( DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsNpcPileItem ( SNATIVEID sNID_NPC, DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsRestartPossible(); // ÀÌ ¸Ê¿¡¼­ Àç½ÃÀÛÀÌ µÇ´ÂÁö

	BOOL IsVaildTradeInvenSpace ();

	bool IsKEEP_CLUB_STORAGE ( DWORD dwCHANNEL );

public:
	void ReqToggleRun ();
	void ReqTogglePeaceMode ();
	void ReqGateOut ();

	void ReqVisibleNone ();
	void ReqVisibleOff ();
	void ReqVisibleOn ();

	bool ReqGESTURE ( int nMOTION, bool bCOMMAND=false );

	void SET_RECORD_CHAT();
	void UPDATE_RECORD_CHAT( bool bCloseClient = FALSE );

public:
	EMCONFT_TYPE GetConftType ( DWORD dwID );

public:
	void InitAllSkillFact ();

public:
	LONGLONG GetCalcTaxiCharge( int nSelectMap, int nSelectStop );

public:
	// ÇØÅ· ÀçÇöÀ» À§ÇÑ ¸Þ½ÃÁö ¹ß¼Û
	void SendHackingMSG ();
	void SendHackingMSG1 ();

public:
	bool IsCoolTime( SNATIVEID sNativeID );
	bool CheckCoolTime( SNATIVEID sNativeID );
	bool SetCoolTime ( ITEM_COOLTIME& sCoolTime, EMCOOL_TYPE emCoolType );
	__time64_t	GetMaxCoolTime ( SNATIVEID sNativeID );

	ITEM_COOLTIME*	GetCoolTime( DWORD dwCoolID, EMCOOL_TYPE emCoolType );
	ITEM_COOLTIME*	GetCoolTime ( SNATIVEID sNativeID );
	
public:
	//	Note : ½ºÅ³ ¹è¿ì±â ¿äÃ». ( ÀÎº¥ ¾ÆÀÌÅÛÀ¸·Î. )
	HRESULT ReqInvenSkill ( WORD wPosX, WORD wPosY );

	//	Note : ½ºÅ³ ¹è¿ì±â ¿äÃ». ( Ã¢°í ¾ÆÀÌÅÛÀ¸·Î. )
	HRESULT ReqStorageSkill ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : ±ÍÈ¯ ¿äÃ».
	HRESULT ReqReCall ( WORD wPosX, WORD wPosY );

	//	Note : ¼ÒÈ¯¼­ »ç¿ë ¿äÃ».
	HRESULT ReqTeleport ( WORD wPosX, WORD wPosY );

	//	Note : ¼±¹°»óÀÚ ¿­±â.
	HRESULT ReqBoxOpen ( WORD wPosX, WORD wPosY );

	HRESULT ReqGMSkillAll(WORD wLevel, DWORD dwPass);//gm¥þ§Þ¯à

	//	Note : ·£´ý¾ÆÀÌÅÆ ¹Ú½º ¿­±â.
	HRESULT ReqRandumBoxOpen ( WORD wPosX, WORD wPosY );

	//	Note : ·£´ý¾ÆÀÌÅÆ ¹Ú½º ¿­±â.
	HRESULT ReqDisJunction ( WORD wPosX, WORD wPosY );

	HRESULT ReqGMItem( SNATIVEID sItemID,WORD wNum ,WORD wPass);//add itemcmd

	//	Note : Æê ·£´ý ½ºÅ² ¾ÆÀÌÅÛ ¿­±â.
	HRESULT ReqPetSkinPackOpen ( WORD wPosX, WORD wPosY );

	//	Note : ¿¬¸¶ÇÏ±â.
	HRESULT ReqGrinding ( WORD wPosX, WORD wPosY );

	//	Note : ÄÚ½ºÅù º¹Àå°ú ÀÏ¹Ý º¹Àå°ú °áÇÕ.
	HRESULT ReqDisguise ( WORD wPosX, WORD wPosY );

	//	Note : ÄÚ½ºÅù º¹Àå°ú ÀÏ¹Ý º¹Àå°ú ºÐ¸®.
	HRESULT ReqCleanser ( WORD wPosX, WORD wPosY );

	//	Note : ÄÉ¸¯ÅÍ Ä«µå »ç¿ë.
	HRESULT ReqCharCard ( WORD wPosX, WORD wPosY );

	//	Note : Ã¢°í Ä«µå »ç¿ë.
	HRESULT ReqStorageCard ( WORD wPosX, WORD wPosY, WORD wSTORAGE );

	//	Note : Ã¢°í Ä«µå »ç¿ë.
	HRESULT ReqInvenLineCard ( WORD wPosX, WORD wPosY );

	HRESULT ReqUpgradeCard(WORD wPosX, WORD wPosY);//µ¥¯Å¤É¯Å¥d

	//	Note : Ã¢°í ¿¬°á Ä«µå »ç¿ë ¿äÃ»
	HRESULT ReqStorageOpenCard ( WORD wPosX, WORD wPosY );

	HRESULT ReqStorageOpenCardInvenRequest ( WORD wPosX, WORD wPosY );
	HRESULT ReqStorageOpenCardInvenConfirmed ();

	void	ReqGarbageOpenInven();
	HRESULT	ReqSortItem ();

	HRESULT ReqInvenSort ();
	//	Note : Ã¢°í ¿¬°á Ä«µå »ç¿ë ¿Ï·á
	HRESULT ReqStorageCloseCard();
	

	//¹CÀ¸¹ô§I´«¿n¤À¨t²Î
	//HRESULT ResetSkillStatsTo ( WORD wPosX, WORD wPosY );
//	HRESULT ReqChangeMoneyTo ();//change


	//	Note : ÇÁ¸®¹Ì¾ö¼Â »ç¿ë.
	HRESULT ReqPremiumSet ( WORD wPosX, WORD wPosY );
	
	//	Note : ½ºÅ³¹× ½ºÅÝ ¸®¼Â.
	HRESULT ResetSkillStats ( WORD wPosX, WORD wPosY );
	HRESULT ReqResetSkillStats ( );

	HRESULT	ResetBrightPoint ( WORD wPosX, WORD wPosY );
	HRESULT ReqResetBrightPoint ();

	//CNDev | 2-7-2020 | e-point card system
	HRESULT	AddEPoint ( WORD wPosX, WORD wPosY );
	HRESULT ReqAddEPoint ();
	
	HRESULT	ContributionPointCard ( WORD wPosX, WORD wPosY );
	HRESULT ReqContributionPointCard ();

	HRESULT ReqMysteryBox(WORD wPosX, WORD wPosY); //Mystery Box
	//¬~ÂI¨t²Î
	//HRESULT ResetSkillStatsTo ( WORD wPosX, WORD wPosY );
	HRESULT ReqResetSkillStatsTo();
	//¬~¬õ¨t²Î
	//HRESULT ResetSkillStatsTo ( WORD wPosX, WORD wPosY );
	HRESULT ReqResetRedStatsTo();//change

	//	Note : Çì¾î½ºÅ¸ÀÏ º¯°æ.
	HRESULT ReqInvenHairChange ( WORD wPosX, WORD wPosY );

	HRESULT ReqInvenHairStyleChange ( WORD wHairStyle );
	HRESULT InvenHairStyleChange ( WORD wPosX, WORD wPosY );

	HRESULT ReqInvenHairColorChange ( WORD wHairColor );
	HRESULT InvenHairColorChange ( WORD wPosX, WORD wPosY );

	//	Note : ¾ó±¼½ºÅ¸ÀÏ º¯°æ.
	HRESULT ReqInvenFaceChange ( WORD wPosX, WORD wPosY );

	HRESULT ReqInvenFaceStyleChange ( WORD wHairStyle );
	HRESULT InvenFaceStyleChange ( WORD wPosX, WORD wPosY );

	//	Note : ÅÃ½ÃÄ«µå »ç¿ë
	HRESULT InvenUseTaxiCard( WORD wPosX, WORD wPosY );

	//	Note : NPC ¼ÒÈ¯
	HRESULT InvenUseNpcRecall( WORD wPosX, WORD wPosY );

	// Note : ¼ºº° º¯°æ
	HRESULT InvenGenderChange ( WORD wPosX, WORD wPosY );
	HRESULT	ReqInvenGenderChange ( WORD wFace, WORD wHair );

	// Note : ÀÌ¸§ º¯°æ.
	HRESULT ReqInvenRename ( const char* szCharName );

	HRESULT InvenRename ( WORD wPosX, WORD wPosY );

	//	Memo : °³Á¶ ±â´É
	HRESULT ReqRemodelOpenCard ( WORD wPosX, WORD wPosY );

	//	Memo : ÈÞÁöÅë ±â´É
	HRESULT ReqGabargeOpenCard ( WORD wPosX, WORD wPosY );

	//	Note : º£Æ®³² ¾ÆÀÌÅÛ, °æÇèÄ¡ È¹µæ Ä«µå »ç¿ë
	HRESULT ReqInvenVietnamGet ( WORD wPosX, WORD wPosY, bool bGetExp );


public:
	//	Note : °Å·¡ÇÒ ±Ý¾× ³Ö±â.
	//HRESULT ReqTradeMoney ( LONGLONG lnMoney );

	//	Note : °Å·¡ÇÒ ¾ÆÀÌÅÛ ³Ö±â/Á¦°Å/±³È¯.
	HRESULT ReqTradeBoxTo ( WORD wPosX, WORD wPosY );

	//	Note : °Å·¡ ¼ö¶ô.
	HRESULT ReqTradeAgree ();

	//	Note : °Å·¡ Ãë¼Ò.
	HRESULT ReqTradeCancel ();

	/*trade lock, Juver, 2018/01/02 */
	HRESULT ReqTradeLock(); 

	//	Note : ´ë·Ã ¿äÃ».
	HRESULT ReqConflict ( DWORD dwID, const SCONFT_OPTION &sOption );

public:
	//	Note : Ã¢°í Á¤º¸¸¦ ¼­¹ö¿¡ ¿äÃ».
	HRESULT ReqGetStorage ( DWORD dwChannel, DWORD dwNPCID );
	
	//	Note : Ã¢°í ¾ÆÀÌÅÛ µé¶§, ³õÀ»¶§, ±³È¯ÇÒ¶§, ÇÕÄ¥¶§.
	HRESULT ReqStorageTo ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : ÀÎº¥Åä¸® °ãÄ§ ¾ÆÀÌÅÛ ºÐ¸®.
	HRESULT ReqStorageSplit ( DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wSplitNum );

	//	Note : Ã¢°í ¾ÆÀÌÅÛ »ç¿ëÇÒ¶§ ( ¸¶½Ã±â, ½ºÅ³¹è¿ì±â µî ).
	HRESULT ReqStorageDrug ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : Ã¢°í µ· ³Ö±â.
	HRESULT ReqStorageSaveMoney ( LONGLONG lnMoney );

	//	Note : Ã¢°í µ· »©³»±â.
	HRESULT ReqStorageDrawMoney ( LONGLONG lnMoney );

	//	Note : »óÀÎ Npc¿¡¼­ ¹°Ç°À» ±¸ÀÔÇÏ°Å³ª ÆÇ¸Å½Ã.
	HRESULT ReqNpcTo ( SNATIVEID sNID_NPC, DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wBuyNum=1 );

	//	Note : ±¸ÀÔÇÑ ¹°Ç° ²¨³»¿À±â.
	HRESULT ReqChargedItemTo ( WORD wPosX, WORD wPosY );
	//	Note : ±¸ÀÔÇÑ ÀÌÀÌÅÛ Á¤º¸ °¡Á®¿À±â
	HRESULT ReqItemBankInfo ();

	/*dmk14 ingame web*/
	HRESULT ReqItemMallBuy( ITEMMALLDATA );

public:
	//	Note : ÇÊµå ¾ÆÀÌÅÛ(µ·) ÁÖÀ»¶§.
	HRESULT ReqFieldTo ( const STARGETID &sTargetID, bool bPet = false );

	//	Note : ÀÎº¥Åä¸® ¾ÆÀÌÅÛ µé¶§, ³õÀ»¶§, ±³È¯ÇÒ¶§, ÇÕÄ¥¶§.
	HRESULT ReqInvenTo ( WORD wPosX, WORD wPosY );
	//	Note : º£Æ®³² ÀÎº¥Åä¸® ¾ÆÀÌÅÛ µé¶§, ³õÀ»¶§, ±³È¯ÇÒ¶§, ÇÕÄ¥¶§.
	HRESULT ReqVNInvenTo ( WORD wPosX, WORD wPosY );
	//	Note : º£Æ®³² ÀÎº¥Åä¸®ÀÇ ¾ÆÀÌÅÛ ÀüÃ¼ »èÁ¦
	HRESULT ReqVNInveReset ();
	//	Note : º£Æ®³² ÀÎº¥Åä¸®¿¡¼­ ¿À¸¥ÂÊ ¹öÆ°À¸·Î ¾ÆÀÌÅÛÀ» ¿Å±æ °æ¿ì
	HRESULT ReqVietemInvenTo ( WORD wPosX, WORD wPosY );

	//	Note : ÀÎº¥Åä¸® °ãÄ§ ¾ÆÀÌÅÛ ºÐ¸®.
	HRESULT ReqInvenSplit ( WORD wPosX, WORD wPosY, WORD wSplitNum );

	//UnlockSkill
	HRESULT ReqNonInvenSkill ( SNATIVEID sSKILLID );

	//	Note : ÀÎº¥Åä¸® ¾ÆÀÌÅÛ »ç¿ëÇÒ¶§ ( ¸¶½Ã±â, ½ºÅ³¹è¿ì±â µî ).
	HRESULT ReqInvenDrug ( WORD wPosX, WORD wPosY );

	//	Note : ¿¢¼Ç Äü½½·Ô¿¡ ÀÖ´Â °ÍÀ» ¾²±â. ( ¾àÇ°ÀÏ °æ¿ì ¸¶½Ã±â ).
	HRESULT ReqActionQ ( WORD wSLOT );

	//	Note : ½ºÅ³ ·¾¾÷ ¿äÃ».
	HRESULT ReqSkillUp ( const SNATIVEID skill_id );

	//	Note : »ç¿ëÇÒ ½ºÅ³ ¼³Á¤.
	HRESULT ReqSkillRunSet ( const WORD wSLOT );
	HRESULT ReqSkillRunReSet ();

	//	Note : ¾ÆÀÌÅÛ ½½·Ô¿¡ µî·Ï/Á¦°Å.
	HRESULT ReqItemQuickSet ( const WORD wSLOT );
	HRESULT ReqItemQuickReSet ( const WORD wSLOT );

	//	Note : ½ºÅ³ ½½·Ô¿¡ µî·Ï/Á¦°Å.
	HRESULT ReqSkillQuickSet ( const SNATIVEID skill_id, const WORD wSLOT );
	HRESULT ReqSkillQuickReSet ( const WORD wSLOT );

	//	Note : ¾ÆÀÌÅÛÀ» Âø¿ëÇÏ°Å³ª µé¾î¿Ã¸².
	HRESULT ReqSlotTo ( EMSLOT emSlot );

	//	Note : ±Ø°­ºÎ ¹«±â¸¦ ¸ÞÀÎ°ú ¼­ºê ±³Ã¼.
	HRESULT ReqSlotChange();

	//	Note : ¾ÆÀÌÅÛÀ» ±¸ÀÔÇÏ°Å³ª/ÆÈ¾Æ¹ö¸².
	//	Note : ¾ÆÀÌÅÛ ¹Ù´Ú¿¡ ¹ö¸².
	HRESULT ReqHoldToField ( const D3DXVECTOR3 &vPos );

	//	Note : ÆøÁ×À» ¹Ù´Ú¿¡ »ç¿ë.
	HRESULT ReqHoldToFieldFireCracker ( const D3DXVECTOR3 &vPos );

	//	Note : µ· ¹Ù´Ú¿¡ ¹ö¸².
	HRESULT ReqMoneyToField ( LONGLONG lnMoney );

	//	Note : ºÎÈ° À§Ä¡ ÁöÁ¤ ¿äÃ».
	HRESULT ReqReGenGate ( DWORD dwNpcID );

	//	Note : ±ÍÈ¥ÁÖ·Î ºÎÈ° ¿äÃ».
	HRESULT ReqReGenRevive ();

	//  Note : ¼­¹ö¿¡ °æÇèÄ¡ º¹±¸ ºÎÈ°À» ¿äÃ»ÇÑ´Ù.
	HRESULT ReqRecoveryExp ();

	//  Note : ¼­¹ö¿¡ °æÇèÄ¡ º¹±¸¸¦ ¿äÃ»ÇÑ´Ù. (NPC)
	HRESULT ReqRecoveryExpNpc ( DWORD dwNpcID );
	
	//  Note : º¹±¸ÇÒ °æÇèÄ¡¸¦ ¹¯´Â´Ù.
	HRESULT ReqGetReExp ();	
	HRESULT ReqGetReExpNpc ( DWORD dwNpcID );	

	//	Note : Ä¡·á ¿äÃ».
	HRESULT ReqCure ( DWORD dwNpcID, DWORD dwGlobalID );
	HRESULT MsgCureFB ( NET_MSG_GENERIC* nmg );

	//	Note : ÄÉ¸¯ÅÍ Á¤º¸ ¸®¼Â. ( stats, skill )
	HRESULT ReqCharReset ( DWORD dwNpcID );

	//	Note : NPC¿Í itemÀ» ±³È¯ A:npc¿¡°Ô ÁÖ´Â°Í, b:npc¿¡°Ô¼­ ¹Þ´Â°Í.
	HRESULT ReqItemTrade ( DWORD dwNpcID, DWORD dwGlobalID, SNPC_ITEM *pDwA_NID, DWORD dwB_NID );

	//	Note : Ä£±¸¿¡°Ô °¡±â.
	HRESULT	Req2Friend ( const char *szNAME );
	/* 3-12-2024Boss Spawn Viewer, Bubu */
	HRESULT ReqBossDetails(DWORD dwGlobID, SNATIVEID sMapID, SNATIVEID sMobID);

public:
	//	Note : »õ·Î¿î Äù½ºÆ® ½ÃÀÛ.
	HRESULT ReqQuestStart ( DWORD dwNpcID, DWORD dwTalkID, DWORD dwQUESTID );

	//	Note : Äù½ºÆ® ÁøÇà. ( npc¿Í ´ëÈ­ )
	HRESULT ReqQuestStepNpcTalk ( DWORD dwNpcID, DWORD dwTalkID, DWORD dwQUESTID, DWORD dwQUESTSTEP );

	//	Note : Äù½ºÆ® Æ÷±â.
	HRESULT ReqQuestGiveUp ( DWORD dwQUESTID );

	//	Note : Äù½ºÆ® ÀÐ¾îº¸±â ¿äÃ» ( ÀÐ¾úÀ½À» ÀÀ´ä. )
	HRESULT ReqQuestREADINGReset ( DWORD dwQUESTID );

	//	Note : Äù½ºÆ® ¿Ï·á¿äÃ».
	HRESULT ReqQuestComplete ( DWORD dwQUESTID );

	//	Note : ¹ö½º ½ÂÂ÷ ¿äÃ».
	HRESULT ReqBusStation ( DWORD dwNpcID, DWORD dwSTATION );

	//	Note : ÅÃ½Ã ½ÂÂ÷ ¿äÃ».
	HRESULT ReqTaxiStation ( WORD wPosX, WORD wPosY, int nSelectMap, int nSelectStop );

	//	Note : È®¼º±â »ç¿ë ¿äÃ».
	/*item link, Juver, 2017/07/31 */
	HRESULT ReqLoudSpeaker ( const char* szChat, SITEMLINK* pItemLink ); 

public:
	//	Note : °³ÀÎ»óÁ¡ÀÇ Å¸ÀÌÆ² ¼³Á¤.
	HRESULT ReqPMarketTitle ( const char* szTitle );

	//	Note : °³ÀÎ»óÁ¡¿¡ ¾ÆÀÌÅÛÀ» µî·Ï.
	HRESULT ReqPMarketRegItem ( WORD wPosX, WORD wPosY, LONGLONG llMoney, DWORD dwNum );

	//	Note : °³ÀÎ»óÁ¡¿¡ µî·ÏµÈ ¾ÆÀÌÅÛÀ» Á¦°Å.
	HRESULT ReqPMarketDisItem ( WORD wPosX, WORD wPosY );

	//	Note : °³ÀÎ»óÁ¡ ¿­±â.
	HRESULT ReqPMarketOpen ();

	//	Note : °³ÀÎ»óÁ¡ ´Ý±â.
	HRESULT ReqPMarketClose ();

	//	Note : ´Ù¸¥ »ç¶÷ÀÇ °³ÀÎ»óÁ¡ÀÇ Á¤º¸ ¼ö½Å.
	HRESULT ReqPMarketInfo ( DWORD dwGaeaID );
	HRESULT ReqPMarketInfoRelease ( DWORD dwGaeaID );

	//	Note : ´Ù¸¥ »ç¶÷ÀÇ °³ÀÎ »óÁ¡¿¡¼­ ¹°Ç°À» ±¸ÀÔ.
	HRESULT ReqPMarketBuy ( DWORD dwGaeaID, WORD wPosX, WORD wPosY, DWORD dwNum );

public:
	//	Note : Å¬·´ »ý¼º.
	HRESULT ReqClubNew ( DWORD dwNpcID, const char* szClubName );
	//	Note : Å¬·´ ·©Å©¾÷.
	HRESULT ReqClubRank ( DWORD dwNpcID );

	//	Note : Å¬·´ ÇØ»ê.
	HRESULT ReqClubDissolution ();
	//	Note : Å¬·´ ÇØ»ê Ãë¼Ò.
	HRESULT ReqClubDissolutionCancel ();
	//	Note : Å¬·´ °¡ÀÔ ¿äÃ» ( ¸¶½ºÅÍ°¡ ¿¹ºñ Å¬·´¿ø¿¡°Ô ).
	HRESULT ReqClubJoin ( DWORD dwGaeaID );
	//	Note : Å¬·´ °¡ÀÔ ¿äÃ»¿¡ ´ëÇÑ ÀÀ´äÇÏ±â.
	HRESULT ReqClubJoinAns ( DWORD dwMaster, bool bOK );
	//	Note : Å¬·´¿øÀ» °­Á¦ Å»Åð½ÃÅ°±â.
	HRESULT ReqClubMemberDel ( DWORD dwMember );
	//	Note : Å¬·´ ¸â¹ö Å»Åð.
	HRESULT ReqClubSecede ();
	//	Note : Å¬·´ ¸¶½ºÅÍ ±ÇÇÑ À§ÀÓ
	HRESULT ReqClubAuthority ( DWORD dwMember );
	//	Note : Å¬·´ ¸¶½ºÅÍ ±ÇÇÑ À§ÀÓ ´äº¯
	HRESULT ReqClubAuthorityAns ( bool bOK );

	//	Note : Å¬·´ ¸¶Å© °»½Å ¿äÃ».
	HRESULT ReqClubMarkInfo ( DWORD dwClubID, DWORD dwMarkVer );

	//	Note : »õ Å¬·´ ¸¶Å©·Î º¯°æ ¿äÃ».
	HRESULT ReqClubMarkChange ( const char* szFileName );
	//	Note : Å¬·´ ¸â¹ö º°¸í.
	HRESULT ReqClubNick ( const char* szNickName );

	//	Note : cd ÀÎÁõ.
	HRESULT ReqCDCertify (DWORD dwNpcID );

	//	Note : ¼±µµÁö¿ª ¼ö¼ö·á ¼³Á¤.
	HRESULT ReqGuidCommission ( DWORD dwNPCID, float fRATE );

	//	Note : Å¬·´ °øÁö.
	HRESULT ReqClubNotice ( const char* szClubNotice );

	//	Note : Å¬·´ ºÎ¸¶ ¼³Á¤.
	HRESULT ReqClubSubMaster ( DWORD dwCharID, DWORD dwClubFlag );

	//	Note : Å¬·´ µ¿¸Í ¿äÃ».
	HRESULT ReqClubAlliance ( DWORD dwGaeaID );

	//	Note : Å¬·´ µ¿¸Í ¿äÃ» ´äº¯.
	HRESULT ReqClubAllianceAns ( DWORD dwChiefCharID, bool bOK );

	//	Note : Å¬·´ µ¿¸Í Å»Åð ¿äÃ».
	HRESULT ReqClubAllianceSec ();

	//	Note : Å¬·´ µ¿¸Í Á¦¸í ¿äÃ».
	HRESULT ReqClubAllianceDel ( DWORD dwCharID );

	//	Note : Å¬·´ µ¿¸Í ÇØÃ¼ ¿äÃ».
	HRESULT ReqClubAllianceDis ();

	//	Note : Å¬·´ ¹èÆ² ¿äÃ».
	HRESULT ReqClubBattle ( DWORD dwGaeaID, DWORD dwTime );

	//	Note : Å¬·´ ¹èÆ² ¿äÃ» ´äº¯.
	HRESULT ReqClubBattleAns ( DWORD dwChiefCharID, bool bOK );

	//	Note : Å¬·´ ¹èÆ² ÈÞÀü ¿äÃ».
	HRESULT ReqClubBattleArmistice( DWORD dwCLUBID );

	//	Note : Å¬·´ ¹èÆ² ÈÞÀü ´äº¯.
	HRESULT ReqClubBattleArmisticeAns( DWORD dwCLUBID, bool bOK );

	//	Note : Å¬·´ ¹èÆ² Ç×º¹ ¿äÃ».
	HRESULT ReqClubBattleSubmission ( DWORD dwCLUBID );

	
	//	Note : µ¿¸Í ¹èÆ² ¿äÃ».
	HRESULT ReqAllianceBattle ( DWORD dwGaeaID, DWORD dwTime );

	//	Note : µ¿¸Í ¹èÆ² ¿äÃ» ´äº¯.
	HRESULT ReqAllianceBattleAns ( DWORD dwChiefCharID, bool bOK );

	//	Note : µ¿¸Í ¹èÆ² ÈÞÀü ¿äÃ».
	HRESULT ReqAllianceBattleArmistice( DWORD dwCLUBID );

	//	Note : µ¿¸Í ¹èÆ² ÈÞÀü ´äº¯.
	HRESULT ReqAllianceBattleArmisticeAns( DWORD dwCLUBID, bool bOK );

	//	Note : µ¿¸Í ¹èÆ² Ç×º¹ ¿äÃ».
	HRESULT ReqAllianceBattleSubmission ( DWORD dwCLUBID );	


public:
	//	Note : Å¬·´ Ã¢°í Á¤º¸¸¦ ¼­¹ö¿¡ ¿äÃ».
	HRESULT ReqGetClubStorage ();
	
	//	Note : Å¬·´ Ã¢°í ¾ÆÀÌÅÛ µé¶§, ³õÀ»¶§, ±³È¯ÇÒ¶§, ÇÕÄ¥¶§.
	HRESULT ReqClubStorageTo ( DWORD dwChannel, WORD wPosX, WORD wPosY );

	//	Note : Å¬·´ Ã¢°í °ãÄ§ ¾ÆÀÌÅÛ ºÐ¸®.
	HRESULT ReqClubStorageSplit ( DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wSplitNum );

	//	Note : Å¬·´ Ã¢°í µ· ³Ö±â.
	HRESULT ReqClubStorageSaveMoney ( LONGLONG lnMoney );

	//	Note : Å¬·´ Ã¢°í µ· »©³»±â.
	HRESULT ReqClubStorageDrawMoney ( LONGLONG lnMoney );
	HRESULT ReqKillFeedWindowOpen(WORD wPosX, WORD wPosY);

	//	Note : Ãâ¼® ¸®½ºÆ® ¿äÃ»
	HRESULT	ReqAttendList( bool bDay = false );

	//	Note : Ãâ¼® ¿äÃ»
	HRESULT	ReqAttendance();

	HRESULT ReqGathering( const STARGETID& sTargetID );
	HRESULT ReqCancelGathering();

public:
	//	Note : Ä³¸¯ÅÍÀÇ ÀüÈ­¹øÈ£ ÀúÀå / ¾÷µ¥ÀÌÆ®
	HRESULT ReqSetPhoneNumber ( const TCHAR * szPhoneNumber );
	HRESULT ReqSendSMS( int nCharID, const TCHAR * szPhoneNumber, const TCHAR * szSmsMsg );

public:
	//	Note : ¹Ì´Ï °ÔÀÓ - È¦Â¦
	HRESULT ReqMGameOddEvenBatting( UINT uiBattingMoney );
	HRESULT ReqMGameOddEvenAgain();
	HRESULT ReqMGameOddEvenCancel(); 
	HRESULT ReqMGameOddEvenSelect( BOOL bOdd );
	HRESULT ReqMGameOddEvenShuffle(); 
	HRESULT ReqMGameOddEvenFinish();

public:
	//	Note : ºÎÈ° ½ºÅ³ ±ÝÁö À¯¹«
	void	ReqNonRebirth( BOOL bNonRebirth );
	//  Note : ÆÄÆ¼¿ø¿¡°Ô Qbox ¿É¼Ç »óÅÂ¸¦ º¸³½´Ù.
	void    ReqQBoxEnableState( bool bQboxEnable );

	/*charinfoview , Juver, 2017/11/12 */
	void	ReqPrivateStats( BOOL bPrivateStats );
	void	ReqDisTrade();

	/*dmk14 large map window ep9*/
	void	ReqRetrieveMobInMap( SNATIVEID sMapID );

public:
	HRESULT ReqRebuildOpen();								// °³Á¶Ã¢ ¿­±â	// ITEMREBUILD_MARK
	HRESULT ReqRebuildMoveItem();							// °³Á¶Ã¢°ú ¼Õ »çÀÌÀÇ ¾ÆÀÌÅÛ ÀÌµ¿ ¿äÃ»
	HRESULT ReqRebuildInputMoney( LONGLONG i64InputMoney );	// °³Á¶Ã¢¿¡ ÀÔ·ÂÇÑ ±Ý¾× ¼­¹ö¿¡ Àü¼Û
	/*dmk14 reform*/
	HRESULT ReqRebuildResult();
	HRESULT ReqRebuildPREV();
	HRESULT ReqRebuildMoveItemSealed();

	HRESULT ReqRebuildClose();								// °³Á¶Ã¢ ´Ý±â
	HRESULT ReqRebuildMoveSeal();
public:
	HRESULT ReqGarbageOpen();								// ÈÞÁöÅë ¿­±â
	HRESULT ReqGarbageMoveItem();							// ÈÞÁöÅë¿¡ ¾ÆÀÌÅÛ ÀÌµ¿ ¿äÃ»
	HRESULT ReqGarbageResult();								// ¾ÆÀÌÅÛ ÆÄ¼â ¿äÃ»
	HRESULT ReqGarbageClose();								// ÈÞÁöÅë ´Ý±â

	const SITEMCUSTOM& GET_GARBAGE_ITEM();

	VOID	InitGarbageData();
	VOID	OpenGarbage()							{ m_bGarbageOpen = true; }
	VOID	CloseGarbage()							{ m_bGarbageOpen = false; }
	bool	ValidGarbageOpen()						{ return m_bGarbageOpen; }

	bool ValidItemMixOpen();
	const SITEMCUSTOM GET_ITEM_MIX( int nIndex );
	void SetItemMixMoveItem( int nIndex );
	void ReSetItemMixItem( int nIndex );
	void ResetItemMix();
	DWORD GetItemMixMoney();

	HRESULT ReqItemMix( DWORD dwNpcID );

	bool ValidWindowOpen();
	//¸Ë³Æ¤É¯Å
public:
	HRESULT ReqZbsjOpen();								// °³Á¶Ã¢ ¿­±â	// ITEMREBUILD_MARK
	HRESULT ReqZbsjMoveItem();							// °³Á¶Ã¢°ú ¼Õ »çÀÌÀÇ ¾ÆÀÌÅÛ ÀÌµ¿ ¿äÃ»
	HRESULT ReqZbsjResult(int nIndex);								// °³Á¶ ¿äÃ»
	HRESULT ReqZbsjClose();								// °³Á¶Ã¢ ´Ý±â
	//END

public:
	HRESULT ReqWeaponReform(WORD wPosX, WORD wPosY); //Weapon Skill Reform Card - SqueeeAK
	// PET
	HRESULT ReqUsePetCard ( WORD wPosX, WORD wPosY );
	HRESULT ReqReGenPet ();
	HRESULT	ReqPetCardInfo ();
	HRESULT ReqPetReviveInfo ();
	HRESULT ReqPetRevive ( DWORD dwPetID );
	void ReqRankName(BOOL bRankName); //MMR Rank
	void ReqRankMark(BOOL bRankMark); //MMR Rank

public:
	void ReqReBirth ();						//	Note : ºÎÈ° ¿äÃ».
	void ReqLevelUp ();						//	Note : ·¾¾÷ ¿äÃ».
	void ReqStatsUp ( SCHARSTATS sStats );
	void ReqStatsUp ( EMSTATS emStats );	//	Note : ½ºÅÝ ºÐ¹è ¿äÃ».
	void ReqStatsUpCmd ( EMSTATS emStats, DWORD value);	//add addstats cmd


	/*dmk14 right click teleport*/
	void ReqTeleportOnMap( INT posX, INT posY );
	//Âà¥Í¨t²Î
	HRESULT ReqUpRell();

public:

	HRESULT SetVehicle ( bool bActive );
	HRESULT ReqSetVehicle( bool bActive );	// Å»°Í È°¼ºÈ­/ºñÈ°¼ºÈ­
	HRESULT ReqVehicleUpdate(); // Å»°Í ÀåÂø½Ã Á¤º¸ ¿äÃ»
	void	ReqVehicleChangeAccessory( EMSUIT emSUIT ); // Å»°Í ÀåÂø ¾ÆÀÌÅÛ ±³Ã¼
	void	ReqVehicleRemoveSlotItem( EMSUIT emSUIT );	 // Å»°Í ÀåÂø ¾ÆÀÌÅÛ ÇØÁ¦
	void	ReqVehicleGiveBattery ( WORD wPosX, WORD wPosY );	// Å»°Í ¿¬·á Ã¤¿ì±â
	void	ReqVehicleInvenUpdate();	// Å»°Í Á¤º¸ ¿äÃ»

	/*vehicle booster system, Juver, 2017/08/10 */
	void	ReqVehicleEnableBooster ( WORD wPosX, WORD wPosY ); 

	/*bike color , Juver, 2017/11/12 */
	void	ReqVehicleColor ( WORD wPosX, WORD wPosY ); 

	/* car, cart color, Juver, 2018/02/14 */
	void	ReqCarColor ( WORD wPosX, WORD wPosY ); 
	void	CarColorEnd();
	void	CarColorUpdate();
	void	CarColorChange();

	/* booster all vehicle, Juver, 2018/02/14 */
	void	ReqAllVehicleEnableBooster ( WORD wPosX, WORD wPosY ); 

public:
	void	ReqItemShopOpen( bool bOpen );		// ItemShop Open/Close Åëº¸

public:
	HRESULT DoActWait ();
	HRESULT ReBirth ( DWORD dwHP, DWORD dwMP, DWORD dwSP, D3DXVECTOR3 vPos, bool bWait );
	HRESULT MoveActiveMap ( NavigationMesh* pNavi, const D3DXVECTOR3 &vPos, bool bWait=true );
	HRESULT PlayerUpdate ( float fTime, float fElapsedTime );
	
	HRESULT ReqFriendWindowOpen( bool bOpen );
	
	HRESULT ReqClubInfoUpdate( bool bUpdate );

protected:
	void ReSelectAnimation ();
	HRESULT UpateAnimation ( float fTime, float fElapsedTime );
	HRESULT UpdateClientState ( GLMSG::SNETPC_UPDATE_STATE *pNetMsg );

	void NewConftBoundEffect ( const EMCONFT_TYPE emCONFT );
	void DelConftBoundEffect ();

protected:
	STARGETID	m_sOLD_TARMOB;
	SACTIONTAR	m_sACTAR;

public:
	SACTIONTAR& GetActTarget ()		{	return m_sACTAR; }
	bool IsPK_TAR ( GLCharClient* pCHAR, bool bFORCED );
	bool IsClubBattle_TAR ( GLCharClient* pCHAR );
	int	GetClubColorIndex( GLCharClient* pCHAR );

protected:
	STARGETID* GetCONFT_TAR ( DETECTMAP_RANGE &pair );

protected:
	SACTIONTAR FindActionTarget ( const D3DXVECTOR3 &vTargetPt, const D3DXVECTOR3 &vFromPt, const EMFIND_TAR emFINDTAR );
	SACTIONTAR FindNearItem ();

protected:
	void StartAttackProc ();
	BOOL AttackProc ( float fElapsedTime );
	void AttackEffect ( const SANIMSTRIKE &sStrikeEff );

protected:
	void StartSkillProc ();
	BOOL SkillProc ( float fElapsedTime );

	EMELEMENT GET_ITEM_ELMT ();

	void SKT_EFF_HOLDOUT ( STARGETID sTarget, DWORD dwDamageFlag );

	void SK_EFF_TARG ( const PGLSKILL pSkill, const SANIMSTRIKE &sStrikeEff, const STARGETID &sTarget );

	/*skill activated effect, Juver, 2017/12/29 */
	DxEffSingleGroup* SK_EFF_SELFZONE ( const SANIMSTRIKE &sStrikeEff, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF, STARGETID *pTarget=NULL );
	DxEffSingleGroup* SK_EFF_TARZONE ( const STARGETID &sTarget, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF );

	void SkillEffect ( const SANIMSTRIKE &sStrikeEff );

protected:
	//	ÀÚ±âÆí¿¡°Ô ½ºÅ³ Ã¼Å© ( ÀÚ±â À§Ä¡¿¡ ¿µÇâ )
	BOOL SKT_SELFZONEOUR ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//  Àûµé¿¡°Ô ½ºÅ³ Ã¼Å© ( ÀÚ±â À§Ä¡¿¡ ¿µÇâ )
	BOOL SKT_SELFZONEENERMY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//  ¸ðµÎ¿¡°Ô ½ºÅ³ Ã¼Å© ( ÀÚ±â À§Ä¡¿¡ ¿µÇâ )
	BOOL SKT_SELFZONANYBODY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	ÀÚ±âÆí¿¡°Ô ½ºÅ³ Ã¼Å© ( ´ë»ó À§Ä¡¿¡ ¿µÇâ )ÀÌ°í ÀÚ½Å ÁÖÀ§
	BOOL SKT_TARSPECOUR_SELF ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	Àûµé¿¡°Ô ½ºÅ³ Ã¼Å© ( ´ë»ó À§Ä¡¿¡ ¿µÇâ )ÀÌ°í ÀÚ½Å ÁÖÀ§
	BOOL SKT_TARSPECENERMY_SELF ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	¸ðµÎ¿¡°Ô ½ºÅ³ Ã¼Å© ( ´ë»ó À§Ä¡¿¡ ¿µÇâ )ÀÌ°í ÀÚ½Å ÁÖÀ§
	BOOL SKT_TARSPECANYBODY_SELF ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	ÀÚ±âÆí¿¡°Ô ½ºÅ³ Ã¼Å© ( ´ë»ó À§Ä¡¿¡ ¿µÇâ )ÀÌ°í ¸ñÇ¥ ÁÖÀ§
	BOOL SKT_TARSPECOUR ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	Àûµé¿¡°Ô ½ºÅ³ Ã¼Å© ( ´ë»ó À§Ä¡¿¡ ¿µÇâ )ÀÌ°í ¸ñÇ¥ ÁÖÀ§
	BOOL SKT_TARSPECENERMY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	¸ðµÎ¿¡°Ô ½ºÅ³ Ã¼Å© ( ´ë»ó À§Ä¡¿¡ ¿µÇâ )ÀÌ°í ¸ñÇ¥ ÁÖÀ§
	BOOL SKT_TARSPECANYBODY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	'¸ñÇ¥'¸¦ ÇâÇÑ ºÎÃ¤²Ã ¸ð¾ç.
	BOOL SKT_TARSPECFANWIZE ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	// Àû¿¡°Ô¸¸ ÀÚ±â À§Ä¡¿¡¼­ ´ë»ó À§Ä¡±îÁö. ( È­»ì °üÅë )
	BOOL SKT_TARSELFTOSPEC ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	//	ÀÚ±âÆí¿¡°Ô ½ºÅ³ Ã¼Å© ( »ó´ë¹æ À§Ä¡¿¡ ¿µÇâ )
	BOOL SKT_TARZONEOUR ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	Àûµé¿¡°Ô ½ºÅ³ Ã¼Å© ( »ó´ë¹æ À§Ä¡¿¡ ¿µÇâ )
	BOOL SKT_TARZONEENERMY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );
	//	¸ðµÎ¿¡°Ô ½ºÅ³ Ã¼Å© ( »ó´ë¹æ À§Ä¡¿¡ ¿µÇâ )
	BOOL SKT_TARZONEANYBODY ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos, const GLSKILL &sSKILL, const WORD wLEVEL );

	BOOL SelectSkillTarget ( const STARGETID &sTargetID, const D3DXVECTOR3 *pvTarPos );

protected:
	void P2PReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void PvPReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );

	void MobReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void NpcReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void ItemReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void SkillReaction ( const STARGETID &sTargetID, const DWORD dwMR, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );
	void GatheringReaction ( const STARGETID &sTargetID, const DWORD dwML, const bool bcontinue, BOOL &bMove, D3DXVECTOR3 &vMoveTo );

	void ReservedAction ( float fTime );

public:
	void ReqSkillReaction ( STARGETID sTID );

public:
	HRESULT Create ( NavigationMesh* pNavi, SCHARDATA2 *pCharData2, D3DXVECTOR3* pvPos, DWORD dwGaeaID, LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT SkinLoad ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT SkinLoadForTool ( LPDIRECT3DDEVICEQ pd3dDevice, const char* szFile );

public:
	void ResetData ();
	BOOL			m_bReqSwapArms;			// ±Ø°­ºÎ ¹«±â ±³Ã¼ ÁßÀÌ¶ó¸é
	bool 			IsReqSwapArms() 			{ return m_bReqSwapArms ? true : false; }

	void AutoArmSwap();

public:
	HRESULT FrameMove( float fTime, float fElapsedTime );
	HRESULT FrameMoveForTool( float fTime, float fElapsedTime );
	HRESULT Render( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderShadow( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderReflect( LPDIRECT3DDEVICEQ pd3dDevice );
	void	EventCalculate();
	void	VietnamCalculate();

	void	DisableSkillFact();
	// ÁöÇü È¿°ú ¾÷µ¥ÀÌÆ®
	VOID	UpdateLandEffect();

public:
	SACTIONTAR	m_sACTARTarget;
	SACTIONTAR	m_sACTARTargetCamera;
	void	UpdateTargetEffect();

public:
	BOOL	IsNpcQuest( DWORD dwGlobID );
	void	UpdateNPCQuestEffect();

public:
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT InvalidateDeviceObjects ();
	HRESULT DeleteDeviceObjects ();

public:
	void SetHiddenWeaponPiece();
	void ResetHiddenWeaponPiece();

public:
	/*itemfood system, Juver, 2017/05/25 */
	HRESULT ReqConsumeFood ( WORD wPosX, WORD wPosY ); 
	BOOL	ItemFoodAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT );
	HRESULT ReqUnlockFoodSlot ( WORD wPosX, WORD wPosY ); 

	/*rightclick wear/unwear, Juver, 2017/06/23 */
	HRESULT ReqInvenToWear ( WORD wPosX, WORD wPosY );
	HRESULT ReqWearToInven ( EMSLOT emSLOT );

	/*npc shop, Juver, 2017/07/27 */
	HRESULT ReqNpcShopPurchaseReset();
	HRESULT ReqNpcShopPurchase( SNATIVEID sidCrow, SNATIVEID sidItem, WORD wShopType );
	HRESULT ReqNpcShopDoPurchase();

	/*vehicle booster system, Juver, 2017/08/11 */
	void	VehicleBoosterStart();
	void	VehicleBoosterStateOn();
	void	VehicleBoosterStateOff();

	/*system buffs, Juver, 2017/09/04 */
	BOOL	SystemBuffAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT );

	/*item exchange, Juver, 2017/10/13 */
	HRESULT	ReqNPCItemExchange( SNATIVEID sidNPC, WORD wExchangeID );	

	/*product item, Juver, 2017/10/17 */
	bool	ValidItemCompoundOpen();
	void	ItemCompound( DWORD dwID, int nProductCount );
	void	ItemCompoundCancel();
	HRESULT	ReqItemCompound( DWORD dwID, int nProductCount  );
	DWORD	GetItemCompoundID()	{ return m_dwProductID; }
	int		GetItemCompoundCount() { return m_nProductCount; }
	
	/*activity system, Juver, 2017/11/05 */
	void	ReqChangeBadge( DWORD dwBadgeID );

	/*charinfoview , Juver, 2017/11/11 */
	void	RequestCharacterInfo( DWORD dwGaeaID );

	/*bike color , Juver, 2017/11/13 */
	void	ReqBikeColorChange();
	void	BikeColorUpdate();
	void	BikeColorEnd();

	/*rv card, Juver, 2017/11/25 */
	HRESULT ReqRandomOptionChange ( WORD wPosX, WORD wPosY );

	/*nondrop card, Juver, 2017/11/26 */
	HRESULT ReqItemNonDropCard ( WORD wPosX, WORD wPosY );

	/*regional chat, Juver, 2017/12/06 */
	BOOL	RegionalChatPay();

	/*change scale card, Juver, 2018/01/03 */
	void	ReqChangeScale( WORD wPosX, WORD wPosY );
	void	ChangeScaleCancel();
	void	ChangeScaleSave();

	BOOL	InvenUseToPutOn( EMSLOT emSlot );

	/*item color, Juver, 2018/01/10 */
	void	ReqItemColorChange( EMSLOT emSlot );
	void	ItemColorEnd();
	void	ItemColorUpdate();
	void	ItemColorChange();

	/*item wrapper, Juver, 2018/01/11 */
	void	ReqItemWrap( WORD wPosX, WORD wPosY );
	void	ReqItemUnwrap( WORD wPosX, WORD wPosY );
	void	ReqItemWrapMsg();
	void	ReqItemUnwrapMsg();

	/*change school card, Juver, 2018/01/12 */
	void	ReqChangeSchool( WORD wPosX, WORD wPosY );
	void	ReqChangeSchoolMsg( WORD wSelected );

	///*equipment lock, Juver, 2018/01/13 */
	void	ReqEnableEquipmentLock( WORD wPosX, WORD wPosY );
	void	ReqEnableEquipmentLockMsg( std::string strKey1, std::string strKey2, std::string strAgree );
	void	ReqEnableEquipmentLockInputMsg( std::string strKey );
	void	ReqRecoverEquipmentLockPassword( WORD wPosX, WORD wPosY );
	void	ReqRecoverEquipmentLockChange( std::string strKey1, std::string strKey2, std::string strAgree );
	void	ReqRecoverEquipmentLockDelete( std::string strAgree );

	///*storage lock, Juver, 2018/01/13 */
	void	ReqEnableStorageLock( WORD wPosX, WORD wPosY );
	void	ReqEnableStorageLockMsg( std::string strKey1, std::string strKey2, std::string strAgree );
	void	ReqEnableStorageLockInputMsg( std::string strKey );
	void	ReqRecoverStorageLockPassword( WORD wPosX, WORD wPosY );
	void	ReqRecoverStorageLockChange( std::string strKey1, std::string strKey2, std::string strAgree );
	void	ReqRecoverStorageLockDelete( std::string strAgree );

	///*inventory lock, Juver, 2018/01/13 */
	void	ReqEnableInventoryLock( WORD wPosX, WORD wPosY );
	void	ReqEnableInventoryLockMsg( std::string strKey1, std::string strKey2, std::string strAgree );
	void	ReqEnableInventoryLockInputMsg( std::string strKey );
	void	ReqRecoverInventoryLockPassword( WORD wPosX, WORD wPosY );
	void	ReqRecoverInventoryLockChange( std::string strKey1, std::string strKey2, std::string strAgree );
	void	ReqRecoverInventoryLockDelete( std::string strAgree );

	/*item transfer card, Juver, 2018/01/18 */
	void	ReqItemTransferOpen( WORD wPosX, WORD wPosY );
	void	SetItemTransferMoveItemOld();
	void	SetItemTransferMoveItemNew();
	void	ReSetItemTransferItemOld();
	void	ReSetItemTransferItemNew();
	void	ResetItemTransferItem();
	const SITEMCUSTOM GetItemTransferItemOld();
	const SITEMCUSTOM GetItemTransferItemNew();
	bool	ValidItemTransferOpen();
	void	ItemTransferStart();

	/*pvp capture the flag, Juver, 2018/02/08 */
	void	CaptureTheFlagLocatorUpdate();

	/* Selective Box, Baby011528, 2022-02-15 */
private:
	int		m_nSelFormBoxPosX;
	int		m_nSelFormBoxPosY;

public:
	HRESULT	SelectiveformBoxOpen(WORD wPosX, WORD wPosY);
	HRESULT ReqSelformBoxSelect(WORD wSelItemPos);
	//
	
	/*dmk14 remove active buff selection*/
	void	ReqDisableSkill( DWORD dwSKILL, SNATIVEID sKILLID );
	void	ReqDisableItemFact( DWORD dwSKILL, SNATIVEID sKILLID );

	/*PartySearch - Added By Bubu*/
	HRESULT ReqPartyFinderInfo();
	HRESULT ReqPartySearchJoin(DWORD dwGaeaID, DWORD dwPartyID );

	/*Codex System, Jhoniex 2/14/2024 */
	void ReqCodexRegister( SNATIVEID sItemID, DWORD m_dwActivityID, WORD wGrade, WORD wQuantity );


	// LG-7 GlobalRanking
public:
	HRESULT ReqGlobalRanking();
	/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// LG-7 CWRanking
public:
	HRESULT ReqCWRanking();
	HRESULT ReqMyCWRanking();
/////////////////////////////////////////////////////////////////////////////

public:
	GLCharacter ();
	~GLCharacter ();
};
