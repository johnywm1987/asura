#pragma	once

#include "UIWindowEx.h"
#include "GLCharDefine.h"

class	CD3DFontPar;
class	CBasicTextBox;
class	CBasicButton;
class	CBasicButtonRadio;
class	CBasicComboBoxRollOver;
struct	GLCHARLOGIC;


class	CCharacterWindowCharStat;
class	CCharacterWindowCharWear;
class	CCharacterWindowCharWearEx;
class	CCharacterWindowCharRender;


class CCharacterWindowChar : public CUIGroup
{
public:
	enum
	{
		CHARACTER_WEAR = NO_ID + 1,
		CHARACTER_WEAR_EX,
		CHARACTER_STATS,
		CHARACTER_ACADEMY_BRIGHTBAR,
		CHARACTER_ACADEMY_HELP,
		/*activity system, Juver, 2017/11/05 */
		CHARACTER_TITLE_BUTTON,
		CHARACTER_TITLE_COMBO_ROLLOVER,
		CHARACTER_VIEW_INFO_BUTTON_LEFT,
		CHARACTER_VIEW_INFO_BUTTON_RIGHT,


		MENU_OPEN_BUTTON,
		MENU_CLOSE_BUTTON,
	};

	enum
	{
		INFO_ICON_SCHOOL = 3
	};

public:
	CCharacterWindowChar();
	virtual	~CCharacterWindowChar();

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual	void SetVisibleSingle(BOOL bVisible);

public:
	void	CreateSubControl();

private:
	CBasicTextBox* CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	CUIControl* CreateControl(const char* szControl, const UIGUID& cID = NO_ID);
	CBasicButton* CreateFlipButton(char* szButton, char* szButtonFlip, UIGUID ControlID, WORD wFlipType);


private:
	void	SetData(const GLCHARLOGIC& sCharData);
	void	SetAdditionalControl(BOOL bWindowOpen);

public:
	CCharacterWindowCharStat* m_pStatRegion;
	CCharacterWindowCharWear* m_pCharWear;
	CCharacterWindowCharWearEx* m_pCharWearEx;
	CCharacterWindowCharRender* m_pRender;

	/*PKScore, PKDeath*/
	//CBasicTextBox*		m_pPKScore;
	//CBasicTextBox*		m_pPKDeath;
	//
	CBasicTextBox* m_pPremiumText;
	CBasicTextBox* m_pBadgeText;
	CBasicTextBox* m_pNameText;
	CBasicTextBox* m_pClubText;
	CBasicTextBox* m_pTitleText;
	CBasicTextBox* m_pAcademy;
	CBasicTextBox* m_pLifeP;
	CBasicTextBox* m_pContrib;
	CBasicTextBox* m_pActivity;
	CUIControl* m_pBrightFrame;
	CUIControl* m_pBrightSlider;
	CBasicButton* m_pHelp;
	CBasicButtonRadio* m_pAbility;
public:
	CUIControl* m_pSchoolIcon[INFO_ICON_SCHOOL];

	CBasicButton* m_pButtonLeft;
	CBasicButton* m_pButtonRight;

public:
	void	LoadBadge(const GLCHARLOGIC& sCharData);
	void	SetArmSwapTabButton(BOOL bArmSub);
	void	ClearRender();

private:
	/*activity system, Juver, 2017/11/05 */
	CBasicButton* m_pButtonTitle;
	CBasicComboBoxRollOver* m_pComboBoxTitleRollOver;


	/*Addtinal Info, Juver, 2017/11/05 */
	CBasicButton* m_pShowButton;
	CBasicButton* m_pHideButton;

private:
	UIGUID	m_RollOverID;
	BOOL	m_bFirstLBUP;

};