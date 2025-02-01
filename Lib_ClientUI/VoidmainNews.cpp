#include "StdAfx.h"
#include "VoidMainNews.h"
#include "BasicTextBox.h"
#include "d3dfont.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "InnerInterface.h"
#include "BasicTextButton.h" 
#include "UIEditBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


VoidMainNews::VoidMainNews()
	: m_pNewsImage0(NULL)
	, m_pNewsImage1(NULL)
	, m_pNewsImage2(NULL)
	, m_pNewsImage3(NULL)
	, m_pNewsImage4(NULL)
	, m_pNewsImage5(NULL)
	, m_pNewsImage6(NULL)
	, m_pNewsImageSwitch(NULL)
	, vm_GuideButton(NULL)
	, vm_NewsButton(NULL)
	, vm_ServerInfoButton(NULL)
	, vm_RVButton(NULL)
	, vm_DonationButton(NULL)
	, vm_GiftCodeButton(NULL)
	, waitfordelay(false)
	, fDelayTime(0.0f)

{

}

VoidMainNews::~VoidMainNews()
{
}

void VoidMainNews::CreateSubControl()
{
	CString LineBoxes;
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 11, _DEFAULT_FONT_SHADOW_FLAG);

	for (int i = 0; i < 4; i++)
	{
		char* LBoxKey;
		LineBoxes.Format("VOIDMAIN_NEWS_LINEBOX_BUTTON_%d", i);
		LBoxKey = (LPTSTR)(LPCTSTR)LineBoxes.GetString();
		CreateLineBoxWhite(LBoxKey);
	}
	//Buttons
	vm_NewsButton = CreateTextButton23("VOIDMAIN_NEWS_NEWS_BUTTON", NEWS_BUTTON_IMAGE, ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 1));
	vm_NewsButton->SetFlip(true);
	vm_NewsButton->SetVisibleSingle(TRUE);

	vm_GuideButton = CreateTextButton23("VOIDMAIN_NEWS_GUIDE_BUTTON", GUIDE_BUTTON_IMAGE, ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 2));
	vm_GuideButton->SetFlip(false);
	vm_GuideButton->SetVisibleSingle(TRUE);

	vm_ServerInfoButton = CreateTextButton23("VOIDMAIN_NEWS_SERVER_BUTTON", SERVERINFO_BUTTON_IMAGE, ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 3));
	vm_ServerInfoButton->SetFlip(false);
	vm_ServerInfoButton->SetVisibleSingle(TRUE);

	vm_RVButton = CreateTextButton23("VOIDMAIN_NEWS_RV_BUTTON", RV_BUTTON_IMAGE, ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 4));
	vm_RVButton->SetFlip(false);
	vm_RVButton->SetVisibleSingle(TRUE);

	vm_DonationButton = CreateTextButton23("VOIDMAIN_NEWS_DONATE_BUTTON", DONATION_BUTTON_IMAGE, ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 5));
	vm_DonationButton->SetFlip(false);
	vm_DonationButton->SetVisibleSingle(TRUE);

	vm_DonationButton = CreateTextButton23("VOIDMAIN_NEWS_DONATE_BUTTON", DONATION_BUTTON_IMAGE, ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 5));
	vm_DonationButton->SetFlip(false);
	vm_DonationButton->SetVisibleSingle(TRUE);

	vm_GiftCodeButton = CreateTextButton23("VOIDMAIN_NEWS_GC_BUTTON", GC_BUTTON_IMAGE, ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 6));
	vm_GiftCodeButton->SetFlip(false);
	vm_GiftCodeButton->SetVisibleSingle(TRUE);




	/*News/Guide  Images VOIDMAIN NRDEV 9/3/2021*/

	m_pNewsImage1 = CreateNewsWindow("VOID_MAIN_NEWS_1");
	m_pNewsImage1->SetVisibleSingle(TRUE);
	m_pNewsImage2 = CreateNewsWindow("VOID_MAIN_NEWS_2");
	m_pNewsImage2->SetVisibleSingle(FALSE);
	m_pNewsImage3 = CreateNewsWindow("VOID_MAIN_NEWS_3");
	m_pNewsImage3->SetVisibleSingle(FALSE);
	m_pNewsImage4 = CreateNewsWindow("VOID_MAIN_NEWS_4");
	m_pNewsImage4->SetVisibleSingle(FALSE);
	m_pNewsImage5 = CreateNewsWindow("VOID_MAIN_NEWS_5");
	m_pNewsImage5->SetVisibleSingle(FALSE);

	m_pNewsImage6 = CreateNewsWindow("VOID_MAIN_NEWS_6");
	m_pNewsImage6->SetVisibleSingle(FALSE);

	NewsLabel = CreateNewsLabel("VOID_MAIN_NEWS_LABEL", pFont, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
	NewsLabel->SetText(ID2GAMEWORD("VOID_MAIN_NEWS_TEXT", 0), NS_UITEXTCOLOR::WHITE);
	NewsLabel->SetVisibleSingle(TRUE);

}

