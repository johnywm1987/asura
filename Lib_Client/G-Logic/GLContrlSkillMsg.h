#pragma once

#include "GLContrlBaseMsg.h"

namespace GLMSG
{
	#pragma pack(1)

	struct SNETPC_REQ_SKILLQUICK_SET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;
		SNATIVEID			skill_id;

		SNETPC_REQ_SKILLQUICK_SET () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_SKILLQUICK_SET);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_SET;
		}
	};

	struct SNETPC_REQ_SKILLQUICK_RESET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;

		SNETPC_REQ_SKILLQUICK_RESET () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_SKILLQUICK_RESET);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_RESET;
		}
	};

	struct SNETPC_REQ_SKILLQUICK_FB
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;
		SNATIVEID			skill_id;

		SNETPC_REQ_SKILLQUICK_FB () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_SKILLQUICK_FB);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_FB;
		}
	};

	struct SNETPC_REQ_SKILLQUICK_ACTIVE
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;

		SNETPC_REQ_SKILLQUICK_ACTIVE () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLQ_ACTIVE;
		}
	};

	struct SNETPC_REQ_ACTIONQUICK_SET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;
		WORD				wACT;

		SNETPC_REQ_ACTIONQUICK_SET () :
			wSLOT(0),
			wACT(EMACT_SLOT_DRUG)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_ACTIONQ_SET;
		}
	};

	struct SNETPC_REQ_ACTIONQUICK_RESET
	{
		NET_MSG_GENERIC		nmg;
		WORD				wSLOT;

		SNETPC_REQ_ACTIONQUICK_RESET () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_ACTIONQ_RESET;
		}
	};

	struct SNETPC_REQ_ACTIONQUICK_FB
	{
		NET_MSG_GENERIC		nmg;

		WORD				wSLOT;
		SACTION_SLOT		sACT;

		SNETPC_REQ_ACTIONQUICK_FB () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(SNETPC_REQ_ACTIONQUICK_FB);
			nmg.nType = NET_MSG_GCTRL_REQ_ACTIONQ_FB;
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_SKILL_CANCEL
	{
		NET_MSG_GENERIC		nmg;

		SNETPC_SKILL_CANCEL ()
		{
			nmg.dwSize = sizeof(SNETPC_SKILL_CANCEL);
			nmg.nType = NET_MSG_GCTRL_SKILL_CANCEL;
		}
	};

	//---------------------------------------------------------------------------NET
	struct SNETPC_SKILL_CANCEL_BRD : public SNETPC_BROAD
	{
		SNETPC_SKILL_CANCEL_BRD ()
		{
			nmg.dwSize = sizeof(SNETPC_SKILL_CANCEL_BRD);
			nmg.nType = NET_MSG_GCTRL_SKILL_CANCEL_BRD;
		}
	};

	struct SNETPC_REQ_SKILLUP
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			skill_id;

		SNETPC_REQ_SKILLUP () :
			skill_id(NATIVEID_NULL())
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLUP;
		}
	};

	struct SNETPC_REQ_SKILLUP_FB
	{
		NET_MSG_GENERIC		nmg;

		SCHARSKILL			sSkill;
		EMSKILL_LEARNCHECK	emCHECK;

		SNETPC_REQ_SKILLUP_FB ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILLUP_FB;
		}
	};

	struct SNETPC_REQ_SKILL
	{
		NET_MSG_GENERIC		nmg;
		SNATIVEID			skill_id;
		D3DXVECTOR3			vTARPOS;
		DWORD				dwFlags;
		bool				bDefenseSkill;			// �ߵ� ��ų

		/*dmk14 anti shit*/
		float				fSum_TarRange;
		WORD				wBodyRadius;
		WORD				wAttRange;
		WORD				wSkillRangeApply;
		WORD				wSkillRangeTar;

		//anti aspeed hack
		float				fAttVelo;
		float				fAttVeloItem;
		DWORD				dwAnimationKey;
		EMANI_MAINTYPE		emMType;
		EMANI_SUBTYPE		emSType;

		WORD				wTARGET_NUM;
		STARID				sTARGET_IDS[EMTARGET_NET];

		SNETPC_REQ_SKILL ()
			: skill_id(NATIVEID_NULL())
			, vTARPOS(0,0,0)
			, wTARGET_NUM(0)
			, dwFlags(NULL)
			, bDefenseSkill(false)
			/*dmk14 anti shit*/
			, fSum_TarRange(0.0f)
			, wBodyRadius(0)
			, wAttRange(0)
			, wSkillRangeApply(0)
			, wSkillRangeTar(0)
			//anti aspeed hack
			, fAttVelo(0.0f)
			, fAttVeloItem(0.0f)
			, dwAnimationKey(0)
			, emMType(AN_SKILL_A)
			, emSType(AN_SUB_00)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL;
		}

		void CALCSIZE ()
		{
			nmg.dwSize = sizeof(*this) - sizeof(STARID) * ( EMTARGET_NET - wTARGET_NUM );
		}

		BOOL ADDTARGET ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARGET_NUM==EMTARGET_NET )	return FALSE;

			sTARGET_IDS[wTARGET_NUM].wCrow = static_cast<EMCROW>(emCrow);
			sTARGET_IDS[wTARGET_NUM].wID = static_cast<WORD>(dwID);
			
			wTARGET_NUM++;
			CALCSIZE ();

			return TRUE;
		}

		BOOL ADDTARGET ( const STARID &starget )
		{
			return ADDTARGET(starget.GETCROW(),starget.GETID());
		}
	};

	struct SNETPC_REQ_SKILL_FB
	{
		NET_MSG_GENERIC		nmg;
		EMSKILLCHECK		emSKILL_FB;
		char				szName[CHAR_SZNAME];

		SNETPC_REQ_SKILL_FB () :
			emSKILL_FB(EMSKILL_OK)
		{
			ZeroMemory( szName, CHAR_SZNAME );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_FB;
		}
	};

	struct SNET_MSG_REQ_SKILL_REVIVEL_FAILED
	{
		NET_MSG_GENERIC		nmg;
		char				szName[CHAR_SZNAME];

		SNET_MSG_REQ_SKILL_REVIVEL_FAILED () 
		{
			ZeroMemory( szName, CHAR_SZNAME );
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_REQ_SKILL_REVIVEL_FAILED;
		}
	};

	struct SNETPC_REQ_SKILL_BRD
	{
		NET_MSG_GENERIC		nmg;

		EMCROW				emCrow;
		DWORD				dwID;

		SNATIVEID			skill_id;
		WORD				wLEVEL;

		D3DXVECTOR3			vTARPOS;

		WORD				wTARNUM;
		STARID				sTARIDS[EMTARGET_NET];

		SNETPC_REQ_SKILL_BRD () :
			emCrow(CROW_PC),
			dwID(0),
			skill_id(NATIVEID_NULL()),
			wLEVEL(0),

			vTARPOS(0,0,0),
			wTARNUM(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SKILL_BRD;
		}

		void CALCSIZE ()
		{
			nmg.dwSize = sizeof(*this) - sizeof(STARID) * ( EMTARGET_NET - wTARNUM );
		}

		BOOL ADDTARGET ( EMCROW emCrow, DWORD dwID )
		{
			if ( wTARNUM==EMTARGET_NET )	return FALSE;

			sTARIDS[wTARNUM].wCrow = static_cast<EMCROW>(emCrow);
			sTARIDS[wTARNUM].wID = static_cast<WORD>(dwID);
			
			wTARNUM++;
			CALCSIZE ();

			return TRUE;
		}
		BOOL ADDTARGET ( const STARID &starget )
		{
			return ADDTARGET(starget.GETCROW(),starget.GETID());
		}
	};

	struct SNETPC_SKILLFACT_BRD : public SNETCROW_BROAD
	{
		STARID				sACTOR;

		DWORD				dwDamageFlag;
		int					nVAR_HP;
		short				nVAR_MP;
		short				nVAR_SP;

		SNETPC_SKILLFACT_BRD () :
			dwDamageFlag( DAMAGE_TYPE_NONE ),
			nVAR_HP(0),
			nVAR_MP(0),
			nVAR_SP(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLFACT_BRD;
		}
	};

	struct SNETPC_SKILLHOLD_BRD : public SNETCROW_BROAD
	{
		SNATIVEID			skill_id;
		WORD				wLEVEL;
		WORD				wSELSLOT;
		float				fPARAM;

		WORD				wCasterCrow;
		DWORD				dwCasterID;

		SNETPC_SKILLHOLD_BRD () :
			skill_id(NATIVEID_NULL()),
			wLEVEL(0),
			wSELSLOT(0),
			wCasterCrow(0),
			dwCasterID(NATIVEID_NULL().dwID),
			fPARAM(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLHOLD_BRD;
		}
	};

	struct SNETPC_SKILLHOLD_RS_BRD : public SNETCROW_BROAD
	{
		bool bRESET[SKILLFACT_SIZE];

		SNETPC_SKILLHOLD_RS_BRD ()
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLHOLD_RS_BRD;

			memset ( bRESET, 0, sizeof(bool)*SKILLFACT_SIZE );
		}
	};

	struct SNETPC_REQ_SKILLHOLDEX_BRD : public SNETCROW_BROAD
	{
		WORD				wSLOT;
		SSKILLFACT			sSKILLEF;

		SNETPC_REQ_SKILLHOLDEX_BRD () :
			wSLOT(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLHOLDEX_BRD;
		}
	};

	struct SNETPC_STATEBLOW_BRD : public SNETCROW_BROAD
	{
		EMSTATE_BLOW		emBLOW;
		float				fAGE;
		float				fSTATE_VAR1;
		float				fSTATE_VAR2;

		SNETPC_STATEBLOW_BRD() :
			emBLOW(EMBLOW_NONE),
			fAGE(0),
			fSTATE_VAR1(0),
			fSTATE_VAR2(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_STATEBLOW_BRD;
		}
	};

	struct SNETPC_CURESTATEBLOW_BRD : public SNETCROW_BROAD
	{
		DWORD				dwCUREFLAG;

		SNETPC_CURESTATEBLOW_BRD () :
			dwCUREFLAG(NULL)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_CURESTATEBLOW_BRD;
		}
	};

	struct SNETPC_SKILLCONSUME_FB
	{
		NET_MSG_GENERIC		nmg;

		WORD				wTurnNum;	// �Ҹ��� �������� ��밡�� Ƚ��.
		DWORD				dwNowHP;
		DWORD				dwNowMP;
		DWORD				dwNowSP;
		DWORD				dwNowCP; /*combatpoint logic, Juver, 2017/05/28 */

		SNETPC_SKILLCONSUME_FB () :
			wTurnNum(0),
			dwNowHP(0),
			dwNowMP(0),
			dwNowSP(0),
			dwNowCP(0) /*combatpoint logic, Juver, 2017/05/28 */
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SKILLCONSUME_FB;
		}
	};

	// Revert to default structure packing
	#pragma pack()
};
