#include "stdafx.h"

#include "DxInputDevice.h"
#include "editmeshs.h"
#include "DxMethods.h"
#include "DxViewPort.h"
#include "DxEffectMan.h"
#include "DxShadowMap.h"
#include "EditMeshs.h"
#include "GLogicData.h"
#include "GLItemMan.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "stl_Func.h"
#include "DxEffGroupPlayer.h"
#include "GLPartyClient.h"
#include "GLBusStation.h"
#include "GLBusData.h"
#include "GLTaxiStation.h"
#include "RANPARAM.h"
#include "GLMaplist.h"
#include "GLFriendClient.h"
#include "RanFilter.h"
#include "DxClubMan.h"
#include "GLFactEffect.h"
#include "GLQUEST.h"
#include "GLQUESTMAN.h"
#include "GLCharacter.h"
#include "GLItemMixMan.h"
#include "GLItem.h"//20141201

#include "UITextControl.h"
#include "GameTextControl.h"
#include "InnerInterface.h"
#include "BasicGameMenu.h"
#include "QBoxButton.h"
#include "ModalWindow.h"
#include "ModalCallerID.h"
#include "ItemShopIconMan.h"
#include "PetskinMixImage.h"
#include "LargeMapWindow.h"

#include "StringUtils.h"

#include "GLSkillLearn.h"

//#include "./ObserverNotifyID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//gm¥þ§Þ¯à
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


//¸Ë³Æ¤É¯Å
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
		// ¼Õ¿¡ µç ¾ÆÀÌÅÛÀÌ ÀåÂø ¾ÆÀÌÅÛÀÌ¾î¾ß ¹Ù²Ü ¼ö ÀÖ´Ù
		NetMsg.wPosX = m_sPreInventoryItem.wPosX;
		NetMsg.wPosY = m_sPreInventoryItem.wPosY;
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

//µ¥¯Å¤É¯Å¥d
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