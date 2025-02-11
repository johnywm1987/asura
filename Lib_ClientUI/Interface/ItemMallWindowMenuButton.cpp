#include "StdAfx.h"
#include "ItemMallWindowMenuButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CItemMallWindowMenuButton::CItemMallWindowMenuButton ()
	: m_pIcon00( NULL )
	, m_pIcon01( NULL )
{
}

CItemMallWindowMenuButton::~CItemMallWindowMenuButton ()
{
}

void CItemMallWindowMenuButton::CreateSubControl( CString strKeywordOff, CString strKeywordOn )
{
	m_pIcon00 = new CUIControl;
	m_pIcon00->CreateSub ( this, strKeywordOff.GetString(), UI_FLAG_DEFAULT, ICON_OFF );
	m_pIcon00->SetTransparentOption( TRUE );
	m_pIcon00->SetVisibleSingle(TRUE);
	RegisterControl ( m_pIcon00 );

	m_pIcon01 = new CUIControl;
	m_pIcon01->CreateSub ( this, strKeywordOn.GetString(), UI_FLAG_DEFAULT, ICON_ON );
	m_pIcon01->SetTransparentOption( TRUE );
	m_pIcon01->SetVisibleSingle(FALSE);
	RegisterControl ( m_pIcon01 );
}

void CItemMallWindowMenuButton::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ICON_OFF:
	case ICON_ON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE( dwMsg ))
				SetOn();
		}
		break;
	}
}