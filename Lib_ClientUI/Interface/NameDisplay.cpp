﻿#include "StdAfx.h"
#include "NameDisplay.h"
#include "BasicLineBoxEx.h"
#include "./BasicTextBox.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"

#include "UITextControl.h"
#include "InnerInterface.h"
#include "DxClubMan.h"
#include "GLGaeaClient.h"
#include "RANPARAM.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNameDisplay::CNameDisplay()
	: m_bUsedMemPool(false)
	, m_bDISP(true)
	, m_pNameLineBoxCD(NULL)
	, m_bClub(false)
{
	memset(m_pSchoolMark, 0, sizeof(CUIControl*) * nSCHOOLMARK);
	memset(m_pPartyMark, 0, sizeof(CUIControl*) * nPARTYMARK);
	memset(m_pBuffIcon, 0, sizeof(CUIControl*) * nSkillBuff);
	memset(m_pRankMark, 0, sizeof(CUIControl*) * nRANKMARK); //MMR Rank

	/*pvp capture the flag, Juver, 2018/01/30 */
	memset(m_pCaptureTheFlagTeamMark, 0, sizeof(CUIControl*) * CAPTURE_THE_FLAG_TEAM_SIZE);
}

CNameDisplay::~CNameDisplay()
{
}

void CNameDisplay::INITIALIZE()
{
	m_bDISP = true;
	m_bClub = false;

	m_pNameBox->ClearText();
	m_pNameBox->SetLocalPos(m_rcNameBox);

	for (int i = 0; i < nSCHOOLMARK; ++i)
	{
		m_pSchoolMark[i]->SetVisibleSingle(FALSE);
		m_pSchoolMark[i]->SetLocalPos(m_rcSchoolMark[i]);
	}

	for (int i = 0; i < nPARTYMARK; ++i)
	{
		m_pPartyMark[i]->SetVisibleSingle(FALSE);
		m_pPartyMark[i]->SetLocalPos(m_rcPartyMark[i]);
	}
	for (int i = 0; i < nRANKMARK; ++i) //MMR Rank
	{
		m_pRankMark[i]->SetVisibleSingle(FALSE);
		m_pRankMark[i]->SetLocalPos(m_rcRankMark[i]);
	}
	m_pClubMark->SetVisibleSingle(FALSE);

	m_pNameLineBoxCD->SetVisibleSingle(FALSE);
	for (int i = 0; i < nSkillBuff; ++i)
	{
		m_pBuffIcon[i]->SetVisibleSingle(FALSE);
	}

	/*pvp capture the flag, Juver, 2018/01/30 */
	for (int i = 0; i < CAPTURE_THE_FLAG_TEAM_SIZE; ++i)
	{
		m_pCaptureTheFlagTeamMark[i]->SetVisibleSingle(FALSE);
		m_pCaptureTheFlagTeamMark[i]->SetLocalPos(m_rcCaptureTheFlagTeamMark[i]);
	}
}

