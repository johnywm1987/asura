#include "stdafx.h"
#include "./GLChar.h"
#include "./GLogicEx.h"
#include "./GLGaeaServer.h"
#include "./GLItemLMT.h"
#include "./GLItemMan.h"
#include "GLogicData.h"
#include "GLCharacter.h"
#include "../Lib_Engine/COMMON/StringUtils.h"
#include "RanFilter.h"
#include "./GLItemMixMan.h"
#include "./GLogicEx.h"

#include "../Interface/GameTextControl.h"
#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/ItemRebuildZbsj.h"
#include "./GLAgentServer.h"
#include "../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"

#include "DxLobyStage.h"
#include "DxGlobalStage.h"

#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//裝備升級
HRESULT GLCharacter::ReqZbsjOpen()	// ITEMREBUILD_MARK
{
	InitZbsjData();
	OpenZbsj();

	GLMSG::SNET_ZBSJ_RESULT NetMsg;

	NetMsg.emResult = EMREBUILD_ZBSJ_OPEN;

	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqZbsjMoveItem()
{
	GLMSG::SNET_ZBSJ_MOVE_ITEM NetMsg;

	if (m_sPreInventoryItem.VALID())
	{
		// 槳縑 萇 嬴檜蠱檜 濰雜 嬴檜蠱檜橫撿 夥羚 熱 氈棻
		SITEMCUSTOM sPreItem = GET_PREHOLD_ITEM();
		SITEM* pItem = GLItemMan::GetInstance().GetItem(sPreItem.sNativeID);
		if (pItem && pItem->sBasicOp.emItemType == ITEM_SUIT || pItem && pItem->sBasicOp.emItemType == ITEM_VEHICLE)
		{
			NetMsg.wPosX = m_sPreInventoryItem.wPosX;
			NetMsg.wPosY = m_sPreInventoryItem.wPosY;

		}

		m_sPreInventoryItem.RESET();
	}

	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}

HRESULT GLCharacter::ReqZbsjResult(int nIndex)
{
	if (!m_sZbsjItem.VALID())
		return S_FALSE;

	GLMSG::SNET_ZBSJ_RESULT NetMsg;

	switch (nIndex)
	{
	case 0:
	{
		NetMsg.emResult = EMREBUILD_ZBSJ_DAMAGE;
		NETSENDTOFIELD(&NetMsg);
	}
	break;
	case 1:
	{
		NetMsg.emResult = EMREBUILD_ZBSJ_DEFENSE;
		NETSENDTOFIELD(&NetMsg);
	}
	break;
	case 2:
	{
		NetMsg.emResult = EMREBUILD_ZBSJ_TYPE1;
		NETSENDTOFIELD(&NetMsg);
	}
	break;
	case 3:
	{
		NetMsg.emResult = EMREBUILD_ZBSJ_TYPE2;
		NETSENDTOFIELD(&NetMsg);
	}
	break;
	case 4:
	{
		NetMsg.emResult = EMREBUILD_ZBSJ_TYPE3;
		NETSENDTOFIELD(&NetMsg);
	}
	break;
	case 5:
	{
		NetMsg.emResult = EMREBUILD_ZBSJ_TYPE4;
		NETSENDTOFIELD(&NetMsg);
	}
	break;
	}

	return S_OK;
}
//等級升級卡
HRESULT GLCharacter::ReqUpgradeCard(WORD wPosX, WORD wPosY)
{
	SINVENITEM* pInvenItem = m_cInventory.FindPosItem(wPosX, wPosY);
	if (!pInvenItem)	return E_FAIL;

	wPosX = pInvenItem->wPosX;
	wPosY = pInvenItem->wPosY;

	SITEM* pItem = GLItemMan::GetInstance().GetItem(pInvenItem->sItemCustom.sNativeID);
	if (!pItem)
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_UPLEVEL_FB_NOITEM"));
		return E_FAIL;
	}

	if (m_wLevel >= pItem->sDrugOp.wCureVolume)
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_UPLEVEL_FB_MAX"));
		return E_FAIL;
	}

	GLMSG::SNET_INVEN_UPGRADE_CARD NetMsg;
	NetMsg.wPosX = wPosX;
	NetMsg.wPosY = wPosY;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}
HRESULT GLCharacter::ReqZbsjClose()
{
	InitZbsjData();

	m_sZbsjItem.RESET();

	CloseZbsj();

	GLMSG::SNET_ZBSJ_RESULT NetMsg;

	NetMsg.emResult = EMREBUILD_ZBSJ_CLOSE;

	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}
//END
//gm全技能
HRESULT GLCharacter::ReqGMSkillAll(WORD wLevel, DWORD dwPass)
{
	if (dwPass != 1)
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMSKILL_PASS"));
		return E_FAIL;
	}

	for (WORD MID = 0; MID < EMSKILLCLASS_NSIZE; ++MID)
	{
		for (WORD SID = 0; SID < GLSkillMan::MAX_CLASSSKILL; ++SID)
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData(MID, SID);
			if (pSkill)
			{
				if ((pSkill->m_sLEARN.dwCLASS & m_emClass))
				{
					if (wLevel + 1 >= SKILL::MAX_LEVEL)
					{
						wLevel = SKILL::MAX_LEVEL - 1;
					}

					if (wLevel >= pSkill->m_sBASIC.dwMAXLEVEL)
					{
						wLevel = pSkill->m_sBASIC.dwMAXLEVEL;
					}

					GLMSG::SNETPC_REQ_LEARNSKILL_GM NetMsg;
					//NetMsg.sSkillID = pSkill->m_sBASIC.sNATIVEID;
					NetMsg.wLevel = wLevel;
					NetMsg.wPass = dwPass;
					NETSENDTOFIELD(&NetMsg);
				}
			}
		}
	}

	return S_OK;
}
HRESULT GLCharacter::ReqResetSkillStatsTo()
{
	GLMSG::SNET_INVEN_RESET_SKST_TO NetMsg;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}
