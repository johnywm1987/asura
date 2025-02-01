#include "StdAfx.h"
#include "KillFeedSelectorWindow.h"
#include "PKRankNotificationInfo.h"

#include "./GLogic.h"
#include "./UIKeyCheck.h"
#include "./GameTextControl.h"
#include "./BasicLineBox.h"
#include "./BasicTextBox.h"
#include "./BasicTextBoxEx.h"
#include "./BasicButton.h"
#include "./BasicTextButton.h"
#include "./BasicComboBoxRollOver.h"
#include "./BasicScrollBarEx.h"
#include "./BasicScrollThumbFrame.h"
#include "./BasicVarTextBox.h"
#include "./DxFontMan.h"
#include "./InnerInterface.h"
#include "./DxGlobalStage.h"
#include "./GLGaeaClient.h"
#include "./GLCharacter.h"

#include "./GLItemMan.h"
#include "./RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CKillFeedSelectorWindow::CKillFeedSelectorWindow(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface)
	: m_pGaeaClient(pGaeaClient)
	, m_pInterface(pInterface)
	, nSelectedKillFeed(-1)
{
	for (int i = 0; i < 6; ++i)
	{
		m_pKillFeedSample[i] = NULL;
		m_pKillFeedSelected[i] = NULL;
	}
}

CKillFeedSelectorWindow::~CKillFeedSelectorWindow()
{
}

void CKillFeedSelectorWindow::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont20 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG);

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub(this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pLineBox->CreateBaseBoxQuestList("KILL_FEED_SELECTOR_LINEBOX");
	RegisterControl(pLineBox);

	CBasicTextBoxEx* pText = new CBasicTextBoxEx;
	pText->CreateSub(this, "KILL_FEED_SELECTOR_TEXT");
	pText->SetFont(pFont9);
	pText->SetTextAlign(TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	pText->SetText(ID2GAMEINTEXT("KILL_FEED_SELECTOR_TEXT"));
	RegisterControl(pText);

	{
		std::string strInfo[6] =
		{
			"KILL_FEED_SELECTOR_0",
			"KILL_FEED_SELECTOR_1",
			"KILL_FEED_SELECTOR_2",
			"KILL_FEED_SELECTOR_3",
			"KILL_FEED_SELECTOR_4",
			"KILL_FEED_SELECTOR_5",
		};
		std::string strInfoLinebox[6] =
		{
			"KILL_FEED_SELECTOR_LINEBOX_0",
			"KILL_FEED_SELECTOR_LINEBOX_1",
			"KILL_FEED_SELECTOR_LINEBOX_2",
			"KILL_FEED_SELECTOR_LINEBOX_3",
			"KILL_FEED_SELECTOR_LINEBOX_4",
			"KILL_FEED_SELECTOR_LINEBOX_5",
		};

		for (int i = 0; i < 6; ++i)
		{
			// SAMPLE DUMMY FOR SELECTION
			SPK_HISTORY sPKHistory;
			sPKHistory.dwKiller = GLGaeaClient::GetInstance().GetCharacter()->GetCharID();
			sPKHistory.dwKilled = 1;
			sPKHistory.cClassKiller = (BYTE)GLGaeaClient::GetInstance().GetCharacter()->GETCHARINDEX();
			sPKHistory.cClassKilled = (BYTE)RandomNumber(0, 11);
			sPKHistory.cSchoolKilled = (BYTE)RandomNumber(0, 2);;
			sPKHistory.cSchoolKiller = (BYTE)GLGaeaClient::GetInstance().GetCharacter()->GetSchool();
			sPKHistory.cKF_KILLER = (BYTE)i + 1;	// KILL FEED
			StringCchCopy(sPKHistory.szCharKiller, CHAR_SZNAME + 1, COMMENT::CHARCLASS[sPKHistory.cClassKiller].c_str());
			StringCchCopy(sPKHistory.szCharKilled, CHAR_SZNAME + 1, COMMENT::CHARCLASS[sPKHistory.cClassKilled].c_str());

			m_pKillFeedSample[i] = new CPKRankNotificationInfo;
			m_pKillFeedSample[i]->CreateSub(this, strInfo[i].c_str(), UI_FLAG_DEFAULT, KILL_FEED_SELECTOR_SLOT00 + i);
			m_pKillFeedSample[i]->CreateSubControl();
			m_pKillFeedSample[i]->SetData(sPKHistory);
			RegisterControl(m_pKillFeedSample[i]);

			m_pKillFeedSelected[i] = new CUIControl;
			m_pKillFeedSelected[i]->CreateSub(this, strInfoLinebox[i].c_str());
			m_pKillFeedSelected[i]->SetVisibleSingle(FALSE);
			RegisterControl(m_pKillFeedSelected[i]);
		}
	}

	{
		CBasicTextButton* pTextButton = new CBasicTextButton;
		pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, KILL_FEED_SELECTOR_OK);
		pTextButton->CreateBaseButton("KILL_FEED_SELECTOR_OK", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, "OK");
		RegisterControl(pTextButton);

		pTextButton = new CBasicTextButton;
		pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, KILL_FEED_SELECTOR_CANCEL);
		pTextButton->CreateBaseButton("KILL_FEED_SELECTOR_CANCEL", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, "CANCEL");
		RegisterControl(pTextButton);
	}
}

void CKillFeedSelectorWindow::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}

void CKillFeedSelectorWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
	switch (ControlID)
	{
	case KILL_FEED_SELECTOR_OK:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			if (CHECK_LB_UP_LIKE(dwMsg))
			{
				if (nSelectedKillFeed != -1)
				{
					GLMSG::SNET_KILLFEED_SELECT_REQ sNetMsg;
					sNetMsg.cIndex = nSelectedKillFeed;
					NETSENDTOFIELD(&sNetMsg);
				}
			}
		}
		return;
	}
	break;
	case KILL_FEED_SELECTOR_CANCEL:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			if (CHECK_LB_UP_LIKE(dwMsg))
			{
				m_pInterface->HideGroup(STYLE_KILL_FEED_WINDOW);
			}
		}
		return;
	}
	break;
	case KILL_FEED_SELECTOR_SLOT00:
	case KILL_FEED_SELECTOR_SLOT01:
	case KILL_FEED_SELECTOR_SLOT02:
	case KILL_FEED_SELECTOR_SLOT03:
	case KILL_FEED_SELECTOR_SLOT04:
	case KILL_FEED_SELECTOR_SLOT05:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			int nIndex = ControlID - KILL_FEED_SELECTOR_SLOT00;
			if (CHECK_LB_UP_LIKE(dwMsg))
			{
				for (int i = 0; i < 6; ++i)
					m_pKillFeedSelected[i]->SetVisibleSingle(FALSE);

				nSelectedKillFeed = m_pKillFeedSample[nIndex]->GetIndex();
				m_pKillFeedSelected[nIndex]->SetVisibleSingle(TRUE);
			}
		}
	}
	break;
	};
}

void CKillFeedSelectorWindow::SetVisibleSingle(BOOL bVisible)
{
	CUIWindowEx::SetVisibleSingle(bVisible);
}