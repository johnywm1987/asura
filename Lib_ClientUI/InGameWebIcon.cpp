#include "StdAfx.h"
#include "InGameWebIcon.h"
#include "./InnerInterface.h"
#include "./GameTextControl.h"
#include "./UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CInGameWebIcon::CInGameWebIcon()
	: m_pButtonImage(NULL)
{
}

CInGameWebIcon::~CInGameWebIcon()
{
}

void CInGameWebIcon::CreateSubControl()
{
	m_pButtonImage = new CUIControl;
	m_pButtonImage->CreateSub(this, "RAN_INGAMEWEBICON_NOTIFY_BUTTON_IMAGE", UI_FLAG_DEFAULT, INGAMEWEB_RECORD_BUTTON_IMAGE);
	m_pButtonImage->SetVisibleSingle(TRUE);
	RegisterControl(m_pButtonImage);
}

void CInGameWebIcon::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}

void CInGameWebIcon::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIGroup::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	case INGAMEWEB_RECORD_BUTTON_IMAGE:
	{
		if (CHECK_MOUSE_IN(dwMsg)) {
			CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(ID2GAMEWORD("RAN_INGAME_WINDOW_TITLE", 0), NS_UITEXTCOLOR::WHITE);
		}

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg)) {
			CInnerInterface::GetInstance().ShowGroupFocus(IN_GAME_WINDOW);
		}
	}break;
	}
}
