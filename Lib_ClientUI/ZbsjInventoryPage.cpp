#include "StdAfx.h"
#include "ZbsjInventoryPage.h"

#include "ItemSlotEx.h"
#include "BasicScrollBarEx.h"
#include "BasicScrollThumbFrame.h"
#include "GLGaeaClient.h"

///////
#include "DxFontMan.h"
#include "BasicTextBox.h"
#include "BasicButton.h"
#include "GameTextControl.h"
//////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const INT CZbsjInventoryPage::nSTARTLINE = 0;
const INT CZbsjInventoryPage::nOUTOFRANGE = -1;

CZbsjInventoryPage::CZbsjInventoryPage()
	: m_pScrollBar(NULL)
	, m_nONE_VIEW_SLOT(nONE_VIEW_SLOT_DEFAULT)
	, m_nCurPos(-1)
{
	//////////////
	memset(m_pItemSlotArray, 0, sizeof(m_pItemSlotArray));
	memset(m_pItemSlotArrayDummy, 0, sizeof(m_pItemSlotArrayDummy));
	memset(m_Button, 0, sizeof(CUIControl*) * 5);
	memset(m_ButtonSet, 0, sizeof(CUIControl*) * 5);
	memset(m_ButtonText, 0, sizeof(CBasicTextBox*) * 5);
	///////////////
}

CZbsjInventoryPage::~CZbsjInventoryPage()
{
}

