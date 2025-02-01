#ifndef GLOGIXEX_H_
#define GLOGIXEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GLCHARLOGIC, GLCROWLOGIC
//	µÎ Å¬·¡½º°¡ °øÅëÀ¸·Î °¡Áö´Â ÇÔ¼öµéÀº µ¥ÀÌÅÍÀÇ ±Ù¿ø "ÄÉ¸¯ÅÍ µ¥ÀÌÅÍ", "NPC µ¥ÀÌÅÍ" ¿¡¼­
//	°¢°¢ ¹Þ¾Æ¼­ °°Àº ³»¿ëÀ» ÇÔ¼ö·Î Á¦ÀÛÇÏ°ÔµÊ. ½ÇÁ¦ ÇÔ¼ö ºÎºÐÀº °¢°¢ µû·Î ÀÖÀ½. ( µ¿ÀÛ È¿À²¼º °í·Á. )
//	´Ü, ÇÔ¼ö ³»¿ëÀº µ¿ÀÏÇÏ°Ô À¯Áö. ( ´Ù¸¥ µ¥ÀÌÅÍ ±Ù¿ø¿¡¼­ °¡Á®¿À´õ¶óµµ ÀÎ¶óÀÎ ÇÔ¼ö Á¤ÀÇ¸¦ µ¿ÀÏÇÏ°ÔÇÏ¿© ¸ÂÃã. )
//
//
//
//
#define MOVE_OPTION_SHOW_EFF			0x00000001
#define MOVE_OPTION_NOT_TURN_IDLE		0x00000002

#include <map>

#include "./GLSKILL.h"
#include "./GLCharData.h"
#include "./GLCrowData.h"
#include "./GLogicData.h"
#include "./GLContrlMsg.h"
#include "./GLPVPTyrannyDefine.h" /*pvp tyranny, Juver, 2017/08/25 */

/*activity system, Juver, 2017/10/30 */
#include "./GLActivityData.h"

#ifndef GLSKILL
	struct GLSKILL;
#endif //GLSKILL

typedef std::map<DWORD,float>	DELAY_MAP;
typedef DELAY_MAP::iterator				DELAY_MAP_ITER;

class STARDIST
{
public:
	float		m_fDIST;
	STARGETID	m_sTARGETID;

public:
	STARDIST () {}

	STARDIST ( const float fDIST, const STARGETID &sTARID )
	{
		m_fDIST = fDIST;
		m_sTARGETID = sTARID;
	}

	bool operator < ( const STARDIST &rvalue )
	{
		return m_fDIST < rvalue.m_fDIST;
	}
};

typedef std::vector<STARDIST>			TARGETMAP;
typedef TARGETMAP::iterator				TARGETMAP_ITER;

typedef std::vector<STARGETID>						DETECTMAP;
typedef DETECTMAP::iterator							DETECTMAP_ITER;
typedef std::pair<DETECTMAP_ITER,DETECTMAP_ITER>	DETECTMAP_RANGE;

class STAR_ORDER
{
public:
	bool operator() ( const STARGETID &lvalue, const STARGETID &rvalue )
	{
		return lvalue.emCrow < rvalue.emCrow;
	}

	bool operator() ( const STARGETID &lvalue, const EMCROW emCROW )
	{
		return lvalue.emCrow < emCROW;
	}

	bool operator() ( const EMCROW emCROW, const STARGETID &rvalue )
	{
		return emCROW < rvalue.emCrow;
	}
};

//	Àû´ëÀÚ.
struct SPLAYHOSTILE
{
	BOOL	bBAD;
	float	fTIME;

	SPLAYHOSTILE ()
	{
		bBAD     = FALSE;
		fTIME    = 0;
	}
};

typedef std::map<DWORD,SPLAYHOSTILE*>	MAPPLAYHOSTILE;
typedef MAPPLAYHOSTILE::iterator				MAPPLAYHOSTILE_ITER;

struct SSUM_ITEM
{
	GLDWDATA	gdDamage;
	int			nDefense;

	int			nAvoidRate;
	int			nHitRate;

	int			nHP;
	int			nMP;
	int			nSP;

	float		fIncR_HP;
	float		fIncR_MP;
	float		fIncR_SP;

	float		fInc_HP;
	float		fInc_MP;
	float		fInc_SP;

	SCHARSTATS	sStats;
	SRESIST		sResist;

	int			nPA;
	int			nSA;
	int			nMA;

	float		fIncR_MoveSpeed;
	float		fIncR_AtkSpeed;
	float		fIncR_Critical;
	float		fIncR_CrushingBlow;

	float		fInc_MoveSpeed;
	float		fInc_AtkSpeed;
	float		fInc_Critical;
	float		fInc_CrushingBlow;
	
	DAMAGE_SPEC m_sDamageSpec;

	//new itemvar
	float		fDecR_Damage_MeleeSkill; // ¹°¸® ±ÙÁ¢ ½ºÅ³ ÇÇ°Ý½Ã °æ¿ì µ¥¹ÌÁö °¨¼Ò;
	float		fDecR_Damage_RangeSkill; // »ç°Ý ½ºÅ³ ÇÇ°Ý½Ã " ;
	float		fDecR_Damage_MagicSkill; // ¸¶¹ý ½ºÅ³ ÇÇ°Ý½Ã " ;

	//add directval
	int		nHP_Potion_Rate;
	int		nMP_Potion_Rate;
	int		nSP_Potion_Rate;

	//add directval hitrate and avoidrate
	float		fRateHit_Per;
	float		fRateAvoid_Per;


	SSUM_ITEM () :
		nDefense(0),

		nAvoidRate(0),
		nHitRate(0),
		nHP(0),
		nMP(0),
		nSP(0),
		fIncR_HP(0.0f),
		fIncR_MP(0.0f),
		fIncR_SP(0.0f),

		fInc_HP(0.0f),
		fInc_MP(0.0f),
		fInc_SP(0.0f),

		nPA(0),
		nSA(0),
		nMA(0),
		fIncR_MoveSpeed(0.0f),
		fIncR_AtkSpeed(0.0f),
		fIncR_Critical(0.0f),
		fIncR_CrushingBlow(0.0f),
		fInc_MoveSpeed(0.0f),
		fInc_AtkSpeed(0.0f),
		fInc_Critical(0.0f),
		fInc_CrushingBlow(0.0f),
		nHP_Potion_Rate(0), //add directval
		nMP_Potion_Rate(0),//add directval
		nSP_Potion_Rate(0),//add directval
		fRateHit_Per(0.0f), //add directval
		fRateAvoid_Per(0.0f),//add directval
		fDecR_Damage_MeleeSkill(0.0f),
		fDecR_Damage_RangeSkill(0.0f),
		fDecR_Damage_MagicSkill(0.0f)
	{
	}

	void RESET ()
	{
		*this = SSUM_ITEM();
	}
};

namespace GLOGICEX
{
	EM_BRIGHT_FB GLSPACEGAP ( const EMBRIGHT emACTOR, const EMBRIGHT emRECEP, const EMBRIGHT emSPACE );

	//	°ø°Ý ¼º°øÀ² °è»ê.
	int GLHITRATE ( const int nHit, const int nAvoid, const EM_BRIGHT_FB bFB );

	//	¿ÏÀü ¼îÅ© È®À².
	BOOL CHECKSHOCK ( const int AttackerLev, const int DefenserLev, const int nDamage, const bool bCritical );

	//	¹æ¾î·Â °è»ê.
	int GLDEFENSE ( const int nDEFENSE, const EMBRIGHT emACTOR, const EMBRIGHT emRecep, const EMBRIGHT emSPACE );

	//	°æÇèÄ¡ È¹µæ ( Å¸°Ý½Ã )
	int GLATTACKEXP ( int AttackerLev, int DefenserLev, DWORD dwDamage, DWORD wMaxHP, DWORD dwBonusExp=0 );

	//	°æÇèÄ¡ È¹µæ ( Á¦°Å½Ã )
	int	GLKILLEXP ( int AttackerLev, int DefenserLev, DWORD dwBonusExp=0 );

