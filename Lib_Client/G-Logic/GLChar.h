#pragma once


//#include <boost/tr1/memory.hpp>
//#include <boost/function.hpp>
#include <memory>
#include <functional>
#include <map>
#include <deque>

//#include <boost/serialization/shared_ptr.hpp>

#include "./GLACTOR.h"
#include "./GLogicEx.h"
#include "./GLARoundSlot.h"
#include "./GLTrade.h"
#include "./GLPrivateMarket.h"


#include "../../Lib_Engine/NaviMesh/Actor.h"

/*skill summon, Juver, 2017/10/08 */
#include "GLSummonData.h"

/*pet skill check, Juver, 2017/12/01 */
#define PET_SKILL_CHECK_TIMER 10.0f

/*qbox check, Juver, 2017/12/05 */
#define QUESTION_BOX_CHECK_TIMER 15.0f

/*equipment lock, Juver, 2018/01/16 */
#define EQUIPMENT_LOCK_TIMER 5.0f

/*storage lock, Juver, 2018/01/16 */
#define STORAGE_LOCK_TIMER 5.0f

/*inventory lock, Juver, 2018/01/16 */
#define INVENTORY_LOCK_TIMER 5.0f

#define ITEM_REBUILD_TIMER 0.5f

#define INVEN_SORT_TIMER 5.0f

enum GAIN_TYPE{ GAINTYPE_MAX, GAINTYPE_HALF, GAINTYPE_EMPTY };

#ifndef GLLandMan
	class GLLandMan;
	class GLLandNode;
	typedef SQUADNODE<GLLandNode> LANDQUADNODE;
#endif //GLLandMan

struct SFIELDCROW;
typedef SFIELDCROW* PSFIELDCROW;
struct GLPARTY_FIELD;
class GLQUEST;
class GLGaeaServer;
struct SSKILLACTEX;
struct SSKILLACT;

//	Note : Ä³¸¯ÅÍ.
//
class GLChar : public GLCHARLOGIC, public GLACTOR
{
private:
	//	µ¥¹ÌÁö Å¥
	struct SSTRIKE
	{
		WORD wDivKey;
		bool bHit;
		bool bLowSP;

		SSTRIKE () : wDivKey(0), bHit(false), bLowSP(false) { }
		SSTRIKE ( WORD _wDivKey, bool _bHit, bool _bLowSP ) : wDivKey(_wDivKey), bHit(_bHit), bLowSP(_bLowSP) { }
	};
	typedef std::deque<SSTRIKE>	HITLIST;
	typedef HITLIST::iterator	HITLIST_ITER;

	typedef CGLLIST<SFIELDCROW*> FIELDCROWLIST;
	typedef SGLNODE<SFIELDCROW*> FIELDCROWNODE;

	typedef std::vector<SSEARCHITEMRESULT> VECSEARCH_RESULT;
	typedef VECSEARCH_RESULT::iterator	  VECSEARCH_RESULT_ITER;

public:
	Actor				m_actorMove;

	D3DXVECTOR3			m_vDir;

protected:
	STARGETID			m_TargetID;
	STARGETID			m_sNPCID;				// ÃÖ±Ù¿¡ ¼±ÅÃµÈ NPC
	WORD				m_wTARNUM;
	STARID				m_sTARIDS[EMTARGET_NET];

	DWORD				m_dwANISUBSELECT;

	std::set<int>		m_setLandEffectNum;

protected:
	EMACTIONTYPE		m_Action;				//	ÇöÁ¦ Çàµ¿.
	DWORD				m_dwActState;			//	ÇöÁ¦ Çàµ¿ ¼³Á¤.

	BOOL				m_bSpam;

protected:
	float				m_fIdleTime;			//	ÈÞ½Ä ½Ã°£.
	float				m_fMBR_TIMER;			//	ÆÄÆ¼ ¸â¹ö¿¡°Ô º¸³»´Â °»½Å ½Ã°£.
	float				m_fSTATE_TIMER;			//	»óÅÂ Á¤º¸ °»½Å ½Ã°£.
	float				m_fPERIOD_TIMER;		//	Period ½Ã°£ °»½Å.
	float				m_fQUEST_TIMER;			//	Äù½ºÆ® Á¤º¸ °»½Å.
	float				m_fattTIMER;			//	°ø°Ý ÇÁ·¡ÀÓ.
	float				m_fMoveDelay;			//	ÀÌµ¿ µô·¡ÀÌ.
	float				m_fITEM_TIMER;			//	¾ÆÀÌÅÛ Á¦ÇÑ½Ã°£ Á¡°Ë.
	float				m_fSAVEDB_TIMER;		//	DB ÀúÀå ½Ã°£.
	float				m_fBRIGHT_TIMER;		//	¼Ó¼º¼öÄ¡ °»½Å ½Ã°£.
	float				m_fSkillDelay;			//  ½ºÅ³ µô·¹ÀÌ (¿ÀÅä°ø°Ý ¹æÁö¿ë)

protected:
	STARGETID			m_sAssault;				//	°¡Àå ÃÖ±Ù¿¡ ÀÚ½ÅÀ» °ø°ÝÇÑÀÚ.
	LONGLONG			m_lnLastSendExp;
	LONGLONG			m_lnLastSendVNExp;		//  ¸¶Áö¸·¿¡ º¸³½ º£Æ®³² ´©Àû °æÇèÄ¡

	WORD				m_ItemRemodelPosX;		//	°³Á¶Ä«µå »ç¿ë½Ã ¾ÆÀÌÅÛ À§Ä¡ ÀúÀå
	WORD				m_ItemRemodelPosY;		

	/*dmk14 reform*/
	SNATIVEID			m_sItemRemodel;
	SNATIVEID			m_sItemSealed;
	bool				m_bIsReformProcess;
	float				m_fReformProcessTimer;

	WORD				m_ItemGarbagePosX;		//	ÈÞÁöÅë »ç¿ë½Ã ¾ÆÀÌÅÛ À§Ä¡ ÀúÀå
	WORD				m_ItemGarbagePosY;
	//¸Ë³Æ¤É¯Å
	WORD				m_ItemZbsjPosX;
	WORD				m_ItemZbsjPosY;
	//END

protected:
	GLARoundSlot		m_ARoundSlot;			//	ÇÇ°ø°Ý½Ã¿¡ ÀÚ½ÅÀÇ ÁÖº¯¿¡ ¸ô·Áµé¼ö ÀÖ´Â À§Ä¡°¡ ÀÖ´ÂÁö Á¡°Ë.

	DAMAGELOG			m_cDamageLog;			//	´ë¹ÌÁö ·Î±×.

	WORD				m_wHealingNum;			//	Èú¸µ ¼ö.

	bool				m_bUseRebuildCard;
public:
	DWORD				m_dwViewFrame;			//	ÁÖÀ§È¯°æ ¾÷µ¥ÀÌÆ® ÇÁ·¹ÀÓ.
	float				m_fAge;					//	°æ°ú ½Ã°£.
	float				m_fGenAge;				//	ºÎÈ°ÈÄ ½Ã°£.
	__int64				m_nDECEXP;				//	»ç¸ÁÈÄ ºÎÈ°½Ã °æÇèÄ¡ °¨¼Ò ¼öÄ¡.
	D3DXVECTOR3			m_vPos;

	DWORD				m_dwClientID;			//	Å¬¶óÀÌ¾ðÆ® ID.
	DWORD				m_dwGaeaID;				//	»ý¼º ¸Þ¸ð¸® ÀÎµ¦½º¿ë.
	DWORD				m_dwPartyID;			//	ÆÄÆ¼ ID.

