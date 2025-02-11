//	마켓 윈도우
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.12.6]
//			@ 작성
//

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "GLDefine.h"
#include "GLCharDefine.h"

class	CItemSlot;
class	CBasicScrollBarEx;
class	GLInventory;
struct	SINVENITEM;

class	CMarketPage : public CUIGroup
{
private:
static	const	int	nSTARTLINE;
static	const	int	nOUTOFRANGE;

protected:
	enum
	{		
		nMAX_ONE_VIEW_SLOT = 8
	};

	enum
	{
		ITEM_SLOT0 = NO_ID + 1,
		ITEM_SLOT1,
		ITEM_SLOT2,
		ITEM_SLOT3,
		ITEM_SLOT4,
		ITEM_SLOT5,
		ITEM_SLOT6,
		ITEM_SLOT7,
		ITEM_SLOT_END = ITEM_SLOT0 + SALE_INVEN_Y,
	};

public:
	CMarketPage ();
	virtual	~CMarketPage ();

public:
	void CreateSubControl ();

public:
	void	SetItemIndex ( int nPosX, int nPosY );
	void	GetItemIndex ( int* pnPosX, int* pnPosY );

public:
	void	LoadItemPage ( GLInventory &ItemData );
	void	LoadItem ( SINVENITEM& ref_InvenItem );

	SINVENITEM&	GetItem ( int nPosX, int nPosY );

    CItemSlot* GetItemSlot( int nRow );

public:
	void	UnLoadItemPage ();
	void	UnLoadItem ( int nPosX, int nPosY );

    void	SetVisibleScrollbar( bool bVisible );

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

protected:
	void	ResetAllItemSlotRender ( int nTotal );
	void	SetItemSlotRender ( int nStartIndex, int nTotal );

protected:
	CItemSlot*	CreateItemSlot ( CString strKeyword, UIGUID ControlID );
	CUIControl*	CreateControl ( const char* szControl );

private:
	int		m_nPosX;
	int		m_nPosY;
	int		m_nCurPos;

private:
	CItemSlot*	m_pItemSlotArray[SALE_INVEN_Y];
	CUIControl*	m_pItemSlotArrayDummy[nMAX_ONE_VIEW_SLOT];

	CBasicScrollBarEx*	m_pScrollBar;
};