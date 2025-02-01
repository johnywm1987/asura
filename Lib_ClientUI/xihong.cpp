#include "StdAfx.h"
#include "xihong.h"
#include "PVPTyrannyPageBattle.h"
#include "PVPTyrannyPageHistory.h"

#include "d3dfont.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "./GLCharacter.h"
#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "BasicTextButton.h"

#include "GLPVPTyrannyClient.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "DxGlobalStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CXihong::CXihong()
	: m_pTextMapName(NULL)
	, m_pInfoBattle(NULL)
	, m_pInfoHistory(NULL)
	, m_pTextInfoTitle(NULL)
	, m_pTextInfoGoal1(NULL)
	, m_pTextInfoGoal2(NULL)
	, m_pTextInfoMatch(NULL)
	, m_pTextInfoMatch1(NULL)
	, m_pTextInfoMatch2(NULL)
	, m_pTextInfoPlayer(NULL)
	, m_pTextInfoLevel(NULL)
	, m_pTextReward(NULL)
	, m_fUpdateTime(0.0f)
	, m_pButtonClose(NULL)
	, m_pButtonReborn(NULL)
{
}

CXihong::~CXihong()
{
}

void CXihong::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);

	CreateUILineBoxQuestList("REBORN_LINEBOX_HEAD");
	CreateUILineBoxQuestList("REBORN_LINEBOX_HEAD");
	CreateUILineBoxQuestList("REBORN_LINEBOX_HEAD2");
	CreateUILineBoxQuestList("REBORN_LINEBOX_BODY");
	CreateUILineBoxQuestList("REBORN_LINEBOX_MAP");
	CreateUILineBoxQuestList("REBORN_LINEBOX_BOTTOM");

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, REBORN_WINDOW_BUTTON_CLOSE);
	m_pButtonClose->CreateBaseButton("REBORN_BUTTON_CLOSE", CBasicTextButton::SIZE14, CBasicButton::CLICK_FLIP, ID2GAMEWORD("CHARXBOX_BUTTON", 8));
	m_pButtonClose->SetFlip(TRUE);
	RegisterControl(m_pButtonClose);

	m_pButtonReborn = new CBasicTextButton;
	m_pButtonReborn->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, REBORN_WINDOW_BUTTON_REBORN);
	m_pButtonReborn->CreateBaseButton("REBORN_BUTTON", CBasicTextButton::SIZE14, CBasicButton::CLICK_FLIP, ID2GAMEWORD("CHARXBOX_BUTTON", 2));
	m_pButtonReborn->SetFlip(TRUE);
	RegisterControl(m_pButtonReborn);



	//battle info