	//	°æÇèÄ¡ È¹µæ ( Å¸°Ý½Ã )
	int CALCATTACKEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan, DWORD dwDamage );
	
	//	°æÇèÄ¡ È¹µæ ( Á¦°Å½Ã )
	int CALCKILLEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan );

	//	Á×¾úÀ»¶§ °æÇèÄ¡ °¨¼Ò.
	float GLDIE_DECEXP ( WORD wACTLEV );

	//	°æÇèÄ¡ º¹±¸ ºñÀ²
	float GLDIE_RECOVERYEXP ( WORD wACTLEV );

	//  °æÇèÄ¡1´ç µ·
	float GLDIE_EXPMONEY ( WORD wACTLEV );

	//	LevelUpÀ» ÇÏ±â À§ÇÑ °æÇèÄ¡
	LONGLONG GLNEEDEXP ( WORD wLev );
	LONGLONG GLNEEDEXP2 ( WORD wLev );

	//	»óÅÂÀÌ»ó ¹ß»ýÇÒ ¼ö ÀÖ´Â È®À² »êÃâ.
	BOOL CHECKSTATEBLOW ( float fACTRATE, WORD wACTLEVEL, WORD wLEVEL, WORD wRESIST );

	//	½ºÅ³ µô·¹ÀÌ »êÃâ.
	float SKILLDELAY ( DWORD dwSKILL_GRADE, WORD wSKILL_LEV, WORD wCHAR_LEVEL, float fDelay );

	//	Note : ÆÄ¶ó¸ÞÅ¸ º¯°æ.
	//		°è»ê½Ã¿¡´Â int ÇüÀ¸·Î °è»êÀ» ¼öÇàÇÏ¿© ¿¬»ê½Ã ¼Õ½ÇÀ» ¸·´Â´Ù.
	//		°ª Àû¿ë½Ã¿¡´Â WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
	inline WORD VARIATION ( WORD &wNow, const WORD wMax, const int nValue )
	{
		int nOld = int(wNow);

		int nNew = nOld + nValue;

		//	WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
		if ( nNew < 0 )					nNew = 0;
		if ( nNew > USHRT_MAX )			nNew = USHRT_MAX;

		wNow = nNew;
		if ( wNow >= wMax )				wNow = wMax;

		WORD wDX = (WORD) abs(nOld-int(wNow));
		return wDX;
	}

	//	Note : ÆÄ¶ó¸ÞÅ¸ º¯°æ.
	//		°è»ê½Ã¿¡´Â int ÇüÀ¸·Î °è»êÀ» ¼öÇàÇÏ¿© ¿¬»ê½Ã ¼Õ½ÇÀ» ¸·´Â´Ù.
	//		°ª Àû¿ë½Ã¿¡´Â WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
	inline DWORD VARIATION ( DWORD &dwNow, const DWORD dwMax, const int nValue )
	{
		int nOld = int(dwNow);

		int nNew = nOld + nValue;

		//	WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
		if ( nNew < 0 )					nNew = 0;

		dwNow = nNew;
		if ( dwNow >= dwMax )			dwNow = dwMax;

		DWORD dwDX = (DWORD) abs(nOld-int(dwNow));
		return dwDX;
	}

	//	³¯¾¾¿Í ¼Ó¼º°£ ÁõÆøÀ².
	float WEATHER_ELEMENT_POW ( EMELEMENT emElement, DWORD dwWeather, BOOL bWeatherActive );

	float WEATHER_BLOW_POW ( EMSTATE_BLOW emBlow, DWORD dwWeather, BOOL bWeatherActive );

	//	Note : Ã¼·Â °°Àº ¼öÄ¡µéÀ» º¯È­À²¿¡ µû¶ó °»½ÅÇØÁØ´Ù.
	//		°è»ê½Ã¿¡´Â int ÇüÀ¸·Î °è»êÀ» ¼öÇàÇÏ¿© ¿¬»ê½Ã ¼Õ½ÇÀ» ¸·´Â´Ù.
	//		°ª Àû¿ë½Ã¿¡´Â WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
	inline void UPDATE_POINT ( GLDWDATA &sPOINT, float &fELP_VAR, const float fVAR, const DWORD dwLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(sPOINT.dwNow) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
		if ( nNEWP < 0 )			nNEWP = 0;
		if ( nNEWP > UINT_MAX )	nNEWP = UINT_MAX;

		if ( nNEWP < dwLOW_LMT )		nNEWP = dwLOW_LMT;

		sPOINT.dwNow = nNEWP;
		sPOINT.LIMIT ();
	}

	//	Note : Ã¼·Â °°Àº ¼öÄ¡µéÀ» º¯È­À²¿¡ µû¶ó °»½ÅÇØÁØ´Ù.
	//		°è»ê½Ã¿¡´Â int ÇüÀ¸·Î °è»êÀ» ¼öÇàÇÏ¿© ¿¬»ê½Ã ¼Õ½ÇÀ» ¸·´Â´Ù.
	//		°ª Àû¿ë½Ã¿¡´Â WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
	inline void UPDATE_POINT ( DWORD &dwNowP, float &fELP_VAR, const DWORD MaxP, const float fVAR, const DWORD dwLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(dwNowP) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
		if ( nNEWP < 0 )				nNEWP = 0;
		if ( nNEWP < (int)dwLOW_LMT )	nNEWP = dwLOW_LMT;

		dwNowP = nNEWP;
		if ( dwNowP > MaxP )		dwNowP = MaxP;
	}

	inline void UPDATE_POINT ( WORD &wNowP, float &fELP_VAR, const WORD MaxP, const float fVAR, const WORD wLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(wNowP) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD Çü °ªÀÇ ¿µ¿ª ÀÌ³»·Î Á¦ÇÑ.
		if ( nNEWP < 0 )			nNEWP = 0;
		if ( nNEWP > USHRT_MAX )	nNEWP = USHRT_MAX;

		if ( nNEWP < wLOW_LMT )		nNEWP = wLOW_LMT;

		wNowP = nNEWP;
		if ( wNowP > MaxP )			wNowP = MaxP;
	}
};

enum EMBEGINATTACK_FB
{
	EMBEGINA_OK		= 0,
	EMBEGINA_SP		= 1,
	EMBEGINA_ARROW	= 2,
	EMBEGINA_BULLET	= 3, /*gun-bullet logic, Juver, 2017/05/27 */
};


//	Note : º¯¼ö Ãß°¡½Ã¿¡. RESET_DATA() ÇÔ¼ö¿¡ º¯¼ö ÃÊ±âÈ­ ³Ö¾î¾ßµÊ.
//
struct GLCHARLOGIC :public SCHARDATA2
{	
	EMCHARINDEX			m_CHARINDEX;
	DELAY_MAP			m_SKILLDELAY;					//	»ç¿ëµÈ ½ºÅ³ÀÇ µô·¡ÀÌ.

	SSKILLFACT			m_sSKILLFACT[SKILLFACT_SIZE];	//	½ºÅ³È¿°úµé.

	/*dual pet skill, Juver, 2017/12/27 */
	SSKILLFACT			m_sPETSKILLFACT_A;				//  ÆÖÀÇ º¸Á¶½ºÅ³È¿°ú
	SSKILLFACT			m_sPETSKILLFACT_B;				//  ÆÖÀÇ º¸Á¶½ºÅ³È¿°ú
	
	SLANDEFFECT			m_sLandEffect[EMLANDEFFECT_MULTI];	//  ÁöÇü ÀÌ»ó È¿°úµé

	DWORD				m_dwTransfromSkill;				//  º¯½Å ½ºÅ³ ³Ñ¹ö


	float				m_fHP_RATE;						//	active ½ºÅ³ÀÇ HP ÁõÆøÀ².
	float				m_fMP_RATE;						//	active ½ºÅ³ÀÇ MP ÁõÆøÀ².
	float				m_fSP_RATE;						//	active ½ºÅ³ÀÇ SP ÁõÆøÀ².

	DWORD				m_dwHOLDBLOW;					//	»óÅÂÀÌ»óÀÌ ¹æÁöµÊ.
	BOOL				m_bSTATEBLOW;					//	»óÅÂÀÌ»óÀÌ ÇÏ³ª ÀÌ»ó ¹ß»ýÇßÀ»¶§.
	SSTATEBLOW			m_sSTATEBLOWS[EMBLOW_MULTI];	//	»óÅÂÀÌ»ó.
	float				m_fSTATE_MOVE;					//	»óÅÂ ÀÌ»ó ÀÌµ¿¼Óµµ. ( 1.0f == 100% )
	float				m_fSTATE_DELAY;					//	»óÅÂ ÀÌ»ó µô·¡ÀÌ. ( 1.0f == 100% ) (+)
	float				m_fSTATE_DAMAGE;				//	»óÅÂ ÀÌ»ó ´ë¹ÌÁö Áõ°¡.

	SQITEM_FACT			m_sQITEMFACT;					//	? ¾ÆÀÌÅÛ Àû¿ë°ª.
	SEVENT_FACT			m_sEVENTFACT;					//	ÀÌº¥Æ® ? ¾ÆÀÌÅÛ Àû¿ë°ª.
	//SEventState			m_sEventState;

	SPKCOMBO_COUNT		m_sPKCOMBOCOUNT;


	bool				m_bSTATE_PANT;					//	»óÅÂÀÌ»ó Çæ¶±ÀÓ.
	bool				m_bSTATE_STUN;					//	»óÅÂÀÌ»ó ±âÀý.
	bool				m_bINVISIBLE;					//	Åõ¸í.
	bool				m_bRECVISIBLE;					//	Åõ¸í È®ÀÎ.

