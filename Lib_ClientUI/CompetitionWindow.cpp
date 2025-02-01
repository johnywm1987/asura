#include "StdAfx.h"
#include "CompetitionWindow.h"
#include "BasicTreeEx.h"
#include "BasicLineBoxEx.h"
#include "GLGaeaClient.h"
#include "InnerInterface.h"
#include "BasicTextButton.h"
#include "BasicTextBox.h"
#include "GameTextControl.h"
#include "../Lib_Engine/DxCommon/DxFontMan.h"
#include "../Lib_Engine/DxCommon/d3dfont.h"
#include "BasicTextButton.h" 
#include "DxGlobalStage.h"
#include "BasicTextBox.h"
#include "BasicScrollBarEx.h"
#include "BasicScrollThumbFrame.h"
#include "UITextControl.h"

#include "PVPTyrannyPage.h"
#include "PVPSchoolWarsPage.h"
#include "PVPCaptureTheFlagPage.h"
//#include "PVPFreePK/PVPFreePKPage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int CCompetitionWindow::nSTARTLINE = 0;
const int CCompetitionWindow::nLIMIT_LINE = 1000;

CCompetitionWindow::CCompetitionWindow()
	: m_pLineButton(NULL)
	, m_pBasicLineBox(NULL)
	, m_pTyrannyPage(NULL)
	, m_pSchoolwarsPage(NULL)
	, m_pCaptureTheFlag(NULL)
	, m_pPageFreePK(NULL)
	, m_bCHECK_MOUSE_STATE(false)
	, m_bFirstGap(FALSE)
	, m_PosX(0)
	, m_PosY(0)
	, m_pTreeScroll(NULL)
	, m_pTree(NULL)
{
}

CCompetitionWindow::~CCompetitionWindow()
{
}

CBasicTextButton* CCompetitionWindow::CreateTextButton23(const char* szButton, UIGUID ControlID, const char* szText)
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE23;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE | UI_FLAG_YSIZE, ControlID);
	pTextButton->CreateBaseButton(szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText);
	RegisterControl(pTextButton);
	return pTextButton;
}

CBasicTextButton* CCompetitionWindow::CreateTextButton18(const char* szButton, UIGUID ControlID, const char* szText)
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE18;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE | UI_FLAG_YSIZE, ControlID);
	pTextButton->CreateBaseButton(szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText);
	RegisterControl(pTextButton);
	return pTextButton;
}

CBasicTextButton* CCompetitionWindow::CreateTextButton(char* szButton, UIGUID ControlID, char* szText)
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE19;
	CBasicTextButton* pButton = new CBasicTextButton;
	pButton->CreateSub(this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ControlID);
	pButton->CreateBaseButton(szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText);
	RegisterControl(pButton);

	return pButton;
}

