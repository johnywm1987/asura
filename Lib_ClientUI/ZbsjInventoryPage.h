#pragma	once

#include "UIGroup.h"
#include "GLInventory.h"

class CItemSlotEx;
class CBasicScrollBarEx;
class	CBasicButton;
class	CBasicTextBox;
class	CD3DFontPar;
struct	SINVENITEM;

class CZbsjInventoryPage : public CUIGroup	// »ﬁ¡ˆ≈Î
{
private:
	static const INT	nSTARTLINE;
	static const INT	nOUTOFRANGE;

protected:
	enum
	{
		nONE_VIEW_SLOT_DEFAULT = 4,
		nMAX_ONE_VIEW_SLOT = 50
	};

	enum
	{
		EM_INVENSIZE_Y_FORCE = EM_INVENSIZE_Y,
		ITEM_SLOT0 = NO_ID + 1,
		ITEM_SLOT1,
		///////////
		ITEM_SLOT2, ITEM_SLOT3, ITEM_SLOT4, ITEM_SLOT5, ITEM_SLOT6, ITEM_SLOT7, ITEM_SLOT8, ITEM_SLOT9,
		ITEM_SLOT10, ITEM_SLOT11, ITEM_SLOT12, ITEM_SLOT13, ITEM_SLOT14, ITEM_SLOT15, ITEM_SLOT16, ITEM_SLOT17, ITEM_SLOT18, ITEM_SLOT19,
		ITEM_SLOT20, ITEM_SLOT21, ITEM_SLOT22, ITEM_SLOT23, ITEM_SLOT24, ITEM_SLOT25, ITEM_SLOT26, ITEM_SLOT27, ITEM_SLOT28, ITEM_SLOT29,
		ITEM_SLOT30, ITEM_SLOT31, ITEM_SLOT32, ITEM_SLOT33, ITEM_SLOT34, ITEM_SLOT35, ITEM_SLOT36, ITEM_SLOT37, ITEM_SLOT38, ITEM_SLOT39,
		ITEM_SLOT40, ITEM_SLOT41, ITEM_SLOT42, ITEM_SLOT43, ITEM_SLOT44, ITEM_SLOT45, ITEM_SLOT46, ITEM_SLOT47, ITEM_SLOT48, ITEM_SLOT49,
		///////////
		ITEM_SLOT_END = ITEM_SLOT0 + EM_INVENSIZE_Y_FORCE,
		/////////
		INVENTORY_BUTTON0,
		INVENTORY_BUTTON1,
		INVENTORY_BUTTON2,
		INVENTORY_BUTTON3,
		INVENTORY_BUTTON4
		//////////
	};

private:
	CItemSlotEx* m_pItemSlotArray[EM_INVENSIZE_Y_FORCE];
	CUIControl* m_pItemSlotArrayDummy[nMAX_ONE_VIEW_SLOT];
	CBasicScrollBarEx* m_pScrollBar;
	int					m_nPosX;
	int					m_nPosY;
	int					m_nCurPos;
	int					m_nONE_VIEW_SLOT;
	//////
	CBasicButton* m_pPageButton[5];

public:
	CZbsjInventoryPage();
	virtual	~CZbsjInventoryPage();

public:
	VOID CreateSubControl();

protected:
	CItemSlotEx* CreateItemSlot(CString strKeyword, UIGUID ControlID);
	CUIControl* CreateControl(const TCHAR* szControl);

public:
	virtual VOID Update(INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl);
	virtual	VOID TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

protected:
	VOID ResetAllItemSlotRender(int index, INT nTotal);
	VOID SetItemSlotRender(INT nStartIndex, INT nTotal);

	///////////////
	CUIControl* m_Button[5];
	CUIControl* m_ButtonSet[5];
	CBasicTextBox* m_ButtonText[5];

public:
	VOID SetItemIndex(INT nPosX, INT nPosY);
	VOID GetItemIndex(INT* pnPosX, INT* pnPosY);

public:
	VOID LoadItemPage(GLInventory& ItemData);
	VOID LoadItem(SINVENITEM& ref_InvenItem);

public:
	VOID UnLoadItemPage();
	VOID UnLoadItem(INT nPosX, INT nPosY);

public:
	VOID ResetAllFlipItem();
	VOID UpdateFlipItem(GLInventory& ref_Inventory);
	VOID UpdateFlipItem(SINVEN_POS& ref_RebuildItem);

public:
	VOID UpdateBLOCK();
	SINVENITEM& GetItem(INT nPosX, INT nPosY);

	////////////
	void	INVENTORY1();
	void	INVENTORY2();
	void	INVENTORY3();
	void	INVENTORY4();
	void	INVENTORY5();

public:
	VOID SetOneViewSlot(const INT& nMAX_ONE_VIEW);
};