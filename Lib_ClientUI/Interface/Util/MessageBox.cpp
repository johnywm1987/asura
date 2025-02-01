#include "stdafx.h"
#include "MessageBox.h"

#include "./RnButton.h"
#include "./GameTextControl.h"
#include "./UIWindowTitle.h"
#include "./UITextControl.h"
#include "./BasicLineBoxSmart.h"
#include "./BasicTextBox.h"
#include "./DxFontMan.h"
#include "./d3dfont.h"

#include "./UIEditBoxMan.h"

CMessageBox::CMessageBox()
    : m_nIdentity( 0 )
    , m_pTitle( NULL )
    , m_pMessage( NULL )
    , m_pOk_Center( NULL )
    , m_pYes( NULL )
    , m_pNo( NULL )
    , m_pOk( NULL )
    , m_pCancel( NULL )
    , m_bIsClick( false )
	, m_nTranslateLoopCount( 0 )
{
}

void CMessageBox::CreateSubControl( const BOXSTYLE eBoxStyle /*= STYLE_DEFAULT*/ )
{
    CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );

    UIRECT rcUIRect = GetParent()->GetLocalPos();
    UIRECT rcUIRectThis = GetLocalPos();
    
    float fX = rcUIRect.sizeX/2 - rcUIRectThis.sizeX/2;
    float fY = rcUIRect.sizeY/2 - rcUIRectThis.sizeY/2;

    this->SetLocalPos( D3DXVECTOR2(fX,fY) );

    CBasicLineBoxSmart* pLineBox;
    pLineBox = new CBasicLineBoxSmart(  );
    pLineBox->CreateSub( this, "UTILE_MESSAGEBOX_TITLE", UI_FLAG_DEFAULT );
    pLineBox->CreateSubControl( "UIUTILE_WINDOW_TITLE_TEXTURE" );
    RegisterControl( pLineBox );

    pLineBox = new CBasicLineBoxSmart(  );
    pLineBox->CreateSub( this, "UTILE_MESSAGEBOX_BG", UI_FLAG_DEFAULT );
    pLineBox->CreateSubControl( "PARTY_WINDOW_RENEWAL_TEXTURE_INFO" );
    RegisterControl( pLineBox );

    pLineBox = new CBasicLineBoxSmart(  );
    pLineBox->CreateSub( this, "UTILE_MESSAGEBOX_BG", UI_FLAG_DEFAULT );
    pLineBox->CreateSubControl( "UIUTILE_WINDOW_BG_TEXTURE" );
    RegisterControl( pLineBox );


    m_pTitle = new CBasicTextBox(  );
    m_pTitle->CreateSub ( this, "UTILE_MESSAGEBOX_TITLE_TEXT", UI_FLAG_DEFAULT );
    m_pTitle->SetFont ( pFont10 );
    m_pTitle->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
    RegisterControl ( m_pTitle );

    m_pMessage = new CBasicTextBox(  );
    m_pMessage->CreateSub ( this, "UTILE_MESSAGEBOX_MAINTEXT", UI_FLAG_DEFAULT );
    m_pMessage->SetFont ( pFont10 );
    m_pMessage->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_CENTER_Y );	
    RegisterControl ( m_pMessage );
    
    pLineBox = new CBasicLineBoxSmart(  );
    pLineBox->CreateSub( this, "UTILE_MESSAGEBOX_INPUTBOX_BG", UI_FLAG_DEFAULT, ID_TEXT_BACKGROUND );
    pLineBox->CreateSubControl( "GRAY_BACKGROUND_BLACK_LINE_TEXTURE" );
    pLineBox->SetVisibleSingle( FALSE );
    RegisterControl( pLineBox );
    m_pEditBoxBG = pLineBox;

    CUIEditBoxMan* pEditBoxMan = new CUIEditBoxMan();
    pEditBoxMan->CreateSub( this, "UTILE_MESSAGEBOX_INPUTBOX_SIZE", UI_FLAG_DEFAULT, ID_TEXT_BOX );
    pEditBoxMan->CreateEditBox( ID_TEXT_TEXTMAN, "UTILE_MESSAGEBOX_INPUTBOX", "UTILE_MESSAGEBOX_CARRAT", TRUE, UINT_MAX, pFont10, 16 );
    pEditBoxMan->SetVisibleSingle( FALSE );
    RegisterControl( pEditBoxMan );
    m_pEditBox = pEditBoxMan;

    //��ư
    RnButton::CreateArg argStyle;
    switch( eBoxStyle )
    {
    case STYLE_DEFAULT:
        // �⺻ ���
        argStyle.pFont = pFont10;
        argStyle.defaultTextureKey = "RNBUTTON_DEFAULT_TEXTURE";
        argStyle.mouseOverTextureKey = "RNBUTTON_OVER_TEXTURE";
        argStyle.mouseClickTextureKey = "RNBUTTON_CLICK_TEXTURE";
        break;
    }
    
    m_pOk_Center = new RnButton(  );
    m_pOk_Center->CreateSub( this, "UTILE_MESSAGEBOX_BUTTON_POSITIVE_CENTER", UI_FLAG_DEFAULT, ID_BTN_OK_CENTER );
    argStyle.text = ID2GAMEWORD("MODAL_BUTTON", 0);
    m_pOk_Center->CreateSubControl( argStyle );
    m_pOk_Center->SetVisibleSingle( FALSE );
    RegisterControl(m_pOk_Center);
    
    m_pOk = new RnButton(  );
    m_pOk->CreateSub( this, "UTILE_MESSAGEBOX_BUTTON_POSITIVE", UI_FLAG_DEFAULT, ID_BTN_OK );
    argStyle.text = ID2GAMEWORD("MODAL_BUTTON", 0);
    m_pOk->CreateSubControl( argStyle );
    m_pOk->SetVisibleSingle( FALSE );
    RegisterControl(m_pOk);
    
    m_pCancel = new RnButton(  );
    m_pCancel->CreateSub( this, "UTILE_MESSAGEBOX_BUTTON_NEGATIVE", UI_FLAG_DEFAULT, ID_BTN_CANCEL );
    argStyle.text = ID2GAMEWORD("MODAL_BUTTON", 1);
    m_pCancel->CreateSubControl( argStyle );
    m_pCancel->SetVisibleSingle( FALSE );
    RegisterControl(m_pCancel);

    m_pYes = new RnButton(  );
    m_pYes->CreateSub( this, "UTILE_MESSAGEBOX_BUTTON_POSITIVE", UI_FLAG_DEFAULT, ID_BTN_YES );
    argStyle.text = ID2GAMEWORD("MODAL_BUTTON", 2);
    m_pYes->CreateSubControl( argStyle );
    m_pYes->SetVisibleSingle( FALSE );
    RegisterControl(m_pYes);
    
    m_pNo = new RnButton(  );
    m_pNo->CreateSub( this, "UTILE_MESSAGEBOX_BUTTON_NEGATIVE", UI_FLAG_DEFAULT, ID_BTN_NO );
    argStyle.text = ID2GAMEWORD("MODAL_BUTTON", 3);
    m_pNo->CreateSubControl( argStyle );
    m_pNo->SetVisibleSingle( FALSE );
    RegisterControl(m_pNo);

    this->SetVisibleSingle( FALSE );
}