	SSTIGMA				m_sSTIGMA_USER;		// ³«ÀÎ È¿°ú - »ç¿ë.
	SSTIGMA				m_sSTIGMA_TAKER;		// ³«ÀÎ È¿°ú - Àû¿ë.
	SLINKHP					m_sLINKHP;					// °í¸® È¿°ú.
	SINCREASEEFF			m_sIncreaseEff;			// °­È­ È¿°ú;

	float				m_fSKILL_MOVE;					//	½ºÅ³ ÀÌµ¿¼Óµµ.
	float				m_fOPTION_MOVE;					//	Ãß°¡ÀûÀÎ ÀÌµ¿¼Óµµ. ¿ùµå¿¡µðÅÍ¿¡¼­ »ç¿ë.


	DEFENSE_SKILL		m_sDefenseSkill;				//  ¹ßµ¿Çü ½ºÅ³
	SCHARSTATS			m_sSUMSTATS;					//	ÇÕ»ê Stats.

	WORD				m_wSUM_AP;						//	±âº» °ø°Ý·Â.
	WORD				m_wSUM_DP;						//	±âº» ¹æ¾î·Â.

	WORD				m_wSUM_PA;						//	°ÝÅõÄ¡.
	WORD				m_wSUM_SA;						//	»ç°ÝÄ¡.
	WORD				m_wSUM_MA;						//	¸¶¹ýÄ¡.

	SRESIST				m_sSUMRESIST;					//	ÀúÇ×°ª. ( ¾ÆÀÌÅÛ + ÆÐ½Ãºê )
	SRESIST				m_sSUMRESIST_SKILL;				//  ½ºÅ³ ÀúÇ×°ª ( ¾ÆÀÌÅÛ + ÆÐ½Ãºê + ¹öÇÁ ) 
	SSUM_ITEM			m_sSUMITEM;						//	¾ÆÀÌÅÛ °¡»ê°ª.
	SPASSIVE_SKILL_DATA	m_sSUM_PASSIVE;					//	Æä½Ãºê °¡»ê°ª.

	int					m_nSUM_PIERCE;					//	°üÅë Á¤µµ ÇÕ»ê.
	float				m_fSUM_TARRANGE;				//	°ø°Ý °¡´É°Å¸® ÇÕ»ê.

	/*skill range spec logic, Juver, 2017/06/06 */
	float				m_fSUM_SKILL_ATTACKRANGE;
	float				m_fSUM_SKILL_APPLYRANGE;

	WORD				m_wACCEPTP;						//	¹«±â ¾ÆÀÌÅÛ¿¡ ´É·ÂÁ¦ÇÑ Çã¿ë ¼öÄ¡.
	WORD				m_wSUM_DisSP;					//	¼Ò¸ð SP ÇÕ»ê.

	int					m_nHIT;							//	¸íÁßÀ².
	int					m_nSUM_HIT;						//	¸íÁßÀ² + ½ºÅ³È¿°ú.
	int					m_nAVOID;						//	È¸ÇÇÀ².
	int					m_nSUM_AVOID;					//	È¸ÇÇÀ² + ½ºÅ³È¿°ú.

	int					m_nDEFENSE_BODY;				//	¹æ¾î·Â ( DP + DEX+°è¼ö ).
	int					m_nDEFENSE;						//	¹æ¾î·Â ( DP + DEX*°è¼ö + ITEM_DEF ) * °ø°£ÀûÀÀ º¸Á¤.
	int					m_nDEFENSE_SKILL;				//	¹æ¾î·Â ( DP + DEX*°è¼ö + ITEM_DEF ) * °ø°£ÀûÀÀ º¸Á¤ + ½ºÅ³.

	GLDWDATA			m_gdDAMAGE;						//	±âº» °ø°Ý·Â. ( AP + Æä½Ãºê ½ºÅ³·Î °¡»êµÈ °ø¾÷ )
	GLDWDATA			m_gdDAMAGE_SKILL;				//	±âº» °ø°Ý·Â + temp ½ºÅ³È¿°ú.
	GLDWDATA			m_gdDAMAGE_PHYSIC;				//	±âº» °ø°Ý·Â + ¹«±â°ø¾÷ + (°ÝÅõÄ¡or»ç°ÝÄ¡).

	float				m_fDamageRate;					//	°ø°Ý·Â º¯È­À².
	float				m_fDefenseRate;					//	¹æ¾î·Â º¯È­À².

	DAMAGE_SPEC			m_sDamageSpec;					//	µ¥¹ÌÁö Èí¼ö ¹× ¹Ý»ç

	float				m_fINCR_HP;						//	HP È¸º¹À².
	float				m_fINCR_MP;						//	MP È¸º¹À².
	float				m_fINCR_SP;						//	SP È¸º¹À².

	float				m_fATTVELO;						//	½ºÅ³ °ø°Ý¼Óµµ º¯È­.
	WORD				m_wATTRANGE;					//	°ø°Ý °¡´É °Å¸®.
	WORD				m_wSUM_ATTRANGE;				//	°ø°Ý °¡´É °Å¸®.
	GLITEM_ATT			m_emITEM_ATT;					//	¾ÆÀÌÅÆ °ø°Ý ¼Ó¼º.

	/*anti shit gs*/
	float				m_fSUM_TARRANGE_ALL;			// »çÁ¤°Å¸® Áõ°¡ ¿É¼Ç ( ÀüÃ¼ ½ºÅ³ Å¸ÀÔ ) 
	float				m_fSUM_APPLYRANGE_ALL;			// Àû¿ë¿µ¿ª Áõ°¡ ¿É¼Ç ( ÀüÃ¼ ½ºÅ³ Å¸ÀÔ )

	float				m_fSKILLDELAY;

	float				m_fEXP_RATE;					//	°æÇèÄ¡ ¼ö½ÅÀ².
	float				m_fGEN_RATE;					//	¾ÆÀÌÅÛ ¹ß»ýÀ².

	EMANI_SUBTYPE		m_emANISUBTYPE;					//	Item Âø¿ë¿¡ µû¸¥ ¿¡´Ï¸ÞÀÌ¼Ç Á¾·ù.

	SNATIVEID			m_idACTIVESKILL;				//	ÇöÁ¦ ¾×Æ¼ºêµÈ ½ºÅ³.
	EMANI_MAINTYPE		m_emANIMAINSKILL;				//	½ºÅ³¿¡ µû¸¥ ¿¡´Ï¸ÞÀÌ¼Ç.
	EMANI_SUBTYPE		m_emANISUBSKILL;				//	½ºÅ³¿¡ µû¸¥ ¿¡´Ï¸ÞÀÌ¼Ç.

	SITEM*				m_pITEMS[SLOT_TSIZE];

	BOOL				m_bUseArmSub;					// ±Ø°­ºÎ¸¦ À§ÇÑ º¸Á¶ ¹«±â »ç¿ë¿©ºÎ

	//	Note : È¸º¹µÇ´Â ¼öÄ¡ »êÃâ¿ë ÀÓ½Ã º¯¼ö.
	float				m_fIncHP;
	float				m_fIncMP;
	float				m_fIncSP;

	MAPPLAYHOSTILE		m_mapPlayHostile;				//	Àû´ë ÇàÀ§ÀÚ. ( ½Ö¹æ )

	float				m_fJumpRange;
	typedef std::map<DWORD, BOOL>			CHECKSET;
	typedef std::map<DWORD, BOOL>::iterator	CHECKSET_ITER;
	CHECKSET			m_mapCheckSet;//maison add ®M¸Ë¨t²Î
//	Å»°Í Ãß°¡
	BOOL				m_bVehicle;						//  Å»°Í È°¼ºÈ­ ¿©ºÎ
	float				m_fVehicleSpeedRate;
	float				m_fVehicleSpeedVol;

	float				m_fCrit_Dmg;//add critdmg
	float				m_fBlow_Dmg;//add blowdmg
	float				m_fCrit_Dmg_Skill;//add critdmgskill
	float				m_fBlow_Dmg_Skill;//add blowdmgskill
	float				m_fCrit_Rate;//add critrate
	float				m_fCrit_Rate_Skill;//add critrateskill
	float				m_fBlow_Rate;//add blowrate
	float				m_fBlow_Rate_Skill;//add blowrateskill
	float				m_fIncR_MoveSpeed;
	float				m_fIncR_AtkSpeed;

	float				m_fInc_MoveSpeed;
	float				m_fInc_AtkSpeed;
	
	bool				m_bSafeZone;

	bool				m_bProhibitPotion; /*prohibit potion skill logic, Juver, 2017/06/06 */
	bool				m_bProhibitSkill; /*prohibit skill logic, Juver, 2017/06/06 */