void CNameDisplay::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);

	CBasicLineBoxEx* pLineBoxCD = new CBasicLineBoxEx;
	pLineBoxCD->CreateSub(this, "BASIC_LINE_BOX_EX_NAME", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	//disable box in HP Name Crow
	//pLineBoxCD->CreateBaseBoxNameCD ( "BASIC_NAME_LINE_BOX_CD" );
	pLineBoxCD->SetVisibleSingle(FALSE);
	RegisterControl(pLineBoxCD);
	m_pNameLineBoxCD = pLineBoxCD;

	CBasicTextBox* pNameBox = new CBasicTextBox;
	pNameBox->CreateSub(this, "NAME_DISPLAY_NAMEBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pNameBox->SetFont(pFont8);
	pNameBox->SetPartInterval(1.0f);
	pNameBox->SetLineInterval(0.0f);
	RegisterControl(pNameBox);
	m_pNameBox = pNameBox;

	{

		CString strBuffIcon[] =
		{
			"NAME_BUFF_ICON00",
			"NAME_BUFF_ICON01",
			"NAME_BUFF_ICON02",
			"NAME_BUFF_ICON03",
		};

		for (int i = 0; i < nSkillBuff; ++i)
		{
			m_pBuffIcon[i] = CreateControl(strBuffIcon[i].GetString(), UI_FLAG_LEFT);
			m_pBuffIcon[i]->SetVisibleSingle(FALSE);
		}
	}

	m_rcNameBox = m_pNameBox->GetLocalPos(); // MEMO : 기본 로컬포지션 저장

	{
		CString strSchoolMark[] =
		{
			"NAME_DISPLAY_SCHOOL1",
			"NAME_DISPLAY_SCHOOL2",
			"NAME_DISPLAY_SCHOOL3",
			"NAME_DISPLAY_GM"
		};

		for (int i = 0; i < nSCHOOLMARK; ++i)
		{
			m_pSchoolMark[i] = CreateControl(strSchoolMark[i].GetString(), UI_FLAG_LEFT);
			m_pSchoolMark[i]->SetVisibleSingle(FALSE);

			m_rcSchoolMark[i] = m_pSchoolMark[i]->GetLocalPos();
		}
	}

	{
		CString strPartyMark[] =
		{
			"NAME_DISPLAY_PARTY_MEMBER",
			"NAME_DISPLAY_PARTY_MASTER",
			"NAME_DISPLAY_PARTY_MEMBER_TARGET",
			"NAME_DISPLAY_PARTY_MASTER_TARGET",
			"NAME_DISPLAY_PARTY_MASTER_GENERAL"
		};

		for (int i = 0; i < nPARTYMARK; ++i)
		{
			m_pPartyMark[i] = CreateControl(strPartyMark[i].GetString(), UI_FLAG_RIGHT);
			m_pPartyMark[i]->SetVisibleSingle(FALSE);

			m_rcPartyMark[i] = m_pPartyMark[i]->GetLocalPos();
		}
	}
	{ //MMR Rank
		CString strRankMark[] =
		{
			"RANK_DISPLAY_1",
			"RANK_DISPLAY_2",
			"RANK_DISPLAY_3",
			"RANK_DISPLAY_4",
			"RANK_DISPLAY_5",
			"RANK_DISPLAY_6",
			"RANK_DISPLAY_7",
			"RANK_DISPLAY_8",
			"RANK_DISPLAY_9",
			"RANK_DISPLAY_10",
			"RANK_DISPLAY_11",
			"RANK_DISPLAY_12",
			"RANK_DISPLAY_13",
			"RANK_DISPLAY_14",
			"RANK_DISPLAY_15",
			"RANK_DISPLAY_16",
			"RANK_DISPLAY_17",
			"RANK_DISPLAY_18",
			"RANK_DISPLAY_19",
			"RANK_DISPLAY_20",
		};

		for (int i = 0; i < nRANKMARK; ++i)
		{
			m_pRankMark[i] = CreateControl(strRankMark[i].GetString(), UI_FLAG_RIGHT);
			m_pRankMark[i]->SetVisibleSingle(FALSE);

			m_rcRankMark[i] = m_pRankMark[i]->GetLocalPos();
		}
	}
	m_pClubMark = CreateControl("NAME_DISPLAY_CLUB_MARK", UI_FLAG_DEFAULT);
	m_pClubMark->SetVisibleSingle(FALSE);

	/*pvp capture the flag, Juver, 2018/01/30 */
	{
		CString strCaptureTheFlagMark[CAPTURE_THE_FLAG_TEAM_SIZE] =
		{
			"CAPTURE_THE_FLAG_ICON_1",
			"CAPTURE_THE_FLAG_ICON_2",
		};

		for (int i = 0; i < CAPTURE_THE_FLAG_TEAM_SIZE; ++i)
		{
			m_pCaptureTheFlagTeamMark[i] = CreateControl(strCaptureTheFlagMark[i].GetString(), UI_FLAG_LEFT);
			m_pCaptureTheFlagTeamMark[i]->SetVisibleSingle(FALSE);
			m_rcCaptureTheFlagTeamMark[i] = m_pCaptureTheFlagTeamMark[i]->GetLocalPos();
		}
	}
}

CUIControl* CNameDisplay::CreateControl(const char* szControl, WORD wAlignFlag)
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub(this, szControl, wAlignFlag);
	RegisterControl(pControl);
	return pControl;
}