void CMessageBox::OpenMessageBox( const std::string& strTitle, const std::string& strMessage, const BOXTYPE eBoxType /*= TYPE_OKCANCEL*/, unsigned int nIdentity /*= 0*/ )
{

    SetBoxType( eBoxType );

    SetTitleText( strTitle );
    SetMessageText( strMessage );
    SetVisibleSingleTrue( nIdentity );
}

void CMessageBox::OpenMessageBox( const BOXTITLE eBoxTitle, const std::string& strMessage, const BOXTYPE eBoxType /*= TYPE_OKCANCEL*/, unsigned int nIdentity /*= 0*/ )
{
    SetBoxType( eBoxType );

    SetTitleText( GetBoxTitle( eBoxTitle ) );
    SetMessageText( strMessage );
    SetVisibleSingleTrue( nIdentity );
}

void CMessageBox::OpenMessageBoxMultiLine( const BOXTITLE eBoxTitle, const std::vector<std::string>& vecMessage, const BOXTYPE eBoxType /*= TYPE_OKCANCEL*/, unsigned int nIdentity /*= 0 */ )
{
    SetBoxType( eBoxType );

    SetTitleText( GetBoxTitle( eBoxTitle ) );
    SetMessageText( vecMessage );
    SetVisibleSingleTrue( nIdentity );
}

void CMessageBox::OpenMessageBoxMultiLine( const std::string& strTitle, const std::vector<std::string>& vecMessage, const BOXTYPE eBoxType /*= TYPE_OKCANCEL*/, unsigned int nIdentity /*= 0 */ )
{
    SetBoxType( eBoxType );

    SetTitleText( strTitle );
    SetMessageText( vecMessage );
    SetVisibleSingleTrue( nIdentity );
}

unsigned int CMessageBox::GetIdentifier()
{
    if( m_bIsClick == true)
    {
        m_bIsClick = false;
        return m_nIdentity;
    }
    else
        return 0;
}   

std::string CMessageBox::GetInputString()
{
    return m_strInputText;
}

void CMessageBox::CloseMessageBox()
{
    SetVisibleSingleFalse();
}

void CMessageBox::SetTitleText( const std::string strTitle )
{
    m_pTitle->ClearText();
    m_pTitle->SetText( strTitle.c_str() );
}

void CMessageBox::SetMessageText( const std::string strMessage )
{
    m_pMessage->ClearText();
    m_pMessage->SetText( strMessage.c_str() );
}

void CMessageBox::SetMessageText(const std::vector<std::string>& vecMessage)
{
    m_pMessage->ClearText();
    for (const std::string& value : vecMessage)
    {
        m_pMessage->AddText(value.c_str(), NS_UITEXTCOLOR::ENABLE);
    }
}