	/*continuous damage skill logic, Juver, 2017/06/10 */
	CONTINUOUS_DAMAGE_DATA_MAP	m_mapContinuousDamage;

	/*curse skill logic, Juver, 2017/06/10 */
	bool				m_bCurse;
	float				m_fCurseDamage;

	/*vehicle booster system, Juver, 2017/08/11 */
	bool				m_bBoosterCharge;
	bool				m_bBoosterStart;
	float				m_fBoosterTimer;

	int					m_nSummonTime; /*summon time, Juver, 2017/12/12 */

	bool				m_bIgnoreDamage; /*ignore damage, Juver, 2017/12/12 */
	/*block back damage, Juver, 2017/12/12 */
	float				m_fBlockBackDamageTime;
	bool				m_bBlockBackDamage;

	//damagecheck
	WORD				m_wDamageHistory;
	float				m_fDamageHistoryTimer;

	WORD				m_wAutoPotsAttemptCount;
	float				m_fAutoPotsAttemptTimer;
	bool				m_bActionType[SKILL::EMACTION_NSIZE];

	GLCHARLOGIC () :
		m_CHARINDEX(GLCI_BRAWLER_M),

		m_dwHOLDBLOW(NULL),
		m_bSTATEBLOW(FALSE),
		m_fSTATE_MOVE(1.0f),
		m_fSTATE_DELAY(1.0f),
		m_fSTATE_DAMAGE(1.0f),
		
		m_bSTATE_PANT(false),
		m_bSTATE_STUN(false),
		m_bINVISIBLE(false),
		m_bRECVISIBLE(false),

		m_fHP_RATE(0),
		m_fMP_RATE(0),
		m_fSP_RATE(0),

		m_fSKILL_MOVE(0.0f),
		m_fOPTION_MOVE(0.0f),

		m_wSUM_AP(0),
		m_wSUM_DP(0),
		m_wSUM_PA(0),
		m_wSUM_SA(0),
		m_wSUM_MA(0),

		m_nSUM_PIERCE(0),
		m_fSUM_TARRANGE(0),

		/*skill range spec logic, Juver, 2017/06/06 */
		m_fSUM_SKILL_ATTACKRANGE(0.0f),
		m_fSUM_SKILL_APPLYRANGE(0.0f),

		m_nHIT(0),
		m_nSUM_HIT(0),
		m_nAVOID(0),
		m_nSUM_AVOID(0),

		m_wACCEPTP(0),
		m_wSUM_DisSP(0),

		m_nDEFENSE(0),

		m_fDamageRate(1.0f),
		m_fDefenseRate(1.0f),

		m_fINCR_HP(0.0f),
		m_fINCR_MP(0.0f),
		m_fINCR_SP(0.0f),

		m_fIncHP(0.0f),
		m_fIncMP(0.0f),
		m_fIncSP(0.0f),

		m_fATTVELO(0.0f),
		m_wATTRANGE(4),
		m_wSUM_ATTRANGE(4),

		/*anti shit gs*/
		m_fSUM_TARRANGE_ALL(0),
		m_fSUM_APPLYRANGE_ALL(0),

		m_fSKILLDELAY(0.0f),

		m_fEXP_RATE(1),
		m_fGEN_RATE(1),

		m_emITEM_ATT(ITEMATT_NOTHING),

		m_emANISUBTYPE(AN_SUB_NONE),

		m_idACTIVESKILL(NATIVEID_NULL()),

		m_emANIMAINSKILL(AN_NONE),
		m_emANISUBSKILL(AN_SUB_NONE),

		m_bUseArmSub(FALSE),
		m_bVehicle(FALSE),
		m_fVehicleSpeedRate( 0.0f ),
		m_fVehicleSpeedVol( 0.0f ),
		m_bSafeZone(false),

		m_bProhibitPotion(false), /*prohibit potion skill logic, Juver, 2017/06/06 */
		m_bProhibitSkill(false), /*prohibit skill logic, Juver, 2017/06/06 */

		/*curse skill logic, Juver, 2017/06/10 */
		m_bCurse(false),
		m_fCurseDamage(0.0f),

		/*vehicle booster system, Juver, 2017/08/11 */
		m_bBoosterCharge(false),
		m_bBoosterStart(false),
		m_fBoosterTimer(0.0f),

		m_nSummonTime(0), /*summon time, Juver, 2017/12/12 */
		m_bIgnoreDamage(false), /*ignore damage, Juver, 2017/12/12 */
		m_fCrit_Dmg(0.0f),//add critdmg
		m_fBlow_Dmg(0.0f),//add blowdmg
		m_fCrit_Dmg_Skill(0.0f),//add critdmgskill
		m_fBlow_Dmg_Skill(0.0f),//add blowdmgskill
		m_fCrit_Rate(0.0f),//add critrate
		m_fCrit_Rate_Skill(0.0f),//add critrateskill
		m_fBlow_Rate(0.0f),//add blowrate
		m_fBlow_Rate_Skill(0.0f),//add blowrateskill
		m_fIncR_MoveSpeed(0.0f),
		m_fIncR_AtkSpeed(0.0f),
		m_fInc_MoveSpeed(0.0f),
		m_fInc_AtkSpeed(0.0f),

		/*block back damage, Juver, 2017/12/12 */
		m_fBlockBackDamageTime(0.0f),
		m_bBlockBackDamage(false),
		//damagecheck
		m_wDamageHistory(0),
		m_fDamageHistoryTimer(0.0f),
		m_wAutoPotsAttemptCount(0),
		m_fAutoPotsAttemptTimer(0.0f),
		m_fJumpRange(0)
	{
		memset ( m_pITEMS, 0x00, sizeof(SITEM*)*SLOT_TSIZE );
		memset ( m_bActionType, false, sizeof( bool ) * SKILL::EMACTION_NSIZE );
	}

	~GLCHARLOGIC ()
	{
		DEL_PLAYHOSTILE_ALL();
	}
public:
	float GetQuestionTime()			{ return m_sQITEMFACT.fTime; }
	float GetPKComboCountTime()		{ return m_sPKCOMBOCOUNT.fTime; }

public:
	EMCROW GETCROW () const			{ return CROW_PC; }
	WORD GETLEVEL () const			{ return m_wLevel; }
	
	DWORD GETHP () const			{ return m_sHP.dwNow; }
	DWORD GETMAXHP () const			{ return m_sHP.dwMax; }

	float GETMAXHPRATE() const      { return m_sLINKHP.fRef; }
	void  SETMAXHPRATE( float fRate ) { m_sLINKHP.fRef = fRate; }

	int GETMP () const				{ return m_sMP.dwNow; }
	int GETMAXMP () const			{ return m_sMP.dwMax; }

	int GETSP () const				{ return m_sSP.dwNow; }
	int GETMAXSP () const			{ return m_sSP.dwMax; }

	/*combatpoint logic, Juver, 2017/05/28 */
	int GETCP () const				{ return m_sCombatPoint.dwNow; }
	int GETMAXCP () const			{ return m_sCombatPoint.dwMax; }

	int GETHIT () const				{ return m_nSUM_HIT; }
	int GETAVOID () const			{ return m_nSUM_AVOID; }
	EMBRIGHT GETBRIGHT () const		{ if(m_nBright<0) return BRIGHT_DARK; return BRIGHT_LIGHT; }
	int GETBRIGHTPER () const		{ return m_nBright; }
	int GETFORCE_LOW () const		{ return m_gdDAMAGE_PHYSIC.dwLow; }
	int GETFORCE_HIGH () const		{ return m_gdDAMAGE_PHYSIC.dwHigh; }
	int GETDEFENSE () const			{ return m_nDEFENSE_SKILL; }

	/*Codex System, Jhoniex 2/14/2024 */
	int GETHPINC() const { return m_dwHPIncrease; }
	int GETMPINC() const { return m_dwMPIncrease; }
	int GETSPINC() const { return m_dwSPIncrease; }
	int GETATTINC() const { return m_dwAttackIncrease; }
	int GETDEFINC() const { return m_dwDefenseIncrease; }
	int GETSHOOTINC() const { return m_dwShootingIncrease; }
	int GETMELEEINC() const { return m_dwMeleeIncrease; }
	int GETENERGYINC() const { return m_dwEnergyIncrease; }
	int GETRESISTINC() const { return m_dwResistanceIncrease; }
	int GETHITRATEINC() const { return m_dwHitrateIncrease; }
	int GETAVOIDINC() const { return m_dwAvoidrateIncrease; }
	int GETHPPER() const { return m_dwHPIncreasePer; }
	
	float GETSUM_TARRANGE () const	{ return m_fSUM_TARRANGE; }		//	Àå°Å¸® ¹«±â(½ºÅ³)¿¡¸¸ °¡»ê.
	int GETSUM_PIERCE () const		{ return m_nSUM_PIERCE; }

