/*!
 * \file CompetitionWindow.h
 *
 * \author Juver
 * \date June 2017
 *
 *
 */

#pragma	once

#include "UIWindowEx.h"

class	CBasicTextButton;
class	CBasicLineBoxEx;
class	CBasicTextBox;
class	CBasicTextButton;
class	CBasicTreeEx;
class	CBasicTreeNode;
class	CBasicScrollBarEx;
class 	CPVPTyrannyPage;
class 	CPVPSchoolWarsPage;
class 	CPVPCaptureTheFlagPage;
class 	CPVPFreePKPage;

class	CCompetitionWindow : public CUIWindowEx
{
private:
	static	const int	nSTARTLINE;
	static	const int	nLIMIT_LINE;
protected:
	enum
	{
		PVP_WINDOW_TREE = ET_CONTROL_NEXT,
		RR_BUTTON_PAGE,
		SW_BUTTON_PAGE,
		FREEPK_BUTTON_PAGE,
		PVP_PAGE,
		PVE_PAGE,
		TW_PAGE,
		CTF_PAGE,
		SW_PAGE,
		FK_PAGE,
		RK_PAGE,
		PK_PAGE,
		RICH_PAGE,
		LVL_PAGE,
		OL_PAGE,
		SCHOOLWAR_BUTTON_PAGE,
	};
	enum
	{
		nTEXT = 2,
		nPAGES = 3,
	};

public:
	CCompetitionWindow();
	virtual	~CCompetitionWindow();
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

public:
	CBasicTextButton* CreateTextButton23(const char* szButton, UIGUID ControlID, const char* szText);
	CBasicTextButton* CreateTextButton18(const char* szButton, UIGUID ControlID, const char* szText);
	CBasicTextButton* CreateTextButton(char* szButton, UIGUID ControlID, char* szText);
	void LoadTreePVP();
	void LoadTreePVE();
	void LoadTreeRK();
	void LoadNode(CBasicTreeNode* pParent, DWORD dwData, char* szText);
	void CreateSubControl();

private:
	CBasicLineBoxEx* m_pBasicLineBox;
	CBasicTreeEx* m_pTree;
	CPVPTyrannyPage* m_pTyrannyPage;
	CPVPSchoolWarsPage* m_pSchoolwarsPage;
	CPVPCaptureTheFlagPage* m_pCaptureTheFlag;
	CPVPFreePKPage* m_pPageFreePK;

	CBasicLineBoxEx* m_pLineButton;
	CBasicScrollBarEx* m_pTreeScroll;

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	void CheckMouseState();

	CPVPFreePKPage* GetPVPFreePKPage() { return m_pPageFreePK; }

	bool m_bCHECK_MOUSE_STATE;
	BOOL m_bFirstGap;
	D3DXVECTOR2	m_vGap;
	int			m_PosX;
	int			m_PosY;

public:

	void	OpenPage(int nPage);
};