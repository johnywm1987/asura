#include "stdafx.h"
#include "./GLCharacter.h"
#include "./GLGaeaClient.h"
#include "./GLPartyClient.h"

#include "../DxGlobalStage.h"

#include "DxInputDevice.h"
#include "editmeshs.h"
#include "DxMethods.h"
#include "DxViewPort.h"
#include "DxShadowMap.h"
#include "EditMeshs.h"
#include "GLogicData.h"
#include "../Lib_ClientUI/Interface/ModalCallerID.h"
#include "../Lib_ClientUI/Interface/ModalWindow.h"
#include "../Lib_ClientUI/Interface/DamageDisplay.h"
#include "stl_Func.h"

#include "GLFactEffect.h"
#include "GlFriendClient.h"
#include "GLCrowRenList.h"
#include "../Lib_ClientUI/Interface/NameDisplayMan.h"
#include "../Lib_ClientUI/Interface/ItemRebuild.h"
#include "../Lib_ClientUI/Interface/ItemRebuildOption.h"

#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/Interface/MapRequireCheck.h"
#include "../Lib_ClientUI/Interface/CharacterWindow.h"
#include "../Lib_ClientUI/Interface/QBoxButton.h"
#include "../Lib_ClientUI/Interface/ItemSearchResultWindow.h"
#include "../../Lib_Engine/DxSound/DxSoundLib.h"
#include "../../Lib_Engine/DxEffect/DxEffectMan.h"
#include "../../Lib_Engine/DxEffect/Single/DxEffGroupPlayer.h"
#include "../Lib_ClientUI/Interface/WaitServerDialogue.h"
#include "../Lib_ClientUI/Interface/BasicGameMenu.h"
#include "../Lib_ClientUI/Interface/ItemShopIconMan.h"

/*game stats, Juver, 2017/06/21 */
#include "../Lib_ClientUI/Interface/GameStats.h"

/*charinfoview , Juver, 2017/11/11 */
#include "../Lib_ClientUI/Interface/CharacterInfoViewWindow.h"

/////////////////////////////////////////////////////////////////////////////
// LG-7 GlobalRanking
#include "../Lib_ClientUI/Interface/GlobalRankingWindow/GlobalRankingWindow.h"
/////////////////////////////////////////////////////////////////////////////
#include "../Lib_ClientUI/Interface/LargeMapWindow.h"
/* 3-12-2024Boss Spawn Viewer, Bubu */
#include "../Lib_ClientUI/Interface/BossdetailWIndow/BossDetailsWindow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HRESULT GLCharacter::UpdateClientState ( GLMSG::SNETPC_UPDATE_STATE *pNetMsg )
{
	//pNetMsg->DECODE ( m_dwGaeaID );

	/*if( !GetConfting().IsCONFRONTING() && !IsDie() && !m_bVehicle && IsValidBody())
	{
		GLMSG::SNETPC_HP_CHECK NetMsg;
		NetMsg.m_sClientHP = m_sHP;
		NetMsg.m_wClientLevel = m_wLevel;
		NetMsg.m_wLastHPCounter = m_wLastHPCounter;
		NETSENDTOFIELD ( &NetMsg );
	}*/

	m_sHP = pNetMsg->sHP;
	m_sMP = pNetMsg->sMP;
	m_sSP = pNetMsg->sSP;
	m_bSafeTime = pNetMsg->bSafeTime;
	m_sCombatPoint = pNetMsg->sCP; /*combatpoint logic, Juver, 2017/05/28 */

	m_fIncHP = 0.0f;
	m_fIncMP = 0.0f;
	m_fIncSP = 0.0f;

	//mismatch check
	if ( strcmp( pNetMsg->szCharName, m_szName ) != 0 )
	{
		m_dwMismatchCount ++;
	}

	//gaeaid check
	if( pNetMsg->dwCharGaeaID != m_dwGaeaID )
	{
		m_dwMismatchCount ++;
	}

	if ( m_dwMismatchCount >= 4 )
	{
		DxGlobalStage::GetInstance().GetNetClient()->CloseConnect();
	}

	return S_OK;
}

void GLCharacter::DisableSkillFact()
{

	EMSLOT emRHand = GetCurRHand();	
	SITEM* pRightItem = GET_SLOT_ITEMDATA(emRHand);	

	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
	
        PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
		if ( !pSkill ) continue;

		// Ω∫≈≥ ¿⁄Ω≈ πˆ«¡
		if ( pSkill->m_sBASIC.emIMPACT_TAR != TAR_SELF || pSkill->m_sBASIC.emIMPACT_REALM != REALM_SELF ) continue;
				
		const bool bExtremeWeapon	= pSkill->m_sLEARN.bHiddenWeapon;
		GLSKILL_ATT emSKILL_RITEM = pSkill->m_sBASIC.emUSE_RITEM;

		// Ω∫≈≥ µµ±∏ ¡æº” æ¯¿Ω
		if ( emSKILL_RITEM == SKILLATT_NOCARE )	continue;

		// Ω∫≈≥ ∞˙ π´±‚∞° ∫“¿œƒ°
		if( !pRightItem || !CHECHSKILL_ITEM(emSKILL_RITEM,pRightItem->sSuitOp.emAttack,bExtremeWeapon) )
		{
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT[i].sNATIVEID );
			DISABLESKEFF( i );
		}
	}
}


void GLCharacter::ReceiveDamage ( DWORD dwDamage, DWORD dwDamageFlag, STARGETID sACTOR )
{
	if ( dwDamageFlag & DAMAGE_TYPE_SHOCK )	ReceiveShock ();
	
	//	Note : »≠∏Èø° ≈∏∞› ∞™ √‚∑¬.
	D3DXVECTOR3 vPos = GetPosBodyHeight();
	CInnerInterface::GetInstance().SetDamage( vPos, dwDamage, dwDamageFlag, UI_UNDERATTACK );

	if ( dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW )
	{
		// ∞≠«—≈∏∞› ¿Ã∆Â∆Æ
		D3DXVECTOR3 vDIR = sACTOR.vPos - m_vPos;

		D3DXVECTOR3 vDIR_ORG(1,0,0);
		float fdir_y = DXGetThetaYFromDirection ( vDIR, vDIR_ORG );

		D3DXMATRIX matTrans;
		D3DXMatrixRotationY ( &matTrans, fdir_y );
		matTrans._41 = m_vPos.x;
		matTrans._42 = m_vPos.y + 10.0f;
		matTrans._43 = m_vPos.z;

		//	Note : ¿⁄±‚ ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
		DxEffGroupPlayer::GetInstance().NewEffGroup ( GLCONST_CHAR::strCRUSHING_BLOW_EFFECT.c_str(), matTrans, &sACTOR );
	}

	//	Note : ≈∏∞›∞™ π›øµ.
	GLCHARLOGIC::RECEIVE_DAMAGE ( dwDamage );

	//	Note : πÊæÓΩ∫≈≥¿« ¿Ã∆Â∆Æ∞° ¿÷¿ª∂ß πﬂµøΩ√≈¥.
	SKT_EFF_HOLDOUT ( sACTOR, dwDamageFlag );
}

void GLCharacter::ReceiveAVoid ()
{
	D3DXVECTOR3 vPos = GetPosBodyHeight();
	CInnerInterface::GetInstance().SetDamage( vPos, 0, DAMAGE_TYPE_NONE, UI_ATTACK );
}

void GLCharacter::ReceiveSwing ()
{
	if ( IsValidBody() && !IsCtrlBlockBody() )		m_pSkinChar->DOSHOCKMIX();
}

void GLCharacter::ReceiveShock ()
{
	if ( IsValidBody() && !IsCtrlBlockBody() )		TurnAction ( GLAT_SHOCK );
}

void GLCharacter::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	switch ( nmg->nType )
	{
		// add this
// KILL FEED
	case NET_MSG_GCTRL_KILLFEED_FB:
	{
		GLMSG::SNET_KILLFEED_SELECT_FB* pNetMsg = (GLMSG::SNET_KILLFEED_SELECT_FB*)nmg;
		switch (pNetMsg->emFB)
		{
		case EMKILL_FEED_SELECT_ALREADY:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMKILL_FEED_SELECT_ALREADY"));
			break;
		case EMKILL_FEED_SELECT_OK:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMKILL_FEED_SELECT_OK"));
			CInnerInterface::GetInstance().HideGroup(STYLE_KILL_FEED_WINDOW);
			break;
		case EMKILL_FEED_SELECT_NOITEM:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMKILL_FEED_SELECT_NOITEM"));
			break;
		case EMKILL_FEED_SELECT_NOTITEM:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMKILL_FEED_SELECT_NOTITEM"));
			break;
		case EMKILL_FEED_SELECT_REQUIRE:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::FORESTGREEN, ID2GAMEINTEXT("EMKILL_FEED_SELECT_REQUIRE"));
			break;
		}
	}
	break;
	case NET_MSG_GCTRL_REMOVE_SKILL_EFFECT_BRD:
		{
			GLMSG::SNETPC_REMOVE_SKILL_EFFECT_BRD *pNetMsg = (GLMSG::SNETPC_REMOVE_SKILL_EFFECT_BRD *)nmg;
			
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT[pNetMsg->dwSkillID].sNATIVEID );

			DISABLESKEFF(pNetMsg->dwSkillID);
		}
		break;

		/////////////////////////////////////////////////////////////////////////////
	//¨~¬I®t≤Œ
	case NET_MSG_GCTRL_INVEN_RESET_SKST_TO_FB:
	{
		GLMSG::SNET_INVEN_RESET_SKST_TO_FB* pNetMsg = (GLMSG::SNET_INVEN_RESET_SKST_TO_FB*)nmg;

		switch (pNetMsg->emFB)
		{
		case EMREQ_RESET_SKST_TO_FB_FAIL:
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_FAIL"));
			break;
		case EMREQ_RESET_SKST_TO_FB_OK:
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_OK"));
			RESET_STATS_TO(pNetMsg->wSTATS_POINT);
			break;
		case EMREQ_RESET_SKST_TO_FB_MONEY:
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_MONEY"), GLCONST_CHAR::lnMoney1);
			break;
			//			case EMREQ_RESET_SKST_TO_FB_MONEY2:
			//				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_MONEY2"), GLCONST_CHAR::lnMoney2 );
			//	break;
			//	case EMREQ_RESET_SKST_TO_FB_MONEY3:
			//				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_MONEY3"), GLCONST_CHAR::lnMoney3 );
			//		break;
		};
	}
	break;

	//¨~¨ı®t≤Œ
	case NET_MSG_GCTRL_INVEN_RESET_SKST_TO_FB1:
	{
		GLMSG::SNET_INVEN_RESET_SKST_TO_FB1* pNetMsg = (GLMSG::SNET_INVEN_RESET_SKST_TO_FB1*)nmg;

		switch (pNetMsg->emFB)
		{
		case EMREQ_RESET_SKST_TO_FB1_FAIL:
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_FAIL"));
			break;
		case EMREQ_RESET_SKST_TO_FB1_OK:
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_OK"));
			RESET_STATS_TO1(pNetMsg->wBRIGHT_POINT);
			break;
			//	case EMREQ_RESET_SKST_TO_FB_MONEY:
			//				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_MONEY"), GLCONST_CHAR::lnMoney1 );
			//		break;
			//			case EMREQ_RESET_SKST_TO_FB_MONEY2:
			//				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_MONEY2"), GLCONST_CHAR::lnMoney2 );
			//	break;
			//	case EMREQ_RESET_SKST_TO_FB_MONEY3:
			//				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_TO_FB_MONEY3"), GLCONST_CHAR::lnMoney3 );
			//		break;
		};
	}
	break;
	/////////////////////////////////////////////////////////////////////////////
	// 
	/////////////////////////////////////////////////////////////////////////////
	// LG-7 GlobalRanking
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_FB:
	{
		GLMSG::SNETPC_REQ_GLOBAL_RANKING_FB* pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_FB*)nmg;

		if (pNetMsg->emFB > EM_GLOBAL_RANKING_FB_NONE)
		{
			switch (pNetMsg->emFB)
			{
			case EM_GLOBAL_RANKING_FB_DELAY:
			{
				CString strTemp;
				strTemp.Format((char*)ID2GAMEWORD("GLOBAL_RANKING_STAGE", 0), pNetMsg->fDelay);

				DoModal(strTemp);
			}
			break;
			}
		}
	}
	break;

	/////////////////////////////////////////////////////////////////////////////
	/*PartySearch - Added By Bubu*/
	case NET_MSG_GCTRL_GET_PARTYLIST_FB:
		{
			GLMSG::SNET_GET_PARTYLIST_FB* pNetMsg = (GLMSG::SNET_GET_PARTYLIST_FB*)nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREQ_PARTYLIST_FB_PARTY_DELAY:
				{
					m_vecPartySearch.clear();
					CString strTemp;
					strTemp.Format("Party Search cooldown time.");

					DoModal(strTemp);
				}
				break;
			case EMREQ_PARTYLIST_FB_CLEAR:
				m_vecPartySearch.clear();
				break;
			case EMREQ_PARTYLIST_FB_END:
				CInnerInterface::GetInstance().SetPartyFinderInfo ();
				break;
			case EMREQ_PARTYLIST_FB_ADD:
				{
					int nIndex = pNetMsg->sParty.dwIndex;
					int nSize = m_vecPartySearch.size();

					if ( nIndex < nSize )
					{
						m_vecPartySearch[nIndex] = pNetMsg->sParty;
					}
					else
					{
						m_vecPartySearch.resize( nIndex+1 );
						m_vecPartySearch[nIndex] = pNetMsg->sParty;
					}
				}
				break;
			case EMREQ_PARTYLIST_FB_UPDATE:
				{
					int nIndex = pNetMsg->sParty.dwIndex;
					m_vecPartySearch[nIndex] = pNetMsg->sParty;					
				}
				break;
			}			
		}
		break;
		
	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_KILL_AGT:
	{
		GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_AGT* pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_KILL_AGT*)nmg;

		if (pNetMsg->emFB > EM_GLOBAL_RANKING_FB_DELAY)
				CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetRankingKill(pNetMsg->emFB, pNetMsg->wPart, pNetMsg->sTopKill, pNetMsg->nIndex );
	}
	break;

	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_RICH_AGT:
	{
		GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_AGT* pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_RICH_AGT*)nmg;

		if (pNetMsg->emFB > EM_GLOBAL_RANKING_FB_DELAY)
			for (int i = 0; i < 10; i++)
				if (pNetMsg->sTopRich_[i].dwChaNum > 0)
					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetRankingRich(pNetMsg->wPart, pNetMsg->sTopRich_[i], i);
	}
	break;

	case NET_MSG_GCTRL_REQ_GLOBAL_RANKING_GUILD_AGT:
	{
		GLMSG::SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT* pNetMsg = (GLMSG::SNETPC_REQ_GLOBAL_RANKING_GUILD_AGT*)nmg;

		if (pNetMsg->emFB > EM_GLOBAL_RANKING_FB_DELAY)
			for (int i = 0; i < 10; i++)
				if (pNetMsg->sTopGuild_[i].wGuNum > 0)
					CInnerInterface::GetInstance().GetGlobalRankingWindow()->SetRankingGuild(pNetMsg->wPart, pNetMsg->sTopGuild_[i], i);
	}
	break;
	/////////////////////////////////////////////////////////////////////////////
	case NET_MSG_GCTRL_SUMMON_ATTACK_AVOID:
		{
			GLMSG::SNET_SUMMON_ATTACK_AVOID *pNetMsg = (GLMSG::SNET_SUMMON_ATTACK_AVOID *) nmg;
			GLCOPY* pActor = GLGaeaClient::GetInstance().GetCopyActor ( pNetMsg->emTarCrow, pNetMsg->dwTarID );
			if ( pActor )	pActor->ReceiveAVoid ();
		}
		break;
	case NET_MSG_GCTRL_SUMMON_ATTACK_DAMAGE:
		{
			GLMSG::SNET_SUMMON_ATTACK_DAMAGE *pNetMsg = (GLMSG::SNET_SUMMON_ATTACK_DAMAGE *) nmg;

			GLCOPY* pActor = GLGaeaClient::GetInstance().GetCopyActor ( pNetMsg->emTarCrow, pNetMsg->dwTarID );
			if ( pActor )
			{
				STARGETID sACTOR(GetCrow(),GetCtrlID(),GetPosition());
				//pActor->ReceiveDamage ( pNetMsg->nDamage, pNetMsg->bShock, pNetMsg->bCritical, sACTOR );
				pActor->ReceiveDamage ( pNetMsg->nDamage, pNetMsg->dwDamageFlag, sACTOR );
			}

			if ( (pNetMsg->dwDamageFlag & DAMAGE_TYPE_CRITICAL ) || ( pNetMsg->dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW ) )	
				DxViewPort::GetInstance().CameraQuake ( 0.5f, 1.0f/25.0f, 1.0f );
		}
		break;
	
		/* 3-12-2024Boss Spawn Viewer, Bubu */
	case NET_MSG_GCTRL_BOSS_DETAIL_FB:
	{
		GLMSG::SNETPC_BOSS_DETAILS_FB* pNetMsg = (GLMSG::SNETPC_BOSS_DETAILS_FB*)nmg;
		CInnerInterface::GetInstance().GetBossDetailsWindow()->SetSlotItem(pNetMsg->sMobID, pNetMsg->fTimer, pNetMsg->bIsAlive);
	}break;

	/////////////////////////////////////////////////////////////////////////////
	case NET_MSG_GCTRL_ATTACK_AVOID:
		{
			GLMSG::SNETPC_ATTACK_AVOID *pNetMsg = (GLMSG::SNETPC_ATTACK_AVOID *) nmg;
			GLCOPY* pActor = GLGaeaClient::GetInstance().GetCopyActor ( pNetMsg->emTarCrow, pNetMsg->dwTarID );
			if ( pActor )	pActor->ReceiveAVoid ();
		}
		break;

	case NET_MSG_GCTRL_ATTACK_DAMAGE:
		{
			GLMSG::SNETPC_ATTACK_DAMAGE *pNetMsg = (GLMSG::SNETPC_ATTACK_DAMAGE *) nmg;
			
			GLCOPY* pActor = GLGaeaClient::GetInstance().GetCopyActor ( pNetMsg->emTarCrow, pNetMsg->dwTarID );
			if ( pActor )
			{
				STARGETID sACTOR(GetCrow(),GetCtrlID(),GetPosition());	
				pActor->ReceiveDamage ( pNetMsg->nDamage, pNetMsg->dwDamageFlag, sACTOR );
			}

			if ( (pNetMsg->dwDamageFlag & DAMAGE_TYPE_CRITICAL ) || ( pNetMsg->dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW ) )	
				DxViewPort::GetInstance().CameraQuake ( 0.5f, 1.0f/25.0f, 1.0f );
		}
		break;

	case NET_MSG_GCTRL_DEFENSE_SKILL_ACTIVE:
		{
			MsgDefenseSkillActive( (GLMSG::SNETPC_DEFENSE_SKILL_ACTIVE*)nmg );
		}
		break;

	case NET_MSG_GCTRL_ACTION_BRD:
		{
			GLMSG::SNET_ACTION_BRD *pNetMsg = (GLMSG::SNET_ACTION_BRD *)nmg;
			
			if ( pNetMsg->emAction==GLAT_FALLING && m_Action>=GLAT_FALLING )	break;

			TurnAction ( pNetMsg->emAction );
		}
		break;

	case NET_MSG_GCTRL_UPDATE_STATE:
		{
			GLMSG::SNETPC_UPDATE_STATE *pNetMsg = (GLMSG::SNETPC_UPDATE_STATE *)nmg;
			UpdateClientState ( pNetMsg );
		}
		break;

	case NET_MSG_GCTRL_UPDATE_EXP:
		{
			GLMSG::SNETPC_UPDATE_EXP *pNetMsg = (GLMSG::SNETPC_UPDATE_EXP *)nmg;

			if ( pNetMsg->lnNowExp < m_sExperience.lnNow )
			{
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("DIS_EXP"), m_sExperience.lnNow-pNetMsg->lnNowExp );
			}
			else if ( pNetMsg->lnNowExp > m_sExperience.lnNow )
			{
				#ifndef NDEBUG
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("INC_EXP"), pNetMsg->lnNowExp-m_sExperience.lnNow );
				#endif
			}

			m_sExperience.lnNow = pNetMsg->lnNowExp;
		}
		break;

	case NET_MSG_GCTRL_UPDATE_MONEY:
		{
			GLMSG::SNETPC_UPDATE_MONEY *pNetMsg = (GLMSG::SNETPC_UPDATE_MONEY *)nmg;

			m_lnMoney = pNetMsg->lnMoney;

			/*dmk14 ingame web*/
			m_dwPremiumPoints = pNetMsg->dwPremium;
			m_dwCombatPoints = pNetMsg->dwCombat;
		}
		break;

	case NET_MSG_GCTRL_UPDATE_SP:
		{
			GLMSG::SNETPC_UPDATE_SP *pNetMsg = (GLMSG::SNETPC_UPDATE_SP *)nmg;

			m_sSP.dwNow = pNetMsg->wNowSP;
		}
		break;
	case NET_MSG_GCTRL_SET_POINTS_FB:
	{
		GLMSG::SNETPC_SET_POINTS_FB* pNetMsg = (GLMSG::SNETPC_SET_POINTS_FB*)nmg;
		switch (pNetMsg->emFB)
		{
		case EMREQ_SET_POINTS_FB_FAIL:
		{
			CInnerInterface::GetInstance().PrintConsoleTextDlg("Retrieve Points Failed");
		}break;
		case EMREQ_SET_POINTS_FB_OK:
		{
			switch (pNetMsg->wType)
			{
			case 1:
			{
				if (pNetMsg->nPoint > 0)
					CInnerInterface::GetInstance().PrintConsoleTextDlg("EPoints Updated from %d -> %d ( +%d )", m_pPPoints, m_pPPoints + pNetMsg->nPoint, pNetMsg->nPoint);
				else
					CInnerInterface::GetInstance().PrintConsoleTextDlg("EPoints Updated from %d -> %d ( %d )", m_pPPoints, m_pPPoints + pNetMsg->nPoint, pNetMsg->nPoint);

				m_pPPoints += pNetMsg->nPoint;
			}
			break;
			case 2:
			{
				if (pNetMsg->nPoint > 0)
					CInnerInterface::GetInstance().PrintConsoleTextDlg("Battle Points Updated from %d -> %d ( +%d )", m_pVPoints, m_pVPoints + pNetMsg->nPoint, pNetMsg->nPoint);
				else
					CInnerInterface::GetInstance().PrintConsoleTextDlg("Battle Points Updated from %d -> %d ( %d )", m_pVPoints, m_pVPoints + pNetMsg->nPoint, pNetMsg->nPoint);

				m_pVPoints += pNetMsg->nPoint;
			}
			break;
			default:	break;
			};
		}break;
		};
	}
	break;
	case NET_MSG_GCTRL_UPDATE_LP:
		{
			GLMSG::SNETPC_UPDATE_LP *pNetMsg = (GLMSG::SNETPC_UPDATE_LP *)nmg;

			int nDxLiving = pNetMsg->nLP - m_nLiving;
			m_nLiving = pNetMsg->nLP;
			
			if ( nDxLiving == 0 )	break;

			if ( nDxLiving > 0 )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("INC_LIVING"), nDxLiving );
			else
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("DIS_LIVING"), nDxLiving );
		}	
		break;

	case NET_MSG_GCTRL_UPDATE_SKP:
		{
			GLMSG::SNETPC_UPDATE_SKP *pNetMsg = (GLMSG::SNETPC_UPDATE_SKP *)nmg;

			int nDx = int(pNetMsg->dwSkillPoint) - int(m_dwSkillPoint);

			m_dwSkillPoint = pNetMsg->dwSkillPoint;

			if ( nDx > 0 )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("INC_SKP"), nDx );
			else
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("DIS_SKP"), nDx );
		}
		break;

	case NET_MSG_GCTRL_UPDATE_BRIGHT:
		{
			GLMSG::SNETPC_UPDATE_BRIGHT *pNetMsg = (GLMSG::SNETPC_UPDATE_BRIGHT *)nmg;

			int nDx = int(pNetMsg->nBright) - int(m_nBright);

			m_nBright = pNetMsg->nBright;
		
			if ( nDx > 0 )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("INC_BRIGHT"), nDx );
			else
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("DIS_BRIGHT"), nDx );
		}
		break;

	case NET_MSG_GCTRL_UPDATE_STATS:
		{
			GLMSG::SNETPC_UPDATE_STATS *pNetMsg = (GLMSG::SNETPC_UPDATE_STATS *)nmg;

			int nDx = int(pNetMsg->wStatsPoint) - int(m_wStatsPoint);

			m_wStatsPoint = pNetMsg->wStatsPoint;

			if ( nDx > 0 )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("INC_STATS"), nDx );
			else
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("DIS_STATS"), nDx );
		}
		break;

	case NET_MSG_GCTRL_PICKUP_MONEY:
		{
			GLMSG::SNETPC_PICKUP_MONEY *pNetMsg = (GLMSG::SNETPC_PICKUP_MONEY *)nmg;
		
			m_lnMoney = pNetMsg->lnMoney;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PICKUP_MONEY"), pNetMsg->lnPickUp );
			DxSoundLib::GetInstance()->PlaySound ( "PICKUP_ITEM" );
		}
		break;

	case NET_MSG_GCTRL_PICKUP_ITEM:
		{
			GLMSG::SNETPC_PICKUP_ITEM *pNetMsg = (GLMSG::SNETPC_PICKUP_ITEM *)nmg;
		
			SITEM * pITEM = GLItemMan::GetInstance().GetItem ( pNetMsg->sNID_ITEM );
			if ( pITEM )
			{
				/*item wrapper, Juver, 2018/01/12 */
				SITEM * pitem_disguise = GLItemMan::GetInstance().GetItem ( pNetMsg->sid_disguise );
				if ( pitem_disguise && pITEM->sBasicOp.emItemType == ITEM_WRAPPER_BOX )
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PICKUP_ITEM_WRAP"), pITEM->GetName(), pitem_disguise->GetName() );
				else
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("PICKUP_ITEM"), pITEM->GetName() );

				DxSoundLib::GetInstance()->PlaySound ( "PICKUP_ITEM" );
			}
		} 
		break;

	case NET_MSG_GCTRL_REQ_HOLD_FB:
		{
			GLMSG::SNETPC_REQ_HOLD_FB *pNetMsg = (GLMSG::SNETPC_REQ_HOLD_FB *)nmg;
			
			switch ( pNetMsg->emHoldMsg )
			{
			case NET_MSG_GCTRL_REQ_HOLD_TO_STORAGE:
				{
					switch ( pNetMsg->emHoldFB )
					{
					case EMHOLD_FB_NONKEEPSTORAGE:
						CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("STORAGE_SPAN_END") );
						break;
					};
				}
				break;
			};
		};
		break;

	case NET_MSG_GCTRL_INVEN_INSERT:
		{
			GLMSG::SNETPC_INVEN_INSERT *pNetMsg = (GLMSG::SNETPC_INVEN_INSERT *)nmg;

#if defined(VN_PARAM) //vietnamtest%%%
			if( pNetMsg->bVietnamInven )
			{
				m_cVietnamInventory.InsertItem ( pNetMsg->Data.sItemCustom, pNetMsg->Data.wPosX, pNetMsg->Data.wPosY );
			}else{
				m_cInventory.InsertItem ( pNetMsg->Data.sItemCustom, pNetMsg->Data.wPosX, pNetMsg->Data.wPosY, pNetMsg->bAllLine );
			}
#else
			m_cInventory.InsertItem ( pNetMsg->Data.sItemCustom, pNetMsg->Data.wPosX, pNetMsg->Data.wPosY, pNetMsg->bAllLine );
#endif

			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->Data.sItemCustom.sNativeID );
			if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && pNetMsg->Data.sItemCustom.dwPetID != 0 )
			{
				GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
				NetMsg.dwPetID = pNetMsg->Data.sItemCustom.dwPetID;
				NETSENDTOFIELD ( &NetMsg );
			}

			// VEHICLE
			// ≈ª∞Õ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
			pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->Data.sItemCustom.sNativeID );
			if ( pItem && pItem->sBasicOp.emItemType == ITEM_VEHICLE && pNetMsg->Data.sItemCustom.dwVehicleID != 0)
			{
				GLMSG::SNET_VEHICLE_REQ_ITEM_INFO NetMsg;
				NetMsg.dwVehicleID = pNetMsg->Data.sItemCustom.dwVehicleID;
				NETSENDTOFIELD ( &NetMsg );
			}
		}
		break;

	case NET_MSG_GCTRL_REQ_VNINVEN_TO_INVEN_FB:
		{
			GLMSG::SNETPC_REQ_VNINVEN_TO_INVEN_FB *pNetMsg = (GLMSG::SNETPC_REQ_VNINVEN_TO_INVEN_FB *)nmg;

			m_cInventory.InsertItem ( pNetMsg->Data.sItemCustom, pNetMsg->wNewPosX, pNetMsg->wNewPosY );
			m_cVietnamInventory.DeleteItem( pNetMsg->wPosX, pNetMsg->wPosY );
		}
		break;

	case NET_MSG_GCTRL_INVEN_DELETE:
		{
			GLMSG::SNETPC_INVEN_DELETE *pNetMsg = (GLMSG::SNETPC_INVEN_DELETE *)nmg;
/*
			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ¡¶∞≈«ÿ¡ÿ¥Ÿ.
			SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			if ( pInvenItem )
			{
				PETCARDINFO_MAP_ITER iter = m_mapPETCardInfo.find ( pInvenItem->sItemCustom.dwPetID );
				if ( iter != m_mapPETCardInfo.end() ) m_mapPETCardInfo.erase ( iter );
			}
*/
#if defined(VN_PARAM) //vietnamtest%%%
			if( pNetMsg->bVietnamInven )
			{
				m_cVietnamInventory.DeleteItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			}else{
				m_cInventory.DeleteItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			}
#else
			m_cInventory.DeleteItem ( pNetMsg->wPosX, pNetMsg->wPosY );
#endif
		}
		break;

	case NET_MSG_GCTRL_ITEM_COOLTIME_UPDATE:
		{
			GLMSG::SNETPC_ITEM_COOLTIME_UPDATE *pNetMsg = (GLMSG::SNETPC_ITEM_COOLTIME_UPDATE *)nmg;

			ITEM_COOLTIME sCoolTime;
			sCoolTime.dwID	= pNetMsg->dwID;
			sCoolTime.dwCoolID	= pNetMsg->dwCoolID;
			sCoolTime.tCoolTime	= pNetMsg->tCoolTime;
			sCoolTime.tUseTime	= pNetMsg->tUseTime;

			SetCoolTime ( sCoolTime, pNetMsg->emCoolType );
		}
		break;

	case NET_MSG_GCTRL_ITEM_COOLTIME_ERROR:
		{
			GLMSG::SNET_ITEM_COOLTIME_ERROR *pNetMsg = (GLMSG::SNET_ITEM_COOLTIME_ERROR *)nmg;
			
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sNativeID );
			if ( !pItem )	break;

			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, 
				ID2GAMEINTEXT("ITEM_COOLTIME"), pItem->GetName() );
		}
		break;
	case NET_MSG_GCTRL_INVEN_DEL_INSERT:
		{
			GLMSG::SNETPC_INVEN_DEL_AND_INSERT *pNetMsg = (GLMSG::SNETPC_INVEN_DEL_AND_INSERT *)nmg;
/*
			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ¡¶∞≈«ÿ¡ÿ¥Ÿ.
			SINVENITEM* pInvenItem = m_cInventory.GetItem ( pNetMsg->wDelX, pNetMsg->wDelX );
			if ( pInvenItem )
			{
				PETCARDINFO_MAP_ITER iter = m_mapPETCardInfo.find ( pInvenItem->sItemCustom.dwPetID );
				if ( iter != m_mapPETCardInfo.end() ) m_mapPETCardInfo.erase ( iter );
			}
*/
#if defined(VN_PARAM) //vietnamtest%%%
			if( pNetMsg->bVietnamInven )
			{
				m_cVietnamInventory.DeleteItem ( pNetMsg->wDelX, pNetMsg->wDelY );
				m_cVietnamInventory.InsertItem ( pNetMsg->sInsert.sItemCustom, pNetMsg->sInsert.wPosX, pNetMsg->sInsert.wPosY );
			}else{
				m_cInventory.DeleteItem ( pNetMsg->wDelX, pNetMsg->wDelY );
				m_cInventory.InsertItem ( pNetMsg->sInsert.sItemCustom, pNetMsg->sInsert.wPosX, pNetMsg->sInsert.wPosY );

				// PET
				// ∆÷ƒ´µÂ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
				SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sInsert.sItemCustom.sNativeID );
				if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && pNetMsg->sInsert.sItemCustom.dwPetID != 0  )
				{
					GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
					NetMsg.dwPetID = pNetMsg->sInsert.sItemCustom.dwPetID;
					NETSENDTOFIELD ( &NetMsg );
				}

				// VEHICLE
				// ≈ª∞Õ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
				pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sInsert.sItemCustom.sNativeID );
				if ( pItem && pItem->sBasicOp.emItemType == ITEM_VEHICLE && pNetMsg->sInsert.sItemCustom.dwVehicleID != 0 ) 
				{
					GLMSG::SNET_VEHICLE_REQ_ITEM_INFO NetMsg;
					NetMsg.dwVehicleID = pNetMsg->sInsert.sItemCustom.dwVehicleID;
					NETSENDTOFIELD ( &NetMsg );
				}
			}
