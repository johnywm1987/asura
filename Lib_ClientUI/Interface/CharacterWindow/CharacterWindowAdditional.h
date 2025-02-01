///////////////////////////////////////////////////////////
////////////CHARACTER EXTRA STATS ARVIN.BC/////////////////
///////////////////////////////////////////////////////////
#pragma	once

#include "UIWindowEx.h"
#include "../Lib_Client/G-Logic/GLCharDefine.h"
#include "../Lib_Client/G-Logic/GLogicEx.h"

class	CD3DFontPar;
class	CBasicProgressBar;
class	CBasicTextBox;
class	CBasicButtonText;
class	CBasicButton;
struct	GLCHARLOGIC;

class 	CCharacterWindowAdditional : public CUIWindowEx
{
protected:
	enum
	{
		nSCHOOL_MARK		= 3,
	};
	enum
	{
		ICON_CLASS_IMAGE0 = ET_CONTROL_NEXT,
		ICON_CLASS_IMAGE1,
		ICON_CLASS_IMAGE2,
		ICON_CLASS_IMAGE3,
		ICON_CLASS_IMAGE4,
		ICON_CLASS_IMAGE5,
		ICON_CLASS_IMAGE6,
		ICON_CLASS_IMAGE7,
		ICON_CLASS_IMAGE8,
		ICON_CLASS_IMAGE9,
		INFO_DISPLAY_SG,
		INFO_DISPLAY_MP,
		INFO_DISPLAY_PHX,
	};

public:
    CCharacterWindowAdditional ();
	virtual	~CCharacterWindowAdditional ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual void SetGlobalPos(const D3DXVECTOR2& vPos);

public:
	void	CreateSubControl ();

private:	
	CUIControl*			CreateControl ( const char* szControl, const UIGUID& cID = NO_ID );
	CBasicButtonText*	CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText );
	CBasicButtonText*	CreateButtonRed ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText );
	CBasicButton*		CreateMouseIn( const char* szButton, const char* szMouseIn ,UIGUID ControlID );
	CBasicTextBox*		CreateStaticControl ( char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID );
	
private:
	void	SetName ( const GLCHARLOGIC& sCharData );
	void	SetSchool ( const GLCHARLOGIC& sCharData );
	void	SetClass ( const GLCHARLOGIC& sCharData );
	void	SetGuild ( const GLCHARLOGIC& sCharData );
	/*PK*/
	void	SetPkWin ( const GLCHARLOGIC& sCharData );
	void	SetPkLoss ( const GLCHARLOGIC& sCharData );
	/* MultiKill Streaks */
	void	SetMegaKill ( const GLCHARLOGIC& sCharData );
	void	SetUnstoppable ( const GLCHARLOGIC& sCharData );
	void	SetWickedSick ( const GLCHARLOGIC& sCharData );
	void	SetMonsterKill ( const GLCHARLOGIC& sCharData );
	void	SetGodlike ( const GLCHARLOGIC& sCharData );
	void	SetHolyShit ( const GLCHARLOGIC& sCharData );
	void	SetOwnage ( const GLCHARLOGIC& sCharData );
	void SetSP(const GLCHARLOGIC& sCharData);
	/*Achievements*/
	void	SetActivity ( const GLCHARLOGIC& sCharData );
	void	SetContribution ( const GLCHARLOGIC& sCharData );
	void	SetEPoints ( const GLCHARLOGIC& sCharData );
	void	SetVPoints ( const GLCHARLOGIC& sCharData );
	void	SetSpendEPoints(const GLCHARLOGIC& sCharData);
	void	SetSpendEPUser(const GLCHARLOGIC& sCharData);
	void	SetTotalPaodian(const GLCHARLOGIC& sCharData);

private:
	CUIControl*		m_pBackGround;
	CBasicTextBox*	m_pNameText;

	CBasicTextBox*	m_pSchoolText;
	CBasicTextBox*	m_pClassText;
	CBasicTextBox*	m_pGuildText;
	/*PK*/
	CBasicTextBox*	m_pPkWinText;
	CBasicTextBox*	m_pPkLossText;
	/* MultiKill Streaks */
	CBasicTextBox*	m_pMegaKillText;
	CBasicTextBox*	m_pUnstoppableText;
	CBasicTextBox*	m_pWickedSickText;
	CBasicTextBox* m_pSPText;
	CBasicTextBox*	m_pMonsterKillText;
	CBasicTextBox*	m_pGodLikeText;
	CBasicTextBox*	m_pHolyShitText;
	CBasicTextBox*	m_pOwnageText;
	/*Achievements*/
	CBasicTextBox*	m_pActivityText;
	CBasicTextBox*	m_pContribText;
	CBasicTextBox*	m_pEPText;
	CBasicTextBox*	m_pVPText;
	CBasicTextBox* m_pSpendEPText;
	CBasicTextBox* m_pSpendEPUserText;
	CBasicTextBox* m_pTotalPaodianText;

private:
	CUIControl*					m_pSchoolMark[nSCHOOL_MARK];
	CUIControl*					m_pClassImg[GLCI_NUM_8CLASS];
};