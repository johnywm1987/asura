///////////////////////////////////////////////////////////
////////////CHARACTER EXTRA STATS ARVIN.BC/////////////////
///////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "./CharacterWindowAdditional.h"

#include "BasicTextBox.h"
#include "BasicProgressBar.h"
#include "BasicButtonText.h"
#include "BasicButton.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"

#include "../BasicTextButton.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../Lib_Client/G-Logic/GLGaeaClient.h"

#include "../InnerInterface.h"
#include "../ModalCallerID.h"
#include "../ModalWindow.h"
#include "GLGaeaServer.h"

#include "../BasicLineBox.h"
#include "GLItemMan.h"
#include "GLItem.h"
#include "GLogicEx.h"

#include "GLCharData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterWindowAdditional::CCharacterWindowAdditional () :
	m_pBackGround( NULL ),
	m_pNameText ( NULL ),
	m_pSchoolText ( NULL ),
	m_pClassText ( NULL ),
	m_pGuildText ( NULL ),
	/* PK */
	m_pPkWinText ( NULL ),
	m_pPkLossText ( NULL ),
	/* MultiKill Streaks */
	m_pMegaKillText ( NULL ),
	m_pUnstoppableText ( NULL ),
	m_pWickedSickText ( NULL ),
	m_pSPText(NULL),
	m_pMonsterKillText ( NULL ),
	m_pGodLikeText ( NULL ),
	m_pHolyShitText ( NULL ),
	m_pOwnageText ( NULL ),
	/*Acvities*/
	m_pActivityText ( NULL ),
	m_pContribText ( NULL ),
	m_pEPText ( NULL ),
	m_pSpendEPText(NULL),
	m_pSpendEPUserText(NULL),
	m_pTotalPaodianText(NULL),
	m_pVPText ( NULL )

{
	memset( m_pSchoolMark, 0, sizeof ( CUIControl* ) * nSCHOOL_MARK );
	memset ( m_pClassImg, 0, sizeof( CUIControl* ) * GLCI_NUM_8CLASS );
}

CCharacterWindowAdditional::~CCharacterWindowAdditional ()
{
}

CBasicTextBox* CCharacterWindowAdditional::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

CUIControl*	CCharacterWindowAdditional::CreateControl ( const char* szControl, const UIGUID& cID )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	RegisterControl ( pControl );
	return pControl;
}

CBasicButton* CCharacterWindowAdditional::CreateMouseIn( const char* szButton, const char* szMouseIn ,UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szMouseIn, CBasicButton::MOUSEIN_FLIP );
	RegisterControl ( pButton );
	return pButton;
}