#else
			m_cInventory.DeleteItem ( pNetMsg->wDelX, pNetMsg->wDelY );
			m_cInventory.InsertItem ( pNetMsg->sInsert.sItemCustom, pNetMsg->sInsert.wPosX, pNetMsg->sInsert.wPosY );
			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sInsert.sItemCustom.sNativeID );
			if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && pNetMsg->sInsert.sItemCustom.dwPetID != 0 )
			{
				GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
				NetMsg.dwPetID = pNetMsg->sInsert.sItemCustom.dwPetID;
				NETSENDTOFIELD ( &NetMsg );
			}

			// VEHICLE
			// ≈ª∞Õ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
			pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sInsert.sItemCustom.sNativeID );
			if ( pItem && pItem->sBasicOp.emItemType == ITEM_VEHICLE && pNetMsg->sInsert.sItemCustom.dwVehicleID != 0 )
			{
				GLMSG::SNET_VEHICLE_REQ_ITEM_INFO NetMsg;
				NetMsg.dwVehicleID = pNetMsg->sInsert.sItemCustom.dwVehicleID;
				NETSENDTOFIELD ( &NetMsg );
			}
#endif

			

		}
		break;

	case NET_MSG_GCTRL_PUTON_RELEASE:
		{
			GLMSG::SNETPC_PUTON_RELEASE *pNetMsg = (GLMSG::SNETPC_PUTON_RELEASE *)nmg;

			EMITEM_TYPE emTYPE_RELOAD = ITEM_NSIZE;
			SNATIVEID sNID_RELOAD = NATIVEID_NULL();
			EMSLOT emLHand = GetCurLHand();

			if ( pNetMsg->bRefresh )
			{
				//	¿Á¿Â¿¸«“ æ∆¿Ã≈∆¿Œ¡ˆ æÀæ∆∫Ω.
				if ( pNetMsg->emSlot == emLHand && GLCHARLOGIC::VALID_SLOT_ITEM(pNetMsg->emSlot) )
				{
					SITEMCUSTOM sItemCustom = GLCHARLOGIC::GET_SLOT_ITEM(pNetMsg->emSlot);
					SITEM* pItem = GLItemMan::GetInstance().GetItem(sItemCustom.sNativeID);
		
					if ( pItem->sBasicOp.emItemType==ITEM_ARROW || 
						pItem->sBasicOp.emItemType==ITEM_CHARM ||

						/*gun-bullet logic, Juver, 2017/05/27 */
						pItem->sBasicOp.emItemType==ITEM_BULLET )
					{
						emTYPE_RELOAD = pItem->sBasicOp.emItemType;
						sNID_RELOAD = sItemCustom.sNativeID;
					}
				}
			}

			//	SLOT ∫Ø∞Ê.
			GLCHARLOGIC::RELEASE_SLOT_ITEM ( pNetMsg->emSlot );
			GLCHARLOGIC::INIT_DATA ( FALSE, FALSE );

			//	«¸ªÛ ∫Ø∞Ê.
			UpdateSuit( TRUE );

			//	Note : ø°¥œ∏ﬁ¿Ãº« √ ±‚»≠.
			ReSelectAnimation();

			if ( emTYPE_RELOAD!=ITEM_NSIZE )
			{
				SINVENITEM* pInvenItem = m_cInventory.FindItem ( emTYPE_RELOAD, sNID_RELOAD );
				if ( !pInvenItem )	break;

				GLMSG::SNETPC_REQ_INVEN_TO_SLOT NetMsg;
				NetMsg.emToSlot = emLHand;
				NetMsg.wPosX = pInvenItem->wPosX;
				NetMsg.wPosY = pInvenItem->wPosY;

				NETSENDTOFIELD ( &NetMsg );
			}

			// ≈ª∞Õ æ∆¿Ã≈€¿Ã∏È ¡§∫∏ ∫Ò»∞º∫
			if ( pNetMsg->emSlot == SLOT_VEHICLE )
			{
				m_sVehicle.SetActiveValue( false );
				m_sVehicle.RESET ();
			}
		}
		break;

	case NET_MSG_GCTRL_PUTON_UPDATE:
		{
			GLMSG::SNETPC_PUTON_UPDATE *pNetMsg = (GLMSG::SNETPC_PUTON_UPDATE *)nmg;

			// »≠ªÏ¿Ã≥™ ∫Œ¿˚∞∞¿∫ æ∆¿Ã≈€¿Ã ¿⁄µø ø≈∞‹¡ˆ¥¬ ∞ÊøÏ
			if ( pNetMsg->emSlotRelease!=SLOT_TSIZE )
			{
				GLCHARLOGIC::RELEASE_SLOT_ITEM ( pNetMsg->emSlotRelease );
			}

			//	Note : æ∆¿Ã≈€ ¬¯øÎ ø‰±∏ƒ°ø° ∫Œ¡∑«“ ∞ÊøÏ ¡§∫∏ √‚∑¬, SP √Ê¥Á ºˆƒ°.
			//

			//	SLOT ∫Ø∞Ê.
			GLCHARLOGIC::SLOT_ITEM ( pNetMsg->sItemCustom, pNetMsg->emSlot );

			if ( pNetMsg->emSlot != SLOT_HOLD )
			{
				WORD wACP = GLCHARLOGIC::CALC_ACCEPTP ( pNetMsg->sItemCustom.sNativeID );
				if ( wACP > 0 )		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("ITEMPUTON_ACCEPT_SP"), wACP );	

				GLCHARLOGIC::INIT_DATA ( FALSE, FALSE );

				//	«¸ªÛ ∫Ø∞Ê.
				UpdateSuit( TRUE );
				
				//	Note : ø°¥œ∏ﬁ¿Ãº« √ ±‚»≠.
				ReSelectAnimation();				
			}

			// π´±‚ø° µ˚∏• πˆ«¡∏¶ √ ±‚»≠ «—¥Ÿ.
			DisableSkillFact();

			// ≈ª∞Õ æ∆¿Ã≈€ ¿Â¬¯Ω√ ¡§∫∏∏¶ ∞°¡Æø…¥œ¥Ÿ.
			if ( pNetMsg->emSlot == SLOT_VEHICLE )
			{
				ReqVehicleUpdate();
			}
		}
		break;

	case NET_MSG_GCTRL_PUTON_CHANGE:
		{
			GLMSG::SNETPC_PUTON_CHANGE *pNetMsg = (GLMSG::SNETPC_PUTON_CHANGE *)nmg;

			// ¡÷/∫∏¡∂π´±‚ º¬∆√
			SetUseArmSub( pNetMsg->bUseArmSub );

			// ¿Œ≈Õ∆‰¿ÃΩ∫ ∫Ø∞Ê
			CInnerInterface::GetInstance().SetArmSwap( !pNetMsg->bUseArmSub );

			GLCHARLOGIC::INIT_DATA ( FALSE, FALSE, pNetMsg->fCONFT_HP_RATE );

			//	«¸ªÛ ∫Ø∞Ê.
			UpdateSuit( TRUE );

			m_bReqSwapArms = FALSE;

			//	Note : ø°¥œ∏ﬁ¿Ãº« √ ±‚»≠.
			ReSelectAnimation();

			// π´±‚ø° µ˚∏• πˆ«¡∏¶ √ ±‚»≠ «—¥Ÿ.
			DisableSkillFact();

		}
		break;

	case NET_MSG_GCTRL_REQ_LEVELUP_FB:
		{
			GLMSG::SNETPC_REQ_LEVELUP_FB *pNetMsg = (GLMSG::SNETPC_REQ_LEVELUP_FB *)nmg;

			STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
			DxEffGroupPlayer::GetInstance().NewEffGroup
			(
				GLCONST_CHAR::strLEVELUP_EFFECT.c_str(),
				m_matTrans,
				&sTargetID
			);

			LEVLEUP ( pNetMsg->bInitNowLevel );

			//	Note : ∑ææ˜»ƒø° ∫Ø∞Êµ» ºˆƒ° ¿˚øÎ.
			m_wLevel = pNetMsg->wLevel;
			m_wStatsPoint = pNetMsg->wStatsPoint;
			m_dwSkillPoint = pNetMsg->dwSkillPoint;

			// ¡÷ºÆµπ∑¡
			if ( m_dwUserLvl < USER_GM3 )
			{
				SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pNetMsg->sMapID );
				if ( pMapNode )
				{
					GLLevelFile cLevelFile;
					if( cLevelFile.LoadFile ( pMapNode->strFile.c_str(), TRUE, NULL ) )

					{
						SLEVEL_REQUIRE* pRequire = cLevelFile.GetLevelRequire ();
						EMREQFAIL emReqFail(EMREQUIRE_COMPLETE);
						if( pRequire ) emReqFail = pRequire->ISCOMPLETE ( this );
						if( emReqFail == EMREQUIRE_LEVEL )
						{
							CInnerInterface::GetInstance().GetRequireCheck()->SetMapCheckType( EMMAPCHECK_LEVELUP );
							CInnerInterface::GetInstance().ShowGroupFocus ( MAP_REQUIRE_CHECK );
						}
					}

				}
			}		 
			


#ifndef CH_PARAM_USEGAIN 
			//** Add EventTime
			if( m_bEventStart )
			{
				if( m_bEventApply )
				{
					if( m_wLevel < m_EventStartLv || m_wLevel > m_EventEndLv )
					{
						m_bEventApply = FALSE;
						CInnerInterface::GetInstance().BONUS_TIME_EVENT_END();
					}
				}else{
					if( m_wLevel >= m_EventStartLv && m_wLevel <= m_EventEndLv )
					{
						m_bEventApply = TRUE;
						CTime crtTime = GLGaeaClient::GetInstance().GetCurrentTime();
						m_tLoginTime =  crtTime.GetTime();
						CInnerInterface::GetInstance().BONUS_TIME_EVENT_START( !m_bEventBuster );
					}
				}
			}