void CCompetitionWindow::CreateSubControl()
{

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 12, D3DFONT_SHADOW | D3DFONT_ASCII);
	D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;


	m_pBasicLineBox = new CBasicLineBoxEx;
	m_pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_DIALOGUE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pBasicLineBox->CreateBaseBoxDialogueAnswer("RN_COMPETITION_BACK_BOX");
	m_pBasicLineBox->SetVisibleSingle(TRUE);
	RegisterControl(m_pBasicLineBox);
	{
		CBasicTreeEx* pTree = new CBasicTreeEx;
		pTree->CreateSub(this, "PVP_WINDOW_TREE", UI_FLAG_DEFAULT, PVP_WINDOW_TREE);
		pTree->SetFont(DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 12, D3DFONT_SHADOW | D3DFONT_ASCII));
		pTree->SetUseLabelOverColor(TRUE);
		pTree->SetLabelOverColor(NS_UITEXTCOLOR::ORANGE);
		RegisterControl(pTree);
		m_pTree = pTree;

		LoadTreePVP();
		LoadTreePVE();
		//LoadTreeRK();
	}

	{
		CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
		pScrollBar->CreateSub(this, "BASIC_SCROLLBAR", UI_FLAG_YSIZE);
		pScrollBar->CreateBaseScrollBar("PVP_TREE_SCROLLBAR");
		pScrollBar->GetThumbFrame()->SetState(1, 1);
		RegisterControl(pScrollBar);
		m_pTreeScroll = pScrollBar;
	}

	{
		CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
		pScrollBar->CreateSub(this, "BASIC_SCROLLBAR", UI_FLAG_YSIZE);
		pScrollBar->CreateBaseScrollBar("PVP_TREE_SCROLLBAR2");
		pScrollBar->GetThumbFrame()->SetState(1, 1);
		RegisterControl(pScrollBar);
		m_pTreeScroll = pScrollBar;
	}

	{
		m_pTyrannyPage = new CPVPTyrannyPage;
		m_pTyrannyPage->CreateSub(this, "TW_PAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		m_pTyrannyPage->CreateSubControl();
		m_pTyrannyPage->SetVisibleSingle(TRUE);
		RegisterControl(m_pTyrannyPage);
	}

	{
		m_pSchoolwarsPage = new CPVPSchoolWarsPage;
		m_pSchoolwarsPage->CreateSub(this, "SW_PAGE", UI_FLAG_LEFT);
		m_pSchoolwarsPage->CreateSubControl();
		m_pSchoolwarsPage->SetVisibleSingle(FALSE);
		RegisterControl(m_pSchoolwarsPage);
	}

	{
		m_pCaptureTheFlag = new CPVPCaptureTheFlagPage;
		m_pCaptureTheFlag->CreateSub(this, "CTF_PAGE", UI_FLAG_LEFT);
		m_pCaptureTheFlag->CreateSubControl();
		m_pCaptureTheFlag->SetVisibleSingle(FALSE);
		RegisterControl(m_pCaptureTheFlag);
	}

	/*{
		m_pPageFreePK = new CPVPFreePKPage;
		m_pPageFreePK->CreateSub(this, "BASIC_WINDOW", UI_FLAG_RIGHT, FK_PAGE);
		m_pPageFreePK->CreateSubControl();
		m_pPageFreePK->SetVisibleSingle(FALSE);
		RegisterControl(m_pPageFreePK);

		UIRECT RcOLD = m_pPageFreePK->GetLocalPos();
		UIRECT RcNEW = UIRECT(
			m_pCaptureTheFlag->GetLocalPos().right,
			m_pCaptureTheFlag->GetLocalPos().top,
			RcOLD.sizeX,
			RcOLD.sizeY);

	}*/
}

