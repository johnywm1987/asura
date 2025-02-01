#include "StdAfx.h"
#include "ingametopleft.h"
#include "./BasicTextBox.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "d3dfont.h"

Cingametopleft::Cingametopleft()
	: m_pLevelText(NULL)
	, m_wLevelBACK(0)
{
}

Cingametopleft::~Cingametopleft()
{
	m_wLevelBACK = 0;
}

void Cingametopleft::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII);

	CBasicTextBox* pLevelText = new CBasicTextBox;
	pLevelText->CreateSub(this, "BASIC_LEVEL_DISPLAY_TEXT");
	pLevelText->SetFont(pFont8);
	pLevelText->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(pLevelText);
	m_pLevelText = pLevelText;
}

void Cingametopleft::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	const WORD& wLevel = GLGaeaClient::GetInstance().GetCharacter()->m_wLevel;

	if (m_wLevelBACK < wLevel)
	{
		m_wLevelBACK = wLevel;

		CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

		CString strLevel;
		strLevel.Format("%d", m_wLevelBACK);
		m_pLevelText->SetOneLineText(strLevel, NS_UITEXTCOLOR::GOLD /*PLUSOPTION*/);
	}
}