#endif

			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::LIGHTSKYBLUE, ID2GAMEINTEXT("LEVELUP"), m_wLevel );
		}
		break;
		//¬‡•Õ®t≤Œ
	case NET_MSG_GCTRL_INVEN_RELL_CHANGE_TO_FB:
	{
		GLMSG::SNETPC_REQ_UPRELL_FB* pNetMsg = (GLMSG::SNETPC_REQ_UPRELL_FB*)nmg;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

		switch (pNetMsg->emFB)
		{
		case EMINVEN_RELL_CHANGE_FB_FAIL:
		{
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_FAIL"));
		}
		break;
		case EMINVEN_RELL_CHANGE_FB_OK:
		{
			//CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_OK"), pCharacter->m_dwReborn+GLCONST_CHAR::sREBOMRANK[0].m_dwRebomL,GLCONST_CHAR::sREBOMRANK[0].m_dwRebomPoint,GLCONST_CHAR::sREBOMRANK[0].m_dwRebomSkillPoint,GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoney
			//,GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoneyV,GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoneyP);
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::BRIGHTGREEN, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_OK"), pCharacter->m_dwReborn + 1, (pCharacter->m_dwReborn + 1) * GLCONST_CHAR::m_dwRebomPoint, GLCONST_CHAR::m_dwRebomSkillPoint, GLCONST_CHAR::m_dwRebomMoney * pCharacter->m_dwReborn + GLCONST_CHAR::m_dwRebomMoney); /*¿u§∆¬‡•Õ®t≤Œ•\Ø‡•NΩX , SSODomain 28/05/2021  */
			DoModal(ID2GAMEINTEXT("MODAL_RELL_CHANGE_END"), MODAL_INFOMATION, OK, MODAL_RELL_CHANGE_END);
		}
		break;
		case EMINVEN_RELL_CHANGE_FB_NOITEM:
		{
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOITEM"));
		}
		break;
		case EMINVEN_RELL_CHANGE_FB_ITEMTYPE:
		{
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_ITEMTYPE"));
		}
		break;
		case EMINVEN_RELL_CHANGE_FB_BADCLASS:
		{
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_BADCLASS"));
		}
		break;
		case EMINVEN_RELL_CHANGE_FB_NOTGRADE:
		{
			int a = GLCONST_CHAR::m_dwRebomLvl + pCharacter->m_dwReborn;
			
			
				CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOTGRADE"), GLCONST_CHAR::m_dwRebomLvl + pCharacter->m_dwReborn); /*¿u§∆¬‡•Õ®t≤Œ•\Ø‡•NΩX , SSODomain 28/05/2021  */
				CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::BRIGHTGREEN, ID2GAMEINTEXT("CURRENTLY_TURN"), pCharacter->m_dwReborn); /*¿u§∆¬‡•Õ®t≤Œ•\Ø‡•NΩX , SSODomain 28/05/2021  */
			
		}
		break;
		case EMINVEN_RELL_CHANGE_FB_NOTREBOM:
		{
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOTREBOM"));
		}
		break;
		case EMINVEN_RELL_CHANGE_FB_NOTREBOMPOINT:
		{
			//CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOTREBOMPOINT"),GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoney );
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOTREBOMPOINT"), GLCONST_CHAR::m_dwRebomMoney * pCharacter->m_dwReborn + GLCONST_CHAR::m_dwRebomMoney); /*¿u§∆¬‡•Õ®t≤Œ•\Ø‡•NΩX , SSODomain 28/05/2021  */
		}
		break;

		break;
		case EMINVEN_RELL_CHANGE_FB_NOTREBOMMAX:
		{
			CInnerInterface::GetInstance().PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOTREBOMMAX"));
		}
		break;
		/*¿u§∆¬‡•Õ®t≤Œ•\Ø‡•NΩX , SSODomain 28/05/2021  */
				/*
			case EMINVEN_RELL_CHANGE_FB_NOTREBOMSKILLPOINT:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOTREBOMSKILLPOINT"),GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoneyV );
				}
				break;
			case EMINVEN_RELL_CHANGE_FB_NOTVALUE:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RELL_CHANGE_FB_NOTVALUE"),GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoneyP );
				}
				break;
				*/
		}
	}
	break;
	case NET_MSG_GCTRL_REQ_STATSUP_FB:
		{
			GLMSG::SNETPC_REQ_STATSUP_FB *pNetMsg = (GLMSG::SNETPC_REQ_STATSUP_FB *)nmg;
			STATSUP(pNetMsg->sStats);
		}
		break;

	//Activate Skill w/o Scroll
	case NET_MSG_GCTRL_REQ_LEARNSKILL_NONINVEN_FB:
		{
			GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB *pNetMsg = (GLMSG::SNETPC_REQ_LEARNSKILL_NONINVEN_FB *)nmg;

			if ( pNetMsg->emCHECK == EMSKILL_OK )
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pNetMsg->skill_id );
				if ( !pSkill )											break;

				m_ExpSkills.insert ( std::make_pair ( pNetMsg->skill_id.dwID, SCHARSKILL(pNetMsg->skill_id,0) ) );

				if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
				{
					INIT_DATA ( FALSE, FALSE );
				}

				STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
				DxEffGroupPlayer::GetInstance().NewEffGroup
				(
					GLCONST_CHAR::strSKILL_LEARN_EFFECT.c_str(),
					m_matTrans,
					&sTargetID
				);
			}
		}
		break;
	case NET_MSG_GCTRL_REQ_RANKNAME_FB: //MMR Rank
	{
		GLMSG::SNETPC_REQ_RANKNAME_FB* pNetMsg = (GLMSG::SNETPC_REQ_RANKNAME_FB*)nmg;
		ShowRankName(pNetMsg->bRankName);
	}
	break;

	case NET_MSG_GCTRL_REQ_RANKMARK_FB: //MMR Rank
	{
		GLMSG::SNETPC_REQ_RANKMARK_FB* pNetMsg = (GLMSG::SNETPC_REQ_RANKMARK_FB*)nmg;
		ShowRankMark(pNetMsg->bRankMark);
	}
	break;
	case NET_MSG_GCTRL_REQ_STATSUPCMD_FB:
		{
			GLMSG::SNETPC_REQ_STATSUPCMD_FB *pNetMsg = (GLMSG::SNETPC_REQ_STATSUPCMD_FB *)nmg;
			STATSUP_CMD(pNetMsg->emStats,pNetMsg->dwValue);
			m_wStatsPoint = pNetMsg->wStatsPoint;
		}
		break;

	case NET_MSG_GCTRL_REQ_LEARNSKILL_FB:
		{
			GLMSG::SNETPC_REQ_LEARNSKILL_FB *pNetMsg = (GLMSG::SNETPC_REQ_LEARNSKILL_FB *)nmg;

			if ( pNetMsg->emCHECK == EMSKILL_OK )
			{
				//	Note : Ω∫≈≥ ¡§∫∏ ∞°¡Æø».
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pNetMsg->skill_id );
				if ( !pSkill )											break;

				m_ExpSkills.insert ( std::make_pair ( pNetMsg->skill_id.dwID, SCHARSKILL(pNetMsg->skill_id,0) ) );

				//	Note : ∆‰Ω√∫Í Ω∫≈≥¿Ã ∫Ø»≠ «“∂ß √ ±‚ ºˆƒ°µÈ¿ª ∏µŒ ¿Á ∞ËªÍ«—¥Ÿ.
				//
				if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
				{
					INIT_DATA ( FALSE, FALSE );
				}

				STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
				DxEffGroupPlayer::GetInstance().NewEffGroup
				(
					GLCONST_CHAR::strSKILL_LEARN_EFFECT.c_str(),
					m_matTrans,
					&sTargetID
				);
			}
			else
			{
				//	∞·∞˙ ∞™ø° µ˚∂Û ø°∑Ø ∏ﬁΩ√¡ˆ √‚∑¬.
			}
		}
		break;

	case NET_MSG_GCTRL_REQ_SKILLUP_FB:
		{
			GLMSG::SNETPC_REQ_SKILLUP_FB *pNetMsg = (GLMSG::SNETPC_REQ_SKILLUP_FB *)nmg;
			if ( pNetMsg->emCHECK == EMSKILL_LEARN_OK )
			{
				//	Note : Ω∫≈≥ ¡§∫∏ ∞°¡Æø».
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pNetMsg->sSkill.sNativeID );
				if ( !pSkill )											break;

				SKILL_MAP_ITER iter_del = m_ExpSkills.find ( pNetMsg->sSkill.sNativeID.dwID );
				if ( iter_del!=m_ExpSkills.end() )	m_ExpSkills.erase ( iter_del );

				m_ExpSkills[pNetMsg->sSkill.sNativeID.dwID] = pNetMsg->sSkill;

				//	Note : ∆‰Ω√∫Í Ω∫≈≥¿Ã ∫Ø»≠ «“∂ß √ ±‚ ºˆƒ°µÈ¿ª ∏µŒ ¿Á ∞ËªÍ«—¥Ÿ.
				//
				if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
				{
					INIT_DATA ( FALSE, FALSE );
				}

				STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
				DxEffGroupPlayer::GetInstance().NewEffGroup
				(
					GLCONST_CHAR::strSKILL_UP_EFFECT.c_str(),
					m_matTrans,
					&sTargetID
				);
			}
		}
		break;

	case NET_MSG_GCTRL_REQ_SKILL_FB:
		{
			GLMSG::SNETPC_REQ_SKILL_FB *pNetMsg = (GLMSG::SNETPC_REQ_SKILL_FB *)nmg;
			if ( pNetMsg->emSKILL_FB != EMSKILL_OK )
			{
				if( pNetMsg->emSKILL_FB == EMSKILL_NOTREBIRTH )
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("NON_REBIRTH_SKILL_MSG"), pNetMsg->szName );
				}else{
					if ( IsACTION(GLAT_SKILL) )		TurnAction(GLAT_IDLE);
				}
			}
		}
		break;

	case NET_MSG_REQ_SKILL_REVIVEL_FAILED:
		{
			GLMSG::SNET_MSG_REQ_SKILL_REVIVEL_FAILED *pNetMsg = (GLMSG::SNET_MSG_REQ_SKILL_REVIVEL_FAILED *)nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("NON_REBIRTH_SKILL_MSG_TARGET"), pNetMsg->szName );
		}
		break;

	case NET_MSG_GCTRL_SKILLCONSUME_FB:
		{
			GLMSG::SNETPC_SKILLCONSUME_FB *pNetMsg = (GLMSG::SNETPC_SKILLCONSUME_FB *)nmg;

			EMSLOT emLHand = GetCurLHand();
			
			SITEM* pItem = GET_SLOT_ITEMDATA(emLHand);
			if ( pItem && pItem->sDrugOp.bInstance )		m_PutOnItems[emLHand].wTurnNum = pNetMsg->wTurnNum;
			m_sHP.dwNow = pNetMsg->dwNowHP;
			m_sMP.dwNow = pNetMsg->dwNowMP;
			m_sSP.dwNow = pNetMsg->dwNowSP;
			m_sCombatPoint.dwNow = pNetMsg->dwNowCP; /*combatpoint logic, Juver, 2017/05/28 */
		}
		break;

	case NET_MSG_GCTRL_SKILLFACT_BRD:
		{
			GLMSG::SNETPC_SKILLFACT_BRD *pNetMsg = (GLMSG::SNETPC_SKILLFACT_BRD *)nmg;

			if( pNetMsg->emCrow != CROW_PC ) break;

			m_sHP.VARIATION ( pNetMsg->nVAR_HP );
			m_sMP.VARIATION ( pNetMsg->nVAR_MP );
			m_sSP.VARIATION ( pNetMsg->nVAR_SP );

			//	µ•πÃ¡ˆ ∏ﬁΩ√¡ˆ.
			if ( pNetMsg->nVAR_HP < 0 )
			{
				if ( pNetMsg->dwDamageFlag & DAMAGE_TYPE_SHOCK )	ReceiveShock ();
				else					ReceiveSwing ();

				D3DXVECTOR3 vPos = GetPosBodyHeight();
				CInnerInterface::GetInstance().SetDamage( vPos, static_cast<DWORD>(-pNetMsg->nVAR_HP), pNetMsg->dwDamageFlag, UI_UNDERATTACK );

				//	Note : πÊæÓΩ∫≈≥¿« ¿Ã∆Â∆Æ∞° ¿÷¿ª∂ß πﬂµøΩ√≈¥.
				STARGETID sACTOR(pNetMsg->sACTOR.GETCROW(),pNetMsg->sACTOR.GETID());
				sACTOR.vPos = GLGaeaClient::GetInstance().GetTargetPos ( sACTOR );

				SKT_EFF_HOLDOUT ( sACTOR, pNetMsg->dwDamageFlag );

				if ( pNetMsg->dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW )
				{
					// ∞≠«—≈∏∞› ¿Ã∆Â∆Æ

					D3DXVECTOR3 vDIR = sACTOR.vPos - m_vPos;

					D3DXVECTOR3 vDIR_ORG(1,0,0);
					float fdir_y = DXGetThetaYFromDirection ( vDIR, vDIR_ORG );

					D3DXMATRIX matTrans;
					D3DXMatrixRotationY ( &matTrans, fdir_y );
					matTrans._41 = m_vPos.x;
					matTrans._42 = m_vPos.y + 10.0f;
					matTrans._43 = m_vPos.z;

					//	Note : ¿⁄±‚ ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
					DxEffGroupPlayer::GetInstance().NewEffGroup ( GLCONST_CHAR::strCRUSHING_BLOW_EFFECT.c_str(), matTrans, &sACTOR );
				}

			}
			else
			{
				if ( pNetMsg->dwDamageFlag & DAMAGE_TYPE_DAMAGE_ABSORBED || pNetMsg->dwDamageFlag & DAMAGE_TYPE_IMMUNE )
				{
					D3DXVECTOR3 vPos = GetPosBodyHeight();
					CInnerInterface::GetInstance().SetDamage( vPos, 0, pNetMsg->dwDamageFlag, UI_UNDERATTACK );
				}
			}

			//	»˙∏µ ∏ﬁΩ√¡ˆ.
			//if ( pNetMsg->nVAR_HP > 0 )
			//{
			//	CPlayInterface::GetInstance().InsertText ( GetPosition(), static_cast<WORD>(pNetMsg->nVAR_HP), pNetMsg->bCRITICAL, 1 );
			//}
		}
		break;

	case NET_MSG_GCTRL_SKILLHOLD_BRD:
		{
			GLMSG::SNETPC_SKILLHOLD_BRD *pNetMsg = (GLMSG::SNETPC_SKILLHOLD_BRD *)nmg;
			bool bReceiveBuff(true);
			//	¡ˆº”«¸ Ω∫≈≥¿« ∞ÊøÏ Ω∫≈≥ ∆—≈Õ √ﬂ∞°µ .
			if ( pNetMsg->skill_id != NATIVEID_NULL() )
			{
				if ( pNetMsg->fPARAM )
					m_sLINKHP.fRef = pNetMsg->fPARAM; //MaxHP ∫Ò¿≤¿ª º≠πˆ∑Œ ∫Œ≈Õ πﬁ¿Ω

				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( pNetMsg->skill_id );
				if ( pSkill )
				{
					if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF && 
						pSkill->m_sBASIC.emIMPACT_REALM == REALM_SELF && 
						pSkill->m_sBASIC.emIMPACT_SIDE == SIDE_OUR )
					{
						switch ( m_emClass )
						{
						case GLCC_BRAWLER_M:
						case GLCC_BRAWLER_W:
							switch( pNetMsg->skill_id.wMainID )
							{
								case EMSKILL_SWORDSMAN_01:
								case EMSKILL_SWORDSMAN_02:
								case EMSKILL_SWORDSMAN_03:
								case EMSKILL_SWORDSMAN_04:
								case EMSKILL_ARCHER_01:
								case EMSKILL_ARCHER_02:
								case EMSKILL_ARCHER_03:
								case EMSKILL_ARCHER_04:
								case EMSKILL_SHAMAN_01:
								case EMSKILL_SHAMAN_02:
								case EMSKILL_SHAMAN_03:
								case EMSKILL_SHAMAN_04:
									bReceiveBuff = false;
									break;					
							}
							break;
						case GLCC_SWORDSMAN_M:
						case GLCC_SWORDSMAN_W:
							switch( pNetMsg->skill_id.wMainID )
							{
								case EMSKILL_BRAWLER_01:
								case EMSKILL_BRAWLER_02:
								case EMSKILL_BRAWLER_03:
								case EMSKILL_BRAWLER_04:
								case EMSKILL_ARCHER_01:
								case EMSKILL_ARCHER_02:
								case EMSKILL_ARCHER_03:
								case EMSKILL_ARCHER_04:
								case EMSKILL_SHAMAN_01:
								case EMSKILL_SHAMAN_02:
								case EMSKILL_SHAMAN_03:
								case EMSKILL_SHAMAN_04:
									bReceiveBuff = false;
									break;					
							}
							break;
						case GLCC_ARCHER_M:
						case GLCC_ARCHER_W:
							switch( pNetMsg->skill_id.wMainID )
							{
								case EMSKILL_BRAWLER_01:
								case EMSKILL_BRAWLER_02:
								case EMSKILL_BRAWLER_03:
								case EMSKILL_BRAWLER_04:
								case EMSKILL_SWORDSMAN_01:
								case EMSKILL_SWORDSMAN_02:
								case EMSKILL_SWORDSMAN_03:
								case EMSKILL_SWORDSMAN_04:
								case EMSKILL_SHAMAN_01:
								case EMSKILL_SHAMAN_02:
								case EMSKILL_SHAMAN_03:
								case EMSKILL_SHAMAN_04:
									bReceiveBuff = false;
									break;					
							}
							break;
						case GLCC_SHAMAN_M:
						case GLCC_SHAMAN_W:
							switch( pNetMsg->skill_id.wMainID )
							{
								case EMSKILL_BRAWLER_01:
								case EMSKILL_BRAWLER_02:
								case EMSKILL_BRAWLER_03:
								case EMSKILL_BRAWLER_04:
								case EMSKILL_SWORDSMAN_01:
								case EMSKILL_SWORDSMAN_02:
								case EMSKILL_SWORDSMAN_03:
								case EMSKILL_SWORDSMAN_04:
								case EMSKILL_ARCHER_01:
								case EMSKILL_ARCHER_02:
								case EMSKILL_ARCHER_03:
								case EMSKILL_ARCHER_04:
									bReceiveBuff = false;
									break;					
							}
							break;
						}
					}
				}
				if( bReceiveBuff )
				{
					RECEIVE_SKILLFACT ( pNetMsg->skill_id, pNetMsg->wLEVEL, pNetMsg->wSELSLOT, pNetMsg->wCasterCrow, pNetMsg->dwCasterID );
					FACTEFF::NewSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, pNetMsg->skill_id, m_matTrans, m_vDir );
				}
				
			}
		}
		break;

	case NET_MSG_GCTRL_SKILLHOLD_RS_BRD:
		{
			GLMSG::SNETPC_SKILLHOLD_RS_BRD *pNetMsg = (GLMSG::SNETPC_SKILLHOLD_RS_BRD *)nmg;

			//	Note : Ω∫≈≥ fact µÈ¿ª ¡æ∑·.
			//		πŸ∑Œ ∏Æª˚ «œ¡ˆ æ ∞Ì ø©±‚º≠ Ω√∞£ ¡∂¡æ«œø© ¡§ªÛ ¡æ∑·µ«∞‘ «‘. ( ¿Ã∆—∆Æ ¡æ∑· ∂ßπÆ. )
			for ( int i=0; i<SKILLFACT_SIZE; ++i )
			{
				if ( pNetMsg->bRESET[i] )
				{
					FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT[i].sNATIVEID );

					DISABLESKEFF(i);
				}
			}
		}
		break;

	case NET_MSG_GCTRL_SKILLHOLDEX_BRD:
		{
			GLMSG::SNETPC_REQ_SKILLHOLDEX_BRD *pNetMsg = (GLMSG::SNETPC_REQ_SKILLHOLDEX_BRD *)nmg;
			m_sSKILLFACT[pNetMsg->wSLOT] = pNetMsg->sSKILLEF;
		}
		break;

	case NET_MSG_GCTRL_SKILL_CANCEL_BRD:
		{
			if ( IsACTION(GLAT_SKILL) )	TurnAction ( GLAT_IDLE );
		}
		break;

		/*dmk14 remove active buff selection*/
	case NET_MSG_GCTRL_REQ_DISABLESKILLEFF_FB:
		{
			GLMSG::SNETPC_REQ_DISABLESKILLEFF_FB *pNetMsg = (GLMSG::SNETPC_REQ_DISABLESKILLEFF_FB *)nmg;
			FACTEFF::DeleteSkillFactEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSKILLFACT[pNetMsg->dwSKILL].sNATIVEID );
			DISABLESKEFF(pNetMsg->dwSKILL);
		}
		break;
	case NET_MSG_GCTRL_REQ_DISABLEITEMFACT_FB:
		{
			GLMSG::SNETPC_REQ_DISABLEITEMFACT_FB *pNetMsg = (GLMSG::SNETPC_REQ_DISABLEITEMFACT_FB *)nmg;
			m_sFITEMFACT[pNetMsg->dwSKILL].RESET();
		}
		break;

	case NET_MSG_GCTRL_STATEBLOW_BRD:
		{
			GLMSG::SNETPC_STATEBLOW_BRD *pNetMsg = (GLMSG::SNETPC_STATEBLOW_BRD *)nmg;

			if ( pNetMsg->emBLOW <= EMBLOW_SINGLE )
				FACTEFF::DeleteBlowSingleEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, m_sSTATEBLOWS );

			SSTATEBLOW *pSTATEBLOW = NULL;
			if ( pNetMsg->emBLOW <= EMBLOW_SINGLE )		pSTATEBLOW = &m_sSTATEBLOWS[0];
			else										pSTATEBLOW = &m_sSTATEBLOWS[pNetMsg->emBLOW-EMBLOW_SINGLE];

			pSTATEBLOW->emBLOW = pNetMsg->emBLOW;
			pSTATEBLOW->fAGE = pNetMsg->fAGE;
			pSTATEBLOW->fSTATE_VAR1 = pNetMsg->fSTATE_VAR1;
			pSTATEBLOW->fSTATE_VAR2 = pNetMsg->fSTATE_VAR2;

			//	Note : »ø∞˙ ª˝º∫.
			FACTEFF::NewBlowEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, pSTATEBLOW->emBLOW, m_matTrans, m_vDir );
		}
		break;

	case NET_MSG_GCTRL_CURESTATEBLOW_BRD:
		{
			GLMSG::SNETPC_CURESTATEBLOW_BRD *pNetMsg = (GLMSG::SNETPC_CURESTATEBLOW_BRD *)nmg;

			for ( int i=0; i<EMBLOW_MULTI; ++i )
			{
				EMSTATE_BLOW emBLOW = m_sSTATEBLOWS[i].emBLOW;

				if ( emBLOW==EMBLOW_NONE )						continue;

				EMDISORDER emDIS = STATE_TO_DISORDER(emBLOW);
				if ( !(pNetMsg->dwCUREFLAG&emDIS) )				continue;

				DISABLEBLOW(i);
				FACTEFF::DeleteBlowEffect ( STARGETID(CROW_PC,m_dwGaeaID,m_vPos), m_pSkinChar, emBLOW );
			}
		}
		break;

	case NET_MSG_GCTRL_INVEN_DRUG_UPDATE:
		{
			GLMSG::SNETPC_INVEN_DRUG_UPDATE *pNetMsg = (GLMSG::SNETPC_INVEN_DRUG_UPDATE *)nmg;
			SINVENITEM* pInvenItem = NULL;
#if defined(VN_PARAM) //vietnamtest%%%
			if( pNetMsg->bVietnamInven )
			{
				pInvenItem = m_cVietnamInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			}else{
				pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			}
			if ( pInvenItem )
			{
				pInvenItem->sItemCustom.wTurnNum = pNetMsg->wTurnNum;
			}
#else
			pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			if ( pInvenItem )
			{
				pInvenItem->sItemCustom.wTurnNum = pNetMsg->wTurnNum;
			}
#endif
			
		}
		break;

	case NET_MSG_GCTRL_PUTON_DRUG_UPDATE:
		{
			GLMSG::SNETPC_PUTON_DRUG_UPDATE *pNetMsg = (GLMSG::SNETPC_PUTON_DRUG_UPDATE *)nmg;
			m_PutOnItems[pNetMsg->emSlot].wTurnNum = pNetMsg->wTurnNum;
		}
		break;

	case NET_MSG_GCTRL_REQ_GETSTORAGE_FB:
		{
			GLMSG::SNETPC_REQ_GETSTORAGE_FB *pNetMsg = (GLMSG::SNETPC_REQ_GETSTORAGE_FB *)nmg;
			
			//	Note : √¢∞Ì ¡§∫∏ ªı∑Œ πﬁ±‚ Ω√µµ.
			//
			const DWORD dwChannel = pNetMsg->dwChannel;

			m_lnStorageMoney = pNetMsg->lnMoney;

			m_bStorage[dwChannel] = false;
			m_dwNumStorageItem[dwChannel] = pNetMsg->dwNumStorageItem;
			m_cStorage[dwChannel].DeleteItemAll ();

			if ( m_cStorage[dwChannel].GetNumItems() == m_dwNumStorageItem[dwChannel] )
			{
				m_bStorage[dwChannel] = true;
				m_dwNumStorageItem[dwChannel] = UINT_MAX;
			}
		}
		break;

	case NET_MSG_GCTRL_REQ_GETSTORAGE_ITEM:
		{
			GLMSG::SNETPC_REQ_GETSTORAGE_ITEM *pNetMsg = (GLMSG::SNETPC_REQ_GETSTORAGE_ITEM *)nmg;

			const SINVENITEM &sInvenItem = pNetMsg->Data;
			const DWORD dwChannel = pNetMsg->dwChannel;

			m_cStorage[dwChannel].InsertItem ( sInvenItem.sItemCustom, sInvenItem.wPosX, sInvenItem.wPosY );

			if ( m_cStorage[dwChannel].GetNumItems() == m_dwNumStorageItem[dwChannel] )
			{
				m_bStorage[dwChannel] = true;
				m_dwNumStorageItem[dwChannel] = UINT_MAX;
			}

			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( sInvenItem.sItemCustom.sNativeID );
			if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && sInvenItem.sItemCustom.dwPetID != 0 )
			{
				GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
				NetMsg.dwPetID = sInvenItem.sItemCustom.dwPetID;
				NETSENDTOFIELD ( &NetMsg );
			}
		}
		break;

	case NET_MSG_GCTRL_STORAGE_INSERT:
		{
			GLMSG::SNETPC_STORAGE_INSERT *pNetMsg = (GLMSG::SNETPC_STORAGE_INSERT *)nmg;

			const SINVENITEM &sInvenItem = pNetMsg->Data;
			const DWORD dwChannel = pNetMsg->dwChannel;

			m_cStorage[dwChannel].InsertItem ( sInvenItem.sItemCustom, sInvenItem.wPosX, sInvenItem.wPosY );

			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( sInvenItem.sItemCustom.sNativeID );
			if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && sInvenItem.sItemCustom.dwPetID != 0 )
			{
				GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
				NetMsg.dwPetID = sInvenItem.sItemCustom.dwPetID;
				NETSENDTOFIELD ( &NetMsg );
			}
		}
		break;

	case NET_MSG_GCTRL_STORAGE_DELETE:
		{
			GLMSG::SNETPC_STORAGE_DELETE *pNetMsg = (GLMSG::SNETPC_STORAGE_DELETE *)nmg;

			const DWORD dwChannel = pNetMsg->dwChannel;
/*
			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ¡¶∞≈«ÿ¡ÿ¥Ÿ.
			SINVENITEM* pInvenItem = m_cStorage[dwChannel].GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			if ( pInvenItem )
			{
				PETCARDINFO_MAP_ITER iter = m_mapPETCardInfo.find ( pInvenItem->sItemCustom.dwPetID );
				if ( iter != m_mapPETCardInfo.end() ) m_mapPETCardInfo.erase ( iter );
			}
*/
			m_cStorage[dwChannel].DeleteItem ( pNetMsg->wPosX, pNetMsg->wPosY );
		}
		break;

	case NET_MSG_GCTRL_STORAGE_ITEM_UPDATE:
		{
			GLMSG::SNETPC_STORAGE_ITEM_UPDATE *pNetMsg = (GLMSG::SNETPC_STORAGE_ITEM_UPDATE *)nmg;
			const DWORD dwChannel = pNetMsg->dwChannel;

			SINVENITEM* pInvenItem = m_cStorage[dwChannel].GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			if ( !pInvenItem )		break;
			if ( pInvenItem->sItemCustom.sNativeID != pNetMsg->sItemCustom.sNativeID )	break;

			pInvenItem->sItemCustom = pNetMsg->sItemCustom;
		}
		break;

	case NET_MSG_GCTRL_STORAGE_DEL_INSERT:
		{
			GLMSG::SNETPC_STORAGE_DEL_AND_INSERT *pNetMsg = (GLMSG::SNETPC_STORAGE_DEL_AND_INSERT *)nmg;

			DWORD dwChannel = pNetMsg->dwChannel;
/*
			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ¡¶∞≈«ÿ¡ÿ¥Ÿ.
			SINVENITEM* pInvenItem = m_cStorage[dwChannel].GetItem ( pNetMsg->wDelX, pNetMsg->wDelX );
			if ( pInvenItem )
			{
				PETCARDINFO_MAP_ITER iter = m_mapPETCardInfo.find ( pInvenItem->sItemCustom.dwPetID );
				if ( iter != m_mapPETCardInfo.end() ) m_mapPETCardInfo.erase ( iter );
			}
*/
			m_cStorage[dwChannel].DeleteItem ( pNetMsg->wDelX, pNetMsg->wDelY );
			m_cStorage[dwChannel].InsertItem ( pNetMsg->sInsert.sItemCustom, pNetMsg->sInsert.wPosX, pNetMsg->sInsert.wPosY );

			// PET
			// ∆÷ƒ´µÂ ¡§∫∏∏¶ ø‰√ª«—¥Ÿ.
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sInsert.sItemCustom.sNativeID );
			if ( pItem && pItem->sBasicOp.emItemType == ITEM_PET_CARD && pNetMsg->sInsert.sItemCustom.dwPetID != 0  )
			{
				GLMSG::SNETPET_REQ_PETCARDINFO NetMsg;
				NetMsg.dwPetID = pNetMsg->sInsert.sItemCustom.dwPetID;
				NETSENDTOFIELD ( &NetMsg );
			}
		}
		break;

	case NET_MSG_GCTRL_STORAGE_DRUG_UPDATE:
		{
			GLMSG::SNETPC_STORAGE_DRUG_UPDATE *pNetMsg = (GLMSG::SNETPC_STORAGE_DRUG_UPDATE *)nmg;
			
			DWORD dwChannel = pNetMsg->dwChannel;

			SINVENITEM* pInvenItem = m_cStorage[dwChannel].GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			if ( pInvenItem )
			{
				pInvenItem->sItemCustom.wTurnNum = pNetMsg->wTurnNum;
			}
		}
		break;

	case NET_MSG_RESET_STATS_FB:
		{
			GLMSG::SNETPC_RESET_STATS_FB *pNetMsg = (GLMSG::SNETPC_RESET_STATS_FB *)nmg;
			RESET_STATS();
			CInnerInterface::GetInstance().PrintConsoleText ( "Resetting Stats. Complete.." );
		}
		break;

	case NET_MSG_GCTRL_STORAGE_UPDATE_MONEY:
		{
			GLMSG::SNETPC_REQ_STORAGE_UPDATE_MONEY *pNetMsg = (GLMSG::SNETPC_REQ_STORAGE_UPDATE_MONEY *)nmg;
			m_lnStorageMoney = pNetMsg->lnMoney;
		}
		break;

	case NET_MSG_GCTRL_REQ_SKILLQ_FB:
		{
			GLMSG::SNETPC_REQ_SKILLQUICK_FB *pNetMsg = (GLMSG::SNETPC_REQ_SKILLQUICK_FB *)nmg;
			m_sSKILLQUICK[pNetMsg->wSLOT] = pNetMsg->skill_id;

			//	Note : ∑± Ω∫≈≥∑Œ ¡ˆ¡§µ» Ω∫≈≥¿Ã ¡∏¡¶«œ¡ˆ æ ¿ª ∞ÊøÏ ¡ˆ¡§«ÿ¡‹.
			if ( GetskillRunSlot()==NATIVEID_NULL() )
			{
				ReqSkillRunSet(pNetMsg->wSLOT);
			}
		}
		break;

	case NET_MSG_GCTRL_REQ_ACTIONQ_FB:
		{
			GLMSG::SNETPC_REQ_ACTIONQUICK_FB *pNetMsg = (GLMSG::SNETPC_REQ_ACTIONQUICK_FB *)nmg;
			m_sACTIONQUICK[pNetMsg->wSLOT] = pNetMsg->sACT;
		}
		break;

	case NET_MSG_GCTRL_INVEN_ITEM_UPDATE:
		{
			GLMSG::SNET_INVEN_ITEM_UPDATE *pNetMsg = (GLMSG::SNET_INVEN_ITEM_UPDATE *)nmg;
			SINVENITEM* pInvenItem = NULL;
#if defined(VN_PARAM) //vietnamtest%%%
			if( pNetMsg->bVietnamInven )
			{
				pInvenItem = m_cVietnamInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			}else{
				pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
			}
#else
			pInvenItem = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
#endif
			if ( !pInvenItem )		break;
			if ( pInvenItem->sItemCustom.sNativeID != pNetMsg->sItemCustom.sNativeID )	break;

			pInvenItem->sItemCustom = pNetMsg->sItemCustom;
		}
		break;

	case NET_MSG_GCTRL_INVEN_GRINDING_FB:
		{
			GLMSG::SNET_INVEN_GRINDING_FB *pNetMsg = (GLMSG::SNET_INVEN_GRINDING_FB *)nmg;

			switch ( pNetMsg->emGrindFB )
			{
			case EMGRINDING_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMGRINDING_FAIL") );

				if( pNetMsg->emANTIDISAPPEAR == EMANTIDISAPPEAR_USE )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMGRINDING_ANTIDISAPPEAR") );
					DxSoundLib::GetInstance()->PlaySound ( "GRINDING_FAIL" );
				}
				else if ( pNetMsg->bTERMINATE )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMGRINDING_TERMINATE") );
					DxSoundLib::GetInstance()->PlaySound ( "GRINDING_BROKEN" );
				}
				else if ( pNetMsg->bRESET )
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMGRINDING_RESET") );
					DxSoundLib::GetInstance()->PlaySound ( "GRINDING_RESET" );
				}
				else
				{
					DxSoundLib::GetInstance()->PlaySound ( "GRINDING_FAIL" );
				}
				break;

			case EMGRINDING_SUCCEED:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::LIGHTSKYBLUE, ID2GAMEINTEXT("EMGRINDING_SUCCEED") );
					DxSoundLib::GetInstance()->PlaySound ( "GRINDING_SUCCEED" );
				}
				break;
			};
		}
		break;

		/* Selective Box, Baby011528, 2022-02-15 */
	case NET_MSG_GCTRL_INVEN_SELFORM_BOX_OPEN_FB:
	{
		GLMSG::SNET_INVEN_SELFORM_BOXOPEN_FB* pNetMsg = (GLMSG::SNET_INVEN_SELFORM_BOXOPEN_FB*)nmg;

		switch (pNetMsg->emFB)
		{
		case EMREQ_BOXOPEN_FB_FAIL:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_FAIL"));
			break;
		case EMREQ_BOXOPEN_FB_OK:
		{
			SITEM* pITEM = GLItemMan::GetInstance().GetItem(pNetMsg->sID);
			if (pITEM)
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_SELFORM_BOXOPEN_FB_OK"), pITEM->GetName());
			}
		}
		break;
		case EMREQ_BOXOPEN_FB_NOITEM:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOITEM"));
			
			break;
		case EMREQ_BOXOPEN_FB_NOBOX:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOBOX"));
			break;
		case EMREQ_BOXOPEN_FB_EMPTY:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_EMPTY"));
			break;
		case EMREQ_BOXOPEN_FB_NOTINVEN:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOTINVEN"));
			break;
		case EMREQ_BOXOPEN_FB_INVALIDITEM:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_INVALIDITEM"));
			break;
		};
		CInnerInterface::GetInstance().CloseSelectiveformBox();
	}
	break;
	//

	case NET_MSG_GCTRL_MYSTERY_BOX_FB:
		{
			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::LIGHTSKYBLUE, ID2GAMEINTEXT ("EMINVEN_RANDOMBOXOPEN_FB_UNLOCK") );
		}
		break;

	case NET_MSG_GCTRL_INVEN_BOXOPEN_FB:
		{
			GLMSG::SNET_INVEN_BOXOPEN_FB *pNetMsg = (GLMSG::SNET_INVEN_BOXOPEN_FB *)nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_BOXOPEN_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_FAIL") );
				break;
			case EMREQ_BOXOPEN_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_OK") );
				break;
			case EMREQ_BOXOPEN_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOITEM") );
				break;
			case EMREQ_BOXOPEN_FB_NOBOX:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOBOX") );
				break;
			case EMREQ_BOXOPEN_FB_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_EMPTY") );
				break;
			case EMREQ_BOXOPEN_FB_NOTINVEN:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_NOTINVEN") );
				break;
			case EMREQ_BOXOPEN_FB_INVALIDITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_BOXOPEN_FB_INVALIDITEM") );
				break;
			};
		}
		break;
	/*Codex System, Jhoniex 2/14/2024 */
	case NET_MSG_GCTRL_REGISTER_CODEX_FB:
		{
			GLMSG::SNET_REGISTER_CODEX_FB *pNetMsg = (GLMSG::SNET_REGISTER_CODEX_FB *)nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_REGISTER_CODEX_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REGISTER_CODEX_FB_FAIL") );
				break;
			case EMREQ_REGISTER_CODEX_FB_OK:
			{
				//SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pNetMsg->nidITEM );
				//if ( !pITEM )	break;

				//CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("INVEN_DEL_ITEM_TIMELMT"), pITEM->GetName() );
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_REGISTER_CODEX_FB_OK") );
				
			}
			break;
			case EMREQ_REGISTER_CODEX_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REGISTER_CODEX_FB_NOITEM") );
				break;
			case EMREQ_REGISTER_CODEX_FB_NOBOX:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REGISTER_CODEX_FB_NOBOX") );
				break;
			case EMREQ_REGISTER_CODEX_FB_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REGISTER_CODEX_FB_EMPTY") );
				break;
			case EMREQ_REGISTER_CODEX_FB_NOTINVEN:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REGISTER_CODEX_FB_NOTINVEN") );
				break;
			case EMREQ_REGISTER_CODEX_FB_INVALIDITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REGISTER_CODEX_FB_DONE") );
				break;
			};
		}
		break;
	case NET_MSG_GCTRL_INVEN_RANDOMBOXOPEN_FB:
		{
			GLMSG::SNET_INVEN_RANDOMBOXOPEN_FB *pNetMsg = (GLMSG::SNET_INVEN_RANDOMBOXOPEN_FB *)nmg;
			
			switch ( pNetMsg->emFB )
			{
			case EMINVEN_RANDOMBOXOPEN_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_FAIL") );
				break;

			case EMINVEN_RANDOMBOXOPEN_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_OK") );
				break;

			case EMINVEN_RANDOMBOXOPEN_FB_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_EMPTY") );
				break;

			case EMINVEN_RANDOMBOXOPEN_FB_BADITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_BADITEM") );
				break;

			case EMINVEN_RANDOMBOXOPEN_FB_NOINVEN:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_NOINVEN") );
				break;

			case EMINVEN_RANDOMBOXOPEN_FB_MISS:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_RANDOMBOXOPEN_FB_MISS") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_GMITEM_FB: //add itemcmd
		{
			GLMSG::SNET_INVEN_GMITEM_FB *pNetMsg = (GLMSG::SNET_INVEN_GMITEM_FB *)nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_GMITEM_FB_PASS:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_PASS") );
				break;
			case EMREQ_GMITEM_FB_NOTITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_NOTITEM") );
				break;
			case EMREQ_GMITEM_FB_MAX:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_MAX") );
				break;
			case EMREQ_GMITEM_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_FAIL") );
				break;
			case EMREQ_GMITEM_FB_INFAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_INFAIL") );
				break;
			case EMREQ_GMITEM_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMITEM_OK") );
				break;
			};
		}
		break;


	case NET_MSG_GCTRL_INVEN_DISJUNCTION_FB:
		{
			GLMSG::SNET_INVEN_DISJUNCTION_FB *pNetMsg = (GLMSG::SNET_INVEN_DISJUNCTION_FB *)nmg;
			
			switch ( pNetMsg->emFB )
			{
			case EMINVEN_DISJUNCTION_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_FAIL") );
				break;

			case EMINVEN_DISJUNCTION_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_OK") );
				break;

			case EMINVEN_DISJUNCTION_FB_BADITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_BADITEM") );
				break;

			case EMINVEN_DISJUNCTION_FB_NOINVEN:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_NOINVEN") );
				break;

			case EMINVEN_DISJUNCTION_FB_NONEED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_DISJUNCTION_FB_NONEED") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_DISGUISE_FB:
		{
			GLMSG::SNET_INVEN_DISGUISE_FB *pNetMsg = (GLMSG::SNET_INVEN_DISGUISE_FB *)nmg;
		
			switch ( pNetMsg->emFB )
			{
			case EMREQ_DISGUISE_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_FAIL") );
				break;
			case EMREQ_DISGUISE_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_OK") );
				break;
			case EMREQ_DISGUISE_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_NOITEM") );
				break;
			case EMREQ_DISGUISE_FB_NODISGUISE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_NODISGUISE") );
				break;
			case EMREQ_DISGUISE_FB_NOTSUIT:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_NOTSUIT") );
				break;
			case EMREQ_DISGUISE_FB_DEFSUIT:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_DEFSUIT") );
				break;
			case EMREQ_DISGUISE_FB_ALREADY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_DISGUISE_FB_ALREADY") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_CLEANSER_FB:
		{
			GLMSG::SNET_INVEN_CLEANSER_FB *pNetMsg = (GLMSG::SNET_INVEN_CLEANSER_FB *)nmg;
		
			switch ( pNetMsg->emFB )
			{
			case EMREQ_CLEANSER_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CLEANSER_FB_FAIL") );
				break;
			case EMREQ_CLEANSER_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_CLEANSER_FB_OK") );
				break;
			case EMREQ_CLEANSER_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CLEANSER_FB_NOITEM") );
				break;
			case EMREQ_CLEANSER_FB_NOCLEANSER:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CLEANSER_FB_NOCLEANSER") );
				break;
			case EMREQ_CLEANSER_FB_NONEED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CLEANSER_FB_NONEED") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_DEL_ITEM_TIMELMT:
		{
			GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT *pNetMsg = (GLMSG::SNET_INVEN_DEL_ITEM_TIMELMT *)nmg;
			
			SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pNetMsg->nidITEM );
			if ( !pITEM )	break;

			CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("INVEN_DEL_ITEM_TIMELMT"), pITEM->GetName() );
		}
		break;

	case NET_MSG_GCTRL_PUSHPULL_BRD:
		{
			GLMSG::SNET_PUSHPULL_BRD *pNetMsg = (GLMSG::SNET_PUSHPULL_BRD *)nmg;
			const D3DXVECTOR3 &vMovePos = pNetMsg->vMovePos;

			//	Note : π–∑¡≥Ø ¿ßƒ°∑Œ ¿Ãµø Ω√µµ.
			//
			BOOL bSucceed = m_actorMove.GotoLocation
			(
				D3DXVECTOR3(vMovePos.x,vMovePos.y+5,vMovePos.z),
				D3DXVECTOR3(vMovePos.x,vMovePos.y-5,vMovePos.z)
			);

			if ( bSucceed )
			{
				//	Note : π–∑¡≥™¥¬ ø¢º« Ω√¿€.
				//
				m_sTargetID.vPos = vMovePos;
				
				/*dash skill logic, Juver, 2017/06/17 */
				m_wActionAnim = pNetMsg->wActionAnim;

				if ( pNetMsg->bSkillDash )
				{
					/*dash skill logic, Juver, 2017/06/17 */
					TurnAction ( GLAT_SKILLDASH );
				}
				else if ( pNetMsg->bSkillMove )
				{
					/*push pull skill logic, Juver, 2017/06/05 */
					TurnAction ( GLAT_SKILLMOVE );
				}else{
					TurnAction ( GLAT_PUSHPULL );
				}

				//	Note : π–∏Æ¥¬ º”µµ º≥¡§.
				//
				/*push pull skill logic, Juver, 2017/06/04 */
				m_actorMove.SetMaxSpeed ( pNetMsg->fSpeed );

				/*push pull skill logic, Juver, 2017/06/05 */
				if ( pNetMsg->bSkillMove && vMovePos.x != FLT_MAX && vMovePos.y != FLT_MAX && vMovePos.z != FLT_MAX )
				{
					D3DXVECTOR3 vNewDirection = m_vPos - vMovePos;
					if ( pNetMsg->bReverseDir )	vNewDirection = vMovePos - m_vPos;
					if ( !DxIsMinVector(vNewDirection,0.2f) )
					{
						D3DXVec3Normalize ( &vNewDirection, &vNewDirection );
						m_vDir = vNewDirection;
					}
				}
			}
		}
		break;

	case NET_MSG_GCTRL_REGEN_GATE_FB:
		{
			GLMSG::SNETPC_REQ_REGEN_GATE_FB *pNetMsg = (GLMSG::SNETPC_REQ_REGEN_GATE_FB *)nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREGEN_GATE_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREGEN_GATE_FAIL") );
				}
				break;

			case EMREGEN_GATE_SUCCEED:
				{
					// Note : Ω√¿€±Õ»Øƒ´µÂø°º≠ ªÁøÎµ… ¡§∫∏∏¶ πﬁΩ¿¥œ¥Ÿ.
					m_sStartMapID = pNetMsg->sMapID;
					m_dwStartGate = pNetMsg->dwGateID;
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREGEN_GATE_SUCCEED") );
				}
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_CHARRESET_FB:
		{
			GLMSG::SNETPC_REQ_CHARRESET_FB *pNetMsg = (GLMSG::SNETPC_REQ_CHARRESET_FB *)nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREGEN_CHARRESET_SUCCEED:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREGEN_CHARRESET_SUCCEED") );

					RESET_STATS_SKILL();

					m_wStatsPoint = (WORD) pNetMsg->dwSTATS_P;
					m_dwSkillPoint = pNetMsg->dwSKILL_P;

					m_sRunSkill = SNATIVEID(false);
					m_sActiveSkill = SNATIVEID(false);
					SetDefenseSkill( false );
					m_sREACTION.RESET();

					//	Note : ∏∏æ‡ SKILL ªÁøÎ¡ﬂ¿Ã∂Û∏È ∏µŒ ∏Æº¬.
					//
					if ( IsACTION(GLAT_SKILL) )
					{
						TurnAction(GLAT_IDLE);
					}
				}
				break;

			case EMREGEN_CHARRESET_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREGEN_CHARRESET_FAIL") );
				break;

			case EMREGEN_CHARRESET_ITEM_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREGEN_CHARRESET_ITEM_FAIL") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_FIRECRACKER_FB:
		{
			GLMSG::SNETPC_REQ_FIRECRACKER_FB *pNetMsg = (GLMSG::SNETPC_REQ_FIRECRACKER_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREQ_FIRECRACKER_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_FIRECRACKER_FB_FAIL") );
				break;

			case EMREQ_FIRECRACKER_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_FIRECRACKER_FB_OK") );
				break;

			case EMREQ_FIRECRACKER_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_FIRECRACKER_FB_NOITEM") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_FIRECRACKER_BRD:
		{
			GLMSG::SNETPC_REQ_FIRECRACKER_BRD *pNetMsg = (GLMSG::SNETPC_REQ_FIRECRACKER_BRD *) nmg;
			
			SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pNetMsg->nidITEM );
			if ( !pITEM )	break;

			D3DXMATRIX matEffect;
			D3DXMatrixTranslation ( &matEffect, pNetMsg->vPOS.x, pNetMsg->vPOS.y, pNetMsg->vPOS.z );

			//	∫∏¿Ã¡ˆ æ ¥¬ ≈∏∞π¿œ ∞ÊøÏ ≈∏∞› ¿Ã∆—∆Æ¥¬ ª˝∑´µ .
			DxEffGroupPlayer::GetInstance().NewEffGroup
			(
				pITEM->GetTargetEffect(),
				matEffect,
				&STARGETID(CROW_PC,0,pNetMsg->vPOS)
			);
		}
		break;

	case NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGETNUM_UPDATE:
		{
			GLMSG::SNETPC_INVEN_VIETNAM_ITEMGETNUM_UPDATE *pNetMsg = (GLMSG::SNETPC_INVEN_VIETNAM_ITEMGETNUM_UPDATE *) nmg;

			m_dwVietnamInvenCount = pNetMsg->dwVietnamInvenCount;
		}
		break;

	case NET_MSG_GCTRL_INVEN_VIETNAM_ITEMGET_FB:
		{
			GLMSG::SNETPC_INVEN_VIETNAM_ITEMGET_FB *pNetMsg = (GLMSG::SNETPC_INVEN_VIETNAM_ITEMGET_FB *) nmg;

			m_dwVietnamInvenCount = pNetMsg->dwVietnamInvenCount;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("VIETNAM_USE_ITEMGET_ITEM") );
		}
		break;

	case NET_MSG_GCTRL_INVEN_VIETNAM_EXPGET_FB:
		{
			m_lVNGainSysMoney = 0;
//			m_lnMoney		  = 0;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("VIETNAM_USE_EXPGET_ITEM") );
		}
		break;

#if defined(VN_PARAM) //vietnamtest%%%
	case NET_MSG_VIETNAM_ALLINITTIME:
		{
			GLMSG::SNETPC_VIETNAM_ALLINITTIME *pNetMsg = (GLMSG::SNETPC_VIETNAM_ALLINITTIME *)nmg;
			m_dwVietnamGainType = GAINTYPE_MAX;
			m_sVietnamSystem.Init();
			m_sVietnamSystem.loginTime = pNetMsg->initTime;
			m_tLoginTime = m_sVietnamSystem.loginTime;
			GLGaeaClient::GetInstance().SetCurrentTime( m_tLoginTime );
			m_sVietnamSystem.gameTime  = 0;
//			CInnerInterface::GetInstance().SET_VNGAINTYPE_GAUGE( 0, 300 );
			CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("VIETNAM_TIME_ALLINIT") );
		}
		break;
#endif

	case NET_MSG_GCTRL_INVEN_RESET_SKST_FB:
		{
			GLMSG::SNET_INVEN_RESET_SKST_FB *pNetMsg = (GLMSG::SNET_INVEN_RESET_SKST_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREQ_RESET_SKST_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RESET_SKST_FB_FAIL") );
				break;
			case EMREQ_RESET_SKST_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RESET_SKST_FB_OK"),
					pNetMsg->wITEM_NUM );

				RESET_STATS_SKILL(pNetMsg->wSTATS_POINT);
				break;
			case EMREQ_RESET_SKST_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RESET_SKST_FB_NOITEM") );
				break;
			case EMREQ_RESET_SKST_FB_NOINVEN:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RESET_SKST_FB_NOINVEN"), pNetMsg->wITEM_NUM );
				break;
			};
		}
		break;

	case NET_MSG_GM_MOVE2GATE_FB:
		{
			GLMSG::SNET_GM_MOVE2GATE_FB *pNetMsg = (GLMSG::SNET_GM_MOVE2GATE_FB *) nmg;
			SetPosition ( pNetMsg->vPOS );
			//DoActWait ();
		}
		break;

	case NET_MSG_GCTRL_CURE_FB:
		{
			GLMSG::SNETPC_REQ_CURE_FB *pNetMsg = (GLMSG::SNETPC_REQ_CURE_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREGEN_CURE_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREGEN_CURE_FAIL") );
				break;

			case EMREGEN_CURE_SUCCEED:
				m_sHP = pNetMsg->sHP;
				m_sMP = pNetMsg->sMP;
				m_sSP = pNetMsg->sSP;

				for ( int i=0; i<EMBLOW_MULTI; ++i )
				{
					if ( m_sSTATEBLOWS[i].emBLOW==EMBLOW_NONE )		continue;

					EMDISORDER emDIS = STATE_TO_DISORDER(m_sSTATEBLOWS[i].emBLOW);
					if ( !(pNetMsg->dwCUREFLAG&emDIS) )				continue;

					m_sSTATEBLOWS[i].fAGE = 0.0f;
				}
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_CHARCARD_FB:
		{
			GLMSG::SNET_INVEN_CHARCARD_FB *pNetMsg = (GLMSG::SNET_INVEN_CHARCARD_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_CHARCARD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHARCARD_FB_FAIL") );
				break;

			case EMREQ_CHARCARD_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_CHARCARD_FB_OK") );
				break;

			case EMREQ_CHARCARD_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHARCARD_FB_NOITEM") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_STORAGECARD_FB:
		{
			GLMSG::SNET_INVEN_STORAGECARD_FB *pNetMsg = (GLMSG::SNET_INVEN_STORAGECARD_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_STORAGECARD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGECARD_FB_FAIL") );
				break;

			case EMREQ_STORAGECARD_FB_OK:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_STORAGECARD_FB_OK") );

					CTime tLMT(pNetMsg->tSTORAGE_LIMIT);
					CTimeSpan tSPAN(pNetMsg->tSPAN);
					CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("STORAGE_EX_STATE"),
						pNetMsg->wSTORAGE+1, tLMT.GetYear(), tLMT.GetMonth(), tLMT.GetDay(), tLMT.GetHour(),
						tSPAN.GetDays() );

					int nINDEX = pNetMsg->wSTORAGE-EMSTORAGE_CHANNEL_SPAN;
					m_bSTORAGE[nINDEX] = true;
					m_tSTORAGE[nINDEX] = pNetMsg->tSTORAGE_LIMIT;
				}
				break;

			case EMREQ_STORAGECARD_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGECARD_FB_NOITEM") );
				break;

			case EMREQ_STORAGECARD_FB_INVNUM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGECARD_FB_INVNUM") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_STORAGE_STATE:
		{
			GLMSG::SNETPC_STORAGE_STATE *pNetMsg = (GLMSG::SNETPC_STORAGE_STATE *) nmg;
			for ( int i=0; i<EMSTORAGE_CHANNEL_SPAN_SIZE; ++i  )
			{
				if ( m_bSTORAGE[i]==true && pNetMsg->bVALID[i]==false )
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_STORAGE_END"), i+1 );
				}

				m_bSTORAGE[i] = pNetMsg->bVALID[i];
			}
		}
		break;

		//µ•Ø≈§…Ø≈•d
	case NET_MSG_GCTRL_INVEN_UPGRADE_CARD_FB:
	{
		GLMSG::SNET_INVEN_UPGRADE_CARD_FB* pNetMsg = (GLMSG::SNET_INVEN_UPGRADE_CARD_FB*)nmg;
		switch (pNetMsg->emFB)
		{
		case EMREQ_UPGRADE_CARD_FB_FAIL:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_UPGRADE_CARD_FB_FAIL"));
			break;

		case EMREQ_UPGRADE_CARD_FB_OK:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_UPGRADE_CARD_FB_OK"));
			m_wLevel = pNetMsg->wLevel;
			m_wStatsPoint = pNetMsg->wStatsPoint;
			m_dwSkillPoint = pNetMsg->dwSkillPoint;
			break;

		case EMREQ_UPGRADE_CARD_FB_NOITEM:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_UPGRADE_CARD_FB_NOITEM"));
			break;

		case EMREQ_UPGRADE_CARD_FB_MAX:
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_UPGRADE_CARD_FB_MAXLINE"));
			break;
		};
	}
	break;

	case NET_MSG_GCTRL_INVEN_INVENLINE_FB:
		{
			GLMSG::SNET_INVEN_INVENLINE_FB *pNetMsg = (GLMSG::SNET_INVEN_INVENLINE_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_INVENLINE_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_INVENLINE_FB_FAIL") );
				break;

			case EMREQ_INVENLINE_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_INVENLINE_FB_OK") );
				
				//	Note : ¿Œ∫• ¡Ÿºˆ º≥¡§.
				m_wINVENLINE = pNetMsg->wINVENLINE;

				//	Note : «ˆ¡¶ »∞º∫»≠µ» ¿Œ∫• ∂Û¿Œ º≥¡§.
				//
				m_cInventory.SetAddLine ( GetOnINVENLINE(), true );
				CInnerInterface::GetInstance().SetInventorySlotViewSize ( EM_INVEN_DEF_SIZE_Y + GetOnINVENLINE() );
				break;

			case EMREQ_INVENLINE_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_INVENLINE_FB_NOITEM") );
				break;

			case EMREQ_INVENLINE_FB_MAXLINE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_INVENLINE_FB_MAXLINE") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_REMODELOPEN_FB:
		{
			GLMSG::SNET_INVEN_REMODELOPEN_FB *pNetMsg = (GLMSG::SNET_INVEN_REMODELOPEN_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREQ_REMODELOPEN_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REMODELOPEN_FB_FAIL") );
				break;

			case EMREQ_REMODELOPEN_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_REMODELOPEN_FB_OK") );
				CInnerInterface::GetInstance().OpenItemRebuildWindow();
				m_sRebuildCardPos.SET( pNetMsg->wPosX, pNetMsg->wPosY );
				break;

			case EMREQ_REMODELOPEN_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REMODELOPEN_FB_NOITEM") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_GARBAGEOPEN_FB:
		{
			GLMSG::SNET_INVEN_GARBAGEOPEN_FB *pNetMsg = (GLMSG::SNET_INVEN_GARBAGEOPEN_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMREQ_GARBAGEOPEN_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GARBAGEOPEN_FB_FAIL") );
				break;

			case EMREQ_GARBAGEOPEN_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GARBAGEOPEN_FB_OK") );
				CInnerInterface::GetInstance().OpenItemGarbageWindow();
				break;

			case EMREQ_GARBAGEOPEN_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GARBAGEOPEN_FB_NOITEM") );
				break;
			};
		}
		break;

		// ∂Ùƒø ∞¸∏Æ¿Œ∞˙¿« ¥Î»≠∏¶ ¡¶ø‹«— ±◊π€¿« πÊπ˝¿∏∑Œ √¢∞Ì ø≠∂˜ ( ±‰±ﬁ √¢∞Ì ø¨∞· ƒ´µÂ... etc )
	case NET_MSG_GCTRL_INVEN_STORAGEOPEN_FB:
		{
			GLMSG::SNET_INVEN_STORAGEOPEN_FB *pNetMsg = (GLMSG::SNET_INVEN_STORAGEOPEN_FB *) nmg;
			//SINVENITEM sItem( pNetMsg->wPosX, pNetMsg->wPosY );
			switch ( pNetMsg->emFB )
			{
			case EMREQ_STORAGEOPEN_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGEOPEN_FB_FAIL") );
				break;

			case EMREQ_STORAGEOPEN_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_STORAGEOPEN_FB_OK") );
				
				//	Note : √¢∞Ì ø≠±‚.
				CInnerInterface::GetInstance().SetDefaultPosInterface( INVENTORY_WINDOW );
				CInnerInterface::GetInstance().SetDefaultPosInterface( STORAGE_WINDOW );
				//CInnerInterface::GetInstance().ShowGroupFocus ( INVENTORY_WINDOW );
				CInnerInterface::GetInstance().ShowInventoryWindow();
				CInnerInterface::GetInstance().SetStorageWindowOpen ( 0 );
				break;

			case EMREQ_STORAGEOPEN_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_STORAGEOPEN_FB_NOITEM") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_PREMIUMSET_FB:
		{
			GLMSG::SNET_INVEN_PREMIUMSET_FB *pNetMsg = (GLMSG::SNET_INVEN_PREMIUMSET_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_PREMIUMSET_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PREMIUMSET_FB_FAIL") );
				break;

			case EMREQ_PREMIUMSET_FB_OK:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_PREMIUMSET_FB_OK") );

					CTime tLMT(pNetMsg->tLMT);
					CTimeSpan tSPAN(pNetMsg->tSPAN);
					CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("PREMIUMSET_EX_STATE"),
						tLMT.GetYear(), tLMT.GetMonth(), tLMT.GetDay(), tLMT.GetHour(),
						tSPAN.GetDays() );

					m_tPREMIUM = pNetMsg->tLMT;
					m_bPREMIUM = true;

					//	Note : «ˆ¡¶ »∞º∫»≠µ» ¿Œ∫• ∂Û¿Œ º≥¡§.
					//
					m_cInventory.SetAddLine ( GetOnINVENLINE(), true );
					CInnerInterface::GetInstance().SetInventorySlotViewSize ( EM_INVEN_DEF_SIZE_Y + GetOnINVENLINE() );
				}
				break;

			case EMREQ_PREMIUMSET_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PREMIUMSET_FB_NOITEM") );
				break;

			case EMREQ_PREMIUMSET_FB_NOTINVEN:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_PREMIUMSET_FB_NOTINVEN") );
				break;
			};
		}
		break;

	case NET_MSG_CHAT_LOUDSPEAKER_FB:
		{
			GLMSG::SNETPC_CHAT_LOUDSPEAKER_FB *pNetMsg = (GLMSG::SNETPC_CHAT_LOUDSPEAKER_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMCHAT_LOUDSPEAKER_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAT_LOUDSPEAKER_FAIL") );
				break;

			case EMCHAT_LOUDSPEAKER_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMCHAT_LOUDSPEAKER_OK") );
				break;

			case EMCHAT_LOUDSPEAKER_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAT_LOUDSPEAKER_NOITEM") );
				break;

			case EMCHAT_LOUDSPEAKER_BLOCK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAT_LOUDSPEAKER_BLOCK") );
				break;

				/*megaphone set, Juver, 2018/01/02 */
			case EMCHAT_LOUDSPEAKER_DISABLED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHAT_LOUDSPEAKER_DISABLED") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_PREMIUM_STATE:
		{
			GLMSG::SNETPC_PREMIUM_STATE *pNetMsg = (GLMSG::SNETPC_PREMIUM_STATE *) nmg;
			
			if ( pNetMsg->bPREMIUM==false && m_bPREMIUM==true )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PREMIUMSERVICE_END") );
			}

			m_bPREMIUM = pNetMsg->bPREMIUM;
		}
		break;

	case NET_MSG_GCTRL_POSITIONCHK_BRD:
		{
			GLMSG::SNET_POSITIONCHK_BRD *pNetNsg = (GLMSG::SNET_POSITIONCHK_BRD *)nmg;
			m_vServerPos = pNetNsg->vPOS;
		}
		break;

	case NET_MSG_GCTRL_NPC_ITEM_TRADE_FB:
		{
			GLMSG::SNETPC_REQ_NPC_ITEM_TRADE_FB *pNetNsg = (GLMSG::SNETPC_REQ_NPC_ITEM_TRADE_FB *)nmg;
			switch ( pNetNsg->emFB )
			{
			case EMNPC_ITEM_TRADE_SUCCEED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMNPC_ITEM_TRADE_SUCCEED") );
				break;
			case EMNPC_ITEM_TRADE_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_TRADE_FAIL") );
				break;
			case EMNPC_ITEM_TRADE_ITEM_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_TRADE_ITEM_FAIL") );
				break;
			case EMNPC_ITEM_TRADE_INSERT_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_TRADE_INSERT_FAIL") );
				break;
			};
		}
		break;
		
	case NET_MSG_GCTRL_ITEMSHOPOPEN_BRD:
		{
			GLMSG::SNETPC_OPEN_ITEMSHOP_BRD* pNetMsg = ( GLMSG::SNETPC_OPEN_ITEMSHOP_BRD* ) nmg;
			m_bItemShopOpen = pNetMsg->bOpen;

			//if ( m_bItemShopOpen )	CInnerInterface::GetInstance().GetItemShopIconMan()->ADD_SHOP_ICON( m_dwGaeaID );
			//else	CInnerInterface::GetInstance().GetItemShopIconMan()->DEL_SHOP_ICON( m_dwGaeaID );	
		}
		break;

	case NET_MSG_GCTRL_GET_CHARGEDITEM_FROMDB_FB:
		{
			GLMSG::SNET_GET_CHARGEDITEM_FROMDB_FB* pNetMsg = (GLMSG::SNET_GET_CHARGEDITEM_FROMDB_FB*)nmg;
			switch ( pNetMsg->emFB )
			{
			case EMREQ_CHARGEDITEM_FROMDB_FB_END:
				CInnerInterface::GetInstance().SetItemBankInfo ();
				break;

			case EMREQ_CHARGEDITEM_FROMDB_FB_OK:
				ADDSHOPPURCHASE ( pNetMsg->szPurKey, pNetMsg->nidITEM );
				break;
			}
		}
		break;

	case NET_MSG_GCTRL_CHARGED_ITEM_GET_FB:
		{
			GLMSG::SNET_CHARGED_ITEM_GET_FB *pNetNsg = (GLMSG::SNET_CHARGED_ITEM_GET_FB *)nmg;
			switch ( pNetNsg->emFB )
			{
			case EMCHARGED_ITEM_GET_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMCHARGED_ITEM_GET_FB_FAIL") );
				break;
			case EMCHARGED_ITEM_GET_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHARGED_ITEM_GET_FB_OK") );
				break;
			case EMCHARGED_ITEM_GET_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHARGED_ITEM_GET_FB_NOITEM") );
				break;
			case EMCHARGED_ITEM_GET_FB_NOINVEN:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCHARGED_ITEM_GET_FB_NOINVEN") );
				break;
			};
			
		}
		break;

	case NET_MSG_GCTRL_CHARGED_ITEM_DEL:
		{
			GLMSG::SNET_CHARGED_ITEM_DEL *pNetNsg = (GLMSG::SNET_CHARGED_ITEM_DEL *)nmg;
			DELSHOPPURCHASE ( pNetNsg->dwID );
			CInnerInterface::GetInstance().REFRESH_ITEMBANK ();
		}
		break;

	case NET_MSG_GCTRL_REVIVE_FB:
		{
			GLMSG::SNETPC_REQ_REVIVE_FB *pNetNsg = (GLMSG::SNETPC_REQ_REVIVE_FB *)nmg;

			switch ( pNetNsg->emFB )
			{
			case EMREQ_REVIVE_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REVIVE_FB_FAIL") );
				break;
			case EMREQ_REVIVE_FB_OK:
				/*pvp tyranny, Juver, 2017/08/25 */
				if ( pNetNsg->bEventRevive )
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_REVIVE_FB_OK2") );
				else
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_REVIVE_FB_OK") );
				break;
			case EMREQ_REVIVE_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REVIVE_FB_NOITEM") );
				break;
			case EMREQ_REVIVE_FB_NOTUSE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REVIVE_FB_NOTUSE") );
				break;
			case EMREQ_REVIVE_FB_COOLTIME:
				break;
			};

			if ( pNetNsg->emFB != EMREQ_REVIVE_FB_OK )
			{
				ReqReBirth();
				CInnerInterface::GetInstance().CloseAllWindow ();
			}
		}
		break;

	case NET_MSG_GCTRL_GETEXP_RECOVERY_FB:
		{

			GLMSG::SNETPC_REQ_GETEXP_RECOVERY_FB *pNetMsg = (GLMSG::SNETPC_REQ_GETEXP_RECOVERY_FB *)nmg;

			// »∏∫π«“ ∞Ê«Ëƒ°∞° æ¯Ω¿¥œ¥Ÿ.
			if ( pNetMsg->nReExp <= 0 )	
			{
				CInnerInterface::GetInstance().PrintMsgText( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREQ_RECOVERY_FB_NOREEXP" ) );

				// ∫Œ»∞«œ±‚
				ReqReBirth();					
				// ø≠∑¡¡¯ √¢µÈ ¥›±‚
				CInnerInterface::GetInstance().CloseAllWindow ();

				return;
			}

			// º“¡ˆ ±›æ◊¿Ã ∏¿⁄∏£¥Ÿ∏È ∫Œ»∞Ω√≈≤¥Ÿ.
			if ( pNetMsg->nDecMoney > m_lnMoney )	
			{
				CInnerInterface::GetInstance().PrintMsgText( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREQ_RECOVERY_FB_NOMONEY" ) );

				// ∫Œ»∞«œ±‚
				ReqReBirth();					
				// ø≠∑¡¡¯ √¢µÈ ¥›±‚
				CInnerInterface::GetInstance().CloseAllWindow ();

				return;				
			}

			CString strTemp;
			strTemp.Format ( ID2GAMEINTEXT("MODAL_RECOVERY_EXP"), 
							 pNetMsg->nDecExp, pNetMsg->nReExp, pNetMsg->nDecMoney );

			DoModal( strTemp, MODAL_QUESTION, OKCANCEL, MODAL_RECOVERY_EXP );

		}
		break;

	case NET_MSG_GCTRL_GETEXP_RECOVERY_NPC_FB:
		{
// ∞Ê«Ëƒ°»∏∫π_¡§¿«_Npc
#if defined( _RELEASED ) || defined ( KRT_PARAM ) || defined ( KR_PARAM ) || defined ( TH_PARAM ) || defined ( MYE_PARAM ) || defined ( MY_PARAM ) || defined ( PH_PARAM ) || defined ( CH_PARAM ) || defined ( TW_PARAM ) || defined ( HK_PARAM ) || defined ( GS_PARAM )
			GLMSG::SNETPC_REQ_GETEXP_RECOVERY_NPC_FB *pNetMsg = (GLMSG::SNETPC_REQ_GETEXP_RECOVERY_NPC_FB *)nmg;

			// »∏∫π«“ ∞Ê«Ëƒ°∞° æ¯Ω¿¥œ¥Ÿ.
			if ( pNetMsg->nReExp <= 0 )	
			{
				CInnerInterface::GetInstance().PrintMsgText( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREQ_RECOVERY_NPC_FB_NOREEXP" ) );
				
				return;
			}

			// º“¡ˆ ±›æ◊¿Ã ∏¿⁄∏£¥Ÿ∏È ∫Œ»∞Ω√≈≤¥Ÿ.
			if ( pNetMsg->nDecMoney > m_lnMoney )	
			{
				CInnerInterface::GetInstance().PrintMsgText( 
						NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREQ_RECOVERY_NPC_FB_NOMONEY" ) );				

				return;				
			}

			CString strTemp;
			strTemp.Format ( ID2GAMEINTEXT("MODAL_RECOVERY_NPC_EXP"), pNetMsg->nReExp, pNetMsg->nDecMoney );

			DoModal( strTemp, MODAL_QUESTION, OKCANCEL, MODAL_RECOVERY_NPC_EXP );

			CInnerInterface::GetInstance().GetModalWindow()->SetModalData( pNetMsg->dwNPCID, 0 );
#endif

		}
		break;
	
	case NET_MSG_GCTRL_RECOVERY_FB:
		{
			GLMSG::SNETPC_REQ_RECOVERY_FB *pNetNsg = (GLMSG::SNETPC_REQ_RECOVERY_FB *)nmg;

			switch ( pNetNsg->emFB )
			{
			case EMREQ_RECOVERY_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_FAIL") );
				break;
			case EMREQ_RECOVERY_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_OK"),  pNetNsg->nReExp );
				break;
			case EMREQ_RECOVERY_FB_NOMONEY:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_NOMONEY") );
				break;
			case EMREQ_RECOVERY_FB_NOREEXP:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_NOREEXP") );
				break;
			case EMREQ_RECOVERY_FB_NOTUSE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_NOTUSE") );
				break;
			};

			if ( pNetNsg->emFB != EMREQ_RECOVERY_FB_OK )
			{
				ReqReBirth();
				CInnerInterface::GetInstance().CloseAllWindow ();
			}
		}
		break;

	case NET_MSG_GCTRL_RECOVERY_NPC_FB:
		{
// ∞Ê«Ëƒ°»∏∫π_¡§¿«_Npc
#if defined( _RELEASED ) || defined ( KRT_PARAM ) || defined ( KR_PARAM ) || defined ( TH_PARAM ) || defined ( MYE_PARAM ) || defined ( MY_PARAM ) || defined ( PH_PARAM ) || defined ( CH_PARAM ) || defined ( TW_PARAM ) || defined ( HK_PARAM ) || defined ( GS_PARAM )
			GLMSG::SNETPC_REQ_RECOVERY_NPC_FB *pNetNsg = (GLMSG::SNETPC_REQ_RECOVERY_NPC_FB *)nmg;

			switch ( pNetNsg->emFB )
			{
			case EMREQ_RECOVERY_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_NPC_FB_FAIL") );
				break;
			case EMREQ_RECOVERY_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_RECOVERY_NPC_FB_OK"), pNetNsg->nReExp );
				break;
			case EMREQ_RECOVERY_FB_NOMONEY:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_NPC_FB_NOMONEY") );
				break;
			case EMREQ_RECOVERY_FB_NOREEXP:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_NPC_FB_NOREEXP") );
				break;
			case EMREQ_RECOVERY_FB_NOTUSE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_NPC_FB_NOTUSE") );
				break;
			};

			if ( pNetNsg->emFB != EMREQ_RECOVERY_FB_OK )
			{
				ReqReBirth();
				CInnerInterface::GetInstance().CloseAllWindow ();
			}
#endif
		}
		break;

	case NET_MSG_GCTRL_PMARKET_TITLE_FB:
		{
			GLMSG::SNETPC_PMARKET_TITLE_FB *pNetMsg = (GLMSG::SNETPC_PMARKET_TITLE_FB *) nmg;
			m_sPMarket.SetTITLE ( pNetMsg->szPMarketTitle );

			//	Note : ¿Œ≈Õ∆‰¿ÃΩ∫ø° ∞ªΩ≈?
		}
		break;

	case NET_MSG_GCTRL_PMARKET_REGITEM_FB:
		{
			GLMSG::SNETPC_PMARKET_REGITEM_FB *pNetMsg = (GLMSG::SNETPC_PMARKET_REGITEM_FB *) nmg;
			
			switch ( pNetMsg->emFB )
			{
			case EMPMARKET_REGITEM_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_FAIL") );
				break;

			case EMPMARKET_REGITEM_FB_OK:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_OK") );

					SINVENITEM* pINVENITEM = m_cInventory.GetItem ( pNetMsg->wPosX, pNetMsg->wPosY );
					if ( pINVENITEM )
					{
						m_sPMarket.RegItem ( *pINVENITEM, pNetMsg->llMoney, pNetMsg->dwNum, pNetMsg->sSALEPOS );

						//	Note : ¿Œ≈Õ∆‰¿ÃΩ∫ø° ∞ªΩ≈?
					}
				}
				break;
			
			case EMPMARKET_REGITEM_FB_MAXNUM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_MAXNUM") );
				break;

			case EMPMARKET_REGITEM_FB_REGITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_REGITEM") );
				break;

			case EMPMARKET_REGITEM_FB_NOSALE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_REGITEM_FB_NOSALE") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_PMARKET_DISITEM_FB:
		{
			GLMSG::SNETPC_PMARKET_DISITEM_FB *pNetMsg = (GLMSG::SNETPC_PMARKET_DISITEM_FB *) nmg;

			m_sPMarket.DisItem ( pNetMsg->sSALEPOS );

			//	Note : ¿Œ≈Õ∆‰¿ÃΩ∫ø° ∞ªΩ≈?
		}
		break;

	case NET_MSG_GCTRL_MOVESTATE_BRD:	
		{
			GLMSG::SNETPC_MOVESTATE_BRD *pNetMsg = reinterpret_cast<GLMSG::SNETPC_MOVESTATE_BRD*> ( nmg );

			BOOL bRun = IsSTATE ( EM_ACT_RUN );
			BOOL bToRun = pNetMsg->dwActState&EM_ACT_RUN;

			if ( bRun != bToRun )
			{
				//	Note : ¿Ãµø ªÛ≈¬ ∫Ø∞Ê.
				if ( bToRun )		SetSTATE(EM_ACT_RUN);
				else				ReSetSTATE(EM_ACT_RUN);

				m_actorMove.SetMaxSpeed ( GetMoveVelo () );
			}

			if ( IsSTATE(EM_ACT_RUN) )
			{
				CBasicGameMenu * pGameMenu = CInnerInterface::GetInstance().GetGameMenu();
				if( pGameMenu ) pGameMenu->SetFlipRunButton( TRUE );
			}
			else
			{
				CBasicGameMenu * pGameMenu = CInnerInterface::GetInstance().GetGameMenu();
				if( pGameMenu ) pGameMenu->SetFlipRunButton( FALSE );
			}

			if ( pNetMsg->dwActState & EM_REQ_VISIBLENONE )	SetSTATE ( EM_REQ_VISIBLENONE );
			else											ReSetSTATE ( EM_REQ_VISIBLENONE );

			if ( pNetMsg->dwActState & EM_REQ_VISIBLEOFF )	SetSTATE ( EM_REQ_VISIBLEOFF );
			else											ReSetSTATE ( EM_REQ_VISIBLEOFF );

			if ( pNetMsg->dwActState & EM_ACT_PEACEMODE )	SetSTATE ( EM_ACT_PEACEMODE );
			else											ReSetSTATE ( EM_ACT_PEACEMODE );

			/*vehicle booster system, Juver, 2017/08/12 */
			if ( pNetMsg->dwActState & EM_ACT_VEHICLE_BOOSTER )	
				SetSTATE ( EM_ACT_VEHICLE_BOOSTER );
			else										
				ReSetSTATE ( EM_ACT_VEHICLE_BOOSTER );
		}
		break;

	case NET_MSG_GCTRL_PMARKET_OPEN_FB:
		{
			GLMSG::SNETPC_PMARKET_OPEN_FB *pNetMsg = (GLMSG::SNETPC_PMARKET_OPEN_FB *) nmg;
			
			switch ( pNetMsg->emFB )
			{
			case EMPMARKET_OPEN_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_FAIL") );
				break;
			
			case EMPMARKET_OPEN_FB_OK:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_OK") );
					m_sPMarket.DoMarketOpen();
				}
				break;

			case EMPMARKET_OPEN_FB_ALREADY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_ALREADY") );
				break;
			
			case EMPMARKET_OPEN_FB_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_EMPTY") );
				break;

				/* map private market setting, Juver, 2017/10/02 */
			case EMPMARKET_OPEN_FB_NOMAP:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_NOMAP") );
				break;

				/*private market set, Juver, 2018/01/02 */
			case EMPMARKET_OPEN_FB_NOTALLOWED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_OPEN_FB_NOTALLOWED") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_PMARKET_BUY_FB:
		{
			GLMSG::SNETPC_PMARKET_BUY_FB *pNetMsg = (GLMSG::SNETPC_PMARKET_BUY_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMPMARKET_BUY_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_FAIL") );
				break;
			case EMPMARKET_BUY_FB_OK:
				if ( pNetMsg->dwGaeaID == m_dwGaeaID )
				{
					SSALEITEM *pSALEITEM = m_sPMarket.GetItem ( pNetMsg->sSALEPOS );
					if ( pSALEITEM )
					{
						SITEM *pITEM = GLItemMan::GetInstance().GetItem ( pSALEITEM->sITEMCUSTOM.sNativeID );
						if ( pITEM )
						{
							CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPMARKET_BUY_FB_OK_SELLER"), pITEM->GetName(), pNetMsg->dwNum );
						}
					}
				}
				else
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMPMARKET_BUY_FB_OK") );
				}
				break;
			case EMPMARKET_BUY_FB_NUM:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_NUM") );

					PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar ( pNetMsg->dwGaeaID );
					if ( pCHAR )
					{
						bool bSOLD = pNetMsg->dwNum == 0;
						pCHAR->m_sPMarket.SetSaleState ( pNetMsg->sSALEPOS, pNetMsg->dwNum, bSOLD );
					}
				}
				break;
			case EMPMARKET_BUY_FB_LOWMONEY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_LOWMONEY") );
				break;
			case EMPMARKET_BUY_FB_SOLD:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_SOLD") );
				break;
			case EMPMARKET_BUY_FB_NOINVEN:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_NOINVEN") );
				break;
			case EMPMARKET_BUY_FB_NOTIME:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMPMARKET_BUY_FB_NOTIME") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_PMARKET_ITEM_UPDATE_BRD:
		{
			GLMSG::SNETPC_PMARKET_ITEM_UPDATE_BRD *pNetMsg = (GLMSG::SNETPC_PMARKET_ITEM_UPDATE_BRD *) nmg;

			SSALEITEM *pSALEITEM = m_sPMarket.GetItem ( pNetMsg->sSALEPOS );
			if ( pSALEITEM )
			{
				pSALEITEM->bSOLD = pNetMsg->bSOLD;
				pSALEITEM->dwNUMBER = pNetMsg->dwNUMBER;
				pSALEITEM->sITEMCUSTOM.wTurnNum = (WORD) pNetMsg->dwNUMBER;

				SINVENITEM *pINVENITEM = m_sPMarket.GetInven().GetItem(pNetMsg->sSALEPOS.wMainID,pNetMsg->sSALEPOS.wSubID);
				if ( pINVENITEM )
				{
					pINVENITEM->sItemCustom.wTurnNum = (WORD) pNetMsg->dwNUMBER;
				}
			}
		}
		break;

	case NET_MSG_GCTRL_PMARKET_CLOSE_BRD:
		{
			GLMSG::SNETPC_PMARKET_CLOSE_BRD *pNetNsg = (GLMSG::SNETPC_PMARKET_CLOSE_BRD *)nmg;

			m_sPMarket.DoMarketClose();
		}
		break;

	case NET_MSG_GCTRL_PMARKET_SEARCH_ITEM_RESULT:
		{
			GLMSG::SNETPC_PMARKET_SEARCH_ITEM_RESULT *pNetResultMsg = (GLMSG::SNETPC_PMARKET_SEARCH_ITEM_RESULT *)nmg;	
			CInnerInterface::GetInstance().GetItemSearchResultWindow()->UpdateSearchResult( pNetResultMsg->sSearchResult, pNetResultMsg->dwSearchNum, pNetResultMsg->dwPageNum );
			CInnerInterface::GetInstance().HideGroup ( ITEM_SHOP_SEARCH_WINDOW );
			CInnerInterface::GetInstance().ShowGroupFocus ( ITEM_SEARCH_RESULT_WINDOW );

		}
		break;

	case NET_MSG_GCTRL_PLAYERKILLING_ADD:
		{
			GLMSG::SNETPC_PLAYERKILLING_ADD *pNetMsg = (GLMSG::SNETPC_PLAYERKILLING_ADD *) nmg;

			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();	
			bool bBRIGHTEVENT    = GLGaeaClient::GetInstance().IsBRIGHTEVENT();
			bool bSCHOOLFREEPK   = GLGaeaClient::GetInstance().IsSchoolFreePk();
			bool bADD(false);

			/*pvp tyranny, Juver, 2017/08/24 */
			/*school wars, Juver, 2018/01/19 */
			/*pvp capture the flag, Juver, 2018/01/31 */
			if ( !( bBRIGHTEVENT||bSCHOOLFREEPK||pLand->m_bClubBattle||pLand->m_bClubDeathMatch||
				pLand->m_bPVPTyrannyMap || pLand->m_bPVPSchoolWarsMap || pLand->m_bPVPCaptureTheFlagMap ) )
			{
				bADD = ADD_PLAYHOSTILE ( pNetMsg->dwCharID, pNetMsg->bBAD );
			}

			// «–ø¯∞£ ¿⁄¿Ø««ƒ… ¡¯«‡¡ﬂ¿Ã∏Á ªÛ¥Î∞° ∞∞¿∫ «–±≥¿œ∞ÊøÏ ¿˚¥Î«‡¿ß ¥ÎªÛ¿⁄∑Œ ±∏∫–«œø© ∞¸∏Æ«‘.
			if ( bSCHOOLFREEPK && m_wSchool == pNetMsg->wSchoolID )
			{
				bADD = ADD_PLAYHOSTILE ( pNetMsg->dwCharID, pNetMsg->bBAD );
			}

			/*pvp capture the flag, Juver, 2018/01/31 */
			if ( bADD && !pNetMsg->bBAD && !pLand->m_bPVPCaptureTheFlagMap )
			{
				if ( pNetMsg->bClubBattle )
				{
					PGLCHARCLIENT pCLIENT = (PGLCHARCLIENT) GLGaeaClient::GetInstance().GetCopyActor( pNetMsg->szName );
					if ( pCLIENT ) 
					{
						if ( m_sCLUB.IsBattle ( pCLIENT->GETCLUBID() ) )
						{
							CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("CLUB_PLAYHOSTILE_ADD"), 
								pCLIENT->GetClubName(), pNetMsg->szName );
						}
						else if ( m_sCLUB.IsBattleAlliance ( pCLIENT->GETALLIANCEID() ) )
						{
							CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("ALLIANCE_PLAYHOSTILE_ADD"), 
								pCLIENT->GetClubName(), pNetMsg->szName );
						}
					}
				}
				else CInnerInterface::GetInstance().PrintConsoleTextDlg ( ID2GAMEINTEXT("PK_PLAYHOSTILE_ADD"), pNetMsg->szName );
			}
		}
		break;

	case NET_MSG_GCTRL_PLAYERKILLING_DEL:
		{
			GLMSG::SNETPC_PLAYERKILLING_DEL *pNetMsg = (GLMSG::SNETPC_PLAYERKILLING_DEL *) nmg;
			DEL_PLAYHOSTILE ( pNetMsg->dwCharID );
		}
		break;

	case NET_MSG_GCTRL_INVEN_HAIR_CHANGE_FB:
		{
			GLMSG::SNETPC_INVEN_HAIR_CHANGE_FB *pNetMsg = (GLMSG::SNETPC_INVEN_HAIR_CHANGE_FB *) nmg;
		
			switch ( pNetMsg->emFB )
			{
			case EMINVEN_HAIR_CHANGE_FB_FAIL:
				HairStyleChange( m_wHair );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_FAIL") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_OK:
				{
					m_wHair = (WORD) pNetMsg->dwID;
					UpdateSuit( TRUE );

					// «ÏæÓΩ∫≈∏¿œ ∫Ø∞Ê
					HairStyleChange( m_wHair );

					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_OK") );
				}
				break;
			case EMINVEN_HAIR_CHANGE_FB_NOITEM:
				HairStyleChange( m_wHair );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_BADITEM:
				HairStyleChange( m_wHair );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_BADITEM") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_BADCLASS:
				HairStyleChange( m_wHair );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_BADCLASS") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_COOLTIME:
				HairStyleChange( m_wHair );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_HAIRCOLOR_CHANGE_FB:
		{
			GLMSG::SNETPC_INVEN_HAIRCOLOR_CHANGE_FB *pNetMsg = (GLMSG::SNETPC_INVEN_HAIRCOLOR_CHANGE_FB*)nmg;

			switch ( pNetMsg->emFB )
			{
			case EMINVEN_HAIR_CHANGE_FB_FAIL:
				HairColorChange( m_wHairColor );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_FAIL") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_OK:
				{
					m_wHairColor = pNetMsg->wHairColor;
					UpdateSuit( TRUE );

					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_OK") );
				}
				break;
			case EMINVEN_HAIR_CHANGE_FB_NOITEM:
				HairColorChange( m_wHairColor );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_NOITEM") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_BADITEM:
				HairColorChange( m_wHairColor );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_BADITEM") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_BADCLASS:
				HairColorChange( m_wHairColor );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_HAIR_CHANGE_FB_BADCLASS") );
				break;
			case EMINVEN_HAIR_CHANGE_FB_COOLTIME:
				HairColorChange( m_wHairColor );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_FACE_CHANGE_FB:
		{
			GLMSG::SNETPC_INVEN_FACE_CHANGE_FB *pNetMsg = (GLMSG::SNETPC_INVEN_FACE_CHANGE_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMINVEN_FACE_CHANGE_FB_FAIL:
				FaceStyleChange( m_wFace );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_FAIL") );
				break;
			case EMINVEN_FACE_CHANGE_FB_OK:
				{
					m_wFace = (WORD) pNetMsg->dwID;
					UpdateSuit( TRUE );

					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_OK") );
				}
				break;
			case EMINVEN_FACE_CHANGE_FB_NOITEM:
				FaceStyleChange( m_wFace );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_NOITEM") );
				break;
			case EMINVEN_FACE_CHANGE_FB_BADITEM:
				FaceStyleChange( m_wFace );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_BADITEM") );
				break;
			case EMINVEN_FACE_CHANGE_FB_BADCLASS:
				FaceStyleChange( m_wFace );
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_FACE_CHANGE_FB_BADCLASS") );
				break;
			case EMINVEN_FACE_CHANGE_FB_COOLTIME:
				FaceStyleChange( m_wFace );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_GENDER_CHANGE_FB:
		{
			GLMSG::SNETPC_INVEN_GENDER_CHANGE_FB *pNetMsg = (GLMSG::SNETPC_INVEN_GENDER_CHANGE_FB *) nmg;
			
			switch ( pNetMsg->emFB )
			{
			case EMINVEN_GENDER_CHANGE_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_FAIL") );
				}
				break;
			case EMINVEN_GENDER_CHANGE_FB_OK:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_OK") );
					DoModal( ID2GAMEINTEXT("MODAL_GENDER_CHANGE_END"),  MODAL_INFOMATION, OK, MODAL_GENDER_CHANGE_END );	
				}
				break;
			case EMINVEN_GENDER_CHANGE_FB_NOITEM:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_NOITEM") );
				}
				break;
			case EMINVEN_GENDER_CHANGE_FB_ITEMTYPE:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_ITEMTYPE") );
				}
				break;
			case EMINVEN_GENDER_CHANGE_FB_BADCLASS:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_BADCLASS") );
				}
				break;
			case EMINVEN_GENDER_CHANGE_FB_NOTVALUE:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMINVEN_GENDER_CHANGE_FB_NOTVALUE") );
				}
			}
		}
		break;

	//CNDev | 2-7-2020 | e-point card system
	case NET_MSG_GCTRL_CARD_EPOINT_FB:
		{
			GLMSG::SNETPC_CARD_EPOINT_FB *pNetMsg = (GLMSG::SNETPC_CARD_EPOINT_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMINVEN_CARD_EPOINT_FB_NOITEMTYPE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_EPOINT_FB_NOITEMTYPE") );
				break;
			case EMINVEN_CARD_EPOINT_FB_NOITEMDATA:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_EPOINT_FB_NOITEMDATA") );
				break;
			case EMINVEN_CARD_EPOINT_FB_NOITEMINVEN:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_EPOINT_FB_NOITEMINVEN") );
				break;
			case EMINVEN_CARD_EPOINT_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_EPOINT_FB_FAIL") );
				break;
			case EMINVEN_CARD_EPOINT_FB_OK:
				//DxGlobalStage::GetInstance().GameToLobbyStage ();
				CInnerInterface::GetInstance().WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOLOBY );
				break;
			case EMINVEN_CARD_EPOINT_FB_CLOSE:
				{
					CInnerInterface::GetInstance().HideGroup ( INVENTORY_WINDOW ); //hide inven window to prevent player doing shit
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREEN_WOW, ID2GAMEINTEXT("EMINVEN_CARD_EPOINT_FB_CLOSE") );
				}
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_CARD_BRIGHT_FB:
		{
			GLMSG::SNETPC_CARD_BRIGHT_FB *pNetMsg = (GLMSG::SNETPC_CARD_BRIGHT_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMINVEN_CARD_BRIGHT_FB_NOITEMTYPE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_BRIGHT_FB_NOITEMTYPE") );
				break;
			case EMINVEN_CARD_BRIGHT_FB_NOITEMDATA:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_BRIGHT_FB_NOITEMDATA") );
				break;
			case EMINVEN_CARD_BRIGHT_FB_NOITEMINVEN:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_BRIGHT_FB_NOITEMINVEN") );
				break;
			case EMINVEN_CARD_BRIGHT_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_BRIGHT_FB_FAIL") );
				break;
			case EMINVEN_CARD_BRIGHT_FB_OK:
				//DxGlobalStage::GetInstance().GameToLobbyStage ();
				//CInnerInterface::GetInstance().WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOLOBY );
				break;
			case EMINVEN_CARD_BRIGHT_FB_CLOSE:
				{
					CInnerInterface::GetInstance().HideGroup ( INVENTORY_WINDOW );
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::LIGHTSKYBLUE, ID2GAMEINTEXT ("EMINVEN_CARD_BRIGHT_FB_CLOSE") );
				}
				break;
			};
		}
		break;

		

	case NET_MSG_GCTRL_CARD_CONTRIBUTION_FB:
		{
			GLMSG::SNETPC_CARD_CONTRIBUTION_FB *pNetMsg = (GLMSG::SNETPC_CARD_CONTRIBUTION_FB *) nmg;
			switch ( pNetMsg->emFB )
			{
			case EMINVEN_CARD_CONTRIBUTION_FB_NOITEMTYPE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_CONTRIBUTION_FB_NOITEMTYPE") );
				break;
			case EMINVEN_CARD_CONTRIBUTION_FB_NOITEMDATA:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_CONTRIBUTION_FB_NOITEMDATA") );
				break;
			case EMINVEN_CARD_CONTRIBUTION_FB_NOITEMINVEN:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_CONTRIBUTION_FB_NOITEMINVEN") );
				break;
			case EMINVEN_CARD_CONTRIBUTION_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_CARD_CONTRIBUTION_FB_FAIL") );
				break;
			case EMINVEN_CARD_CONTRIBUTION_FB_OK:
				break;
			case EMINVEN_CARD_CONTRIBUTION_FB_CLOSE:
				{
					CInnerInterface::GetInstance().HideGroup ( INVENTORY_WINDOW );
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::LIGHTSKYBLUE, ID2GAMEINTEXT ("EMINVEN_CARD_CONTRIBUTION_FB_CLOSE") );
				}
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_INVEN_RENAME_FB:
		{
			GLMSG::SNETPC_INVEN_RENAME_FB *pNetMsg = (GLMSG::SNETPC_INVEN_RENAME_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMINVEN_RENAME_FB_OK:
				{
					StringCchCopy ( m_szName, CHAR_SZNAME, pNetMsg->szName );

					// ¿Ã∏ß«• ∫Ø∞Ê
					CNameDisplayMan *pDISP_NAME_MAN = CInnerInterface::GetInstance().GetDispName();
					if ( pDISP_NAME_MAN )
					{
						CROWREN sDISP_NAME;
						sDISP_NAME.INIT ( this );
						sDISP_NAME.SETTYPEFLAG ( this );
						pDISP_NAME_MAN->ADD_DISP_NAME ( sDISP_NAME );
					}
					
					if ( m_sCLUB.IsMember ( m_dwCharID ) )
					{
						GLCLUBMEMBER* pCMember = m_sCLUB.GetMember( m_dwCharID );
						if ( pCMember )
						{
							StringCchCopy ( pCMember->m_szName, CHAR_SZNAME, pNetMsg->szName );
						}
					}

					if ( m_sCLUB.IsMaster ( m_dwCharID ) )
					{
						StringCchCopy ( m_sCLUB.m_szMasterName, CHAR_SZNAME, pNetMsg->szName );
					}

					GLPARTY_CLIENT* pPMember = GLPartyClient::GetInstance().FindMember ( m_dwGaeaID );
					if ( pPMember )
					{
						StringCchCopy ( pPMember->m_szName, CHAR_SZNAME, pNetMsg->szName );
					}

					//	Note : ¿Œ≈Õ∆‰¿ÃΩ∫ø° ∫Ø∞Ê æÀ∏≤.
					CInnerInterface::GetInstance().REFRESH_FRIEND_LIST ();
					CInnerInterface::GetInstance().REFRESH_CLUB_LIST ();

					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMINVEN_RENAME_FB_OK"), pNetMsg->szName );
				}
				break;
			case EMINVEN_RENAME_FB_LENGTH:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEEXTEXT("NEWCHAR_NAME_TOO_SHORT") );	
				break;
			case EMINVEN_RENAME_FROM_DB_BAD:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEEXTEXT("CHARACTER_BADNAME") );
				break;
			case EMINVEN_RENAME_FROM_DB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FROM_DB_FAIL"), pNetMsg->szName );
				break;
			case EMINVEN_RENAME_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_FAIL") );
				break;
			case EMINVEN_RENAME_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_NOITEM") );
				break;
			case EMINVEN_RENAME_FB_BADITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_BADITEM") );
				break;
			case EMINVEN_RENAME_FB_THAICHAR_ERROR:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_THAICHAR_ERROR") );
				break;
			case EMINVEN_RENAME_FB_VNCHAR_ERROR:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_VNCHAR_ERROR") );
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_QITEMFACT_BRD:
		{
			GLMSG::SNETPC_QITEMFACT_BRD *pNetMsg = (GLMSG::SNETPC_QITEMFACT_BRD *)nmg;
			
 			if ( pNetMsg->sFACT.emType == QUESTION_NONE )	break;

			// ≈ª∞Õ ≈æΩ¬Ω√ ƒ˘º«æ∆¿Ã≈€ πÃ ¿˚øÎ
			if ( m_bVehicle )	break;

			DxSoundLib::GetInstance()->PlaySound ( "QITEM_FACT" );
			CInnerInterface::GetInstance().SET_QUESTION_ITEM_ID ( pNetMsg->sFACT.emType );

			switch ( pNetMsg->sFACT.emType )
			{
			case QUESTION_SPEED_UP:
			case QUESTION_CRAZY:
			case QUESTION_ATTACK_UP:
			case QUESTION_EXP_UP:
			case QUESTION_LUCKY:
			case QUESTION_SPEED_UP_M:
			case QUESTION_MADNESS:
			case QUESTION_ATTACK_UP_M:
				m_sQITEMFACT.emType = pNetMsg->sFACT.emType;
				m_sQITEMFACT.fTime = pNetMsg->sFACT.fTime;
				m_sQITEMFACT.wParam1 = pNetMsg->sFACT.wParam1;
				m_sQITEMFACT.wParam2 = pNetMsg->sFACT.wParam2;
	
				CInnerInterface::GetInstance().SET_KEEP_QUESTION_ITEM_ID ( pNetMsg->sFACT.emType );
				break;

			case QUESTION_EXP_GET:
				{
					//	Note : ¿⁄±‚ ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
					STARGETID vTARID(CROW_PC,m_dwGaeaID,m_vPos);
					DxEffGroupPlayer::GetInstance().NewEffGroup ( "QI_expget.egp", m_matTrans, &vTARID );
				}
				break;

			case QUESTION_BOMB:
				{
					//	Note : ¿⁄±‚ ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
					STARGETID vTARID(CROW_PC,m_dwGaeaID,m_vPos);
					DxEffGroupPlayer::GetInstance().NewEffGroup ( "QI_bomb.egp", m_matTrans, &vTARID );
				}
				break;

			case QUESTION_MOBGEN:
				break;

			case QUESTION_HEAL:
				{
					//	Note : ¿⁄±‚ ¿ßƒ° ¿Ã∆Â∆Æ πﬂª˝Ω√≈¥.
					STARGETID vTARID(CROW_PC,m_dwGaeaID,m_vPos);
					DxEffGroupPlayer::GetInstance().NewEffGroup ( "QI_heal.egp", m_matTrans, &vTARID );
				}
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_QITEMFACT_END_BRD:
		{
			GLMSG::SNETPC_QITEMFACT_END_BRD *pNetMsg = (GLMSG::SNETPC_QITEMFACT_END_BRD *)nmg;

			m_sQITEMFACT.RESET ();
			CInnerInterface::GetInstance().RESET_KEEP_QUESTION_ITEM ();
		}
		break;

	case NET_MSG_GCTRL_PKCOMBO_BRD:
		{
			GLMSG::SNETPC_PKCOMBO_BRD *pNetMsg = (GLMSG::SNETPC_PKCOMBO_BRD *)nmg;

			if ( pNetMsg->sCOMBO.nCount == 0 )	break;

			if ( m_bVehicle )	break;

			m_sPKCOMBOCOUNT.bShow = pNetMsg->sCOMBO.bShow;
			m_sPKCOMBOCOUNT.fTime = pNetMsg->sCOMBO.fTime;
			m_sPKCOMBOCOUNT.nCount = pNetMsg->sCOMBO.nCount;

			if ( pNetMsg->sCOMBO.nCount == 2 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_DOUBLE" );
			else if ( pNetMsg->sCOMBO.nCount == 3 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_TRIPLE" );
			else if ( pNetMsg->sCOMBO.nCount == 4 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_ULTRA" );
			else if ( pNetMsg->sCOMBO.nCount == 5 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_RAMPAGE" );
			else if ( pNetMsg->sCOMBO.nCount == 6 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_KILLING_SPREE" );
			else if ( pNetMsg->sCOMBO.nCount == 7 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_DOMINATING" );
			else if ( pNetMsg->sCOMBO.nCount == 8 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_MEGA_KILL" );
			else if ( pNetMsg->sCOMBO.nCount == 9 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_UNSTOPPABLE" );
			else if ( pNetMsg->sCOMBO.nCount == 10 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_WICKED_SICK" );
			else if ( pNetMsg->sCOMBO.nCount == 11 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_MONSTER_KILL" );
			else if ( pNetMsg->sCOMBO.nCount == 12 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_GODLIKE" );
			else if ( pNetMsg->sCOMBO.nCount == 13 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_HOLY_SHIT" );
			else if ( pNetMsg->sCOMBO.nCount >= 14 )	DxSoundLib::GetInstance()->PlaySound ( "PKCOMBO_OWNAGE" );


			CInnerInterface::GetInstance().SET_PK_COMBO(pNetMsg->sCOMBO.nCount);
		}
		break;

	case NET_MSG_GCTRL_PKCOMBO_END_BRD:
		{
			GLMSG::SNETPC_PKCOMBO_END_BRD *pNetMsg = (GLMSG::SNETPC_PKCOMBO_END_BRD *)nmg;

			m_sPKCOMBOCOUNT.RESET();

			CInnerInterface::GetInstance().RESET_PK_COMBO();
			DxEffGroupPlayer::GetInstance().DeletePassiveEffect ( "supersayan.egp", STARGETID(CROW_PC,m_dwGaeaID,m_vPos) );
		}
		break;

	case NET_MSG_GCTRL_EVENTFACT_BRD:
		{
			GLMSG::SNETPC_EVENTFACT_BRD *pNetMsg = (GLMSG::SNETPC_EVENTFACT_BRD *)nmg;

			switch ( pNetMsg->emType )
			{
			case EMGM_EVENT_SPEED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMEVENTFACT_SPEED"), pNetMsg->wValue );
				break;

			case EMGM_EVENT_ASPEED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMEVENTFACT_ASPEED"), pNetMsg->wValue );
				break;

			case EMGM_EVENT_ATTACK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMEVENTFACT_ATTACK"), pNetMsg->wValue );
				break;
			};

			m_sEVENTFACT.SetEVENT( pNetMsg->emType, pNetMsg->wValue );
		}
		break;

	case NET_MSG_GCTRL_EVENTFACT_END_BRD:
		{
			GLMSG::SNETPC_EVENTFACT_END_BRD *pNetMsg = (GLMSG::SNETPC_EVENTFACT_END_BRD *)nmg;

			switch( pNetMsg->emType )
			{
			case EMGM_EVENT_SPEED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMEVENTFACT_SPEED_END") );
				break;

			case EMGM_EVENT_ASPEED:	
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMEVENTFACT_ASPEED_END") );
				break;

			case EMGM_EVENT_ATTACK:	
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMEVENTFACT_ATTACK_END") );
				break;
			}

			m_sEVENTFACT.ResetEVENT( pNetMsg->emType );
		}
		break;

	case NET_MSG_GCTRL_EVENTFACT_INFO:
		{
			GLMSG::SNETPC_EVENTFACT_INFO *pNetMsg = (GLMSG::SNETPC_EVENTFACT_INFO*)nmg;

			m_sEVENTFACT = pNetMsg->sEVENTFACT;
		}
		break;

	case NET_MSG_GCTRL_2_FRIEND_FB:
		{
			GLMSG::SNETPC_2_FRIEND_FB *pNetMsg = (GLMSG::SNETPC_2_FRIEND_FB *)nmg;
			switch ( pNetMsg->emFB )
			{
			case EM2FRIEND_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_FAIL") );
				break;

			case EM2FRIEND_FB_OK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EM2FRIEND_FB_OK") );
				break;

			case EM2FRIEND_FB_NO_ITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_NO_ITEM") );
				break;

			case EM2FRIEND_FB_FRIEND_CONDITION:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_FRIEND_CONDITION") );
				break;

			case EM2FRIEND_FB_MY_CONDITION:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_MY_CONDITION") );
				break;

			case EM2FRIEND_FB_PK_CONDITION:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_PK_CONDITION") );
				break;

			case EM2FRIEND_FB_MAP_CONDITION:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_MAP_CONDITION") );
				break;

			case EM2FRIEND_FB_FRIEND_CHANNEL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_FRIEND_CHANNEL") );
				break;

			case EM2FRIEND_FB_IMMOVABLE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_IMMOVABLE") );
				break;

			case EM2FRIEND_FB_FRIEND_BLOCK:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_FRIEND_BLOCK") );
				break;

			case EM2FRIEND_FB_FRIEND_OFF:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EM2FRIEND_FB_FRIEND_OFF") );
				break;
			};
		}
		break;

	/*dmk14 reform*/
	case NET_MSG_REBUILD_RESULT:	// ITEMREBUILD_MARK
		{
			GLMSG::SNET_REBUILD_RESULT* pNetMsg = (GLMSG::SNET_REBUILD_RESULT*)nmg;
			switch( pNetMsg->emResult )
			{
			case EMREBUILD_RESULT_MONEY:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ±›æ◊¿Ã ∏¬¡ˆ æ æ∆º≠ ø¿∑˘ ¿¸º€
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREBUILD_RESULT_MONEY" ) );
				break;

			case EMREBUILD_RESULT_DESTROY:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ Ω«∆–, æ∆¿Ã≈€ º“∏Í
				InitRebuildData();
				CInnerInterface::GetInstance().HideGroup( ITEM_REBUILD_WINDOW );
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREBUILD_RESULT_DESTROY" ) );
				break;

			case EMREBUILD_RESULT_SUCCESS:	
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "ITEM_REBUILD_SUCCESS" ) );
					/*dmk14 reform*/
					m_sItemPrevCopy = pNetMsg->sItemCopy;
				}
				break;

			/*dmk14 reform*/
			case EMREBUILD_RESULT_PREVIOUS:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "ITEM_REBUILD_SUCCESS" ) );
					m_sItemPrevCopy = SITEMCUSTOM();
					CItemRebuild* cItemRebuild = CInnerInterface::GetInstance().GetItemRebuild();
					if( cItemRebuild )	cItemRebuild->ResetPrevDATA();
				}
				break;
			case EMREBUILD_RESULT_SUCCESS_WITH_STAMPCARD:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREBUILD_RESULT_SUCCESS_WITH_STAMPCARD" ) );
					/*dmk14 reform*/
					m_sItemPrevCopy = pNetMsg->sItemCopy;
				}
				break;
			}
		}
		break;

	case NET_MSG_GCTRL_GARBAGE_RESULT_FB:
		{
			GLMSG::SNET_GARBAGE_RESULT_FB* pNetMsg = (GLMSG::SNET_GARBAGE_RESULT_FB*)nmg;

			switch( pNetMsg->emResult )
			{
			case EMGARBAGE_RESULT_FB_FAIL:
				InitGarbageData();
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMGARBAGE_RESULT_FB_FAIL" ) );
				break;

			case EMGARBAGE_RESULT_FB_OK:
				CInnerInterface::GetInstance().CloseItemGarbageWindow();
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMGARBAGE_RESULT_FB_OK" ) );
				break;

			case EMGARBAGE_RESULT_FB_NOITEM:
				CInnerInterface::GetInstance().CloseItemGarbageWindow();				
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMGARBAGE_RESULT_FB_NOITEM" ) );
				break;

			case EMGARBAGE_RESULT_FB_ITEMTYPE:
				InitGarbageData();
				CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMGARBAGE_RESULT_FB_ITEMTYPE" ) );
				break;
			}
		}

	case NET_MSG_REBUILD_MOVE_ITEM:
		{
			GLMSG::SNET_REBUILD_MOVE_ITEM* pNetMsg = (GLMSG::SNET_REBUILD_MOVE_ITEM*)nmg;
			m_sRebuildItem.SET( pNetMsg->wPosX, pNetMsg->wPosY );

			/*dmk14 reform*/
			m_sItemPrevCopy = SITEMCUSTOM();
		}
		break;

	/*dmk14 reform*/
	case NET_MSG_REBUILD_MOVE_ITEMSEALED:
		{
			GLMSG::SNET_REBUILD_MOVE_ITEMSEALED* pNetMsg = (GLMSG::SNET_REBUILD_MOVE_ITEMSEALED*)nmg;
			m_sRebuildItemSealed = pNetMsg->sItemSeal;
		}
		break;

	case NET_MSG_REBUILD_COST_MONEY:
		{
			GLMSG::SNET_REBUILD_COST_MONEY* pNetMsg = (GLMSG::SNET_REBUILD_COST_MONEY*)nmg;
			m_i64RebuildCost = pNetMsg->i64CostMoney;

			if( m_lnMoney < m_i64RebuildCost )
			{
				//InitRebuildData();
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "EMREBUILD_RESULT_MONEY" ) );
				break;
			}

			ReqRebuildInputMoney( m_i64RebuildCost );
		}
		break;

	case NET_MSG_REBUILD_INPUT_MONEY:
		{
			GLMSG::SNET_REBUILD_INPUT_MONEY* pNetMsg = (GLMSG::SNET_REBUILD_INPUT_MONEY*)nmg;
			m_i64RebuildInput = pNetMsg->i64InputMoney;
		}
		break;
		//∏À≥∆§…Ø≈
	case NET_MSG_ZBSJ_RESULT:
	{
		GLMSG::SNET_ZBSJ_RESULT* pNetMsg = (GLMSG::SNET_ZBSJ_RESULT*)nmg;
		switch (pNetMsg->emResult)
		{
		case EMREBUILD_ZBSJ_DAMAGE:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			InitZbsjData();
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::AQUA, ID2GAMEINTEXT("EMZBSJ_RESULT_DAMAGE"));
			break;
		case EMREBUILD_ZBSJ_DEFENSE:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			InitZbsjData();
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::AQUA, ID2GAMEINTEXT("EMZBSJ_RESULT_DEFENSE"));
			break;
		case EMREBUILD_ZBSJ_TYPE1:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			InitZbsjData();
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::AQUA, ID2GAMEINTEXT("EMZBSJ_RESULT_TYPE1"));
			break;
		case EMREBUILD_ZBSJ_TYPE2:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			InitZbsjData();
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::AQUA, ID2GAMEINTEXT("EMZBSJ_RESULT_TYPE2"));
			break;
		case EMREBUILD_ZBSJ_TYPE3:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			InitZbsjData();
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::AQUA, ID2GAMEINTEXT("EMZBSJ_RESULT_TYPE3"));
			break;
		case EMREBUILD_ZBSJ_TYPE4:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			InitZbsjData();
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::AQUA, ID2GAMEINTEXT("EMZBSJ_RESULT_TYPE4"));
			break;
		case EMREBUILD_ZBSJ_SUCCESS:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_NOTYPE"));
			break;
		case EMREBUILD_ZBSJ_MONEY:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_MONEY"));
			break;
		case EMREBUILD_ZBSJ_MAX:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_MAX"));
			break;
		case EMREBUILD_ZBSJ_NO:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_NO"));
			break;
		case EMREBUILD_ZBSJ_BO:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_LOCK"));
			break;
		case EMREBUILD_ZBSJ_REBA:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_REBA"));
			break;
		case EMREBUILD_ZBSJ_ITEM:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_ITEM"));
			break;
		case EMREBUILD_ZBSJ_ITEMRATE:	// »Æ¿Œ πˆ∆∞ ¥≠∑∂∞Ì, ∞≥¡∂ º∫∞¯
			CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMZBSJ_RESULT_ITEMRATE"));
			break;
		}
	}
	break;
	case NET_MSG_GCTRL_UPDATE_LASTCALL:
		{
			GLMSG::SNETPC_UPDATE_LASTCALL *pNetMsg = (GLMSG::SNETPC_UPDATE_LASTCALL *)nmg;

			// Note : ¡˜¿¸±Õ»Øƒ´µÂø°º≠ ªÁøÎµ… ¡§∫∏∏¶ πﬁΩ¿¥œ¥Ÿ.
			m_sLastCallMapID = pNetMsg->sLastCallMapID;
			m_vLastCallPos = pNetMsg->vLastCallPos;
		}
		break;

	case NET_MSG_GCTRL_UPDATE_STARTCALL:
		{
			GLMSG::SNETPC_UPDATE_STARTCALL *pNetMsg = (GLMSG::SNETPC_UPDATE_STARTCALL *)nmg;
			
			// Ω√¿€¿ßƒ° ∫Ø∞Ê
			m_sStartMapID = pNetMsg->sStartMapID;
			m_dwStartGate = pNetMsg->dwStartGateID;
			m_vSavePos	  = pNetMsg->vStartPos;
		}
		break;

	case NET_MSG_SMS_SEND_FB:
		{
			GLMSG::SNETPC_SEND_SMS_FB *pNetMsg = (GLMSG::SNETPC_SEND_SMS_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMSMS_SEND_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSMS_SEND_FB_FAIL") );
				break;
			case EMSMS_SEND_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMSMS_SEND_FB_OK") );
				break;
			case EMSMS_SEND_FROM_DB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSMS_SEND_FROM_DB_FAIL") );
				break;
			case EMSMS_SEND_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSMS_SEND_FB_NOITEM") );
				break;
			case EMSMS_SEND_FB_BADITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSMS_SEND_FB_BADITEM") );
				break;
			};
		}
		break;

	case NET_MSG_SMS_PHONE_NUMBER_FB:
		{
			GLMSG::SNETPC_PHONE_NUMBER_FB *pNetMsg = (GLMSG::SNETPC_PHONE_NUMBER_FB *) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMSMS_PHONE_NUMBER_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMINVEN_RENAME_FB_FAIL") );
				break;
			case EMSMS_PHONE_NUMBER_FB_OK:
				{
					StringCchCopy ( m_szPhoneNumber, SMS_RECEIVER, pNetMsg->szPhoneNumber );

					// ƒ≥∏Ø≈Õ ¡§∫∏√¢ø° ∆˘ π¯»£ ∫Ø∞Ê
					//CInnerInterface::GetInstance().GetCharacterWindow()->SetPhoneNumber( CString( pNetMsg->szPhoneNumber ) );

					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMSMS_PHONE_NUMBER_FB_OK"), pNetMsg->szPhoneNumber );
				}
				break;
			case EMSMS_PHONE_NUMBER_FROM_DB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMSMS_PHONE_NUMBER_FROM_DB_FAIL"), pNetMsg->szPhoneNumber );
				break;
			};
		}
		break;

#if defined(_RELEASED) || defined(KRT_PARAM)
	case NET_MSG_GM_SHOWMETHEMONEY:
		{
			GLMSG::SNET_GM_SHOWMETHEMONEY* pNetMsg = (GLMSG::SNET_GM_SHOWMETHEMONEY*)nmg;
			m_lnMoney = pNetMsg->llMoney;
		}
		break;
#endif

	case NET_MSG_GCTRL_TRADE_FB:
	case NET_MSG_GCTRL_TRADE_AGREE_TAR:
	case NET_MSG_GCTRL_TRADE_LOCK_TAR:  /*trade lock, Juver, 2018/01/02 */
	case NET_MSG_GCTRL_TRADE_ITEM_REGIST_TAR:
	case NET_MSG_GCTRL_TRADE_ITEM_REMOVE_TAR:
	case NET_MSG_GCTRL_TRADE_MONEY_TAR:
	case NET_MSG_GCTRL_TRADE_COMPLETE_TAR:
	case NET_MSG_GCTRL_TRADE_CANCEL_TAR:
		{
			GLTradeClient::GetInstance().MsgProcess ( nmg );
		}
		break;

	case NET_MSG_GCTRL_CONFRONT_FB:
	case NET_MSG_GCTRL_CONFRONT_START2_CLT:
	case NET_MSG_GCTRL_CONFRONT_FIGHT2_CLT:
	case NET_MSG_GCTRL_CONFRONT_END2_CLT:
	case NET_MSG_GCTRL_CONFRONTPTY_START2_CLT:
	case NET_MSG_GCTRL_CONFRONTPTY_END2_CLT:
	case NET_MSG_GCTRL_CONFRONT_RECOVE:
	case NET_MSG_GCTRL_CONFRONT_END2_CLT_MBR:
	case NET_MSG_GCTRL_CONFRONTCLB_START2_CLT:
		{
			MsgProcessConfront( nmg );
		}
		break;

	case NET_MSG_GCTRL_REQ_QUEST_START_FB:
	case NET_MSG_GCTRL_QUEST_PROG_STREAM:
	case NET_MSG_GCTRL_QUEST_PROG_STEP_STREAM:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN:
	case NET_MSG_GCTRL_QUEST_PROG_DEL:
	case NET_MSG_GCTRL_QUEST_END_STREAM:
	case NET_MSG_GCTRL_QUEST_END_DEL:
	case NET_MSG_GCTRL_QUEST_PROG_NPCTALK_FB:
	case NET_MSG_GCTRL_QUEST_PROG_MOBKILL:
	case NET_MSG_GCTRL_QUEST_PARTY_PROG_MOBKILL:
	case NET_MSG_GCTRL_QUEST_PROG_QITEM:
	case NET_MSG_GCTRL_QUEST_PARTY_PROG_QITEM:
	case NET_MSG_GCTRL_QUEST_PROG_REACHZONE:
	case NET_MSG_GCTRL_QUEST_PROG_TIMEOVER:
	case NET_MSG_GCTRL_QUEST_PROG_TIME:
	case NET_MSG_GCTRL_QUEST_PROG_NONDIE:
	case NET_MSG_GCTRL_QUEST_PROG_LEAVEMAP:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN_INSERT:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN_DELETE:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN_TURN:
	case NET_MSG_GCTRL_QUEST_PROG_INVEN_PICKUP:
	case NET_MSG_GCTRL_QUEST_COMPLETE_FB:
		{
			MsgProcessQuest( nmg );
		}
		break;

	case NET_MSG_GCTRL_CLUB_INFO_2CLT:
	case NET_MSG_GCTRL_CLUB_DEL_2CLT:
	case NET_MSG_GCTRL_CLUB_INFO_DISSOLUTION:
	case NET_MSG_GCTRL_CLUB_MEMBER_2CLT:
	case NET_MSG_GCTRL_CLUB_NEW_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_MEMBER_REQ_FB:
	case NET_MSG_GCTRL_CLUB_MARK_CHANGE_2CLT:
	case NET_MSG_GCTRL_CLUB_RANK_2CLT:
	case NET_MSG_GCTRL_CLUB_RANK_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_NICK_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_STATE:
	case NET_MSG_GCTRL_CLUB_MEMBER_POS:
	case NET_MSG_GCTRL_CLUB_DISSOLUTION_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_DEL_2CLT:
	case NET_MSG_GCTRL_CLUB_MEMBER_DEL_FB:
	case NET_MSG_GCTRL_CLUB_MEMBER_SECEDE_FB:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_REQ_FB:
	case NET_MSG_GCTRL_CLUB_AUTHORITY_CLT:

	case NET_MSG_GCTRL_CLUB_CD_CERTIFY_FB:
	case NET_MSG_GCTRL_CLUB_COMMISSION_FB:
	case NET_MSG_GCTRL_CLUB_STORAGE_RESET:
	case NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_MONEY:
	case NET_MSG_GCTRL_CLUB_GETSTORAGE_ITEM:
	case NET_MSG_GCTRL_CLUB_STORAGE_INSERT:
	case NET_MSG_GCTRL_CLUB_STORAGE_DELETE:
	case NET_MSG_GCTRL_CLUB_STORAGE_DEL_INS:
	case NET_MSG_GCTRL_CLUB_STORAGE_UPDATE_ITEM:
	case NET_MSG_GCTRL_CLUB_NOTICE_FB:
	case NET_MSG_GCTRL_CLUB_NOTICE_CLT:
	case NET_MSG_GCTRL_CLUB_MBR_RENAME_CLT:
	case NET_MSG_GCTRL_CLUB_SUBMASTER_FB:
	case NET_MSG_GCTRL_CLUB_SUBMASTER_BRD:
		{
			MsgProcessClub( nmg );
		}
		break;

	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_FB:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_FB:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_SEC_FB:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_FB:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_ADD_CLT:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DEL_CLT:
	case NET_MSG_GCTRL_CLUB_ALLIANCE_DIS_CLT:
		{
			MsgProcessAlliance( nmg );
		}
		break;

	case NET_MSG_GCTRL_CLUB_BATTLE_REQ_FB:
	case NET_MSG_GCTRL_CLUB_BATTLE_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT:
	case NET_MSG_GCTRL_CLUB_BATTLE_BEGIN_CLT2:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_FB:
	case NET_MSG_GCTRL_CLUB_BATTLE_ARMISTICE_REQ_ASK:
	case NET_MSG_GCTRL_CLUB_BATTLE_OVER_CLT:
	case NET_MSG_GCTRL_CLUB_BATTLE_SUBMISSION_REQ_FB:
	case NET_MSG_GCTRL_CLUB_BATTLE_KILL_UPDATE:
	case NET_MSG_GCTRL_CLUB_BATTLE_POINT_UPDATE:
		{
			MsgProcessClubBattle( nmg );
		}
		break;

	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_FB:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_REQ_ASK:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_FB:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_ARMISTICE_REQ_ASK:
	case NET_MSG_GCTRL_ALLIANCE_BATTLE_SUBMISSION_REQ_FB:
		{
			MsgProcessAllianceBattle( nmg );
		}
		break;

	case NET_MSG_MGAME_ODDEVEN_FB:
		{
			MsgProcessMiniGame( nmg );
		}
		break;

#ifdef CH_PARAM_USEGAIN //chinatest%%%
	case NET_MSG_CHINA_GAINTYPE:
		{
			GLMSG::SNETPC_CHINA_GAINTYPE *pNetMsg = (GLMSG::SNETPC_CHINA_GAINTYPE *) nmg;
			if( pNetMsg->dwGainType == 1 )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("CHINA_GAIN_50PERCENT") );
			}else if( pNetMsg->dwGainType == 2 )
			{
//				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("CHINA_GAIN_0PERCENT") );				
//				CInnerInterface::GetInstance().WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_CLOSEGAME );
//				CInnerInterface::GetInstance().WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("CHINA_GAIN_CLOSEGAME"), WAITSERVER_CLOSEGAME, 30 );
				DoModal( ID2GAMEINTEXT("CHINA_GAIN_CLOSEGAME"), MODAL_INFOMATION, OK, MODAL_CLOSEGAME );
			}/*else if( pNetMsg->dwGainType == 3 )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("CHINA_GAIN_15MINUTE") );				
			}*/else if( pNetMsg->dwGainType == 4 )
			{
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("CHINA_GAIN_1HOUR") );				
			}
		}
		break;
#endif

#if defined(VN_PARAM) //vietnamtest%%%
	case NET_MSG_VIETNAM_GAINTYPE:
		{
			GLMSG::SNETPC_VIETNAM_GAINTYPE *pNetMsg = (GLMSG::SNETPC_VIETNAM_GAINTYPE *) nmg;
			if( pNetMsg->dwGainType == GAINTYPE_HALF )
			{
				m_dwVietnamGainType = GAINTYPE_HALF;
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("VIETNAM_GAINTYPE_HALF") );
			}else if( pNetMsg->dwGainType == GAINTYPE_EMPTY )
			{
				m_dwVietnamGainType = GAINTYPE_EMPTY;
				CInnerInterface::GetInstance().PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("VIETNAM_GAINTYPE_EMPTY") );
			}
		}
		break;
#endif

	case NET_MSG_VIETNAM_GAIN_MONEY:
		{
			GLMSG::SNETPC_VIETNAM_GAIN_MONEY *pNetMsg = (GLMSG::SNETPC_VIETNAM_GAIN_MONEY *) nmg;
			m_lVNGainSysMoney = pNetMsg->gainMoney;
		}
		break;

	case NET_MSG_VIETNAM_GAIN_EXP:
		{
			GLMSG::SNETPC_VIETNAM_GAIN_EXP *pNetMsg = (GLMSG::SNETPC_VIETNAM_GAIN_EXP *) nmg;
			m_lVNGainSysExp = pNetMsg->gainExp;
		}
		break;

#if defined(VN_PARAM) //vietnamtest%%%
	case NET_MSG_VIETNAM_TIME_REQ_FB:
		{
			GLMSG::SNETPC_VIETNAM_TIME_REQ_FB *pNetMsg = (GLMSG::SNETPC_VIETNAM_TIME_REQ_FB *)nmg;
			m_sVietnamSystem.loginTime = pNetMsg->loginTime;
			m_sVietnamSystem.gameTime  = pNetMsg->gameTime;
			m_VietnamGameTime  = pNetMsg->gameTime;

		}
		break;
#endif



#ifndef CH_PARAM_USEGAIN //** Add EventTime
	case NET_MSG_GM_LIMIT_EVENT_APPLY_START:
		{
			GLMSG::SNET_GM_LIMIT_EVENT_APPLY_START *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_APPLY_START *) nmg;
			m_bEventBuster    = TRUE;
			m_tLoginTime      = pNetMsg->loginTime;
			m_RemainEventTime = 0;
			m_RemainBusterTime = m_EventBusterTime;

			//dmk14 btg info
			m_fBtgExpRate	= pNetMsg->expGain_Rate;
			m_fBtgItemRate	= pNetMsg->itemGain_Rate;

			CInnerInterface::GetInstance().BONUS_TIME_BUSTER_START();
			
		}
		break;

	case NET_MSG_GM_LIMIT_EVENT_APPLY_END:
		{
			GLMSG::SNET_GM_LIMIT_EVENT_APPLY_END *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_APPLY_END *) nmg;
			m_bEventBuster    = FALSE;
			m_tLoginTime      = pNetMsg->loginTime;
			GLGaeaClient::GetInstance().SetCurrentTime( m_tLoginTime );
			m_RemainEventTime  = m_EventStartTime;
			m_RemainBusterTime = m_EventBusterTime;

			//dmk14 btg info
			m_fBtgExpRate	= 0;
			m_fBtgItemRate	= 0;

			CInnerInterface::GetInstance().BONUS_TIME_BUSTER_END();

		}
		break;

	case NET_MSG_GM_LIMIT_EVENT_BEGIN_FB:
		{
			GLMSG::SNET_GM_LIMIT_EVENT_BEGIN_FB *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_BEGIN_FB *) nmg;
			/*CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, 
														  "%d ∑π∫ß ∫Œ≈Õ %d ∑π∫ß ±Ó¡ˆ %d∫– «√∑π¿Ã %d ∫ŒΩ∫≈Õ %.2f ∞Ê«Ëƒ° %.2f µÂ∂¯¿≤",
														  pNetMsg->start_Lv, pNetMsg->end_Lv, pNetMsg->play_Time, pNetMsg->buster_Time,
														  pNetMsg->expGain_Rate, pNetMsg->itemGain_Rate );*/

			m_bEventStart = TRUE;
			m_bEventBuster = FALSE;

			m_EventStartLv = pNetMsg->start_Lv;
			m_EventEndLv   = pNetMsg->end_Lv;

			m_EventBusterTime = pNetMsg->buster_Time;
			m_EventStartTime  = pNetMsg->play_Time;

			m_RemainEventTime  = m_EventStartTime;
			m_RemainBusterTime = m_EventBusterTime;

			//dmk14 btg info
			m_fBtgExpRate	= pNetMsg->expGain_Rate;
			m_fBtgItemRate	= pNetMsg->itemGain_Rate;

			if( m_wLevel >= m_EventStartLv && m_wLevel <= m_EventEndLv )
			{
				CTime crtTime = GLGaeaClient::GetInstance().GetCurrentTime();
				m_tLoginTime  = crtTime.GetTime();
				m_bEventApply = TRUE;
				CInnerInterface::GetInstance().BONUS_TIME_EVENT_START( !m_bEventBuster );
			}else{
				m_bEventApply = FALSE;
				CInnerInterface::GetInstance().BONUS_TIME_EVENT_END();
			}
			
			
		}
		break;

	case NET_MSG_GM_LIMIT_EVENT_END_FB:
		{
			//CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, "¿Ã∫•∆Æ ¡æ∑·!!" );

			m_bEventApply  = FALSE;
			m_bEventStart  = FALSE;
			m_bEventBuster = FALSE;
			CInnerInterface::GetInstance().BONUS_TIME_EVENT_END();
		}
		break;

	case NET_MSG_GM_LIMIT_EVENT_TIME_REQ_FB:
		{
			GLMSG::SNET_GM_LIMIT_EVENT_TIME_REQ_FB *pNetMsg = (GLMSG::SNET_GM_LIMIT_EVENT_TIME_REQ_FB *)nmg;

			m_bEventStart  = pNetMsg->bEventStart;
			if( m_bEventStart )
			{
				m_tLoginTime   = pNetMsg->loginTime;
				GLGaeaClient::GetInstance().SetCurrentTime( m_tLoginTime );
				m_EventStartLv = pNetMsg->start_Lv;
				m_EventEndLv   = pNetMsg->end_Lv;
				m_EventBusterTime = pNetMsg->buster_Time;
				m_EventStartTime  = pNetMsg->play_Time;

				if( m_wLevel >= m_EventStartLv && m_wLevel <= m_EventEndLv )
				{
					m_bEventApply  = TRUE;
					CInnerInterface::GetInstance().BONUS_TIME_EVENT_START( !m_bEventBuster );
				}else{
					m_bEventApply  = FALSE;
					CInnerInterface::GetInstance().BONUS_TIME_EVENT_END();
				}
			}
			


		}
		break;
#endif

	case NET_MSG_GCTRL_ACTIVE_VEHICLE_FB:
		{
			GLMSG::SNETPC_ACTIVE_VEHICLE_FB* pNetMsg = (GLMSG::SNETPC_ACTIVE_VEHICLE_FB*) nmg;

			switch( pNetMsg->emFB)
			{
			case EMVEHICLE_SET_FB_RESET:
				{
					break;
				}
			case EMVEHICLE_SET_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_FAIL") );
					break;
				}			
			case EMVEHICLE_SET_FB_OK:
				{
 					if ( pNetMsg->bActive )
					{
						SetVehicle(  true );
					}
					else
					{
						if ( pNetMsg->bLeaveFieldServer )	m_bIsVehicleActive = true;
						SetVehicle(  false );
						
					}

					break;
				}

			case EMVEHICLE_SET_FB_NOTENOUGH_OIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_NOTENOUGH_OIL") );
					break;
				}
			case EMVEHICLE_SET_FB_MAP_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_MAP_FAIL") );
					break;
				}

			case EMVEHICLE_SET_FB_NO_ITEM:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_NO_ITEM") );
					break;
				}
			}

			
		}
		break;

	case NET_MSG_GCTRL_LANDEFFECT:
		{
			DISABLEALLLANDEFF();
			GLMSG::SNETPC_LANDEFFECT* pNetMsg = ( GLMSG::SNETPC_LANDEFFECT* ) nmg;
			memcpy( m_sLandEffect, pNetMsg->sLandEffect, sizeof(m_sLandEffect) );
		}
		break;


	case NET_MSG_GCTRL_GET_VEHICLE_FB:
		{
			GLMSG::SNETPC_GET_VEHICLE_FB* pNetMsg = (GLMSG::SNETPC_GET_VEHICLE_FB*) nmg;

			switch( pNetMsg->emFB ) 
			{
			case EMVEHICLE_GET_FB_OK:
				{
					m_PutOnItems[SLOT_VEHICLE].dwVehicleID = pNetMsg->dwGUID;
					m_sVehicle.m_dwGUID		= pNetMsg->dwGUID;
					m_sVehicle.m_emTYPE		= pNetMsg->emTYPE;
					m_sVehicle.m_dwOwner	= pNetMsg->dwOwner;
					m_sVehicle.m_sVehicleID = pNetMsg->sVehicleID;
					m_sVehicle.m_nFull		= pNetMsg->nFull;
					m_sVehicle.m_bBooster	= pNetMsg->bBooster; /*vehicle booster system, Juver, 2017/08/10 */

					for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
					{
						m_sVehicle.m_PutOnItems[i] = pNetMsg->PutOnItems[i];
					}

					/*bike color , Juver, 2017/11/13 */
					for( WORD i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
						m_sVehicle.m_wColor[i] = pNetMsg->wColor[i];
					
					m_sVehicle.SetActiveValue( true );
					
					m_sVehicle.ITEM_UPDATE();

					m_fVehicleSpeedRate = m_sVehicle.GetSpeedRate();
					m_fVehicleSpeedVol = m_sVehicle.GetSpeedVol();
					
					UpdateSuit ( FALSE );
					INIT_DATA( FALSE, FALSE );

					if ( m_bIsVehicleActive ) 
					{
						ReqSetVehicle( true );
						m_bIsVehicleActive = FALSE;
					}
				}
				break;
			case EMVEHICLE_GET_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "VEHICLE_GET_FB_FAIL" ));
				}
				break;
			case EMVEHICLE_GET_FB_INVALIDITEM:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "VEHICLE_GET_FB_INVALIDITEM" ));					
				}
				break;
			case EMVEHICLE_GET_FB_NOITEM:
				{
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "VEHICLE_GET_FB_NOITEM" ));					
				}
				break;			
			case EMVEHICLE_GET_FB_NODATA:
				{
					if ( m_PutOnItems[SLOT_VEHICLE].sNativeID != NATIVEID_NULL() ) 
						m_PutOnItems[SLOT_VEHICLE].dwVehicleID = 0;
					
					CInnerInterface::GetInstance().PrintMsgText( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT( "VEHICLE_GET_FB_NODATA" ));
				}
				break;

			}
		break;
			
		}
	case NET_MSG_GCTRL_UNGET_VEHICLE_FB:
		{
			GLMSG::SNETPC_UNGET_VEHICLE_FB* pNetMsg = (GLMSG::SNETPC_UNGET_VEHICLE_FB*) nmg;

			m_sVehicle.SetActiveValue( false );
			m_sVehicle.RESET();
		}
		break;

	case NET_MSG_VEHICLE_REQ_SLOT_EX_HOLD_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_SLOT_EX_HOLD_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_SLOT_EX_HOLD_FB* ) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL") );

				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_OK:
				{
					// ¿Â¬¯µ» æ∆¿Ã≈€ BackUp
					SITEMCUSTOM sSlotItemCustom = m_sVehicle.GetSlotitembySuittype ( pNetMsg->emSuit );

					// º’ø°µÁ æ∆¿Ã≈€¿ª ∆÷ø°∞‘ ¿Â¬¯Ω√≈∞∞Ì
					m_sVehicle.SetSlotItem ( pNetMsg->emSuit, GET_HOLD_ITEM () );
					m_fVehicleSpeedRate = m_sVehicle.GetSpeedRate();
					m_fVehicleSpeedVol = m_sVehicle.GetSpeedVol();


					// ≈ª∞Õ¿« ¡§∫∏∏¶ ∞ªΩ≈«ÿ¡ÿ¥Ÿ.
					VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( m_sVehicle.m_dwGUID );
					if ( iter!= m_mapVEHICLEItemInfo.end() )
					{
						SVEHICLEITEMINFO& sVehicle = (*iter).second;
						sVehicle.m_PutOnItems[(WORD)pNetMsg->emSuit-(WORD)SUIT_VEHICLE_SKIN] = GET_HOLD_ITEM ();
					}

					// ¿Â¬¯µ∆¥¯ æ∆¿Ã≈€¿ª º’ø° µÁ¥Ÿ
					HOLD_ITEM ( sSlotItemCustom );
					UpdateSuit ( FALSE );
					INIT_DATA( FALSE, FALSE );
				}
				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_NOMATCH:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_NOMATCH") );
				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_INVALIDITEM:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_INVALIDITEM") );
				break;
			};
		}
		break;
	case NET_MSG_VEHICLE_REQ_HOLD_TO_SLOT_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_HOLD_TO_SLOT_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_HOLD_TO_SLOT_FB* ) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL") );
				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_OK:
				{
					// º’ø°µÁ æ∆¿Ã≈€¿ª ∆÷ø°∞‘ ¿Â¬¯Ω√≈∞∞Ì
					m_sVehicle.SetSlotItem ( pNetMsg->emSuit, GET_HOLD_ITEM () );
					m_fVehicleSpeedRate = m_sVehicle.GetSpeedRate();
					m_fVehicleSpeedVol = m_sVehicle.GetSpeedVol();					

					// ≈ª∞Õ¿« ¡§∫∏∏¶ ∞ªΩ≈«ÿ¡ÿ¥Ÿ.
					VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( m_sVehicle.m_dwGUID );
					if ( iter!= m_mapVEHICLEItemInfo.end() )
					{
						SVEHICLEITEMINFO& sVehicle = (*iter).second;
						sVehicle.m_PutOnItems[(WORD)pNetMsg->emSuit-(WORD)SUIT_VEHICLE_SKIN] = GET_HOLD_ITEM ();
					}

					// º’ø°µÁ æ∆¿Ã≈€¿ª ¡¶∞≈«œ∞Ì
					RELEASE_HOLD_ITEM ();
					UpdateSuit ( FALSE );
					INIT_DATA( FALSE, FALSE );
				}
				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_NOMATCH:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_NOMATCH") );
				break;

			case EMPET_REQ_SLOT_EX_HOLD_FB_INVALIDITEM:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_INVALIDITEM") );
				break;
			};
		}
		break;
	case NET_MSG_VEHICLE_REQ_SLOT_TO_HOLD_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_SLOT_TO_HOLD_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_SLOT_TO_HOLD_FB* ) nmg;

			switch ( pNetMsg->emFB )
			{
			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_FAIL") );
				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_OK:
				{
					// ¿Â¬¯µ» æ∆¿Ã≈€ BackUp
					SITEMCUSTOM sSlotItemCustom = m_sVehicle.GetSlotitembySuittype ( pNetMsg->emSuit );

					// ¿Â¬¯µ» æ∆¿Ã≈€ ¡¶∞≈
					m_sVehicle.ReSetSlotItem ( pNetMsg->emSuit );
					m_fVehicleSpeedRate = m_sVehicle.GetSpeedRate();
					m_fVehicleSpeedVol = m_sVehicle.GetSpeedVol();

					// ≈ª∞Õ¿« ¡§∫∏∏¶ ∞ªΩ≈«ÿ¡ÿ¥Ÿ.
					VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( m_sVehicle.m_dwGUID );
					if ( iter!= m_mapVEHICLEItemInfo.end() )
					{
						SVEHICLEITEMINFO& sVehicle = (*iter).second;
						WORD i = (WORD)pNetMsg->emSuit - (WORD ) SUIT_VEHICLE_SKIN;
						if ( i < VEHICLE_ACCETYPE_SIZE ) sVehicle.m_PutOnItems[i] = SITEMCUSTOM ( NATIVEID_NULL() );
					}

					// ¿Â¬¯µ∆¥¯ æ∆¿Ã≈€¿ª º’ø° µÁ¥Ÿ
					HOLD_ITEM ( sSlotItemCustom );
					UpdateSuit ( FALSE );
					INIT_DATA( FALSE, FALSE );
				}
				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_NOMATCH:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_NOMATCH") );
				break;

			case EMVEHICLE_REQ_SLOT_EX_HOLD_FB_INVALIDITEM:
				CInnerInterface::GetInstance().PrintMsgText ( 
					NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("EMVEHICLE_REQ_SLOT_EX_HOLD_FB_INVALIDITEM") );
				break;
			}
		}
		break;
	
	case NET_MSG_VEHICLE_REMOVE_SLOTITEM_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_REMOVE_SLOTITEM_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_REMOVE_SLOTITEM_FB* ) nmg;
	
			WORD wPosX(0), wPosY(0);

			SITEMCUSTOM	sSlotItemCustom = m_sVehicle.GetSlotitembySuittype ( pNetMsg->emSuit );

			SITEM* pSlotItem = GLItemMan::GetInstance().GetItem ( sSlotItemCustom.sNativeID );
			if ( !pSlotItem ) 
			{
				// ¿œπ› ø¿∑˘
				return;
			}

			BOOL bOk = m_cInventory.FindInsrtable ( pSlotItem->sBasicOp.wInvenSizeX, pSlotItem->sBasicOp.wInvenSizeY, wPosX, wPosY );
			if ( !bOk )
			{
				//	¿ŒπÍ¿Ã ∞°µÊ¬˛¿Ω.
				return;
			}

			// ¿Œ∫•ø° ≥÷±‚
			m_cInventory.InsertItem ( sSlotItemCustom, wPosX, wPosY );

			// ΩΩ∑‘æ∆¿Ã≈€ ¡¶∞≈
			m_sVehicle.ReSetSlotItem ( pNetMsg->emSuit );

			m_fVehicleSpeedRate = m_sVehicle.GetSpeedRate();
			m_fVehicleSpeedVol = m_sVehicle.GetSpeedVol();


			// ≈ª∞Õ¿« ¡§∫∏∏¶ ∞ªΩ≈«ÿ¡ÿ¥Ÿ.
			VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( m_sVehicle.m_dwGUID );
			if ( iter!= m_mapVEHICLEItemInfo.end() )
			{
				SVEHICLEITEMINFO& sVehicle = (*iter).second;
				WORD i = (WORD)pNetMsg->emSuit - (WORD ) SUIT_VEHICLE_SKIN;
				if ( i < VEHICLE_ACCETYPE_SIZE ) sVehicle.m_PutOnItems[i] = SITEMCUSTOM ( NATIVEID_NULL() );
			}

			UpdateSuit ( FALSE );
			INIT_DATA( FALSE, FALSE );
		}
		break;
	case NET_MSG_VEHICLE_ACCESSORY_DELETE:
		{
			GLMSG::SNET_VEHICLE_ACCESSORY_DELETE* pNetMsg = (GLMSG::SNET_VEHICLE_ACCESSORY_DELETE*) nmg;
			
			if ( m_sVehicle.IsActiveValue() && m_sVehicle.m_dwGUID == pNetMsg->dwVehicleNum )
			{
                m_sVehicle.m_PutOnItems[pNetMsg->accetype] = SITEMCUSTOM ();			
				m_sVehicle.ITEM_UPDATE();
				m_fVehicleSpeedRate = m_sVehicle.GetSpeedRate();
				m_fVehicleSpeedVol = m_sVehicle.GetSpeedVol();
				UpdateSuit ( FALSE );
				INIT_DATA( FALSE, FALSE );
			}

			VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pNetMsg->dwVehicleNum  );
			if ( iter!=m_mapVEHICLEItemInfo.end() )
			{
				SVEHICLEITEMINFO& sVehicle = (*iter).second;
				// ∆÷ƒ´µÂ¿« ¡§∫∏ ∞ªΩ≈
				sVehicle.m_PutOnItems[pNetMsg->accetype] = SITEMCUSTOM ();	
			}


		}
		break;
	case NET_MSG_VEHICLE_UPDATE_CLIENT_BATTERY:
		{
			GLMSG::SNET_VEHICLE_UPDATE_CLIENT_BATTERY* pNetMsg = ( GLMSG::SNET_VEHICLE_UPDATE_CLIENT_BATTERY* ) nmg;
			m_sVehicle.m_nFull = pNetMsg->nFull;

			
			VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( m_sVehicle.m_dwGUID );
			if ( iter!=m_mapVEHICLEItemInfo.end() )
			{
				SVEHICLEITEMINFO& sVehicle = (*iter).second;
				// ∆÷ƒ´µÂ¿« ¡§∫∏ ∞ªΩ≈
				sVehicle.m_nFull = pNetMsg->nFull;
			}

		}
        break;
	case NET_MSG_VEHICLE_REQ_GIVE_BATTERY_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_GIVE_BATTERY_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_GIVE_BATTERY_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMVEHICLE_REQ_GIVE_BATTERY_FB_OK:
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sBatteryID );
					SITEM* pVehicleItem	= GLItemMan::GetInstance().GetItem ( pNetMsg->sItemID );
					if ( pItem && pVehicleItem )
					{
						// ∆÷∆˜∏∏∞® ∞ªΩ≈
						if ( m_sVehicle.m_dwGUID == pNetMsg->dwVehicleID ) m_sVehicle.IncreaseFull ( pItem->sDrugOp.wCureVolume, pItem->sDrugOp.bRatio );

						if ( DxGlobalStage::GetInstance().IsEmulator() )
						{
							CInnerInterface::GetInstance().PrintMsgText ( 
								NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMVEHICLE_REQ_GIVE_BATTERY_FB_OK"), pVehicleItem->GetName(), pItem->sDrugOp.wCureVolume );					
							break;
						}


						VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pNetMsg->dwVehicleID );
						if ( iter!=m_mapVEHICLEItemInfo.end() )
						{
							SVEHICLEITEMINFO& sVehicle = (*iter).second;
							// ∆÷ƒ´µÂ¿« ¡§∫∏ ∞ªΩ≈
							sVehicle.m_nFull = pNetMsg->nFull;

							CInnerInterface::GetInstance().PrintMsgText ( 
								NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMVEHICLE_REQ_GIVE_BATTERY_FB_OK"), pVehicleItem->GetName(), pItem->sDrugOp.wCureVolume );
							break;
						}

					}
				}
                break;
			case EMVEHICLE_REQ_GIVE_BATTERY_FB_FAIL:
                break;
			case EMVEHICLE_REQ_GIVE_BATTERY_FB_INVALIDBATTERY:
                break;
			case EMVEHICLE_REQ_GIVE_BATTERY_FB_INVALIDITEM:
                break;
			case EMVEHICLE_REQ_GET_BATTERY_FROMDB_OK:
                break;
			case EMVEHICLE_REQ_GET_BATTERY_FROMDB_FAIL:
                break;
			}
		}
		break;

		/*vehicle booster system, Juver, 2017/08/10 */
	case NET_MSG_VEHICLE_REQ_ENABLE_BOOSTER_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_ENABLE_BOOSTER_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_ENABLE_BOOSTER_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMVEHICLE_REQ_ENABLE_BOOSTER_FB_OK:
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sCardID );
					SITEM* pVehicleItem	= GLItemMan::GetInstance().GetItem ( pNetMsg->sItemID );
					if ( pItem && pVehicleItem )
					{
						if ( m_sVehicle.m_dwGUID == pNetMsg->dwVehicleID ) 
						{
							m_sVehicle.SetBooster( TRUE );
						}

						if ( DxGlobalStage::GetInstance().IsEmulator() )
						{
							CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_OK"), pVehicleItem->GetName() );					
							break;
						}

						VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pNetMsg->dwVehicleID );
						if ( iter!=m_mapVEHICLEItemInfo.end() )
						{
							SVEHICLEITEMINFO& sVehicle = (*iter).second;

							sVehicle.m_bBooster = true;

							CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_OK"), pVehicleItem->GetName() );
							break;
						}

					}
				}break;
			case EMVEHICLE_REQ_ENABLE_BOOSTER_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_FAIL") );
				}break;
			case EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD") );
				}break;
			case EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM") );
				}break;

			case EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE") );
				}break;

			case EMVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED") );
				}break;
			}
		}
		break;

		/* booster all vehicle, Juver, 2018/02/14 */
	case NET_MSG_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB:
		{
			GLMSG::SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB* pNetMsg = ( GLMSG::SNET_ALLVEHICLE_REQ_ENABLE_BOOSTER_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_OK:
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( pNetMsg->sCardID );
					SITEM* pVehicleItem	= GLItemMan::GetInstance().GetItem ( pNetMsg->sItemID );
					if ( pItem && pVehicleItem )
					{
						if ( m_sVehicle.m_dwGUID == pNetMsg->dwVehicleID ) 
						{
							m_sVehicle.SetBooster( TRUE );
						}

						if ( DxGlobalStage::GetInstance().IsEmulator() )
						{
							CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_OK"), pVehicleItem->GetName() );					
							break;
						}

						VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pNetMsg->dwVehicleID );
						if ( iter!=m_mapVEHICLEItemInfo.end() )
						{
							SVEHICLEITEMINFO& sVehicle = (*iter).second;

							sVehicle.m_bBooster = true;

							CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_OK"), pVehicleItem->GetName() );
							break;
						}
					}
				}break;

			case EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_FAIL") );
				}break;

			case EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_CARD") );
				}break;

			case EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_ITEM") );
				}break;

			case EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_INVALID_TYPE") );
				}break;

			case EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMALLVEHICLE_REQ_ENABLE_BOOSTER_FB_ALREADY_ENABLED") );
				}break;
			}
		}break;

	case NET_MSG_VEHICLE_REQ_ITEM_INFO_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_ITEM_INFO_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_ITEM_INFO_FB* ) nmg;

			// DBø° æ¯¿∏∏È ±◊≥… √ ±‚∞™¿∏∑Œ ≥—æÓø¿¥¬ ∞ÊøÏ∞° ¿÷¿∏π«∑Œ √º≈©«ÿ¡ÿ¥Ÿ.
			if ( pNetMsg->emTYPE == VEHICLE_TYPE_NONE ) break;

			SVEHICLEITEMINFO sVehicleItemInfo;
			sVehicleItemInfo.m_emTYPE	= pNetMsg->emTYPE;
			sVehicleItemInfo.m_nFull	= pNetMsg->nFull;
			sVehicleItemInfo.m_bBooster = pNetMsg->bBooster; /*vehicle booster system, Juver, 2017/08/10 */

			for ( WORD i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i )
			{
				sVehicleItemInfo.m_PutOnItems[i] = pNetMsg->PutOnItems[i];
			}

			/*bike color , Juver, 2017/11/13 */
			for( WORD i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
				sVehicleItemInfo.m_wColor[i] = pNetMsg->wColor[i];


			if ( !pNetMsg->bTrade )
			{
				VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pNetMsg->dwVehicleID );
				if ( iter != m_mapVEHICLEItemInfo.end() ) m_mapVEHICLEItemInfo.erase ( iter );

				m_mapVEHICLEItemInfo.insert ( std::make_pair(pNetMsg->dwVehicleID, sVehicleItemInfo) );
			}
			else
			{
				m_mapVEHICLEItemInfoTemp.insert ( std::make_pair(pNetMsg->dwVehicleID, sVehicleItemInfo) );
			}						

		}

		break;

	case NET_QBOX_OPTION_MEMBER:
		{
			GLMSG::SNET_QBOX_OPTION_MEMBER* pNetMsg = ( GLMSG::SNET_QBOX_OPTION_MEMBER* ) nmg;
			CInnerInterface::GetInstance().GetQBoxButton()->SetQBoxEnable(pNetMsg->bQBoxEnable);			
		}
		break;

	case NET_MSG_UPDATE_TRACING_CHARACTER:
		{
			NET_UPDATE_TRACINGCHAR* pNetMsg = ( NET_UPDATE_TRACINGCHAR* ) nmg;
			if( pNetMsg->updateNum == 0 )
				m_bTracingUser = FALSE;
			else
				m_bTracingUser = TRUE;
		}
		break;
	case NET_MSG_REQ_ATTENDLIST_FB:
		{
			GLMSG::SNETPC_REQ_ATTENDLIST_FB* pNetMsg = ( GLMSG::SNETPC_REQ_ATTENDLIST_FB* ) nmg;
			
			m_vecAttend.clear();

			for ( int i = 0; i < pNetMsg->dwAttenNum; ++i )
			{
				m_vecAttend.push_back( pNetMsg->sAttend[i] );	
			}
			
			m_dwComboAttend = pNetMsg->dwAttendCombo;
			
			m_tAttendLogin = pNetMsg->tLoginTime;
			m_dwAttendTime = pNetMsg->dwAttendTime;
			CInnerInterface::GetInstance().REFRESH_ATTENDBOOK();

		}
		break;
	case NET_MSG_REQ_ATTENDANCE_FB:
		{
			GLMSG::SNETPC_REQ_ATTENDANCE_FB* pNetMsg = ( GLMSG::SNETPC_REQ_ATTENDANCE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMREQ_ATTEND_FB_OK:
				{
					m_dwComboAttend = pNetMsg->dwComboAttend;
					USER_ATTEND_INFO sAttend;
					sAttend.tAttendTime = pNetMsg->tAttendTime;
					sAttend.bAttendReward = pNetMsg->bAttendReward;
					m_vecAttend.push_back( sAttend );

					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_ATTEND_FB_OK"), m_dwComboAttend );
				}
				break;
			case EMREQ_ATTEND_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ATTEND_FB_FAIL") );
				}
				break;
			case EMREQ_ATTEND_FB_ALREADY:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ATTEND_FB_ALREADY") );
				}
				break;
			case EMREQ_ATTEND_FB_ATTENTIME:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_ATTEND_FB_ATTENTIME"),m_dwAttendTime );
				}
				break;
			}

			CInnerInterface::GetInstance().REFRESH_ATTENDBOOK();

		}
		break;
	case NET_MSG_REQ_ATTEND_REWARD_CLT:
		{
			GLMSG::SNETPC_REQ_ATTEND_REWARD_CLT* pNetMsg = ( GLMSG::SNETPC_REQ_ATTEND_REWARD_CLT* ) nmg;

			SITEM* pItem = GLItemMan::GetInstance().GetItem( pNetMsg->idAttendReward );
			
			if ( pItem )
			{
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("ATTEND_REWARD_ITEM"), pItem->GetName()  );
			}
			else
			{
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("ATTEND_REWARD_NOITEM") );
			}

		}
		break;
	case NET_MSG_GCTRL_NPC_RECALL_FB:
		{
			GLMSG::SNET_INVEN_NPC_RECALL_FB* pNetMsg = ( GLMSG::SNET_INVEN_NPC_RECALL_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMREQ_NPC_RECALL_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_NPC_RECALL_FB_FAIL") );
				break;
			case EMREQ_NPC_RECALL_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_NPC_RECALL_FB_OK") );
				break;
			case EMREQ_NPC_RECALL_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_NPC_RECALL_FB_NOITEM") );
				break;
			case EMREQ_NPC_RECALL_FB_NPC:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_NPC_RECALL_FB_NPC") );
				break;
			case EMREQ_NPC_RECALL_FB_POS:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_NPC_RECALL_FB_POS") );
				break;
			}
		}
		break;
	case NET_MSG_GCTRL_INVEN_ITEM_MIX_FB:
		{
			GLMSG::SNET_INVEN_ITEM_MIX_FB* pNetMsg = ( GLMSG::SNET_INVEN_ITEM_MIX_FB* ) nmg;
			CString strMsg;

			bool bSucc = false;
			bool bFail = false;

			DWORD dwColor = NS_UITEXTCOLOR::DISABLE;

			switch( pNetMsg->emFB )
			{
			case EMITEM_MIX_FB_FAIL:
				{				
					strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_FAIL");
					bFail = true;
				}
				break;
			case EMITEM_MIX_FB_OK:
				{				
					SITEM* pItem = GLItemMan::GetInstance().GetItem( pNetMsg->sNativeID );
					if ( pItem ) 
					{
						strMsg = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("EMITEM_MIX_FB_OK") , pItem->GetName(), pNetMsg->wTurnNum );
						dwColor = NS_UITEXTCOLOR::PALEGREEN;
						bSucc = true;
					}					
				}				
				break;
			case EMITEM_MIX_FB_ERROR:
				{				
					strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_ERROR");
				}
				break;				
			case EMITEM_MIX_FB_NOMIX:
				strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_NOMIX");
				break;
			case EMITEM_MIX_FB_NOMIXNUM:
				strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_NOMIXNUM");
				break;
			case EMITEM_MIX_FB_NOITEM:
				strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_NOITEM");
				break;
			case EMITEM_MIX_FB_NOMONEY:
				strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_NOMONEY");
				break;
			case EMITEM_MIX_FB_NOINVEN:
				strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_NOINVEN");
				break;
			case EMITEM_MIX_FB_NONPC:
				strMsg = ID2GAMEINTEXT("EMITEM_MIX_FB_NONPC");
				break;
			}
			
			CInnerInterface::GetInstance().PrintMsgText ( dwColor, strMsg );
			CInnerInterface::GetInstance().SetItemMixResult( strMsg, bSucc, bFail );
			
			GLGaeaClient::GetInstance().GetCharacter()->ResetItemMix();
		}
		break;
	case NET_MSG_REQ_GATHERING_FB:
		{
			GLMSG::SNETPC_REQ_GATHERING_FB* pNetMsg = ( GLMSG::SNETPC_REQ_GATHERING_FB* ) nmg;
			
			switch ( pNetMsg->emFB )
			{
			case EMREQ_GATHER_FB_OK:
				{					
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_GATHER_FB_OK") );
					
					m_dwANISUBGESTURE = pNetMsg->dwAniSel;
					TurnAction( GLAT_GATHERING );
					CInnerInterface::GetInstance().SET_GATHER_GAUGE( pNetMsg->dwGaeaID, (float)pNetMsg->dwTime );
					CInnerInterface::GetInstance().ShowGroupBottom( GATHER_GAUGE );

				}
				break;
			case EMREQ_GATHER_FB_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_FAIL") );
				}
				break;
			case EMREQ_GATHER_FB_DISTANCE:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_DISTANCE") );
				}
				break;
			case EMREQ_GATHER_FB_NOTTYPE:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_NOTTYPE") );
				}
				break;
			case EMREQ_GATHER_FB_NOCROW:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_NOCROW") );
				}
				break;
			case EMREQ_GATHER_FB_USE:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_USE") );
				}
				break;
			case EMREQ_GATHER_FB_GATHERING:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_FB_GATHERING") );
				}
				break;
			}
		}
		break;
	case NET_MSG_REQ_GATHERING_RESULT:
		{
			GLMSG::SNETPC_REQ_GATHERING_RESULT* pNetMsg = ( GLMSG::SNETPC_REQ_GATHERING_RESULT* ) nmg;
			
			switch ( pNetMsg->emFB )
			{
			case EMREQ_GATHER_RESULT_SUCCESS:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, 
							ID2GAMEINTEXT("EMREQ_GATHER_RESULT_SUCCESS") );

					m_dwANISUBGESTURE = 0;	//	º∫∞¯
				}
				break;
			case EMREQ_GATHER_RESULT_SUCCESS_EX:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_RESULT_SUCCESS_EX") );	
					
					m_dwANISUBGESTURE = 0;	//	º∫∞¯
				}
				break;
			case EMREQ_GATHER_RESULT_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_RESULT_FAIL") );	

					m_dwANISUBGESTURE = 1;	//	Ω«∆–
				}
				break;
			case EMREQ_GATHER_RESULT_ITEMFAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText 
						( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GATHER_RESULT_ITEMFAIL") );					
					
					m_dwANISUBGESTURE = 1;	//	Ω«∆–
				}
				break;
			}			
			
			CInnerInterface::GetInstance().HideGroup( GATHER_GAUGE );
		}
		break;

		/*itemfood system, Juver, 2017/05/25 */
	case NET_MSG_GCTRL_INVEN_CONSUME_FOOD_FB:
		{
			GLMSG::SNET_INVEN_CONSUME_FOOD_FB* pNetMsg = ( GLMSG::SNET_INVEN_CONSUME_FOOD_FB* ) nmg;
			
			switch( pNetMsg->emFB )
			{
			case EMCONSUME_FOOD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_FAIL") );		
				break;
			case EMCONSUME_FOOD_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_OK") );
				break;
			case EMCONSUME_FOOD_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NOITEM") );		
				break;
			case EMCONSUME_FOOD_FB_NOSLOT:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NOSLOT") );		
				break;
			case EMCONSUME_FOOD_FB_NONAREA:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NONAREA") );		
				break;
			case EMCONSUME_FOOD_FB_COOLTIME:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_COOLTIME") );		
				break;
			case EMCONSUME_FOOD_FB_NONDATA:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONSUME_FOOD_FB_NONDATA") );		
				break;
			};
		}break;

		/*itemfood system, Juver, 2017/05/25 */
	case NET_MSG_GCTRL_FITEMFACT_BRD:
		{
			GLMSG::SNETPC_FITEMFACT_BRD *pNetMsg = (GLMSG::SNETPC_FITEMFACT_BRD *)nmg;
			if ( pNetMsg->sidSkill != NATIVEID_NULL() && pNetMsg->wSLOT != FITEMFACT_SIZE )
			{
				ItemFoodAdd ( pNetMsg->sidSkill, pNetMsg->sFACT.wLEVEL, pNetMsg->wSLOT );
			}
		}break;

		/*itemfood system, Juver, 2017/05/26 */
	case NET_MSG_GCTRL_INVEN_UNLOCK_FOOD_FB:
		{
			GLMSG::SNET_INVEN_UNLOCK_FOOD_FB* pNetMsg = ( GLMSG::SNET_INVEN_UNLOCK_FOOD_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMUNLOCK_FOOD_FB_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_FAIL") );		
				break;
			case EMUNLOCK_FOOD_FB_NOITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_NOITEM") );		
				break;
			case EMUNLOCK_FOOD_FB_UNLOCKED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_UNLOCKED") );		
				break;
			case EMUNLOCK_FOOD_FB_OK:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMUNLOCK_FOOD_FB_OK") );

					for( int i=0; i<FITEMFACT_SIZE; ++i ){
						m_sFITEMFACT[i].bENABLE = TRUE;
					}
				}break;
			};
		}break;

		/*combatpoint logic, Juver, 2017/05/29 */
	case NET_MSG_GCTRL_UPDATE_CP:
		{
			GLMSG::SNETPC_UPDATE_CP *pNetMsg = (GLMSG::SNETPC_UPDATE_CP *)nmg;
			m_sCombatPoint.dwNow = pNetMsg->wNowCP;

			if ( pNetMsg->bDie )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("COMBAT_POINT_DIE") );	

			if ( pNetMsg->bReset )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("COMBAT_POINT_RESET") );	
		}break;

		/*game stats, Juver, 2017/06/21 */
	case NET_MSG_GCTRL_PING_PACKET_FB:
		{
			GLMSG::SNETPC_PING_PACKET_FB *pNetMsg = (GLMSG::SNETPC_PING_PACKET_FB *)nmg;
						
			if ( pNetMsg->dwType == 1 )
			{
			}

			if ( pNetMsg->dwType == 2 )
			{
				int nLastClockDiffField = (int)clock() - m_clock_Packet;
				CInnerInterface::GetInstance().GetGameStatsDisplay()->UpdatePing( float(nLastClockDiffField) );
			}
		}break;

		/*npc shop, Juver, 2017/07/27 */
	case NET_MSG_GCTRL_NPCSHOP_PURCHASE_MONEY_FB:
		{
			GLMSG::SNETPC_REQ_NPCSHOP_PURCHASE_MONEY_FB *pNetMsg = (GLMSG::SNETPC_REQ_NPCSHOP_PURCHASE_MONEY_FB *)nmg;
			switch( pNetMsg->emFB )
			{
			case EMNPCSHOP_PURCHASE_FB_INVALID_ITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_ITEM") );
				break;

			case EMNPCSHOP_PURCHASE_FB_INVALID_CROW:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_CROW") );
				break;

			case EMNPCSHOP_PURCHASE_FB_INVALID_SHOPITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_SHOPITEM") );
				break;

			case EMNPCSHOP_PURCHASE_FB_INVALID_SHOPTYPE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_INVALID_SHOPTYPE") );
				break;

			case EMNPCSHOP_PURCHASE_FB_NOT_MONEY:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_NOT_MONEY") );
				break;

			case EMNPCSHOP_PURCHASE_FB_NOT_INVEN_SPACE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_NOT_INVEN_SPACE") );
				break;

			case EMNPCSHOP_PURCHASE_FB_PURCHASED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMNPCSHOP_PURCHASE_FB_PURCHASED") );
				break;
			};
		}break;

		/*vehicle booster system, Juver, 2017/08/11 */
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_CHARGE:
		{
			GLMSG::SNETPC_REQ_VEHICLE_BOOSTER_STATE_CHARGE *pNetMsg = (GLMSG::SNETPC_REQ_VEHICLE_BOOSTER_STATE_CHARGE *)nmg;
			m_bBoosterCharge = pNetMsg->bStart;
			m_fBoosterTimer = pNetMsg->fTime;

			if ( m_bBoosterCharge )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("BIKE_BOOSTER_RECHARGE") );
		}break;

		/*vehicle booster system, Juver, 2017/08/11 */
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_START:
		{
			GLMSG::SNETPC_REQ_VEHICLE_BOOSTER_STATE_START *pNetMsg = (GLMSG::SNETPC_REQ_VEHICLE_BOOSTER_STATE_START *)nmg;
			m_bBoosterStart = pNetMsg->bStart;
			m_fBoosterTimer = pNetMsg->fTime;

			if ( m_bBoosterStart )
			{
				VehicleBoosterStateOn();
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("BIKE_BOOSTER_START") );
			}
			
			if ( !m_bBoosterStart && IsSTATE( EM_ACT_VEHICLE_BOOSTER ) )	ReSetSTATE( EM_ACT_VEHICLE_BOOSTER );
		}break;

		/*vehicle booster system, Juver, 2017/08/11 */
	case NET_MSG_GCTRL_VEHICLE_BOOSTER_STATE_RESET:
		{
			GLMSG::SNETPC_REQ_VEHICLE_BOOSTER_STATE_RESET *pNetMsg = (GLMSG::SNETPC_REQ_VEHICLE_BOOSTER_STATE_RESET *)nmg;

			m_bBoosterCharge = false;
			m_bBoosterStart = false;
			m_fBoosterTimer = 0.0f;

			if ( IsSTATE( EM_ACT_VEHICLE_BOOSTER ) )	ReSetSTATE( EM_ACT_VEHICLE_BOOSTER );
		}break;

		/*contribution point, Juver, 2017/08/23 */
	case NET_MSG_GCTRL_UPDATE_CONTRIBUTION_POINT:
		{
			GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT *pNetMsg = (GLMSG::SNETPC_UPDATE_CONTRIBUTION_POINT *)nmg;

			LONGLONG llPoint = m_llContributionPoint;
			m_llContributionPoint = pNetMsg->llPoint;

			if ( pNetMsg->bNotice && m_llContributionPoint > llPoint )
			{
				LONGLONG llAdded = m_llContributionPoint - llPoint;
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("CONTRIBUTION_POINT_UPDATE"), llAdded );
			}
		}break;

		/*activity point, Juver, 2017/08/23 */
	case NET_MSG_GCTRL_UPDATE_ACTIVITY_POINT:
		{
			GLMSG::SNETPC_UPDATE_ACTIVITY_POINT *pNetMsg = (GLMSG::SNETPC_UPDATE_ACTIVITY_POINT *)nmg;

			DWORD dwPoint = m_dwActivityPoint;
			m_dwActivityPoint = pNetMsg->dwPoint;

			if ( pNetMsg->bNotice && m_dwActivityPoint > dwPoint )
			{
				DWORD dwAdded = m_dwActivityPoint - dwPoint;
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("ACTIVITY_POINT_UPDATE"), dwAdded );
				
			}
		}break;

		/*system buffs, Juver, 2017/09/05 */
	case NET_MSG_GCTRL_SYSTEM_BUFF_BRD:
		{
			GLMSG::SNETPC_SYSTEMBUFF_BRD *pNetMsg = (GLMSG::SNETPC_SYSTEMBUFF_BRD *)nmg;
			for( int i=0; i<SYSTEM_BUFF_SIZE; ++i )
			{
				SystemBuffAdd( pNetMsg->sSYSTEM_BUFF[i].sNATIVEID, pNetMsg->sSYSTEM_BUFF[i].wLEVEL, pNetMsg->sSYSTEM_BUFF[i].wSLOT );
			}
		}break;

		/*item exchange, Juver, 2017/10/13 */
	case NET_MSG_GCTRL_NPC_ITEM_EXCHANGE_TRADE_FB:
		{
			GLMSG::SNETPC_REQ_NPC_ITEM_EXCHANGE_TRADE_FB *pNetMsg = (GLMSG::SNETPC_REQ_NPC_ITEM_EXCHANGE_TRADE_FB *)nmg;
			switch( pNetMsg->emFB )
			{
			case EMNPC_ITEM_EXCHANGE_FB_INVALID_NPC:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_NPC") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_INVALID_ID:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_ID") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_INVALID_ITEM:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INVALID_ITEM") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_NOCONTRIBUTIONPOINT:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_NOCONTRIBUTIONPOINT") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_NOPOINT:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_NOPOINT") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_NOT_INVEN_SPACE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_NOT_INVEN_SPACE") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_REQUIRE_MISSING") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_DELETE_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_DELETE_FAIL") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_INSERT_FAIL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_INSERT_FAIL") );
				break;

			case EMNPC_ITEM_EXCHANGE_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMNPC_ITEM_EXCHANGE_FB_OK") );
				break;
			};
		}break;

	case NET_MSG_GCTRL_INVEN_REFORM_WEAPON_FB://Weapon Skill Reform Card - SqueeeAK
		{
			GLMSG::SNET_INVEN_REFORM_WEAPON_FB *pNetMsg = (GLMSG::SNET_INVEN_REFORM_WEAPON_FB *)nmg;

			if(pNetMsg->bSuccess) CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::LIGHTSKYBLUE, ID2GAMEINTEXT("SUCCESS_WEAPON_REFORM") );
			else CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("FAILED_WEAPON_REFORM") );
		}
		break;

		/*product item, Juver, 2017/10/18 */
	case NET_MSG_GCTRL_ITEM_COMPOUND_START_FB:
		{
			GLMSG::SNETPC_REQ_ITEM_COMPOUND_START_FB *pNetMsg = (GLMSG::SNETPC_REQ_ITEM_COMPOUND_START_FB *)nmg;
			switch( pNetMsg->emFB )
			{
			case RAN_PRODUCT_FB_STARTED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("RAN_PRODUCT_FB_STARTED") );
				m_bItemCompoundTask = pNetMsg->bItemCompoundTask;
				m_dwItemCompoundTaskID = pNetMsg->dwItemCompoundTaskID;
				m_fItemCompoundTaskTime = pNetMsg->fItemCompoundTaskTime;
				m_fItemCompoundTaskTimer = pNetMsg->fItemCompoundTaskTimer;

				break;

			case RAN_PRODUCT_FB_CANCEL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_CANCEL") );
				m_bItemCompoundTask = pNetMsg->bItemCompoundTask;
				m_dwItemCompoundTaskID = pNetMsg->dwItemCompoundTaskID;
				m_fItemCompoundTaskTime = pNetMsg->fItemCompoundTaskTime;
				m_fItemCompoundTaskTimer = pNetMsg->fItemCompoundTaskTimer;

				break;

			case RAN_PRODUCT_FB_INVALID_ID:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_ID") );
				break;

			case RAN_PRODUCT_FB_INVALID_COST:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_COST") );
				break;

			case RAN_PRODUCT_FB_INVALID_CHAR_LEVEL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_INVALID_CHAR_LEVEL") );
				break;

			case RAN_PRODUCT_FB_RESULT_ITEM_INVALID:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_RESULT_ITEM_INVALID") );
				break;

			case RAN_PRODUCT_FB_RESULT_ITEM_NO_SPACE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_RESULT_ITEM_NO_SPACE") );
				break;

			case RAN_PRODUCT_FB_MATERIAL:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_MATERIAL") );
				break;

			case RAN_PRODUCT_FB_FAIL_COSTUME:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_FAIL_COSTUME") );
				break;

			case RAN_PRODUCT_FB_FAIL_DELETE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_FAIL_DELETE") );
				break;

			case RAN_PRODUCT_FB_FAIL_INSERT:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_FAIL_INSERT") );
				break;

			case RAN_PRODUCT_FB_TASK_RUNNING:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_TASK_RUNNING") );
				break;

			case RAN_PRODUCT_FB_FEATURE_OFF:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_FB_FEATURE_OFF") );
				break;
			};
		}break;

		/*product item, Juver, 2017/10/18 */
	case NET_MSG_GCTRL_ITEM_COMPOUND_PROCESS_FB:
		{
			GLMSG::SNETPC_REQ_ITEM_COMPOUND_PROCESS_FB *pNetMsg = (GLMSG::SNETPC_REQ_ITEM_COMPOUND_PROCESS_FB *)nmg;
			switch( pNetMsg->emFB )
			{
			case RAN_PRODUCT_PROCESS_FB_OK:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("RAN_PRODUCT_PROCESS_FB_OK") );
				break;

			case RAN_PRODUCT_PROCESS_FB_FAILED:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_PROCESS_FB_FAILED") );
				break;

			case RAN_PRODUCT_PROCESS_FB_FULL_INVEN:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_PROCESS_FB_FULL_INVEN") );
				break;

			case RAN_PRODUCT_PROCESS_FB_FAILED_INSERT:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("RAN_PRODUCT_PROCESS_FB_FAILED_INSERT") );
				break;
			};
		}break;
	
		/*activity system, Juver, 2017/10/30 */
	case NET_MSG_GCTRL_ACTIVITY_UPDATE:
		{
			GLMSG::SNETPC_ACTIVITY_UPDATE *pNetMsg = (GLMSG::SNETPC_ACTIVITY_UPDATE *)nmg;

			SACTIVITY_CHAR_DATA sactivity_char_data = pNetMsg->sData;

			SACTIVITY_CHAR_DATA* pactivity_char_data = GetActivityProg( sactivity_char_data.dwActivityID );
			if ( pactivity_char_data )
			{
				*pactivity_char_data = sactivity_char_data;
			}
		}break;

		/*activity system, Juver, 2017/10/30 */
	case NET_MSG_GCTRL_ACTIVITY_COMPLETE:
		{
			GLMSG::SNETPC_ACTIVITY_COMPLETE *pNetMsg = (GLMSG::SNETPC_ACTIVITY_COMPLETE *)nmg;

			SACTIVITY_CHAR_DATA sactivity_char_data = pNetMsg->sData;

			SACTIVITY_CHAR_DATA_MAP_ITER iterdel = m_mapActivityProg.find( sactivity_char_data.dwActivityID );
			if ( iterdel != m_mapActivityProg.end() )
			{
				m_mapActivityProg.erase( iterdel );
			}

			m_mapActivityDone.insert( std::make_pair( sactivity_char_data.dwActivityID, sactivity_char_data ) );

			STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
			DxEffGroupPlayer::GetInstance().NewEffGroup( GLCONST_CHAR::strCOMPLETE_ACTIVITY_EFFECT.c_str(), m_matTrans, &sTargetID );

		}break;

		/*activity system, Juver, 2017/10/30 */
	case NET_MSG_GCTRL_ACTIVITY_NOTIFY_CLIENT:
		{
			GLMSG::SNETPC_ACTIVITY_NOTIFY_CLIENT *pNetMsg = (GLMSG::SNETPC_ACTIVITY_NOTIFY_CLIENT *)nmg;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GOLD, ID2GAMEINTEXT("RAN_ACTIVITY_COMPLETE"), pNetMsg->szCharacterName, pNetMsg->szActivityTitle );
		}break;