HRESULT GLCharacter::ReqResetRedStatsTo()
{
	GLMSG::SNET_INVEN_RESET_SKST_TO1 NetMsg;
	NETSENDTOFIELD(&NetMsg);

	return S_OK;
}
//裝備升級
HRESULT GLChar::MsgReqZbsjResult(NET_MSG_GENERIC* nmg)	// ITEMREBUILD_MARK
{
	GLMSG::SNET_ZBSJ_RESULT* pNetMsg = (GLMSG::SNET_ZBSJ_RESULT*)nmg;

	switch (pNetMsg->emResult)
	{
	case EMREBUILD_ZBSJ_OPEN:		// 偃褻 檣攪檜蝶 翮晦
	{
		InitZbsjData();
		OpenZbsj();
	}
	break;

	case EMREBUILD_ZBSJ_DAMAGE:	//攻擊研磨
	{
		SINVENITEM* pInvenItem = m_cInventory.GetItem(m_sZbsjItem.wPosX, m_sZbsjItem.wPosY);

		for (int i = 0; i < 10; ++i)
		{
			if (pInvenItem->sItemCustom.sNativeID == GLCONST_CHAR::dwZbsjRandomItem[i].sNID)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_ITEM;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}
		}

		float fRate = (float)GLCONST_CHAR::dwZbsj[0].m_dwRate;
		if (!RANDOM_GEN(fRate))
		{
			/*m_lnVipMoney -= GLCONST_CHAR::dwZbsj[0].m_fPoint;

			if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*NetMsgMoney.lnVoteP = m_lnVoteP;
			NetMsgMoney.lnPremP = m_lnPremP;
			NetMsgMoney.lnVipMoney = m_lnVipMoney;*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			pNetMsg->emResult = EMREBUILD_ZBSJ_ITEMRATE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		/*if (m_lnVipMoney < GLCONST_CHAR::dwZbsj[0].m_fPoint)
		{
			pNetMsg->emResult = EMREBUILD_ZBSJ_MONEY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}*/

		if (pInvenItem->sItemCustom.cDAMAGE >= GLCONST_CHAR::dwZbsj[0].m_dwMax)
		{
			pNetMsg->emResult = EMREBUILD_ZBSJ_MAX;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		if (pInvenItem)
		{

			m_bMoneyUpdate = TRUE;
			//m_lnVipMoney -= GLCONST_CHAR::dwZbsj[0].m_fPoint;

			pInvenItem->sItemCustom.cDAMAGE += GLCONST_CHAR::dwZbsj[0].m_fValue;

			pInvenItem->sItemCustom.cGenType = EMGEN_GMEDIT;
			pInvenItem->sItemCustom.cFieldID = (BYTE)0xff;

			if (GLGaeaServer::GetInstance().GetDBMan())
			{
				//CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				//GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*	NetMsgMoney.lnVoteP = m_lnVoteP;
				NetMsgMoney.lnPremP = m_lnPremP;
				NetMsgMoney.lnVipMoney = m_lnVipMoney;*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			// 偃褻 旎擋 煎斜 盪濰
			GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, -m_i64ZbsjCost, EMITEM_ROUTE_NPCREMAKE);
			//GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVipMoney, EMITEM_ROUTE_CHAR);

			GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
			NetItemUpdate.wPosX = m_sZbsjItem.wPosX;
			NetItemUpdate.wPosY = m_sZbsjItem.wPosY;
			NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetItemUpdate);

			pNetMsg->emResult = EMREBUILD_ZBSJ_DAMAGE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);

			SITEM* pItemData = GLItemMan::GetInstance().GetItem(pInvenItem->sItemCustom.sNativeID);

			std::string strTIME = CDebugSet::GetCurTime();
			/*CDebugSet::ToFile("ITEM_ZBSJ.txt", "%s [CharID: %d][CharName:%s][Point:%I64d][MainID:%d SubID:%d][Name:%s][TurnNum:%d][Damage:%d Defense:%d][TYPE1:%d 1:%d TYPE2:%d 2:%d TYPE3:%d 3:%d TYPE4:%d 4:%d]",
				strTIME.c_str(), m_dwCharID, m_szName, m_lnVipMoney, pInvenItem->sItemCustom.sNativeID.wMainID, pInvenItem->sItemCustom.sNativeID.wSubID, pItemData->GetName(), pInvenItem->sItemCustom.wTurnNum, pInvenItem->sItemCustom.cDAMAGE, pInvenItem->sItemCustom.cDEFENSE, pInvenItem->sItemCustom.cOptTYPE1, pInvenItem->sItemCustom.nOptVALUE1,
				pInvenItem->sItemCustom.cOptTYPE2, pInvenItem->sItemCustom.nOptVALUE2, pInvenItem->sItemCustom.cOptTYPE3, pInvenItem->sItemCustom.nOptVALUE3, pInvenItem->sItemCustom.cOptTYPE4, pInvenItem->sItemCustom.nOptVALUE4);
				*/
		}
		InitZbsjData();
	}
	break;

	case EMREBUILD_ZBSJ_DEFENSE:	//防禦研磨
	{
		SINVENITEM* pInvenItem = m_cInventory.GetItem(m_sZbsjItem.wPosX, m_sZbsjItem.wPosY);

		for (int i = 0; i < 10; ++i)
		{
			if (pInvenItem->sItemCustom.sNativeID == GLCONST_CHAR::dwZbsjRandomItem[i].sNID)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_ITEM;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}
		}

		/*if (m_lnVipMoney < GLCONST_CHAR::dwZbsj[1].m_fPoint)
		{
			pNetMsg->emResult = EMREBUILD_ZBSJ_MONEY;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}*/

		if (pInvenItem->sItemCustom.cDEFENSE >= GLCONST_CHAR::dwZbsj[1].m_dwMax)
		{
			pNetMsg->emResult = EMREBUILD_ZBSJ_MAX;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		float fRate = (float)GLCONST_CHAR::dwZbsj[1].m_dwRate;
		if (!RANDOM_GEN(fRate))
		{
			//m_lnVipMoney -= GLCONST_CHAR::dwZbsj[1].m_fPoint;

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			//NetMsgMoney.lnVoteP = m_lnVoteP;
			//NetMsgMoney.lnPremP = m_lnPremP;
			//NetMsgMoney.lnVipMoney = m_lnVipMoney;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			pNetMsg->emResult = EMREBUILD_ZBSJ_ITEMRATE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		if (pInvenItem)
		{


			//CheckMoneyUpdate(m_lnVipMoney, GLCONST_CHAR::dwZbsj[1].m_fPoint, FALSE, "Rebuild Cost.");
			m_bMoneyUpdate = TRUE;

			//m_lnVipMoney -= GLCONST_CHAR::dwZbsj[1].m_fPoint;

			pInvenItem->sItemCustom.cDEFENSE += GLCONST_CHAR::dwZbsj[1].m_fValue;

			pInvenItem->sItemCustom.cGenType = EMGEN_GMEDIT;
			pInvenItem->sItemCustom.cFieldID = (BYTE)0xff;

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*NetMsgMoney.lnVoteP = m_lnVoteP;
			NetMsgMoney.lnPremP = m_lnPremP;
			NetMsgMoney.lnVipMoney = m_lnVipMoney;*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			// 偃褻 旎擋 煎斜 盪濰
			GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, -m_i64ZbsjCost, EMITEM_ROUTE_NPCREMAKE);
			//GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVipMoney, EMITEM_ROUTE_CHAR);

			GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
			NetItemUpdate.wPosX = m_sZbsjItem.wPosX;
			NetItemUpdate.wPosY = m_sZbsjItem.wPosY;
			NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetItemUpdate);

			pNetMsg->emResult = EMREBUILD_ZBSJ_DEFENSE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);

			SITEM* pItemData = GLItemMan::GetInstance().GetItem(pInvenItem->sItemCustom.sNativeID);

			std::string strTIME = CDebugSet::GetCurTime();
			/*CDebugSet::ToFile("ITEM_ZBSJ.txt", "%s [CharID: %d][CharName:%s][Point:%I64d][MainID:%d SubID:%d][Name:%s][TurnNum:%d][Damage:%d Defense:%d][TYPE1:%d 1:%d TYPE2:%d 2:%d TYPE3:%d 3:%d TYPE4:%d 4:%d]",
				strTIME.c_str(), m_dwCharID, m_szName, m_lnVipMoney, pInvenItem->sItemCustom.sNativeID.wMainID, pInvenItem->sItemCustom.sNativeID.wSubID, pItemData->GetName(), pInvenItem->sItemCustom.wTurnNum, pInvenItem->sItemCustom.cDAMAGE, pInvenItem->sItemCustom.cDEFENSE, pInvenItem->sItemCustom.cOptTYPE1, pInvenItem->sItemCustom.nOptVALUE1,
				pInvenItem->sItemCustom.cOptTYPE2, pInvenItem->sItemCustom.nOptVALUE2, pInvenItem->sItemCustom.cOptTYPE3, pInvenItem->sItemCustom.nOptVALUE3, pInvenItem->sItemCustom.cOptTYPE4, pInvenItem->sItemCustom.nOptVALUE4);*/

		}
		InitZbsjData();
	}
	break;

	case EMREBUILD_ZBSJ_TYPE1:	//隨機第一行
	{
		SINVENITEM* pInvenItem = m_cInventory.GetItem(m_sZbsjItem.wPosX, m_sZbsjItem.wPosY);

		INT nRandOptType = (EMRANDOM_OPT)pInvenItem->sItemCustom.cOptTYPE1;

		float fRate = (float)GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwRate;
		if (!RANDOM_GEN(fRate))
		{
			//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			//NetMsgMoney.lnVoteP = m_lnVoteP;
			//NetMsgMoney.lnPremP = m_lnPremP;
			//NetMsgMoney.lnVipMoney = m_lnVipMoney;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			pNetMsg->emResult = EMREBUILD_ZBSJ_ITEMRATE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		if (pInvenItem)
		{
			m_bMoneyUpdate = TRUE;

			for (int i = 0; i < 10; ++i)
			{
				if (pInvenItem->sItemCustom.sNativeID == GLCONST_CHAR::dwZbsjRandomItem[i].sNID)
				{
					pNetMsg->emResult = EMREBUILD_ZBSJ_ITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
					return S_OK;
				}
			}

			if (nRandOptType == NULL)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_NO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (GLCONST_CHAR::dwZbsjRandom[nRandOptType].bLock != TRUE)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_BO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE1 + GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue > GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MAX;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE1 < 0)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_SUCCESS;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			/*if (m_lnVipMoney < GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MONEY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}*/

			if (pInvenItem->sItemCustom.nOptVALUE1 > 1)
			{
				pInvenItem->sItemCustom.nOptVALUE1 += GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue;
				//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;
				pInvenItem->sItemCustom.cGenType = EMGEN_GMEDIT;
				pInvenItem->sItemCustom.cFieldID = (BYTE)0xff;
			}

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*	NetMsgMoney.lnVoteP = m_lnVoteP;
				NetMsgMoney.lnPremP = m_lnPremP;
				NetMsgMoney.lnVipMoney = m_lnVipMoney;*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			// 偃褻 旎擋 煎斜 盪濰
			GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, -m_i64ZbsjCost, EMITEM_ROUTE_NPCREMAKE);
			//GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVipMoney, EMITEM_ROUTE_CHAR);

			GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
			NetItemUpdate.wPosX = m_sZbsjItem.wPosX;
			NetItemUpdate.wPosY = m_sZbsjItem.wPosY;
			NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetItemUpdate);

			pNetMsg->emResult = EMREBUILD_ZBSJ_TYPE1;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);

			SITEM* pItemData = GLItemMan::GetInstance().GetItem(pInvenItem->sItemCustom.sNativeID);

			std::string strTIME = CDebugSet::GetCurTime();
			/*CDebugSet::ToFile("ITEM_ZBSJ.txt", "%s [CharID: %d][CharName:%s][Point:%I64d][MainID:%d SubID:%d][Name:%s][TurnNum:%d][Damage:%d Defense:%d][TYPE1:%d 1:%d TYPE2:%d 2:%d TYPE3:%d 3:%d TYPE4:%d 4:%d]",
				strTIME.c_str(), m_dwCharID, m_szName, m_lnVipMoney, pInvenItem->sItemCustom.sNativeID.wMainID, pInvenItem->sItemCustom.sNativeID.wSubID, pItemData->GetName(), pInvenItem->sItemCustom.wTurnNum, pInvenItem->sItemCustom.cDAMAGE, pInvenItem->sItemCustom.cDEFENSE, pInvenItem->sItemCustom.cOptTYPE1, pInvenItem->sItemCustom.nOptVALUE1,
				pInvenItem->sItemCustom.cOptTYPE2, pInvenItem->sItemCustom.nOptVALUE2, pInvenItem->sItemCustom.cOptTYPE3, pInvenItem->sItemCustom.nOptVALUE3, pInvenItem->sItemCustom.cOptTYPE4, pInvenItem->sItemCustom.nOptVALUE4);*/
		}
		InitZbsjData();
	}
	break;

	case EMREBUILD_ZBSJ_TYPE2:	//隨機第二行
	{
		SINVENITEM* pInvenItem = m_cInventory.GetItem(m_sZbsjItem.wPosX, m_sZbsjItem.wPosY);

		INT nRandOptType = (EMRANDOM_OPT)pInvenItem->sItemCustom.cOptTYPE2;

		float fRate = (float)GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwRate;
		if (!RANDOM_GEN(fRate))
		{
			//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			//NetMsgMoney.lnVoteP = m_lnVoteP;
			//NetMsgMoney.lnPremP = m_lnPremP;
			//NetMsgMoney.lnVipMoney = m_lnVipMoney;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			pNetMsg->emResult = EMREBUILD_ZBSJ_ITEMRATE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		if (pInvenItem)
		{
			m_bMoneyUpdate = TRUE;

			for (int i = 0; i < 10; ++i)
			{
				if (pInvenItem->sItemCustom.sNativeID == GLCONST_CHAR::dwZbsjRandomItem[i].sNID)
				{
					pNetMsg->emResult = EMREBUILD_ZBSJ_ITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
					return S_OK;
				}
			}

			if (nRandOptType == NULL)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_NO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (GLCONST_CHAR::dwZbsjRandom[nRandOptType].bLock != TRUE)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_BO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE2 + GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue > GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MAX;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE2 < 0)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_SUCCESS;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			/*if (m_lnVipMoney < GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MONEY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}*/

			if (pInvenItem->sItemCustom.nOptVALUE2 > 1)
			{
				pInvenItem->sItemCustom.nOptVALUE2 += GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue;
				//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;
				pInvenItem->sItemCustom.cGenType = EMGEN_GMEDIT;
				pInvenItem->sItemCustom.cFieldID = (BYTE)0xff;
			}


			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*NetMsgMoney.lnVoteP = m_lnVoteP;
			NetMsgMoney.lnPremP = m_lnPremP;
			NetMsgMoney.lnVipMoney = m_lnVipMoney;*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			// 偃褻 旎擋 煎斜 盪濰
			GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, -m_i64ZbsjCost, EMITEM_ROUTE_NPCREMAKE);
			//GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVipMoney, EMITEM_ROUTE_CHAR);

			GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
			NetItemUpdate.wPosX = m_sZbsjItem.wPosX;
			NetItemUpdate.wPosY = m_sZbsjItem.wPosY;
			NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetItemUpdate);

			pNetMsg->emResult = EMREBUILD_ZBSJ_TYPE2;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);

			SITEM* pItemData = GLItemMan::GetInstance().GetItem(pInvenItem->sItemCustom.sNativeID);

			std::string strTIME = CDebugSet::GetCurTime();
			/*CDebugSet::ToFile("ITEM_ZBSJ.txt", "%s [CharID: %d][CharName:%s][Point:%I64d][MainID:%d SubID:%d][Name:%s][TurnNum:%d][Damage:%d Defense:%d][TYPE1:%d 1:%d TYPE2:%d 2:%d TYPE3:%d 3:%d TYPE4:%d 4:%d]",
				strTIME.c_str(), m_dwCharID, m_szName, m_lnVipMoney, pInvenItem->sItemCustom.sNativeID.wMainID, pInvenItem->sItemCustom.sNativeID.wSubID, pItemData->GetName(), pInvenItem->sItemCustom.wTurnNum, pInvenItem->sItemCustom.cDAMAGE, pInvenItem->sItemCustom.cDEFENSE, pInvenItem->sItemCustom.cOptTYPE1, pInvenItem->sItemCustom.nOptVALUE1,
				pInvenItem->sItemCustom.cOptTYPE2, pInvenItem->sItemCustom.nOptVALUE2, pInvenItem->sItemCustom.cOptTYPE3, pInvenItem->sItemCustom.nOptVALUE3, pInvenItem->sItemCustom.cOptTYPE4, pInvenItem->sItemCustom.nOptVALUE4);*/

		}
		InitZbsjData();
	}
	break;

	case EMREBUILD_ZBSJ_TYPE3:	//隨機第三行
	{
		SINVENITEM* pInvenItem = m_cInventory.GetItem(m_sZbsjItem.wPosX, m_sZbsjItem.wPosY);

		INT nRandOptType = (EMRANDOM_OPT)pInvenItem->sItemCustom.cOptTYPE3;

		float fRate = (float)GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwRate;
		if (!RANDOM_GEN(fRate))
		{
			//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			//NetMsgMoney.lnVoteP = m_lnVoteP;
			//NetMsgMoney.lnPremP = m_lnPremP;
			//NetMsgMoney.lnVipMoney = m_lnVipMoney;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}
*/
			pNetMsg->emResult = EMREBUILD_ZBSJ_ITEMRATE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		if (pInvenItem)
		{
			m_bMoneyUpdate = TRUE;

			for (int i = 0; i < 10; ++i)
			{
				if (pInvenItem->sItemCustom.sNativeID == GLCONST_CHAR::dwZbsjRandomItem[i].sNID)
				{
					pNetMsg->emResult = EMREBUILD_ZBSJ_ITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
					return S_OK;
				}
			}

			if (nRandOptType == NULL)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_NO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (GLCONST_CHAR::dwZbsjRandom[nRandOptType].bLock != TRUE)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_BO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE3 + GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue > GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MAX;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE3 < 0)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_SUCCESS;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			/*if (m_lnVipMoney < GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MONEY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}*/

			if (pInvenItem->sItemCustom.nOptVALUE3 > 1)
			{
				pInvenItem->sItemCustom.nOptVALUE3 += GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue;
				//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;
				pInvenItem->sItemCustom.cGenType = EMGEN_GMEDIT;
				pInvenItem->sItemCustom.cFieldID = (BYTE)0xff;
			}

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*NetMsgMoney.lnVoteP = m_lnVoteP;
			NetMsgMoney.lnPremP = m_lnPremP;
			NetMsgMoney.lnVipMoney = m_lnVipMoney;*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			// 偃褻 旎擋 煎斜 盪濰
			GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, -m_i64ZbsjCost, EMITEM_ROUTE_NPCREMAKE);
			//GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVipMoney, EMITEM_ROUTE_CHAR);

			GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
			NetItemUpdate.wPosX = m_sZbsjItem.wPosX;
			NetItemUpdate.wPosY = m_sZbsjItem.wPosY;
			NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetItemUpdate);

			pNetMsg->emResult = EMREBUILD_ZBSJ_TYPE3;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);

			SITEM* pItemData = GLItemMan::GetInstance().GetItem(pInvenItem->sItemCustom.sNativeID);

			std::string strTIME = CDebugSet::GetCurTime();
			/*CDebugSet::ToFile("ITEM_ZBSJ.txt", "%s [CharID: %d][CharName:%s][Point:%I64d][MainID:%d SubID:%d][Name:%s][TurnNum:%d][Damage:%d Defense:%d][TYPE1:%d 1:%d TYPE2:%d 2:%d TYPE3:%d 3:%d TYPE4:%d 4:%d]",
				strTIME.c_str(), m_dwCharID, m_szName, m_lnVipMoney, pInvenItem->sItemCustom.sNativeID.wMainID, pInvenItem->sItemCustom.sNativeID.wSubID, pItemData->GetName(), pInvenItem->sItemCustom.wTurnNum, pInvenItem->sItemCustom.cDAMAGE, pInvenItem->sItemCustom.cDEFENSE, pInvenItem->sItemCustom.cOptTYPE1, pInvenItem->sItemCustom.nOptVALUE1,
				pInvenItem->sItemCustom.cOptTYPE2, pInvenItem->sItemCustom.nOptVALUE2, pInvenItem->sItemCustom.cOptTYPE3, pInvenItem->sItemCustom.nOptVALUE3, pInvenItem->sItemCustom.cOptTYPE4, pInvenItem->sItemCustom.nOptVALUE4);*/

		}
		InitZbsjData();
	}
	break;
	case EMREBUILD_ZBSJ_TYPE4:	//隨機第四行
	{
		SINVENITEM* pInvenItem = m_cInventory.GetItem(m_sZbsjItem.wPosX, m_sZbsjItem.wPosY);

		INT nRandOptType = (EMRANDOM_OPT)pInvenItem->sItemCustom.cOptTYPE4;

		float fRate = (float)GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwRate;
		if (!RANDOM_GEN(fRate))
		{
			//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*	NetMsgMoney.lnVoteP = m_lnVoteP;
				NetMsgMoney.lnPremP = m_lnPremP;
				NetMsgMoney.lnVipMoney = m_lnVipMoney;
			*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			pNetMsg->emResult = EMREBUILD_ZBSJ_ITEMRATE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
			return S_OK;
		}

		if (pInvenItem)
		{
			m_bMoneyUpdate = TRUE;

			for (int i = 0; i < 10; ++i)
			{
				if (pInvenItem->sItemCustom.sNativeID == GLCONST_CHAR::dwZbsjRandomItem[i].sNID)
				{
					pNetMsg->emResult = EMREBUILD_ZBSJ_ITEM;
					GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
					return S_OK;
				}
			}

			if (nRandOptType == NULL)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_NO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (GLCONST_CHAR::dwZbsjRandom[nRandOptType].bLock != TRUE)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_BO;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE4 + GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue > GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MAX;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			if (pInvenItem->sItemCustom.nOptVALUE4 < 0)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_SUCCESS;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}

			/*if (m_lnVipMoney < GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint)
			{
				pNetMsg->emResult = EMREBUILD_ZBSJ_MONEY;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);
				return S_OK;
			}*/

			if (pInvenItem->sItemCustom.nOptVALUE4 > 1)
			{
				pInvenItem->sItemCustom.nOptVALUE4 += GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue;
				//m_lnVipMoney -= GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint;
				pInvenItem->sItemCustom.cGenType = EMGEN_GMEDIT;
				pInvenItem->sItemCustom.cFieldID = (BYTE)0xff;
			}

			GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
			NetMsgMoney.lnMoney = m_lnMoney;
			/*NetMsgMoney.lnVoteP = m_lnVoteP;
			NetMsgMoney.lnPremP = m_lnPremP;
			NetMsgMoney.lnVipMoney = m_lnVipMoney;*/
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

			/*if (GLGaeaServer::GetInstance().GetDBMan())
			{
				CItemChaPointNumIncrease* pDbAction = new CItemChaPointNumIncrease(m_dwUserID, m_lnVipMoney);
				GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDbAction);
			}*/

			// 偃褻 旎擋 煎斜 盪濰
			GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, -m_i64ZbsjCost, EMITEM_ROUTE_NPCREMAKE);
			//GLITEMLMT::GetInstance().ReqMoneyExc(ID_CHAR, m_dwCharID, ID_CHAR, 0, m_lnVipMoney, EMITEM_ROUTE_CHAR);

			GLMSG::SNET_INVEN_ITEM_UPDATE NetItemUpdate;
			NetItemUpdate.wPosX = m_sZbsjItem.wPosX;
			NetItemUpdate.wPosY = m_sZbsjItem.wPosY;
			NetItemUpdate.sItemCustom = pInvenItem->sItemCustom;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetItemUpdate);

			pNetMsg->emResult = EMREBUILD_ZBSJ_TYPE4;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, pNetMsg);

			SITEM* pItemData = GLItemMan::GetInstance().GetItem(pInvenItem->sItemCustom.sNativeID);

			std::string strTIME = CDebugSet::GetCurTime();
			/*CDebugSet::ToFile("ITEM_ZBSJ.txt", "%s [CharID: %d][CharName:%s][Point:%I64d][MainID:%d SubID:%d][Name:%s][TurnNum:%d][Damage:%d Defense:%d][TYPE1:%d 1:%d TYPE2:%d 2:%d TYPE3:%d 3:%d TYPE4:%d 4:%d]",
				strTIME.c_str(), m_dwCharID, m_szName, m_lnVipMoney, pInvenItem->sItemCustom.sNativeID.wMainID, pInvenItem->sItemCustom.sNativeID.wSubID, pItemData->GetName(), pInvenItem->sItemCustom.wTurnNum, pInvenItem->sItemCustom.cDAMAGE, pInvenItem->sItemCustom.cDEFENSE, pInvenItem->sItemCustom.cOptTYPE1, pInvenItem->sItemCustom.nOptVALUE1,
				pInvenItem->sItemCustom.cOptTYPE2, pInvenItem->sItemCustom.nOptVALUE2, pInvenItem->sItemCustom.cOptTYPE3, pInvenItem->sItemCustom.nOptVALUE3, pInvenItem->sItemCustom.cOptTYPE4, pInvenItem->sItemCustom.nOptVALUE4);*/

		}
		InitZbsjData();
	}
	break;

	case EMREBUILD_ZBSJ_FAIL:		// 檜楛 韃擎 螢 熱 橈棻
	case EMREBUILD_ZBSJ_CLOSE:	// 偃褻 檣攪檜蝶 殘晦
	{

		InitZbsjData();
		CloseZbsj();

		m_ItemZbsjPosX = EM_INVENSIZE_X;
		m_ItemZbsjPosY = EM_INVENSIZE_Y;
	}
	break;
	}

	return S_OK;
}
//END
// 
// //裝備升級
HRESULT GLChar::MsgReqZbsjMoveItem(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNET_ZBSJ_MOVE_ITEM* pNetMsg = (GLMSG::SNET_ZBSJ_MOVE_ITEM*)nmg;
	GLMSG::SNET_ZBSJ_MOVE_ITEM NetMsg;

	m_sZbsjItem.RESET();

	if (pNetMsg->wPosX != USHRT_MAX && pNetMsg->wPosY != USHRT_MAX)
	{
		SINVENITEM* pResistItem = m_cInventory.GetItem(pNetMsg->wPosX, pNetMsg->wPosY);
		if (pResistItem)
		{
			// 濰雜 嬴檜蠱檜橫撿 夥羚 熱 氈棻
			SITEM* pItem = GLItemMan::GetInstance().GetItem(pResistItem->sItemCustom.sNativeID);
			if (pItem && pItem->sBasicOp.emItemType == ITEM_SUIT || pItem && pItem->sBasicOp.emItemType == ITEM_VEHICLE)
			{
				NetMsg.wPosX = pNetMsg->wPosX;
				NetMsg.wPosY = pNetMsg->wPosY;

				m_sZbsjItem.SET(NetMsg.wPosX, NetMsg.wPosY);
			}
		}
	}
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsg);

	return S_OK;
}
//END
// //等級升級卡
HRESULT GLChar::MsgReqUpgradeCard(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNET_INVEN_UPGRADE_CARD* pNetMsg = (GLMSG::SNET_INVEN_UPGRADE_CARD*)nmg;

	GLMSG::SNET_INVEN_UPGRADE_CARD_FB MsgFB;

	SINVENITEM* pINVENITEM = m_cInventory.GetItem(pNetMsg->wPosX, pNetMsg->wPosY);
	if (!pINVENITEM)
	{
		MsgFB.emFB = EMREQ_UPGRADE_CARD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);
		return E_FAIL;
	}

	if (CheckCoolTime(pINVENITEM->sItemCustom.sNativeID))	return E_FAIL;

	SITEM* pITEM = GLItemMan::GetInstance().GetItem(pINVENITEM->sItemCustom.sNativeID);
	if (pITEM->sBasicOp.emItemType != ITEM_UPGRADE_CARD)
	{
		MsgFB.emFB = EMREQ_UPGRADE_CARD_FB_NOITEM;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);
		return E_FAIL;
	}

	WORD wLevel = m_wLevel + pITEM->sDrugOp.wCureVolume;

	if (wLevel >= GLCONST_CHAR::wMAX_LEVEL)
	{
		MsgFB.emFB = EMREQ_UPGRADE_CARD_FB_MAX;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);
		return E_FAIL;
	}

	//DWORD StatsPoint = m_wStatsPoint / GLCONST_CHAR::wLVL_STATS_P;
	//DWORD SkpPoint = m_dwSkillPoint / GLCONST_CHAR::wLVL_SKILL_P;

	//	Note : 檣漸 塭檣 隸陛.
	//
	m_wLevel += pITEM->sDrugOp.wCureVolume;
	m_wStatsPoint += pITEM->sDrugOp.wCureVolume * GLCONST_CHAR::wLVL_STATS_P;
	m_dwSkillPoint += pITEM->sDrugOp.wCureVolume * GLCONST_CHAR::wLVL_SKILL_P;

	//	[濠褐縑啪] 檣實 嬴檜蠱 模賅衛霽.
	DoDrugInvenItem(pNetMsg->wPosX, pNetMsg->wPosY);

	//	Note : 籀葬 唸婁 憲葡.
	MsgFB.emFB = EMREQ_UPGRADE_CARD_FB_OK;
	MsgFB.wLevel = m_wLevel;
	MsgFB.wStatsPoint = m_wStatsPoint;
	MsgFB.dwSkillPoint = m_dwSkillPoint;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);

	return S_OK;
}
//洗點系統
HRESULT GLChar::MsgReqInvenResetSkStTo(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNET_INVEN_RESET_SKST_TO* pNetMsg = (GLMSG::SNET_INVEN_RESET_SKST_TO*)nmg;

	GLMSG::SNET_INVEN_RESET_SKST_TO_FB	MsgFB;

	if (m_lnMoney < GLCONST_CHAR::lnMoney1)
	{
		MsgFB.emFB = EMREQ_RESET_SKST_TO_FB_MONEY;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);
		return FALSE;
	}

	m_lnMoney -= GLCONST_CHAR::lnMoney1;

	RESET_STATS_TO(65535);

	MsgFB.emFB = EMREQ_RESET_SKST_TO_FB_OK;
	MsgFB.wSTATS_POINT = 65535;
	MsgFB.lnMoney = m_lnMoney;


	GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	NetMsgMoney.lnMoney = m_lnMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);


	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);

	return S_OK;
}
//轉生系統
HRESULT GLChar::MsgReqUpRell(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPC_REQ_UPRELL* pNetMsg = (GLMSG::SNETPC_REQ_UPRELL*)nmg;
	GLMSG::SNETPC_REQ_UPRELL_FB MsgFB;
	/*優化轉生系統功能代碼 , SSODomain 28/05/2021  */
	/*
	DWORD dwRebomLbmax = GLCONST_CHAR::sREBOMRANK[0].m_dwMasterLvl;
	DWORD dwRebomLvl = GLCONST_CHAR::sREBOMRANK[0].m_dwRebomLvl;
	DWORD dwRebomL = GLCONST_CHAR::sREBOMRANK[0].m_dwRebomL;
	DWORD dwRebomPoint = GLCONST_CHAR::sREBOMRANK[0].m_dwRebomPoint;
	DWORD dwRebomSkillPoint = GLCONST_CHAR::sREBOMRANK[0].m_dwRebomSkillPoint;

	LONGLONG dwRebomMoney = GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoney;
	LONGLONG dwRebomMoneyV = GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoneyV;
	LONGLONG dwRebomMoneyP = GLCONST_CHAR::sREBOMRANK[0].m_dwRebomMoneyP;
	*/
	DWORD NeedLevel = 0;
	DWORD cost = 0;
	DWORD FreeStats = 0;
	DWORD FreeSkill = 0;
	DWORD rebornreqlev = 0;
	DWORD newborn = m_dwReborn + 1;

	if (newborn <= GLCONST_CHAR::m_dwRebom1 && newborn >= 0)
		rebornreqlev = 1;
	else if (newborn > GLCONST_CHAR::m_dwRebom1 && newborn <= GLCONST_CHAR::m_dwRebom1_2)
	{
		rebornreqlev = 2;
	}
	else if (newborn > GLCONST_CHAR::m_dwRebom1_2 && newborn <= GLCONST_CHAR::m_dwRebom1_3)
	{
		rebornreqlev = 3;
	}

	switch (rebornreqlev)
	{
	case 1:
		NeedLevel = GLCONST_CHAR::m_dwRebomLvl + m_dwReborn;
		cost = GLCONST_CHAR::m_dwRebomMoney * newborn;
		FreeStats = GLCONST_CHAR::m_dwRebomPoint * newborn;
		FreeSkill = GLCONST_CHAR::m_dwRebomSkillPoint * newborn;
		break;
	case 2:
		NeedLevel = GLCONST_CHAR::m_dwRebomLvl_2 + m_dwReborn;
		cost = GLCONST_CHAR::m_dwRebomMoney_2 * newborn;
		FreeStats = GLCONST_CHAR::m_dwRebomPoint_2 * newborn;
		FreeSkill = GLCONST_CHAR::m_dwRebomSkillPoint_2 * newborn;
		break;
	case 3:
		NeedLevel = GLCONST_CHAR::m_dwRebomLvl_3 + m_dwReborn;
		cost = GLCONST_CHAR::m_dwRebomMoney_3 * newborn;
		FreeStats = GLCONST_CHAR::m_dwRebomPoint_3 * newborn;
		FreeSkill = GLCONST_CHAR::m_dwRebomSkillPoint_3 * newborn;
		break;
	default:

		break;
	}

	if (newborn <= GLCONST_CHAR::m_dwMasterLvl)
	{
		if (m_wLevel >= NeedLevel)
		{
			if (m_lnMoney >= cost)
			{
				m_lnMoney -= cost;

				m_wLevel = 1;
				m_sStats.wDex = 0;
				m_sStats.wInt = 0;
				m_sStats.wPow = 0;
				m_sStats.wSpi = 0;
				m_sStats.wSta = 0;
				m_sStats.wStr = 0;

				m_dwReborn = newborn;
				m_wStatsPoint = FreeStats;
				m_dwSkillPoint = FreeSkill;

				std::string strTIME = CDebugSet::GetCurTime();
				CDebugSet::ToFile("CharReborn.txt", "%s CHAR REBORN [CharID: %d][CharName:%s][REB:%d][STATSPOINT:%d][SKILLPOINT:%d] ",
					strTIME.c_str(), m_dwCharID, m_szName, newborn, m_wStatsPoint, m_dwSkillPoint);

			}
			else
			{
				MsgFB.emFB = EMINVEN_RELL_CHANGE_FB_NOTREBOMPOINT;
				GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);
				return FALSE;
			}

		}
		else
		{
			MsgFB.emFB = EMINVEN_RELL_CHANGE_FB_NOTGRADE;
			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);
			return FALSE;
		}

	}
	else
	{
		MsgFB.emFB = EMINVEN_RELL_CHANGE_FB_NOTREBOMMAX;
		GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);
		return FALSE;
	}



	MsgFB.lnMoney = m_lnMoney;
	//	MsgFB.lnVoteP = m_lnVoteP;
	//	MsgFB.lnPremP = m_lnPremP;
	MsgFB.m_dwReborn = m_dwReborn;
	MsgFB.wStatsPoint = m_wStatsPoint;
	MsgFB.wLevel = m_wLevel;
	MsgFB.dwSkillPoint = m_dwSkillPoint;
	MsgFB.emFB = EMINVEN_RELL_CHANGE_FB_OK;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);

	//	[濠褐縑啪] 檣實 嬴檜齪 模賅衛霽.
	//DoDrugInvenItem ( pNetMsg->wPosX, pNetMsg->wPosY );

	// DB縑 盪濰
	CSetChaRellChange* pDBAction = new CSetChaRellChange(m_dwCharID, m_dwReborn, m_wStatsPoint, m_wLevel, m_dwSkillPoint, m_lnMoney);

	if (GLGaeaServer::GetInstance().GetDBMan())
	{
		GLGaeaServer::GetInstance().GetDBMan()->AddJob(pDBAction);
	}

	return S_OK;
}
/*/gm全技能
HRESULT GLCharacter::ReqGMSkillAll( WORD wLevel, DWORD dwPass )
{
	if ( dwPass != GLCONST_CHAR::wGMItemPass )
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_GMSKILL_PASS") );
		return E_FAIL;
	}

	for ( WORD MID=0; MID<EMSKILLCLASS_NSIZE; ++MID )
	{
		for ( WORD SID=0; SID<GLSkillMan::MAX_CLASSSKILL; ++SID )
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( MID, SID );
			if ( pSkill )
			{
				if ( (pSkill->m_sLEARN.dwCLASS&m_emClass) )
				{
					if ( wLevel+1 >= SKILL::MAX_LEVEL )
					{
						wLevel = SKILL::MAX_LEVEL -1;
					}

					if ( wLevel >= pSkill->m_sBASIC.dwMAXLEVEL )
					{
						wLevel = pSkill->m_sBASIC.dwMAXLEVEL;
					}

					GLMSG::SNETPC_REQ_LEARNSKILL_GM NetMsg;
					//NetMsg.sSkillID = pSkill->m_sBASIC.sNATIVEID;
					NetMsg.wLevel = wLevel;
					NetMsg.wPass = dwPass;
					NETSENDTOFIELD ( &NetMsg );
				}
			}
		}
	}

	return S_OK;
}*/
//洗紅系統
HRESULT GLChar::MsgReqInvenResetSkStTo1(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNET_INVEN_RESET_SKST_TO1* pNetMsg = (GLMSG::SNET_INVEN_RESET_SKST_TO1*)nmg;
	GLMSG::SNET_INVEN_RESET_SKST_TO_FB1 MsgFB;

	// 先检查金钱是否足够
	if (m_lnMoney < GLCONST_CHAR::lnMoney11)
	{
		CInnerInterface::GetInstance().PrintMsgTextDlg(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMFB_CHANGE_BRIGHT_NO_MONEY"));
		return S_FALSE;
	}

	// 检查红名值是否需要洗
	if (m_nBright > 0)
	{
		return S_FALSE;
	}

	// 只扣除一次金钱
	m_lnMoney -= GLCONST_CHAR::lnMoney11;

	// 增加红名值
	m_nBright += 10;

	// 更新红名值
	GLMSG::SNETPC_UPDATE_BRIGHT NetMsg;
	NetMsg.nBright = m_nBright;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsg);

	// 更新金钱
	GLMSG::SNETPC_UPDATE_MONEY NetMsgMoney;
	NetMsgMoney.lnMoney = m_lnMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsgMoney);

	// 更新反馈消息
	MsgFB.lnMoney = m_lnMoney;
	GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &MsgFB);

	return S_OK;
}
//遊戲幣兌換積分系統
//HRESULT GLChar::MsgReqChangeMoneyTo ( NET_MSG_GENERIC* nmg )
//{
//GLMSG::SNET_INVEN_CHANGE_MONEY_TO1 *pNetMsg = (GLMSG::SNET_INVEN_CHANGE_MONEY_TO1 *)nmg;

	//maison add change bright price
//DWORD costing =1000000;
//	GLMSG::SNET_INVEN_CHANGE_MONEY_TO_FB1	MsgFB;
//}