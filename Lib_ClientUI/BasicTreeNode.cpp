#include "StdAfx.h"

#include "BasicTreeNode.h"
#include "BasicButton.h"
//#include "BasicTextBox.h"
#include "../DxCommon/DxFontMan.h"
#include "BasicLineBoxEx.h"
#include "TipBox.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicTreeNode::CBasicTreeNode() :
	m_pButton(NULL),
	m_pLabel(NULL),
	m_pParentNode(NULL),
	m_pBasicLineBox(NULL),
	m_bLineBoxVisible(FALSE),
	m_bBranchVisible(FALSE)
{
}

CBasicTreeNode::~CBasicTreeNode()
{
}

void	CBasicTreeNode::SetVisibleChild(const TREENODE_LIST& list, BOOL bVisible)
{
	TREENODE_LIST_CITER iter = list.begin();
	TREENODE_LIST_CITER iter_end = list.end();
	for (; iter != iter_end; ++iter)
	{
		(*iter)->SetVisibleSingle(bVisible);
		SetVisibleChild((*iter)->GetChildList(), bVisible);
	}
}

void CBasicTreeNode::SetBranchVisible(BOOL bBranchVisible)
{
	if (m_ChildList.empty()) return;

	m_bBranchVisible = bBranchVisible;

	if (bBranchVisible)
	{
		TREENODE_LIST_ITER iter = m_ChildList.begin();
		TREENODE_LIST_ITER iter_end = m_ChildList.end();
		for (; iter != iter_end; ++iter)
		{
			(*iter)->SetVisibleSingle(TRUE);
		}
	}
	else
	{
		SetVisibleChild(m_ChildList, FALSE);
	}
}

void CBasicTreeNode::CreateLabel(char* szKeyword, bool bCentered, bool bLineBox, bool bBOLD)
{
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);
	CD3DFontPar* pFont11 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	if (bLineBox)
	{	//Select Tree Bubu
		CString str;
		str.Format("%s_LINEBOX", szKeyword);
		CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
		pLineBox->CreateSub(this, "BASIC_VAR_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pLineBox->CreateBaseBoxVarTextBoxBubu((char*)str.GetString());
		RegisterControl(pLineBox);
		m_pBasicLineBox = pLineBox;
		m_bLineBoxVisible = bLineBox;
	}
	else
	{	//Select PageTree Bubu
		CString str;
		str.Format("%s_LINEBOX", szKeyword);
		CTipBox* pLineBox = new CTipBox;
		pLineBox->CreateSub(this, "QUEST_WINDOW_CHILD_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pLineBox->CreateQuestList((char*)str.GetString());
		pLineBox->SetVisibleSingle(TRUE);
		RegisterControl(pLineBox);

		//		m_pSelect = new CTipBox;
		//		m_pSelect->CreateSub( this, "QUEST_WINDOW_CHILD_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//		m_pSelect->CreateQuestSelect( "RNCOMPETITION_TYPE_SUB1" );
		//		RegisterControl ( m_pSelect );
		//		m_pSelect->SetVisibleSingle( TRUE );
	}

	CBasicTextBox* pLabel = new CBasicTextBox;
	pLabel->CreateSub(this, szKeyword, UI_FLAG_DEFAULT, TREENODE_TEXTBOX);
	(bBOLD) ? pLabel->SetFont(pFont10) : pLabel->SetFont(pFont11);
	(bCentered) ? pLabel->SetTextAlign(TEXT_ALIGN_CENTER_X) : pLabel->SetTextAlign(TEXT_ALIGN_LEFT);
	RegisterControl(pLabel);
	m_pLabel = pLabel;
}

void CBasicTreeNode::CreateButton(char* szKeyword, char* szFlipKeyword)
{
	CString strKeyWord, strKeyWordFlip;

	strKeyWord.Format("%s_OVER", szKeyword);
	strKeyWordFlip.Format("%s_OVER", szFlipKeyword);
	char szKeyWordOver[256];
	char szKeyWordFlipOver[256];

	strcpy(szKeyWordOver, strKeyWord);
	strcpy(szKeyWordFlipOver, strKeyWordFlip);

	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub(this, szKeyword, UI_FLAG_DEFAULT, TREENODE_EXPAND_BUTTON);
	pButton->CreateFlip(szFlipKeyword, CBasicButton::RADIO_FLIP);
	pButton->CreateMouseOver(szKeyWordOver);
	pButton->CreateFlipMouseOver(szKeyWordFlipOver);
	pButton->SetVisibleSingle(FALSE);
	RegisterControl(pButton);
	m_pButton = pButton;
}

void CBasicTreeNode::InsertChildNode(CBasicTreeNode* pChild)
{
	m_ChildList.push_back(pChild);
	if (m_pButton) m_pButton->SetVisibleSingle(TRUE);
}

CBasicTreeNode* CBasicTreeNode::GetChildNode()
{
	if (m_ChildList.empty()) return NULL;
	return m_ChildList.front();
}

void CBasicTreeNode::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	m_pLabel->SetUseOverColor(FALSE);
	if (m_pBasicLineBox)	m_pBasicLineBox->SetVisibleSingle(FALSE);

	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}

void CBasicTreeNode::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIGroup::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	case TREENODE_EXPAND_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetBranchVisible(!m_bBranchVisible);
			m_pButton->SetFlip(m_bBranchVisible);
		}
	}
	break;

	case TREENODE_TEXTBOX:
	case TREENODE_IMAGE:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			m_pLabel->SetUseOverColor(TRUE);
			if (m_pBasicLineBox) m_pBasicLineBox->SetVisibleSingle(TRUE);
			if (dwMsg & UIMSG_LB_DUP)
			{
				SetBranchVisible(!m_bBranchVisible);
				m_pButton->SetFlip(m_bBranchVisible);
			}
		}
	}
	break;
	}
}