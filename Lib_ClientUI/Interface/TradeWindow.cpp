#include "StdAfx.h"
#include "TradeWindow.h"
#include "TradeControlPage.h"
#include "GLGaeaClient.h"
#include "InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTradeWindow::CTradeWindow () :
	m_pTarPage ( NULL ),
	m_pMyPgae ( NULL )
{
}

CTradeWindow::~CTradeWindow ()
{
}

void CTradeWindow::CreateSubControl ()
{
	m_pTarPage = CreateTradeControlPage ( "TRADE_CONTROL_TARPAGE", FALSE, TAR_TRADE_PAGE );
	CreateControl ( "TRADE_BLACK_LINE" );
	m_pMyPgae = CreateTradeControlPage ( "TRADE_CONTROL_MYPAGE", TRUE, MY_TRADE_PAGE );
}

CTradeControlPage*	CTradeWindow::CreateTradeControlPage ( char *szControl, BOOL bMyPage, UIGUID ControlID )
{
	CTradeControlPage* pTradeControlPage = new CTradeControlPage;
	pTradeControlPage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pTradeControlPage->CreateSubControl ( bMyPage );
	RegisterControl ( pTradeControlPage );
	return pTradeControlPage;
}

void CTradeWindow::SetTradeInfo ( CString strPlayerName, DWORD dwMasterID )
{		
	m_dwMasterID = dwMasterID;

	if ( m_pMyPgae ) 
	{
		CString strMyName = GLGaeaClient::GetInstance().GetCharacter()->m_szName;
		m_pMyPgae->SetPlayerName ( strMyName );
	}
	if ( m_pTarPage )
	{
		m_pTarPage->SetPlayerName ( strPlayerName );
	}
}

DWORD CTradeWindow::GetMasterID ()
{
	return m_dwMasterID;
}

void CTradeWindow::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	//	의도적으로 윈도우 이동을 막음
//	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
	switch ( ControlID )
	{
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetTradeWindowCloseReq ();
			}
		}
		break;

	case MY_TRADE_PAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nPosX, nPosY;
				m_pMyPgae->GetItemIndex ( &nPosX, &nPosY );

				if ( nPosX < 0 || nPosY < 0 ) return ;
//				if ( CUIMan::GetFocusControl () == this )
				{

					SINVENITEM sInvenItem = m_pMyPgae->GetItem ( nPosX, nPosY );
					if ( sInvenItem.sItemCustom.sNativeID != NATIVEID_NULL () )
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO_NEW ( sInvenItem.sItemCustom, FALSE, FALSE, FALSE,FALSE,FALSE, sInvenItem.wPosX, sInvenItem.wPosY );
					}

					/*item preview, Juver, 2017/07/27 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sInvenItem.sItemCustom );
							return;
						}
					}

					/*box contents, Juver, 2017/08/30 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_RB_UP )
						{
							CInnerInterface::GetInstance().ShowBoxContents( sInvenItem.sItemCustom.sNativeID );
							return;
						}
					}
				}
			}
		}
		break;

	case TAR_TRADE_PAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nPosX, nPosY;
				m_pTarPage->GetItemIndex ( &nPosX, &nPosY );

				if ( nPosX < 0 || nPosY < 0 ) return ;
//				if ( CUIMan::GetFocusControl () == this )
				{
					SINVENITEM sInvenItem = m_pTarPage->GetItem ( nPosX, nPosY );
					if ( sInvenItem.sItemCustom.sNativeID != NATIVEID_NULL () )
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO_NEW ( sInvenItem.sItemCustom, FALSE, FALSE, FALSE,FALSE,FALSE, sInvenItem.wPosX, sInvenItem.wPosY );
					}

					/*item preview, Juver, 2017/07/27 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_LB_UP )
						{
							CInnerInterface::GetInstance().PreviewItem( sInvenItem.sItemCustom );
							return;
						}
					}

					/*box contents, Juver, 2017/08/30 */
					if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU  ) & DXKEY_DOWNED )
					{
						if ( dwMsg & UIMSG_RB_UP )
						{
							CInnerInterface::GetInstance().ShowBoxContents( sInvenItem.sItemCustom.sNativeID );
							return;
						}
					}
				}
			}
		}
		break;
	}
}

HRESULT CTradeWindow::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;
	hr = CUIWindowEx::RestoreDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	const UIRECT& rcWindow = GetGlobalPos ();

	const long lResolution = CInnerInterface::GetInstance().GetResolution ();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	D3DXVECTOR2 vPos;
	vPos.x = ((X_RES) / 2.0f) - rcWindow.sizeX;
	vPos.y = (Y_RES - rcWindow.sizeY) / 2.0f;
	SetGlobalPos ( vPos );

	return S_OK;
}