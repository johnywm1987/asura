#pragma once
#pragma	once

#include "UIWindowEx.h"

class CItemImage;
class CBasicTextButton;
class CBasicComboBoxRollOver;
class CBasicComboBox;
class CBasicTextBoxEx;
class CBasicLineBox;

//--------------------------------------------------------------------
// ItemRebuild : √÷¡ÿ«ı
//--------------------------------------------------------------------
class CItemRebuildZbsj : public CUIWindowEx	// ITEMREBUILD_MARK
{
protected:
	enum
	{
		ITEM_ZBSJ_ITEM_SLOT = ET_CONTROL_NEXT,
		ITEM_ZBSJ_COAST_TEXT_STATIC,
		ITEM_ZBSJ_COAST_TEXT,
		ITEM_ZBSJ_MONEY_BACK,
		ITEM_ZBSJ_MONEY_IMAGE,
		ITEM_ZBSJ_MONEY_TEXT,
		ITEM_ZBSJ_OK_BUTTON,
		ITEM_ZBSJ_CANCEL_BUTTON,
		ITEM_ZBSJ_REDOK_BUTTON1,
		ITEM_ZBSJ_REDOK_BUTTON2,
		ITEM_ZBSJ_REDOK_BUTTON3,
		ITEM_ZBSJ_REDOK_BUTTON4,
		ITEM_ZBSJ_REDOK_BUTTON5,
		ITEM_ZBSJ_REDOK_BUTTON6,
		ITEM_ZBSJ_OK_BUTTONOPEN,
		ZBSJ_BUTTON1,
		ITEM_ZBSJ_STOP_LIST
	};

protected:
	CItemImage* m_pItemImage;
	CBasicTextBox* m_pCostTextBoxStatic;
	CBasicTextBox* m_pCostTextBox;
	CBasicTextBox* m_pMoneyTextBox;
	CBasicTextButton* m_pOkButton;

	CBasicTextButton* m_pZbsjRebOkButton1;
	CBasicTextButton* m_pZbsjRebOkButton2;
	CBasicTextButton* m_pZbsjRebOkButton3;
	CBasicTextButton* m_pZbsjRebOkButton4;
	CBasicTextButton* m_pZbsjRebOkButton5;
	CBasicTextButton* m_pZbsjRebOkButton6;
	CBasicTextBox* m_pZbsjRebTextBox1;
	CBasicTextBox* m_pZbsjRebTextBox2;
	CBasicTextBox* m_pZbsjRebTextBox3;
	CBasicTextBox* m_pZbsjRebTextBox4;
	CBasicTextBox* m_pZbsjRebTextBox5;
	CBasicTextBox* m_pZbsjRebTextBox6;
	CBasicTextBox* m_pZbsjRebTextBoxView1;
	CBasicTextBox* m_pZbsjRebTextBoxView2;
	CBasicTextBox* m_pZbsjRebTextBoxView3;
	CBasicTextBox* m_pZbsjRebTextBoxView4;
	CBasicTextBox* m_pZbsjRebTextBoxView5;
	CBasicTextBox* m_pZbsjRebTextBoxView6;

	int m_nType;
	DWORD m_dwValue;
	bool	m_bOK;
	BOOL	m_bReqModal;
	BOOL	m_bFIX;
	int		m_nZbsj;

	CUIControl* m_bg_white;
	CBasicLineBox* m_pBackLine1;

public:
	CItemRebuildZbsj();
	virtual	~CItemRebuildZbsj();

public:
	VOID CreateSubControl();
	void LoadZbsj();
	void ZBSJ_PROCESS(int nSelect);
	BOOL IsValidItem();

public:
	virtual	VOID Update(INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl);
	virtual	VOID TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	CBasicTextBox* CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);

	CBasicComboBox* m_pcbSkill_A;
	CBasicComboBoxRollOver* m_pcbSkillRollOver_A;

	UIGUID	m_RollOverID;
	BOOL	m_bFirstLBUP;

	BOOL					m_bSearch;

	int		GetSelectZbsj() { return m_nZbsj; }
	void	SetSelectZbsj(int nZbsj) { m_nZbsj = nZbsj; }


};