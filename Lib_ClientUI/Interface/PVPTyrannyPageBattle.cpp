#include "StdAfx.h"
#include "PVPTyrannyPageBattle.h"

#include "d3dfont.h"
#include "GameTextControl.h"
#include "UITextControl.h"

#include "BasicTextBox.h"
#include "BasicLineBox.h"
#include "BasicTextButton.h"

#include "GLPVPTyrannyClient.h"
#include "InnerInterface.h"
#include "DxGlobalStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPVPTyrannyPageBattle::CPVPTyrannyPageBattle()
	: m_pTextBattleTitleStatic(NULL)
	, m_pTextBattleTimeStartStatic(NULL)
	, m_pTextBattleTimeEndStatic(NULL)
	, m_pTextBattleProgressStatic(NULL)
	, m_pTextBattleStatusStatic(NULL)
	, m_pTextBattleTimeStartTimer(NULL)
	, m_pTextBattleTimeEndTimer(NULL)
	, m_pTextBattleProgress(NULL)
	, m_pTextBattleStatus(NULL)
	, m_pButtonRegister(NULL)
	, m_pButtonCloseDummy(NULL)
	, m_fUpdateTime(0.0f)
{
}

CPVPTyrannyPageBattle::~CPVPTyrannyPageBattle()
{
}

void CPVPTyrannyPageBattle::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont12 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_EX_FLAG);

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub(this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pLineBox->CreateBaseBoxQuestList("COMPETITION_CTF_PAGE_BATTLE_LINBOX");
	RegisterControl(pLineBox);

	pLineBox = new CBasicLineBox;
	pLineBox->CreateSub(this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pLineBox->CreateBaseBoxWhiteBlankBody("COMPETITION_CTF_PAGE_BATTLE_LINBOX");
	RegisterControl(pLineBox);

	m_pTextBattleTitleStatic = CreateUITextBox("COMPETITION_CTF_PAGE_BATTLE_TITLE_STATICTEXT", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y);
	m_pTextBattleTimeStartStatic = CreateUITextBox("COMPETITION_CTF_PAGE_BATTLE_STARTTIME_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	m_pTextBattleTimeEndStatic = CreateUITextBox("COMPETITION_CTF_PAGE_BATTLE_ENDTIME_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	m_pTextBattleProgressStatic = CreateUITextBox("COMPETITION_CTF_PAGE_BATTLE_PROGRESSTIME_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	m_pTextBattleStatusStatic = CreateUITextBox("COMPETITION_CTF_PAGE_BATTLE_STATUS_STATICTEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);

	m_pTextBattleTitleStatic->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_STATIC", 0), NS_UITEXTCOLOR::ORANGE);
	m_pTextBattleTimeStartStatic->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_STATIC", 1), NS_UITEXTCOLOR::ORANGE);
	m_pTextBattleTimeEndStatic->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_STATIC", 2), NS_UITEXTCOLOR::ORANGE);
	//m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_CTF_BATTLE_STATIC", 3 ), NS_UITEXTCOLOR::ORANGE );
	m_pTextBattleStatusStatic->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_STATIC", 4), NS_UITEXTCOLOR::ORANGE);

	m_pTextBattleTimeStartTimer = CreateUITextBox("COMPETITION_CTF_PAGE_BATTLE_STARTTIME_TEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	//m_pTextBattleTimeEndTimer = CreateUITextBox( "COMPETITION_CTF_PAGE_BATTLE_ENDTIME_TEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	//m_pTextBattleProgress = CreateUITextBox( "COMPETITION_CTF_PAGE_BATTLE_PROGRESSTIME_TEXT", pFont9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	m_pTextBattleStatus = CreateUITextBox("COMPETITION_CTF_PAGE_BATTLE_STATUS_TEXT", pFont12, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);


	m_pButtonRegister = new CBasicTextButton;
	m_pButtonRegister->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, TYRANNY_PAGE_INFO_BATTLE_BUTTON_REGISTER);
	m_pButtonRegister->CreateBaseButton("COMPETITION_CTF_PAGE_BATTLE_BUTTON", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 0));
	RegisterControl(m_pButtonRegister);

	m_pButtonCloseDummy = new CBasicTextButton;
	m_pButtonCloseDummy->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, TYRANNY_PAGE_INFO_BATTLE_BUTTON_CLOSEDUMMY);
	m_pButtonCloseDummy->CreateBaseButton("COMPETITION_CTF_PAGE_CLOSE_BUTTON", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("COMPETITION_WINDOW", 1));
	RegisterControl(m_pButtonCloseDummy);
}

CBasicTextBox* CPVPTyrannyPageBattle::CreateUITextBox(const char* szControl, CD3DFontPar* pFont, int nTextAlign)
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub(this, szControl);
	pTextBox->SetFont(pFont);
	pTextBox->SetTextAlign(nTextAlign);
	RegisterControl(pTextBox);
	return pTextBox;
}

void CPVPTyrannyPageBattle::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIGroup::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	case TYRANNY_PAGE_INFO_BATTLE_BUTTON_REGISTER:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			GLPVPTyrannyClient::GetInstance().DoRegister();
		}
	}break;
	case TYRANNY_PAGE_INFO_BATTLE_BUTTON_CLOSEDUMMY:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{

		}
	}break;
	};
}

void CPVPTyrannyPageBattle::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	m_fUpdateTime += fElapsedTime;
	if (m_fUpdateTime >= 0.5f)
	{
		ShowInfo();
		m_fUpdateTime = 0.0f;
	}
}