	WORD GETBODYRADIUS () const		{ return GLCONST_CHAR::wBODYRADIUS; }
	WORD GETATTACKRANGE () const	{ return m_wSUM_ATTRANGE; }	

	/*anti shit gs*/
	float GETSUM_TARRANGE_ALL () const { return m_fSUM_TARRANGE_ALL; }
	float GETSUM_APPLYRANGE_ALL () const { return m_fSUM_APPLYRANGE_ALL; }

	const SCHARSTATS& GETSTATS_ADD () const;
	const SCHARSTATS& GETSTATS_ITEM () const;

	void OptionMoveUp();
	void OptionMoveDown();

	void SetUseArmSub( BOOL bSub )	{ m_bUseArmSub = bSub; }
	BOOL IsUseArmSub() const		{ return m_bUseArmSub; }

	EMSLOT GetCurRHand();
	EMSLOT GetCurLHand();

	BOOL IsCurUseArm( EMSLOT emSlot );

	bool IsDefenseSkill()				{ return m_sDefenseSkill.m_bActive; }
	void SetDefenseSkill(bool bDefense)	{ m_sDefenseSkill.m_bActive = bDefense; }

public:
	void SLOT_ITEM ( const SITEMCUSTOM &sItemCustom, EMSLOT _slot );
	void RELEASE_SLOT_ITEM ( EMSLOT _slot );

	const SITEMCUSTOM& GET_SLOT_ITEM ( EMSLOT _slot )				{ return m_PutOnItems[_slot]; }
	const SNATIVEID& GET_SLOT_NID ( EMSLOT _slot )					{ return m_PutOnItems[_slot].sNativeID; }
	SITEM* GET_SLOT_ITEMDATA ( EMSLOT _slot )						{ return m_pITEMS[_slot]; }
	BOOL VALID_SLOT_ITEM ( EMSLOT _slot );

	void HOLD_ITEM ( const SITEMCUSTOM &sItemCustom );
	void RELEASE_HOLD_ITEM ();
	const SITEMCUSTOM& GET_HOLD_ITEM ()					{ return m_PutOnItems[SLOT_HOLD]; }
	BOOL VALID_HOLD_ITEM ()								{ return GET_HOLD_ITEM().sNativeID!=NATIVEID_NULL(); }

	BOOL ISLONGRANGE_ARMS ();

	BOOL ISHAVEITEM ( SNATIVEID &sNID, DWORD *pNum=NULL );
	BOOL ISREVIVE ();

	SITEM* GET_ELMT_ITEM ();	//	¿Þ¼Õ, ¿À¸¥¼Õ ¾ÆÀÌÅÛÁß¿¡¼­ 'BLOW' ¼Ó¼ºÀÌ ÀÖ´Â ¾ÆÀÌÅÛÀ» ¹ÝÈ¯.

	WORD GETSKILLRANGE_TAR ( const GLSKILL &sSKILL ) const;
	WORD GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL ) const;

public:
	EMBEGINATTACK_FB BEGIN_ATTACK ( WORD wStrikeNum );
	BOOL VALID_LEVELUP () const;
	LONGLONG GET_LEVELUP_EXP () const;
	void LEVLEUP ( bool bInitNowExp = TRUE );
	void STATSUP ( EMSTATS emStats );
	//¬~ÂI¨t²Î
	bool RESET_STATS_TO(const DWORD dwDIS_STAT = UINT_MAX);
	//¬~¬õ¨t²Î
	bool RESET_STATS_TO1(const DWORD dwDIS_STAT = UINT_MAX);
	//¹CÀ¸¹ô§I´«¿n¤À¨t²Î

	void STATSUP ( SCHARSTATS sStats );
	void ShowRankName(BOOL bShowName); //MMR Rank
	void ShowRankMark(BOOL bShowMark); //MMR Rank
	/*Codex System, Jhoniex 2/14/2024 */
	void CODEX_STATS( int nIndex, DWORD dwStatPoint );

	void STATSUP_CMD ( EMSTATS emStats, DWORD value);		//add addstats cmd

	bool RESET_STATS_SKILL ( const WORD wDIS_STAT=USHRT_MAX );
	bool RESET_SKILL ();
	bool RESET_STATS ( const WORD wDIS=USHRT_MAX );

protected:
	void SUM_ITEM ();
	void SUM_PASSIVE ();
	void UPDATE_ITEMSETLIST();//MAISON add ®M¸Ë¨t²Î
	void SUM_ADDITION ( float fCONFT_POINT_RATE );

public:
	void INIT_NEW_CHAR ( const EMCHARINDEX _emCIndex, const DWORD dwUserID, const DWORD dwServerID, const char* szCharName,
		const WORD _wSchool=0, const WORD wHair=0, const WORD wFace=0, const WORD _wHairColor=0, const WORD _wSex=0, const float _fScaleRange=1.0f );

	BOOL INIT_DATA ( BOOL bNEW, BOOL bReGen, float fCONFT_POINT_RATE = 1.0f, bool bInitNowExp = TRUE );

	void INIT_RECOVER ( int nRECOVER=30 );

	void RESET_DATA ();

	void DoPkScore();
	void DoPkDeath();
	void DoPkReset();

public:
	void UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient=FALSE, float fCONFT_POINT_RATE=1.0f );
	void UPDATE_MAX_POINT ( float fCONFT_POINT_RATE );
	bool UPDATE_QITEMFACT ( float fElapsedTime );
	bool UPDATE_PKCOMBO ( float fElapsedTime );

public:
	BOOL CHECKSLOT_ITEM ( SNATIVEID sNativeID, EMSLOT emSlot );
	BOOL ACCEPT_ITEM ( SNATIVEID sNativeID );
	BOOL ACCEPT_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID );
	BOOL ISEMPTY_SLOT ( SNATIVEID sNativeID, EMSLOT emSlot );
	WORD CALC_ACCEPTP ( SNATIVEID sNativeID );
	BOOL SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID );
	BOOL SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID );

public:
	BOOL CHECKHIT ( const STARGETID &cTargetID, const GLLandMan* pLandMan, const BOOL bLowSP );

	/**
	* Ä³¸¯ÅÍ°¡ ¸÷À» °ø°ÝÇßÀ»¶§ µ¥¹ÌÁö °è»ê °ø½Ä
	*/
	DWORD CALCDAMAGE(
		int &rResultDAMAGE,
		const DWORD	dwGaeaID,
		const STARGETID &cTargetID,
		const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL,
		const DWORD dwskill_lev=0,
		const DWORD dwWeatherFlag=NULL,
		const int dwDivCount=0);
	
	DWORD CALCDAMAGE_20060328(
		int &rResultDAMAGE,
		const DWORD	dwGaeaID,
		const STARGETID &cTargetID,
		const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL,
		const DWORD dwskill_lev=0,
		const DWORD dwWeatherFlag=NULL,
		const int dwDivCount=0);

	DWORD CALCDAMAGE_2004(
		int &rResultDAMAGE,
		const DWORD	dwGaeaID,
		const STARGETID &cTargetID,
		const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL,
		const DWORD dwskill_lev=0,
		const DWORD dwWeatherFlag=NULL,
		const int dwDivCount=0);

	

	DWORD RECEIVE_DAMAGE ( const DWORD wDamage );

public:
	/*qbox check, Juver, 2017/12/05 */
	BOOL RECEIVE_QITEMFACT ( const SNATIVEID &nidITEM, BOOL bMobGen );
	//	Memo :	ÀÌº¥Æ®·Î ÀÎÇØ ¹Þ´Â ?¾ÆÀÌÅÛ È¿°ú¸¦ ¼³Á¤ÇÑ´Ù.
	void RECEIVE_EVENTFACT_BEGIN ( const EMGM_EVENT_TYPE emType, const WORD wValue );
	//	Memo :	ÀÌº¥Æ®·Î ÀÎÇØ ¹Þ´Â ?¾ÆÀÌÅÛ È¿°ú¸¦ ÇØÁ¦ÇÑ´Ù.
	void RECEIVE_EVENTFACT_END ( const EMGM_EVENT_TYPE emType );
	BOOL RECEIVE_PKCOMBO ( int nCount );

public:
	BOOL ISLEARNED_SKILL ( SNATIVEID skill_id, WORD wLEVEL=0 );
	BOOL ISMASTER_SKILL ( SNATIVEID skill_id );
	SCHARSKILL* GETLEARNED_SKILL ( SNATIVEID skill_id );

	EMSKILL_LEARNCHECK CHECKLEARNABLE_SKILL ( SNATIVEID skill_id );
	void LEARN_SKILL ( SNATIVEID skill_id );
	VOID LVLUP_SKILL ( SNATIVEID skill_id, WORD wToLevel );

	bool LEARN_SKILL_QUEST ( SNATIVEID skill_id );

