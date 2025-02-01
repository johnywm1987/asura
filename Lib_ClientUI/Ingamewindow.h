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


class CBasicTextButton;
class CMultiModeButton;
class CReborn;			/*pvp tyranny, Juver, 2017/08/25 */
class CXidian;
class CXihong;
class CPaodian;
class CPVPSchoolWarsPage;		/*school wars, Juver, 2018/01/19 */
class CPVPCaptureTheFlagPage;	/*pvp capture the flag, Juver, 2018/01/25 */

class	CIngamewindow : public CUIWindowEx
{
	enum
	{
		COMPETITION_WINDOW_BUTTON_CLOSE = ET_CONTROL_NEXT,
		COMPETITION_WINDOW_BUTTON_TYRANNY,
		COMPETITION_WINDOW_BUTTON_SCHOOLWARS,
		COMPETITION_WINDOW_BUTTON_CAPTURE_THE_FLAG,
		COMPETITION_WINDOW_BUTTON_PAODIAN,

		COMPETITION_WINDOW_PAGE_TYRANNY,
		COMPETITION_WINDOW_PAGE_SCHOOLWARS,
		COMPETITION_WINDOW_PAGE_CAPTURE_THE_FLAG,
		COMPETITION_WINDOW_PAGE_PAODIAN,
	};

	enum
	{
		COMPETITION_WINDOW_TYPE_TYRANNY = 0,
		COMPETITION_WINDOW_TYPE_SCHOOLWARS = 1,
		COMPETITION_WINDOW_TYPE_CAPTURE_THE_FLAG = 2,
		COMPETITION_WINDOW_TYPE_PAODIAN = 3,
	};

public:
	CIngamewindow();
	virtual ~CIngamewindow();

public:
	void	CreateSubControl();
	CMultiModeButton* CreateTabButton(const char* szButtonBox, const char* szButtonText, UIGUID ControlID);

public:
	CBasicTextButton* m_pButtonClose;
	CMultiModeButton* m_pButtonEventTyranny;
	CMultiModeButton* m_pButtonEventSchoolWars;
	CMultiModeButton* m_pButtonEventCaptureTheFlag;
	CMultiModeButton* m_pPaodianEvent;


	CReborn* m_pReborn;
	CXidian* m_pxidian;		/*school wars, Juver, 2018/01/19 */
	CXihong* m_pxihong;
	CPaodian* m_pPaodian;
	CPVPCaptureTheFlagPage* m_pPageCaptureTheFlag;	/*pvp capture the flag, Juver, 2018/01/25 */
public:
	int		m_nPage;

public:
	virtual	void SetVisibleSingle(BOOL bVisible);
	virtual	void TranslateUIMessage(UIGUID cID, DWORD dwMsg);

public:
	void	OpenPage(int nPage);
};