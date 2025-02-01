#include "StdAfx.h"
#include "ItemRebuildZbsj.h"

#include "D3DFont.h"
#include "ItemImage.h"
#include "ItemMove.h"
#include "BasicTextButton.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "InnerInterface.h"
#include "GLItemMan.h"
#include "GLGaeaClient.h"

#include "BasicTextBox.h"
#include "DxFontMan.h"
#include "ModalWindow.h"
#include "ModalCallerID.h"

#include "BasicComboBox.h"
#include "BasicComboBoxRollOver.h"
#include "BasicTextBoxEx.h"
#include "BasicLineBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// ItemRebuild
//--------------------------------------------------------------------
CItemRebuildZbsj::CItemRebuildZbsj()	// ITEMREBUILD_MARK
	: m_pItemImage(NULL)
	, m_pCostTextBoxStatic(NULL)
	, m_pCostTextBox(NULL)
	, m_pMoneyTextBox(NULL)
	, m_pOkButton(NULL)
	, m_pZbsjRebOkButton1(NULL)
	, m_pZbsjRebOkButton2(NULL)
	, m_pZbsjRebOkButton3(NULL)
	, m_pZbsjRebOkButton4(NULL)
	, m_pZbsjRebOkButton5(NULL)
	, m_pZbsjRebOkButton6(NULL)
	, m_pZbsjRebTextBox1(NULL)
	, m_pZbsjRebTextBox2(NULL)
	, m_pZbsjRebTextBox3(NULL)
	, m_pZbsjRebTextBox4(NULL)
	, m_pZbsjRebTextBox5(NULL)
	, m_pZbsjRebTextBox6(NULL)
	, m_pZbsjRebTextBoxView1(NULL)
	, m_pZbsjRebTextBoxView2(NULL)
	, m_pZbsjRebTextBoxView3(NULL)
	, m_pZbsjRebTextBoxView4(NULL)
	, m_pZbsjRebTextBoxView5(NULL)
	, m_pZbsjRebTextBoxView6(NULL)
	, m_bSearch(FALSE)
	, m_bOK(false)
	, m_nType(0)
	, m_nZbsj(0)
{
}

CItemRebuildZbsj::~CItemRebuildZbsj()
{
}

