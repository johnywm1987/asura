#include "StdAfx.h"
#include "BubuCompetition.h"
#include "BubuPage.h"
#include "BasicLineBox.h"
#include "MultiModeButton.h"
#include "BasicTextBox.h"
#include "BasicTextButton.h"
#
#include "GameTextControl.h"
#include "InnerInterface.h"
#include "UITextControl.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCompetitionWindowNew::CCompetitionWindowNew()
	: m_nPage(-1)
	, m_pButtonClose(NULL)
	, m_pButtonEventTyranny(NULL)
	, m_pPageTyranny(NULL)
{

}

CCompetitionWindowNew::~CCompetitionWindowNew()
{
}

void CCompetitionWindowNew::CreateSubControl()
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub(this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pLineBox->CreateBaseBoxWhite("RAN_WORLD_COMPETITION_BACK_LINE");
	RegisterControl(pLineBox);

	m_pButtonClose = new CBasicTextButton;
	m_pButtonClose->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, COMPETITION_WINDOW_BUTTON_CLOSE);
	m_pButtonClose->CreateBaseButton("RAN_WORLD_COMPETITION_OBJECT_CLOSE", CBasicTextButton::SIZE22, CBasicButton::CLICK_FLIP, ID2GAMEWORD("RAN_WORLD_COMPETITION_WINDOW_NEW", 1));
	m_pButtonClose->SetFlip(TRUE);
	RegisterControl(m_pButtonClose);

	m_pButtonEventTyranny = CreateTabButton("COMPETITION_OBJECT_TAP_0", ID2GAMEWORD("COMPETITION_BUTTON", 0), COMPETITION_WINDOW_BUTTON_TYRANNY);

	m_pPageTyranny = new BubuCompetitionNew;
	m_pPageTyranny->CreateSub(this, "RAN_WORLD_COMPETITION_OBJECT_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE, COMPETITION_WINDOW_PAGE_TYRANNY);
	m_pPageTyranny->CreateSubControl();
	RegisterControl(m_pPageTyranny);

}

CMultiModeButton* CCompetitionWindowNew::CreateTabButton(const char* szButtonBox, const char* szButtonText, UIGUID ControlID)
{
	CMultiModeButton* pButton = new CMultiModeButton;
	pButton->CreateSub(this, "TAB_BUTTON_WHITE", UI_FLAG_XSIZE, ControlID);
	pButton->CreateImageBaseMulti("TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT", "TAB_BUTTON_WHITE_IMAGE_CENTER", "TAB_BUTTON_WHITE_IMAGE_RIGHT");
	pButton->CreateImageFlipMulti("TAB_BUTTON_WHITE_IMAGE", "TAB_BUTTON_WHITE_IMAGE_LEFT_F", "TAB_BUTTON_WHITE_IMAGE_CENTER_F", "TAB_BUTTON_WHITE_IMAGE_RIGHT_F");
	pButton->CreateTextBox("TAB_BUTTON_WHITE_IMAGE_TEXTBOX", 9, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y, _DEFAULT_FONT_FLAG);
	pButton->AlignToControl(szButtonBox);
	pButton->SetOneLineText(szButtonText, NS_UITEXTCOLOR::WHITE);
	pButton->SetTextColor(NS_UITEXTCOLOR::BLACK, NS_UITEXTCOLOR::WHITE, NS_UITEXTCOLOR::WHITE);
	RegisterControl(pButton);
	return pButton;
}

void CCompetitionWindowNew::SetVisibleSingle(BOOL bVisible)
{
	CUIWindowEx::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		OpenPage(COMPETITION_WINDOW_TYPE_TYRANNY);
	}
	else
	{
	}
}

void CCompetitionWindowNew::TranslateUIMessage(UIGUID cID, DWORD dwMsg)
{
	CUIWindowEx::TranslateUIMessage(cID, dwMsg);

	switch (cID)
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
	{
		if ((dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN(dwMsg)) {
			CInnerInterface::GetInstance().SetDefaultPosInterface(GetWndID());
		}
	}break;


	case COMPETITION_WINDOW_BUTTON_CLOSE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg)) {
			CInnerInterface::GetInstance().HideGroup(GetWndID());
		}
	}break;

	case COMPETITION_WINDOW_BUTTON_TYRANNY:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			int nSelected = cID - COMPETITION_WINDOW_BUTTON_TYRANNY;
			OpenPage(nSelected);
		}
	}break;
	};

}

void CCompetitionWindowNew::OpenPage(int nPage)
{
	if (nPage == m_nPage)	return;

	if (m_pButtonEventTyranny)		m_pButtonEventTyranny->DoImageFlip(FALSE);

	if (m_pPageTyranny)	m_pPageTyranny->SetVisibleSingle(FALSE);

	m_nPage = nPage;

	switch (nPage)
	{
	case COMPETITION_WINDOW_TYPE_TYRANNY:
	{

		if (m_pButtonEventTyranny)	m_pButtonEventTyranny->DoImageFlip(TRUE);
		if (m_pPageTyranny)			m_pPageTyranny->SetVisibleSingle(TRUE);
	}break;
	}
}