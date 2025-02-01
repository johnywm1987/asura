#include "StdAfx.h"
#include "BossListSlot.h"

#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "BasicTextButton.h"
#include "ItemImage.h"

#include "UITextControl.h"
#include "GameTextControl.h"
#include "GLGaeaClient.h"
#include "InnerInterface.h"

#include "../Lib_Engine/DxCommon/d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBossListSlot::CBossListSlot()
	:m_pTextName(NULL)
	, m_pTextBossStatus(NULL)
	, m_fTimer(0.0f)
	, m_fUpdateTime(0.0f)
	, m_sMobID(NATIVEID_NULL())
{
}

CBossListSlot::~CBossListSlot()
{
}

void CBossListSlot::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG);
	CBasicLineBox* pBasicLineBoxBlackBossSlot = new CBasicLineBox;
	pBasicLineBoxBlackBossSlot->CreateSub(this, "BASIC_LINE_BOX_BODY_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pBasicLineBoxBlackBossSlot->CreateBaseBoxQuestList("BOSS_LIST_NODE_BACK_WHITE");
	RegisterControl(pBasicLineBoxBlackBossSlot);

	CBasicLineBox* pBasicLineBoxWhiteResult = new CBasicLineBox;
	pBasicLineBoxWhiteResult->CreateSub(this, "BASIC_LINE_BOX_BODY_OUTER_BLANK_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pBasicLineBoxWhiteResult->CreateBaseBoxWhiteBlankBody("BOSS_LIST_BOX_QUEST_LIST");
	RegisterControl(pBasicLineBoxWhiteResult);

	m_pTextName = new CBasicTextBox;
	m_pTextName->CreateSub(this, "BOSS_DETAILS_ITEM_LIST_NODE_NAME");
	m_pTextName->SetFont(pFont8);
	m_pTextName->SetTextAlign(TEXT_ALIGN_LEFT);
	m_pTextName->SetVisibleSingle(FALSE);
	RegisterControl(m_pTextName);

	m_pTextBossStatus = new CBasicTextBox;
	m_pTextBossStatus->CreateSub(this, "BOSS_DETAILS_ITEM_LIST_NODE_STATUS");
	m_pTextBossStatus->SetFont(pFont8);
	m_pTextBossStatus->SetTextAlign(TEXT_ALIGN_LEFT);
	m_pTextBossStatus->SetVisibleSingle(FALSE);
	RegisterControl(m_pTextBossStatus);
}

void CBossListSlot::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIGroup::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	};
}

void CBossListSlot::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	m_fUpdateTime += fElapsedTime;


	if (m_fTimer > 0.0f && m_fUpdateTime >= 0.5f) m_fTimer -= fElapsedTime;

	if (m_fUpdateTime >= 1.0f)
	{
		m_fUpdateTime = 0.0f;
		UpdateInfo();
	}

}

void CBossListSlot::UpdateInfo()
{
	if (m_pTextBossStatus)
	{

		if (m_fTimer <= 0.0f || m_bIsALive)
		{
			m_pTextBossStatus->SetText("ALIVE/¤w´_¬¡", NS_UITEXTCOLOR::WHITE);
		}
		else
		{
			CString strCombine;
			strCombine.Format("%02d:%02d:%02d", (DWORD)m_fTimer / 3600, ((DWORD)m_fTimer / 60) % 60, (DWORD)m_fTimer % 60);
			m_pTextBossStatus->SetText(strCombine.GetString(), NS_UITEXTCOLOR::WHITE);
		}
	}
}

void CBossListSlot::ItemSet(CString szBossName, float fTimer, BOOL isAlive)
{
	ItemReset();

	m_fTimer = fTimer;
	m_bIsALive = isAlive;

	if (m_pTextName)
	{
		m_pTextName->SetText(szBossName, NS_UITEXTCOLOR::BRIGHTGREEN);
		m_pTextName->SetVisibleSingle(TRUE);
	}

	if (m_pTextBossStatus)
	{
		m_pTextBossStatus->SetVisibleSingle(TRUE);
	}
}

void CBossListSlot::ItemReset()
{
	m_fTimer = 0.0f;
	m_bIsALive = FALSE;

	if (m_pTextName)
	{
		m_pTextName->ClearText();
		m_pTextName->SetVisibleSingle(FALSE);
	}
	if (m_pTextBossStatus)
	{
		m_pTextBossStatus->ClearText();
		m_pTextBossStatus->SetVisibleSingle(FALSE);
	}
}