void  CCompetitionWindow::LoadTreePVP()
{
	CBasicTreeNode* pTreeNode = m_pTree->InsertNodePVP(NULL, ID2GAMEWORD("PVP_WINDOW_TREE", 0), FALSE);
	pTreeNode->SetNodeData((DWORD_PTR)PVP_PAGE);
	LoadNode(pTreeNode, TW_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_PVP", 0));
	LoadNode(pTreeNode, SW_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_PVP", 1));
	LoadNode(pTreeNode, CTF_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_PVP", 2));
//	LoadNode(pTreeNode, FK_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_PVP", 3));

}
void  CCompetitionWindow::LoadTreePVE()
{
	CBasicTreeNode* pTreeNode = m_pTree->InsertNodePVP(NULL, ID2GAMEWORD("PVP_WINDOW_TREE", 1), FALSE);
	pTreeNode->SetNodeData((DWORD_PTR)PVE_PAGE);
	LoadNode(pTreeNode, PVE_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_PVE", 0));
	LoadNode(pTreeNode, PVE_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_PVE", 1));
	LoadNode(pTreeNode, PVE_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_PVE", 2));
}
void  CCompetitionWindow::LoadTreeRK()
{
	CBasicTreeNode* pTreeNode = m_pTree->InsertNodePVP(NULL, ID2GAMEWORD("PVP_WINDOW_TREE", 2), FALSE);
	pTreeNode->SetNodeData((DWORD_PTR)RK_PAGE);
	LoadNode(pTreeNode, PK_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_RANK", 0));
	LoadNode(pTreeNode, RICH_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_RANK", 1));
	LoadNode(pTreeNode, LVL_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_RANK", 2));
	LoadNode(pTreeNode, OL_PAGE, (char*)ID2GAMEWORD("PVP_WINDOW_TREE_RANK", 3));
}

void CCompetitionWindow::LoadNode(CBasicTreeNode* pParent, DWORD dwData, char* szText)
{
	CBasicTreeNode* pTreeNode = m_pTree->InsertNodePVP(pParent, szText, TRUE, FALSE);
	pTreeNode->SetNodeData((DWORD_PTR)dwData);
}

void CCompetitionWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
	{
		if ((dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN(dwMsg))
		{
			CInnerInterface::GetInstance().SetDefaultPosInterface(GetWndID());
		}
	}break;
	case ET_CONTROL_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CInnerInterface::GetInstance().HideGroup(GetWndID());
		}
	}break;
	case PVP_WINDOW_TREE:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CBasicTreeNode* pNode = m_pTree->GetSelectedNode();
			if (pNode)
			{
				switch (pNode->GetNodeData())
				{
				case TW_PAGE: OpenPage(1);	break;
				case SW_PAGE: OpenPage(2);	break;
				case CTF_PAGE: OpenPage(3);	break;
				case FK_PAGE: OpenPage(4);	break;
				};
			}
		}
	}break;

	};
	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CCompetitionWindow::CheckMouseState()
{
	const DWORD dwMsg = GetMessageEx();
	if (CHECK_MOUSE_IN(dwMsg))
	{
		if (UIMSG_LB_DUP & dwMsg)
			return;

		if (dwMsg & UIMSG_LB_DOWN)
		{
			SetExclusiveControl();

			if (!m_bFirstGap)
			{
				UIRECT rcPos = GetGlobalPos();
				m_vGap.x = m_PosX - rcPos.left;
				m_vGap.y = m_PosY - rcPos.top;
				m_bFirstGap = TRUE;
			}
		}
		else if (CHECK_LB_UP_LIKE(dwMsg))
		{
			ResetExclusiveControl();
			m_bFirstGap = FALSE;
		}
	}
	else if (CHECK_LB_UP_LIKE(dwMsg))
	{
		ResetExclusiveControl();
		m_bFirstGap = FALSE;
	}
}

void CCompetitionWindow::OpenPage(int nPage)
{
	m_pTyrannyPage->SetVisibleSingle(FALSE);
	m_pSchoolwarsPage->SetVisibleSingle(FALSE);
	m_pCaptureTheFlag->SetVisibleSingle(FALSE);
	//m_pPageFreePK->SetVisibleSingle(FALSE);
	switch (nPage)
	{
	case 1:
	{
		m_pTyrannyPage->SetVisibleSingle(TRUE);
		m_pTreeScroll->SetVisibleSingle(TRUE);
	}break;
	case 2:
	{
		m_pSchoolwarsPage->SetVisibleSingle(TRUE);
		m_pTreeScroll->SetVisibleSingle(FALSE);
	}break;
	case 3:
	{
		m_pCaptureTheFlag->SetVisibleSingle(TRUE);
		m_pTreeScroll->SetVisibleSingle(FALSE);
	}break;
	/*case 4:
	{
		m_pPageFreePK->SetVisibleSingle(TRUE);
		m_pTreeScroll->SetVisibleSingle(FALSE);
	}break;*/
	};
}

void CCompetitionWindow::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	if (m_pTreeScroll)
	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pTreeScroll->GetThumbFrame();
		m_pTree->SetCurLine(0);

		const int nTotalLine = m_pTree->GetTotalLine();
		const int nLinePerOneView = m_pTree->GetLinePerOneView();
		int nZeroTest = nTotalLine;
		int nZeroTest2 = nLinePerOneView;

		if (!nZeroTest) nZeroTest = 1;
		if (!nZeroTest2) nZeroTest2 = 1;

		pThumbFrame->SetState(nZeroTest, nZeroTest2);



		if (nLinePerOneView < nTotalLine)
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent();
			int nPos = (int)floor(fPercent * nMovableLine);

			if (nPos < nSTARTLINE) nPos = nSTARTLINE;


			m_pTree->SetCurLine(nPos);
		}
	}

	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

}