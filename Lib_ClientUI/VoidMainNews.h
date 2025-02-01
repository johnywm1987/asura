#pragma once
#pragma	once

#include "UIWindowEx.h"
#include "BasicLineBoxEx.h"

class	CBasicTextBox;
class	CBasicTextButton;
class	CUIEditBox;

class VoidMainNews : public CUIWindowEx
{
protected:
	enum
	{
		GUIDE_BUTTON_IMAGE = ET_CONTROL_NEXT,
		NEWS_BUTTON_IMAGE,
		SERVERINFO_BUTTON_IMAGE,
		RV_BUTTON_IMAGE,
		DONATION_BUTTON_IMAGE,
		GC_BUTTON_IMAGE
	};

private:
	CString imageUpdate;
	CUIControl* m_pNewsImage0;
	CUIControl* m_pNewsImage1;
	CUIControl* m_pNewsImage2;
	CUIControl* m_pNewsImage3;
	CUIControl* m_pNewsImage4;
	CUIControl* m_pNewsImage5;
	CUIControl* m_pNewsImage6;
	CUIControl* m_pNewsImageSwitch;
	CBasicTextButton* vm_GuideButton;
	CBasicTextButton* vm_NewsButton;
	CBasicTextButton* vm_ServerInfoButton;
	CBasicTextButton* vm_RVButton;
	CBasicTextButton* vm_DonationButton;
	CBasicTextButton* vm_GiftCodeButton;


	CBasicTextBox* NewsLabel;
	float					fDelayTime;
	bool					waitfordelay;
	CUIEditBox* pEditBox;

public:
	VoidMainNews();
	~VoidMainNews();

	void	CreateSubControl();
	CUIControl* CreateNewsWindow(const char* guiKeyword);
	CBasicTextButton* CreateTextButton23(const char* szButton, UIGUID ControlID, const char* szText);
	CBasicLineBoxEx* CreateLineBoxWhite(char* szLineBoxKeyWord);
	CBasicTextButton* CreateTextButton(const char* szButton, UIGUID ControlID, const char* szText);
	CBasicTextBox* CreateNewsLabel(const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	void ResetControls();

};