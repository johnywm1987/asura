#pragma	once

#include "./UIWindowEx.h"
#include "./PKRankData.h"
#include "./GLCharDefine.h"
#include "./GLCharData.h"

class   GLGaeaClient;
class   CInnerInterface;
class	CBasicTextBoxEx;
class	CBasicScrollBarEx;
class	CBasicTextButton;
class	CBasicButton;
class	CKillFeedSelectorWindowSlot;
class   CPKRankNotificationInfo;

class CKillFeedSelectorWindow : public CUIWindowEx
{
public:
	enum
	{
		KILL_FEED_SELECTOR_SLOT00 = ET_CONTROL_NEXT,
		KILL_FEED_SELECTOR_SLOT01,
		KILL_FEED_SELECTOR_SLOT02,
		KILL_FEED_SELECTOR_SLOT03,
		KILL_FEED_SELECTOR_SLOT04,
		KILL_FEED_SELECTOR_SLOT05,
		KILL_FEED_SELECTOR_OK,
		KILL_FEED_SELECTOR_CANCEL,
	};

public:
	CKillFeedSelectorWindow(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface);
	virtual	~CKillFeedSelectorWindow();

public:
	void	CreateSubControl();

protected:
	GLGaeaClient* m_pGaeaClient;
	CInnerInterface* m_pInterface;

	CPKRankNotificationInfo* m_pKillFeedSample[6];
	CUIControl* m_pKillFeedSelected[6];

	int							nSelectedKillFeed;

public:
	virtual	void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual	void SetVisibleSingle(BOOL bVisible);
};