/*pvp capture the flag, Juver, 2018/01/30 */
void CNameDisplay::SetName(const CString& strName, const BOOL bRankName, const BOOL bRankMark, const CString& strBadge, const D3DCOLOR& dwColor, WORD wSchoolMark, DISP_PARTY emnPartyMark,
	WORD wCaptureTheFlagTeam, WORD wMMR, bool bGM)
{
	if (wSchoolMark == NODATA && emnPartyMark == NODATA)
	{
		m_pNameBox->SetTextAlign(TEXT_ALIGN_CENTER_X);
	}

	/*hide gm info, Juver, 2017/11/26 */
	if (RANPARAM::bFeatureHideGMInfo && m_sDISP_INFO.m_bHideGMInfo)
	{
		//wSchoolMark = NODATA;
		//m_bClub = FALSE;
		//m_pClubMark->SetVisibleSingle ( FALSE );
	}

	const float fGapMarkName = 2.0f;
	const float fSchoolMark = m_pSchoolMark[0]->GetLocalPos().sizeX + fGapMarkName;
	const float fPartyMark = m_pPartyMark[0]->GetLocalPos().sizeX;
	const float fBuffMark = m_pBuffIcon[0]->GetLocalPos().sizeX;
	const float fGapMarkNameRank = 3.0f;//MMR Rank

	float fSchoolMarkEdit = 0;//MMR Rank
	if (bRankMark && GLCONST_CHAR::bEnableRankMark)
	{
		fSchoolMarkEdit = m_pSchoolMark[0]->GetLocalPos().sizeX + fGapMarkName;
	}
	else
	{
		fSchoolMarkEdit = m_pSchoolMark[0]->GetLocalPos().sizeX + fGapMarkName;
	}

	const float fRankMark = m_pRankMark[0]->GetLocalPos().sizeX + fGapMarkNameRank; //MMR Rank



	/*pvp capture the flag, Juver, 2018/01/31 */
	const float fCaptureTheFlagMark = m_pCaptureTheFlagTeamMark[0]->GetLocalPos().sizeX + fGapMarkName;
	/*if ( bRankName && GLCONST_CHAR::bEnableRankName )
	{

		const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();


		CBasicTextBox::STEXTMULTIWORDVEC vectext;
		CString strTitle;
		strTitle.Format( "<%s>", GLCONST_CHAR::strRank_Name[CalculateVal(sCharData.m_wLevel)].c_str() );

		CBasicTextBox::STEXTMULTIWORD word_title;
		word_title.strWord = strTitle.GetString();
		word_title.dwColor = NS_UITEXTCOLOR::DODGERBLUE;
		vectext.push_back( word_title );

		CBasicTextBox::STEXTMULTIWORD word_name;
		word_name.strWord = strName.GetString();
		word_name.dwColor = dwColor;
		vectext.push_back( word_name );

		m_pNameBox->AddMultiTextNoSplit( vectext );
	}
	else
	{
		m_pNameBox->AddTextNoSplit ( strName, dwColor );
	}*/

	/*activity system, Juver, 2017/11/05 */
	if (RANPARAM::bFeatureActivity && strBadge.GetLength())
	{
		CBasicTextBox::STEXTMULTIWORDVEC vectext;
		CString strTitle;
		CString strRank;

		if (bRankName && GLCONST_CHAR::bEnableRankName)
		{
			const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic();
			if (sCharData.m_ChaUsePoint < 10000) {
				strRank.Format("(VIP[0])");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_title;
				word_title.strWord = strTitle.GetString();
				word_title.dwColor = NS_UITEXTCOLOR::DODGERBLUE;
				vectext.push_back(word_title);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}

			if (sCharData.m_ChaUsePoint >= 10000 && sCharData.m_ChaUsePoint <= 29999) {
				strRank.Format("(VIP[1])");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_title;
				word_title.strWord = strTitle.GetString();
				word_title.dwColor = NS_UITEXTCOLOR::DODGERBLUE;
				vectext.push_back(word_title);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}

			if (sCharData.m_ChaUsePoint >= 30000 && sCharData.m_ChaUsePoint <= 59999) {
				strRank.Format("(VIP[2])");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_title;
				word_title.strWord = strTitle.GetString();
				word_title.dwColor = NS_UITEXTCOLOR::DODGERBLUE;
				vectext.push_back(word_title);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}

			if (sCharData.m_ChaUsePoint >= 60000 && sCharData.m_ChaUsePoint <= 1000000) {
				strRank.Format("(VIP3)");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_title;
				word_title.strWord = strTitle.GetString();
				word_title.dwColor = NS_UITEXTCOLOR::DODGERBLUE;
				vectext.push_back(word_title);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}
		}
		//strTitle.Format("[%s] ", strBadge.GetString());

	

		m_pNameBox->AddMultiTextNoSplit(vectext);
	}
	else {
		if (bRankName && GLCONST_CHAR::bEnableRankName)
		{
			CBasicTextBox::STEXTMULTIWORDVEC vectext;
			CString strRank;
			const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic();
			if (sCharData.m_ChaUsePoint < 10000) {
				strRank.Format("(VIP[0])");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}

			if (sCharData.m_ChaUsePoint >= 10000 && sCharData.m_ChaUsePoint <= 29999) {
				strRank.Format("(VIP[1])");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}

			if (sCharData.m_ChaUsePoint >= 30000 && sCharData.m_ChaUsePoint <= 59999) {
				strRank.Format("(VIP[2])");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}

			if (sCharData.m_ChaUsePoint >= 60000 && sCharData.m_ChaUsePoint <= 1000000) {
				strRank.Format("(VIP[3])");
				CBasicTextBox::STEXTMULTIWORD word_rank;
				word_rank.strWord = strRank.GetString();
				word_rank.dwColor = NS_UITEXTCOLOR::HOTPINK;
				vectext.push_back(word_rank);

				CBasicTextBox::STEXTMULTIWORD word_name;
				word_name.strWord = strName.GetString();
				word_name.dwColor = dwColor;
				vectext.push_back(word_name);
			}

		

			m_pNameBox->AddMultiTextNoSplit(vectext);
		}
		else {
			m_pNameBox->AddTextNoSplit(strName, dwColor);
		}

	}

	const UIRECT& GetControlVec = m_pNameBox->GetLocalPos();
	const float& fLineInterval = m_pNameBox->GetPartInterval();

	long lLineSize = m_pNameBox->GetLongestLine();

	if ((NODATA != wSchoolMark) && (wSchoolMark < nSCHOOLMARK))
	{
		if (bGM) m_pSchoolMark[3]->SetVisibleSingle(TRUE);
		else m_pSchoolMark[wSchoolMark]->SetVisibleSingle(TRUE);


		if (m_bClub)
		{
			const UIRECT& rcClubMark = m_pClubMark->GetLocalPos();
			const UIRECT& rcMark = m_pSchoolMark[wSchoolMark]->GetLocalPos();
			m_pSchoolMark[wSchoolMark]->SetLocalPos(D3DXVECTOR2(rcMark.left, rcMark.top + rcClubMark.sizeY + fLineInterval + 1.0f));
		}

		lLineSize += long(fSchoolMark);
		m_pNameBox->SetLocalPos(D3DXVECTOR2(GetControlVec.left + fSchoolMark, GetControlVec.top));
		m_pNameBox->SetTextAlign(TEXT_ALIGN_LEFT);

		/*pvp capture the flag, Juver, 2018/01/30 */
		if (wCaptureTheFlagTeam < CAPTURE_THE_FLAG_TEAM_SIZE)
		{
			m_pCaptureTheFlagTeamMark[wCaptureTheFlagTeam]->SetVisibleSingle(TRUE);
			const UIRECT& rcMark = m_pSchoolMark[wSchoolMark]->GetLocalPos();
			m_pCaptureTheFlagTeamMark[wCaptureTheFlagTeam]->SetLocalPos(rcMark);
		}
	}
	else
	{
		/*pvp capture the flag, Juver, 2018/01/30 */
		if (wCaptureTheFlagTeam < CAPTURE_THE_FLAG_TEAM_SIZE)
		{
			m_pCaptureTheFlagTeamMark[wCaptureTheFlagTeam]->SetVisibleSingle(TRUE);

			if (m_bClub)
			{
				const UIRECT& rcClubMark = m_pClubMark->GetLocalPos();
				const UIRECT& rcMark = m_pCaptureTheFlagTeamMark[wCaptureTheFlagTeam]->GetLocalPos();
				m_pCaptureTheFlagTeamMark[wCaptureTheFlagTeam]->SetLocalPos(D3DXVECTOR2(rcMark.left, rcMark.top + rcClubMark.sizeY + fLineInterval + 1.0f));
			}

			lLineSize += long(fCaptureTheFlagMark);
			m_pNameBox->SetLocalPos(D3DXVECTOR2(GetControlVec.left + fCaptureTheFlagMark, GetControlVec.top));
			m_pNameBox->SetTextAlign(TEXT_ALIGN_LEFT);
		}
	}

	if (PARTY_MASTER == emnPartyMark || PARTY_MASTER_TARGET == emnPartyMark || PARTY_MASTER_GENERAL == emnPartyMark)
	{
		m_pPartyMark[emnPartyMark]->SetVisibleSingle(TRUE);

		if (m_bClub)
		{
			const CBasicTextBox::STEXTPART& sPART1 = m_pNameBox->GetTextPart(0);
			const CBasicTextBox::STEXTPART& sPART2 = m_pNameBox->GetTextPart(1);

			long Line1Size = sPART1.dequeLine[0].strSize.cx;
			long Line2Size = long(sPART2.dequeLine[0].strSize.cx + fPartyMark);

			if (Line2Size > Line1Size)
			{
				lLineSize += long(fPartyMark);
			}
		}
		else
		{
			lLineSize += long(fPartyMark);
		}
	}

	float fGapX = GetControlVec.sizeX - float(lLineSize);
	float fCLUBSIZE = 0.0f;
	if (m_bClub)
	{
		fCLUBSIZE = m_pNameBox->CalcMaxHEIGHT(0, 0) + 2.0f;
	}

	const UIRECT& rcLocalPosOld = GetLocalPos();
	UIRECT rcLocalPosNew(rcLocalPosOld.left, rcLocalPosOld.top,
		rcLocalPosOld.sizeX - fGapX + 2.0f, rcLocalPosOld.sizeY + fCLUBSIZE);

	AlignSubControl(rcLocalPosOld, rcLocalPosNew);
	SetLocalPos(rcLocalPosNew);

	if ((NODATA != wSchoolMark) && (wSchoolMark < nSCHOOLMARK))
	{
		const UIRECT& rcLocalPos = m_pNameBox->GetLocalPos();
		m_pNameBox->SetLocalPos(UIRECT(rcLocalPos.left, rcLocalPos.top, rcLocalPos.sizeX - fSchoolMark, rcLocalPos.sizeY));
	}
	else
	{
		/*pvp capture the flag, Juver, 2018/01/30 */
		if (wCaptureTheFlagTeam < CAPTURE_THE_FLAG_TEAM_SIZE)
		{
			const UIRECT& rcLocalPos = m_pNameBox->GetLocalPos();
			m_pNameBox->SetLocalPos(UIRECT(rcLocalPos.left, rcLocalPos.top, rcLocalPos.sizeX - fCaptureTheFlagMark, rcLocalPos.sizeY));
		}
	}
	if (bRankMark && GLCONST_CHAR::bEnableRankMark) //MMR Rank
	{

	

		const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic();
		if (sCharData.m_ChaUsePoint < 10000) {
			m_pRankMark[19]->SetVisibleSingle(TRUE);
		}

		if (sCharData.m_ChaUsePoint >= 10000 && sCharData.m_ChaUsePoint <= 29999) {
			m_pRankMark[18]->SetVisibleSingle(TRUE);
			m_pRankMark[19]->SetVisibleSingle(FALSE);
		}

		if (sCharData.m_ChaUsePoint >= 30000 && sCharData.m_ChaUsePoint <= 59999) {
			m_pRankMark[17]->SetVisibleSingle(TRUE);
			m_pRankMark[18]->SetVisibleSingle(FALSE);
			m_pRankMark[19]->SetVisibleSingle(FALSE);
		}

		if (sCharData.m_ChaUsePoint >= 60000 && sCharData.m_ChaUsePoint <= 1000000) {
			m_pRankMark[16]->SetVisibleSingle(TRUE);
			m_pRankMark[19]->SetVisibleSingle(FALSE);
			m_pRankMark[19]->SetVisibleSingle(FALSE);
			m_pRankMark[19]->SetVisibleSingle(FALSE);
		}
		const UIRECT& rcNewMark2 = m_pNameBox->GetLocalPos();

		if (m_bClub)
		{
			const UIRECT& rcClubMark = m_pClubMark->GetLocalPos();
			const UIRECT& rcMark = m_pSchoolMark[wSchoolMark]->GetLocalPos();
			m_pRankMark[19]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 30 + rcClubMark.sizeY + fLineInterval + 1.0f));
			m_pRankMark[18]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 30 + rcClubMark.sizeY + fLineInterval + 1.0f));
			m_pRankMark[17]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 30 + rcClubMark.sizeY + fLineInterval + 1.0f));
			m_pRankMark[16]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 30 + rcClubMark.sizeY + fLineInterval + 1.0f));
		}
		else {

			const UIRECT& rcClubMark = m_pClubMark->GetLocalPos();
			const UIRECT& rcMark = m_pSchoolMark[wSchoolMark]->GetLocalPos();
			m_pRankMark[19]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 10));
			m_pRankMark[18]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 10));
			m_pRankMark[17]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 10));
			m_pRankMark[16]->SetLocalPos(D3DXVECTOR2(rcMark.left - 43, rcMark.top - 10));
		}
	}
	if (m_bClub)
	{
		if ((NODATA != emnPartyMark) && (emnPartyMark < nPARTYMARK))
		{
			float fRIGHT = 0.0f;

			const UIRECT& rcLocalPos = m_pNameBox->GetLocalPos();
			const float fHEIGHT = m_pNameBox->CalcMaxHEIGHT(0, 2);

			m_pNameBox->SetGlobalPos(UIRECT(rcLocalPos.left, rcLocalPos.top, rcLocalPos.sizeX, fHEIGHT));
			m_pNameBox->SetLocalPos(UIRECT(rcLocalPos.left, rcLocalPos.top, rcLocalPos.sizeX, fHEIGHT));

			if (m_pNameBox->GetCount() > 1)
			{
				UIRECT rcPart;
				m_pNameBox->GetPartInfo(1, rcPart);
				fRIGHT = rcPart.right;
			}
			else if (m_pNameBox->GetCount() == 1)
			{
				UIRECT rcPart;
				m_pNameBox->GetPartInfo(0, rcPart);
				fRIGHT = rcPart.right;
			}
			else
			{
				GASSERT(0 && "CNameDisplay::SetName(), m_pNameBox->GetCount <= 0");
				return;
			}

			const UIRECT& rcClubMark = m_pClubMark->GetLocalPos();
			const UIRECT& rcMark = m_pPartyMark[emnPartyMark]->GetLocalPos();
			m_pPartyMark[emnPartyMark]->SetLocalPos(D3DXVECTOR2(fRIGHT, rcMark.top + rcClubMark.sizeY + fLineInterval + 1.0f));
		}
	}

	SetGlobalPos(rcLocalPosNew);

	for (int i = 0; i < nSkillBuff; ++i)
	{
		const UIRECT& rcLocalPos = m_pNameBox->GetLocalPos();
		const UIRECT& rcMark = m_pBuffIcon[i]->GetLocalPos();
		m_pBuffIcon[i]->SetLocalPos(D3DXVECTOR2(rcMark.left, rcLocalPos.top - rcMark.sizeY));
	}
}