CUIControl* VoidMainNews::CreateNewsWindow(const char* guiKeyword)
{
	CUIControl* m_pNewsImage = new CUIControl;
	m_pNewsImage->CreateSub(this, guiKeyword, UI_FLAG_DEFAULT);
	m_pNewsImage->SetVisibleSingle(TRUE);
	RegisterControl(m_pNewsImage);
	return m_pNewsImage;
}

CBasicTextBox* VoidMainNews::CreateNewsLabel(const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	pStaticText->SetVisibleSingle(FALSE);
	RegisterControl(pStaticText);

	return pStaticText;
}

CBasicTextButton* VoidMainNews::CreateTextButton23(const char* szButton, UIGUID ControlID, const char* szText)
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE22;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE, ControlID);
	pTextButton->CreateBaseButton(szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText, _DEFAULT_FONT_SHADOW_FLAG);
	RegisterControl(pTextButton);
	return pTextButton;
}
CBasicLineBoxEx* VoidMainNews::CreateLineBoxWhite(char* szLineBoxKeyWord)
{
	CBasicLineBoxEx* m_pLineBoxes = new CBasicLineBoxEx;
	m_pLineBoxes->CreateSub(this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pLineBoxes->CreateBaseBoxDialogue(szLineBoxKeyWord);
	m_pLineBoxes->SetVisibleSingle(TRUE);
	RegisterControl(m_pLineBoxes);

	return m_pLineBoxes;
}
CBasicTextButton* VoidMainNews::CreateTextButton(const char* szButton, UIGUID ControlID, const char* szText)
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE19;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ControlID);
	pTextButton->CreateBaseButton(szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText);
	pTextButton->SetFlip(FALSE);
	RegisterControl(pTextButton);
	return pTextButton;
}

void VoidMainNews::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	fDelayTime += fElapsedTime;
	if (waitfordelay)
	{
		if (fDelayTime > 8.3f)
		{
			waitfordelay = false;
			fDelayTime = 0.0f;
		}
	}



}
void VoidMainNews::ResetControls()
{
	vm_NewsButton->SetFlip(false);
	vm_GuideButton->SetFlip(false);
	vm_ServerInfoButton->SetFlip(false);
	vm_RVButton->SetFlip(false);
	vm_DonationButton->SetFlip(false);
	vm_GiftCodeButton->SetFlip(false);

	m_pNewsImage1->SetVisibleSingle(FALSE);
	m_pNewsImage2->SetVisibleSingle(FALSE);
	m_pNewsImage3->SetVisibleSingle(FALSE);
	m_pNewsImage4->SetVisibleSingle(FALSE);
	m_pNewsImage5->SetVisibleSingle(FALSE);
	m_pNewsImage6->SetVisibleSingle(FALSE);
}
void VoidMainNews::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	case NEWS_BUTTON_IMAGE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			ResetControls();
			vm_NewsButton->SetFlip(true);
			m_pNewsImage1->SetVisibleSingle(TRUE);

		}

	}break;
	case GUIDE_BUTTON_IMAGE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			ResetControls();
			vm_GuideButton->SetFlip(true);
			m_pNewsImage2->SetVisibleSingle(TRUE);
		}


	}break;
	case SERVERINFO_BUTTON_IMAGE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			ResetControls();
			vm_ServerInfoButton->SetFlip(true);
			m_pNewsImage3->SetVisibleSingle(TRUE);
		}


	}break;

	case RV_BUTTON_IMAGE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			ResetControls();
			vm_RVButton->SetFlip(true);
			m_pNewsImage4->SetVisibleSingle(TRUE);
		}


	}break;

	case DONATION_BUTTON_IMAGE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			ResetControls();
			vm_DonationButton->SetFlip(true);
			m_pNewsImage5->SetVisibleSingle(TRUE);
		}


	}break;
	case GC_BUTTON_IMAGE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			ResetControls();
			vm_GiftCodeButton->SetFlip(true);
			m_pNewsImage6->SetVisibleSingle(TRUE);
		}


	}break;
	}
}
