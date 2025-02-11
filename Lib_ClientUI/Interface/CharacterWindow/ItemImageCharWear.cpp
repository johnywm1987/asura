#include "StdAfx.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "SkillFunc.h"
#include "BasicProgressBar.h"
#include "BasicTextBox.h"
#include "UITextControl.h"
#include "d3dfont.h"

#include "ItemImageCharWear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	리차징 컬러
const DWORD	RECHARGECOLOR = D3DCOLOR_ARGB(150,0,0,0);

CItemImageCharWear::CItemImageCharWear () 
	: m_pItemImage ( NULL )
	, m_pd3dDevice ( NULL )
	, m_sICONINDEX ( NATIVEID_NULL () )
	, m_TotElapsedTime(0.0f)
	, m_bItemUsed(false)
	, m_bItemEnd(false)
	, m_bTwinkle(true)
	, m_pItemProcess( NULL )
	, m_sNativeID(false)
	, m_pTextBox(NULL)
{
}

CItemImageCharWear::~CItemImageCharWear ()
{
}

HRESULT CItemImageCharWear::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	m_pd3dDevice = pd3dDevice;

	return S_OK;
}

void	CItemImageCharWear::CreateSubControl ()
{

	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pItemImage = new CUIControl;
	m_pItemImage->CreateSub ( this, "ITEM_IMAGE" );	
	m_pItemImage->SetUseRender ( TRUE );
	m_pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pItemImage );

	CBasicProgressBar* pItemProcess = new CBasicProgressBar;
	pItemProcess->CreateSub ( this, "ITEM_IMAGE_PROGRESS" );
	pItemProcess->CreateOverImage ( "ITEM_IMAGE_PROGRESS_OVER" );
	pItemProcess->SetType ( CBasicProgressBar::VERTICAL );
	pItemProcess->SetOverImageUseRender ( TRUE );
	pItemProcess->SetOverImageDiffuse ( RECHARGECOLOR );
	pItemProcess->SetControlNameEx ( "프로그래스바" );	
	RegisterControl ( pItemProcess );
	m_pItemProcess = pItemProcess;

	m_pTextBox = new CBasicTextBox;
	m_pTextBox->CreateSub ( this, "ITEM_NUMBER0" );
	m_pTextBox->SetFont ( pFont8 );
	m_pTextBox->SetTextAlign ( TEXT_ALIGN_RIGHT );
	RegisterControl ( m_pTextBox );
}

void CItemImageCharWear::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if( m_bItemUsed && m_bItemEnd )
	{
		m_TotElapsedTime += fElapsedTime;

		if( m_TotElapsedTime < 0.3f )		m_pItemProcess->SetPercent(0);
		else if( m_TotElapsedTime < 0.6f )	m_pItemProcess->SetPercent(1);
		else if( m_TotElapsedTime < 0.9f )	m_pItemProcess->SetPercent(0);
		else if( m_TotElapsedTime < 1.2f )	m_pItemProcess->SetPercent(1);
		else
		{
			m_pItemProcess->SetPercent(0);
			m_TotElapsedTime = 0.0f;
			m_bItemEnd = false;
			m_bItemUsed = false;
		}
	}

	//	쿨타임 표시
    if ( !m_pItemProcess )
	{
		GASSERT ( 0 && "프로그래스바가 만들어지지 않았습니다." );
		return ;
	}

	SITEM* pItem = GLItemMan::GetInstance().GetItem( m_sNativeID );
	if ( !pItem )	return ;

	if ( !GLGaeaClient::GetInstance().GetCharacter()->IsCoolTime( m_sNativeID ) )
	{
		m_bItemEnd = true;
		return;
	}	

	ITEM_COOLTIME* pItemCoolTime = GLGaeaClient::GetInstance().GetCharacter()->GetCoolTime( m_sNativeID );
	if ( !pItemCoolTime )
	{
		m_bItemEnd = true;
		return;
	}

	m_bItemEnd = false;

	__time64_t tCurTime = GLGaeaClient::GetInstance().GetCurrentTime().GetTime();
	__time64_t tUseTime = pItemCoolTime->tUseTime;
	__time64_t tCoolTime = pItemCoolTime->tCoolTime;

	float fCoolTime = tCoolTime - tUseTime;
	float fCurTime = fCoolTime - ( tCurTime - tUseTime );
	
	if ( fCoolTime == 0.0f )
	{
		fCoolTime = 1.0f;
	}

	const float fPercent = fCurTime / fCoolTime;
	m_pItemProcess->SetPercent ( fPercent );
	m_bItemUsed = true;
}