void CCharacterWindowAdditional::CreateSubControl ()
{
	
	CBasicLineBox* pBasicLineBox = new CBasicLineBox;
	pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pBasicLineBox->CreateBaseBoxOuterBlank( "CHARACTER_ADDITIONAL_WINDOW_LINEBOX" );
	RegisterControl ( pBasicLineBox );

	CD3DFontPar* pFontShadow = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	const int nAlignLeft = TEXT_ALIGN_LEFT;
	const int nAlignCenter = TEXT_ALIGN_CENTER_X;
	const int nAlignCenterBoth = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;
	const int nAlignRight = TEXT_ALIGN_RIGHT;
	const DWORD& dwSilver = NS_UITEXTCOLOR::SILVER;
	const DWORD& dwWhite = NS_UITEXTCOLOR::WHITE;
	const DWORD& dwSubTitle = NS_UITEXTCOLOR::PALEGOLDENROD;
	const DWORD& dwOrange = NS_UITEXTCOLOR::ORANGE;

	//LineBox
	char* szLineBox[4] = 
	{
		"CHARACTER_WINDOW_ADDITIONAL_REGION1",
		"CHARACTER_WINDOW_ADDITIONAL_REGION2",
		"CHARACTER_WINDOW_ADDITIONAL_REGION3",
		"CHARACTER_WINDOW_ADDITIONAL_REGION4",
	};

	for ( int i=0; i<4; ++i )
	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxQuestList( szLineBox[i] );
		RegisterControl ( pBasicLineBox );
	}

	CBasicTextBox* pTextBox = NULL;
	
	///////////////////////////////////////////////////////////////
	/// CHARACTER INFO
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_CHARCATERINFO_TITLE_TEXT", pFont, nAlignCenter );
	pTextBox->AddText ( "角色基本信息", dwOrange );
	m_pNameText = CreateStaticControl ( "CHARACTER_ADDITIONAL_NAMEBOX", pFont, nAlignLeft );
	m_pSchoolText = CreateStaticControl ( "CHARACTER_ADDITIONAL_SCHOOLBOX", pFont, nAlignLeft );
	m_pClassText = CreateStaticControl ( "CHARACTER_ADDITIONAL_CLASSBOX", pFont, nAlignLeft );
	m_pGuildText = CreateStaticControl ( "CHARACTER_ADDITIONAL_GUILDBOX", pFont, nAlignLeft );
	
	///////////////////////////////////////////////////////////////
	/// PKWIN | PKLOSS
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_PK_TITLE_TEXT", pFont, nAlignCenter );
	pTextBox->AddText ( "角色數據信息", dwOrange );
	m_pPkWinText = CreateStaticControl ( "CHARACTER_ADDITIONAL_PKWINBOX", pFont, nAlignLeft );
	m_pPkLossText = CreateStaticControl ( "CHARACTER_ADDITIONAL_PKLOSSBOX", pFont, nAlignLeft );
	
	///////////////////////////////////////////////////////////////
	/// Kill Streak Records
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_STREAK_TITLE_TEXT", pFont, nAlignCenter );
	pTextBox->AddText ( "角色擊殺數據", dwOrange );	
	m_pMegaKillText = CreateStaticControl ( "CHARACTER_ADDITIONAL_MEGAKILLBOX", pFont, nAlignLeft );
	m_pUnstoppableText = CreateStaticControl ( "CHARACTER_ADDITIONAL_UNSTOPPABLEBOX", pFont, nAlignLeft );
	m_pWickedSickText = CreateStaticControl ( "CHARACTER_ADDITIONAL_WICKEDSICKBOX", pFont, nAlignLeft );
	m_pMonsterKillText = CreateStaticControl ( "CHARACTER_ADDITIONAL_MONSTERKILLBOX", pFont, nAlignLeft );
	m_pGodLikeText = CreateStaticControl ( "CHARACTER_ADDITIONAL_GODLIKEBOX", pFont, nAlignLeft );
	m_pHolyShitText = CreateStaticControl ( "CHARACTER_ADDITIONAL_HOLYSHITBOX", pFont, nAlignLeft );
	m_pOwnageText = CreateStaticControl ( "CHARACTER_ADDITIONAL_OWNAGEBOX", pFont, nAlignLeft );
	m_pSPText = CreateStaticControl("CHARACTER_ADDITIONAL_SPBOX", pFont, nAlignLeft);
	
	///////////////////////////////////////////////////////////////
	/// Activity Records
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_ACTIVITYRECORDS_TITLE_TEXT", pFont, nAlignCenter );
	pTextBox->AddText ( "角色額外加成數據", dwOrange );	
	m_pActivityText = CreateStaticControl ( "CHARACTER_ADDITIONAL_ACTIVITYBOX", pFont, nAlignLeft );
	m_pContribText = CreateStaticControl ( "CHARACTER_ADDITIONAL_CONTRIBBOX", pFont, nAlignLeft );
	m_pEPText = CreateStaticControl ( "CHARACTER_ADDITIONAL_EPBOX", pFont, nAlignLeft );
	m_pVPText = CreateStaticControl ( "CHARACTER_ADDITIONAL_VPBOX", pFont, nAlignLeft );
	m_pSpendEPText = CreateStaticControl("CHARACTER_ADDITIONAL_SPENTEPBOX", pFont, nAlignLeft);
	m_pSpendEPUserText = CreateStaticControl("CHARACTER_ADDITIONAL_SPENTEPUSERBOX", pFont, nAlignLeft);
	m_pTotalPaodianText = CreateStaticControl("CHARACTER_ADDITIONAL_TOTALPAODIANBOX", pFont, nAlignLeft);
	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_NAMEBOX_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "角色名稱", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_SCHOOL_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "學院", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_CLASS_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "職業", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_GUILD_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "組織", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_KILLS_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "擊殺", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_DEATHS_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "死亡", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_MEGAKILL_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "暴擊打擊幾率", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_UNSTOPPABLE_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "毀滅打擊幾率", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_WICKED_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "移動速度加成", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_MONSTER_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "攻擊速度加成", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_GODLIKE_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "經驗值加成", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_HOLYSHIT_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "HP回復率", dwSilver );	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_OWNAGE_STATIC", pFont, nAlignLeft );
	pTextBox->AddText ( "MP回復率", dwSilver );		
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_ACTIVITY_STATIC", pFont, nAlignLeft );
	pTextBox->AddText("活動點數", dwSilver);

	
	
	pTextBox = CreateStaticControl("CHARACTER_ADDITIONAL_SP_STATIC", pFont, nAlignLeft);
	pTextBox->AddText ( "SP回復率", dwSilver );		
	
	pTextBox = CreateStaticControl("CHARACTER_ADDITIONAL_CONTRIBUTION_STATIC", pFont, nAlignLeft);
	pTextBox->AddText ( "貢獻點數", dwSilver );		
	
	pTextBox = CreateStaticControl("CHARACTER_ADDITIONAL_EP_STATIC", pFont, nAlignLeft);
	pTextBox->AddText ( "積分點數", dwSilver );		
	
	pTextBox = CreateStaticControl("CHARACTER_ADDITIONAL_SPENT_EP_USER_STATIC", pFont, nAlignLeft);
	pTextBox->AddText("角色會員等級", dwSilver);
	

	pTextBox = CreateStaticControl("CHARACTER_ADDITIONAL_SPENT_EP_STATIC", pFont, nAlignLeft);
	pTextBox->AddText("累積消費積分", dwSilver);
	
	pTextBox = CreateStaticControl ( "CHARACTER_ADDITIONAL_VP_STATIC", pFont, nAlignLeft );	
	pTextBox->AddText ( "泡點點數", dwSilver );		

	pTextBox = CreateStaticControl("CHARACTER_ADDITIONAL_PAODIAN_STATIC", pFont, nAlignLeft);
	pTextBox->AddText("總獲得泡點", dwSilver);
	
	{
		CUIControl	TempControl;
		TempControl.Create ( 1, "CHARACTER_ADDITIONAL_SCHOOL_MARK" );

		const UIRECT& rcLocalPos = TempControl.GetLocalPos ();

		CString strSchoolMark[] = 
		{
			"CHARACTER_ADDITIONAL_WINDOW_SCHOOL1",
			"CHARACTER_ADDITIONAL_WINDOW_SCHOOL2",
			"CHARACTER_ADDITIONAL_WINDOW_SCHOOL3"
		};

		for ( int i = 0; i < nSCHOOL_MARK; ++i )
		{
			m_pSchoolMark[i] = CreateControl ( strSchoolMark[i].GetString(), INFO_DISPLAY_SG+i );
			m_pSchoolMark[i]->SetLocalPos ( rcLocalPos );
			m_pSchoolMark[i]->SetVisibleSingle ( FALSE );
		}

	}
	
	CString strTemp;
	for ( int i = 0 ; i < GLCI_NUM_8CLASS; ++i ) 
	{
		strTemp.Format ( "CHARACTER_ADDITIONAL_ICON_CLASS_IMAGE%d",i); 	
		m_pClassImg[i] = new CUIControl;
		m_pClassImg[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, ICON_CLASS_IMAGE0+i );	
		m_pClassImg[i]->SetVisibleSingle ( FALSE );
		m_pClassImg[i]->SetTransparentOption( TRUE );
		RegisterControl ( m_pClassImg[i] );
	}

}