/*Codex System, Jhoniex 2/14/2024 */
	case NET_MSG_GCTRL_CODEX_UPDATE:
		{
			GLMSG::SNETPC_CODEX_UPDATE *pNetMsg = (GLMSG::SNETPC_CODEX_UPDATE *)nmg;

			SCODEX_CHAR_DATA sactivity_char_data = pNetMsg->sData;

			SCODEX_CHAR_DATA* pactivity_char_data = GetCodexProg( sactivity_char_data.dwCodexID );
			if ( pactivity_char_data )
			{
				*pactivity_char_data = sactivity_char_data;
			}
		}break;
		/*Codex System, Jhoniex 2/14/2024 */
	case NET_MSG_GCTRL_CODEX_COMPLETE:
		{
			GLMSG::SNETPC_CODEX_COMPLETE *pNetMsg = (GLMSG::SNETPC_CODEX_COMPLETE *)nmg;

			SCODEX_CHAR_DATA sactivity_char_data = pNetMsg->sData;

			SCODEX_CHAR_DATA_MAP_ITER iterdel = m_mapCodexProg.find( sactivity_char_data.dwCodexID );
			if ( iterdel != m_mapCodexProg.end() )
			{
				m_mapCodexProg.erase( iterdel );
			}

			m_mapCodexDone.insert( std::make_pair( sactivity_char_data.dwCodexID, sactivity_char_data ) );
			
			CODEX_STATS(1,1);

			STARGETID sTargetID(CROW_PC,m_dwGaeaID,m_vPos);
			DxEffGroupPlayer::GetInstance().NewEffGroup( GLCONST_CHAR::strCOMPLETE_ACTIVITY_EFFECT.c_str(), m_matTrans, &sTargetID );

		}break;

		/*Codex System, Jhoniex 2/14/2024 */
	case NET_MSG_GCTRL_CODEX_NOTIFY_CLIENT:
		{
			GLMSG::SNETPC_CODEX_NOTIFY_CLIENT *pNetMsg = (GLMSG::SNETPC_CODEX_NOTIFY_CLIENT *)nmg;
			//CODEX_STATS(1,1);
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GOLD, ID2GAMEINTEXT("RAN_CODEX_COMPLETE"), pNetMsg->szCharacterName, pNetMsg->szCodexTitle );
		}break;
		/*activity system, Juver, 2017/11/05 */
	case NET_MSG_GCTRL_CHARACTER_BADGE_CHANGE_FB:
		{
			GLMSG::SNETPC_REQ_CHARACTER_BADGE_CHANGE_FB *pNetMsg = (GLMSG::SNETPC_REQ_CHARACTER_BADGE_CHANGE_FB *)nmg;
			switch( pNetMsg->emFB )
			{
			case EMFB_CHAR_TITLE_FAIL:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAR_TITLE_FAIL") );
				}break;
			case EMFB_CHAR_TITLE_SAME:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHAR_TITLE_SAME") );
				}break;
			case EMFB_CHAR_TITLE_DONE:
				{
					StringCchCopy ( m_szBadge, CHAR_SZNAME, pNetMsg->szBadge );
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_CHAR_TITLE_DONE") );
				}break;
			};
			
		}break;

		/*charinfoview , Juver, 2017/11/11 */
	case NET_MSG_GCTRL_REQ_CHARINFO_FB:
		{
			GLMSG::SNETPC_REQ_CHARINFO_FB* pNetMsg = ( GLMSG::SNETPC_REQ_CHARINFO_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMREQ_CHARINFO_FB_INVALID_TARGET:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_SHOW_CHARACTER_INVALID_TARGET") );	
				}break;

			case EMREQ_CHARINFO_FB_TIMEDELAY:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_SHOW_CHARACTER_TIMEDELAY") );	
				}break;

			case EMREQ_CHARINFO_FB_WEARINFO:
				{
					if ( !CInnerInterface::GetInstance().IsVisibleGroup( CHARACTER_INFO_VIEW_WINDOW )	)
						CInnerInterface::GetInstance().ShowGroupFocus( CHARACTER_INFO_VIEW_WINDOW );

					CInnerInterface::GetInstance().GetCharInfoViewWindow()->SetCharacterData( &pNetMsg->sCharInfo );

				}break;
			};
		}break;

		/*bike color , Juver, 2017/11/13 */
	case NET_MSG_VEHICLE_REQ_CHANGE_COLOR_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_CHANGE_COLOR_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_CHANGE_COLOR_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMREQ_CHANGE_BIKE_COLOR_FB_CHANGE_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_FAIL") );
				break;

			case EMREQ_CHANGE_BIKE_COLOR_FB_INVALID_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_CARD") );
				break;

			case EMREQ_CHANGE_BIKE_COLOR_FB_VEHICLE_INACTIVE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_VEHICLE") );
				break;

			case EMREQ_CHANGE_BIKE_COLOR_FB_VEHICLE_NOT_BIKE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_BIKE_COLOR_NOT_BIKE") );
				break;

			case EMREQ_CHANGE_BIKE_COLOR_FB_CHANGE_DONE:
				{
					VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pNetMsg->dwVehicleID );
					if ( iter!=m_mapVEHICLEItemInfo.end() )
					{
						SVEHICLEITEMINFO& sVehicle = (*iter).second;

						for( WORD i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
						{
							sVehicle.m_wColor[i] = pNetMsg->wColor[i];
						}
					}

					if ( m_sVehicle.m_dwGUID == pNetMsg->dwVehicleID )
					{
						for( WORD i=0; i<BIKE_COLOR_SLOT_PART_SIZE; ++i )
						{
							m_sVehicle.m_wColor[i] = pNetMsg->wColor[i];
						}

						UpdateSuit();

						CInnerInterface::GetInstance().PrintMsgText (  NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_BIKE_COLOR_DONE") );
					}
				}break;
			};
		}
		break;

		/*pk info, Juver, 2017/11/17 */
	case NET_MSG_GCTRL_UPDATE_PK_SCORE:
		{
			GLMSG::SNETPC_UPDATE_PK_SCORE* pNetMsg = ( GLMSG::SNETPC_UPDATE_PK_SCORE* ) nmg;
			m_dwPKScore = pNetMsg->dwVal;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::GREENYELLOW, ID2GAMEINTEXT("UPDATE_PK_SCORE"), pNetMsg->szName );
		}break;

		/*pk info, Juver, 2017/11/17 */
	case NET_MSG_GCTRL_UPDATE_PK_DEATH:
		{
			GLMSG::SNETPC_UPDATE_PK_DEATH* pNetMsg = ( GLMSG::SNETPC_UPDATE_PK_DEATH* ) nmg;
			m_dwPKDeath = pNetMsg->dwVal;
			CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::ORNAGERED, ID2GAMEINTEXT("UPDATE_PK_DEATH"), pNetMsg->szName );
		}break;

		/*rv card, Juver, 2017/11/25 */
	case NET_MSG_GCTRL_INVEN_RANDOM_OPTION_CHANGE_FB:
		{
			GLMSG::SNET_INVEN_RANDOM_OPTION_CHANGE_FB* pNetMsg = ( GLMSG::SNET_INVEN_RANDOM_OPTION_CHANGE_FB* ) nmg;

			switch ( pNetMsg->emFB )
			{
			case EM_RANDOM_OPTION_CHANGE_INVALID_CARD_ITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_INVALID_CARD_ITEM") );
				break;
			case EM_RANDOM_OPTION_CHANGE_INVALID_TARGET_ITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_INVALID_TARGET_ITEM") );
				break;
			case EM_RANDOM_OPTION_CHANGE_TARGET_NOT_SUIT:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_NOT_SUIT") );
				break;
			case EM_RANDOM_OPTION_CHANGE_SUIT_MISMATCH:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_SUIT_MISMATCH") );
				break;
			case EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_EXIST") );
				break;
			case EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_ALREADY_SET") );
				break;
			case EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_FREE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_TARGET_OPTION_NOT_FREE") );
				break;
			case EM_RANDOM_OPTION_CHANGE_DONE_CHANGE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EM_RANDOM_OPTION_CHANGE_DONE_CHANGE") );
				break;
			};
		}break;

		/*nondrop card, Juver, 2017/11/26 */
	case NET_MSG_GCTRL_INVEN_NONDROP_CARD_FB:
		{
			GLMSG::SNET_INVEN_NONDROP_CARD_FB* pNetMsg = ( GLMSG::SNET_INVEN_NONDROP_CARD_FB* ) nmg;

			switch ( pNetMsg->emFB )
			{
			case EM_NONDROP_CARD_INVALID_CARD_ITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_INVALID_CARD_ITEM") );
				break;
			case EM_NONDROP_CARD_INVALID_TARGET_ITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_INVALID_TARGET_ITEM") );
				break;
			case EM_NONDROP_CARD_TARGET_NOT_SUIT:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_TARGET_NOT_SUIT") );
				break;
			case EM_NONDROP_CARD_TARGET_ALREADY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EM_NONDROP_CARD_TARGET_ALREADY") );
				break;
			case EM_NONDROP_CARD_DONE_CHANGE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EM_NONDROP_CARD_DONE_CHANGE") );
				break;
			};
		}break;

	/*change scale card, Juver, 2018/01/04 */
	case NET_MSG_GCTRL_INVEN_SCALE_CHANGE_FB:
		{
			GLMSG::SNETPC_INVEN_SCALE_CHANGE_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_SCALE_CHANGE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_CHANGE_SCALE_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCALE_FAIL") );
				break;

			case EMFB_CHANGE_SCALE_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCALE_NOT_CARD") );
				break;

			case EMFB_CHANGE_SCALE_OK:
				{
					m_fScaleRange = pNetMsg->fScale;
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_CHANGE_SCALE_OK") );
				}break;
			};
		}break;

		/*item color, Juver, 2018/01/10 */
	case NET_MSG_GCTRL_INVEN_ITEMCOLOR_CHANGE_FB:
		{
			GLMSG::SNETPC_INVEN_ITEMCOLOR_CHANGE_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_ITEMCOLOR_CHANGE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_ITEM_COLOR_CHANGE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_FAILED") );
				break;

			case EMFB_ITEM_COLOR_CHANGE_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_NOT_CARD") );
				break;

			case EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_SLOT_ERROR") );
				break;

			case EMFB_ITEM_COLOR_CHANGE_DONE:
				{
					m_PutOnItems[pNetMsg->emSlot].wColor1 = pNetMsg->wColor1;
					m_PutOnItems[pNetMsg->emSlot].wColor2 = pNetMsg->wColor2;
					UpdateSuit();
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_ITEM_COLOR_CHANGE_DONE") );
				}break;
			};
		}break;

		/*item wrapper, Juver, 2018/01/12 */
	case NET_MSG_GCTRL_INVEN_WRAP_FB:
		{
			GLMSG::SNETPC_INVEN_WRAP_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_WRAP_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_ITEM_WRAP_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_FAILED") );
				break;

			case EMFB_ITEM_WRAP_TARGET_NOT_VALID:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_NOT_VALID") );
				break;

			case EMFB_ITEM_WRAP_TARGET_HAVE_DISGUISE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_TARGET_HAVE_DISGUISE") );
				break;

			case EMFB_ITEM_WRAP_HOLD_INVALID:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_INVALID") );
				break;

			case EMFB_ITEM_WRAP_HOLD_NOT_WRAPPER:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_NOT_WRAPPER") );
				break;

			case EMFB_ITEM_WRAP_HOLD_NOT_BOX:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_WRAP_HOLD_NOT_BOX") );
				break;

			case EMFB_ITEM_WRAP_DONE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_ITEM_WRAP_DONE") );
				break;
			};
		}break;

		/*item wrapper, Juver, 2018/01/12 */
	case NET_MSG_GCTRL_INVEN_UNWRAP_FB:
		{
			GLMSG::SNETPC_INVEN_UNWRAP_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_UNWRAP_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_ITEM_UNWRAP_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_FAILED") );
				break;

			case EMFB_ITEM_UNWRAP_TARGET_NOT_VALID:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_TARGET_NOT_VALID") );
				break;

			case EMFB_ITEM_UNWRAP_DONE:
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_ITEM_UNWRAP_DONE") );
				break;
			};
		}break;

		/*change school card, Juver, 2018/01/12 */
	case NET_MSG_GCTRL_INVEN_CHANGE_SCHOOL_FB:
		{
			GLMSG::SNETPC_INVEN_CHANGE_SCHOOL_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_CHANGE_SCHOOL_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_CHANGE_SCHOOL_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_FAILED") );
				break;

			case EMFB_CHANGE_SCHOOL_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_CARD") );
				break;

			case EMFB_CHANGE_SCHOOL_NOT_SELECTED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_NOT_SELECTED") );
				break;

			case EMFB_CHANGE_SCHOOL_REQ_CLEAN_QUEST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_CLEAN_QUEST") );
				break;

			case EMFB_CHANGE_SCHOOL_REQ_CLEAN_GUILD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_CLEAN_GUILD") );
				break;

			case EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_REQ_MAX_LEVEL") );
				break;

			case EMFB_CHANGE_SCHOOL_SAME_SCHOOL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_SAME_SCHOOL") );
				break;

			case EMFB_CHANGE_SCHOOL_DONE:
				{
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_DONE") );
					DoModal( ID2GAMEINTEXT("EMFB_CHANGE_SCHOOL_DONE"),  MODAL_INFOMATION, OK, MODAL_GENDER_CHANGE_END );	
				}break;
			};
		}break;

		/*equipment lock, Juver, 2018/01/14 */
	case NET_MSG_GCTRL_INVEN_EQUIPMENT_LOCK_ENABLE_FB:
		{
			GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_ENABLE_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_ENABLE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_EQUIPMENT_LOCK_ENABLE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_FAILED") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_NOT_CARD") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_ALREADY_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_ALREADY_ENABLED") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_KEY_EMPTY") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_KEY_SIZE") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_KEY_NOT_SAME:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_KEY_NOT_SAME") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_KEY_NUMBER_ONLY") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_ALREADY_ENABLED_DB:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_ALREADY_ENABLED_DB") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_CHAR_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_CHAR_NOT_EXIST") );
				break;

			case EMFB_EQUIPMENT_LOCK_ENABLE_DONE:
				{
					m_bEnableEquipmentLock = pNetMsg->bEquipmentLockEnable;
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_ENABLE_DONE") );
				}break;
			};
		}break;

		/*equipment lock, Juver, 2018/01/16 */
	case NET_MSG_GCTRL_INVEN_EQUIPMENT_LOCK_INPUT_FB:
		{
			GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_INPUT_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_INPUT_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_EQUIPMENT_LOCK_INPUT_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_FAILED") );
				break;

			case EMFB_EQUIPMENT_LOCK_INPUT_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_NOT_ENABLED") );
				break;

			case EMFB_EQUIPMENT_LOCK_INPUT_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_KEY_EMPTY") );
				break;

			case EMFB_EQUIPMENT_LOCK_INPUT_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_KEY_SIZE") );
				break;

			case EMFB_EQUIPMENT_LOCK_INPUT_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_KEY_NUMBER_ONLY") );
				break;

			case EMFB_EQUIPMENT_LOCK_INPUT_DATA_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_DATA_NOT_EXIST") );
				break;

			case EMFB_EQUIPMENT_LOCK_INPUT_TIMER:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_TIMER") );
				break;

			case EMFB_EQUIPMENT_LOCK_INPUT_DONE:
				{
					m_bEquipmentLockStatus = pNetMsg->bEquipmentLockStatus;
					
					if ( m_bEquipmentLockStatus )
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_DONE1") );
					else
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_INPUT_DONE2") );
				}break;
			};
		}break;

		/*equipment lock, Juver, 2018/01/16 */
	case NET_MSG_GCTRL_INVEN_EQUIPMENT_LOCK_RECOVER_FB:
		{
			GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_RECOVER_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_RECOVER_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_EQUIPMENT_LOCK_RECOVER_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_FAILED") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_NOT_ENABLED") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_NOT_CARD") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_NOT_EXIST") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_DONE:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_DONE") );
					CInnerInterface::GetInstance().OpenEquipmentLockRecover( pNetMsg->szName, pNetMsg->szPin );
				}break;
			};
		}break;

		/*equipment lock, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_INVEN_EQUIPMENT_LOCK_RECOVER_CHANGE_FB:
		{
			GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_RECOVER_CHANGE_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_RECOVER_CHANGE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_FAILED") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_EMPTY") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_SIZE") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_NOT_SAME:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_NOT_SAME") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_KEY_NUMBER_ONLY") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_NOT_ENABLED") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_NOT_FOUND:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_NOT_FOUND") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_DONE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_CHANGE_DONE") );
				break;
			};
		}break;

		/*equipment lock, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_INVEN_EQUIPMENT_LOCK_RECOVER_DELETE_FB:
		{
			GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_RECOVER_DELETE_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_EQUIPMENT_LOCK_RECOVER_DELETE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_FAILED") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_NOT_FOUND:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_NOT_FOUND") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_NOT_ENABLED") );
				break;

			case EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_DONE:
				{
					m_bEnableEquipmentLock = pNetMsg->bEquipmentLockEnable;
					m_bEquipmentLockStatus = pNetMsg->bEquipmentLockStatus;

					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_EQUIPMENT_LOCK_RECOVER_DELETE_DONE") );
				}break;
			};
		}break;

		/*item transfer card, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_INVEN_ITEM_TRANSFER_FB:
		{	
			GLMSG::SNETPC_INVEN_TRANSFER_STATS_FB* pNetMsg = ( GLMSG::SNETPC_INVEN_TRANSFER_STATS_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_ITEM_TRANSFER_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_FAILED") );
				break;

			case EMFB_ITEM_TRANSFER_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_CARD") );
				break;

			case EMFB_ITEM_TRANSFER_INVALID_ITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_INVALID_ITEM") );
				break;

			case EMFB_ITEM_TRANSFER_INVALID_ITEM_TYPE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_INVALID_ITEM_TYPE") );
				break;

			case EMFB_ITEM_TRANSFER_TIME_LIMIT:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_TIME_LIMIT") );
				break;

			case EMFB_ITEM_TRANSFER_DISGUISE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_DISGUISE") );
				break;

			case EMFB_ITEM_TRANSFER_SUIT_MISMATCH:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_SUIT_MISMATCH") );
				break;

			case EMFB_ITEM_TRANSFER_ATTACK_MISMATCH:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_ATTACK_MISMATCH") );
				break;

			case EMFB_ITEM_TRANSFER_COST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_COST") );
				break;

			case EMFB_ITEM_TRANSFER_HAVE_DISGUISE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_HAVE_DISGUISE") );
				break;

			case EMFB_ITEM_TRANSFER_NOT_TRANSFER_ITEM:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_NOT_TRANSFER_ITEM") );
				break;

			case EMFB_ITEM_TRANSFER_DONE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_ITEM_TRANSFER_DONE") );
				break;
			};
		}break;

		/*storage lock, Juver, 2018/01/14 */
	case NET_MSG_GCTRL_STORAGE_LOCK_ENABLE_FB:
		{
			GLMSG::SNETPC_STORAGE_LOCK_ENABLE_FB* pNetMsg = ( GLMSG::SNETPC_STORAGE_LOCK_ENABLE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_STORAGE_LOCK_ENABLE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_FAILED") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_NOT_CARD") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_ALREADY_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_ALREADY_ENABLED") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_KEY_EMPTY") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_KEY_SIZE") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_KEY_NOT_SAME:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_KEY_NOT_SAME") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_KEY_NUMBER_ONLY") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_ALREADY_ENABLED_DB:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_ALREADY_ENABLED_DB") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_CHAR_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_CHAR_NOT_EXIST") );
				break;

			case EMFB_STORAGE_LOCK_ENABLE_DONE:
				{
					m_bEnableStorageLock = pNetMsg->bStorageLockEnable;
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_ENABLE_DONE") );
				}break;
			};
		}break;

		/*storage lock, Juver, 2018/01/16 */
	case NET_MSG_GCTRL_STORAGE_LOCK_INPUT_FB:
		{
			GLMSG::SNETPC_STORAGE_LOCK_INPUT_FB* pNetMsg = ( GLMSG::SNETPC_STORAGE_LOCK_INPUT_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_STORAGE_LOCK_INPUT_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_FAILED") );
				break;

			case EMFB_STORAGE_LOCK_INPUT_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_NOT_ENABLED") );
				break;

			case EMFB_STORAGE_LOCK_INPUT_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_KEY_EMPTY") );
				break;

			case EMFB_STORAGE_LOCK_INPUT_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_KEY_SIZE") );
				break;

			case EMFB_STORAGE_LOCK_INPUT_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_KEY_NUMBER_ONLY") );
				break;

			case EMFB_STORAGE_LOCK_INPUT_DATA_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_DATA_NOT_EXIST") );
				break;

			case EMFB_STORAGE_LOCK_INPUT_TIMER:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_TIMER") );
				break;

			case EMFB_STORAGE_LOCK_INPUT_DONE:
				{
					m_bStorageLockStatus = pNetMsg->bStorageLockStatus;
					
					if ( m_bStorageLockStatus )
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_DONE1") );
					else
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_INPUT_DONE2") );
				}break;
			};
		}break;

		/*storage lock, Juver, 2018/01/16 */
	case NET_MSG_GCTRL_STORAGE_LOCK_RECOVER_FB:
		{
			GLMSG::SNETPC_STORAGE_LOCK_RECOVER_FB* pNetMsg = ( GLMSG::SNETPC_STORAGE_LOCK_RECOVER_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_STORAGE_LOCK_RECOVER_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_FAILED") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_NOT_ENABLED") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_NOT_CARD") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_NOT_EXIST") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_DONE:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_DONE") );
					CInnerInterface::GetInstance().OpenStorageLockRecover( pNetMsg->szName, pNetMsg->szPin );
				}break;
			};
		}break;

		/*storage lock, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_STORAGE_LOCK_RECOVER_CHANGE_FB:
		{
			GLMSG::SNETPC_STORAGE_LOCK_RECOVER_CHANGE_FB* pNetMsg = ( GLMSG::SNETPC_STORAGE_LOCK_RECOVER_CHANGE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_FAILED") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_EMPTY") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_SIZE") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_NOT_SAME:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_NOT_SAME") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_KEY_NUMBER_ONLY") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_NOT_ENABLED") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_NOT_FOUND:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_NOT_FOUND") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_CHANGE_DONE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_CHANGE_DONE") );
				break;
			};
		}break;

		/*storage lock, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_STORAGE_LOCK_RECOVER_DELETE_FB:
		{
			GLMSG::SNETPC_STORAGE_LOCK_RECOVER_DELETE_FB* pNetMsg = ( GLMSG::SNETPC_STORAGE_LOCK_RECOVER_DELETE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_STORAGE_LOCK_RECOVER_DELETE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_DELETE_FAILED") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_DELETE_NOT_FOUND:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_DELETE_NOT_FOUND") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_DELETE_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_DELETE_NOT_ENABLED") );
				break;

			case EMFB_STORAGE_LOCK_RECOVER_DELETE_DONE:
				{
					m_bEnableStorageLock = pNetMsg->bStorageLockEnable;
					m_bStorageLockStatus = pNetMsg->bStorageLockStatus;

					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_STORAGE_LOCK_RECOVER_DELETE_DONE") );
				}break;
			};
		}break;

		/*inventory lock, Juver, 2018/01/14 */
	case NET_MSG_GCTRL_INVENTORY_LOCK_ENABLE_FB:
		{
			GLMSG::SNETPC_INVENTORY_LOCK_ENABLE_FB* pNetMsg = ( GLMSG::SNETPC_INVENTORY_LOCK_ENABLE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_INVENTORY_LOCK_ENABLE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_FAILED") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_NOT_CARD") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_ALREADY_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_ALREADY_ENABLED") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_KEY_EMPTY") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_KEY_SIZE") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_KEY_NOT_SAME:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_KEY_NOT_SAME") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_KEY_NUMBER_ONLY") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_ALREADY_ENABLED_DB:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_ALREADY_ENABLED_DB") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_CHAR_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_CHAR_NOT_EXIST") );
				break;

			case EMFB_INVENTORY_LOCK_ENABLE_DONE:
				{
					m_bEnableInventoryLock = pNetMsg->bInventoryLockEnable;
					CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_ENABLE_DONE") );
				}break;
			};
		}break;

		/*inventory lock, Juver, 2018/01/16 */
	case NET_MSG_GCTRL_INVENTORY_LOCK_INPUT_FB:
		{
			GLMSG::SNETPC_INVENTORY_LOCK_INPUT_FB* pNetMsg = ( GLMSG::SNETPC_INVENTORY_LOCK_INPUT_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_INVENTORY_LOCK_INPUT_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_FAILED") );
				break;

			case EMFB_INVENTORY_LOCK_INPUT_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_NOT_ENABLED") );
				break;

			case EMFB_INVENTORY_LOCK_INPUT_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_KEY_EMPTY") );
				break;

			case EMFB_INVENTORY_LOCK_INPUT_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_KEY_SIZE") );
				break;

			case EMFB_INVENTORY_LOCK_INPUT_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_KEY_NUMBER_ONLY") );
				break;

			case EMFB_INVENTORY_LOCK_INPUT_DATA_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_DATA_NOT_EXIST") );
				break;

			case EMFB_INVENTORY_LOCK_INPUT_TIMER:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_TIMER") );
				break;

			case EMFB_INVENTORY_LOCK_INPUT_DONE:
				{
					m_bInventoryLockStatus = pNetMsg->bInventoryLockStatus;
					
					if ( m_bInventoryLockStatus )
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_DONE1") );
					else
						CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_INPUT_DONE2") );
				}break;
			};
		}break;

		/*inventory lock, Juver, 2018/01/16 */
	case NET_MSG_GCTRL_INVENTORY_LOCK_RECOVER_FB:
		{
			GLMSG::SNETPC_INVENTORY_LOCK_RECOVER_FB* pNetMsg = ( GLMSG::SNETPC_INVENTORY_LOCK_RECOVER_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_INVENTORY_LOCK_RECOVER_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_FAILED") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_NOT_ENABLED") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_NOT_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_NOT_CARD") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_NOT_EXIST:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_NOT_EXIST") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_DONE:
				{
					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_DONE") );
					CInnerInterface::GetInstance().OpenInventoryLockRecover( pNetMsg->szName, pNetMsg->szPin );
				}break;
			};
		}break;

		/*inventory lock, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_INVENTORY_LOCK_RECOVER_CHANGE_FB:
		{
			GLMSG::SNETPC_INVENTORY_LOCK_RECOVER_CHANGE_FB* pNetMsg = ( GLMSG::SNETPC_INVENTORY_LOCK_RECOVER_CHANGE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_FAILED") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_EMPTY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_EMPTY") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_SIZE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_SIZE") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_NOT_SAME:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_NOT_SAME") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_NUMBER_ONLY:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_KEY_NUMBER_ONLY") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_NOT_ENABLED") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_NOT_FOUND:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_NOT_FOUND") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_CHANGE_DONE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_CHANGE_DONE") );
				break;
			};
		}break;

		/*inventory lock, Juver, 2018/01/18 */
	case NET_MSG_GCTRL_INVENTORY_LOCK_RECOVER_DELETE_FB:
		{
			GLMSG::SNETPC_INVENTORY_LOCK_RECOVER_DELETE_FB* pNetMsg = ( GLMSG::SNETPC_INVENTORY_LOCK_RECOVER_DELETE_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMFB_INVENTORY_LOCK_RECOVER_DELETE_FAILED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_DELETE_FAILED") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_DELETE_NOT_FOUND:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_DELETE_NOT_FOUND") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_DELETE_NOT_ENABLED:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_DELETE_NOT_ENABLED") );
				break;

			case EMFB_INVENTORY_LOCK_RECOVER_DELETE_DONE:
				{
					m_bEnableInventoryLock = pNetMsg->bInventoryLockEnable;
					m_bInventoryLockStatus = pNetMsg->bInventoryLockStatus;

					CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMFB_INVENTORY_LOCK_RECOVER_DELETE_DONE") );
				}break;
			};
		}break;

		/*pvp capture the flag, Juver, 2018/01/30 */
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_PLAYER_TEAM_BRD:
		{
			GLMSG::SNETPC_CAPTURE_THE_FLAG_F2C_PLAYER_TEAM_BRD* pNetMsg = ( GLMSG::SNETPC_CAPTURE_THE_FLAG_F2C_PLAYER_TEAM_BRD* ) nmg;
			m_wCaptureTheFlagTeam = pNetMsg->wTeam;

			if ( m_wCaptureTheFlagTeam < CAPTURE_THE_FLAG_TEAM_SIZE )
				CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEWORD("PVP_CAPTURE_THE_FLAG_TEAM_TEXT", m_wCaptureTheFlagTeam ) );
		}break;

		/*pvp capture the flag, Juver, 2018/02/07 */
	case NET_MSG_GCTRL_CAPTURE_THE_FLAG_F2C_FLAG_HOLD_BRD:
		{
			GLMSG::SNETPC_CAPTURE_THE_FLAG_F2C_PLAYER_FLAG_HOLD* pNetMsg = ( GLMSG::SNETPC_CAPTURE_THE_FLAG_F2C_PLAYER_FLAG_HOLD* ) nmg;
			m_bCaptureTheFlagHoldFlag = pNetMsg->bHold;
			m_fCaptureTheFlagHoldFlagTimer = CAPTURE_THE_FLAG_HOLD_MAX_TIME;

			D3DXMATRIX matEffect;
			D3DXMatrixTranslation ( &matEffect, m_vPos.x, m_vPos.y, m_vPos.z );

			for ( int i=0; i< CAPTURE_THE_FLAG_TEAM_SIZE; ++ i )
				DxEffGroupPlayer::GetInstance().DeletePassiveEffect( GLCONST_CHAR::strCaptureTheFlagEffect[i].c_str(), STARGETID( CROW_PC, m_dwGaeaID, m_vPos ) );

			if ( m_wCaptureTheFlagTeam < CAPTURE_THE_FLAG_TEAM_SIZE && m_bCaptureTheFlagHoldFlag )
			{
				if ( m_wCaptureTheFlagTeam == CAPTURE_THE_FLAG_TEAM_A )
					DxEffGroupPlayer::GetInstance().PassiveEffect( GLCONST_CHAR::strCaptureTheFlagEffect[CAPTURE_THE_FLAG_TEAM_B].c_str(), matEffect, STARGETID( CROW_PC, m_dwGaeaID, m_vPos ) );
				else if (m_wCaptureTheFlagTeam == CAPTURE_THE_FLAG_TEAM_B )
					DxEffGroupPlayer::GetInstance().PassiveEffect( GLCONST_CHAR::strCaptureTheFlagEffect[CAPTURE_THE_FLAG_TEAM_A].c_str(), matEffect, STARGETID( CROW_PC, m_dwGaeaID, m_vPos ) );
			}

			if ( m_bCaptureTheFlagHoldFlag )
				CInnerInterface::GetInstance().ShowGroupBottom( PVP_CAPTURE_THE_FLAG_HOLD_ICON );
			else
				CInnerInterface::GetInstance().HideGroup( PVP_CAPTURE_THE_FLAG_HOLD_ICON );
		}break;

		/* car, cart color, Juver, 2018/02/14 */
	case NET_MSG_VEHICLE_REQ_CHANGE_CAR_COLOR_FB:
		{
			GLMSG::SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_FB* pNetMsg = ( GLMSG::SNET_VEHICLE_REQ_CHANGE_CAR_COLOR_FB* ) nmg;

			switch( pNetMsg->emFB )
			{
			case EMREQ_CHANGE_CAR_COLOR_FB_CHANGE_FAIL:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_CHANGE_FAIL") );
				break;

			case EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_INVALID_CARD") );
				break;

			case EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_INACTIVE:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_INACTIVE") );
				break;

			case EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_NOT_CAR:
				CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_VEHICLE_NOT_CAR") );
				break;

			case EMREQ_CHANGE_CAR_COLOR_FB_CHANGE_DONE:
				{
					VEHICLEITEMINFO_MAP_ITER iter = m_mapVEHICLEItemInfo.find ( pNetMsg->dwVehicleID );
					if ( iter!=m_mapVEHICLEItemInfo.end() )
					{
						SVEHICLEITEMINFO& sVehicle = (*iter).second;
						sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A1] = pNetMsg->wColorA;
						sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A2] = pNetMsg->wColorB;
					}

					if ( m_sVehicle.m_dwGUID == pNetMsg->dwVehicleID )
					{
						m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A1] = pNetMsg->wColorA;
						m_sVehicle.m_wColor[BIKE_COLOR_SLOT_PART_A2] = pNetMsg->wColorB;
						UpdateSuit();

						CInnerInterface::GetInstance().PrintMsgText (  NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_CHANGE_CAR_COLOR_FB_CHANGE_DONE") );
					}
				}break;
			};
		}break;
	
	//Teleport Skill Effect
	case NET_MSG_GCTRL_TELEPORT_BRD:
		{
			GLMSG::SNET_TELEPORT_BRD *pNetMsg = (GLMSG::SNET_TELEPORT_BRD *)nmg;
			const D3DXVECTOR3 &vMovePos = pNetMsg->vMovePos;

			SetPosition(pNetMsg->vMovePos);
		}
		break;

	default:
		CDebugSet::ToListView ( "GLCharacter::MsgProcess() msg leak TYPE[%d]", nmg->nType );
		break;
	};


}