void CNameDisplay::SetClubName(const CString& strName, const D3DCOLOR& dwColor)
{
	if (m_pNameBox->GetCount())
	{
		GASSERT(0 && "클럽 이름 설정 순서가 적절하지 않습니다.");
		return;
	}

	/*hide gm info, Juver, 2017/11/26 */
	if (RANPARAM::bFeatureHideGMInfo && m_sDISP_INFO.m_bHideGMInfo)
	{
		m_pClubMark->SetVisibleSingle(FALSE);
		m_bClub = FALSE;
		return;
	}

	m_pNameBox->AddTextNoSplit(strName, dwColor);
	m_pClubMark->SetVisibleSingle(TRUE);
	m_bClub = true; // MEMO : 클럽으로 설정
}

void CNameDisplay::SetPetOwnerName(const CString& strName, const D3DCOLOR& dwColor)
{
	if (m_pNameBox->GetCount())
	{
		GASSERT(0 && "펫 이름 설정 순서가 적절하지 않습니다.");
		return;
	}

	m_pNameBox->AddTextNoSplit(strName, dwColor);
	m_bClub = true; // MEMO : 클럽으로 설정
}

void CNameDisplay::SetBuffIcon(int nIndex, bool bVisible)
{
	if (nIndex >= nSkillBuff) return;
	if (m_pBuffIcon[nIndex])
	{
		if (bVisible && !m_pBuffIcon[nIndex]->IsVisible())
		{
			m_pBuffIcon[nIndex]->SetVisibleSingle(TRUE);
		}
		else if (!bVisible && m_pBuffIcon[nIndex]->IsVisible())
		{
			m_pBuffIcon[nIndex]->SetVisibleSingle(FALSE);
		}
	}
}

