#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

#include "GLCharDefine.h"
#include "PKRankData.h"

class CBasicTextBoxEx;
class CBasicLineBox;


class CPKRankNotificationInfo : public CUIGroup
{
	enum
	{
		NAME_KILLER0 = NO_ID + 1,
		NAME_KILLER1,
		NAME_KILLER2,
		NAME_KILLER3,
		NAME_KILLER4,
		NAME_KILLER5,
		NAME_KILLER6,
		NAME_KILLER7,
		NAME_KILLER8,
		NAME_KILLER9,
		NAME_KILLED0,
		NAME_KILLED1,
		NAME_KILLED2,
		NAME_KILLED3,
		NAME_KILLED4,
		NAME_KILLED5,
		NAME_KILLED6,
		NAME_KILLED7,
		NAME_KILLED8,
		NAME_KILLED9,
	};
	enum
	{
		RANK_INFO_ICON_SCHOOL = 3,
		RANK_INFO_ICON_CLASS = GLCI_NUM_5CLASS_EX,
	};

private:
	std::string strNameKiller;
	std::string strNameKilled;
	DWORD	m_dwCharIDKiller;
	DWORD	m_dwCharIDKilled;
	SPK_HISTORY				m_sHistory;


public:
	CPKRankNotificationInfo();
	virtual ~CPKRankNotificationInfo();

public:
	void CreateSubControl();

	std::string GetKiller() { return strNameKiller; }
	std::string GetKilled() { return strNameKilled; }
	DWORD GetCharIDKilled() { return m_dwCharIDKilled; }
	DWORD GetCharIDKiller() { return m_dwCharIDKiller; }

public:
	virtual	void TranslateUIMessage(UIGUID cID, DWORD dwMsg);
	virtual	void SetVisibleSingle(BOOL bVisible);
	virtual	void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

public:
	CUIControl* m_pClassIconKiller[RANK_INFO_ICON_CLASS];
	CUIControl* m_pSchoolIconKiller[RANK_INFO_ICON_SCHOOL];
	CUIControl* m_pKillFeedStyleKiller[6];
	CUIControl* m_pClassIconKilled[RANK_INFO_ICON_CLASS];
	CUIControl* m_pSchoolIconKilled[RANK_INFO_ICON_SCHOOL];

	CBasicTextBoxEx* m_pNameKiller;
	CBasicTextBoxEx* m_pNameKilled;


	CBasicTextBoxEx* m_pMapName;

	CBasicLineBox* m_pLineBox;
	CUIControl* m_pKillIcon;


public:
	void SetData(SPK_HISTORY sHistory);
	int GetIndex() { return m_sHistory.cKF_KILLER; }
};