void CMessageBox::SetVisibleSingleTrue( unsigned int nIdentity )
{
    if( this->IsVisible() == TRUE )
        GASSERT( 0 && "�̹� �޽��� �ڽ��� ���� �ִ� ����");

    this->SetVisibleSingle( TRUE );
    m_nIdentity = nIdentity;

	ResetMessageEx();
/*    SetExclusiveControl();*/
}

void CMessageBox::SetVisibleSingleFalse()
{ 
    m_pEditBoxBG->SetVisibleSingle( FALSE );
    m_pEditBox->SetVisibleSingle( FALSE );
    m_pOk_Center->SetVisibleSingle( FALSE );
    m_pOk->SetVisibleSingle( FALSE );
    m_pCancel->SetVisibleSingle( FALSE );  
    m_pYes->SetVisibleSingle( FALSE );
    m_pNo->SetVisibleSingle( FALSE );

    m_pEditBox->ClearEdit( ID_TEXT_TEXTMAN );
    this->SetVisibleSingle( FALSE );

	m_nTranslateLoopCount = 0;
/*    ResetExclusiveControl();*/
}

void CMessageBox::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
    CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CMessageBox::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
    //////////////////////////////////////////////////////////////////////////
    // �� �޽��� �ڽ��� �ɹ��� ���� �����ϴ� ��� �ϴ� ���
    // ������ CMessageBox::CreateSub() �Լ����� ID�� �ο����� ������ 
    // TranslateUIMessage �޼������� UIMSG_MESSAGEBOX_POSITIVE��
    // ���� �� ����.
    // ������ ID�� �ο����� �ʴ� ��� TranslateUIMessage�� Loop ��Ű�� �ʱ� �����̴�.
    //
    CUIGroup::TranslateUIMessage( ControlID, dwMsg );
 
	if( ID_BTN_OK_CENTER <= ControlID && ControlID <= ID_BTN_NO )
	{
		if( m_nTranslateLoopCount < 1 )
		{
			m_nTranslateLoopCount++;
			return;
		}
	}
	

	if( ControlID == ID_TEXT_BACKGROUND )
        if( !CHECK_MOUSE_IN( dwMsg ) && CHECK_LB_DOWN_LIKE( dwMsg ) )
            m_pEditBox->EndEdit();


    switch( ControlID )
    {
    case ID_BTN_OK:
    case ID_BTN_YES:
        if( m_nIdentity == 0 )
            return;

    case ID_BTN_OK_CENTER:
        if( CHECK_MOUSE_IN ( dwMsg ) && CHECK_LB_UP_LIKE ( dwMsg ))
        {
            if( m_pEditBox->IsVisible() )
                m_strInputText = m_pEditBox->GetEditString().GetString();

            m_bIsClick = true;
            SetVisibleSingleFalse();
            AddMessageEx( UIMSG_MESSAGEBOX_POSITIVE );
        }

        break;
    case ID_BTN_CANCEL:
    case ID_BTN_NO:
        if( m_nIdentity == 0 )
            return;

        if( CHECK_MOUSE_IN ( dwMsg ) && CHECK_LB_UP_LIKE ( dwMsg ))
        {
             m_bIsClick = true;
            SetVisibleSingleFalse();
            AddMessageEx( UIMSG_MESSAGEBOX_NEGATIVE );
        }
        break;

    }


}

void CMessageBox::SetVisibleSingle( BOOL bVisible )
{
    CUIGroup::SetVisibleSingle( bVisible );
}

std::string CMessageBox::GetBoxTitle( const BOXTITLE& eBoxTitle )
{
    switch( eBoxTitle )
    {
    case TITLE_ANNOUNCEMENT:
        return ID2GAMEWORD("MODAL_TITLE", 0);
        break;
    case TITLE_QUESTION:
        return ID2GAMEWORD("MODAL_TITLE", 1);
        break;
    case TITLE_WARNING:
        return ID2GAMEWORD("MODAL_TITLE", 2);
        break;
    case TITLE_ERROR:
        return ID2GAMEWORD("MODAL_TITLE", 3);
        break;
    case TITLE_CRITICAL:
        return ID2GAMEWORD("MODAL_TITLE", 4);
        break;
    case TITLE_ENTER:
        return ID2GAMEWORD("MODAL_TITLE", 5);
        break;
    } 

    return "ERROR";

}

void CMessageBox::SetBoxType( const BOXTYPE& eBoxType )
{
    switch( eBoxType )
    {
    case TYPE_OK:
        m_pOk_Center->SetVisibleSingle( TRUE );
        break;
    case TYPE_OKCANCEL:
        m_pOk->SetVisibleSingle( TRUE );
        m_pCancel->SetVisibleSingle( TRUE );
        break;
    case TYPE_YESNO:
        m_pYes->SetVisibleSingle( TRUE );
        m_pNo->SetVisibleSingle( TRUE );
        break;
    case TYPE_INPUT:
        m_pEditBoxBG->SetVisibleSingle( TRUE );
        m_pEditBox->SetVisibleSingle( TRUE );
        m_pOk->SetVisibleSingle( TRUE );
        m_pCancel->SetVisibleSingle( TRUE );
        break;
    }

}
