#include "stdafx.h"
#include  "GLSkillToolTip.h"
//#include "./Skill/SpecialEffect/GLSpecExperiencePoint.h"
//#include "./GLUseFeatures.h"
#include "../Lib_ClientUI/Interface/GameTextControl.h"
#include "../Lib_Engine/Common/StringFormat.h"
#include "GLCrowData.h"
#include "GLItemMan.h"
#include "GLCharDefine.h"
//#include "GLogicDataMan.h"
//#include "./Crow/GLCrowDataMan.h"

namespace GLSkillToolTip
{
    std::string MakeTooltipSting_In_Skill_Type( GLCHARLOGIC* pTarget, const GLSKILL* glSkill, WORD wLevel )
    {
        std::string strText = "";


        const SKILL::SAPPLY& sAPPLY = glSkill->m_sAPPLY;
        const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
        const SKILL::SSKILLBASIC& sBASIC = glSkill->m_sBASIC;

        SKILL::EMROLE emRole = glSkill->m_sBASIC.emROLE;

        SKILL_INFOR_TYPE emInforType = INFOR_NONE_TYPE;
        if ( emRole == SKILL::EMROLE_NORMAL )
        {
            if ( sDATA_LVL.fLIFE )
            {
                if ( sDATA_LVL.fBASIC_VAR > 0.0f )
                    emInforType = NORMAL_DURATION_PLUS;
                else if ( sDATA_LVL.fBASIC_VAR < 0.0f )
                    emInforType = NORMAL_DURATION_MINUS;
            }
            else
            {
                if ( sDATA_LVL.fBASIC_VAR > 0.0f )
                    emInforType = NORMAL_NONDURATION_PLUS;
                else if ( sDATA_LVL.fBASIC_VAR < 0.0f )
                    emInforType = NORMAL_NONDURATION_MINUS;
            }
        }
        else if ( emRole == SKILL::EMROLE_PASSIVE )
        {
            if ( sDATA_LVL.fLIFE )
            {
                if ( sDATA_LVL.fBASIC_VAR > 0.0f )
                    emInforType = PASSIVE_DURATION_PLUS;
                else if ( sDATA_LVL.fBASIC_VAR < 0.0f )
                    emInforType = PASSIVE_DURATION_MINUS;
            }
            else
            {
                if ( sDATA_LVL.fBASIC_VAR > 0.0f )
                    emInforType = PASSIVE_NONDURATION_PLUS;
                else if ( sDATA_LVL.fBASIC_VAR < 0.0f )
                    emInforType = PASSIVE_NONDURATION_MINUS;
            }
        }

        // Info : ���� Ÿ��
        {
            //if ( emInforType != INFOR_NONE_TYPE )
            //{
            //    strText = ID2GAMEWORD( "SKILL_BASIC_INFO", 13);
            //    m_pSkillLabel5->AddText( strText.c_str(), SKILL_INFOR_COLOR::dwAddAbility );
            //}

            float fBASIC_VAR_SCALE;

			BOOL bSKILL_PER;
			std::string strSKILL_TYPE;
			
			fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];
			bSKILL_PER = COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE);
			/*if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
			{
				strSKILL_TYPE = CGameTextMan::GetInstance().GetCommentText("SKILL_TYPES", sAPPLY.emBASIC_TYPE).GetString();
			}
			else*/
			{
				strSKILL_TYPE = COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE];;
			}
            switch ( sAPPLY.emBASIC_TYPE )
            {
            case SKILL::EMFOR_HP://	ü��ġ ��ȭ.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                GLDWDATA gdDamage = pTarget->m_gdDAMAGE_SKILL;

                                /*if ( sDATA_LVL.wUSE_CHARMNUM > 0 )
                                {
                                    EMSLOT emLHand = pTarget->GetCurLHand();
                                    SITEMCUSTOM pItemCustom = pTarget->GET_SLOT_ITEM(emLHand);
									SITEM* pItem = GLItemMan::GetInstance().GetItem(pItemCustom.sNativeID);
									//GLPADATA& sDATA = pItem->sSuitOp.gdDamage;
									const GLDWDATA& sDATA = pItem->sSuitOp.gdDamage;
									//const GLPADATA& sDATA = pItemCustom.GetBasicAttackDamage();
									gdDamage.dwNow += int(sDATA.dwLow);
									gdDamage.dwMax += int(sDATA.dwHigh);
                                }*/

								int nSkillDmg = ::abs( int( sDATA_LVL.fBASIC_VAR ) );

								const SKILL::EMAPPLY emApply(glSkill->m_sBASIC.emAPPLY);

								switch ( emApply )
								{
								case SKILL::EMAPPLY_PHY_SHORT: // ����;
									gdDamage.dwNow += (pTarget->m_sSUMITEM.gdDamage.dwNow)/* + pTarget->m_sumPowerAttribute[SKILL::EMAPPLY_MELEE]*/;
									gdDamage.dwMax += (pTarget->m_sSUMITEM.gdDamage.dwMax)/* + pTarget->m_sumPowerAttribute[SKILL::EMAPPLY_MELEE]*/;
									break;

								case SKILL::EMAPPLY_PHY_LONG: // ���;
									gdDamage.dwNow += int(pTarget->m_sSUMITEM.gdDamage.dwNow)/* + pTarget->m_sumPowerAttribute[SKILL::EMAPPLY_RANGE]*/;
									gdDamage.dwMax += int(pTarget->m_sSUMITEM.gdDamage.dwMax)/* + pTarget->m_sumPowerAttribute[SKILL::EMAPPLY_RANGE]*/;		
									break;

								case SKILL::EMAPPLY_MAGIC: // ����;
									gdDamage.dwNow/* += pTarget->m_sumPowerAttribute[SKILL::EMAPPLY_MAGIC]*/;
									gdDamage.dwMax/* += pTarget->m_sumPowerAttribute[SKILL::EMAPPLY_MAGIC]*/;
									break;
								};

                                WORD wGRADE = 0;
                                EMSLOT emRHand = pTarget->GetCurRHand();
                                const SITEMCUSTOM& sRHAND = pTarget->GET_SLOT_ITEM(emRHand);
                                if ( sRHAND.sNativeID != SNATIVEID(false) )
                                    wGRADE = sRHAND.GETGRADE(EMGRINDING_DAMAGE);
                                float fGrade = static_cast<float>(wGRADE) / GLCONST_CHAR::fDAMAGE_GRADE_K;

                                gdDamage.dwNow += static_cast< int >( nSkillDmg + static_cast<float>( gdDamage.dwNow ) * fGrade );
                                gdDamage.dwMax += static_cast< int >( nSkillDmg + static_cast<float>( gdDamage.dwMax ) * fGrade );

                                gdDamage.dwNow = static_cast< int >( static_cast<float>( gdDamage.dwNow ) * pTarget->m_fDamageRate );
                                gdDamage.dwMax = static_cast< int >( static_cast<float>( gdDamage.dwMax ) * pTarget->m_fDamageRate );

                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1_2")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5)
									, static_cast< float >( nSkillDmg )
                                    , static_cast< float >( gdDamage.dwNow )
									, static_cast< float >( gdDamage.dwMax ) );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_MP://	���ġ ��ȭ.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 6), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_SP://	�ٷ�ġ ��ȭ.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 7), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_VARHP://	ä��ġ ��ȭ��.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_VARMP://	���ġ ��ȭ��.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_VARSP://	�ٷ�ġ ��ȭ��.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_DEFENSE://	���ġ ��ȭ.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_CURE://	�ص�. ( �ص� ���� �ʿ� )
                {
                    strText = sc::string::format( "%s", ID2GAMEINTEXT("SKILL_INFOR_CURE") );
                }
                break;

            case SKILL::EMFOR_HITRATE://	������.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_AVOIDRATE://	ȸ����.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_VARAP://	��� (ü��,���,�ٷ�) ��ȭ��.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    , sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_VARDAMAGE://	����ġ ��ȭ��.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_VARDEFENSE://	���ġ ��ȭ��.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_PA://	����ġ.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_SA://	���ġ.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                };
                break;

            case SKILL::EMFOR_MA://	���ġ.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_HP_RATE://	HP ������.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_MP_RATE://	MP ������.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_SP_RATE://	SP ������.
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE6")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE6")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE1_1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE1")
                                    , ID2GAMEWORD("ITEM_ADDON_INFO", 5), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_RESIST: //	���� ��ȭ��
                {
                    switch ( emInforType )
                    {
                    case NORMAL_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case NORMAL_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_DURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                    ,sDATA_LVL.fLIFE, strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_PLUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            if ( bSKILL_PER == TRUE )
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                    , strSKILL_TYPE.c_str(), -fBASIC_VAR_SCALE );
                            }
                            else
                            {
                                strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                    , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                            }
                        }
                        break;
                    };
                }
                break;

            case SKILL::EMFOR_SUMMONTIME: //	��ȯ�ð� ����
                {
                    switch ( emInforType )
                    {
                    case PASSIVE_NONDURATION_PLUS:
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE5")
                                , strSKILL_TYPE.c_str(), sDATA_LVL.fBASIC_VAR );
                        }
                        break;

                    case PASSIVE_NONDURATION_MINUS:
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE5")
                                , strSKILL_TYPE.c_str(), -sDATA_LVL.fBASIC_VAR );
                        }
                        break;
                    };
                }
                break;
            };
        }
        return strText;
    }

	std::vector<std::string>MakeTooltipSting_In_Skill_Impact( const GLSKILL* glSkill, WORD wLevel )
    {
        std::string strText = "";
        std::vector<std::string> vecstrText;

        const SKILL::SAPPLY& sAPPLY = glSkill->m_sAPPLY;
        const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];

        // Info : �ΰ� ȿ��
        {
            SKILL::EMROLE emRole = glSkill->m_sBASIC.emROLE;

            if ( !sAPPLY.vecADDON.empty() )
            {
				for ( DWORD i = 0; i < sAPPLY.vecADDON.size(); ++i )
				{
					const float fADDON_VAR = sAPPLY.vecADDON[i].fADDON_VAR[wLevel];
					float fLife = sAPPLY.sDATA_LVL[ wLevel ].fLIFE;
					float fADDON_VAR_SCALE = fADDON_VAR * COMMENT::IMPACT_ADDON_SCALE[sAPPLY.vecADDON[i].emADDON];
					vecstrText.push_back(MakeTooltipSting_In_Skill_Impact( fLife, fADDON_VAR, fADDON_VAR_SCALE, emRole, sAPPLY.vecADDON[i].emADDON ));
				}
            }
        }
        return vecstrText;
    }

    std::string MakeTooltipSting_In_Skill_Impact(
        float fLife,
        const float fADDON_VAR, 
        float fADDON_VAR_SCALE, 
        SKILL::EMROLE emRole, 
        EMIMPACT_ADDON emADDON )
    {
        std::string strText = "";

        SKILL_INFOR_TYPE emInforType = INFOR_NONE_TYPE;
        if ( emRole == SKILL::EMROLE_NORMAL )
        {
            if ( fLife )
            {
                if ( fADDON_VAR > 0.0f )
                    emInforType = NORMAL_DURATION_PLUS;
                else if ( fADDON_VAR < 0.0f )
                    emInforType = NORMAL_DURATION_MINUS;
            }
            else
            {
                if ( fADDON_VAR > 0.0f )
                    emInforType = NORMAL_NONDURATION_PLUS;
                else if ( fADDON_VAR < 0.0f )
                    emInforType = NORMAL_NONDURATION_MINUS;
            }
        }
        else if ( emRole == SKILL::EMROLE_PASSIVE )
        {
            if (fLife )
            {
                if ( fADDON_VAR > 0.0f )
                    emInforType = PASSIVE_DURATION_PLUS;
                else if ( fADDON_VAR < 0.0f )
                    emInforType = PASSIVE_DURATION_MINUS;
            }
            else
            {
                if ( fADDON_VAR > 0.0f )
                    emInforType = PASSIVE_NONDURATION_PLUS;
                else if ( fADDON_VAR < 0.0f )
                    emInforType = PASSIVE_NONDURATION_MINUS;
            }
        }

        int nIndex = static_cast<int>(emADDON);
        int nINC = 1;
        std::string strInc= "";
        std::string strUItext = "";

        if ( fADDON_VAR > 0.0f )
        {
            nINC = 1;
			//if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
			//	strInc = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
            //else
				strInc = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_PLUS].c_str();
        }
        else if ( fADDON_VAR < 0.0f )
        {
            nINC = -1;
			//if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
			//	strInc = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
			//else
				strInc = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_MINUS].c_str();
        }

		BOOL bIsIMPACT_ADDON_PER = FALSE;
		std::string strIMPACT_ADDON;
		bIsIMPACT_ADDON_PER = COMMENT::IsIMPACT_ADDON_PER(emADDON);
		//if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
		//{
		//	strIMPACT_ADDON = CGameTextMan::GetInstance().GetCommentText("IMPACT_ADDON", emADDON).GetString();
		//}
		//else
		{
			strIMPACT_ADDON = COMMENT::IMPACT_ADDON[emADDON].c_str();
		}

        switch ( emADDON )
        {
        case EMIMPACTA_HP_RECOVERY_VAR:   //  HP ȸ���� ����
            {
                strUItext = sc::string::format( "SKILL_IMPACT_ADDON_%d", nIndex );
                strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, fADDON_VAR * nINC, strInc.c_str() );
            }
            break;
        case EMIMPACTA_MP_RECOVERY_VAR:  //  MP ȸ���� ����
            {
                strUItext = sc::string::format( "SKILL_IMPACT_ADDON_%d", nIndex );
                strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, fADDON_VAR * nINC, strInc.c_str()  );
            }
            break;
        case EMIMPACTA_SP_RECOVERY_VAR:  //  SP ȸ���� ����
            {
                strUItext = sc::string::format( "SKILL_IMPACT_ADDON_%d", nIndex );
                strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, fADDON_VAR * nINC, strInc.c_str()  );
            }
            break;
        case EMIMPACTA_CP_RECOVERY_VAR:  //  CP ȸ���� ����
            {
                strUItext = sc::string::format( "SKILL_IMPACT_ADDON_%d", nIndex );
                strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, fADDON_VAR * nINC, strInc.c_str()  );
            }
            break;
        case EMIMPACTA_CP_AUTO_VAR:  //  CP �ڵ����� ����
            {
                strUItext = sc::string::format( "SKILL_IMPACT_ADDON_%d", nIndex );
                strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, fADDON_VAR * nINC, strInc.c_str()  );
            }
            break;

        case EMIMPACTA_HITRATE:     // ������ ��ȭ��.
        case EMIMPACTA_AVOIDRATE:   // ȸ���� ��ȭ��.
        case EMIMPACTA_DAMAGE:  // ����ġ ��ȭ��.
        case EMIMPACTA_DEFENSE: // ���ġ ��ȭ��.
        case EMIMPACTA_VARHP:   // HP ��ȭ��.
        case EMIMPACTA_VARMP:   // MP ��ȭ��.
        case EMIMPACTA_VARSP:   // SP ��ȭ��.
        case EMIMPACTA_VARAP:   // HP,MP,SP ��ȭ��.
        case EMIMPACTA_DAMAGE_RATE:     // ����ġ ��ȭ��.
        case EMIMPACTA_DEFENSE_RATE:    // ���ġ ��ȭ��.
        case EMIMPACTA_PA:      //	����ġ.
        case EMIMPACTA_SA:      //	���ġ.
        case EMIMPACTA_MA:      //	���ġ.
        case EMIMPACTA_HP_RATE: //	HP ������.
        case EMIMPACTA_MP_RATE: //	MP ������.
        case EMIMPACTA_SP_RATE: //	SP ������.
        case EMIMPACTA_RESIST:  //	���� ��ġ ����
        case EMIMPACTA_CHANGESTATS:   //	���� ��ġ ���� ( ��,��ø,���� )
            {
                switch ( emInforType )
                {
                case NORMAL_DURATION_PLUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4_1")
                                ,fLife, strIMPACT_ADDON.c_str(), fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE4")
                                ,fLife, strIMPACT_ADDON.c_str(), fADDON_VAR);
                        }
                    }
                    break;

                case NORMAL_DURATION_MINUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4_1")
                                ,fLife, strIMPACT_ADDON.c_str(), -fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE4")
                                ,fLife, strIMPACT_ADDON.c_str(), -fADDON_VAR );
                        }
                    }
                    break;

                case NORMAL_NONDURATION_PLUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5_1")
                                , strIMPACT_ADDON.c_str(), fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_INCREASE_CASE5")
                                , strIMPACT_ADDON.c_str(), fADDON_VAR );
                        }
                    }
                    break;

                case NORMAL_NONDURATION_MINUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5_1")
                                , strIMPACT_ADDON.c_str(), -fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_NORMAL_DECREASE_CASE5")
                                , strIMPACT_ADDON.c_str(), -fADDON_VAR );
                        }
                    }
                    break;

                case PASSIVE_DURATION_PLUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2_1")
                                ,fLife, strIMPACT_ADDON.c_str(), fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE2")
                                ,fLife, strIMPACT_ADDON.c_str(), fADDON_VAR );
                        }
                    }
                    break;

                case PASSIVE_DURATION_MINUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2_1")
                                ,fLife, strIMPACT_ADDON.c_str(), -fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE2")
                                ,fLife, strIMPACT_ADDON.c_str(), -fADDON_VAR );
                        }
                    }
                    break;

                case PASSIVE_NONDURATION_PLUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3_1")
                                , strIMPACT_ADDON.c_str(), fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_INCREASE_CASE3")
                                , strIMPACT_ADDON.c_str(), fADDON_VAR );
                        }
                    }
                    break;

                case PASSIVE_NONDURATION_MINUS:
                    {
                        if ( bIsIMPACT_ADDON_PER )
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3_1")
                                , strIMPACT_ADDON.c_str(), -fADDON_VAR_SCALE );
                        }
                        else
                        {
                            strText = sc::string::format( ID2GAMEINTEXT("SKILL_INFOR_PASSIVE_DECREASE_CASE3")
                                , strIMPACT_ADDON.c_str(), -fADDON_VAR);
                        }
                    }
                    break;
                };
            }
            break;
        };
        return strText;
    }

    std::vector<std::string> MakeTooltipSting_In_Skill_Special( const GLSKILL* glSkill, WORD wLevel, bool bIsTool )
    {
        const SKILL::SAPPLY& sAPPLY = glSkill->m_sAPPLY;
        const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
        EMIMPACT_TAR emIMPACT_TAR = glSkill->m_sBASIC.emIMPACT_TAR;


        std::vector<std::string> vecstrText;

        // Info : Ư�� ȿ��
        {
            if ( !sAPPLY.vecSPEC.empty() )
            {
                for ( DWORD i = 0; i <  sAPPLY.vecSPEC.size(); ++i )
                {
                    std::string strText = "";

                    const SKILL::SSPEC& sSPEC = sAPPLY.vecSPEC[i].sSPEC[wLevel];
					float fVAR1_SCALE, fVAR2_SCALE;
					fVAR1_SCALE = sSPEC.fVAR1 * COMMENT::SPEC_ADDON_VAR1_SCALE[sAPPLY.vecSPEC[i].emSPEC];
					fVAR2_SCALE = sSPEC.fVAR2 * COMMENT::SPEC_ADDON_VAR2_SCALE[sAPPLY.vecSPEC[i].emSPEC];
                    float fLife = sAPPLY.sDATA_LVL[ wLevel ].fLIFE;
					//const SKILL::SSPEC_CA& sSPECCA = sAPPLY.sCAData.sSPEC_CA[wLevel];

                    vecstrText.push_back( MakeTooltipSting_In_Skill_Special(fLife, fVAR1_SCALE, fVAR2_SCALE, sAPPLY.vecSPEC[i].emSPEC, sSPEC, emIMPACT_TAR, bIsTool ) );

                }
            }
        }
        return vecstrText;
    }
    std::string MakeTooltipSting_In_Skill_Special( 
        float fLife,
        float fVAR1_SCALE,
        float fVAR2_SCALE,
        EMSPEC_ADDON emSPEC,
        const SKILL::SSPEC& sSPEC, EMIMPACT_TAR emIMPACT_TAR,
		bool bIsTool/*,
		const SKILL::SSPEC_CA& sSPECCA*/ )
    {
		std::string strText = "";
		std::string strUItext = "SKILL_SPEC_ADDON_";
            std::string strTemp = "";
            CString straddText1;
            CString straddText2;
            std::string str = "";
            int nIndex = static_cast<int>( emSPEC );

            switch ( emSPEC )
            {
            case EMSPECA_PUSHPULL://	����/�б�.
                {
					if ( fVAR2_SCALE == 0 )
						break;

                    if ( fVAR1_SCALE > 0.0f )
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    else
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_%d", nIndex, 1 );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fVAR2_SCALE, fVAR1_SCALE );
                }
                break;

            case EMSPECA_REFDAMAGE://	����� �ݻ�.
                {
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }
                }
                break;

            case EMSPECA_REBIRTH://	��Ȱ.
                {
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fVAR1_SCALE );
                }
                break;

            case EMSPECA_HP_GATHER://	HP�� �������� ������.
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fVAR1_SCALE );
                }
                break;

            case EMSPECA_MP_GATHER://	MP�� �������� ������.
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fVAR1_SCALE );
                }
                break;

            case EMSPECA_SP_GATHER://	SP�� �������� ������.
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fVAR1_SCALE );
                }
                break;

            case EMSPECA_HP_DIV://	HP�� �Ϻθ� ������ ��.
                {
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                }
                break;

            case EMSPECA_MP_DIV://	MP�� �Ϻθ� ������ ��.
                {
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                }
                break;

            case EMSPECA_SP_DIV://	SP�� �Ϻθ� ������ ��.
                {
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                }
                break;

            case EMSPECA_NONBLOW://	�����̻� ����.
                {
					/*if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
					{
						straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_NUMB).GetString() );

						if ( sSPEC.dwFLAG & DIS_STUN )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_STUN).GetString() );

						if ( sSPEC.dwFLAG & DIS_STONE )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_STONE).GetString() );

						if ( sSPEC.dwFLAG & DIS_BURN )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_BURN).GetString() );

						if ( sSPEC.dwFLAG & DIS_FROZEN )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_FROZEN).GetString() );

						if ( sSPEC.dwFLAG & DIS_MAD )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_MAD).GetString() );

						if ( sSPEC.dwFLAG & DIS_POISON )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_POISON).GetString() );

						if ( sSPEC.dwFLAG & DIS_CURSE )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_CURSE).GetString() );
					}
					else*/
					{
						straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_NUMB].c_str() );

						if ( sSPEC.dwFLAG & DIS_STUN )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_STUN].c_str() );

						if ( sSPEC.dwFLAG & DIS_STONE )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_STONE].c_str() );

						if ( sSPEC.dwFLAG & DIS_BURN )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_BURN].c_str() );

						if ( sSPEC.dwFLAG & DIS_FROZEN )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_FROZEN].c_str() );

						if ( sSPEC.dwFLAG & DIS_MAD )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_MAD].c_str() );

						if ( sSPEC.dwFLAG & DIS_POISON )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_POISON].c_str() );

						if ( sSPEC.dwFLAG & DIS_CURSE )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_CURSE].c_str() );
					}
                    //' ,' ������ �ܾ� �޸��� �������� �ڵ�
                    straddText1 = straddText1.Left(straddText1.GetLength()-2);
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    //strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), strTemp.c_str() );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), straddText1.GetString() );
                }
                break;

            case EMSPECA_RECBLOW://	�����̻� ȸ��.
                {
					/*if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
					{
						if ( sSPEC.dwFLAG & DIS_NUMB )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_NUMB).GetString() );

						if ( sSPEC.dwFLAG & DIS_STUN )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_STUN).GetString() );

						if ( sSPEC.dwFLAG & DIS_STONE )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_STONE).GetString() );

						if ( sSPEC.dwFLAG & DIS_BURN )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_BURN).GetString() );

						if ( sSPEC.dwFLAG & DIS_FROZEN )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_FROZEN).GetString() );

						if ( sSPEC.dwFLAG & DIS_MAD )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_MAD).GetString() );

						if ( sSPEC.dwFLAG & DIS_POISON )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_POISON).GetString() );

						if ( sSPEC.dwFLAG & DIS_CURSE )
							straddText1.AppendFormat( "%s, ", CGameTextMan::GetInstance().GetCommentText("BLOW",EMBLOW_CURSE).GetString() );
					}
					else*/
					{
						if ( sSPEC.dwFLAG & DIS_NUMB )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_NUMB].c_str() );

						if ( sSPEC.dwFLAG & DIS_STUN )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_STUN].c_str() );

						if ( sSPEC.dwFLAG & DIS_STONE )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_STONE].c_str() );

						if ( sSPEC.dwFLAG & DIS_BURN )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_BURN].c_str() );

						if ( sSPEC.dwFLAG & DIS_FROZEN )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_FROZEN].c_str() );

						if ( sSPEC.dwFLAG & DIS_MAD )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_MAD].c_str() );

						if ( sSPEC.dwFLAG & DIS_POISON )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_POISON].c_str() );

						if ( sSPEC.dwFLAG & DIS_CURSE )
							straddText1.AppendFormat( "%s, ", COMMENT::BLOW[EMBLOW_CURSE].c_str() );
					}
                    //' ,' ������ �ܾ� �޸��� �������� �ڵ�
                    straddText1 = straddText1.Left(straddText1.GetLength()-2);

                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    //strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), strTemp.c_str() );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), straddText1.GetString() );
                }
                break;

            case EMSPECA_PIERCE://	��������.
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR1_SCALE );  
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_TARRANGE://	���� ����� �����Ÿ� �� ���뿵�� ����( ���� )
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( fVAR1_SCALE > 0.0f )
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fVAR1_SCALE );
                        }
                    }
                    else
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, -fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                -fVAR1_SCALE );
                        }
                    }
                }
                break;

            case EMSPECA_MOVEVELO://	�̵��ӵ�.
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( fVAR1_SCALE > 0.0f )
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fVAR1_SCALE );
                        }
                    }
                    else
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, -fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                -fVAR1_SCALE );
                        }
                    }
                }
                break;

            case EMSPECA_ONWARD://	����.
                {
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fVAR1_SCALE, fVAR2_SCALE );
                }
                break;

            case EMSPECA_INVISIBLE://	����
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, fVAR1_SCALE );

                }
                break;

            case EMSPECA_RECVISIBLE://	���� ����
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, fVAR1_SCALE );
                }
                break;

            case EMSPECA_ATTACKVELO://	���ݼӵ�.
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if (fVAR1_SCALE < 0.0f )
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, -fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                -fVAR1_SCALE );
                        }
                    }
                    else
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fVAR1_SCALE );
                        }
                    }
                }
                break;

            /*case EMSPECA_SKILLDELAY://	��ų ������ �ð� ��ȭ.
                {
					if ( fVAR2_SCALE == 0 )
						break;
                    std::string strTemptext1 = " ";
                    int nInc1 = 1;


                    if ( fVAR1_SCALE < 0 )
                    {
                        nInc1 = -1 ;

						//if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
						//	strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_MINUS).GetString();
						//else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_MINUS].c_str();
                        
                    }
                    else
                    {
                        nInc1 = 1 ;

						//if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
						//	strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
						//else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_PLUS].c_str();
                    }

                    if (sSPEC.dwFLAG == (EMSPEC_SKILL_ASING_DWORD_BEFORE|
                        EMSPEC_SKILL_ASING_DWORD_AFTER) )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR2_SCALE, fLife, fVAR1_SCALE * nInc1, strTemptext1  );
                    }

                    if (sSPEC.dwFLAG == EMSPEC_SKILL_ASING_DWORD_BEFORE )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR2_SCALE, fVAR1_SCALE * nInc1, strTemptext1 );
                    }
                      if (sSPEC.dwFLAG == EMSPEC_SKILL_ASING_DWORD_AFTER )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR2_SCALE, fLife, fVAR1_SCALE * nInc1, strTemptext1);
                    }
                }
                break;*/

            case EMSPECA_CRUSHING_BLOW://  ����Ÿ�� ��ų
                {
					if ( fVAR2_SCALE == 0 )
						break;

					if( fVAR1_SCALE == 0 )
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
						strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fVAR2_SCALE );
					}
					else
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
						strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
							fVAR2_SCALE, GLCONST_CHAR::fPUSHPULL_VELO, fVAR1_SCALE );
					}
                }
                break;

            case EMSPECA_PSY_DAMAGE_REDUCE:// ���� ������ �����
                {
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR1_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_MAGIC_DAMAGE_REDUCE:// ���� ������ �����
                {
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR1_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_PSY_DAMAGE_REFLECTION:// ���� ������ �ݻ���
                {
					if ( fVAR2_SCALE == 0 )
						break;
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR2_SCALE, fVAR1_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR2_SCALE, fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_MAGIC_DAMAGE_REFLECTION:// ���� ������ �ݻ���
                {
					if ( fVAR2_SCALE == 0 )
						break;
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR2_SCALE, fVAR1_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR2_SCALE, fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_BUFF_REMOVE://	ȿ�� ���� ( �Ϲ�,����,�����,����Ʈ �������� )
                {
                    if ( sSPEC.dwFLAG == SKILL::EMACTION_NORMAL )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_0", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }
                    else if ( sSPEC.dwFLAG == SKILL::EMACTION_BUFF )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }
                    else if ( sSPEC.dwFLAG == SKILL::EMACTION_DEBUFF )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }
                    else if ( sSPEC.dwFLAG == SKILL::EMACTION_LIMIT )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }
                }
                break;

            case EMSPECA_STUN://	���� ȿ��
                {
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife );
                    }
                    /*else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }*/
                }
                break;

            case EMSPECA_PULL://	��� ����
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fVAR1_SCALE, fVAR2_SCALE );
                }
                break;

            case EMSPECA_PUSH://	�о� ����
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fVAR1_SCALE, GLCONST_CHAR::fPUSHPULL_VELO, fVAR2_SCALE );
                }
                break;

            case EMSPECA_CONTINUOUS_DAMAGE://	���ӵ�����
                {
					if ( fLife == 0 )
						break;
                    if ( fVAR2_SCALE > 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR1_SCALE, fVAR2_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR1_SCALE, fVAR2_SCALE * -1 );
                    }
                }
                break;

            case EMSPECA_CURSE://	����
                {
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR1_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_CHANGE_ATTACK_RANGE://	��ų �����Ÿ� ��ȭ
                {
					if ( fVAR1_SCALE == 0 )
						break;
					std::string strTemptext1 = " ";
					int nInc1 = 1;


					if ( fVAR1_SCALE < 0 )
					{
						nInc1 = -1 ;

						//if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
						//	strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_MINUS).GetString();
						//else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_MINUS].c_str();
					}
					else
					{
						nInc1 = 1 ;

						//if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
						//	strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
						//else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_PLUS].c_str();
					}

                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife, fVAR1_SCALE * nInc1, strTemptext1 );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE* nInc1, strTemptext1 );
                    }
                }
                break;

            case EMSPECA_CHANGE_APPLY_RANGE://	��ų ���뿵�� ��ȭ
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( fVAR1_SCALE > 0.0f )
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fVAR1_SCALE );
                        }
                    }
                    else
                    {
                        if ( fLife != 0.0f )
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                fLife, -fVAR1_SCALE );
                        }
                        else
                        {
                            strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", nIndex );
                            strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                                -fVAR1_SCALE );
                        }
                    }
                }
                break;

            case EMSPECA_PROHIBIT_POTION://	���� ��� ���� ( ��ǰ�� ��ü ���� )
                {
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }
                }
                break;

            case EMSPECA_PROHIBIT_SKILL://	��ų ��� ���� ( ��ǰ�� ��ü ���� )
                {
                    if ( fLife != 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( "%s", ID2GAMEINTEXT( strUItext.c_str() ) );
                    }
                }
                break;

            case EMSPECA_IGNORE_DAMAGE://	������ ���(����)
                {
					if ( DWORD(fVAR1_SCALE) != 0 && DWORD(fVAR2_SCALE) != 0 )
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
						strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
							fLife, fVAR1_SCALE, fVAR2_SCALE);
					}
					else if (DWORD(fVAR2_SCALE) != 0 )
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
						strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
							fLife, fVAR2_SCALE);
					}
					else if (DWORD(fVAR1_SCALE) != 0 )
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
						strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
							fLife, fVAR1_SCALE);
					}                    
                }
                break;

			/*case EMSPECA_EXP_RATE:	//  ����ġ ȹ���
				{
					if ( fVAR1_SCALE == 0 )
						break;
					if ( fVAR1_SCALE > 0.0f )
					{
						if ( fLife != 0.0f )
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
								fLife, fVAR1_SCALE );
						}
						else
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
								fVAR1_SCALE );
						}
					}
					else
					{
						if ( fLife != 0.0f )
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
								fLife, -fVAR1_SCALE );
						}
						else
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", nIndex );
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
								-fVAR1_SCALE );
						}
					}
				}
				break;

                //����
            case EMSPECA_TELEPORT: //�����̵�
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( emIMPACT_TAR == TAR_SELF || emIMPACT_TAR == TAR_SPEC || emIMPACT_TAR == TAR_SELF_TOSPEC )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), 1 );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), 1 );
                    }
                }
                break;

            case EMSPECA_SWAPPOS: // ��ġ����
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( sSPEC.dwFLAG == EMSPEC_SWAPPOS_DIR_BOTH)
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fVAR1_SCALE );
                    }
                    else if ( sSPEC.dwFLAG == EMSPEC_SWAPPOS_DIR_SELF)
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fVAR1_SCALE );
                    }
                    else if ( sSPEC.dwFLAG == EMSPEC_SWAPPOS_DIR_TARGET)
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_INCREASE_EFF: // ��ȭ ȿ��
                {
					if ( ( fVAR1_SCALE == 0 ) && ( fVAR2_SCALE == 0 ) )
						break;
                    for ( int i = 0; i < SKILL::EMAPPLY_NSIZE; i++ )
                    {
                        if ( sSPEC.dwFLAG & ( 1 <<  i  ) )
                        {
							if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
								straddText1.AppendFormat("%s, ", CGameTextMan::GetInstance().GetCommentText("SKILL_APPLY", i).GetString() );
                            else
								straddText1.AppendFormat( "%s, ", COMMENT::SKILL_APPLY[i].c_str());
                        }
                    }

                    std::string strTemptext1 = " ";
                    std::string strTemptext2 = " ";
                    int nInc1 = 1;
                    int nInc2 = 1;

                    if ( fVAR1_SCALE > 0 )
					{
						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
						else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_PLUS].c_str();
					}
                    else
                    {
                        nInc1 = -1;

						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_MINUS).GetString();
						else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_MINUS].c_str();

                    }

                    if ( fVAR2_SCALE > 0 )
					{
						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext2 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
						else
							strTemptext2 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_PLUS].c_str();
					}
                    else
                    {
                        nInc2 = -1;
						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext2 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_MINUS).GetString();
						else
							strTemptext2 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_MINUS].c_str();
                    }
                    
                    straddText1 = straddText1.Left(straddText1.GetLength()-2);
                
					if ( fVAR2_SCALE == 0 )
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
						fLife, straddText1, fVAR1_SCALE*nInc1,strTemptext1 );
					}
					else if ( fVAR1_SCALE == 0 )
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
						fLife, straddText1, fVAR2_SCALE * nInc2, strTemptext2 );
					}
					else
					{
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fLife, straddText1, fVAR1_SCALE*nInc1,strTemptext1, fVAR2_SCALE * nInc2, strTemptext2 );
					}
                }
                break;

            case EMSPECA_ILLUSION: // ���ȿ��
                {
					if ( fVAR2_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                        fLife );
                }
                break;

            case EMSPECA_VEHICLE_GETOFF: // ���� ȿ��
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( fLife > 0 && fVAR2_SCALE == 1 )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE, fLife );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE );
                    }
                }
                break;

            case EMSPECA_ENDURE_VARY: // ���ӽð� ����
                {
					if ( (fVAR1_SCALE == 0) && (fVAR2_SCALE == 0) )
						break;
                    for ( int i = 0; i < SKILL::EMACTION_TYPE_NSIZE; i++ )
                    {
                        if ( sSPEC.dwFLAG & ( 1 <<  i  ) )
                        {
							if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
								straddText1.AppendFormat("%s, ", CGameTextMan::GetInstance().GetCommentText("SKILL_ACTION_TYPE", i).GetString() );
							else
								straddText1.AppendFormat( "%s, ", COMMENT::SKILL_ACTION_TYPE[ i ].c_str());
                        }
                    }


                    std::string strTemptext1 = " ";
                    std::string strTemptext2 = " ";
                    int nInc1 = 1;
                    int nInc2 = 1;

                    if ( fVAR1_SCALE > 0 )
					{
						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
						else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_PLUS].c_str();
					}
                    else
                    {
                        nInc1 = -1;
						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext1 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_MINUS).GetString();
						else
							strTemptext1 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_MINUS].c_str();
                    }

                    if ( fVAR2_SCALE > 0 )
					{
						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext2 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_PLUS).GetString();
						else
							strTemptext2 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_PLUS].c_str();
					}
                    else
                    {
                        nInc2 = -1;
						if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
							strTemptext2 = CGameTextMan::GetInstance().GetCommentText("SPEC_INC_TYPE", EMSPEC_INC_TYPE_MINUS).GetString();
						else
							strTemptext2 = COMMENT::SPEC_INC_TYPE[EMSPEC_INC_TYPE_MINUS].c_str();
                    }

                    straddText1 = straddText1.Left(straddText1.GetLength()-2);
                    straddText2 = straddText2.Left(straddText2.GetLength()-2);

                    if (  fVAR2_SCALE == 0  )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            straddText1, fVAR1_SCALE * nInc1,  strTemptext1 );
                    }
					else if ( fVAR1_SCALE == 0 )
					{
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
						strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
							fLife, straddText1, fVAR2_SCALE * nInc2,  strTemptext2 );
					}

                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            straddText1, fVAR1_SCALE * nInc1, strTemptext1, fLife, fVAR2_SCALE * nInc2, strTemptext2 );
                    }

                }
                break;

            case EMSPECA_STIGMA: // ����ȿ��
                {
					if ( fVAR2_SCALE == 0 )
						break;
                    for ( int i = 0; i <EMSPEC_STIGMA_TYPE_DWORD_DAMAGE_SKILL; i++ )
                    {
                        if ( sSPEC.dwFLAG  & ( 1 <<  i   ) )
                        {
							if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
								straddText1.AppendFormat("%s, ", CGameTextMan::GetInstance().GetCommentText("SPEC_STIGMA_TYPE", i).GetString() );
							else
								straddText1.AppendFormat( "%s, ", COMMENT::SPEC_STIGMA_TYPE[ i ].c_str());
                        }
                    }
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, straddText1.GetString(), fVAR2_SCALE );
                }
                break;

            case EMSPECA_HALLUCINATE: // ȯ��ȿ��
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( sSPEC.dwFLAG == 1  )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife );
                    }
                }
                break;

            case EMSPECA_RELEASE_ENDURE: // �ǰݽ� ȿ�� ����
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fVAR1_SCALE );
                }
                break;
            case EMSPECA_LINK_HP: // �� ȿ�� ( HP ��ũ )
                {
                    if ( fLife > 0 && fVAR2_SCALE == 1 )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fLife );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fLife  );
                    }
                }
                break;

            case EMSPECA_RELEASE_PET: //  ����ȿ��( �� ���� )
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if (  fLife == 0 )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),
                            fVAR1_SCALE, fLife );
                    }
                }
                break;

            case EMSPECA_IMMUNE: // �鿪 ȿ��
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    for ( int i = 0; i < SKILL::EMAPPLY_NSIZE; i++ )
                    {
                        if ( static_cast<DWORD>(fVAR1_SCALE) & ( 1 <<  i   ) )
                        {
							if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
								straddText1.AppendFormat("%s, ", CGameTextMan::GetInstance().GetCommentText("SKILL_APPLY", i).GetString() );
							else
								straddText1.AppendFormat( "%s, ", COMMENT::SKILL_APPLY[ i ].c_str());
                        }
                    }

                    for ( int i = 0; i < SKILL::EMACTION_TYPE_NSIZE; i++ )
                    {
                        if ( static_cast<DWORD>(fVAR2_SCALE) & ( 1 <<  i  ) )
                        {
							if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
								straddText1.AppendFormat("%s, ", CGameTextMan::GetInstance().GetCommentText("SKILL_ACTION_TYPE", i).GetString() );
							else
								straddText2.AppendFormat( "%s, ", COMMENT::SKILL_ACTION_TYPE[ i ].c_str());
                        }
                        
                    }
                    straddText1 = straddText1.Left(straddText1.GetLength()-2);
                    straddText2 = straddText2.Left(straddText2.GetLength()-2);


                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), 
                                                                                    fLife, 
                                                                                    straddText1.GetString(), 
                                                                                    straddText2.GetString() );
                }
                break;

            case EMSPECA_TURN_STATE_BATTLE: // �������� ȿ�� ( ���� ���·� �ٲ� )
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fVAR1_SCALE, fVAR2_SCALE );
                }
                break;

            case EMSPECA_TURN_STATE_PEACE: // �������� ���� ( ��ȭ ���·� �ٲ� )
                {
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),1);
                }
                break;
            case EMSPECA_TAUNT: // ���� ȿ��
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                    strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fVAR1_SCALE, fLife, fVAR2_SCALE );
                }
                break;

            case EMSPECA_DOMINATION: // ���� ȿ��
                {
					if ( fVAR1_SCALE == 0 )
						break;
                    if ( sSPEC.dwFLAG == 1  )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fVAR1_SCALE, fLife, fVAR2_SCALE );
                    }
                    else
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fVAR1_SCALE, fLife, fVAR2_SCALE );
                    }
                }
                break;
			case EMSPECA_RANDOM_EXP_RATE:				
				{
					SKILL::ExperiencePointSettings* const pExpSettings(SKILL::ExperiencePointSettings::getInstance());					
					const float fIncreaseRate(pExpSettings->getRate(DWORD(fVAR1_SCALE)));
					const DWORD nAccum = DWORD(fVAR2_SCALE);
					if ( fIncreaseRate == 0 || nAccum == 0 )
						break;
					
					if ( nAccum == 1 )					
					{						
						if ( fIncreaseRate > 0.0f )
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", EMSPECA_EXP_RATE);
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fIncreaseRate );
						}
						else
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", EMSPECA_EXP_RATE);
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fIncreaseRate );
						}
					}
					else
					{
						if ( fIncreaseRate > 0.0f )
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", EMSPECA_RANDOM_EXP_RATE);
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fIncreaseRate, nAccum );
						}
						else
						{
							strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_3", EMSPECA_RANDOM_EXP_RATE);
							strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ),fIncreaseRate, nAccum );
						}
					}
				}
				break;
			case EMSPECA_SKILL_LINK:
				{
					if ( bIsTool == false )
						break;
					if ( fVAR1_SCALE == 0 )
						break;
					strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fVAR1_SCALE*100 );
				}
				break;
			case EMSPECA_DAMAGE_VALUE:
			case EMSPECA_DEFENSE_VALUE:
			case EMSPECA_HIT_VALUE:
			case EMSPECA_AVOID_VALUE:
			case EMSPECA_HP_VALUE:
			case EMSPECA_MP_VALUE:
			case EMSPECA_SP_VALUE:
			case EMSPECA_CP_VALUE:
			case EMSPECA_ATTACK_RANGE_VALUE:
			case EMSPECA_DIS_SP_VALUE:
			case EMSPECA_RESIST_VALUE:
			case EMSPECA_STATS_POW_VALUE:
			case EMSPECA_STATS_STR_VALUE:
			case EMSPECA_STATS_SPI_VALUE:
			case EMSPECA_STATS_DEX_VALUE:
			case EMSPECA_STATS_STA_VALUE:
			case EMSPECA_PA_VALUE:
			case EMSPECA_SA_VALUE:
			case EMSPECA_MA_VALUE:
			case EMSPECA_HP_POTION_INC_VALUE:
			case EMSPECA_MP_POTION_INC_VALUE:
			case EMSPECA_SP_POTION_INC_VALUE:
			case EMSPECA_CP_INC_VALUE:
			case EMSPECA_MOVEVELO_VALUE:
				{
					if ( fVAR1_SCALE < 0.0f )
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
					else
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );

					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), int(fVAR1_SCALE) );
				}
				break;
			case EMSPECA_DAMAGE_RATE:
			case EMSPECA_DEFENSE_RATE:
			case EMSPECA_HIT_RATE:
			case EMSPECA_AVOID_RATE:
			case EMSPECA_HP_VARIATION_RATE:
			case EMSPECA_MP_VARIATION_RATE:
			case EMSPECA_SP_VARIATION_RATE:
			case EMSPECA_HMS_VARIATION_RATE:
			case EMSPECA_DAMAGE_RATE_ALL:
				{
					if ( fVAR1_SCALE < 0.0f )
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
					else
						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );

					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fVAR1_SCALE );
				}
				break;

			case EMSPECA_VIEW_RANGE_RATE: // �þ� ����;
				{
					if ( int(fVAR1_SCALE) >= 1 )
						break;
					else if ( fVAR1_SCALE < 0.0f )
						break;
					const int _maxViewRange(static_cast<int>((fVAR1_SCALE * 100.0f)));
					strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), _maxViewRange );
				}
				break;

			case EMSPECA_AIRBORNE:				// ���;
				{
					if ( fLife <= 0.0f )
						break;
					strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife );
				}

				break;
			case EMSPECA_SUMMON_CHANGE_SKILLPAGE:		// ��ȯ�� ��ų ������ ����;
				//strText = "��ȯ�� ��ų ������";
				break;
			case EMSPECA_SUMMON_EVENT_SKILL:			// ��ȯ�� �̺�Ʈ ��ų;
				//strText =  "��ȯ�� �̺�Ʈ ��ų";
				break;
			case EMSPECA_SUMMON_TARGETING:				// ��ȯ�� Ÿ�� ����;
				//strText =  "��ȯ�� Ÿ�� ����";
				break;
			case EMSPECA_SUMMON_RUNAWAY:				// ��ȯ�� ����;
				//strText =  "��ȯ�� ����";
				break;
			case EMSPECA_DELAY_ACTION_SKILL:			// �����ߵ�
				{
					if ( bIsTool == false )
						break;

					if ( fLife <= 0.0f )
						break;
					strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
					GLSKILL* pGlSkill = GLogicData::GetInstance().GetSkillData( 
						SNATIVEID( static_cast<WORD>(sSPEC.fVAR1), static_cast<WORD>(sSPEC.fVAR2 )) );
					std::string strSkillname ="";
					if ( pGlSkill )
						strSkillname = pGlSkill->GetNameStr();
					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, strSkillname.c_str() );
				}
				break;
			case EMSPECA_CLONE:							// ����ȿ��
				{
					if ( fLife <= 0.0f )
						break;
					strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
					strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), fLife, sSPEC.fVAR1  );
				}
				break;
            case EMSPECA_MOVEVELO_LIMIT:
                {
					//  [3/20/2015 gbgim] - RM# 1028;
					// ���� 0%~100%���� ����ϴ��� -100%~0%�� �ٲ�޶�� ��û�̿�;
					// ���ϴ� �ǵ��� ���ڿ� �ƶ��� ���� �����ʴٰ� ��������� ����������;
                    if ( sSPEC.fVAR1 >= 0.0f )
                    {            
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_1", nIndex );
                        std::string strTemp = sc::string::format(ID2GAMEINTEXT(strUItext.c_str()), fVAR1_SCALE-100.0f);
                        straddText1.AppendFormat( "%s ", strTemp.c_str() );
                    }
                    if ( sSPEC.fVAR2 >= 0.0f )
                    {
                        strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d_2", nIndex );
                        std::string strTemp = sc::string::format(ID2GAMEINTEXT(strUItext.c_str()), fVAR2_SCALE-100.0f);
                        straddText1.AppendFormat( "%s ", strTemp.c_str() );
                    }
                    strText = straddText1.GetString();
                }
                break;

			case EMSPECA_REMOVE_SKILL_EFFECT:
				{
					DWORD dwValue = (DWORD)sSPEC.nVAR1;

					if ( dwValue != 0 )
					{
						strTemp += sc::string::format( "%1%", ID2GAMEWORD( "ITEM_INFO_SET_BONUS", dwValue ) );	

						strUItext = sc::string::format( "SKILL_SPEC_ADDON_%d", nIndex );
						strText = sc::string::format( ID2GAMEINTEXT( strUItext.c_str() ), strTemp.c_str() );
					}
				}
				break;*/
            };

		return strText;
	}

	/*std::vector<std::string> MakeTooltipSting_In_Skill_Link( const GLSKILL* glSkill, WORD wLevel )
	{
		const SKILL::SAPPLY& sAPPLY = glSkill->m_sAPPLY;
		const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
		EMIMPACT_TARGET emIMPACT_TAR = glSkill->m_sBASIC.emIMPACT_TAR;


		std::vector<std::string> vecstrText;

		if ( !sAPPLY.vecSPEC.empty() )
		{
			for ( DWORD i = 0; i <  sAPPLY.vecSPEC.size(); ++i )
			{
				std::string strText = "";

				if( sAPPLY.vecSPEC[i].emSPEC != EMSPECA_SKILL_LINK )
					continue;

				SNATIVEID sLinkID = sAPPLY.vecSPEC[i].sSPEC[wLevel].dwLinkID;
				if( sLinkID == NATIVEID_NULL() )
					continue;

				SNATIVEID sNativeID = sAPPLY.vecSPEC[i].sSPEC[wLevel].dwNativeID;
				if( sNativeID == NATIVEID_NULL() )
					continue;

				GLSKILL* pSLinkSkill = GLSkillMan::GetInstance().GetData( sLinkID.wMainID, sLinkID.wSubID );
				if( !pSLinkSkill )
					continue;

				GLSKILL* pSNativeSkill = GLSkillMan::GetInstance().GetData( sNativeID.wMainID, sNativeID.wSubID );
				if( !pSNativeSkill )
					continue;

				strText = sc::string::format( ID2GAMEINTEXT( "SKILL_SPEC_LINK" ),
											  pSLinkSkill->GetNameStr() ,					// ���Ǻν�ų;
											  sAPPLY.vecSPEC[i].sSPEC[wLevel].fVAR1*100,	// Ȯ��;
											  pSNativeSkill->GetNameStr() );				// �ߵ� ��ų;
				vecstrText.push_back( strText );
			}
		}
		return vecstrText;
    }


	// ���/�Ӽ�;
	bool MakeTooltipString_In_Skill_Grade (
		std::string& strText,
		GLSKILL* pSkill )
	{
		if ( !pSkill )
			return false;

		strText = "";
		strText = sc::string::format( "%d%s", pSkill->m_sBASIC.dwGRADE, ID2GAMEWORD( "SKILL_BASIC_INFO", 1 ) );
		strText += "/";

		if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
		{
			strText += sc::string::format( " %s", CGameTextMan::GetInstance().GetText("ELEMENT", pSkill->m_sAPPLY.emELEMENT, CGameTextMan::EM_COMMENT_TEXT).GetString());
		}
		else
		{
			strText += sc::string::format( " %s", COMMENT::ELEMENT[ pSkill->m_sAPPLY.emELEMENT ] );
		}

		return true;
	}

	// ���� Ÿ��;
	bool MakeTooltipString_In_Skill_ApplyType (
		std::string& strText,
		GLSKILL* pSkill )
	{
		if ( !pSkill )
			return false;

		bool bDisplay = false;
		strText = "";

		// Info : ���� Ÿ��
		switch ( pSkill->m_sBASIC.emACTION_TYPE )
		{
		case SKILL::EMACTION_TYPE_NORMAL:
			{
				strText = sc::string::format( "%s",
					ID2GAMEWORD( "SKILL_TOOLTIP_BASIC_TYPE", 0 ) );
				bDisplay = true;
			}
			break;

		case SKILL::EMACTION_TYPE_BUFF:
			{
				strText = sc::string::format( "%s",
					ID2GAMEWORD( "SKILL_TOOLTIP_BASIC_TYPE", 1 ) );
				bDisplay = true;
			}
			break;

		case SKILL::EMACTION_TYPE_DEBUFF:
			{
				strText = sc::string::format( "%s",
					ID2GAMEWORD( "SKILL_TOOLTIP_BASIC_TYPE", 2 ) );
				bDisplay = true;
			}
			break;

		case SKILL::EMACTION_TYPE_LIMIT:
			{
				strText = sc::string::format( "%s",
					ID2GAMEWORD( "SKILL_TOOLTIP_BASIC_TYPE", 3 ) );
				bDisplay = true;
			}
			break;
		}

		if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_NORMAL )
		{
			switch ( pSkill->m_sBASIC.emAPPLY )
			{
			case SKILL::EMAPPLY_MELEE:
				{
					strText += sc::string::format( "/ %s",
						ID2GAMEWORD( "SKILL_TOOLTIP_BASIC_TYPE", 4 ) );
					bDisplay = true;
				}
				break;

			case SKILL::EMAPPLY_RANGE:
				{
					strText += sc::string::format( "/ %s",
						ID2GAMEWORD( "SKILL_TOOLTIP_BASIC_TYPE", 5 ) );
					bDisplay = true;
				}
				break;

			case SKILL::EMAPPLY_MAGIC:
				{
					strText += sc::string::format( "/ %s",
						ID2GAMEWORD( "SKILL_TOOLTIP_BASIC_TYPE", 6 ) );
					bDisplay = true;
				}
				break;
			}
		}
		else if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )
		{
			strText += ID2GAMEWORD( "SKILL_BASIC_INFO", 8 );
			bDisplay = true;
		}

		return bDisplay;
	}

	// ��� ����;
	bool MakeTooltipString_In_Skill_UseWeapon (
		std::string& strText,
		GLSKILL* pSkill )
	{
		if ( !pSkill )
			return false;

		bool bDisplay = false;
		strText = "";

		const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;

		if ( ( sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != SKILL::SKILLATT_NOCARE ) ) || 
			( sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != SKILL::SKILLATT_NOCARE ) ) )
		{
			bool bDistinction = false;
			if ( sBASIC.emUSE_LITEM )
			{
				if ( sBASIC.emUSE_LITEM != SKILL::SKILLATT_NOCARE )
				{
					bDistinction = true;

					strText += sc::string::format( "%s",
						ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM",
						sBASIC.emUSE_LITEM - 1 ) );
					bDisplay = true;
				}
			}

			if ( sBASIC.emUSE_RITEM )
			{
				if ( sBASIC.emUSE_RITEM != SKILL::SKILLATT_NOCARE )
				{
					if ( bDistinction )
						strText += "/";

					strText += sc::string::format( "%s",
						ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM",
						sBASIC.emUSE_RITEM - 1 ) );
					bDisplay = true;
				}
			}
		}

		return bDisplay;
	}

	// �Ҹ� ����;
	bool MakeTooltipString_In_Skill_Requirement (
		std::string& strText,
		GLSKILL* pSkill,
		WORD wLevel )
	{
		if ( !pSkill )
			return false;

		bool bDisplay = false;
		strText = "";

		//	���� ������
		if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE )
		{
			const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;
			const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[ wLevel ];

			int nIndex = 0;

			//	ȭ�� ����
			if ( 0 < sDATA_LVL.wUSE_ARROWNUM )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 0 ),
					sDATA_LVL.wUSE_ARROWNUM );
				bDisplay = true;
			}

			//	���� ����
			if ( 0 < sDATA_LVL.wUSE_CHARMNUM )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 1 ),
					sDATA_LVL.wUSE_CHARMNUM );
				bDisplay = true;
			}

			//	�Ѿ� ����
			if ( 0 < sDATA_LVL.wUSE_BULLETNUM )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 7 ),
					sDATA_LVL.wUSE_BULLETNUM );
				bDisplay = true;
			}

			// ���͸� �Ҹ�
			if ( 0 < sDATA_LVL.nUSE_BATTERY )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 7 ),
					sDATA_LVL.nUSE_BATTERY);
				bDisplay = true;
			}

			//	HP ������
			if ( 0 < sDATA_LVL.wUSE_HP )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 3 ),
					sDATA_LVL.wUSE_HP);
				bDisplay = true;
			}

			//	MP ������
			if ( 0 < sDATA_LVL.wUSE_MP )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 4 ),
					sDATA_LVL.wUSE_MP);
				bDisplay = true;
			}

			//	SP ������
			if ( 0 < sDATA_LVL.wUSE_SP )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 5 ),
					sDATA_LVL.wUSE_SP);
				bDisplay = true;
			}

			//	CP ������
			if ( 0 < sDATA_LVL.wUSE_CP )
			{
				strText += sc::string::format("%s %d ",
					ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 6 ),
					sDATA_LVL.wUSE_CP);
				bDisplay = true;
			}
		}

		return bDisplay;
	}

	// ���� �ð�;
	bool MakeTooltipString_In_Skill_ReuseTime (
		std::string& strText,
		GLSKILL* pSkill,
		WORD wLevel )
	{
		if ( !pSkill )
			return false;

		bool bDisplay = false;
		strText = "";

		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;
		const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];

		if ( 0 < sDATA_LVL.fDELAYTIME )
		{
			strText = sc::string::format("%2.1f%s",
				sDATA_LVL.fDELAYTIME,
				ID2GAMEWORD( "WAITSERVER_TIMELEFT_UNIT", 0 ) );
			bDisplay = true;
		}

		return bDisplay;
	}

	// ��ų ����;
	bool MakeTooltipString_In_Skill_SkillScope (
		std::string& strText,
		GLSKILL* pSkill,
		WORD wLevel,
		GLCHARLOGIC* pTarget )
	{
		if ( !pSkill )
			return false;

		bool bDisplay = false;
		strText = "";

		const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;
		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;

		const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];

		switch ( pSkill->m_sBASIC.emIMPACT_TAR )
		{
		case TAR_SELF: // �ڽ�;
			switch ( pSkill->m_sBASIC.emIMPACT_REALM )
			{
			case REALM_SELF: // '��ǥ' �ڽ�;
				strText += sc::string::format( "%s", ID2GAMEINTEXT( "SKILL_AREA_SELF_SELF" ) );
				bDisplay = true;
				break;

			case REALM_ZONE: // '��ǥ' ����;
				switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
				{
				case EMIMPACT_SIDE_PARTY: // �ڱ�����;
					if ( 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
					{
						strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_SELF_ZONE_OUR" ),
							sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
						bDisplay = true;
					}
					break;

				case EMIMPACT_SIDE_ENEMY: // ������;
					if ( 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
					{
						strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_SELF_ZONE_ENEMY" ),
							sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
						bDisplay = true;
					}
					break;
				case EMIMPACT_SIDE_ANYBODY: // ��ο���;
					if ( 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
					{
						strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_SELF_ZONE_ANYBODY" ),
							sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
						bDisplay = true;
					}
					break;
				};
			};
			break;

		case TAR_SPEC: // Ÿ��;
			{
				switch ( pSkill->m_sBASIC.emIMPACT_REALM )
				{
				case REALM_SELF: // '��ǥ' �ڽ�;
					switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
					{
					case EMIMPACT_SIDE_PARTY: // �ڱ�����;
						if ( 0 < sBASIC.wTARRANGE )
						{
							strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TARGET_SELF_OUR" ),
								sBASIC.wTARRANGE * 0.1f );
							bDisplay = true;
						}
						break;
					case EMIMPACT_SIDE_ENEMY: // ������;
						if ( 0 < sBASIC.wTARRANGE )
						{
							strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TARGET_SELF_ENEMY" ),
								sBASIC.wTARRANGE * 0.1f );
							bDisplay = true;
						}
						break;
					case EMIMPACT_SIDE_ANYBODY: // ��ο���;
						if ( 0 < sBASIC.wTARRANGE )
						{
							strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TARGET_SELF_ANYBODY" ),
								sBASIC.wTARRANGE * 0.1f );
							bDisplay = true;
						}
						break;
					};

				case REALM_ZONE: // '��ǥ' ����;
					switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
					{
					case EMIMPACT_SIDE_PARTY: // �ڱ�����;
						if ( 0 < sBASIC.wTARRANGE && 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
						{
							strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TARGET_ZONE_OUR" ),
								sBASIC.wTARRANGE * 0.1f, sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
							bDisplay = true;
						}
						break;
					case EMIMPACT_SIDE_ENEMY: // ������;
						if ( 0 < sBASIC.wTARRANGE && 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
						{
							strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TARGET_ZONE_ENEMY" ),
								sBASIC.wTARRANGE * 0.1f, sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
							bDisplay = true;
						}
						break;
					case EMIMPACT_SIDE_ANYBODY: // ��ο���;
						if ( 0 < sBASIC.wTARRANGE && 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
						{
							strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TARGET_ZONE_ANYBODY" ),
								sBASIC.wTARRANGE * 0.1f, sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
							bDisplay = true;
						}
						break;
					};

				case REALM_FANWIZE:
					switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
					{
					case EMIMPACT_SIDE_ENEMY:
						if ( 0 < sBASIC.wTARRANGE && 0 < sDATA_LVL.wAPPLYANGLE && 0 < sDATA_LVL.wTARNUM )
						{
							strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TARGET_FANWIZE_ENEMY" ),
								sBASIC.wTARRANGE * 0.1f, sDATA_LVL.wAPPLYANGLE, sDATA_LVL.wTARNUM );
							bDisplay = true;
						}
						break;
					};
				};
			}
			break;

		case TAR_SELF_TOSPEC: // �ڽź��� Ÿ�ٱ���;
			switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
			{
			case EMIMPACT_SIDE_ENEMY:
				WORD wPierce = sDATA_LVL.wPIERCENUM + pTarget->GETSUM_PIERCE() + 1;

				if ( 0 < sBASIC.wTARRANGE && 0 < sDATA_LVL.wTARNUM && 0 < wPierce )
				{
					strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_TOSPEC_ENEMY" ),
						sBASIC.wTARRANGE * 0.1f, sDATA_LVL.wTARNUM, wPierce );
					bDisplay = true;
				}
				break;
			};
			break;

		case TAR_ZONE: // ���� ��ġ�� �ֺ� Ÿ��;
			switch ( pSkill->m_sBASIC.emIMPACT_SIDE )
			{
			case EMIMPACT_SIDE_PARTY:
				if ( 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
				{
					strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_ZONE_OUR" ),
						sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
					bDisplay = true;
				}
				break;
			case EMIMPACT_SIDE_ENEMY:
				if ( 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
				{
					strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_ZONE_ENEMY" ),
						sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
					bDisplay = true;
				}
				break;
			case EMIMPACT_SIDE_ANYBODY:
				if ( 0 < sDATA_LVL.wAPPLYRANGE && 0 < sDATA_LVL.wTARNUM )
				{
					strText += sc::string::format( ID2GAMEINTEXT( "SKILL_AREA_ZONE_ANYBODY" ),
						sDATA_LVL.wAPPLYRANGE * 0.1f, sDATA_LVL.wTARNUM );
					bDisplay = true;
				}
				break;
			};
			break;

		case TAR_LAND: // ���� ��ġ;
			strText += sc::string::format( "%s", ID2GAMEINTEXT( "SKILL_AREA_LAND" ) );
			bDisplay = true;
			break;
		}

		return bDisplay;
	}

	// ��ų ȿ��;
	bool MakeTooltipString_In_Skill_SkillEffect (
		std::string& strText,
		GLSKILL* pSkill,
		WORD wLevel,
		GLCHARLOGIC* pTarget )
	{
		if ( !pSkill )
			return false;

		bool bDisplay = false;
		strText = "";

		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;
		const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[ wLevel ];

		int effectCount = 0;
		if( GLSkillToolTip::MakeTooltipSting_In_Skill_Type(
			pTarget, pSkill, wLevel) != "" )
		{
			++effectCount;
			strText = sc::string::format( "%d. %s\n\n", effectCount,
				GLSkillToolTip::MakeTooltipSting_In_Skill_Type(
				pTarget, pSkill, wLevel ) );
			bDisplay = true;
		}

		std::vector<std::string> skillImpact =
			GLSkillToolTip::MakeTooltipSting_In_Skill_Impact(
			pSkill, wLevel );
		BOOST_FOREACH( std::string& addStr, skillImpact )
		{
			++effectCount;
			strText += sc::string::format( "%d. %s", effectCount, addStr.c_str() );
			strText += "\r\n\r\n";
			bDisplay = true;
		}

		std::vector<std::string> skillSpecial =
			GLSkillToolTip::MakeTooltipSting_In_Skill_Special(
			pSkill, wLevel, true );
		BOOST_FOREACH( std::string& addStr, skillSpecial )
		{
			++effectCount;
			strText += sc::string::format( "%d. %s", effectCount, addStr.c_str() );
			strText += "\r\n\r\n";
			bDisplay = true;
		}

		return bDisplay;
	}

	// ���� �̻�;
	bool MakeTooltipString_In_Skill_Blow (
		std::string& strText,
		GLSKILL* pSkill,
		WORD wLevel )
	{
		if ( !pSkill )
			return false;

		bool bDisplay = false;
		strText = "";

		std::vector< std::string > stateExplain =
			GLSkillToolTip::MakeTooltipSting_In_Skill_Special(
			pSkill, wLevel, true );

		for ( DWORD i=0 ; i < pSkill->m_sAPPLY.vecSPEC.size(); ++i )
		{
			std::string strName;
			if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
			{
				strName = CGameTextMan::GetInstance().GetCommentText("SPEC_ADDON", pSkill->m_sAPPLY.vecSPEC[i].emSPEC).GetString();
			}
			else
			{
				strName = 
					COMMENT::SPEC_ADDON[ pSkill->m_sAPPLY.vecSPEC[i].emSPEC ];
			}

			strText += sc::string::format( "%s. ", i+1 );
			strText += strName;
			strText += " : ";
			strText += stateExplain.at( i );
			strText += "\n\n";
			bDisplay = true;
		}

		return bDisplay;
	}*/

}
