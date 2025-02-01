#include "StdAfx.h"
#include "PotionSettingsWindow.h"

#include "./BasicProgressBar.h"
#include "./BasicTextBox.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "GameTextControl.h"
#include "./UIEditBox.h"
#include "BasicLineBox.h"
#include "BasicTextButton.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"

#include "d3dfont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CPotionSettingsWindow::nLIMIT_NUMBER_CHAR = 3;
const int CPotionSettingsWindow::nMAX_COLOR = 100;
const float CPotionSettingsWindow::fRGB_RATIO = 8.225806f;

CPotionSettingsWindow::CPotionSettingsWindow()
	: m_pRProgressBar(NULL)
	, m_pGProgressBar(NULL)
	, m_pBProgressBar(NULL)
	, m_pREditBox(NULL)
	, m_pGEditBox(NULL)
	, m_pBEditBox(NULL)
	, m_nR(0)
	, m_nG(0)
	, m_nB(0)
	, m_nRBack(0)
	, m_nGBack(0)
	, m_nBBack(0)
	, m_nRegR(0)
	, m_nRegG(0)
	, m_nRegB(0)
	, m_bOKButton(FALSE)
{
}

CPotionSettingsWindow::~CPotionSettingsWindow()
{
}

CBasicTextBox* CPotionSettingsWindow::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CPotionSettingsWindow::CreateSubControl()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, D3DFONT_SHADOW | D3DFONT_ASCII);
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, D3DFONT_SHADOW | D3DFONT_ASCII);

	CreateControl("POTION_SETTINGS_TITLE_BACK_L");
	CreateControl("POTION_SETTINGS_TITLE_BACK_R");
	CBasicTextBox* pTextBox = CreateStaticControl("POTION_SETTINGS_TITLE", pFont, TEXT_ALIGN_CENTER_X);
	pTextBox->AddText(ID2GAMEINTEXT("POTION_SETTINGS_TITLE"));

	{ // Note : R,G,B Static
		/*CUIControl * pUIControl = new CUIControl;
		pUIControl->CreateSub( this, "POTION_SETTINGS_R" );
		RegisterControl( pUIControl );

		pUIControl = new CUIControl;
		pUIControl->CreateSub( this, "POTION_SETTINGS_G" );
		RegisterControl( pUIControl );

		pUIControl = new CUIControl;
		pUIControl->CreateSub( this, "POTION_SETTINGS_B" );
		RegisterControl( pUIControl );*/

		CBasicTextBox* pTextBoxHP = CreateStaticControl("POTION_SETTINGS_HP", pFont9, TEXT_ALIGN_CENTER_X);
		pTextBoxHP->AddText("HP");

		CBasicTextBox* pTextBoxMP = CreateStaticControl("POTION_SETTINGS_MP", pFont9, TEXT_ALIGN_CENTER_X);
		pTextBoxMP->AddText("MP");

		CBasicTextBox* pTextBoxSP = CreateStaticControl("POTION_SETTINGS_SP", pFont9, TEXT_ALIGN_CENTER_X);
		pTextBoxSP->AddText("SP");

		CBasicTextBox* pTextBoxPercentHP = CreateStaticControl("POTION_SETTINGS_HP_PERCENT", pFont9, TEXT_ALIGN_CENTER_X);
		pTextBoxPercentHP->AddText("%");

		CBasicTextBox* pTextBoxPercentMP = CreateStaticControl("POTION_SETTINGS_MP_PERCENT", pFont9, TEXT_ALIGN_CENTER_X);
		pTextBoxPercentMP->AddText("%");

		CBasicTextBox* pTextBoxPercentSP = CreateStaticControl("POTION_SETTINGS_SP_PERCENT", pFont9, TEXT_ALIGN_CENTER_X);
		pTextBoxPercentSP->AddText("%");
	}

	{	// Note : 프로그래스바
		CBasicProgressBar* pProgressBar(NULL);
		CUIControl* pDummyBar(NULL);

		pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub(this, "POTION_SETTINGS_R_PROGRESS");
		pProgressBar->CreateOverImage("POTION_SETTINGS_R_PROGRESS_OVERIMAGE");
		RegisterControl(pProgressBar);
		m_pRProgressBar = pProgressBar;

		pDummyBar = new CUIControl;
		pDummyBar->CreateSub(this, "POTION_SETTINGS_R_PROGRESS_DUMMY", UI_FLAG_DEFAULT, POTION_SETTINGS_R_PROGRESS);
		RegisterControl(pDummyBar);

		pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub(this, "POTION_SETTINGS_G_PROGRESS");
		pProgressBar->CreateOverImage("POTION_SETTINGS_G_PROGRESS_OVERIMAGE");
		RegisterControl(pProgressBar);
		m_pGProgressBar = pProgressBar;

		pDummyBar = new CUIControl;
		pDummyBar->CreateSub(this, "POTION_SETTINGS_G_PROGRESS_DUMMY", UI_FLAG_DEFAULT, POTION_SETTINGS_G_PROGRESS);
		RegisterControl(pDummyBar);

		pProgressBar = new CBasicProgressBar;
		pProgressBar->CreateSub(this, "POTION_SETTINGS_B_PROGRESS");
		pProgressBar->CreateOverImage("POTION_SETTINGS_B_PROGRESS_OVERIMAGE");
		RegisterControl(pProgressBar);
		m_pBProgressBar = pProgressBar;

		pDummyBar = new CUIControl;
		pDummyBar->CreateSub(this, "POTION_SETTINGS_B_PROGRESS_DUMMY", UI_FLAG_DEFAULT, POTION_SETTINGS_B_PROGRESS);
		RegisterControl(pDummyBar);
	}

	{ // Note : 입력 에디터 박스
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxEditBox("POTION_SETTINGS_R_EDITBOX_BACK");
		RegisterControl(pBasicLineBox);

		CUIEditBox* pEditBox = new CUIEditBox;
		pEditBox->CreateSub(this, "POTION_SETTINGS_R_EDITBOX", UI_FLAG_XSIZE | UI_FLAG_RIGHT, POTION_SETTINGS_R_EDITBOX);
		pEditBox->CreateCarrat("MODAL_CARRAT", TRUE, UINT_MAX);
		pEditBox->SetLimitInput(nLIMIT_NUMBER_CHAR);
		pEditBox->SetFont(pFont);
		pEditBox->DoUSE_NUMBER(true);
		RegisterControl(pEditBox);
		m_pREditBox = pEditBox;

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxEditBox("POTION_SETTINGS_G_EDITBOX_BACK");
		RegisterControl(pBasicLineBox);

		pEditBox = new CUIEditBox;
		pEditBox->CreateSub(this, "POTION_SETTINGS_G_EDITBOX", UI_FLAG_XSIZE | UI_FLAG_RIGHT, POTION_SETTINGS_G_EDITBOX);
		pEditBox->CreateCarrat("MODAL_CARRAT", TRUE, UINT_MAX);
		pEditBox->SetLimitInput(nLIMIT_NUMBER_CHAR);
		pEditBox->SetFont(pFont);
		pEditBox->DoUSE_NUMBER(true);
		RegisterControl(pEditBox);
		m_pGEditBox = pEditBox;

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_EDIT", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxEditBox("POTION_SETTINGS_B_EDITBOX_BACK");
		RegisterControl(pBasicLineBox);

		pEditBox = new CUIEditBox;
		pEditBox->CreateSub(this, "POTION_SETTINGS_B_EDITBOX", UI_FLAG_XSIZE | UI_FLAG_RIGHT, POTION_SETTINGS_B_EDITBOX);
		pEditBox->CreateCarrat("MODAL_CARRAT", TRUE, UINT_MAX);
		pEditBox->SetLimitInput(nLIMIT_NUMBER_CHAR);
		pEditBox->SetFont(pFont);
		pEditBox->DoUSE_NUMBER(true);
		RegisterControl(pEditBox);
		m_pBEditBox = pEditBox;
	}

	{ // Note : 버튼
		CBasicTextButton* pButton = new CBasicTextButton;
		pButton->CreateSub(this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, POTION_SETTINGS_OK);
		pButton->CreateBaseButton("POTION_SETTINGS_OK", CBasicTextButton::SIZE14,
			CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("MODAL_BUTTON", 0));
		pButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
		RegisterControl(pButton);

		pButton = new CBasicTextButton;
		pButton->CreateSub(this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, POTION_SETTINGS_CANCEL);
		pButton->CreateBaseButton("POTION_SETTINGS_CANCEL", CBasicTextButton::SIZE14,
			CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("MODAL_BUTTON", 1));
		pButton->SetShortcutKey(DIK_ESCAPE);
		RegisterControl(pButton);
	}
}