VOID CZbsjInventoryPage::CreateSubControl()
{
	/////////////////
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);

	CString strSlotKeyword[nMAX_ONE_VIEW_SLOT] =
	{
		"ITEMMIXINVENSLOT_ITEM_SLOT0",
		"ITEMMIXINVENSLOT_ITEM_SLOT1",
		"ITEMMIXINVENSLOT_ITEM_SLOT2",
		"ITEMMIXINVENSLOT_ITEM_SLOT3",
		"ITEMMIXINVENSLOT_ITEM_SLOT4",
		"ITEMMIXINVENSLOT_ITEM_SLOT5",
		"ITEMMIXINVENSLOT_ITEM_SLOT6",
		"ITEMMIXINVENSLOT_ITEM_SLOT7",
		"ITEMMIXINVENSLOT_ITEM_SLOT8",
		"ITEMMIXINVENSLOT_ITEM_SLOT9",
		"ITEMMIXINVENSLOT_ITEM_SLOT10",
		"ITEMMIXINVENSLOT_ITEM_SLOT11",
		"ITEMMIXINVENSLOT_ITEM_SLOT12",
		"ITEMMIXINVENSLOT_ITEM_SLOT13",
		"ITEMMIXINVENSLOT_ITEM_SLOT14",
		"ITEMMIXINVENSLOT_ITEM_SLOT15",
		"ITEMMIXINVENSLOT_ITEM_SLOT16",
		"ITEMMIXINVENSLOT_ITEM_SLOT17",
		"ITEMMIXINVENSLOT_ITEM_SLOT18",
		"ITEMMIXINVENSLOT_ITEM_SLOT19",
		"ITEMMIXINVENSLOT_ITEM_SLOT20",
		"ITEMMIXINVENSLOT_ITEM_SLOT21",
		"ITEMMIXINVENSLOT_ITEM_SLOT22",
		"ITEMMIXINVENSLOT_ITEM_SLOT23",
		"ITEMMIXINVENSLOT_ITEM_SLOT24",
		"ITEMMIXINVENSLOT_ITEM_SLOT25",
		"ITEMMIXINVENSLOT_ITEM_SLOT26",
		"ITEMMIXINVENSLOT_ITEM_SLOT27",
		"ITEMMIXINVENSLOT_ITEM_SLOT28",
		"ITEMMIXINVENSLOT_ITEM_SLOT29",
		"ITEMMIXINVENSLOT_ITEM_SLOT30",
		"ITEMMIXINVENSLOT_ITEM_SLOT31",
		"ITEMMIXINVENSLOT_ITEM_SLOT32",
		"ITEMMIXINVENSLOT_ITEM_SLOT33",
		"ITEMMIXINVENSLOT_ITEM_SLOT34",
		"ITEMMIXINVENSLOT_ITEM_SLOT35",
		"ITEMMIXINVENSLOT_ITEM_SLOT36",
		"ITEMMIXINVENSLOT_ITEM_SLOT37",
		"ITEMMIXINVENSLOT_ITEM_SLOT38",
		"ITEMMIXINVENSLOT_ITEM_SLOT39",
		"ITEMMIXINVENSLOT_ITEM_SLOT40",
		"ITEMMIXINVENSLOT_ITEM_SLOT41",
		"ITEMMIXINVENSLOT_ITEM_SLOT42",
		"ITEMMIXINVENSLOT_ITEM_SLOT43",
		"ITEMMIXINVENSLOT_ITEM_SLOT44",
		"ITEMMIXINVENSLOT_ITEM_SLOT45",
		"ITEMMIXINVENSLOT_ITEM_SLOT46",
		"ITEMMIXINVENSLOT_ITEM_SLOT47",
		"ITEMMIXINVENSLOT_ITEM_SLOT48",
		"ITEMMIXINVENSLOT_ITEM_SLOT49"
	};

	///////////////////////////
	CString strButton[5] =
	{
		"INVENTORY_BUTTON0",
		"INVENTORY_BUTTON1",
		"INVENTORY_BUTTON2",
		"INVENTORY_BUTTON3",
		"INVENTORY_BUTTON4"
	};

	CString strButtonSet[5] =
	{
		"INVENTORY_BUTTON0_F",
		"INVENTORY_BUTTON1_F",
		"INVENTORY_BUTTON2_F",
		"INVENTORY_BUTTON3_F",
		"INVENTORY_BUTTON4_F"
	};

	CString strButtonText[5] =
	{
		"INVENTORY_BUTTON_TEXT0",
		"INVENTORY_BUTTON_TEXT1",
		"INVENTORY_BUTTON_TEXT2",
		"INVENTORY_BUTTON_TEXT3",
		"INVENTORY_BUTTON_TEXT4"
	};

	for (int i = 0; i < 5; i++)
	{
		m_Button[i] = new CUIControl;
		m_Button[i]->CreateSub(this, strButton[i].GetString(), UI_FLAG_DEFAULT, INVENTORY_BUTTON0 + i);
		m_Button[i]->SetVisibleSingle(TRUE);
		m_Button[i]->SetTransparentOption(TRUE);
		RegisterControl(m_Button[i]);

		m_ButtonSet[i] = new CUIControl;
		m_ButtonSet[i]->CreateSub(this, strButtonSet[i].GetString(), UI_FLAG_DEFAULT);
		m_ButtonSet[i]->SetVisibleSingle(FALSE);
		m_ButtonSet[i]->SetTransparentOption(TRUE);
		RegisterControl(m_ButtonSet[i]);

		m_ButtonText[i] = new CBasicTextBox;
		m_ButtonText[i]->CreateSub(this, strButtonText[i].GetString(), UI_FLAG_DEFAULT);
		m_ButtonText[i]->SetFont(pFont8);
		m_ButtonText[i]->SetTextAlign(TEXT_ALIGN_LEFT);
		m_ButtonText[i]->AddText(ID2GAMEWORD("NEW_INVENTORY_BUTTON_SLOT", i));
		RegisterControl(m_ButtonText[i]);
	}
	/////////////////

	for (int i = 0; i < nMAX_ONE_VIEW_SLOT; i++)
	{
		m_pItemSlotArrayDummy[i] = CreateControl(strSlotKeyword[i].GetString());
	}

	//	실제 데이타
	for (int i = 0; i < EM_INVENSIZE_Y_FORCE; i++)
	{
		m_pItemSlotArray[i] = CreateItemSlot(strSlotKeyword[0], ITEM_SLOT0 + i);
	}

	ResetAllItemSlotRender(10, 50);
	SetItemSlotRender(0, 10);
}

CItemSlotEx* CZbsjInventoryPage::CreateItemSlot(CString strKeyword, UIGUID ControlID)
{
	CItemSlotEx* pItemSlot = new CItemSlotEx;
	pItemSlot->CreateSub(this, strKeyword.GetString(), UI_FLAG_DEFAULT, ControlID);
	pItemSlot->CreateSubTradeInventory(EM_INVENSIZE_X);
	RegisterControl(pItemSlot);

	return pItemSlot;
}

CUIControl* CZbsjInventoryPage::CreateControl(const TCHAR* szControl)
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub(this, szControl);
	RegisterControl(pControl);
	return pControl;
}

VOID CZbsjInventoryPage::Update(INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl)
{
	SetItemIndex(nOUTOFRANGE, nOUTOFRANGE);

	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	UpdateBLOCK();
}