	SNATIVEID			m_sMapID;				//	¸Ê ID.
	SNATIVEID			m_sOldMapID;			//  ÀüÀÇ ¸Ê ID. 
	DWORD				m_dwCeID;				//	¼¿ ID.
	
	GLLandMan*			m_pLandMan;				//	Á¸ÀçÇÏ´Â Land.

	virtual GLLandMan*	GetLandMan() { return m_pLandMan; }
	virtual bool	 	MoveTo( const D3DXVECTOR3* pTarPos, bool bJump, DWORD dwFlags );
	SGLNODE<GLChar*>*	m_pGaeaNode;			//	Àü¿ª ¸®½ºÆ® ³ëµå.
	SGLNODE<GLChar*>*	m_pLandNode;			//	·£µå ¸®½ºÆ® ³ëµå.

	LANDQUADNODE*		m_pQuadNode;			//	Äõµå Æ®¸® ³ëµå. ( Á÷Á¢ Æ÷ÀÎÅÍ ÇüÀ» ÁöÁ¤ÇÏ±â ³­°¨ÇÔ. )
	SGLNODE<GLChar*>*	m_pCellNode;			//	¼¿ ¸®½ºÆ® ³ëµå.

	GLTrade				m_sTrade;				//	°Å·¡ Á¤º¸.
	GLPrivateMarket		m_sPMarket;				//	°³ÀÎ »óÁ¡.

	SCONFTING			m_sCONFTING;			//	ÁøÇà ´ë·Ã Á¤º¸.

	VECSEARCH_RESULT	m_vecSearchResult;			//  Ã£Àº ¾ÆÀÌÅÛµé 

	/*activity system, Juver, 2017/11/02 */
	SNATIVEID			m_sActivityCheckMapID;

private:
	void StigmaProc_Skill(const SSKILLACT* pSkillAct, BOOL bStigmaEffect = FALSE);
	void StigmaProc_Normal(const bool bBroad);
	void StigmaProc_SendMessage(const STARGETID& sTargetID, const float fRate, const bool bBroad);	

public:
	bool				m_bUsedStorageCard;		// ±ä±ÞÃ¢°í¿¬°áÄ«µå »ç¿ë¿©ºÎ
	BOOL				m_bNon_Rebirth;			// ºÎÈ° ½ºÅ³ ±ÝÁö ¿©ºÎ
	BOOL				m_bPrivateStats;		/*charinfoview , Juver, 2017/11/12 */
	BOOL				m_bDisTrade;

	// PET
	DWORD				m_dwPetGUID;			// ÆÖ¸Þ¸ð¸®ÀÎµ¦½º
	DWORD				m_dwPetID;				// ÆÖ°íÀ¯¾ÆÀÌµð
	bool				m_bProtectPutOnItem;	// ¾ÆÀÌÅÛ ¶³¾îÁö´Â°ÍÀ» ¸·À»Áö ¿©ºÎ
	bool				m_bGetPetFromDB;		// DB¿¡¼­ ÆÖÀÇ Á¤º¸¸¦ °¡Á®¿À´Â ÁßÀÎÁö...

	// Summon
	/*skill summon, Juver, 2017/10/09 */
	DWORD				m_dwSummonGUID_FLD[SKILL_SUMMON_MAX_CLIENT_NUM];			// ¼ÒÈ¯¼ö¸Þ¸ð¸®ÀÎµ¦½º
	float				m_fSummonCheckTime;

	
	bool				m_bEntryFailed;			// ÁøÀÔ½ÇÆÐ
	

	/*int					m_dwThaiCCafeClass;
	SChinaTime			m_sChinaTime;
	SEventTime			m_sEventTime;*/

	bool				m_bEventStart;
	bool				m_bEventApply;

	BYTE				m_ChinaGainType;	    // Áß±¹ Å½´Ð¹æÁö ½Ã½ºÅÛ 255: default, 0: 1½Ã°£ Áö³µÀ½, 1: ¼öÀÍ 50%, 2: ¼öÀÍ 0%
	CTime				m_China15Time;			// Áß±¹ Å½´Ð¹æÁö ½Ã½ºÅÛ  15ºÐ ¸¶´Ù º¸³»Áö´Â ½Ã°£

	GAIN_TYPE			m_dwVietnamGainType;	    // º£Æ®³² Å½´Ð¹æÁö ½Ã½ºÅÛ  255: default, 0: 3½Ã°£ Áö³µÀ½ ¼öÀÍ 50%, 1: 0%

	//CTime				m_China1Hour;			// 1½Ã°£ Áö³µÀ» ¶§ È£ÃâµÊ

	GLVEHICLE			m_sVehicle;				// Å»°Í ±¸Á¶Ã¼
	BOOL				m_bGetVehicleFromDB;	// DB¿¡¼­ °¡Á®¿À´Â Áß
	float				m_fVehicleTimer;

	bool				m_bItemShopOpen;		// ¾ÆÀÌÅÛ¼¥ ÀÌ¿ëÁßÀÎÁö È®ÀÎ ( Japan )
	bool				m_bDefenseSkill;		// ¹ßµ¿½ºÅ³ÀÌ Á¤»óÀûÀÎÁö È®ÀÎ

	
	WORD				m_ItemSealPosX;		//	°³Á¶Ä«µå »ç¿ë½Ã ¾ÆÀÌÅÛ À§Ä¡ ÀúÀå
	WORD				m_ItemSealPosY;		

	/*continuous damage skill logic, Juver, 2017/06/10 */
	float				m_fContinuousDamageCheck;

	/*contribution/activity bonus, Juver, 2017/10/14 */
	BOOL				m_bUseContributionPointTimer;
	float				m_fContributionPointBonusTimer;
	float				m_fContributionPointBonusTimeMax;

	BOOL				m_bUseActivityPointTimer;
	float				m_fActivityPointBonusTimer;
	float				m_fActivityPointBonusTimeMax;

	BOOL				m_bInstanceDropOut;
	float				m_fInstanceDropTimer;

	/*product item, Juver, 2017/10/18 */
	BOOL				m_bItemCompoundTask;
	DWORD				m_dwItemCompoundTaskID;
	float				m_fItemCompoundTaskTime;
	float				m_fItemCompoundTaskTimer;
	int					m_nProductMake;
	int					m_nProductCounter;

	GLDWDATA			m_sFreezeCheckHP;
	WORD				m_wFreezeCount;
	WORD				m_wObviousCount;
	BOOL				m_bDeathTimer;
	float				m_fDeathTimer;
	/* HPFreeze DMK14 */
	float				m_fPotsDelay;
	BOOL				m_bPotsDelay;

	BOOL				m_bHoverConfirm;


	/*charinfoview , Juver, 2017/11/11 */
	float				m_fReqCharInfoTimer;

	/*pet skill check, Juver, 2017/12/01 */
	float				m_fPetSkillCheckTimer;

	/*qbox check, Juver, 2017/12/05 */
	float				m_fQBoxCheckTimer;

	/*clubwar verify non push or pull, Juver, 2017/12/13 */
	BOOL				m_bClubWarVerify;
	float				m_fClubWarVerifyTimer;

	/*equipment lock, Juver, 2018/01/16 */
	float				m_fEquipmentLockTimer;

	/*storage lock, Juver, 2018/01/16 */
	float				m_fStorageLockTimer;

	/*inventory lock, Juver, 2018/01/16 */
	float				m_fInventoryLockTimer;

	//cndev
	float				m_fItemRebuildTimer;

	float				m_fInvenSortTimer;

	/* tyranny anti-afk, SeiferXIII 2020/05/17 */
	float				m_fAntiSafeZoneAFK;
	float				m_fTWAntiAFK;
	float				m_fDeadAFK;

	/*pvp capture the flag, Juver, 2018/02/06 */
	float				m_fCaptureTheFlagDetectGateTimer;
	float				m_fCaptureTheFlagHoldFlagTimer;

