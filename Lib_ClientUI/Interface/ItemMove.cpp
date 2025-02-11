#include "StdAfx.h"
#include "ItemMove.h"
#include "ItemImage.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD CItemMove::dwDEFAULT_TRANSPARENCY = D3DCOLOR_ARGB ( 160, 255, 255, 255 );
const float CItemMove::fDEFAULT_MOUSE_INTERPOLIATION = 20.0f;

CItemMove::CItemMove () :
	m_pItemImage ( NULL )
{
}

CItemMove::~CItemMove ()
{
}

void CItemMove::CreateSubControl ()
{
	m_pItemImage = new CItemImage;
	m_pItemImage->CreateSub ( this, "SKILL_WINDOW_TO_TARY_IMAGE" );
	m_pItemImage->CreateSubControl ();
	m_pItemImage->SetUseRender ( TRUE );
	m_pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pItemImage );

	ResetItemIcon ();
}

SNATIVEID	CItemMove::GetItem ()
{
	//	Note : 트래이드에 등록할 아이템이 있을 경우.
	//
	SITEMCUSTOM sItemCustom;
	sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_PRETRADE_ITEM();

	// 트레이드에 등록할 아이템이 없을 경우에는 개조에 등록할 아이템을 뿌린다	// ITEMREBUILD_MARK
	if( sItemCustom.sNativeID == NATIVEID_NULL() )
		sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_PREHOLD_ITEM();

	//	Note : 트레이드에 등록할 아이템과 개조에 등록할 아이템 모두 없을 경우에는 손에든 아이템을 뿌려줌.
	//
	if ( sItemCustom.sNativeID == NATIVEID_NULL() )
		sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM();

	return sItemCustom.sNativeID;
}

void CItemMove::SetItemIcon( const SITEMCUSTOM& sItemCustom )
{	
	if ( m_pItemImage )
	{
		m_pItemImage->SetItem ( sItemCustom );
		m_pItemImage->SetVisibleSingle ( TRUE );
		m_pItemImage->SetDiffuse ( dwDEFAULT_TRANSPARENCY );
	}
}

void  CItemMove::ResetItemIcon ()
{
	if ( m_pItemImage )
	{
		m_pItemImage->SetVisibleSingle ( FALSE );
		m_pItemImage->ResetItem ();
	}
}

SNATIVEID	CItemMove::GetItemIcon ()
{
	if ( m_pItemImage )
	{
		return m_pItemImage->GetItem ();
	}
	return NATIVEID_NULL();
}

const CString&	CItemMove::GetItemIconTexture () const
{
	if ( m_pItemImage )
	{
		return m_pItemImage->GetItemTextureName ();
	}

	static CString strNULL;
	return strNULL;
}

void CItemMove::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );	
	
	if ( !m_pItemImage ) return ;

	//	Note : 트래이드에 등록할 아이템이 있을 경우.
	//
	SITEMCUSTOM sItemCustom;
	sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_PRETRADE_ITEM();

	// 트레이드에 등록할 아이템이 없을 경우에는 개조에 등록할 아이템을 뿌린다	// ITEMREBUILD_MARK
	if( sItemCustom.sNativeID == NATIVEID_NULL() )
		sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_PREHOLD_ITEM();

	//	Note : 트레이드에 등록할 아이템과 개조에 등록할 아이템 모두 없을 경우에는 손에든 아이템을 뿌려줌.
	//
	if ( sItemCustom.sNativeID == NATIVEID_NULL() )
		sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM();

	if ( sItemCustom.sNativeID != NATIVEID_NULL () )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		if ( pItemData ) 
		{
			if ( pItemData->sBasicOp.sICONID != GetItemIcon () ||
				 pItemData->sBasicOp.strInventoryFile.c_str() != GetItemIconTexture () )
			{
				SetItemIcon ( sItemCustom );		
			}

			/*item wrapper, Juver, 2018/01/12 */
			/*if ( pItemData->sBasicOp.emItemType == ITEM_WRAPPER_BOX )
			{
				SITEM* pitem_disguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
				if ( pitem_disguise )
				{
					if ( pitem_disguise->sBasicOp.sICONID != GetItemIconBack () ||
						pitem_disguise->sBasicOp.strInventoryFile.c_str() != GetItemIconTextureBack () )
					{
						SetItemIconBack ( pitem_disguise->sBasicOp.sICONID, pitem_disguise->sBasicOp.strInventoryFile.c_str() );		
					}
				}
			}*/

			if ( !CInnerInterface::GetInstance().IsSnapItem () )
			{
				const D3DXVECTOR2 vImagePos ( x - fDEFAULT_MOUSE_INTERPOLIATION, y - fDEFAULT_MOUSE_INTERPOLIATION );
				SetGlobalPos ( vImagePos );
			}

			//	만약, 모달이 떠 있는 상태라면
			//	손에 든 이미지를 잠시 꺼 둡니다.
			if ( CInnerInterface::GetInstance().IsVisibleGroup ( MODAL_WINDOW ) )
			{
				m_pItemImage->SetVisibleSingle ( FALSE );			
			}
			else
			{
				m_pItemImage->SetVisibleSingle ( TRUE );
				m_pItemImage->SetDiffuse ( dwDEFAULT_TRANSPARENCY );
			}
		}
	}
	else
	{
		ResetItemIcon ();
	}
}

