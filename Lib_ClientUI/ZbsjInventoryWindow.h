#pragma	once

#include "UIWindowEx.h"

class CZbsjInventoryPage;

//--------------------------------------------------------------------
// CItemMixInvenWindow : 
//--------------------------------------------------------------------
class CZbsjInventoryWindow : public CUIWindowEx	// ������
{
protected:
	enum
	{
		ITEMMIXINVEN_MONEY_BUTTON = ET_CONTROL_NEXT,
		ITEMMIXINVEN_PAGE
	};

private:
	CZbsjInventoryPage* m_pPage;
	CBasicTextBox* m_pMoneyTextBox;
	INT						m_nONE_VIEW_SLOT;

public:
	CZbsjInventoryWindow();
	virtual	~CZbsjInventoryWindow();

public:
	VOID CreateSubControl();

public:
	virtual	VOID Update(INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl);
	virtual	VOID TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);

public:
	VOID SetOneViewSlot(const INT& nONE_VIEW_SLOT);
}; 
