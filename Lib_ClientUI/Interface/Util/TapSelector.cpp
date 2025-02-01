#include "StdAfx.h"

#include "../../../Lib_Engine/DxCommon/DxFontMan.h"
#include "../../../Lib_Engine/DxCommon/d3dfont.h"
#include "TapSelector.h"
#include "./BasicTextButton.h"

#include "./BasicLineBoxSmart.h"
#include "./BasicTextButton.h"
#include "./BasicTextBox.h"

#include "./CheckBox.h"
#include "TapSelectorEx.h"

#include "UITextControl.h"

CTapSelector::CTapSelector ()
	: m_SelectedTapID( 0 )
{
}

CTapSelector::~CTapSelector()
{
	if ( !m_MapTapButton.empty() )
	{
		m_MapTapButton.clear();
	}

	m_SelectedTapID = 0;
}

CBasicTextButton* CTapSelector::CreateTapButton ( CUIGroup* pParent, const char* szKeyword, const char* szControl, const char* szText, int nType, UIGUID WndID )
{
	std::map<UIGUID, CBasicTextButton*>::iterator Finditer = m_MapTapButton.find( WndID );
	if ( Finditer != m_MapTapButton.end() )
	{
		return NULL;
	}

	CBasicTextButton* pTapButton = new CBasicTextButton();
	pTapButton->CreateSub( pParent, szKeyword, UI_FLAG_XSIZE, WndID );
	pTapButton->CreateBaseButton( szControl, CBasicTextButton::SIZE19, CBasicButton::RADIO_FLIP, szText );
	pParent->RegisterControl( pTapButton );

	m_MapTapButton.insert( std::pair<UIGUID, CBasicTextButton*>(WndID, pTapButton) );

	return pTapButton;
}