VOID CZbsjInventoryPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	if (ITEM_SLOT0 <= ControlID && ControlID < ITEM_SLOT_END)
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			const INT nPosY = ControlID - ITEM_SLOT0;
			CItemSlot* pSlot = m_pItemSlotArray[nPosY];
			const INT nPosX = pSlot->GetItemIndex();

			if (nPosY < 0 || nPosX < 0)
				return;

			SetItemIndex(nPosX, nPosY);
		}
	}

	///////////////////////////
	switch (ControlID)
	{
	case INVENTORY_BUTTON0:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			INVENTORY1();
		}
	}
	break;
	case INVENTORY_BUTTON1:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			INVENTORY2();
		}
	}
	break;
	case INVENTORY_BUTTON2:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			INVENTORY3();
		}
	}
	break;
	case INVENTORY_BUTTON3:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			INVENTORY4();
		}
	}
	break;

	case INVENTORY_BUTTON4:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			INVENTORY5();
		}
	}
	break;
	}
}

void CZbsjInventoryPage::INVENTORY1()
{
	for (int i = 0; i < 3; i++)
	{
		m_Button[i]->SetVisibleSingle(TRUE);
		m_ButtonSet[i]->SetVisibleSingle(FALSE);
	}
	m_ButtonSet[0]->SetVisibleSingle(TRUE);
	m_Button[0]->SetVisibleSingle(FALSE);
	ResetAllItemSlotRender(10, 30);
	SetItemSlotRender(0, 10);
}
void CZbsjInventoryPage::INVENTORY2()
{
	for (int i = 0; i < 3; i++)
	{
		m_Button[i]->SetVisibleSingle(TRUE);
		m_ButtonSet[i]->SetVisibleSingle(FALSE);
	}
	m_ButtonSet[1]->SetVisibleSingle(TRUE);
	m_Button[1]->SetVisibleSingle(FALSE);
	ResetAllItemSlotRender(0, 10);
	ResetAllItemSlotRender(20, 30);
	SetItemSlotRender(10, 20);
}
void CZbsjInventoryPage::INVENTORY3()
{
	for (int i = 0; i < 3; i++)
	{
		m_Button[i]->SetVisibleSingle(TRUE);
		m_ButtonSet[i]->SetVisibleSingle(FALSE);
	}
	m_ButtonSet[2]->SetVisibleSingle(TRUE);
	m_Button[2]->SetVisibleSingle(FALSE);
	ResetAllItemSlotRender(0, 20);
	SetItemSlotRender(20, 30);
}

void CZbsjInventoryPage::INVENTORY4()
{
	for (int i = 0; i < 5; i++)
	{
		m_Button[i]->SetVisibleSingle(TRUE);
		m_ButtonSet[i]->SetVisibleSingle(FALSE);
	}
	m_ButtonSet[3]->SetVisibleSingle(TRUE);
	m_Button[3]->SetVisibleSingle(FALSE);
	ResetAllItemSlotRender(0, 30);
	ResetAllItemSlotRender(40, 50);
	SetItemSlotRender(30, 40);
}
void CZbsjInventoryPage::INVENTORY5()
{
	for (int i = 0; i < 5; i++)
	{
		m_Button[i]->SetVisibleSingle(TRUE);
		m_ButtonSet[i]->SetVisibleSingle(FALSE);
	}
	m_ButtonSet[4]->SetVisibleSingle(TRUE);
	m_Button[4]->SetVisibleSingle(FALSE);
	ResetAllItemSlotRender(0, 40);
	SetItemSlotRender(40, 50);
}

VOID CZbsjInventoryPage::ResetAllItemSlotRender(int index, INT nTotal)
{
	if (nTotal < 0)
		return;

	for (INT i = index; i < nTotal; i++)
	{
		CItemSlot* pItemSlot = m_pItemSlotArray[i];
		if (pItemSlot)
			pItemSlot->SetVisibleSingle(FALSE);
	}
}

VOID CZbsjInventoryPage::SetItemSlotRender(INT nStartIndex, INT nTotal)
{
	if (nTotal < 0)
		return;

	const UIRECT& rcParentPos = GetGlobalPos();

	for (INT i = nStartIndex; i < nTotal; i++)
	{
		CItemSlotEx* pItemSlot = m_pItemSlotArray[i];
		if (pItemSlot)
		{
			INT nAbsoluteIndex = i - nStartIndex;

			CUIControl* pDummyControl = m_pItemSlotArrayDummy[nAbsoluteIndex];
			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos();

			pItemSlot->SetLocalPos(rcSlotLocalPos);
			pItemSlot->SetGlobalPos(rcSlotPos);

			pItemSlot->SetVisibleSingle(TRUE);
		}
	}
}

VOID CZbsjInventoryPage::SetItemIndex(INT nPosX, INT nPosY)
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

VOID CZbsjInventoryPage::GetItemIndex(INT* pnPosX, INT* pnPosY)
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