public:
	float GETATTVELO ();
	float GETMOVEVELO ();
	float GETATT_ITEM (); // Àý´ëÄ¡ Áõ°¡
	float GETMOVE_ITEM (); // Àý´ëÄ¡ Áõ°¡ % 

public:
	//	Note : Àû´ëÀÚ µî·Ï. ( pk °ü·Ã ),	true : ½Å±Ô µî·Ï½Ã, false : ±âÁ¸ µî·Ï °»½Å½Ã.
	bool ADD_PLAYHOSTILE ( DWORD dwCHARID, BOOL bBAD );
	bool DEL_PLAYHOSTILE ( DWORD dwCHARID );
	void DEL_PLAYHOSTILE_ALL ();

public:
	//	Note : Àû´ë ¸®½ºÆ®¿¡ µî·Ï µÇ¾î ÀÖ´Â°¡?
	bool IS_PLAYHOSTILE ( DWORD dwCHARID );
	//	Note : Àû´ëÇàÀ§ÀÚ?
	bool IS_HOSTILE_ACTOR ( DWORD dwCHARID );
	//	Note : ÀÚ½ÅÀÌ ¹üÁËÀÚÀÎ°¡?
	bool ISOFFENDER ();

	bool ISPLAYKILLING ();

public:
	//	Note : ÇöÁ¦ pk ·¹º§À» ¾Ë¾Æº»´Ù. ( UINT_MAX ÀÏ°æ¿ì pk ÇØ´ç»çÇ× ¾øÀ½. )
	DWORD GET_PK_LEVEL ();

	//	Note : Á×À»¶§ °æÇèÄ¡ °¨¼Ò pk ¹è¼ö.
	float GET_PK_DECEXP_RATE ();
	std::string GET_PK_NAME ();
	DWORD GET_PK_COLOR ();
	float GET_PK_SHOP2BUY ();
	float GET_PK_SHOP2SALE ();

	DWORD GET_PK_ITEMDROP_NUM ();
	float GET_PK_ITEMDROP_RATE ();

public:
	BOOL DOGRINDING(SITEMCUSTOM &sCusItem, 
					const SITEM *pHold, 
					bool &bRESET, 
					bool &bTERMINATE, 
					EMANTIDISAPPEAR &emANTIDISAPPEAR );

public:
	BOOL ISSTATEBLOW ()							{ return m_bSTATEBLOW; }
	void DISABLESKEFF ( int i )					
	{ 
		CONTINUOUSDAMAGE_DELETE(m_sSKILLFACT[i].sNATIVEID);	/*continuous damage skill logic, Juver, 2017/06/10 */		
		m_sSKILLFACT[i].RESET(); 
	}
	void DISABLEBLOW ( int i )					{ m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE; }
	void RESETAGESKEFF ( int i )				{ m_sSKILLFACT[i].fAGE = 0.0f; }
	void RESETAGEBLOW ( int i )					{ m_sSTATEBLOWS[i].fAGE = 0.0f; }

	void DISABLEALLLANDEFF();
	void ADDLANDEFF( SLANDEFFECT landEffect, int iNum );


	void UPDATESKILLDELAY ( float fElapsedTime );

	void SETACTIVESKILL ( SNATIVEID skill_id );
	SNATIVEID GETACTIVESKILL ()					{ return m_idACTIVESKILL; }
	EMSKILLCHECK CHECHSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bNotLearn = false );
	void ACCOUNTSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bServer=false );

	void LEARN_SKILL_NONSCROLL ( SNATIVEID skill_id );
	
	/*continuous damage skill logic, Juver, 2017/06/10 */
	BOOL CONTINUOUSDAMAGE_EXIST( SNATIVEID _sID );
	void CONTINUOUSDAMAGE_INSERT( SCONTINUOUS_DAMAGE_DATA _sDATA );
	void CONTINUOUSDAMAGE_DELETE( SNATIVEID _sID );

	/*npc talk buff condition, Juver, 2017/10/03 */
	BOOL ISHAVE_BUFF( SNATIVEID skill_id, WORD wLEVEL=1 );

	int ISHAVE_BUFF( EMSPEC_ADDON emADDON );
	int ISHAVE_BUFF( EMIMPACT_ADDON emADDON );

	/*activity system, Juver, 2017/10/30 */
	SACTIVITY_CHAR_DATA*	GetActivityProg( DWORD dwActivityID );
	SACTIVITY_CHAR_DATA*	GetActivityDone( DWORD dwActivityID );
	DWORD	GetActivityProgNum();
	DWORD	GetActivityDoneNum();

	/*Codex System, Jhoniex 2/14/2024 */
	SCODEX_CHAR_DATA*	GetCodexProg( DWORD dwCodexID );
	SCODEX_CHAR_DATA*	GetCodexDone( DWORD dwCodexID );
	DWORD	GetCodexProgNum();
	DWORD	GetCodexDoneNum();

public:
	virtual void ReSelectAnimation ()	{}
};

//	Note : º¯¼ö Ãß°¡½Ã¿¡. RESET_DATA() ÇÔ¼ö¿¡ º¯¼ö ÃÊ±âÈ­ ³Ö¾î¾ßµÊ.
//
struct GLCROWLOGIC
{
	DWORD				m_dwGlobalID;				//	»ý¼º ¸Þ¸ð¸® ÀÎµ¦½º¿ë ( ÇØ´ç GLLandMan³»¿¡¼­ À¯´ÏÅ©ÇÔ )
	SNATIVEID			m_sNativeID;					//	Crow NativeID.
	PCROWDATA			m_pCrowData;					//	Crow ±âº» Á¤º¸.

	float				m_fACTIONDELAY[SCROWDATA::EMMAXATTACK];	//	µ¿ÀÛ cool time;

	SSKILLFACT			m_sSKILLFACT[SKILLFACT_SIZE];	//	½ºÅ³È¿°úµé.

	DWORD				m_dwHOLDBLOW;					//	»óÅÂÀÌ»óÀÌ ¹æÁöµÊ.

	SSTATEBLOW			m_sSTATEBLOWS[EMBLOW_MULTI];	//	»óÅÂÀÌ»ó.
	float				m_fSTATE_MOVE;					//	»óÅÂ ÀÌ»ó ÀÌµ¿¼Óµµ. ( 1.0f == 100% )
	float				m_fSTATE_DELAY;					//	»óÅÂ ÀÌ»ó µô·¡ÀÌ. ( 1.0f == 100% ) (+)
	float				m_fSTATE_DAMAGE;				//	»óÅÂ ÀÌ»ó ´ë¹ÌÁö Áõ°¡.
	
	bool				m_bSTATE_PANT;					//	»óÅÂÀÌ»ó Çæ¶±ÀÓ.
	bool				m_bSTATE_STUN;					//	»óÅÂÀÌ»ó ±âÀý.
	bool				m_bINVISIBLE;					//	Åõ¸í.
	bool				m_bRECVISIBLE;					//	Åõ¸í È®ÀÎ.

	SSTIGMA				m_sSTIGMA_USER;		// ³«ÀÎ È¿°ú - »ç¿ë.
	SSTIGMA				m_sSTIGMA_TAKER;		// ³«ÀÎ È¿°ú - Àû¿ë.
	SLINKHP				m_sLINKHP;					// °í¸® È¿°ú.
	SINCREASEEFF		m_sIncreaseEff;			// °­È­ È¿°ú;

	float				m_fATTVELO;						//	°ø°Ý¼Óµµ.
	float				m_fSKILL_MOVE;					//	½ºÅ³ ÀÌµ¿¼Óµµ.


	int					m_nSUM_HIT;						//	¸íÁßÀ² + ½ºÅ³È¿°ú.
	int					m_nSUM_AVOID;					//	È¸ÇÇÀ² + ½ºÅ³È¿°ú.

	int					m_nSUM_DEFENSE;					//	¹æ¾î·Â ( DP + POW*°è¼ö + ITEM_DEF ) * °ø°£ÀûÀÀ º¸Á¤ + ½ºÅ³.
	int					m_nDX_DAMAGE;					//	±âº» °ø°Ý·Â. + ½ºÅ³È¿°ú.
	
	SRESIST				m_sSUMRESIST;					//	ÀúÇ×°ª.

	int					m_nSUM_PIERCE;					//	Ãß°¡ °üÅë Á¤µµ.
	float				m_fSUM_TARRANGE;				//	°ø°Ý °¡´É°Å¸® ÇÕ»ê.

	/*skill range spec logic, Juver, 2017/06/06 */
	float				m_fSUM_SKILL_ATTACKRANGE;
	float				m_fSUM_SKILL_APPLYRANGE;

	float				m_fDamageRate;					//	°ø°Ý·Â º¯È­À².
	float				m_fDefenseRate;					//	¹æ¾î·Â º¯È­À².

