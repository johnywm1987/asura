#pragma once
#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicLineBox;
class CD3DFontPar;

class BubuCompetitionNewBattle;
class BubuCompetitionNewHistory;

class CTipBox;

const DWORD UIMSG_TYRANNY_PAGE_BUTTON_CLOSE = UIMSG_USER1;

class BubuCompetitionNew : public CUIGroup
{
private:
	enum
	{
		TYRANNY_PAGE_INFO_BATTLE = NO_ID + 1,
		TYRANNY_PAGE_INFO_HISTORY,
	};


public:
	BubuCompetitionNew();
	virtual	~BubuCompetitionNew();

public:
	void	CreateSubControl();
	CBasicTextBox* CreateUITextBox(const char* szControl, CD3DFontPar* pFont, int nTextAlign);
	CBasicLineBox* CreateUILineBoxWhiteNoBody(char* szBaseControl);
	CUIControl* CreateUIControl(const char* szControl);

public:
	CBasicTextBox* m_pTextMapName;

	CTipBox* m_pSelect;

public:
	float	m_fUpdateTime;

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
};