BOOL CPotionSettingsWindow::UpdateProgressBar(CBasicProgressBar* pBar, int x)
{
	if (pBar && pBar->IsExclusiveSelfControl())
	{
		const float fSize = pBar->GetGlobalPos().sizeX;
		const float fPos = float(x) - pBar->GetGlobalPos().left;
		const float fPercent = fPos / fSize;
		pBar->SetPercent(fPercent);

		return TRUE;
	}

	return FALSE;
}

void CPotionSettingsWindow::UpdateProgressBarToEditBox(CBasicProgressBar* pBar, CUIEditBox* pEditBox, INT& nColorVal)
{
	int nVal(0);
	float fPercent = pBar->GetPercent();
	if (fPercent > 0)	nVal = (int)(nMAX_COLOR * fPercent);

	CString strTemp;
	strTemp.Format("%d", nVal);

	pEditBox->ClearEdit();
	pEditBox->SetEditString(strTemp);

	nColorVal = nVal;
}

BOOL CPotionSettingsWindow::UpdateEditBoxToProgressBar(CUIEditBox* pEditBox, CBasicProgressBar* pBar, INT& nColorVal)
{
	if (pEditBox && pEditBox->IsBegin())
	{
		const CString& strRVal = pEditBox->GetEditString();
		nColorVal = _ttoi(strRVal.GetString());
		if (nColorVal > nMAX_COLOR)
		{
			nColorVal = nMAX_COLOR;

			pEditBox->ClearEdit();
			pEditBox->SetEditString(CString("31"));
		}

		float fPercent(0);
		if (nColorVal > 0)	fPercent = nColorVal / float(nMAX_COLOR);
		pBar->SetPercent(fPercent);

		return TRUE;
	}

	return FALSE;
}

