﻿//	僭擒 溯檜
//
//	譆蟾 濛撩濠 : 撩晦膨
//	檜 熱薑濠 : 
//	煎斜
//		[2003.11.24]
//			@ 濛撩
//

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "GLCharData.h"

////////////////////////////////////////////////////////////////////
//	餌辨濠 詭衛雖 薑曖
const DWORD UIMSG_MOUSEIN_POTIONSLOT = UIMSG_USER1;
const DWORD UIMSG_TRAY_CLOSE_LBUP = UIMSG_USER2;
////////////////////////////////////////////////////////////////////


class	CBasicTextBox;
class	CD3DFontPar;
class	CBasicQuickPotionSlotEx;

class CBasicPotionTray : public CUIGroup
{
private:
static	const	int		nOUTOFRANGE;

private:
	enum
	{
		QUICK_POTION_SLOT0 = NO_ID + 1,
		QUICK_POTION_SLOT1,
		QUICK_POTION_SLOT2,

		QUICK_POTION_SLOT3,
		QUICK_POTION_SLOT4,
		QUICK_POTION_SLOT5,

		QUICK_POTION_TRAY_CLOSE_BUTTON,
	};

public:
	CBasicPotionTray ();
	virtual	~CBasicPotionTray ();

public:
	void	CreateSubControl ();

public:
	virtual void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	int		GetSlotIndex ()						{ return m_nIndex; }
	void	SetSlotIndex ( int nIndex )			{ m_nIndex = nIndex; }
	void 	SetShotcutText ( DWORD nID, CString& strTemp );

private:
	CD3DFontPar*	m_pFont8;

private:
	int		m_nIndex;

private:
	CBasicQuickPotionSlotEx*	m_pSlot[EMACTIONQUICK_SIZE];
};