HRESULT CNameDisplay::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
	GASSERT(pd3dDevice);

	if (!m_sDISP_INFO.IsCLUB()) return CUIGroup::Render(pd3dDevice);

	/*hide gm info, Juver, 2017/11/26 */
	if (RANPARAM::bFeatureHideGMInfo && m_sDISP_INFO.m_bHideGMInfo)
		return CUIGroup::Render(pd3dDevice);

	HRESULT hr = S_OK;

	// MEMO : 아래 코드는 테스트 용입니다.
	// 나중에 메세지를 받을 때만 변경되도록 수정해야 합니다.

	m_pNameLineBoxCD->SetVisibleSingle(FALSE);
	if (IsCDCERTIFY()) m_pNameLineBoxCD->SetVisibleSingle(TRUE);


	m_pClubMark->SetVisibleSingle(FALSE);
	{
		hr = CUIGroup::Render(pd3dDevice);
		if (FAILED(hr)) return hr;
	}
	m_pClubMark->SetVisibleSingle(TRUE);

	const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
	const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData(pd3dDevice, nSERVER, m_sDISP_INFO.m_dwCLUB, m_sDISP_INFO.m_dwCLUB_MARK_VER);

	m_pClubMark->SetTexturePos(0, sMarkData.vTex_1_LU);
	m_pClubMark->SetTexturePos(1, sMarkData.vTex_2_RU);
	m_pClubMark->SetTexturePos(3, sMarkData.vTex_3_LD);
	m_pClubMark->SetTexturePos(2, sMarkData.vTex_4_RD);

	const UIRECT& rcGlobalPos = m_pClubMark->GetGlobalPos();
	m_pClubMark->SetGlobalPos(UIRECT(ceil(rcGlobalPos.left), ceil(rcGlobalPos.top),
		ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY)));

	//	만들어야함
	m_pClubMark->SetTexture(sMarkData.pddsTexture);
	hr = m_pClubMark->Render(pd3dDevice);

	return hr;
}

//void	CNameDisplay::SetNameColor ( const D3DCOLOR& dwColor )
//{
//	if ( m_pNameBox->GetCount () > 1 )
//	{
//		m_pNameBox->SetUseTextColor ( 1, TRUE );
//		m_pNameBox->SetTextColor ( 1, dwColor );
//	}
//}
//
//void	CNameDisplay::SetClubNameColor ( const D3DCOLOR& dwColor )
//{
//	if ( m_pNameBox->GetCount () > 1 )
//	{
//		m_pNameBox->SetUseTextColor ( 0, TRUE );
//		m_pNameBox->SetTextColor ( 0, dwColor );
//	}
//}

bool CNameDisplay::DIFFERENT(const CROWREN& sINFO)
{
	return m_sDISP_INFO.DIFFERENT(sINFO);
}

void CNameDisplay::UPDATE(DWORD _dwCOUNT, D3DXVECTOR3 vPOS)
{
	m_sDISP_INFO.m_dwCOUNT = _dwCOUNT;
	m_sDISP_INFO.m_vPOS = vPOS;
}