//	CreateUIControl("COMPETITION_CTF_PAGE_INFO_MAP");
	//CreateUIControl("COMPETITION_CTF_PAGE_INFO_RACE_SCHOOL0");
	//CreateUIControl("COMPETITION_CTF_PAGE_INFO_RACE_SCHOOL1");
	//CreateUIControl("COMPETITION_CTF_PAGE_INFO_RACE_SCHOOL2");

	m_pTextMapName = CreateUITextBox("COMPETITION_CTF_PAGE_INFO_TITLE", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y);
	m_pTextMapName->AddText(ID2GAMEWORD("CHARXBOX_BUTTON", 2), NS_UITEXTCOLOR::ORANGE);
	m_pTextInfoTitle = CreateUITextBox("COMPETITION_REBORN_PAGE_INFO_DESC_TITLE", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoGoal0 = CreateUITextBox("XIDIAN_INFO_DESC_GOAL0", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoGoal1 = CreateUITextBox("XIDIAN_INFO_DESC_GOAL1", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoGoal2 = CreateUITextBox("XIDIAN_INFO_DESC_GOAL2", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch = CreateUITextBox("XIDIAN_INFO_DESC_MATCH", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch1 = CreateUITextBox("XIDIAN_INFO_DESC_LEVEL", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch2 = CreateUITextBox("XIDIAN_INFO_DESC_MONEY", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch3 = CreateUITextBox("XIDIAN_INFO_AFTER_TITLE", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch4 = CreateUITextBox("XIDIAN_INFO_AFTER_XIDIAN", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch5 = CreateUITextBox("XIDIAN_INFO_AFTER_LEVEL", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch6 = CreateUITextBox("XIDIAN_INFO_AFTER_MONEY", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch7 = CreateUITextBox("XIDIAN_INFO_AFTER_POINT", pFont9, TEXT_ALIGN_LEFT);
	m_pTextInfoMatch8 = CreateUITextBox("XIDIAN_INFO_AFTER_SKILLPOINT", pFont9, TEXT_ALIGN_LEFT);

	CString strCombine;
	CString strCombine1;
	int nIndex = 0;
	if (m_pTextInfoTitle)
	{
		m_pTextInfoTitle->ClearText();
		nIndex = m_pTextInfoTitle->AddText(ID2GAMEWORD("XIHONG_DESC", 0), NS_UITEXTCOLOR::RED);
		m_pTextInfoTitle->AddString(nIndex, strCombine.GetString(), NS_UITEXTCOLOR::RED);
	}

	if (m_pTextInfoGoal0)
	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		m_pTextInfoGoal0->ClearText();
		nIndex = m_pTextInfoGoal0->AddText(ID2GAMEINTEXT("MAX_XIHONG_LEVEL"), NS_UITEXTCOLOR::GREEN);
		strCombine.Format(ID2GAMEINTEXT("MAX_XIHONG_LEVE_REQ"), GLCONST_CHAR::m_dwMasterLvl);
		m_pTextInfoGoal0->AddString(nIndex, strCombine.GetString(), NS_UITEXTCOLOR::WHITE);

	}



	//goal
	if (m_pTextInfoGoal2)
	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		m_pTextInfoGoal2->ClearText();
		nIndex = m_pTextInfoGoal2->AddText(ID2GAMEINTEXT("XIHONG_REBORN_REQUIRE"), NS_UITEXTCOLOR::GREEN);
		strCombine.Format(ID2GAMEINTEXT("MAX_XIHONG_REBORN_REQ"));
		m_pTextInfoGoal2->AddString(nIndex, strCombine.GetString(), NS_UITEXTCOLOR::WHITE);

	}
	if (m_pTextInfoMatch)
	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		m_pTextInfoMatch->ClearText();
		nIndex = m_pTextInfoMatch->AddText(ID2GAMEINTEXT("XIHONG_BENIFIT"), NS_UITEXTCOLOR::GREEN);
		strCombine.Format(ID2GAMEINTEXT("XIHONG_BENIFIT_DES"), GLCONST_CHAR::m_dwRebomPoint, GLCONST_CHAR::m_dwRebomSkillPoint);
		m_pTextInfoMatch->AddString(nIndex, strCombine.GetString(), NS_UITEXTCOLOR::WHITE);
	}
	if (m_pTextInfoMatch1)
	{
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		m_pTextInfoMatch1->ClearText();
		int nIndex = 0;
		nIndex = m_pTextInfoMatch1->AddText(ID2GAMEINTEXT("MAX_XIHONG_MONEY"), NS_UITEXTCOLOR::GREEN);
		strCombine.Format(ID2GAMEINTEXT("MAX_XIHONG_MONEY_REQ"), GLCONST_CHAR::lnMoney1);
		m_pTextInfoMatch1->AddString(nIndex, strCombine.GetString(), NS_UITEXTCOLOR::WHITE);
	}

}

CBasicTextBox* CXihong::CreateUITextBox(const char* szControl, CD3DFontPar* pFont, int nTextAlign)
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub(this, szControl);
	pTextBox->SetFont(pFont);
	pTextBox->SetTextAlign(nTextAlign);
	RegisterControl(pTextBox);
	return pTextBox;
}

CBasicLineBox* CXihong::CreateUILineBoxQuestList(char* szBaseControl)
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub(this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pLineBox->CreateBaseBoxQuestList(szBaseControl);
	RegisterControl(pLineBox);
	return pLineBox;
}

CBasicLineBox* CXihong::CreateUILineBoxWhiteNoBody(char* szBaseControl)
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub(this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pLineBox->CreateBaseBoxWhiteBlankBody(szBaseControl);
	RegisterControl(pLineBox);

	return pLineBox;
}

CUIControl* CXihong::CreateUIControl(const char* szControl)
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub(this, szControl, UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pControl->SetVisibleSingle(TRUE);
	RegisterControl(pControl);
	return pControl;
}

void CXihong::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIGroup::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{


	case REBORN_WINDOW_BUTTON_CLOSE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg)) {
			CInnerInterface::GetInstance().HideGroup(GetWndID());
		}
	}break;
	case REBORN_WINDOW_BUTTON_REBORN:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg)) {
			GLGaeaClient::GetInstance().GetCharacter()->ReqResetRedStatsTo();
		}
	}break;

	}

}

void CXihong::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

}