VOID CZbsjInventoryPage::LoadItemPage(GLInventory& ItemData)
{
	GLInventory::CELL_MAP* pItemList = ItemData.GetItemList();

	if (pItemList->size() > (EM_INVENSIZE_X * EM_INVENSIZE_Y))
	{
		GASSERT(0 && "인터페이스 사이즈보다, 데이타크기가 큽니다.");
		return;
	}

	//	삭제
	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for (WORD y = 0; y < EM_INVENSIZE_Y; y++)
		{
			for (WORD x = 0; x < EM_INVENSIZE_X; x++)
			{
				SINVENITEM& ref_InvenItem = GetItem(x, y);
				if (ref_InvenItem.sItemCustom.sNativeID != NATIVEID_NULL())
				{
					GLInventory::CELL_KEY foundkey(x, y);
					GLInventory::CELL_MAP_ITER found = pItemList->find(foundkey);
					if (found == iter_end)
						UnLoadItem(x, y);
				}
			}
		}
	}

	//	등록
	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for (; iter != iter_end; ++iter)
		{
			SINVENITEM* pInvenItem = (*iter).second;
			SINVENITEM& ref_InvenItemOld = GetItem(pInvenItem->wPosX, pInvenItem->wPosY);
			if (*pInvenItem != ref_InvenItemOld)
				LoadItem(*pInvenItem);
		}
	}
}

VOID CZbsjInventoryPage::LoadItem(SINVENITEM& ref_InvenItem)
{
	INT nPosX = ref_InvenItem.wPosX;
	INT nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[nPosY]->SetItemImage(nPosX, ref_InvenItem);
}

VOID CZbsjInventoryPage::UnLoadItemPage()
{
	for (int y = 0; y < EM_INVENSIZE_Y_FORCE; y++)
	{
		for (int x = 0; x < EM_INVENSIZE_X; x++)
		{
			UnLoadItem(x, y);
		}
	}
}

VOID CZbsjInventoryPage::UnLoadItem(INT nPosX, INT nPosY)
{
	m_pItemSlotArray[nPosY]->ResetItemImage(nPosX);
}

VOID CZbsjInventoryPage::ResetAllFlipItem()
{
	for (WORD y = 0; y < EM_INVENSIZE_Y; y++)
		for (WORD x = 0; x < EM_INVENSIZE_X; x++)
			m_pItemSlotArray[y]->SetFlipItem(x, FALSE);
}

VOID CZbsjInventoryPage::UpdateFlipItem(GLInventory& ref_Inventory)
{
	ResetAllFlipItem();

	GLInventory::CELL_MAP_ITER iter = ref_Inventory.GetItemList()->begin();
	GLInventory::CELL_MAP_ITER iter_end = ref_Inventory.GetItemList()->end();

	for (; iter != iter_end; ++iter)
	{
		SINVENITEM* pInvenItem = (*iter).second;
		if (pInvenItem)
		{
			INT nPosX = pInvenItem->wBackX;
			INT nPosY = pInvenItem->wBackY;
			m_pItemSlotArray[nPosY]->SetFlipItem(nPosX, TRUE);
		}
	}

}

VOID CZbsjInventoryPage::UpdateFlipItem(SINVEN_POS& ref_GarbageItem)
{
	if (ref_GarbageItem.wPosY >= 0 && ref_GarbageItem.wPosY < EM_INVENSIZE_Y)
	{
		if (ref_GarbageItem.wPosX >= 0 && ref_GarbageItem.wPosX < EM_INVENSIZE_X)
			m_pItemSlotArray[ref_GarbageItem.wPosY]->SetFlipItem(ref_GarbageItem.wPosX, TRUE);
	}
}

VOID CZbsjInventoryPage::UpdateBLOCK()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	INT nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if (EM_INVENSIZE_Y_FORCE < nUSABLE_INVENLINE)
	{
		GASSERT(0 && "데이타 오류입니다.");

		nUSABLE_INVENLINE = EM_INVENSIZE_Y_FORCE;
	}

	// 사용 가능
	for (INT i = 0; i < nUSABLE_INVENLINE; i++)
		m_pItemSlotArray[i]->ResetBLOCK();

	// 사용 불가능
	for (INT i = nUSABLE_INVENLINE; i < EM_INVENSIZE_Y; i++)
		m_pItemSlotArray[i]->SetBLOCK();
}

SINVENITEM& CZbsjInventoryPage::GetItem(INT nPosX, INT nPosY)
{
	return m_pItemSlotArray[nPosY]->GetItemImage(nPosX);
}

VOID CZbsjInventoryPage::SetOneViewSlot(const INT& nMAX_ONE_VIEW)
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
	m_nCurPos = -1;
	//m_pScrollBar->GetThumbFrame()->SetState( EM_INVENSIZE_Y, nMAX_ONE_VIEW );
}