	SITEMCUSTOM			m_sItemCustomRebuild;

	/*dmk14 chat limit*/
	bool				m_bChatProc;
	float				m_fChatTimerProc;
	int					m_wChatCount;

	HMODULE m_hCheckStrDLL;
	BOOL (_stdcall *m_pCheckString)(CString);

protected:
	// ¹Ì´Ï°ÔÀÓ - È¦Â¦
	WORD				m_wMGOddEvenCurRnd;		// È¦Â¦ °ÔÀÓÀÇ ÇöÀç ¶ó¿îµå
	ULONGLONG			m_ui64DividendMoney;	// Áö±ÞÇØ¾ßÇÒ ¹è´ç±Ý
	ULONGLONG			m_ui64BattingMoney;		// ¹èÆÃÇÑ ±Ý¾×
	INT					m_nOddEven;				// »ç¿ëÀÚ°¡ ¼±ÅÃÇÑ È¦(1) ¶Ç´Â Â¦(0)

	HRESULT MGOddEvenOK( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg );
	HRESULT MGOddEvenCANCEL( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg );
	HRESULT MGOddEvenSelect( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg );
	HRESULT MGOddEvenAGAIN( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg );
	HRESULT MGOddEvenShuffle( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg );
	HRESULT MGOddEvenFinish( GLMSG::SNETPC_MGAME_ODDEVEN* pNetMsg );

	void MGOddEvenInit()
	{
		m_wMGOddEvenCurRnd = 0;
        m_ui64DividendMoney = 0;
		m_ui64BattingMoney = 0;
		m_nOddEven = 0;
	}

private:
	PSFIELDCROW*	m_arrayFieldPC;
	PSFIELDCROW		m_arrayFieldCROW[MAXCROW];
	PSFIELDCROW		m_arrayFieldMATERIAL[MAXCROW];
	PSFIELDCROW		m_arrayFieldITEM[MAXITEM];
	PSFIELDCROW		m_arrayFieldMONEY[MAXMONEY];

	// PET
	PSFIELDCROW*	m_arrayFieldPET;
	PSFIELDCROW*	m_arrayFieldSummon;

	FIELDCROWLIST	m_arrayPC_FIELDLIST;
	FIELDCROWLIST	m_arrayFIELDLIST;

	void SETFIELDPC ( PSFIELDCROW pFIELDCROW );
	void SETFIELDCROW ( PSFIELDCROW pFIELDCROW );
	void SETFIELDMATERIAL ( PSFIELDCROW pFIELDCROW );
	void SETFIELDITEM ( PSFIELDCROW pFIELDCROW );
	void SETFIELDMONEY ( PSFIELDCROW pFIELDCROW );

	void RESETFIELDPC ( DWORD dwID );
	void RESETFIELDCROW ( DWORD dwID );
	void RESETFIELDMATERIAL ( DWORD dwID );
	void RESETFIELDITEM ( DWORD dwID );
	void RESETFIELDMONEY ( DWORD dwID );

	// PET
	void SETFIELDPET ( PSFIELDCROW pFIELDCROW );
	void RESETFIELDPET ( DWORD dwID );

	/*skill summon, Juver, 2017/10/09 */
	void SETFIELDSUMMON ( PSFIELDCROW pFIELDCROW );
	void RESETFIELDSUMMON ( DWORD dwID );

public:
	/*skill summon, Juver, 2017/10/09 */
	WORD GetFreeSummon();
	BOOL isMySummon( DWORD dwID );
	void ReTargetSummon( STARID sid_target );

	bool m_bGMCamera;
	bool IsGMCamera() const { return m_bGMCamera; }

private:
	HITLIST						m_sHITARRAY;
	EMANI_MAINTYPE				m_SKILLMTYPE;
	EMANI_SUBTYPE				m_SKILLSTYPE;

protected:
	GLGaeaServer*				m_pGLGaeaServer;

public:
	SCHARDATA2& GetCharData2 ()				{ return *this; }

	GLARoundSlot& GetARoundSlot ()			{ return m_ARoundSlot; }
	BOOL IsFreeARSlot ()					{ return m_ARoundSlot.GetAmount()!=GLARoundSlot::SLOT_SIZE; }

	void SetGLGaeaServer( GLGaeaServer *pGLGaeaServer ) { m_pGLGaeaServer = pGLGaeaServer; }

	GLGaeaServer* GetGLGaeaServer() { return m_pGLGaeaServer; }

public:
	void RESET_DATA ();
	void RESET_TIMELMT_ITEM ();
	void RESET_CHECK_ITEM ();

	void CHECK_PVP_ITEM();

public:
	void SET_DECEXP ( __int64 nDECEXP ) { m_nDECEXP = nDECEXP; }

public:
	HRESULT CreateChar ( GLLandMan* pLandMan, D3DXVECTOR3 &vPos, PCHARDATA2 pCharData, LPDIRECT3DDEVICEQ pd3dDevice, BOOL bNEW );
	HRESULT SetNavi ( NavigationMesh* pNavi, D3DXVECTOR3 &vPos );
	HRESULT SetData ( PCHARDATA2 pCharData, BOOL bNEW );
	HRESULT SetPosition ( D3DXVECTOR3 &vPos );


	bool		ApplySkillSelfTeleport(GLGaeaServer* const pGaeaServer, GLACTOR* pActor, PGLSKILL pSkill, const WORD wSKILL_LVL, const SKILL::SSPEC& sSKILL_SPEC, const STARGETID& TargetID, const WORD& wTARNUM, const STARID* sTARIDS );
	bool		ApplySkillSelfSwapPos( GLACTOR* pActor, PGLSKILL pSkill, const WORD wSKILL_LVL, const SKILL::SSPEC& sSKILL_SPEC, const WORD& wTARNUM, const STARID* sTARIDS, GLLandMan* pLandMan );
	bool 		ApplySkillTargetOnward( GLACTOR* pActor, GLACTOR* pTarget, const STARGETID& sSelectTARID, const SKILL::SSPEC& sSKILL_SPEC, SSKILLACTEX& sSKILLACTEX );
	void		ApplySkillTargetStun( GLACTOR* pActor );	

	HRESULT SavePosition ();
	HRESULT SaveLastCall ();

	HRESULT LoadStorageFromDB ();
	
public:
	BOOL IsSTATE ( DWORD dwState ) const		{ return m_dwActState&dwState; }
	
	DWORD GetSTATE()	{ return m_dwActState; }
	void SetSTATE ( DWORD dwState )				{ m_dwActState |= dwState; }
	void ReSetSTATE ( DWORD dwState )			{ m_dwActState &= ~dwState; }

	BOOL IsACTION ( EMACTIONTYPE emCur ) const	{ return m_Action == emCur; }
	BOOL IsRunning () const						{ return IsACTION(GLAT_MOVE) && IsSTATE(EM_ACT_RUN); };

	BOOL IsVisibleDetect ( const BOOL bRECVISIBLE );

public:
	BOOL IsVaildTradeData ();
	BOOL IsVaildTradeInvenSpace ( GLInventory &sTradeBox );
	BOOL DoTrade ( GLTrade &sTrade, DWORD dwFromCharID );

public:
	void DelPlayHostile ();
	void DelPlayHostile ( DWORD dwCharID );
	void DelPlayHostileClub( DWORD dwClubID );

protected:
	float GetMoveVelo ();
	void TurnAction ( EMACTIONTYPE toAction );

public:
	void ResetAction() { TurnAction( GLAT_IDLE ); }

public:
	const STARGETID& GetTargetID () const		{ return m_TargetID; }