	DAMAGE_SPEC			m_sDamageSpec;					//	µ¥¹ÌÁö Èí¼ö ¹× ¹Ý»ç
	DEFENSE_SKILL		m_sDefenseSkill;				//  ¹ßµ¿Çü ½ºÅ³

	DWORD				m_dwAType;						//	°ø°Ý ¼Ó¼º ¼ø¹ø.
	
	DWORD				m_dwNowHP;						//	»ý¸í·®.
	WORD				m_wNowMP;						//	Á¤½Å·®.
	WORD				m_wNowSP;						//	Á¤½Å·®.

	SNATIVEID			m_idACTIVESKILL;				//	È°¼º ½ºÅ³.
	WORD				m_wACTIVESKILL_LVL;				//	È°¼º ½ºÅ³  ·¡º§.

	//	Note : È¸º¹µÇ´Â ¼öÄ¡ »êÃâ¿ë ÀÓ½Ã º¯¼ö.
	float				m_fIncHP;
	float				m_fIncMP;
	float				m_fIncSP;

	//	Note : º¯¼ö Ãß°¡½Ã¿¡. RESET_DATA() ÇÔ¼ö¿¡ º¯¼ö ÃÊ±âÈ­ ³Ö¾î¾ßµÊ.
	//

	bool				m_bProhibitPotion; /*prohibit potion skill logic, Juver, 2017/06/06 */
	bool				m_bProhibitSkill; /*prohibit skill logic, Juver, 2017/06/06 */

	/*continuous damage skill logic, Juver, 2017/06/10 */
	CONTINUOUS_DAMAGE_DATA_MAP	m_mapContinuousDamage;

	/*curse skill logic, Juver, 2017/06/10 */
	bool				m_bCurse;
	float				m_fCurseDamage;

	/*pvp tyranny, Juver, 2017/08/25 */
	WORD				m_wTyrannyOwner;
	float				m_fTyrannyDamage[TYRANNY_SCHOOL_SIZE];

	bool				m_bIgnoreDamage; /*ignore damage, Juver, 2017/12/12 */

	/*block back damage, Juver, 2017/12/12 */
	float				m_fBlockBackDamageTime;
	bool				m_bBlockBackDamage;

	/*pvp capture the flag, Juver, 2018/02/06 */
	bool				m_bCaptureTheFlagCrowHide;

	GLCROWLOGIC () :
		m_dwGlobalID(0),
		m_sNativeID(0,0),
		m_pCrowData(NULL),

		m_dwHOLDBLOW(NULL),
		m_fSTATE_MOVE(1.0f),
		m_fSTATE_DELAY(1.0f),
		m_fSTATE_DAMAGE(1.0f),

		m_bSTATE_PANT(false),
		m_bSTATE_STUN(false),
		m_bINVISIBLE(false),
		m_bRECVISIBLE(false),

		m_fATTVELO(0.0f),
		m_fSKILL_MOVE(0.0f),

		m_nSUM_HIT(0),
		m_nSUM_AVOID(0),

		m_nSUM_DEFENSE(0),
		m_nDX_DAMAGE(0),

		m_fSUM_TARRANGE(0),
		m_nSUM_PIERCE(0),

		/*skill range spec logic, Juver, 2017/06/06 */
		m_fSUM_SKILL_ATTACKRANGE(0.0f),
		m_fSUM_SKILL_APPLYRANGE(0.0f),

		m_fDamageRate(1.0f),
		m_fDefenseRate(1.0f),

		m_dwAType(0),

		m_dwNowHP(0),
		m_wNowMP(0),
		m_wNowSP(0),
		
		m_idACTIVESKILL(false),
		m_wACTIVESKILL_LVL(0),

		m_fIncHP(0.0f),
		m_fIncMP(0.0f),
		m_fIncSP(0.0f),

		m_bProhibitPotion(false), /*prohibit potion skill logic, Juver, 2017/06/06 */
		m_bProhibitSkill(false), /*prohibit skill logic, Juver, 2017/06/06 */

		/*curse skill logic, Juver, 2017/06/10 */
		m_bCurse(false),
		m_fCurseDamage(0.0f),

		m_bIgnoreDamage(false), /*ignore damage, Juver, 2017/12/12 */

		/*block back damage, Juver, 2017/12/12 */
		m_fBlockBackDamageTime(0.0f),
		m_bBlockBackDamage(false),

		/*pvp tyranny, Juver, 2017/08/25 */
		m_wTyrannyOwner(TYRANNY_SCHOOL_SIZE),

		/*pvp capture the flag, Juver, 2018/02/06 */
		m_bCaptureTheFlagCrowHide(false)
	{
		for ( int i=0; i< SCROWDATA::EMMAXATTACK; ++i )
			m_fACTIONDELAY[i] = 0.0f;

		/*pvp tyranny, Juver, 2017/08/25 */
		for ( int i=0; i<TYRANNY_SCHOOL_SIZE; ++ i )
			m_fTyrannyDamage[i] = 0.0f;
	}

	void RESET_DATA ();

	bool VALIDACTION ()
	{
		const SCROWATTACK *pAttack = m_pCrowData->m_sCrowAttack;
		for ( int i=0; i<SCROWDATA::EMMAXATTACK; ++i )
		{
			if ( pAttack[i].bUsed )		return true;
		}

		return true;
	}

	bool VALIDSKILL ()
	{
		const SCROWATTACK *pAttack = m_pCrowData->m_sCrowAttack;
		for ( int i=0; i<SCROWDATA::EMMAXATTACK; ++i )
		{
			if ( pAttack[i].bUsed && pAttack[i].skill_id!=SNATIVEID(false) )		return true;
		}

		return true;
	}

	bool VALIDATTACK ()
	{
		const SCROWATTACK *pAttack = m_pCrowData->m_sCrowAttack;
		for ( int i=0; i<SCROWDATA::EMMAXATTACK; ++i )
		{
			if ( pAttack[i].bUsed && pAttack[i].skill_id==SNATIVEID(false) )		return true;
		}

		return true;
	}

	EMCROW GETCROW () const 	{ return m_pCrowData->m_emCrow; }
	WORD GETLEVEL () const		{ return m_pCrowData->m_wLevel; }
	const char* GETNAME ();
	
	DWORD GETHP () const		{ return m_dwNowHP; }
	DWORD GETMAXHP () const		{ return m_pCrowData->m_dwHP; }

	float GETMAXHPRATE() const;
	void  SETMAXHPRATE( float fRate );

	int GETMP () const			{ return m_wNowMP; }
	int GETMAXMP () const		{ return m_pCrowData->m_wMP; }

	int GETSP () const			{ return m_wNowSP; }
	int GETMAXSP () const		{ return m_pCrowData->m_wSP; }

	int GETHIT () const			{ return m_nSUM_HIT; }
	int GETAVOID () const		{ return m_nSUM_AVOID; }

	EMBRIGHT GETBRIGHT () const	{ return m_pCrowData->m_emBright; }

	int GETFORCE_LOW () const;
	int GETFORCE_HIGH () const;
	int GETDEFENSE () const			{ return m_nSUM_DEFENSE; }

	float GETSUM_TARRANGE () const	{ return m_fSUM_TARRANGE; }		//	Àå°Å¸® ¹«±â(½ºÅ³)¿¡¸¸ °¡»ê.
	int GETSUM_PIERCE () const		{ return m_nSUM_PIERCE; }

	WORD GETBODYRADIUS () const		{ return m_pCrowData->m_sAction.m_wBodyRadius; }
	WORD GETATTACKRANGE () const	{ return m_pCrowData->m_sCrowAttack[m_dwAType].wRange; }

	WORD GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL ) const;

	BOOL INIT_DATA ();

	BOOL CHECKHIT ( const STARGETID &cTargetID, const GLLandMan* pLandMan );
	DWORD CALCDAMAGE ( int& rResultDAMAGE, const DWORD dwGaeaID, const STARGETID &cTargetID, const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL, DWORD dwskill_lev=0, DWORD dwWeatherFlag=NULL, DWORD dwDivCount=0 );

	DWORD RECEIVE_DAMAGE ( const DWORD wDamage );

	
	void DISABLESKEFF ( int i )					
	{ 
		CONTINUOUSDAMAGE_DELETE(m_sSKILLFACT[i].sNATIVEID); /*continuous damage skill logic, Juver, 2017/06/10 */
		m_sSKILLFACT[i].sNATIVEID = NATIVEID_NULL(); 
	} 

	void DISABLEBLOW ( int i )					{ m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE; }

	void UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient=FALSE );

	void SETACTIVESKILL ( SNATIVEID skill_id, WORD wLevel );
	SNATIVEID GETACTIVESKILL ()						{ return m_idACTIVESKILL; }
	EMSKILLCHECK CHECHSKILL ( DWORD dwAType, bool bNotLearn = false );
	void ACCOUNTSKILL ( WORD wStrikeNum );

	float GETATTVELO ();
	float GETMOVEVELO ();

	bool IsDefenseSkill()				{ return m_sDefenseSkill.m_bActive; }
	void SetDefenseSkill(bool bDefense)	{ m_sDefenseSkill.m_bActive = bDefense; }

	/*continuous damage skill logic, Juver, 2017/06/10 */
	BOOL CONTINUOUSDAMAGE_EXIST( SNATIVEID _sID );
	void CONTINUOUSDAMAGE_INSERT( SCONTINUOUS_DAMAGE_DATA _sDATA );
	void CONTINUOUSDAMAGE_DELETE( SNATIVEID _sID );
	BOOL ISHAVE_BUFF( SNATIVEID skill_id, WORD wLEVEL=1 );

	int ISHAVE_BUFF( EMSPEC_ADDON emADDON );
	int ISHAVE_BUFF( EMIMPACT_ADDON emADDON );
};