VOID CItemRebuildZbsj::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII);

	CD3DFontPar* pFont6Shadow = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 7, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont8Shadow = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 10, _DEFAULT_FONT_SHADOW_FLAG);

	const int nAlignLeft = TEXT_ALIGN_LEFT;
	const int nAlignCenter = TEXT_ALIGN_CENTER_X;
	const int nAlignCenterBoth = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;

	const DWORD& dwSilver = NS_UITEXTCOLOR::SILVER;
	const DWORD& dwWhite = NS_UITEXTCOLOR::WHITE;
	const DWORD& dwSubTitle = NS_UITEXTCOLOR::PALEGOLDENROD;

	m_bg_white = new CUIControl;
	m_bg_white->CreateSub(this, "ZBSJ_BG_WHITE", UI_FLAG_DEFAULT);
	m_bg_white->SetVisibleSingle(TRUE);
	RegisterControl(m_bg_white);

	m_pBackLine1 = new CBasicLineBox;
	m_pBackLine1->CreateSub(this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	m_pBackLine1->CreateBaseBoxSkill("ZBSJ_STOP_LIST_BACK");
	RegisterControl(m_pBackLine1);


	{
		m_pItemImage = new CItemImage;
		m_pItemImage->CreateSub(this, "ITEM_ZBSJ_ITEM_SLOT", UI_FLAG_DEFAULT, ITEM_ZBSJ_ITEM_SLOT);
		m_pItemImage->CreateSubControl();
		RegisterControl(m_pItemImage);
	}

	{
		m_pZbsjRebTextBoxView1 = CreateStaticControl("ITEM_ZBSJ_REDVIEW_TEXT1", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBoxView2 = CreateStaticControl("ITEM_ZBSJ_REDVIEW_TEXT2", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBoxView3 = CreateStaticControl("ITEM_ZBSJ_REDVIEW_TEXT3", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBoxView4 = CreateStaticControl("ITEM_ZBSJ_REDVIEW_TEXT4", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBoxView5 = CreateStaticControl("ITEM_ZBSJ_REDVIEW_TEXT5", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBoxView6 = CreateStaticControl("ITEM_ZBSJ_REDVIEW_TEXT6", pFont, TEXT_ALIGN_LEFT);

		m_pZbsjRebTextBox1 = CreateStaticControl("ITEM_ZBSJ_RED_TEXT1", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBox2 = CreateStaticControl("ITEM_ZBSJ_RED_TEXT2", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBox3 = CreateStaticControl("ITEM_ZBSJ_RED_TEXT3", pFont, TEXT_ALIGN_LEFT);
		m_pZbsjRebTextBox4 = CreateStaticControl("ITEM_ZBSJ_RED_TEXT4", pFont, TEXT_ALIGN_LEFT);
	}

	{
		CBasicTextButton* pButton = new CBasicTextButton;
		pButton->CreateSub(this, "BASIC_TEXT_BUTTON14", UI_FLAG_DEFAULT, ITEM_ZBSJ_CANCEL_BUTTON);
		pButton->CreateBaseButton("ITEM_ZBSJ_CANCEL_BUTTON", CBasicTextButton::SIZE14,
			CBasicButton::CLICK_FLIP, ID2GAMEINTEXT("ITEM_ZBSJ_CANCEL"));
		pButton->SetShortcutKey(DIK_ESCAPE);
		RegisterControl(pButton);
	}

	{
		m_pOkButton = new CBasicTextButton;
		m_pOkButton->CreateSub(this, "BASIC_TEXT_BUTTON14", UI_FLAG_DEFAULT, ZBSJ_BUTTON1);
		m_pOkButton->CreateBaseButton("ITEM_ZBSJ_OK_BUTTON", CBasicTextButton::SIZE14,
			CBasicButton::CLICK_FLIP, ID2GAMEINTEXT("ITEM_ZBSJ_REDOK"));
		m_pOkButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
		m_pOkButton->SetVisibleSingle(FALSE);
		RegisterControl(m_pOkButton);
	}

	CBasicComboBox* pComboBox_A = new CBasicComboBox;
	pComboBox_A->CreateSub(this, "BASIC_COMBOBOX", UI_FLAG_XSIZE, ITEM_ZBSJ_OK_BUTTONOPEN);
	pComboBox_A->CreateBaseComboBox("ZBSJ_COMBO_OPEN");
	RegisterControl(pComboBox_A);
	m_pcbSkill_A = pComboBox_A;

	CBasicComboBoxRollOver* pComboBoxRollOver_A = new CBasicComboBoxRollOver;
	pComboBoxRollOver_A->CreateSub(this, "BASIC_COMBOBOX_ROLLOVER", UI_FLAG_XSIZE | UI_FLAG_YSIZE, ITEM_ZBSJ_OK_BUTTON);
	pComboBoxRollOver_A->CreateBaseComboBoxRollOver("ZBSJ_COMBO_ROLLOVER");
	pComboBoxRollOver_A->SetVisibleSingle(FALSE);
	RegisterControl(pComboBoxRollOver_A);
	m_pcbSkillRollOver_A = pComboBoxRollOver_A;
}

CBasicTextBox* CItemRebuildZbsj::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void APPEND_ITEM_GRADE(CString& strOrigin, BYTE uGRADE)
{
	if (!uGRADE) return;

	CString strGRADE;
	strGRADE.Format(" [+%d]", uGRADE);
	strOrigin += strGRADE;
}

VOID CItemRebuildZbsj::Update(INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl)
{
	if (m_RollOverID == NO_ID)
	{
		CUIWindow::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

		SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_ZBSJ_ITEM();
		if (m_pItemImage)
		{
			if (sItemCustom.sNativeID == NATIVEID_NULL())
			{
				m_pItemImage->ResetItem();

				m_pZbsjRebTextBoxView1->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView2->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView3->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView4->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView5->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView6->SetVisibleSingle(FALSE);

				m_pZbsjRebTextBox1->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBox2->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBox3->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBox4->SetVisibleSingle(FALSE);

				m_pcbSkill_A->SetVisibleSingle(FALSE);
				m_pOkButton->SetVisibleSingle(FALSE);

			}
			else
			{
				SITEM* pItem = GLItemMan::GetInstance().GetItem(sItemCustom.sNativeID);
				if (pItem)
					m_pItemImage->SetItem(pItem->sBasicOp.sICONID);
				else
					m_pItemImage->ResetItem();

				m_pZbsjRebTextBoxView1->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView2->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView3->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView4->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView5->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBoxView6->SetVisibleSingle(FALSE);

				m_pZbsjRebTextBox1->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBox2->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBox3->SetVisibleSingle(FALSE);
				m_pZbsjRebTextBox4->SetVisibleSingle(FALSE);

				m_pcbSkill_A->SetVisibleSingle(FALSE);
				m_pOkButton->SetVisibleSingle(FALSE);
			}
		}

		//物品屬性值
		if (sItemCustom.sNativeID != NATIVEID_NULL())
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem(sItemCustom.sNativeID);

			BYTE uAttackGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
			BYTE uDefenseGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);

			if (pItem)
			{
				if (m_pZbsjRebTextBoxView1)
				{
					CString strTemp;
					strTemp.Format(ID2GAMEINTEXT("ITEM_ZBSJ_DAMAGEPOINT"), uAttackGRADE, GLCONST_CHAR::dwZbsj[0].m_fValue, GLCONST_CHAR::dwZbsj[0].m_fPoint,
						GLCONST_CHAR::dwZbsj[0].m_dwMax);
					m_pZbsjRebTextBoxView1->SetOneLineText(strTemp);
					m_pZbsjRebTextBoxView1->SetVisibleSingle(TRUE);
				}

				if (m_pZbsjRebTextBoxView2)
				{
					CString strTemp;
					strTemp.Format(ID2GAMEINTEXT("ITEM_ZBSJ_DEFENSEPOINT"), uDefenseGRADE, GLCONST_CHAR::dwZbsj[1].m_fValue, GLCONST_CHAR::dwZbsj[1].m_fPoint,
						GLCONST_CHAR::dwZbsj[1].m_dwMax);

					m_pZbsjRebTextBoxView2->SetOneLineText(strTemp);
					m_pZbsjRebTextBoxView2->SetVisibleSingle(TRUE);
				}
				if (m_pZbsjRebTextBoxView3)
				{
					CString strDescText;

					INT nRandOptType = sItemCustom.GETOptTYPE1();

					if ((EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE))
					{
						strDescText.Format("存在屬性1 %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType));

						float fVal = sItemCustom.GETOptVALUE1();
						if (fVal != 0.0f)
						{
							if (fVal > 0.0f) strDescText += _T('+');

							strDescText.AppendFormat("%.2f", fVal);
							if (sItemCustom.IsPerRandOpt(nRandOptType)) strDescText += _T('%');

							m_pZbsjRebTextBox1->SetOneLineText(strDescText, NS_UITEXTCOLOR::AQUA);
							m_pZbsjRebTextBox1->SetVisibleSingle(TRUE);

							CString strTemp;
							strTemp.Format(ID2GAMEINTEXT("ITEM_ZBSJ_POINT1"), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType), GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue, GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint,
								GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax);
							m_pZbsjRebTextBoxView3->SetOneLineText(strTemp);
							m_pZbsjRebTextBoxView3->SetVisibleSingle(TRUE);
						}
					}
				}

				if (m_pZbsjRebTextBoxView4)
				{
					CString strDescText;

					INT nRandOptType = sItemCustom.GETOptTYPE2();
					if ((EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE))
					{
						strDescText.Format("存在屬性2 %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType));

						float fVal = sItemCustom.GETOptVALUE2();
						if (fVal != 0.0f)
						{
							if (fVal > 0.0f) strDescText += _T('+');

							strDescText.AppendFormat("%.2f", fVal);
							if (sItemCustom.IsPerRandOpt(nRandOptType)) strDescText += _T('%');

							m_pZbsjRebTextBox2->SetOneLineText(strDescText, NS_UITEXTCOLOR::AQUA);

							m_pZbsjRebTextBox2->SetVisibleSingle(TRUE);

							CString strTemp;
							strTemp.Format(ID2GAMEINTEXT("ITEM_ZBSJ_POINT2"), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType), GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue, GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint,
								GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax);
							m_pZbsjRebTextBoxView4->SetOneLineText(strTemp);
							m_pZbsjRebTextBoxView4->SetVisibleSingle(TRUE);
						}
					}
				}

				if (m_pZbsjRebTextBoxView5)
				{
					CString strDescText;

					INT nRandOptType = sItemCustom.GETOptTYPE3();
					if ((EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE))
					{
						strDescText.Format("存在屬性3 %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType));

						float fVal = sItemCustom.GETOptVALUE3();
						if (fVal != 0.0f)
						{
							if (fVal > 0.0f) strDescText += _T('+');

							strDescText.AppendFormat("%.2f", fVal);
							if (sItemCustom.IsPerRandOpt(nRandOptType)) strDescText += _T('%');

							m_pZbsjRebTextBox3->SetOneLineText(strDescText, NS_UITEXTCOLOR::AQUA);

							m_pZbsjRebTextBox3->SetVisibleSingle(TRUE);

							CString strTemp;
							strTemp.Format(ID2GAMEINTEXT("ITEM_ZBSJ_POINT3"), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType), GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue, GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint,
								GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax);
							m_pZbsjRebTextBoxView5->SetOneLineText(strTemp);
							m_pZbsjRebTextBoxView5->SetVisibleSingle(TRUE);
						}

					}

				}

				if (m_pZbsjRebTextBoxView6)
				{
					CString strDescText;

					INT nRandOptType = sItemCustom.GETOptTYPE4();
					if ((EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE))
					{
						strDescText.Format("存在屬性4 %s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType));

						float fVal = sItemCustom.GETOptVALUE4();
						if (fVal != 0.0f)
						{
							if (fVal > 0.0f) strDescText += _T('+');

							strDescText.AppendFormat("%.2f", fVal);
							if (sItemCustom.IsPerRandOpt(nRandOptType)) strDescText += _T('%');

							m_pZbsjRebTextBox4->SetOneLineText(strDescText, NS_UITEXTCOLOR::AQUA);

							m_pZbsjRebTextBox4->SetVisibleSingle(TRUE);

							CString strTemp;
							strTemp.Format(ID2GAMEINTEXT("ITEM_ZBSJ_POINT4"), ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType), GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptValue, GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_fOptPoint,
								GLCONST_CHAR::dwZbsjRandom[nRandOptType].m_dwOptMax);
							m_pZbsjRebTextBoxView6->SetOneLineText(strTemp);
							m_pZbsjRebTextBoxView6->SetVisibleSingle(TRUE);

						}
					}
				}
			}

			m_pcbSkill_A->SetVisibleSingle(TRUE);
			m_pOkButton->SetVisibleSingle(TRUE);
			LoadZbsj();
		}



	}
	else
	{
		ResetMessageEx();

		CUIControl::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

		CUIControl* pControl = m_ControlContainer.FindControl(m_RollOverID);
		if (!pControl)
		{
			GASSERT(0 && "褕陝 螃盟, 喻萄陛 割殮棲棻.");
			return;
		}

		pControl->Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

		DWORD dwControlMsg = pControl->GetMessageEx();
		NS_UIDEBUGSET::BlockBegin();
		if (dwControlMsg) TranslateUIMessage(m_RollOverID, dwControlMsg);
		NS_UIDEBUGSET::BlockEnd();

		//	蝶觼煤夥縑 婦啗脹 詭衛雖陛 嬴棲堅		
		if (!(dwControlMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL))
		{
			DWORD dwMsg = GetMessageEx();
			if (dwMsg & UIMSG_LB_UP)
			{
				if (!m_bFirstLBUP)
				{
					m_RollOverID = NO_ID;
					pControl->SetVisibleSingle(FALSE);
				}
				m_bFirstLBUP = FALSE;
			}
		}
	}
}

void CItemRebuildZbsj::LoadZbsj()
{
	CString strTemp;
	{
		m_pcbSkillRollOver_A->ClearText();

		ITEMZBSJ_MIX sItemZbsjMix;


		for (int i = 0; i < 6; i++)
		{
			m_pcbSkillRollOver_A->AddText((char*)ID2GAMEWORD("ITEM_ZBSJ_RED", i));
		}
	}
}

VOID CItemRebuildZbsj::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case ZBSJ_BUTTON1:
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{

			GLGaeaClient::GetInstance().GetCharacter()->ReqZbsjResult(m_nZbsj);

			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED); // Focus 僥薯煎 詭撮雖 餉薯
		}
		break;

	case ITEM_ZBSJ_ITEM_SLOT:
		if (CHECK_MOUSE_IN(dwMsg))
		{
			CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
			if (!pItemMove)
			{
				GASSERT(0 && "CItemRebuildZbsj::TranslateUIMessage, pItemMove == NULL");
				break;
			}

			SNATIVEID sItem = pItemMove->GetItem();
			if (sItem != NATIVEID_NULL())
			{
				const UIRECT& rcSlotPos = m_pItemImage->GetGlobalPos();
				pItemMove->SetGlobalPos(rcSlotPos);
				CInnerInterface::GetInstance().SetSnapItem();
			}

			if (dwMsg & UIMSG_LB_UP)
				GLGaeaClient::GetInstance().GetCharacter()->ReqZbsjMoveItem();

			SITEMCUSTOM sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_ZBSJ_ITEM();
			if (sItemCustom.sNativeID != NATIVEID_NULL())
			{
				CInnerInterface::GetInstance().SHOW_ITEM_INFO(sItemCustom, FALSE, FALSE, FALSE, 0, 0);
			}
		}
		break;

	case ITEM_ZBSJ_OK_BUTTONOPEN:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			if (dwMsg & UIMSG_LB_DOWN)
			{
				m_pcbSkillRollOver_A->SetVisibleSingle(TRUE);
				m_pcbSkillRollOver_A->SetScrollPercent(0.0f);

				m_RollOverID = ITEM_ZBSJ_OK_BUTTON;
				m_bFirstLBUP = TRUE;
			}
		}
	}
	break;

	case ITEM_ZBSJ_OK_BUTTON:
	{
		if (dwMsg & UI_MSG_COMBOBOX_ROLLOVER_SCROLL)
		{
			DWORD dwMsg = GetMessageEx();
			dwMsg &= ~UI_MSG_COMBOBOX_ROLLOVER_SCROLL;
			ResetMessageEx();
			AddMessageEx(dwMsg);
			break;
		}

		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			int nIndex = m_pcbSkillRollOver_A->GetSelectIndex();
			if (nIndex < 0) return;

			CString strTemp = m_pcbSkillRollOver_A->GetSelectText(nIndex);
			m_pcbSkill_A->SetText(strTemp);

			ZBSJ_PROCESS(nIndex);
		}
	}
	break;

	case ITEM_ZBSJ_STOP_LIST:
	{

	}
	break;

	case ITEM_ZBSJ_CANCEL_BUTTON:
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			CInnerInterface::GetInstance().CloseItemZbsjWindow();
		break;

	case ET_CONTROL_BUTTON:
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			CInnerInterface::GetInstance().CloseItemZbsjWindow();
		break;
	}

	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);
}

void CItemRebuildZbsj::ZBSJ_PROCESS(int nSelect)
{
	m_nZbsj = nSelect;
}