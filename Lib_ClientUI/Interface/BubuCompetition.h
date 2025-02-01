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
class BubuCompetitionNew;

class	CCompetitionWindowNew : public CUIWindowEx
{
	enum
	{
		COMPETITION_WINDOW_BUTTON_CLOSE = ET_CONTROL_NEXT,
		COMPETITION_WINDOW_BUTTON_TYRANNY,

		COMPETITION_WINDOW_PAGE_TYRANNY,
	};

	enum
	{
		COMPETITION_WINDOW_TYPE_TYRANNY = 0,
	};

public:
	CCompetitionWindowNew();
	virtual ~CCompetitionWindowNew();

public:
	void	CreateSubControl();
	CMultiModeButton* CreateTabButton(const char* szButtonBox, const char* szButtonText, UIGUID ControlID);

public:
	CBasicTextButton* m_pButtonClose;
	CMultiModeButton* m_pButtonEventTyranny;


	BubuCompetitionNew* m_pPageTyranny;

public:
	int		m_nPage;

public:
	virtual	void SetVisibleSingle(BOOL bVisible);
	virtual	void TranslateUIMessage(UIGUID cID, DWORD dwMsg);

public:
	void	OpenPage(int nPage);
};