void CItemImageCharWear::SetItem ( SNATIVEID sICONINDEX, const char* szTexture, int nGrind, int wPileNum, int wTurnNum, SNATIVEID sNativeID /*= false*/ )
{	
	if ( !m_pItemImage )	return;

	//	NOTE
	//		이전 프레임과 아이콘 ID가 같고,
	//		텍스쳐가 동일하다면 같은 이미지 이므로,
	//		더 이상 작업을 수행할 필요가 없다.

	if( m_sNativeID == sNativeID && sNativeID != NATIVEID_NULL() )	return;

	if ( (sICONINDEX == m_sICONINDEX) && (szTexture == m_pItemImage->GetTextureName ()) )	return ;


	ResetItem ();
	SetItemProcess( sNativeID );

	if ( m_pItemImage )
	{
		m_pItemImage->SetTexturePos ( NS_SKILL::GetIconTexurePos ( sICONINDEX ) );
		m_pItemImage->SetTextureName ( szTexture );	

		if ( m_pd3dDevice )
		{	
			m_pItemImage->InitDeviceObjects ( m_pd3dDevice );
			m_pItemImage->SetVisibleSingle ( TRUE );

			m_sICONINDEX = sICONINDEX;
			m_strTextureName = szTexture;
		}
	}

	if ( m_pTextBox && nGrind >= 1 && nGrind <= 4 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::RED );
	}else if ( m_pTextBox && nGrind == 5 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::PALEGOLDENROD );
	}else if ( m_pTextBox && nGrind == 6 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::TOMATO );
	}else if ( m_pTextBox && nGrind >= 7 && nGrind <= 8 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::DARKORANGE );
	}else if ( m_pTextBox && nGrind == 9 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::GOLD );
	}else if ( m_pTextBox && nGrind == 10 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::DARKTURQUOISE );
	}else if ( m_pTextBox && nGrind == 11 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::DARKVIOLET );
	}else if ( m_pTextBox && nGrind >= 12 && nGrind <= 15 ){
		CString strGrind;
		strGrind.Format( "+%d", nGrind );
		m_pTextBox->SetOneLineText( strGrind.GetString(),NS_UITEXTCOLOR::DARKVIOLET );
	}
	else{
        m_pTextBox->SetOneLineText( "+0",NS_UITEXTCOLOR::WHITE );

	  }      

}

SNATIVEID	 CItemImageCharWear::GetItem ()
{
	return m_sICONINDEX;
}

void CItemImageCharWear::ResetItem ()
{
	if ( m_pItemImage )
	{
		if ( m_sICONINDEX != NATIVEID_NULL () )
		{
			m_pItemImage->DeleteDeviceObjects ();
			m_pItemImage->SetTextureName ( NULL );
			m_pItemImage->SetVisibleSingle ( FALSE );

			m_sICONINDEX = NATIVEID_NULL ();

			ReSetItemProcess();

			m_strTextureName.Empty ();
		}		
	}

	if ( m_pTextBox )
	{
		m_pTextBox->ClearText();
	}
}

const CString&	CItemImageCharWear::GetItemTextureName () const
{
	return m_strTextureName;
}

void CItemImageCharWear::SetItemProcess( SNATIVEID sNativeID )
{
	m_bItemUsed = false;	
	m_sNativeID = sNativeID;

	if ( sNativeID == NATIVEID_NULL() )
	{
		m_pItemProcess->SetVisibleSingle( FALSE );
	}
	else
	{
		m_pItemProcess->SetVisibleSingle( TRUE );
	}

	return;
}

void CItemImageCharWear::ReSetItemProcess()
{
	m_bItemUsed = false;
	m_sNativeID = NATIVEID_NULL();
	m_pItemProcess->SetVisibleSingle( FALSE );
	m_pItemProcess->SetPercent( 0.0f );
	
	return;
}

void CItemImageCharWear::SetDiffuse(D3DCOLOR _diffuse)
{
	CUIGroup::SetDiffuse( _diffuse );

	m_pItemProcess->SetOverImageDiffuse ( RECHARGECOLOR );
}