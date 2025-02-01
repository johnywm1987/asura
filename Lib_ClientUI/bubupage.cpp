#include "StdAfx.h"
#include "BubuPage.h"

#include "d3dfont.h"
#include "GameTextControl.h"
#include "UITextControl.h"

#include "BasicLineBox.h"
#include "BasicTextBox.h"
#include "BasicTextButton.h"

#include "GLPVPTyrannyClient.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "DxGlobalStage.h"

#include "TipBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BubuCompetitionNew::BubuCompetitionNew()
	: m_pTextMapName(NULL)
{
}

BubuCompetitionNew::~BubuCompetitionNew()
{
}

void BubuCompetitionNew::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);




	m_pTextMapName = CreateUITextBox("COMPETITION_CTF_PAGE_INFO_TITLE", pFont9, TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y);
	m_pTextMapName->AddText(ID2GAMEWORD("COMPETITION_BUTTON", 0), NS_UITEXTCOLOR::ORANGE);
}

CBasicTextBox* BubuCompetitionNew::CreateUITextBox(const char* szControl, CD3DFontPar* pFont, int nTextAlign)
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub(this, szControl);
	pTextBox->SetFont(pFont);
	pTextBox->SetTextAlign(nTextAlign);
	RegisterControl(pTextBox);
	return pTextBox;
}

CBasicLineBox* BubuCompetitionNew::CreateUILineBoxWhiteNoBody(char* szBaseControl)
{
	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub(this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pLineBox->CreateBaseBoxWhiteBlankBody(szBaseControl);
	RegisterControl(pLineBox);

	return pLineBox;
}

CUIControl* BubuCompetitionNew::CreateUIControl(const char* szControl)
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub(this, szControl, UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pControl->SetVisibleSingle(TRUE);
	RegisterControl(pControl);
	return pControl;
}

void BubuCompetitionNew::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIGroup::TranslateUIMessage(ControlID, dwMsg);
}
