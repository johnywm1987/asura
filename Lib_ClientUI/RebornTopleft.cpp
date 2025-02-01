#include "StdAfx.h"
#include "reborntopleft.h"
#include "../Lib_ClientUI/Interface/BasicTextBox.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "d3dfont.h"



CReborntf::CReborntf()
	: m_pRebornText(NULL)
	, m_dwRebornBACK(0)
{
}

CReborntf::~CReborntf()
{
	m_dwRebornBACK = 0;
}

void CReborntf::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII);

	CBasicTextBox* pRebornText = new CBasicTextBox;
	pRebornText->CreateSub(this, "BASIC_REBORN_TEXT");
	pRebornText->SetFont(pFont8);
	pRebornText->SetTextAlign(TEXT_ALIGN_CENTER_X);
	RegisterControl(pRebornText);
	m_pRebornText = pRebornText;
}

void CReborntf::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	const DWORD& Reborn = GLGaeaClient::GetInstance().GetCharacter()->m_dwReborn;

	if (m_dwRebornBACK < Reborn)
	{
		m_dwRebornBACK = Reborn;

		CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

		CString strReborn;
		strReborn.Format("%d", m_dwRebornBACK);
		m_pRebornText->SetOneLineText(strReborn, NS_UITEXTCOLOR::YELLOW /*PLUSOPTION*/);
	}
	if (Reborn == 0) // Add 0 Default Display | Eduj
	{
		m_pRebornText->SetOneLineText("0", NS_UITEXTCOLOR::RED /*PLUSOPTION*/);
	}

}