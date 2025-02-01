#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;

class CPaodian : public CUIOuterWindow
{
	enum
	{
		SELCHAR_BUTTON_OK = ET_CONTROL_NEXT,
		SELCHAR_BUTTON_CANCEL,
		SELCHAR_EDIT_MAN,
		SELCHAR_EDIT_CAPTCHA,
	};

public:
	CPaodian();
	virtual	~CPaodian();

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

public:
	void CreateSubControl();
	CBasicButton* CreateFlipButton(char* szButton, char* szButtonFlip, UIGUID ControlID);

	void ResetAll(bool bEdit = false);

private:
	CUIEditBoxMan* m_pEditBoxMan;
	CBasicTextBox* pTextBoxCaptcha;
	CBasicTextBox* pTextBoxGameTime;

	int nLIMIT_CAPTCHA;
	std::string	strCaptcha;
};