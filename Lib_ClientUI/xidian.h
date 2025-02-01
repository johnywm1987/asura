#pragma once
#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class CBasicTextBox;
class CBasicLineBox;
class CD3DFontPar;
class CBasicTextButton;
class CPVPTyrannyPageBattle;
class CPVPTyrannyPageHistory;



class CXidian : public CUIGroup
{
private:
	enum
	{
		TYRANNY_PAGE_INFO_BATTLE = NO_ID + 1,
		TYRANNY_PAGE_INFO_HISTORY,
		REBORN_WINDOW_BUTTON_CLOSE,
		REBORN_WINDOW_BUTTON_REBORN,
	};


public:
	CXidian();
	virtual	~CXidian();

public:
	void	CreateSubControl();
	CBasicTextBox* CreateUITextBox(const char* szControl, CD3DFontPar* pFont, int nTextAlign);
	CBasicLineBox* CreateUILineBoxQuestList(char* szBaseControl);
	CBasicLineBox* CreateUILineBoxWhiteNoBody(char* szBaseControl);
	CUIControl* CreateUIControl(const char* szControl);
	CBasicTextButton* m_pButtonClose;
	CBasicTextButton* m_pButtonReborn;

public:
	CBasicTextBox* m_pTextMapName;

	CBasicTextBox* m_pTextInfoTitle;
	CBasicTextBox* m_pTextInfoGoal0;
	CBasicTextBox* m_pTextInfoGoal1;
	CBasicTextBox* m_pTextInfoGoal2;
	CBasicTextBox* m_pTextInfoMatch;
	CBasicTextBox* m_pTextInfoMatch1;
	CBasicTextBox* m_pTextInfoMatch2;
	CBasicTextBox* m_pTextInfoMatch3;
	CBasicTextBox* m_pTextInfoMatch4;
	CBasicTextBox* m_pTextInfoMatch5;
	CBasicTextBox* m_pTextInfoMatch6;
	CBasicTextBox* m_pTextInfoMatch7;
	CBasicTextBox* m_pTextInfoMatch8;
	CBasicTextBox* m_pTextInfoPlayer;
	CBasicTextBox* m_pTextInfoLevel;


	CBasicTextBox* m_pTextReward;

	CPVPTyrannyPageBattle* m_pInfoBattle;
	CPVPTyrannyPageHistory* m_pInfoHistory;

public:
	float	m_fUpdateTime;

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

	void	UpdateInfo();
};