    GLPARTY_FIELD* GetMyParty(void);
    GLPARTY_FIELD* GetParty(const DWORD dwPartyID);

public:
	void SetPartyID ( DWORD dwPartyID )			{ m_dwPartyID = dwPartyID; }
	void ReSetPartyID ()						{ m_dwPartyID = PARTY_NULL; }

protected:
	DWORD SELECT_SKILLSLOT ( SNATIVEID skill_id );

public:
	DWORD GetGaeaID () const						{ return m_dwGaeaID; }
	virtual EMCROW GetCrow () const					{ return CROW_PC; }
	virtual DWORD GetCtrlID () const				{ return m_dwGaeaID; }
	virtual DWORD GetCharID () const				{ return m_dwCharID; }
	virtual WORD GetSchool () const					{ return m_wSchool; }

	virtual DWORD GetPartyID () const				{ return m_dwPartyID; }

	virtual BOOL IsAction ( EMACTIONTYPE emCur ) const	{ return IsACTION(emCur); }
	virtual BOOL IsDie () const;
	virtual BOOL IsValidBody () const;
	virtual BOOL IsSafeTime () const				{ return m_fMoveDelay < GLCONST_CHAR::fPK_SAFE_TIME; }
	virtual bool IsSafeZone () const				{ return m_bSafeZone; }

	virtual const D3DXVECTOR3& GetPosition () const	{ return m_vPos; }
	virtual WORD GetBodyRadius () const				{ return GETBODYRADIUS(); }
	virtual GLARoundSlot* GETSLOT ()				{ return &m_ARoundSlot; }
	virtual SCONFTING* GETCONFTING ()				{ return &m_sCONFTING; }

	virtual const SRESIST& GETRESIST () const		{ return m_sSUMRESIST_SKILL; }
	virtual DWORD GETHOLDBLOW () const				{ return m_dwHOLDBLOW; }
	virtual WORD GetLevel () const					{ return GETLEVEL(); }

	virtual DWORD GetNowHP () const					{ return GETHP(); }
	virtual DWORD GetMaxHP () const					{ return GETMAXHP(); }
	virtual float GetMaxHpRate() const				{ return GETMAXHPRATE(); }
	virtual void  SetMaxHpRate( float fRate )       {  SETMAXHPRATE( fRate ); }

	virtual WORD GetNowMP () const					{ return GETMP(); };
	virtual WORD GetMaxMP () const					{ return GETMAXMP(); };

	virtual WORD GetNowSP () const					{ return GETMAXSP(); };
	virtual WORD GetMaxSP () const					{ return GETMAXSP(); };

	virtual int GetDefense () const					{ return GETDEFENSE(); };
	virtual int GetBodyDefense () const				{ return m_nDEFENSE_BODY; }
	virtual int GetItemDefense () const				{ return m_sSUMITEM.nDefense; }
	
	virtual float GetDecR_DamageMeleeSkill() const	{ return m_sSUMITEM.fDecR_Damage_MeleeSkill; }
	virtual float GetDecR_DamageRangeSkill() const	{ return m_sSUMITEM.fDecR_Damage_RangeSkill; }
	virtual float GetDecR_DamageMagicSkill() const	{ return m_sSUMITEM.fDecR_Damage_MagicSkill; }

	virtual DWORD GetBonusExp () const;

	virtual int GetAvoid () const					{ return GETAVOID(); }
	virtual EMBRIGHT GetBright () const				{ return GETBRIGHT(); }

	virtual DWORD ReceiveDamage ( const EMCROW emACrow, const DWORD dwAID, const DWORD dwDamage, const BOOL bShock );

	/*push pull skill logic, Juver, 2017/06/04 */
	/*dash skill logic, Juver, 2017/06/17 */
	virtual void ReceivePushPull ( const D3DXVECTOR3 &vMovePos, float fSpeed, bool bSkillMove, bool bReverseDir, bool bSkillDash, WORD wActionAnim );
	virtual void ReceiveTeleport ( const D3DXVECTOR3 &vMovePos );	//Teleport Skill Effect 

	virtual float GETSTATE_DAMAGE () const			{ return m_fSTATE_DAMAGE; }

	virtual DAMAGE_SPEC GetDamageSpec() const		{ return m_sDamageSpec; }
	virtual DEFENSE_SKILL GetDefenseSkill() const	{ return m_sDefenseSkill; }

	/*ignore damage, Juver, 2017/12/12 */
	virtual BOOL IsIgnoreDamage () const			{ return (BOOL)m_bIgnoreDamage; }

	/*block back damage, Juver, 2017/12/12 */
	virtual BOOL IsBlockDamageBack () const			{ return (BOOL)m_bBlockBackDamage; }
	virtual void SetBlockDamageBack( bool bSet );

	virtual SINCREASEEFF* const GetIncreaseEff() { return &m_sIncreaseEff; }

public:
	void ReceiveExp ( int nGenExp, const bool bupdate_msg );
	// ÀÌº¥Æ®³ª Å½´Ð ¹æÁö ½Ã½ºÅÛ¿¡ ÀÇÇØ º¯°æµÈ °æÇèÄ¡¸¦ ±¸ÇÑ´Ù.
	int	 CalculateReceveExp ( int nGenExp );

public:
	void ReceiveKillExp ( const STARGETID &cTargetID, bool bConftSchool=false, bool bSummonReceive=false );
	void ReceiveAttackExp ( const STARGETID &cTargetID, DWORD dwDamage, BOOL bPartySkill, bool bConftSchool=false, bool bSummonReceive=false );
	void ReceivePartyExp ( const int nGenExp );
	void ReceiveLivingPoint ( int nLP );
	/*qbox check, Juver, 2017/12/05 */
	BOOL RecieveQItemFact ( const SNATIVEID nidITEM, BOOL bMobGen );
	void ReceiveEventEx ( const EMGM_EVENT_TYPE emType, const WORD wValue );
	void ReceiveBeginEvent ( const SEventState emType );
	void ReceiveVietnam ( const __time64_t initTime );
	BOOL ReceivePKCombo ( int nCount );

	void ReBirthDecExp ();

	LONGLONG	GetReExp ();
	LONGLONG	GetReExpMoney ( LONGLONG nReExp );

public:
	virtual DWORD GETCLIENTID ()					{ return m_dwClientID; }

	virtual void STATEBLOW ( const SSTATEBLOW &sStateBlow );
	virtual void CURE_STATEBLOW ( DWORD dwCUREFLAG );
	virtual void BUFF_REMOVE( DWORD dwBuffFlag );
	virtual void DO_STUN_ACTION();
	virtual void VAR_BODY_POINT ( const EMCROW emACrow, const DWORD dwAID, const BOOL bPartySkill, int nvar_hp, int nvar_mp, int nvar_sp );
	virtual BOOL RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, DWORD &dwSELECT, WORD _wCasterCrow, DWORD _dwCasterID );

	virtual BOOL RECEIVE_DRUGFACT ( EMITEM_DRUG emDRUG, WORD wCURE, BOOL bRATIO );

public:
	DWORD ToDamage ( const STARGETID &sTargetID, const int nDamage, const BOOL bShock );
	
private:
	void GenerateReward ( bool bCONFT=false, bool bCONFT_SCHOOL=false );
	void DoFalling ();

public:
	bool	IsConflictTarget ( GLACTOR* pTARGET );
	bool	IsReActionable ( GLACTOR* pTARGET, bool benermy=true );

public:
	void	AddPlayHostile ( DWORD dwCHARID, BOOL bBAD, BOOL bClubBattle );

private:
	void	PreStrikeProc ( BOOL bSkill, BOOL bLowSP );