void GLCharacter::MsgDefenseSkillActive( GLMSG::SNETPC_DEFENSE_SKILL_ACTIVE* nmg )
{
	BOOL bMove(FALSE);
	D3DXVECTOR3 vMoveTo;

	PGLSKILL pRunSkill = GLSkillMan::GetInstance().GetData ( nmg->sNativeID );
	if ( !pRunSkill )										return;

	GLCOPY* pCOPY = GLGaeaClient::GetInstance().GetCopyActor ( nmg->emTarCrow, nmg->dwTarID );
	if ( !pCOPY )	return;

	if ( IsACTION(GLAT_SKILL) && IsDefenseSkill() )
	{
		return;
	}
	
	// Ω∫≈≥ ƒµΩΩ ∏ﬁºº¡ˆ
	if ( IsACTION(GLAT_SKILL) )
	{
		TurnAction(GLAT_IDLE);
		GLMSG::SNETPC_SKILL_CANCEL NetMsg;
		NETSENDTOFIELD ( &NetMsg );
	}

	
	STARGETID	sTID( nmg->emTarCrow, nmg->dwTarID);
	sTID.vPos = pCOPY->GetPosition();

	SetDefenseSkill( true );

	m_sActiveSkill = nmg->sNativeID;
	SkillReaction ( sTID, DXKEY_UP, false, bMove, vMoveTo );

	//	Note : Reaction ø°º≠ ¿Ãµø¿ª ø‰√ª«— ∞ÊøÏ.
	//
	if ( bMove )
	{
		ActionMoveTo ( 0.0f, vMoveTo+D3DXVECTOR3(0,+5,0), vMoveTo+D3DXVECTOR3(0,-5,0), FALSE, TRUE );
	}
}