void CPVPTyrannyPageBattle::ShowInfo()
{
	TYRANNY_SCHED_NEXT& sNextSched = GLPVPTyrannyClient::GetInstance().m_sScheduleNext;
	TYRANNY_STATE emState = GLPVPTyrannyClient::GetInstance().m_emState;
	BOOL	bRegistered = GLPVPTyrannyClient::GetInstance().m_bRegistered;
	BOOL	bQueued = GLPVPTyrannyClient::GetInstance().m_bQueued;
	WORD	wQueueNum = GLPVPTyrannyClient::GetInstance().m_wQueueNum;
	float	fTime = GLPVPTyrannyClient::GetInstance().m_fRemain;

	if (m_pTextBattleTimeStartTimer)
	{
		m_pTextBattleTimeStartTimer->ClearText();
		if (sNextSched.dwIndex != UINT_MAX)
		{
			CString strCombine;
			strCombine.Format("%02d:%02d", sNextSched.wBattleStartHour, sNextSched.wBattleStartMinute);
			m_pTextBattleTimeStartTimer->AddText(strCombine.GetString(), NS_UITEXTCOLOR::WHITE);
		}
		else
		{
			m_pTextBattleTimeStartTimer->AddText("00:00", NS_UITEXTCOLOR::WHITE);
		}
	}

	if (m_pTextBattleTimeEndTimer)
	{
		m_pTextBattleTimeEndTimer->ClearText();
		if (sNextSched.dwIndex != UINT_MAX)
		{
			CString strCombine;
			strCombine.Format("%02d:%02d", sNextSched.wBattleEndHour, sNextSched.wBattleEndMinute);
			m_pTextBattleTimeEndTimer->AddText(strCombine.GetString(), NS_UITEXTCOLOR::WHITE);
		}
		else
		{
			m_pTextBattleTimeEndTimer->AddText("00:00", NS_UITEXTCOLOR::WHITE);
		}
	}

	//if ( m_pTextBattleProgressStatic )
	//{
	//	m_pTextBattleProgressStatic->ClearText();
		//if ( emState == TYRANNY_STATE_REGISTER )
			//m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_CTF_BATTLE_PROGRESS_TEXT", 0 ), NS_UITEXTCOLOR::ORANGE );
		//else if ( emState == TYRANNY_STATE_BATTLE )
			//m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_CTF_BATTLE_PROGRESS_TEXT", 1 ), NS_UITEXTCOLOR::ORANGE );
		//else if ( emState == TYRANNY_STATE_REWARD)
			//m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_CTF_BATTLE_PROGRESS_TEXT", 2 ), NS_UITEXTCOLOR::ORANGE );
		//else
			//m_pTextBattleProgressStatic->AddText( ID2GAMEWORD( "COMPETITION_CTF_BATTLE_STATIC", 3 ), NS_UITEXTCOLOR::WHITE );
	//}

	if (m_pTextBattleProgress)
	{
		m_pTextBattleProgress->ClearText();
		if (fTime > 0.0f && (emState == TYRANNY_STATE_REGISTER || emState == TYRANNY_STATE_BATTLE || emState == TYRANNY_STATE_REWARD))
		{
			DWORD dwTextColor = NS_UITEXTCOLOR::WHITE;
			if (fTime < 300.0f) {
				dwTextColor = NS_UITEXTCOLOR::RED;
			}
			CString strCombine;
			strCombine.Format("%02d:%02d", (DWORD)fTime / 60, (DWORD)fTime % 60);
			m_pTextBattleProgress->AddText(strCombine.GetString(), dwTextColor);
		}
		else
		{
			m_pTextBattleProgress->AddText("00:00", NS_UITEXTCOLOR::WHITE);
		}
	}


	if (m_pTextBattleStatus)
	{
		m_pTextBattleStatus->ClearText();

		if (DxGlobalStage::GetInstance().GetChannel() != 0)
		{
			m_pTextBattleStatus->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 9), NS_UITEXTCOLOR::RED);
		}
		else
		{
			if (emState == TYRANNY_STATE_REGISTER)
			{
				if (bRegistered && bQueued)
				{
					CString strCombine;
					strCombine.Format("%s:%02d", ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 10), wQueueNum);
					m_pTextBattleStatus->AddText(strCombine.GetString(), NS_UITEXTCOLOR::ORANGE);
				}
				else if (bRegistered)
					m_pTextBattleStatus->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 5), NS_UITEXTCOLOR::ORANGE);
				else
					m_pTextBattleStatus->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 6), NS_UITEXTCOLOR::GREENYELLOW);
			}
			else
			{
				if (bRegistered)
					m_pTextBattleStatus->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 11), NS_UITEXTCOLOR::ORANGE);
				else
					m_pTextBattleStatus->AddText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 3), NS_UITEXTCOLOR::RED);
			}
		}
	}


	if (m_pButtonRegister)
	{
		if (bRegistered && emState == TYRANNY_STATE_REGISTER)
			m_pButtonRegister->SetOneLineText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 1));
		else if (bRegistered && emState == TYRANNY_STATE_BATTLE)
			m_pButtonRegister->SetOneLineText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 11));
		else
			m_pButtonRegister->SetOneLineText(ID2GAMEWORD("COMPETITION_CTF_BATTLE_TEXT", 0));
	}
}