private:
	//	°ø°Ý ÇÁ·Î¼¼½º.
	BOOL	AttackProcess ( float fElapsedTime );
	void	AvoidProc ( const SNATIVEID skill_id=NATIVEID_NULL(), const BOOL bLowSP=FALSE );
	void	DamageProc ( int nDAMAGE, DWORD dwDamageFlag, const BOOL bLowSP );
	void	Damage_SendMessage(const int nDAMAGE, const DWORD dwDamageFlag, const STARGETID& sTargetID, const bool bBroad);
	void	Heal_SendMessage(const int nHEAL, const DWORD dwHealFlag, const STARGETID& sTargetID, const bool bBroad);
	void	BlockProc ( const STARGETID& sTargetID, const SNATIVEID& skill_id=NATIVEID_NULL(), const WORD wSkillLevel = 1, const BOOL bLowSP=FALSE );
public:
	virtual void DamageReflectionProc ( int nDAMAGE, STARGETID sACTOR );
	virtual void DefenseSkill( SNATIVEID sNativeID, WORD wLevel, STARGETID sACTOR );
	

	//	½ºÅ³ ÇÁ·Î¼¼½º.
	BOOL	SkillProcess ( float fElapsedTime );
	void	SkillProc ( SNATIVEID skill_id, BOOL bLowSP );
	void	WeaponSkillProc ();
	void	ApplySkillTarget_HP( PGLSKILL pSkill, WORD wSKILL_LVL, SKILL::CDATA_LVL& sSKILL_DATA, GLACTOR* pACTOR, WORD	wAttackDivCount, const STARGETID&	sTARID, int& nHP, DWORD& dwDamageFlag );
	void	ApplySkillTarget_MP( PGLSKILL pSkill, SKILL::CDATA_LVL& sSKILL_DATA, GLACTOR* pACTOR, int& nMP );
	void	ApplySkillTarget_SP( PGLSKILL pSkill, SKILL::CDATA_LVL& sSKILL_DATA, GLACTOR* pACTOR, int& nMP );
	void	ApplySkillTargetStigma(const STARGETID& sTARID, const SKILL::SSPEC& sSPEC, const PGLSKILL pSkill);
	void	ApplySkillTargetSkillLink(const STARGETID& sSelectTARID, const STARGETID& sTARGETID, const SKILL::SSPEC& sSPEC, const SCHARSKILL& sSkill);
	void	ApplySkillTarget_SpecToMob( const EMSPEC_ADDON& emSpec, const SKILL::SSPEC& sSKILL_SPEC, GLACTOR* pACTOR, DWORD& dwDamageFlag, int* nGather, int* Vars );
	void	ApplySkillTarget_SpecToMobElse( const EMSPEC_ADDON& emSpec, const SKILL::SSPEC& sSKILL_SPEC, GLACTOR* pACTOR, DWORD& dwDamageFlag, int* nGather, int* Vars );
	void	ApplySkillTarget_Spec( PGLSKILL pSkill, WORD wSKILL_LVL, GLACTOR* pACTOR, DWORD& dwDamageFlag, int* nGather, int* Vars );
	
	void	RECEIVE_SKILLFACT_ReleasePet(void);

	//	¼îÅ© ÇÁ·Î¼¼½º.
	BOOL	ShockProcess ( float fElapsedTime );

public:
	HRESULT GetViewAround ();
	HRESULT UpdateViewAround ();
	HRESULT ResetViewAround ();

	// µ·ÀÌ ¾÷µ¥ÀÌÆ® µÇ¾úÀ» °æ¿ì Ã¼Å©ÇÏ´Â º¯¼ö
	BOOL	CheckMoneyUpdate( const LONGLONG lnCurrentMoney, const LONGLONG lnUpdateMoney, const bool bPlus, const char* msg );

public:
	HRESULT MsgGameJoin ();
	HRESULT MsgClubInfo ();

	HRESULT MsgClubStorage ();
	HRESULT MsgFieldMoveReset ();

	BOOL IsInViewAround ( DWORD dwGAEAID );


    virtual void SendToClient(NET_MSG_GENERIC* pMsg);
	virtual void SendToClient(const DWORD dwClientSlot, NET_MSG_GENERIC* pMsg);
	HRESULT SendMsgViewAround ( NET_MSG_GENERIC* nmg );
	virtual HRESULT SNDMSGAROUND ( NET_MSG_GENERIC *pnet )		{ return SendMsgViewAround ( pnet ); }

	HRESULT CheckInstanceItem ();

public:
	const SCONFTING& GETCONFRONTING ()							{ return m_sCONFTING; }
	void ResetConfront ( EMCONFRONT_END emEND );

public:
	NET_MSG_GENERIC* ReqNetMsg_Drop ();
	void ReqNetMsg_PMarketItem ( DWORD dwClientID );

	void MsgSendPeriod ();
	void MsgSendUpdateState ( bool bparty=true, bool bconfront=true, bool bview=false );

	void DoConftStateBackup ();
	void DoConftStateRestore ();

protected:
	bool CheckQuestItemReward ( GLQUEST* pQUEST );
	// Äù½ºÆ® ÁøÇà¿¡ µû¶ó ½ÇÇàµÇ´Â ÀÌº¥Æ®¸¦ 0 : ½ÃÀÛ½Ã, 1 : ¿Ï·á½Ã, 2 : ½ÇÆÐ½Ã
	bool CheckQuestProgressEvent ( BYTE eventType, GLQUEST* pQUEST );
	bool QuestMoveProgressEvent ( SNATIVEID mapID, DWORD wPosX, DWORD wPosY, DWORD dwGateID, bool bStartMove = FALSE );

public:
	bool DoQuestProgress ( GLQUESTPROG* pQUEST_PROG );
	void DoQuestGenerateReward ( GLQUEST* pQUEST );
	void DoQuestMobKill ( SNATIVEID nidMOB );
	void DoQuestReachZone ();
	WORD DoRevive2Skill ( SNATIVEID skill_id, WORD wSKILL_LVL, WORD wSPEC );

	void MsgSendQuestTime ();
	void DoQuestCheckLimitTime ( float fTime, float fElapsedTime );
	void DoQuestCheckLimitDie ();
	void DoQuestCheckLeaveMap ();
	void DoQuestLevel ();

public:
	void ClubBattleSetPoint( DWORD dwClub_P, DWORD dwClub_S, bool bAlliancBattle );

public:
	bool IsInsertToQuestInven ( GLInventory &_sINVENTORY, SITEMCUSTOM &sCUSTOM );
	HRESULT InsertToQuestInven ( DWORD dwClientID, DWORD dwQID, GLInventory &_sINVENTORY, SITEMCUSTOM &sCUSTOM );

public:
	bool IsInsertToInven ( PITEMDROP pItemDrop, bool bVietnamInven = FALSE );
	bool IsInsertToInvenEx ( PITEMDROP pItemDrop );