EMANI_SUBTYPE CHECK_ANISUB ( SITEM* pRHAND, SITEM* pLHAND );
EMANI_SUBTYPE CHECK_ATTACK_ANISUB ( SITEM* pRHAND, SITEM* pLHAND );

//! ¾ÆÀÌÅÛ È¹µæ ±ÇÇÑÀ» ÁÖ±âÀ§ÇÑ...
//! µ¥¹ÌÁö ·Î±×¸¦ ±â·ÏÇÏ±â À§ÇÑ ±¸Á¶Ã¼
struct SDAMAGELOG
{
	DWORD dwUserID; ///< User ID
	DWORD dwGaeaID; ///< °¡ÀÌ¾Æ ID
	DWORD dwDamage; ///< ÁØ µ¥¹ÌÁö	

	SDAMAGELOG () 
		: dwUserID(0)
		, dwGaeaID(0)
		, dwDamage(0)
	{
	}

	SDAMAGELOG ( DWORD dwuserid, DWORD dwgaeaid, DWORD dwdamage ) 
		: dwUserID(dwuserid)
		, dwGaeaID(dwgaeaid)
		, dwDamage(dwdamage)
	{
	}
};

typedef std::map<DWORD, SDAMAGELOG>	DAMAGELOG;
typedef DAMAGELOG::iterator					DAMAGELOG_ITER;

inline void AddDamageLog ( DAMAGELOG &cDamageLog, DWORD dwGaeaID, DWORD dwUserID, DWORD dwDamage )
{
	DAMAGELOG_ITER iter = cDamageLog.find ( dwUserID );
	DAMAGELOG_ITER iter_end = cDamageLog.end();
	
	if ( iter != iter_end )
	{
		SDAMAGELOG &sDamageLog = (*iter).second;
		if ( sDamageLog.dwUserID == dwUserID )	dwDamage += sDamageLog.dwDamage;
	}
	
	cDamageLog[dwUserID] = SDAMAGELOG(dwUserID,dwGaeaID,dwDamage);
}

//! Ãß°¡:2006-04-12 Jgkim
//! ÆÄÆ¼ µ¥¹ÌÁö ·Î±×±â·ÏÀ» À§ÇÑ ±¸Á¶Ã¼
struct SDAMAGELOGPARTY
{
	DWORD m_dwPartyID; ///< ÆÄÆ¼ °íÀ¯¹øÈ£
	DWORD m_dwDamage; ///< ÆÄÆ¼°¡ ÁØ µ¥¹ÌÁö

	SDAMAGELOGPARTY()
		: m_dwPartyID(0)
		, m_dwDamage(0)
	{
	}

	SDAMAGELOGPARTY(DWORD dwPartyID, DWORD dwDamage)
		: m_dwPartyID(dwPartyID)
		, m_dwDamage(dwDamage)
	{
	}
};
typedef std::map<DWORD, SDAMAGELOGPARTY>	DAMAGELOGPARTY;
typedef DAMAGELOGPARTY::iterator					DAMAGELOGPARTY_ITER;

inline void AddDamageLogParty(DAMAGELOGPARTY &cDamageLogParty, DWORD dwPartyID, DWORD dwDamage)
{
	if (dwPartyID == PARTY_NULL) return;

	DAMAGELOGPARTY_ITER iter     = cDamageLogParty.find(dwPartyID);
	DAMAGELOGPARTY_ITER iter_end = cDamageLogParty.end();
	
	if (iter != iter_end)
	{
		SDAMAGELOGPARTY &sDamageLogParty = (*iter).second;
		if (sDamageLogParty.m_dwPartyID == dwPartyID)
		{
			dwDamage += sDamageLogParty.m_dwDamage;
		}
	}	
	cDamageLogParty[dwPartyID] = SDAMAGELOGPARTY(dwPartyID, dwDamage);
}

inline bool CHECHSKILL_ITEM ( GLSKILL_ATT emSKILL, GLITEM_ATT emITEM, const bool bHiddenWeapon )
{
	// ±Ø°­ºÎ ±Û·¯ºê Âø¿ë ½Ã ½ºÅ³ »ç¿ë °¡´É À¯¹«;
	if ( emITEM == ITEMATT_FIST && bHiddenWeapon )
	{
		return true;
	}

	switch ( emSKILL )
	{
	case SKILLATT_NOTHING:			
		return ( emITEM == ITEMATT_NOTHING );
	case SKILLATT_SWORD:			
		return ( emITEM == ITEMATT_SWORD );
	case SKILLATT_BLADE:			
		return ( emITEM == ITEMATT_BLADE );
	case SKILLATT_SWORDBLADE:		
		return ( emITEM == ITEMATT_SWORD || emITEM == ITEMATT_BLADE );
	case SKILLATT_DAGGER:			
		return ( emITEM == ITEMATT_DAGGER );
	case SKILLATT_SPEAR:			
		return ( emITEM == ITEMATT_SPEAR );
	case SKILLATT_STICK:			
		return ( emITEM == ITEMATT_STICK );
	case SKILLATT_GAUNT:			
		return ( emITEM == ITEMATT_GAUNT );
	case SKILLATT_BOW:				
		return ( emITEM == ITEMATT_BOW );
	case SKILLATT_THROW:			
		return ( emITEM == ITEMATT_THROW );

	case SKILLATT_FIST:
		return ( emITEM==ITEMATT_FIST );

		/*gun-bullet logic, Juver, 2017/05/27 */
	case SKILLATT_DUAL_GUN:			
		return ( emITEM == ITEMATT_GUN);
	case SKILLATT_RAIL_GUN:			
		return ( emITEM == ITEMATT_RAILGUN);
	case SKILLATT_PORTAL_GUN:			
		return ( emITEM == ITEMATT_PORTALGUN);
	case SKILLATT_SHOTGUN:			
		return ( emITEM == ITEMATT_RAILGUN || emITEM == ITEMATT_PORTALGUN );
	case SKILLATT_GUN:			
		return ( emITEM == ITEMATT_GUN || emITEM == ITEMATT_RAILGUN || emITEM == ITEMATT_PORTALGUN );

	case SKILLATT_SCYTHE:			
		return ( emITEM == ITEMATT_SCYTHE );

	case SKILLATT_DUALSPEAR:			
		return ( emITEM == ITEMATT_DUALSPEAR );

	case SKILLATT_SHURIKEN:			
		return ( emITEM == ITEMATT_SHURIKEN );

	case SKILLATT_NOCARE:			
		return ( emITEM == ITEMATT_NOCARE );
	};

	return	false;
}

/*vehicle system, Juver, 2017/08/07 */
EMANI_MAINTYPE	CHECK_VEHICLE_MAIN( SNATIVEID sVehicleID );
EMANI_SUBTYPE   CHECK_VEHICLE_SUB( EMANI_MAINTYPE emCharMainAni, bool bPassenger = FALSE );


D3DXVECTOR3 UpdateSkillDirection ( const D3DXVECTOR3 &vMyPos, const D3DXVECTOR3 vMyDir, const SNATIVEID sSKILLID, const D3DXVECTOR3 vTARPOS, const STARID *pTARID );


inline void GetRandomPos( D3DXVECTOR3* pDestPos, D3DXVECTOR3* pOrgPos, float fDist )
{
	D3DXVECTOR3			vDir;
	vDir.x = (((((float) rand())/RAND_MAX)*2.f)-1.f);
	vDir.y = (((((float) rand())/RAND_MAX)*2.f)-1.f);
	vDir.z = (((((float) rand())/RAND_MAX)*2.f)-1.f);

	D3DXVec3Normalize( &vDir, &vDir );

	pDestPos->x = pOrgPos->x + ( vDir.x * fDist );
	pDestPos->y = pOrgPos->y + ( vDir.y * fDist );
	pDestPos->z = pOrgPos->z + ( vDir.z * fDist );
} //GetRandomPos

#endif // GLOGIXEX_H_