void CCharacterWindowAdditional::SetGlobalPos(const D3DXVECTOR2& vPos)
{
	CUIGroup::SetGlobalPos( vPos );
}

void CCharacterWindowAdditional::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
		case ET_CONTROL_TITLE:
		case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SetDefaultPosInterface( CHARACTER_ADDITIONAL_WINDOW );
			}
		}
		break;
	}
}

void CCharacterWindowAdditional::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	for ( int i = 0 ; i < GLCI_NUM_8CLASS; ++i ) 
		m_pClassImg[i]->SetVisibleSingle(FALSE);

	const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();	

	m_pSchoolMark[sCharData.m_wSchool]->SetVisibleSingle ( TRUE );
	m_pClassImg[CharClassToIndex(sCharData.m_emClass)]->SetVisibleSingle(TRUE);

	/*Basic Info*/
	SetName ( sCharData );
	SetSchool ( sCharData );
	SetClass ( sCharData );
	SetGuild ( sCharData );
	/*PK*/
	SetPkWin ( sCharData );
	SetPkLoss ( sCharData );
	/* MultiKill Streaks */
	SetMegaKill ( sCharData );
	SetUnstoppable ( sCharData );
	SetWickedSick ( sCharData );
	SetMonsterKill ( sCharData );
	SetGodlike ( sCharData );
	SetHolyShit ( sCharData );
	SetOwnage ( sCharData );
	SetSP(sCharData);
	/*Achievements*/
	SetActivity ( sCharData );
	SetContribution ( sCharData );
	SetSpendEPoints(sCharData);
	SetEPoints ( sCharData );
	SetVPoints ( sCharData );
	SetSpendEPUser(sCharData);
	SetTotalPaodian(sCharData);
	
	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCharacterWindowAdditional::SetName ( const GLCHARLOGIC& sCharData )
{
	if ( m_pNameText )
	{
		m_pNameText->SetOneLineText ( sCharData.m_szName, NS_UITEXTCOLOR::GREENYELLOW );
	}
}
void CCharacterWindowAdditional::SetSchool ( const GLCHARLOGIC& sCharData )
{
	if ( m_pSchoolText )
	{
		CString strCombine;
		strCombine.Format ( "%s", ID2GAMEWORD("ACADEMY_NAME", sCharData.m_wSchool) );
		m_pSchoolText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::GREENYELLOW );
	}
}
void CCharacterWindowAdditional::SetClass ( const GLCHARLOGIC& sCharData )
{
	if ( m_pClassText )
	{
		CString strCombine;
		strCombine.Format ( "%s",COMMENT::CHARCLASS[CharClassToIndex( sCharData.m_emClass)].c_str() );
		m_pClassText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::GREENYELLOW );
	}	
}
void CCharacterWindowAdditional::SetGuild ( const GLCHARLOGIC& sCharData )
{
	if ( m_pGuildText )
	{

		GLCLUB& sCLUB = GLGaeaClient::GetInstance().GetCharacter ()->m_sCLUB;

		CString strGuildCombine;
		if( sCharData.m_dwGuild > 0 )
			strGuildCombine.Format ("%s", sCLUB.m_szName);
		else
			strGuildCombine.Format ("None" );

		m_pGuildText->SetOneLineText ( strGuildCombine, NS_UITEXTCOLOR::GREENYELLOW );
	}
}
void CCharacterWindowAdditional::SetPkWin ( const GLCHARLOGIC& sCharData )
{
	if ( m_pPkWinText )
	{
		CString strTemp;
		strTemp.Format("%d", sCharData.m_dwPKScore);
		m_pPkWinText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}
}
void CCharacterWindowAdditional::SetPkLoss ( const GLCHARLOGIC& sCharData )
{
	if ( m_pPkLossText )
	{
		CString strTemp;
		strTemp.Format("%d", sCharData.m_dwPKDeath);
		m_pPkLossText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}
}
void CCharacterWindowAdditional::SetMegaKill ( const GLCHARLOGIC& sCharData )
{
	
		CString strTemp;
		
		strTemp.Format("%.2f", sCharData.m_fCrit_Rate * 100);
		if (m_pMegaKillText) m_pMegaKillText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
	
}
void CCharacterWindowAdditional::SetUnstoppable ( const GLCHARLOGIC& sCharData )
{
	
		CString strTemp;
		strTemp.Format("%.2f", sCharData.m_fBlow_Rate * 100);
		if (m_pUnstoppableText) m_pUnstoppableText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
	
}
void CCharacterWindowAdditional::SetWickedSick ( const GLCHARLOGIC& sCharData )
{
	
		CString strTemp;
		strTemp.Format("%2.2f", sCharData.m_fIncR_MoveSpeed * 100);
		m_pWickedSickText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW);
	
}
void CCharacterWindowAdditional::SetMonsterKill ( const GLCHARLOGIC& sCharData )
{
	
		CString strTemp;
		strTemp.Format("%2.2f", sCharData.m_fIncR_AtkSpeed * 100);
		m_pMonsterKillText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	
}
void CCharacterWindowAdditional::SetGodlike ( const GLCHARLOGIC& sCharData )
{
	
		CString strTemp;
		strTemp.Format("%2.2f", sCharData.m_fEXP_RATE);
		m_pGodLikeText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	
}
void CCharacterWindowAdditional::SetHolyShit ( const GLCHARLOGIC& sCharData )
{

		CString strTemp;
		strTemp.Format("%2.2f", sCharData.m_fINCR_HP * 100.0f);
		m_pHolyShitText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
		
	
}
void CCharacterWindowAdditional::SetOwnage ( const GLCHARLOGIC& sCharData )
{

		CString strTemp;
		strTemp.Format("%2.2f", sCharData.m_fINCR_MP * 100.0f);
		m_pOwnageText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	
}
void CCharacterWindowAdditional::SetSP(const GLCHARLOGIC& sCharData)
{

	CString strTemp;
	strTemp.Format("%2.2f", sCharData.m_fINCR_SP * 100.0f);
	m_pSPText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);

}
void CCharacterWindowAdditional::SetActivity ( const GLCHARLOGIC& sCharData )
{
	
		CString strTemp;
		strTemp.Format("%d", sCharData.m_dwActivityPoint);
		m_pActivityText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	
}
void CCharacterWindowAdditional::SetContribution ( const GLCHARLOGIC& sCharData )
{
	if ( m_pContribText )
	{
		CString strTemp;
		strTemp.Format("%d",sCharData.m_llContributionPoint);
		m_pContribText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}
}
void CCharacterWindowAdditional::SetEPoints ( const GLCHARLOGIC& sCharData )
{
	if ( m_pEPText )
	{
		CString strTemp;
		strTemp.Format("%d",sCharData.m_dwPremiumPoints);
		m_pEPText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}
}
void CCharacterWindowAdditional::SetSpendEPoints(const GLCHARLOGIC& sCharData)
{
	if (m_pSpendEPText)
	{
		CString strTemp;
		strTemp.Format("%d", sCharData.m_ChaUsePoint);
		m_pSpendEPText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
	}
}
void CCharacterWindowAdditional::SetSpendEPUser(const GLCHARLOGIC& sCharData)
{
	if (m_pSpendEPUserText)
	{
		CString strTemp;
		if (sCharData.m_ChaUsePoint<10000) {
			strTemp.Format("VIP(0)");
			m_pSpendEPUserText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
		}
		if (sCharData.m_ChaUsePoint >= 10000 && sCharData.m_ChaUsePoint <= 29999) {
			strTemp.Format("VIP(1)");
			m_pSpendEPUserText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
		}
		if (sCharData.m_ChaUsePoint >= 30000 && sCharData.m_ChaUsePoint <= 59999) {
			strTemp.Format("VIP(2)");
			m_pSpendEPUserText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
		}
		if (sCharData.m_ChaUsePoint >= 60000 && sCharData.m_ChaUsePoint <= 1000000) {
			strTemp.Format("VIP(3)");
			m_pSpendEPUserText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
		}
		
	}
}


void CCharacterWindowAdditional::SetVPoints ( const GLCHARLOGIC& sCharData )
{
	if ( m_pVPText )
	{
		CString strTemp;
		strTemp.Format("%d",sCharData.m_dwCombatPoints);
		m_pVPText->SetOneLineText ( strTemp, NS_UITEXTCOLOR::GREENYELLOW );
	}
}

void CCharacterWindowAdditional::SetTotalPaodian(const GLCHARLOGIC& sCharData) {
	if (m_pTotalPaodianText) {
	
		
		
			CString strTemp;
			strTemp.Format("%d", sCharData.m_dwChaBuyPoint);
			m_pTotalPaodianText->SetOneLineText(strTemp, NS_UITEXTCOLOR::GREENYELLOW);
		
	}
}
void CCharacterWindowAdditional::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
}