void CPotionSettingsWindow::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (bFirstControl)
	{
		BOOL bChange(FALSE);

		if (UpdateProgressBar(m_pRProgressBar, x))
		{
			UpdateProgressBarToEditBox(m_pRProgressBar, m_pREditBox, m_nR);
			bChange = TRUE;
		}
		else if (UpdateProgressBar(m_pGProgressBar, x))
		{
			UpdateProgressBarToEditBox(m_pGProgressBar, m_pGEditBox, m_nG);
			bChange = TRUE;
		}
		else if (UpdateProgressBar(m_pBProgressBar, x))
		{
			UpdateProgressBarToEditBox(m_pBProgressBar, m_pBEditBox, m_nB);
			bChange = TRUE;
		}

		if (UpdateEditBoxToProgressBar(m_pREditBox, m_pRProgressBar, m_nR))		bChange = TRUE;
		else if (UpdateEditBoxToProgressBar(m_pGEditBox, m_pGProgressBar, m_nG))	bChange = TRUE;
		else if (UpdateEditBoxToProgressBar(m_pBEditBox, m_pBProgressBar, m_nB))	bChange = TRUE;
	}
}

void CPotionSettingsWindow::TranslateProgressBarMsg(CBasicProgressBar* pBar, DWORD dwMsg)
{
	if (!pBar) return;

	if (CHECK_MOUSE_IN(dwMsg))
	{
		if (CHECK_LB_DOWN_LIKE(dwMsg))
		{
			pBar->SetExclusiveControl();
		}
		else if (CHECK_LB_UP_LIKE(dwMsg))
		{
			pBar->ResetExclusiveControl();
		}
	}
	else if (CHECK_LB_UP_LIKE(dwMsg))
	{
		pBar->ResetExclusiveControl();
	}
}

void CPotionSettingsWindow::TranslateEditBoxMsg(CUIEditBox* pEditBox, DWORD dwMsg)
{
	if (!pEditBox) return;

	if (CHECK_MOUSE_IN_LBDOWNLIKE(dwMsg))
	{
		pEditBox->BeginEdit();
	}
	else if (CHECK_LB_DOWN_LIKE(dwMsg))
	{
		pEditBox->EndEdit();
	}
}

void CPotionSettingsWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case POTION_SETTINGS_R_PROGRESS: TranslateProgressBarMsg(m_pRProgressBar, dwMsg); break;
	case POTION_SETTINGS_G_PROGRESS: TranslateProgressBarMsg(m_pGProgressBar, dwMsg); break;
	case POTION_SETTINGS_B_PROGRESS: TranslateProgressBarMsg(m_pBProgressBar, dwMsg); break;

	case POTION_SETTINGS_R_EDITBOX: TranslateEditBoxMsg(m_pREditBox, dwMsg); break;
	case POTION_SETTINGS_G_EDITBOX: TranslateEditBoxMsg(m_pGEditBox, dwMsg); break;
	case POTION_SETTINGS_B_EDITBOX: TranslateEditBoxMsg(m_pBEditBox, dwMsg); break;

	case POTION_SETTINGS_OK:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			m_bOKButton = TRUE;
			CInnerInterface::GetInstance().HideGroup(GetWndID());
		}
	}
	break;

	case POTION_SETTINGS_CANCEL:
	case ET_CONTROL_BUTTON:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CInnerInterface::GetInstance().HideGroup(GetWndID());
		}
	}
	break;
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CPotionSettingsWindow::SetVisibleSingle(BOOL bVisible)
{
	CUIWindowEx::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		InitUIControl();

		//GLGaeaClient::GetInstance().GetCharacter()->HairColorInitData();
	}
	else
	{
		if (m_bOKButton)
		{
			//WORD wTemp = m_nR << 10	| m_nG << 5 | m_nB;
			WORD wHPPercent = m_nR;
			WORD wMPPercent = m_nB;
			WORD wSPPercent = m_nG;
		//	GLGaeaClient::GetInstance().GetCharacter()->ReqToggleRunAutoPots(wHPPercent, wMPPercent, wSPPercent);

			m_bOKButton = FALSE;
		}
		//else
		//{
		//	WORD wTemp = m_nRBack << 10	| m_nGBack << 5 | m_nBBack;
		//	GLGaeaClient::GetInstance().GetCharacter()->HairColorChange( wTemp );
		//}

		ClearEditBox();

		//GLGaeaClient::GetInstance().GetCharacter()->HairColorEnd();
	}
}

void CPotionSettingsWindow::InitUIControl()
{
	//const WORD & wHairColor = GLGaeaClient::GetInstance().GetCharacter()->m_wHairColor;
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (!pCharacter) return;

	// Note : Hair Color Data 구조
	// |  X |       RED      |   GREEN   |    BLUE   |
	// | 15 | 14 13 12 11 10 | 9 8 7 6 5 | 4 3 2 1 0 | 
	m_nRBack = m_nR = pCharacter->m_wHPPercent;
	m_nGBack = m_nG = pCharacter->m_wMPPercent;
	m_nBBack = m_nB = pCharacter->m_wSPPercent;

	CString strTemp;

	strTemp.Format("%d", m_nR);
	m_pREditBox->SetEditString(strTemp);

	strTemp.Format("%d", m_nG);
	m_pGEditBox->SetEditString(strTemp);

	strTemp.Format("%d", m_nB);
	m_pBEditBox->SetEditString(strTemp);

	float fPercent(0);
	if (m_nR > 0)	fPercent = m_nR / float(nMAX_COLOR);
	m_pRProgressBar->SetPercent(fPercent);

	fPercent = 0;
	if (m_nG > 0)	fPercent = m_nG / float(nMAX_COLOR);
	m_pGProgressBar->SetPercent(fPercent);

	fPercent = 0;
	if (m_nB > 0)	fPercent = m_nB / float(nMAX_COLOR);
	m_pBProgressBar->SetPercent(fPercent);
}
void CPotionSettingsWindow::ClearEditBox()
{
	m_pREditBox->ClearEdit();
	m_pREditBox->EndEdit();

	m_pGEditBox->ClearEdit();
	m_pGEditBox->EndEdit();

	m_pBEditBox->ClearEdit();
	m_pBEditBox->EndEdit();
}