void CTapSelector::TapSelectorUIMessage ( const UIGUID ControlID, const DWORD dwMsg, EMTAPSELECTORRESULT& emOutResult )
{
	emOutResult = EMTAPSELECTORRESULT_NONE;

	if ( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
	{
		if ( !TapSelect ( ControlID ) )
		{
			return;
		}

		emOutResult = EMTAPSELECTORRESULT_SELECT;
	}
}

const BOOL CTapSelector::TapSelect ( const UIGUID ControlID )
{
	BOOL bFind = FALSE;

	std::map<UIGUID, CBasicTextButton*>::iterator iter = m_MapTapButton.begin();
	for ( ; iter!=m_MapTapButton.end(); ++iter )
	{
		if ( iter->first == ControlID )
		{
			TapEnable ( iter->second );
			bFind = TRUE;
		}
		else
		{
			TapDisable ( iter->second );
		}
	}

	if ( bFind )
	{
		m_SelectedTapID = ControlID;
		return TRUE;
	}

	return FALSE;
}

void CTapSelector::TapEnable ( CBasicTextButton* pTap )
{
	if ( pTap )
	{
		pTap->SetFlip( TRUE );
	}
}

void CTapSelector::TapDisable ( CBasicTextButton* pTap )
{
	if ( pTap )
	{
		pTap->SetFlip( FALSE );
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void DEFAULT_TAPBUTTON::CreateSubControl ( const std::string& strLabel,
										   const std::string& activeBaseID,
										   const std::string& activeSubID,
										   const std::string& inactiveBaseID,
										   const std::string& inactiveSubID,
										   const std::string& textBoxID)
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	pActiveLineBox = new CBasicLineBoxSmart();
	pActiveLineBox->CreateSub( this, activeBaseID.c_str() );
	pActiveLineBox->CreateSubControl( activeSubID.c_str() );
	RegisterControl( pActiveLineBox );

	pInactive = new CBasicLineBoxSmart();
	pInactive->CreateSub( this, inactiveBaseID.c_str() );
	pInactive->CreateSubControl( inactiveSubID.c_str() );
	RegisterControl( pInactive );

	pTextBox = new CBasicTextBox();
	pTextBox->CreateSub( this, textBoxID.c_str() );
	pTextBox->SetFont( pFont );
	pTextBox->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	pTextBox->SetText( strLabel.c_str() );
	pTextBox->SetUseTextColor( 0, TRUE );
	RegisterControl( pTextBox );
}

void DEFAULT_TAPBUTTON::SetEnable( const bool bEnable )
{
    if( pActiveLineBox == NULL || pTextBox == NULL || pInactive == NULL )
        return;

	pEnable = bEnable;

	if ( bEnable )
	{
		pActiveLineBox->SetVisibleSingle( TRUE );
		pInactive->SetVisibleSingle( FALSE );
		pTextBox->SetTextColor(0, NS_UITEXTCOLOR::BLACK);
	}
	else
	{
		pActiveLineBox->SetVisibleSingle( FALSE );
		pInactive->SetVisibleSingle( TRUE );
		pTextBox->SetTextColor(0, NS_UITEXTCOLOR::WHITE);
	}
}

void DEFAULT_TAPBUTTON::SetTabLabel( const std::string& strLabel )
{
	if( pTextBox )
	{
		pTextBox->SetOneLineText( strLabel.c_str() );
		pTextBox->SetUseTextColor( 0, TRUE );
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void DEFAULT_RADIO_BUTTON::CreateSubControl( const std::string& text, 
											 DWORD align,
											 const std::string& textID,
											 DWORD textColor,
											 const std::string& radioButtonID,
											 const std::string& onButton,
											 const std::string& offButton )
{	
	if(!text.empty())
	{
		CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
		pTextBox = new CBasicTextBox();
		pTextBox->CreateSub( this, textID.c_str() );
		pTextBox->SetFont( pFont );
		pTextBox->SetTextAlign( align );
		pTextBox->SetText( text.c_str() , textColor);
		RegisterControl( pTextBox );
	}

	pCheckBox = new CCheckBox();
	pCheckBox->CreateSub ( this, radioButtonID.c_str());
	pCheckBox->CreateSubControl( onButton.c_str(), offButton.c_str() );
	// ��ü ������Ʈ�� üũ �ڽ��� �����°� ����
	pCheckBox->SetNoUpdate(TRUE);
	RegisterControl ( pCheckBox );
}

void DEFAULT_RADIO_BUTTON::CreateSubControl( const std::string& text, 
                                            DWORD align,
                                            const std::string& textID,
                                            DWORD textColor,
                                            const std::string& radioButtonID,
                                            const std::string& onButton,
                                            const std::string& offButton,
                                            CD3DFontPar* pFont )
{	
    if(!text.empty())
    {
        pTextBox = new CBasicTextBox();
        pTextBox->CreateSub( this, textID.c_str() );
        pTextBox->SetFont( pFont );
        pTextBox->SetTextAlign( align );
        pTextBox->SetText( text.c_str() , textColor);
        RegisterControl( pTextBox );
    }

    pCheckBox = new CCheckBox();
    pCheckBox->CreateSub ( this, radioButtonID.c_str());
    pCheckBox->CreateSubControl( onButton.c_str(), offButton.c_str() );
    // ��ü ������Ʈ�� üũ �ڽ��� �����°� ����
    pCheckBox->SetNoUpdate(TRUE);
    RegisterControl ( pCheckBox );
}

void DEFAULT_RADIO_BUTTON::SetEnable( const bool bEnable )
{
	pCheckBox->SetCheck(bEnable);
}

void DEFAULT_RADIO_BUTTON::SetText( const std::string& text, DWORD textColor )
{
    pTextBox->SetText( text.c_str() , textColor);
}

void TAPBUTTON_RNBUTTON::SetEnable(bool isEnable)
{
	if(isEnable)
	{
//		SetNoUpdate(TRUE);
		RnButton::SetEnable(false);
		SetState(MOUSECLICK);
	}
	else
	{
		RnButton::SetEnable(true);
		SetState(DEFAULT);
//		SetNoUpdate(FALSE);
	}
}