public:
	HRESULT InsertToInven ( PITEMDROP pItemDrop );
	HRESULT	InsertToInvenEx ( PITEMDROP pItemDrop );
	HRESULT InsertToVNGainInven ( PITEMDROP pItemDrop );
	// ÀÌº¥Æ®³ª Å½´Ð ¹æÁö ½Ã½ºÅÛ¿¡ ÀÇÇØ º¯°æµÈ µ·ÀÇ ¾çÀ» ±¸ÇÑ´Ù.
	LONGLONG CalculateInsertmoney( LONGLONG lnAmount );

	void InsertMoney ( LONGLONG lnAmount );
	HRESULT InsertToInven ( PMONEYDROP pMoneyDrop );
	bool InsertPileItem ( const SITEMCUSTOM& sItemCustom, WORD wREQINSRTNUM );
	bool DeletePileItem ( SNATIVEID sNID, WORD wREQDELNUM );
	bool DoDrugInvenItem ( WORD wPosX, WORD wPosY, bool b2AGENT=false );
	bool DoDrugStorageItem ( DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool DoDrugSlotItem ( EMSLOT emSLOT, WORD wNum = 1 );
	float GetExpMultipleRateFromSlotItem();


	ITEM_COOLTIME*	GetCoolTime( DWORD dwCoolID, EMCOOL_TYPE emCoolType );

	bool IsCoolTime( SNATIVEID sNativeID );		// Àç»ç¿ë ´ë±â½Ã°£ ³²¾Ò´ÂÁö
	bool CheckCoolTime( SNATIVEID sNativeID );		// ¾ÆÀÌÅÛ »ç¿ë½Ã ÄðÅ¸ÀÓ Ã¼Å©
	bool SetCoolTime( SNATIVEID sNativeID , EMCOOL_TYPE emCoolType );	//	ÄðÅ¸ÀÓ ÀÏ°ý ¼ÂÆÃ
	__time64_t	GetMaxCoolTime ( SNATIVEID sNativeID );


	bool UpdateNpcCommission( LONGLONG lnCommission );


	void DoReBirth2StartMap ();
	BOOL CheckStartMap();
	void ReSetStartMap();

public:
	void QuestStartFromGetITEM ( SNATIVEID nidITEM );
	void QuestStartFromGetSKILL ( SNATIVEID nidSKILL );
	void QuestStartFromGetLEVEL ( WORD wLEVEL );

protected:
	HRESULT TourchQItem ( DWORD dwGlobID );
	bool IsKEEP_CLUB_STORAGE ( DWORD dwCHANNEL );
	void DROP_PUTONITEM ( DWORD _dwNUM, float _fRATE );


public:
	bool IsCHATBLOCK ();

	void SENDBOXITEMINFO ();

	/*dmk14 chat limit*/
	bool IsPapering();

public:
	BOOL IsSpam()	{ return m_bSpam; }
	void SetSpam()	{ m_bSpam = TRUE; }

public:
	HRESULT	ActiveVehicle( bool bActive, bool bMoveMap );	

protected:
	/* Selective Box, Baby011528, 2022-02-15 */
	HRESULT MsgSelformBoxOpen(NET_MSG_GENERIC* nmg);
	//
	
	HRESULT MsgReady ( NET_MSG_GENERIC* nmg );
	HRESULT MsgActState ( NET_MSG_GENERIC* nmg );
	HRESULT MsgMoveState ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGoto ( NET_MSG_GENERIC* nmg );
	HRESULT MsgAttack ( NET_MSG_GENERIC* nmg );
	HRESULT MsgAttackCancel ( NET_MSG_GENERIC* nmg );
	
	HRESULT MsgReqFieldToInven ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqFieldToHold ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqInvenToHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenExHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenToSlot ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqVNGainToHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqVNGainExHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqHoldToVNGain ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqVNGainInvenReset ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqVNInvenToInven ( NET_MSG_GENERIC* nmg );
	
	HRESULT MsgReqInvenSplit ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqSlotToHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSlotExHold ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqHoldToInven ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqHoldArray ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqHoldArrayToInven ();
	HRESULT MsgReqHoldToField ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqHoldToSlot ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSlotChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqMoneyToField ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqUpRell(NET_MSG_GENERIC* nmg);//Âà¥Í¨t²Î

	HRESULT MsgReqBuyFromNpc ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSaleToNpc ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqReBirth ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqLevelUp ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStatsUp ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStatsUpCmd ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenDrug ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqDamageOverlap ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSuicideCmd ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenGMSkillAll(NET_MSG_GENERIC* nmg); //GM¥þ§Þ¯à
	HRESULT MsgReqInvenBoxOpen ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenGMItem ( NET_MSG_GENERIC* nmg ); //add itemcmd
	HRESULT MsgReqInvenBoxInfo ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenDisguise ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenCleanser ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqRankName(NET_MSG_GENERIC* nmg); //MMR Rank
	HRESULT MsgReqRankMark(NET_MSG_GENERIC* nmg); //MMR Rank
	
	HRESULT MsgReqInvenChargedItem ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqSkill ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSkillCancel ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqLearnSkill ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqLearnSkillNonInven ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSkillUp ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqSkillQSetActive ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSkillQSet ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSkillQReSet ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqActionQSet ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqActionQReSet ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqGetStorage ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStorageDrug ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStorageSkill ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqStorageToHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStorageExHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqHoldToStorage ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenResetSkStTo(NET_MSG_GENERIC* nmg);//¬~ÂI¨t²Î
	HRESULT MsgReqInvenResetSkStTo1(NET_MSG_GENERIC* nmg);//¬~¬õ¨t²Î

	HRESULT MsgReqStorageSaveMoney ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStorageDrawMoney ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStorageSplit ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqInvenGrinding ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenResetSkSt ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqReGenGate ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqCure ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqRevive ( NET_MSG_GENERIC* nmg );
    HRESULT MsgReqFriendFld ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqMove2CharFld ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqGetExpRecovery ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqRecovery ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqGetExpRecoveryNpc ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqRecoveryNpc ( NET_MSG_GENERIC* nmg );
	

	HRESULT MsgReqCharReset ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqCharCard ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqStorageCard ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenLine ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenStorageOpen ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenStorageClose ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRemodelOpen ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenGarbageOpen( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenPremiumSet ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqFireCracker ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRandomBoxOpen ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenDisJunction ( NET_MSG_GENERIC* nmg );
	//µ¥¯Å¤É¯Å¥d
	HRESULT MsgReqUpgradeCard(NET_MSG_GENERIC* nmg);
	HRESULT MsgReqInvenHairChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenHairStyleChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenHairColorChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenFaceChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenFaceStyleChange ( NET_MSG_GENERIC* nmg );
	HRESULT	MsgReqInvenGenderChange( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqResetBrightPoint ( NET_MSG_GENERIC* nmg );
	//CNDev | 2-7-2020 | e-point card system
	HRESULT MsgReqCardEPoint ( NET_MSG_GENERIC* nmg );
	
	HRESULT MsgReqCardContribution ( NET_MSG_GENERIC* nmg );

	HRESULT MsgMysteryBox(NET_MSG_GENERIC* nmg); //Mystery Box

	HRESULT MsgReqInvenRename ( NET_MSG_GENERIC* nmg );
	HRESULT MsgInvenRename ( NET_MSG_GENERIC* nmg );

	HRESULT MsgInvenVietnamGet ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqConFrontStart ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqConFrontEnd ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqGesture ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqNpcItemTrade ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqNpcQuestStart ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqNpcQuestTalk ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqQuestGiveUp ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqQuestReadReset ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqQuestComplete ( NET_MSG_GENERIC* nmg );

	HRESULT MsgLoudSpeaker ( NET_MSG_GENERIC* nmg );
	
	HRESULT MsgPMarketTitle ( NET_MSG_GENERIC* nmg );
	HRESULT MsgPMarketReqItem ( NET_MSG_GENERIC* nmg );
	HRESULT MsgPMarketDisItem ( NET_MSG_GENERIC* nmg );
	HRESULT MsgPMarketOpen ( NET_MSG_GENERIC* nmg );
	HRESULT MsgPMarketClose ( NET_MSG_GENERIC* nmg );
	HRESULT MsgPMarketItemInfo ( NET_MSG_GENERIC* nmg );
	HRESULT MsgPMarketBuy ( NET_MSG_GENERIC* nmg );
	
	HRESULT MsgClubNew ( NET_MSG_GENERIC* nmg );
	HRESULT MsgClubRank ( NET_MSG_GENERIC* nmg );
	HRESULT MsgClubNew2FLD ( NET_MSG_GENERIC* nmg );
	HRESULT MsgClubMemberReq ( NET_MSG_GENERIC* nmg );
	HRESULT MsgClubMemberAns ( NET_MSG_GENERIC* nmg );
	HRESULT MsgClubMemberNick ( NET_MSG_GENERIC* nmg );
	HRESULT MsgClubCDCertify ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqClubGetStorage ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqClubStorageToHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqClubStorageExHold ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqClubHoldToStorage ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqClubStorageSplit ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqClubStorageSaveMoney ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqClubStorageDrawMoney ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqClubInComeReNew ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqRebuildMoveSeal( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqRebuildResult( NET_MSG_GENERIC* nmg );	// ITEMREBUILD_MARK
	HRESULT MsgReqRebuildMoveItem( NET_MSG_GENERIC* nmg );
	/*dmk14 reform*/
	//¸Ë³Æ¤É¯Å
	HRESULT MsgReqZbsjResult(NET_MSG_GENERIC* nmg);	// ITEMREBUILD_MARK
	HRESULT MsgReqZbsjMoveItem(NET_MSG_GENERIC* nmg);
	//END
	HRESULT MsgReqRebuildMoveItemSealed( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqRebuildCostMoney();
	HRESULT MsgReqRebuildInputMoney( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqGarbageResult( NET_MSG_GENERIC* nmg );	// ÈÞÁöÅë »ç¿ë

	HRESULT MsgReqPhoneNumber( NET_MSG_GENERIC* nmg );
	HRESULT MsgPhoneNumber( NET_MSG_GENERIC* nmg );
	HRESULT MsgSendSMS( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqSendSMS( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqMGameOddEvenEvent( NET_MSG_GENERIC* nmg ); // ¹Ì´Ï °ÔÀÓ - È¦Â¦

	HRESULT MsgItemShopOpen( NET_MSG_GENERIC* nmg );	// ItemShopOpen

	HRESULT MsgReqItemMix( NET_MSG_GENERIC* nmg );		// ItemMIx

	// PET
	/*dual pet skill, Juver, 2017/12/28 */
	HRESULT MsgReqGetRightOfItem_A ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqGetRightOfItem_B ( NET_MSG_GENERIC* nmg );

	/*dual pet skill, Juver, 2017/12/28 */
	HRESULT MsgGetFieldAllItem_A ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldRareItem_A ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldPotions_A ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldMoney_A ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldStone_A ( NET_MSG_GENERIC* nmg );

	/*dual pet skill, Juver, 2017/12/28 */
	HRESULT MsgGetFieldAllItem_B ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldRareItem_B ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldPotions_B ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldMoney_B ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetFieldStone_B ( NET_MSG_GENERIC* nmg );

	HRESULT	MsgGiveFood ( NET_MSG_GENERIC* nmg );
	HRESULT MsgGetPetFullFromDB ( NET_MSG_GENERIC* nmg );
	HRESULT	MsgPetReviveInfo ( NET_MSG_GENERIC* nmg );

	/*dual pet skill, Juver, 2017/12/29 */
	HRESULT	MsgEnableDualSkill ( NET_MSG_GENERIC* nmg );

	// vehicle 
	HRESULT	MsgVehicleSlotExHold( NET_MSG_GENERIC* nmg );
	HRESULT	MsgVehicleHoldToSlot( NET_MSG_GENERIC* nmg );
	HRESULT	MsgVehicleSlotToHold( NET_MSG_GENERIC* nmg );
	HRESULT	MsgVehicleRemoveSlot( NET_MSG_GENERIC* nmg );
	HRESULT	MsgVehicleGiveBattery( NET_MSG_GENERIC* nmg );
	HRESULT	MsgGetVehicleFullFromDB ( NET_MSG_GENERIC* nmg );

	/*vehicle booster system, Juver, 2017/08/10 */
	HRESULT	MsgVehicleEnableBooster( NET_MSG_GENERIC* nmg );
	HRESULT	MsgReqVehicleBooster( NET_MSG_GENERIC* nmg );

	// vietNam GainType System
	HRESULT MsgReqVietnamGainType ( NET_MSG_GENERIC* nmg );

	HRESULT MsgAttendReward ( NET_MSG_GENERIC* nmg );

	HRESULT	MsgGathering( NET_MSG_GENERIC* nmg );
	HRESULT MsgGatheringCancel ( NET_MSG_GENERIC* nmg );

	/*bike color , Juver, 2017/11/13 */
	HRESULT	MsgVehicleChangeColor( NET_MSG_GENERIC* nmg );

	/* car, cart color, Juver, 2018/02/14 */
	HRESULT	MsgCarChangeColor( NET_MSG_GENERIC* nmg );
	/* 3-12-2024Boss Spawn Viewer, Bubu */
	HRESULT MsgBossDetailsReq(NET_MSG_GENERIC* nmg);

	/* booster all vehicle, Juver, 2018/02/14 */
	HRESULT	MsgAllVehicleEnableBooster( NET_MSG_GENERIC* nmg );

	/*dmk14 ingame web*/
	HRESULT MsgReqBuyFromItemMall ( NET_MSG_GENERIC* nmg );
	/*dmk14 remove active buff selection*/
	HRESULT MsgReqDisableSkillEff ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqDisableItemFact ( NET_MSG_GENERIC* nmg );

	HRESULT MsgReqReformWeapon ( NET_MSG_GENERIC* nmg ); //Weapon Skill Reform Card - SqueeeAK
	/*Codex System, Jhoniex 2/14/2024 */
	HRESULT MsgReqRegisterCodex ( NET_MSG_GENERIC* nmg );
public:
	HRESULT MsgProcess ( NET_MSG_GENERIC* nmg );	

protected:
	HRESULT UpdateClientState ( float fElapsedTime );
	HRESULT UpdateVehicle( float fElapsedTime );
	// ÁöÇü¿¡ ´ëÇÑ ÀÌ»óÈ¿°ú 
	VOID	UpdateLandEffect();

public:
	HRESULT FrameMove ( float fTime, float fElapsedTime );
	void	ChinaGainCalculate();
	void	VietnamGainCalculate();
	void	EventCalculate();

	void	DisableSkillFact();


public:
	/*itemfood system, Juver, 2017/05/25 */
	HRESULT MsgReqConsumeFood( NET_MSG_GENERIC* nmg );	
	HRESULT MsgReqUnlockFood( NET_MSG_GENERIC* nmg );

	/*game stats, Juver, 2017/06/21 */
	HRESULT MsgReplyPing( NET_MSG_GENERIC* nmg ); 

public:
	SNATIVEID m_sidLastMapCheck;
	void	CheckChangeMap();

public:
	/*combatpoint logic, Juver, 2017/05/29 */
	void	ReceiveCP ( int nGen, bool bSummonReceive=false );

private:
	/*itemfood system, Juver, 2017/05/25 */
	WORD	ItemFoodAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT );
	WORD	ItemFoodFindSlot ( SNATIVEID skill_id );

public:
	HRESULT MsgHPCheck ( NET_MSG_GENERIC* nmg );
	 /*rightclick wear/unwear, Juver, 2017/06/24 */
	HRESULT MsgReqInvenToWear ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqWearToInven ( NET_MSG_GENERIC* nmg );

	/*npc shop, Juver, 2017/07/27 */
	HRESULT ReqNpcShopPurchaseMoney ( NET_MSG_GENERIC* nmg );

	/*vehicle booster system, Juver, 2017/08/11 */
	void	VehicleBoosterStateCharge( bool bStart );
	void	VehicleBoosterStateStart( bool bStart );
	void	VehicleBoosterStateReset();

	/*system buffs, Juver, 2017/09/05 */
	WORD	SystemBuffFindSlot ( SNATIVEID skill_id );
	void	SystemBuffAdd ( SNATIVEID skill_id, WORD wLevel, WORD wSLOT );
	void	SystemBuffReset ( WORD wSLOT );
	
	/*pvp tyranny, Juver, 2017/08/25 */
	BOOL	PVPTyrannyRevive( DWORD dwGateID );

	/*item exchange, Juver, 2017/10/13 */
	HRESULT ReqNpcItemExchangeTrade ( NET_MSG_GENERIC* nmg );

	/*product item, Juver, 2017/10/18 */
	HRESULT ReqItemCompoundStart ( NET_MSG_GENERIC* nmg );
	HRESULT ReqItemCompoundCancel ( NET_MSG_GENERIC* nmg );
	HRESULT	DoItemCompoundTask();

	/*activity system, Juver, 2017/11/05 */
	HRESULT ReqCharacterBadgeChange ( NET_MSG_GENERIC* nmg );

public:
	/*activity system, Juver, 2017/10/30 */
	void	ActivityComplete( SACTIVITY_CHAR_DATA& sactivity_char_data );
	void	DoActivityLevel();
	void	DoActivityMobKill( SNATIVEID sidMob );
	void	DoActivityMapKill( SNATIVEID sidMap );
	void	DoActivityMapReach( SNATIVEID sidMap );
	void	DoActivityTakeItem( SNATIVEID sidItem, WORD wTurnNum );
	void	DoActivityUseItem( SNATIVEID sidItem, WORD wUseNum );
	void	DoActivityObtainQBox( EMITEM_QUESTION emQuestion );
	void	DoActivityCompleteQuest( DWORD dwQuestID );
	void	DoActivityReachPoint();
	
	/*Codex System, Jhoniex 2/14/2024 */
	void	CodexComplete( SCODEX_CHAR_DATA& sactivity_char_data );
	void	DoCodexRegisterItem( SNATIVEID sidItem, DWORD dwActivityID, WORD wTurnNum, WORD wGrade );

public:
	/*charinfoview , Juver, 2017/11/11 */
	HRESULT MsgReqCharInfo( NET_MSG_GENERIC* nmg );

	/*pk info, Juver, 2017/11/17 */
	void	PKIncreaseScore( DWORD dwCharID );
	void	PKIncreaseDeath( DWORD dwCharID );


	/*rv card, Juver, 2017/11/25 */
	HRESULT	MsgRandomOptionChange( NET_MSG_GENERIC* nmg );

	/*nondrop card, Juver, 2017/11/26 */
	HRESULT	MsgNonDropCard( NET_MSG_GENERIC* nmg );

	/*regional chat, Juver, 2017/12/06 */
	HRESULT	MsgRegionalChatPay( NET_MSG_GENERIC* nmg );

	/*skill summon, Juver, 2017/10/10 */
	void	RemoveSummonBuff( DWORD dwSummonID );

	/*clubwar verify non push or pull, Juver, 2017/12/13 */
	void	SetClubWarVerify( BOOL bSET );
	/*hackshield implementation, Juver, 2018/06/21 */
	float	hs_heartbeat_timer;
	LONGLONG hs_disconnect_num;
	LONGLONG hs_heartbeat_num;
	void	hs_client_callback( NET_MSG_GENERIC* nmg );
	void	hs_client_hearbeat( NET_MSG_GENERIC* nmg );
	void	hs_client_disconnect();

	HRESULT MsgReqRemoveSkill ( NET_MSG_GENERIC* nmg );

	/*change scale card, Juver, 2018/01/04 */
	HRESULT	MsgChangeScale( NET_MSG_GENERIC* nmg );

	/*item color, Juver, 2018/01/10 */
	HRESULT MsgReqInvenItemColorChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqKillFeedSelect(NET_MSG_GENERIC* nmg);

	/*item wrapper, Juver, 2018/01/11 */
	HRESULT MsgReqInvenItemWrap ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenItemUnWrap ( NET_MSG_GENERIC* nmg );

	/*change school card, Juver, 2018/01/12 */
	HRESULT MsgReqInvenChangeSchool ( NET_MSG_GENERIC* nmg );

	/*equipment lock, Juver, 2018/01/13 */
	HRESULT MsgReqInvenEnableEquipmentLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenEnableEquipmentLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenInputEquipmentLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenInputEquipmentLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverEquipmentLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverEquipmentLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverEquipmentLockChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverEquipmentLockChangeDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverEquipmentLockDelete ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverEquipmentLockDeleteDB ( NET_MSG_GENERIC* nmg );

	/*storage lock, Juver, 2018/01/13 */
	HRESULT MsgReqInvenEnableStorageLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenEnableStorageLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenInputStorageLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenInputStorageLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverStorageLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverStorageLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverStorageLockChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverStorageLockChangeDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverStorageLockDelete ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverStorageLockDeleteDB ( NET_MSG_GENERIC* nmg );

	/*inventory lock, Juver, 2018/01/13 */
	HRESULT MsgReqInvenEnableInventoryLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenEnableInventoryLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenInputInventoryLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenInputInventoryLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverInventoryLock ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverInventoryLockDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverInventoryLockChange ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverInventoryLockChangeDB ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverInventoryLockDelete ( NET_MSG_GENERIC* nmg );
	HRESULT MsgReqInvenRecoverInventoryLockDeleteDB ( NET_MSG_GENERIC* nmg );

	/*item transfer card, Juver, 2018/01/18 */
	HRESULT MsgReqInvenItemTransfer ( NET_MSG_GENERIC* nmg );

	/*school wars, Juver, 2018/01/20 */
	BOOL	PVPSchoolWarsRevive( DWORD dwGateID );

	/*pvp capture the flag, Juver, 2018/02/01 */
	BOOL	PVPCaptureTheFlagRevive( DWORD dwGateID );
	void	PVPCaptureTheFlagDetectGate();
	void	PVPCaptureTheFlagHoldFlag( bool bHold );

public:
	GLChar ();
	~GLChar ();
};

/*dmk14 anti shit gs*/
struct GLSkillTargetArgv
{
	GLChar* const pChar;
	GLMSG::SNETPC_REQ_SKILL* const pNetMsg;
	const GLSKILL* const pSkill;
	const STARGETID& TargetID;	
	const WORD wSkillLevel;
	
	const GLSkillTargetArgv& operator =(const GLSkillTargetArgv&){}
	GLSkillTargetArgv(GLChar* const _pChar, GLMSG::SNETPC_REQ_SKILL* const _pNetMsg, const GLSKILL* const _pSkill, const STARGETID& _TargetID, const WORD _wSkillLevel)
		: pChar(_pChar)
		, pNetMsg(_pNetMsg)
		, pSkill(_pSkill)
		, TargetID(_TargetID)		
		, wSkillLevel(_wSkillLevel)		
	{
	}
};

const bool setTarget_Self(GLSkillTargetArgv& SkillTargetArgv);
const bool setTarget_Spec(GLSkillTargetArgv& SkillTargetArgv);
const bool setTarget_SelfToSpec(GLSkillTargetArgv& SkillTargetArgv);
const bool setTarget_Zone(GLSkillTargetArgv& SkillTargetArgv);
const bool setTarget_Land(GLSkillTargetArgv& SkillTargetArgv);
/*dmk14 anti shit end*/

typedef GLChar*				PGLCHAR;
typedef CGLLIST<PGLCHAR>	GLCHARLIST;
typedef SGLNODE<PGLCHAR>	GLCHARNODE;

typedef std::map<std::string,PGLCHAR>	GLCHAR_MAP;
typedef std::pair<std::string,PGLCHAR>	GLCHAR_MAP_PAIR;
typedef GLCHAR_MAP::iterator			GLCHAR_MAP_ITER;


