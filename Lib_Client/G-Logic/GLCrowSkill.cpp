#include "stdafx.h"
#include "./GLCrow.h"
#include "./GLGaeaServer.h"

/*pvp tyranny, Juver, 2017/08/24 */
#include "GLPVPTyrannyField.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL GLCrow::PreSkillProc ()
{
	GASSERT(m_pAttackProp);
	if ( !m_pAttackProp )	return FALSE;

	SNATIVEID sRunSkill = m_pAttackProp->skill_id;
	WORD wLevel = m_pAttackProp->skill_lev;

	//	Note : 스킬 수행 가능 검사.
	//
	SANIATTACK &sAniAttack = m_pAttackProp->sAniAttack;
	WORD wStrikeNum = sAniAttack.m_wDivCount;
	
	EMSKILLCHECK emSkillCheck = CHECHSKILL ( m_dwAType );
	if ( emSkillCheck!=EMSKILL_OK && emSkillCheck!=EMSKILL_NOTSP)
	{
		return FALSE;
	}


	//	Note : 목표물의 위치를 항상 갱신해야 한다.
	//
	GLACTOR *pActor = GLGaeaServer::GetInstance().GetTarget(m_pLandMan,m_TargetID);
	if ( pActor )	m_TargetID.vPos = pActor->GetPosition();

	//	Note : Skill 목표를 선택함.
	//
	D3DXVECTOR3 vTarPos(0,0,0);
	BOOL bOK = SelectSkillTarget ( m_TargetID, &vTarPos, sRunSkill, wLevel );
	if ( !bOK )		return FALSE;

	SETACTIVESKILL ( sRunSkill, wLevel );

	//	Note : 스킬이 발동됨.
	//
	TurnAction ( GLAT_SKILL );

	return TRUE;
}

void GLCrow::StartSkillProc ()
{
	//	Note : 스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL.wMainID, m_idACTIVESKILL.wSubID );
	if ( !pSkill )								return;

	//	Note : 스킬사용 소모 수치 감산.
	ACCOUNTSKILL ( 1 );

	//	Note : 스킬 메시지 발생.
	//
	GLMSG::SNETCROW_SKILL NetMsg;
	NetMsg.dwGlobID = m_dwGlobID;

	NetMsg.dwAType = m_dwAType;
	NetMsg.skill_id = m_idACTIVESKILL;
	NetMsg.skill_lev = m_wACTIVESKILL_LVL;
	NetMsg.vTARPOS = m_vTARPOS;
	for ( WORD i=0; i<m_wTARNUM; ++i )		NetMsg.ADDTARGET ( m_sTARIDS[i] );
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsg );
	
	STARGETID sTargetID = STARGETID ( CROW_MOB, m_dwGlobID, m_vPos );
	
	if( m_TargetID.emCrow == CROW_PC )
	{
		PGLCHAR pCHAR = GLGaeaServer::GetInstance().GetChar ( m_TargetID.dwID );
		if ( pCHAR )
		{
			// PET Attack 처리
			PGLPETFIELD pEnemyPet = GLGaeaServer::GetInstance().GetPET ( pCHAR->m_dwPetGUID );
			if ( pEnemyPet && pEnemyPet->IsValid () && !pEnemyPet->IsSTATE ( EM_PETACT_ATTACK ) )
			{
				D3DXVECTOR3 vOwnerPos, vDist;
				float fDist;
				vOwnerPos = pCHAR->GetPosition ();
				vDist = pEnemyPet->m_vPos - vOwnerPos;
				fDist = D3DXVec3Length(&vDist);

				// 일정거리 안에 있으면 
				if ( fDist <= GLCONST_PET::fWalkArea )
				{
					pEnemyPet->ReSetAllSTATE ();
					pEnemyPet->SetSTATE ( EM_PETACT_ATTACK );

					GLMSG::SNETPET_ATTACK NetMsg;
					NetMsg.sTarID = sTargetID;
					GLGaeaServer::GetInstance().SENDTOCLIENT ( pCHAR->m_dwClientID, &NetMsg );

					GLMSG::SNETPET_ATTACK_BRD NetMsgBRD;
					NetMsgBRD.dwGUID = pEnemyPet->m_dwGUID;
					NetMsgBRD.sTarID = sTargetID;
					pCHAR->SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
				}
			}
			// Summon Attack 처리
			/*skill summon, Juver, 2017/10/09 */
			for ( int ii=0; ii<SKILL_SUMMON_MAX_CLIENT_NUM; ++ii )
			{
				PGLSUMMONFIELD pEnemySummon = GLGaeaServer::GetInstance().GetSummon ( pCHAR->m_dwSummonGUID_FLD[ii] );
				if ( pEnemySummon && pEnemySummon->IsValid () && !pEnemySummon->IsSTATE ( EM_SUMMONACT_ATTACK ) && 
					pEnemySummon->GetAttackTarget( sTargetID ) )
				{
					/*D3DXVECTOR3 vOwnerPos, vDist;
					float fDist;
					vOwnerPos = pCHAR->GetPosition ();
					vDist = pEnemySummon->m_vPos - vOwnerPos;
					fDist = D3DXVec3Length(&vDist);

					if ( fDist <= pEnemySummon->m_fWalkArea )*/
					{
						pEnemySummon->SetAttackTarget( sTargetID );
					}
				}
			}
		}
	}else if( m_TargetID.emCrow == CROW_SUMMON )
	{
		// Summon Attack 처리
		PGLSUMMONFIELD pEnemySummon = GLGaeaServer::GetInstance().GetSummon ( m_TargetID.dwID );
		if ( pEnemySummon && pEnemySummon->IsValid () && !pEnemySummon->IsSTATE ( EM_SUMMONACT_ATTACK ) && 
			pEnemySummon->GetAttackTarget( sTargetID ) )
		{
			//D3DXVECTOR3 vOwnerPos, vDist;
			//float fDist;
			//vOwnerPos = pCHAR->GetPosition ();
			//vDist = pEnemySummon->m_vPos - vOwnerPos;
			//fDist = D3DXVec3Length(&vDist);

			//// 일정거리 안에 있으면 
			//if ( fDist <= GLCONST_SUMMON::fWalkArea )
			//{
			pEnemySummon->SetAttackTarget( sTargetID );
		}				
	}

	return;
}

void GLCrow::SkillProc ( BOOL bLowSP )
{
	if ( !m_pAttackProp )	return;

	SANIATTACK &sAniAttack = m_pAttackProp->sAniAttack;

	//	스킬 정보 가져옴.
	SNATIVEID skill_id = m_idACTIVESKILL;
	WORD wSKILL_LVL = m_wACTIVESKILL_LVL;

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id );
	if ( !pSkill )	return;

	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wSKILL_LVL];
	
	int nEXP_NUM = 0;
	int nALL_LVL = 0;
	int nALL_VARHP = 0, nALL_FULLHP = 0;
	int nALL_VARMP = 0, nALL_FULLMP = 0;
	int nALL_VARSP = 0, nALL_FULLSP = 0;

	WORD wMAX_TAR = 0;
	if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC )			wMAX_TAR = sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE() + 1;
	else if ( pSkill->m_sBASIC.emIMPACT_REALM == REALM_FANWIZE )	wMAX_TAR = sSKILL_DATA.wTARNUM * ( sSKILL_DATA.wPIERCENUM + GETSUM_PIERCE() + 1 );
	else															wMAX_TAR = sSKILL_DATA.wTARNUM;

	if ( m_wTARNUM > wMAX_TAR )		m_wTARNUM = wMAX_TAR;

	/*dash skill logic, Juver, 2017/06/17 */
	BOOL bACTDASH = FALSE;

	//	Note : 스킬이 적용되는 타겟 검사.
	//
	STARGETID sTARID;
	for ( WORD i=0; i<m_wTARNUM; ++i )
	{
		const STARID &_sTARID = m_sTARIDS[i];

		sTARID.emCrow = _sTARID.GETCROW();
		sTARID.dwID = _sTARID.GETID();
		GLACTOR* pACTOR = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTARID );
		if ( !pACTOR )					continue;

		for ( WORD j=0; j<sSKILL_DATA.wAPPLYNUM; ++j )
		{
			if ( pACTOR->GetNowHP()==0 )	continue;

			//	Note : 물리 데미지 발생.
			//
			DWORD dwDamageFlag = DAMAGE_TYPE_NONE;
			int nVAR_HP(0), nVAR_MP(0), nVAR_SP(0);

			//	저항치.
			short nRESIST = pACTOR->GETRESIST().GetElement(pSkill->m_sAPPLY.emELEMENT);
			if ( nRESIST>GLCONST_CHAR::fMAX_RESIST )	nRESIST = GLCONST_CHAR::fMAX_RESIST;

			//	SKILL 기본 적용.
			//
			switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
			{
			case SKILL::EMFOR_HP:
				if ( sSKILL_DATA.fBASIC_VAR < 0.0f )		//	대상에게 피해를 주는 스킬.
				{
//					bCRITICAL = CALCDAMAGE ( nVAR_HP, bShock, sTARID, m_pLandMan, pSkill, wSKILL_LVL, GLPeriod::GetInstance().GetWeather(), sAniAttack.m_wDivCount );
					DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
					dwDamageFlag = CALCDAMAGE ( nVAR_HP, m_dwGlobID, sTARID, m_pLandMan, pSkill, wSKILL_LVL, dwWeather, sAniAttack.m_wDivCount );
					nVAR_HP = - nVAR_HP;
				}
				else										//	대상을 회복시켜 주는 스킬.
				{
					nVAR_HP += (int) ( sSKILL_DATA.fBASIC_VAR );
				}
				break;

			case SKILL::EMFOR_MP:
				if ( sSKILL_DATA.fBASIC_VAR < 0.0f )		//	대상에게 피해를 주는 스킬.
				{
					int nVAR = int(-sSKILL_DATA.fBASIC_VAR);
					nVAR_MP -= (int) ( nVAR - (nVAR*nRESIST*0.01f*GLCONST_CHAR::fRESIST_G ) );
				}
				else										//	대상을 회복시켜 주는 스킬.
				{
					nVAR_MP += (int) ( sSKILL_DATA.fBASIC_VAR );
				}
				break;

			case SKILL::EMFOR_SP:
				if ( sSKILL_DATA.fBASIC_VAR < 0.0f )		//	대상에게 피해를 주는 스킬.
				{
					int nVAR = int(-sSKILL_DATA.fBASIC_VAR);
					nVAR_SP -= (int) ( nVAR - (nVAR*nRESIST*0.01f*GLCONST_CHAR::fRESIST_G ) );
				}
				else										//	대상을 회복시켜 주는 스킬.
				{
					nVAR_SP += (int) ( sSKILL_DATA.fBASIC_VAR );
				}
				break;
			};

			//	Note : SP 부족시에는 변화 값을 반으로 조정.
			//
			if ( bLowSP )
			{
				nVAR_HP /= 2;
				nVAR_MP /= 2;
				nVAR_SP /= 2;
			}

			//	Note : 스킬 특수 기능.
			//
			int nGATHER_HP(0), nGATHER_MP(0), nGATHER_SP(0);

			for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
			{
				const SKILL::SSPEC &sSKILL_SPEC = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL];

				switch ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC )
				{
				case EMSPECA_HP_GATHER:
					nGATHER_HP += int ( abs(nVAR_HP) * sSKILL_SPEC.fVAR1 );
					break;

				case EMSPECA_MP_GATHER:
					nGATHER_MP += int ( abs(nVAR_MP) * sSKILL_SPEC.fVAR1 );
					break;

				case EMSPECA_SP_GATHER:
					nGATHER_SP += int ( abs(nVAR_SP) * sSKILL_SPEC.fVAR1 );
					break;
				};
			}

			

			//	Note : 스킬 기본 영향.
			//
			if ( pACTOR->GetCrow() == CROW_PC && sSKILL_DATA.fBASIC_VAR < 0.0f )
			{
				PGLCHAR pCHARACTOR = (PGLCHAR)pACTOR;
				WORD wAbsorb = (WORD)pCHARACTOR->ISHAVE_BUFF( (EMSPEC_ADDON) EMSPECA_IGNORE_DAMAGE );
				if ( wAbsorb != SKILLFACT_SIZE ) 
				{
					pCHARACTOR->DISABLESKEFF(wAbsorb);

					GLMSG::SNETPC_REMOVE_SKILL_EFFECT_BRD NetMsgBrd;
					NetMsgBrd.dwGaeaID = pCHARACTOR->m_dwGaeaID;
					NetMsgBrd.dwSkillID = wAbsorb;
					pCHARACTOR->SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBrd );
					pCHARACTOR->SendToClient ( (NET_MSG_GENERIC*) &NetMsgBrd );

					SSKILLACT sSKILLACT;
					sSKILLACT.sID = STARGETID(CROW_MOB,GetCtrlID());
					sSKILLACT.sID_TAR = STARGETID(_sTARID.GETCROW(),_sTARID.GETID());
					sSKILLACT.dwDamageFlag = DAMAGE_TYPE_NONE;
					m_pLandMan->RegSkillAct ( sSKILLACT );

					pCHARACTOR->MsgSendUpdateState(false,false,true);
				}
				else
				{
					//	Note : 스킬 기본 영향.
					//
					SSKILLACT sSKILLACT;
					sSKILLACT.sID = STARGETID(CROW_MOB,GetCtrlID());
					sSKILLACT.sID_TAR = STARGETID(_sTARID.GETCROW(),_sTARID.GETID());
					sSKILLACT.fDELAY = pSkill->m_sEXT_DATA.fDELAY4DAMAGE;
					sSKILLACT.emAPPLY = pSkill->m_sBASIC.emAPPLY;
					sSKILLACT.dwDamageFlag = dwDamageFlag;

					//	Note : 체력 변화 저장.
					//
					sSKILLACT.nVAR_HP = nVAR_HP;
					sSKILLACT.nVAR_MP = nVAR_MP;
					sSKILLACT.nVAR_SP = nVAR_SP;

					//	Note : 체력 흡수 저장.
					//
					sSKILLACT.nGATHER_HP = nGATHER_HP;
					sSKILLACT.nGATHER_MP = nGATHER_MP;
					sSKILLACT.nGATHER_SP = nGATHER_SP;

					if ( sSKILLACT.VALID() )
					{
						m_pLandMan->RegSkillAct ( sSKILLACT );
					}
				}
			}
		}

		//	Note : 스킬 보조 영향.
		//
		SSKILLACTEX sSKILLACTEX;
		sSKILLACTEX.sID = STARGETID(CROW_MOB,GetCtrlID());
		sSKILLACTEX.sID_TAR = STARGETID(_sTARID.GETCROW(),_sTARID.GETID());
		sSKILLACTEX.fDELAY = pSkill->m_sEXT_DATA.fDELAY4DAMAGE;

		sSKILLACTEX.idSKILL = skill_id;
		sSKILLACTEX.wSKILL_LVL = wSKILL_LVL;

		//	Note : 상태 이상 유발.
		//
		ITEM::SSATE_BLOW sBLOW;
		sBLOW.emTYPE = EMBLOW_NONE;

		const SKILL::SSTATE_BLOW &sSKILL_BLOW = pSkill->m_sAPPLY.sSTATE_BLOW[wSKILL_LVL];

		sBLOW.emTYPE = pSkill->m_sAPPLY.emSTATE_BLOW;
		sBLOW.fRATE = sBLOW.fRATE;
		sBLOW.fLIFE = pSkill->m_sAPPLY.sDATA_LVL[wSKILL_LVL].fLIFE;
		sBLOW.fVAR1 = sBLOW.fVAR1;
		sBLOW.fVAR2 = sBLOW.fVAR2;
        
		if ( sBLOW.emTYPE!=EMBLOW_NONE )
		{
			//	Note : 발생 확율 계산.
			//
			short nBLOWRESIST = pACTOR->GETRESIST().GetElement ( STATE_TO_ELEMENT(sBLOW.emTYPE) );
			if ( nBLOWRESIST>GLCONST_CHAR::fRESIST_G )	nBLOWRESIST = GLCONST_CHAR::fRESIST_G;

//			float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBLOW.emTYPE, GLPeriod::GetInstance().GetWeather(), m_pLandMan->IsWeatherActive() );
			DWORD dwWeather = GLPeriod::GetInstance().GetMapWeather( m_pLandMan->GetMapID().wMainID, m_pLandMan->GetMapID().wSubID );
			float fPOWER = GLOGICEX::WEATHER_BLOW_POW ( sBLOW.emTYPE, dwWeather, m_pLandMan->IsWeatherActive() );

			BOOL bBLOW(FALSE);
			if ( !(pACTOR->GETHOLDBLOW()&STATE_TO_DISORDER(sBLOW.emTYPE)) )
			{
				bBLOW = GLOGICEX::CHECKSTATEBLOW ( sBLOW.fRATE * fPOWER, GETLEVEL(), pACTOR->GetLevel(), nBLOWRESIST );
			}

			if ( bBLOW )
			{
				//	Note : 상태이상 발생.
				//	
				SSTATEBLOW sSTATEBLOW;
				float fLIFE = sBLOW.fLIFE * fPOWER;
				fLIFE = ( fLIFE - (fLIFE*nBLOWRESIST*0.01f*GLCONST_CHAR::fRESIST_G ) );

				sSTATEBLOW.emBLOW = sBLOW.emTYPE;
				sSTATEBLOW.fAGE = fLIFE;
				sSTATEBLOW.fSTATE_VAR1 = sBLOW.fVAR1;
				sSTATEBLOW.fSTATE_VAR2 = sBLOW.fVAR2;

				//	Note : 상태 이상 발생 저장.
				//
				sSKILLACTEX.sSTATEBLOW = sSTATEBLOW;
			}
		}

		//	Note : 스킬 특수 기능.
		//
		BOOL bActionMovement = FALSE;
		BOOL bCUREFLAG = FALSE;
		BOOL bREMOVEFLAG = FALSE;

		//if ( bCrushBlowOne )	bActionMovement = TRUE;
		if ( bACTDASH )			bActionMovement = TRUE;

		for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
		{
			const SKILL::SSPEC &sSKILL_SPEC = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL];
			switch ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC )
			{
			case EMSPECA_PUSHPULL:
				{
					if ( bActionMovement )	break;
					if ( !pACTOR->IsValidBody() )				break;
					if ( RANDOM_POS > sSKILL_SPEC.fVAR2 )		break;	//	Note : 발생확율 적용.
					sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR1;
					bActionMovement = TRUE;
				}break;

				/*push pull skill logic, Juver, 2017/06/04 */
			case EMSPECA_PUSH:
				{
					if ( bActionMovement )			break;
					if ( !pACTOR->IsValidBody() )	break;
					if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
					sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR2; //distance
					sSKILLACTEX.bPush = true;
					bActionMovement = TRUE;
				}break;

				/*push pull skill logic, Juver, 2017/06/04 */
			case EMSPECA_PULL:
				{
					if ( bActionMovement )			break;
					if ( !pACTOR->IsValidBody() )	break;
					if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
					sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR2; //pull speed
					sSKILLACTEX.bPull = true;
					bActionMovement = TRUE;
				}break;

				/*dash skill logic, Juver, 2017/06/17 */
			case EMSPECA_ONWARD:
				{
					if ( bActionMovement )			break;
					if ( bACTDASH )					break;
					if ( !pACTOR->IsValidBody() )	break;
					if ( RANDOM_POS > sSKILL_SPEC.fVAR1 )		break;
					sSKILLACTEX.fPUSH_PULL = sSKILL_SPEC.fVAR2; //dash speed
					sSKILLACTEX.wActionAnim = (WORD)sSKILL_SPEC.dwFLAG; //animation
					sSKILLACTEX.bDash = true;
					bACTDASH = TRUE;
					bActionMovement = TRUE;
				}break;

			case EMSPECA_HP_DIV:
			case EMSPECA_MP_DIV:
			case EMSPECA_SP_DIV:
				break;

			case EMSPECA_RECBLOW:
				{
					//	Note : 상태 이상 치료.
					//
					if ( bCUREFLAG )	break;
					sSKILLACTEX.dwCUREFLAG |= pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL].dwFLAG;
					bCUREFLAG = TRUE;
				}break;

				/*debuff skill logic, Juver, 2017/06/10 */
			case EMSPECA_BUFF_REMOVE:
				{
					if ( bREMOVEFLAG )	break;
					if ( sSKILLACTEX.bDebuff )	break;
					sSKILLACTEX.dwRemoveFlag = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wSKILL_LVL].dwFLAG;
					sSKILLACTEX.bDebuff = true;
					bREMOVEFLAG = TRUE;
				}break;
			};
		}
		

		switch (pSkill->m_sAPPLY.emBASIC_TYPE)
		{
		case SKILL::EMFOR_CURE:
			//	Note : 상태 이상 치료.
			//
			sSKILLACTEX.dwCUREFLAG |= pSkill->m_sAPPLY.dwCUREFLAG;
			break;
		};

		if ( sSKILLACTEX.VALID() )
		{
			m_pLandMan->RegSkillActEx ( sSKILLACTEX );
		}
	}

	//	Note : 경험치 획득 산출.
	//
	int nSUM_EXP = 0;
	if ( nEXP_NUM!=0 )
	{
		//	Note : 회복에 기여했을 때만 경험치 발생.	( 공격시는 제외 )
		//
		if ( nALL_VARHP>0 && nALL_VARMP>0 && nALL_VARSP>0 )
		{
			nALL_LVL /= nEXP_NUM;

			nALL_VARHP = abs(nALL_VARHP) / nEXP_NUM;
			nALL_VARMP = abs(nALL_VARMP) / nEXP_NUM;
			nALL_VARSP = abs(nALL_VARSP) / nEXP_NUM;

			nALL_FULLHP /= nEXP_NUM;
			nALL_FULLMP /= nEXP_NUM;
			nALL_FULLSP /= nEXP_NUM;

			//	실제 변화량이 있을 때만 경험치를 산출.
			if ( nALL_VARHP!=0 )	nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARHP,nALL_FULLHP);
			if ( nALL_VARMP!=0 )	nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARMP,nALL_FULLMP);
			if ( nALL_VARSP!=0 )	nSUM_EXP += GLOGICEX::GLATTACKEXP(GETLEVEL(),nALL_LVL,nALL_VARSP,nALL_FULLSP);
		}
		//	Note : 보조 스킬 사용시 경험치.
		//
		else
		{
			nSUM_EXP = (pSkill->m_sBASIC.dwGRADE*(wSKILL_LVL+1)*100) / GETLEVEL();
			if ( nSUM_EXP < 1 )		nSUM_EXP = 1;
			if ( nSUM_EXP > 5 )		nSUM_EXP = 5;
		}
	}
}

BOOL GLCrow::SkillProcess ( float fElapsedTime )
{
	//	스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_idACTIVESKILL );
	if ( !pSkill )	return FALSE;

	float fSkillRange = (float) ( GETBODYRADIUS() + GETSKILLRANGE_APPLY(*pSkill,m_wACTIVESKILL_LVL) + 2 );

	//	Note : 스킬이 적용되는 타겟 검사.
	//
	DWORD dwVALIDNUM(0);
	STARGETID sTARID;
	for ( WORD i=0; i<m_wTARNUM; ++i )
	{
		sTARID.emCrow = m_sTARIDS[i].GETCROW();
		sTARID.dwID = m_sTARIDS[i].GETID();
		GLACTOR* pACTOR = GLGaeaServer::GetInstance().GetTarget ( m_pLandMan, sTARID );
		if ( !pACTOR )					continue;

		float fReActionRange = (float) ( pACTOR->GetBodyRadius() + fSkillRange );

		D3DXVECTOR3 vDist = m_vPos - pACTOR->GetPosition();
		float fDist = D3DXVec3Length(&vDist);

		if ( fReActionRange*GLCONST_CHAR::fREACT_VALID_SCALE < fDist )
		{
			m_sTARIDS[i].wID = USHRT_MAX;
		}
		else
		{
			dwVALIDNUM++;
		}
	}

	if ( dwVALIDNUM==0 )
	{
		TurnAction ( GLAT_IDLE, false );
		return FALSE;
	}

	m_fattTIMER += fElapsedTime * GETATTVELO();

	SANIATTACK &sAniAttack = m_pAttackProp->sAniAttack;
	int nTotalKeys = int(sAniAttack.m_dwETime) - int(sAniAttack.m_dwSTime);
	int nThisKey = int(sAniAttack.m_dwSTime) + int(m_fattTIMER*UNITANIKEY_PERSEC);

	if ( !m_sHITARRAY.empty() )
	{
		SSTRIKE sStrike = *m_sHITARRAY.begin();

		if ( nThisKey >= sStrike.wDivKey )
		{
			if ( sStrike.bHit )		SkillProc ();
			else					AvoidProc ();

			//	큐 데이타가 처리된 경우 삭제한다.
			m_sHITARRAY.pop_front ();
		}	
	}

	//	이상증상에 의해 데이타가 전혀 처리되지 않은경우,
	//	마지막에 한꺼번에 처리하고, FALSE를 리턴한다.
	//
	if ( nThisKey >= int(sAniAttack.m_dwETime) )
	{
		if ( !m_sHITARRAY.empty() )
		{
			while ( !m_sHITARRAY.empty() )
			{
				SSTRIKE sStrike = *m_sHITARRAY.begin();

				if ( sStrike.bHit )		SkillProc ();
				else					AvoidProc ();

				//	큐 데이타가 처리된 경우 삭제한다.
				m_sHITARRAY.pop_front ();
			}
		}

		return FALSE;
	}

	return TRUE;
}

void GLCrow::STATEBLOW ( const SSTATEBLOW &sStateBlow )
{
	int nIndex = 0;

	if ( sStateBlow.emBLOW <= EMBLOW_SINGLE )	nIndex = 0;
	else										nIndex = sStateBlow.emBLOW-EMBLOW_SINGLE;

	m_sSTATEBLOWS[nIndex] = sStateBlow;

	//	상태이상 ( 기절 ) 을 받을때 현제 행위 중단.
	if ( sStateBlow.emBLOW == EMBLOW_STUN )
	{
		TurnAction ( GLAT_IDLE );
	}
}

void GLCrow::CURE_STATEBLOW ( DWORD dwCUREFLAG )
{
	BOOL bChanged = FALSE;

	for ( int i=0; i<EMBLOW_MULTI; ++i )
	{
		if ( m_sSTATEBLOWS[i].emBLOW!=EMBLOW_NONE )		continue;

		bChanged = TRUE;
		if ( STATE_TO_DISORDER(m_sSTATEBLOWS[i].emBLOW)&dwCUREFLAG )	m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE;
	}

	if ( !bChanged )	return;

	//	Note : 상태 이상 변화 MsgFB.
	//
	GLMSG::SNETPC_CURESTATEBLOW_BRD NetMsgBRD;
	NetMsgBRD.dwCUREFLAG = dwCUREFLAG;

	//	Note : 주변 클라이언트들에게 메세지 전송.
	//
	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgBRD );
}

/*debuff skill logic, Juver, 2017/06/10 */
void GLCrow::BUFF_REMOVE( DWORD dwBuffFlag )
{
	GLMSG::SNETPC_SKILLHOLD_RS_BRD NetMsgSkillBrd;
	for ( int i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID == NATIVEID_NULL() ) continue;
	
        PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sSKILLFACT[i].sNATIVEID );
		if ( !pSkill ) continue;
		if ( pSkill->m_sBASIC.emACTION != (SKILL::EMACTION)dwBuffFlag ) continue;
	
		DISABLESKEFF( i );
		NetMsgSkillBrd.bRESET[i] = true;
	}
	
	NetMsgSkillBrd.dwID = m_dwGlobID;
	NetMsgSkillBrd.emCrow = CROW_MOB;

	SendMsgViewAround ( (NET_MSG_GENERIC*) &NetMsgSkillBrd );
}

void GLCrow::DO_STUN_ACTION ()
{
	//if ( IsNoAllPenalty() )
	//	return;

	TurnAction ( GLAT_IDLE );
}

//! 아이템 우선권을 주기위한 데미지 로그 기록
void GLCrow::VAR_BODY_POINT ( const EMCROW emACrow, const DWORD dwAID, const BOOL bPartySkill, int nvar_hp, int nvar_mp, int nvar_sp )
{
	if ( m_dwNowHP==0 )		return;

	DWORD dwDxHP = GLOGICEX::VARIATION ( m_dwNowHP, GETMAXHP(), nvar_hp );
	GLOGICEX::VARIATION ( m_wNowMP, GETMAXMP(), nvar_mp );
	GLOGICEX::VARIATION ( m_wNowSP, GETMAXSP(), nvar_sp );

	BOOL bDamage = nvar_hp<0;

	if ( bDamage )
	{
		//	자신을 공격한 사람 등록.
		m_sAssault.emCrow = emACrow;
		m_sAssault.dwID = dwAID;

		//	데미지 기록.
		if ( emACrow == CROW_PC )
		{
			PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar ( dwAID );
			if ( pChar )
			{
				AddDamageLog      ( m_cDamageLog, dwAID, pChar->GetUserID(), dwDxHP );
				AddDamageLogParty ( m_cDamageLogParty, pChar->m_dwPartyID, dwDxHP );
			}
		}

		//  소환수 공격일 경우 SummonDamageLog에 기록한다.
		if ( emACrow == CROW_SUMMON )
		{
			PGLSUMMONFIELD pSummon = GLGaeaServer::GetInstance().GetSummon ( dwAID );
			if( pSummon )
			{
				AddDamageLog      ( m_cSummonDamageLog, dwAID, pSummon->GetCtrlID(), dwDxHP );
				AddDamageLogParty ( m_cDamageLogParty, pSummon->m_pOwner->m_dwPartyID, dwDxHP );
			}
		}

		/*pvp tyranny, Juver, 2017/08/24 */
		if (emACrow == CROW_PC && m_pCrowData && m_pCrowData->m_emNPCType == EMNPC_TYPE_EVENT_TOWER)
		{
			PGLCHAR _pchar = GLGaeaServer::GetInstance().GetChar(dwAID);

			if (_pchar && _pchar->m_wSchool != m_wTyrannyOwner)
			{
				m_dwTyrannyDamage[_pchar->m_wSchool] += dwDxHP;
				TyrannyDamageCheck();

				if (m_pLandMan && m_pLandMan->m_bPVPTyrannyMap)
					GLPVPTyrannyField::GetInstance().ScoreDamage(_pchar->m_dwCharID, dwDxHP);
			}
		}
	}

			

	HRESULT hrESCAPE = S_OK;
	EMCROWACT_UP emCROWACT = GetActPattern();
	int nDamageHP = abs(nvar_hp);
	if ( bDamage )		hrESCAPE = ESCAPE ( STARGETID ( emACrow, dwAID ), nDamageHP );
	
	if ( EMCROWACT_UP_ESCAPE != emCROWACT || hrESCAPE!=S_OK )
	{
		//	Note :타겟이 존제하지 않을때 공격자가 있으면 타겟으로 등록 시도.
		//
		if ( (!IsFLAG(EMTARGET) && bDamage) || CheckGroupAttack() )
		{
			if ( emACrow==CROW_PC )	
			{
				PGLCHAR pTarget = GLGaeaServer::GetInstance().GetChar(dwAID);	
				if ( pTarget )
				{
					STARGETID sTARID(CROW_PC,pTarget->GetCtrlID(),pTarget->GetPosition());
					NewTarget ( sTARID, TRUE );
				}
			}
			
			/*skill summon, Juver, 2017/10/10 */
			if ( emACrow==CROW_SUMMON )	
			{
				PGLSUMMONFIELD psummon_field = GLGaeaServer::GetInstance().GetSummon(dwAID);	
				if ( psummon_field )
				{
					STARGETID sTARID(CROW_SUMMON,psummon_field->GetCtrlID(),psummon_field->GetPosition());
					NewTarget ( sTARID, TRUE );
				}
			}

			
		}
	}
}

//! 아이템 우선권을 주기위한 데미지 로그 기록
/*
void GLCrow::VAR_BODY_POINT (const EMCROW emACrow,
							 const DWORD dwAID,
							 const BOOL bPartySkill,
							 int nvar_hp,
							 int nvar_mp,
							 int nvar_sp)
{
	if ( m_dwNowHP==0 )		return;

	DWORD dwDxHP = GLOGICEX::VARIATION ( m_dwNowHP, GETMAXHP(), nvar_hp );
	GLOGICEX::VARIATION ( m_wNowMP, GETMAXMP(), nvar_mp );
	GLOGICEX::VARIATION ( m_wNowSP, GETMAXSP(), nvar_sp );

	BOOL bDamage = nvar_hp<0;

	if ( bDamage )
	{
		//	자신을 공격한 사람 등록.
		m_sAssault.emCrow = emACrow;
		m_sAssault.dwID = dwAID;

		//	데미지 기록.
		if ( emACrow == CROW_PC )
		{
			PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar ( dwAID );
			if ( pChar )
			{				
				// 자신이 준 데미지 이지만... 파티에 가입되어 있다면...
				if (pChar->m_dwPartyID != PARTY_NULL)
				{
					GLPARTY_FIELD *pParty = GLGaeaServer::GetInstance().GetParty(pChar->m_dwPartyID);
					if (pParty != NULL)
					{
						pParty->m_dwMASTER; // 마스터의 ID
						GLGaeaServer::GetInstance().GetChar(
					}
				}
				AddDamageLog ( m_cDamageLog, dwAID, pChar->m_dwUserID, dwDxHP );
			}
		}
	}

	HRESULT hrESCAPE = S_OK;
	EMCROWACT_UP emCROWACT = GetActPattern();
	int nDamageHP = abs(nvar_hp);
	if ( bDamage )		hrESCAPE = ESCAPE ( STARGETID ( emACrow, dwAID ), nDamageHP );
	
	if ( EMCROWACT_UP_ESCAPE != emCROWACT || hrESCAPE!=S_OK )
	{
		//	Note :타겟이 존제하지 않을때 공격자가 있으면 타겟으로 등록 시도.
		//
		if ( !IsFLAG(EMTARGET) && bDamage )
		{
			PGLCHAR pTarget = NULL;
			if ( emACrow==CROW_PC )		pTarget = GLGaeaServer::GetInstance().GetChar(dwAID);
			
			if ( pTarget )
			{
				STARGETID sTARID(CROW_PC,pTarget->GetCtrlID(),pTarget->GetPosition());
				NewTarget ( sTARID );
			}
		}
	}
}
*/

DWORD GLCrow::SELECT_SKILLSLOT ( SNATIVEID skill_id )
{
	//	Note : 스킬이 들어갈 기본 슬롯 선택.
	//
	DWORD dwSELECT = UINT_MAX;
	for ( DWORD i=0; i<SKILLFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID==skill_id )
		{
			dwSELECT = i;
			break;
		}
	}

	if ( dwSELECT == UINT_MAX )
	{
		float fAGE = FLT_MAX;

		for ( DWORD i=0; i<SKILLFACT_SIZE; ++i )
		{
			if ( m_sSKILLFACT[i].sNATIVEID==NATIVEID_NULL() )
			{
				dwSELECT = i;
				break;
			}

			if ( m_sSKILLFACT[i].fAGE < fAGE )
			{
				fAGE = m_sSKILLFACT[i].fAGE;
				dwSELECT = i;
			}
		}
	}

	return dwSELECT;
}

BOOL GLCrow::RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, DWORD &dwSELECT, WORD _wCasterCrow, DWORD _dwCasterID )
{
	dwSELECT = SKILLFACT_SIZE;

	//	스킬 정보 가져옴.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )				return FALSE;
	if ( wlevel >= SKILL::MAX_LEVEL )	return FALSE;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wlevel];

	BOOL bHOLD = FALSE;
	SSKILLFACT sSKILLEF;
	sSKILLEF._wCasterCrow = _wCasterCrow;
	sSKILLEF._dwCasterID = _dwCasterID;

	switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
	{
	case SKILL::EMFOR_VARHP:
	case SKILL::EMFOR_VARMP:
	case SKILL::EMFOR_VARSP:
	case SKILL::EMFOR_DEFENSE:
	case SKILL::EMFOR_HITRATE:
	case SKILL::EMFOR_AVOIDRATE:
	case SKILL::EMFOR_VARAP:
	case SKILL::EMFOR_VARDAMAGE:
	case SKILL::EMFOR_VARDEFENSE:
	case SKILL::EMFOR_PA:
	case SKILL::EMFOR_SA:
	case SKILL::EMFOR_MA:
	case SKILL::EMFOR_RESIST:
		bHOLD = TRUE;
		sSKILLEF.emTYPE = pSkill->m_sAPPLY.emBASIC_TYPE;
		sSKILLEF.fMVAR = sSKILL_DATA.fBASIC_VAR;
		break;
	};

	for( int nImpact=0; nImpact<SKILL::MAX_IMPACT; ++nImpact )
	{
		if ( pSkill->m_sAPPLY.sImpacts[nImpact].emADDON != EMIMPACTA_NONE )
		{
			bHOLD = TRUE;
			sSKILLEF.sImpacts[nImpact].emADDON = pSkill->m_sAPPLY.sImpacts[nImpact].emADDON;
			sSKILLEF.sImpacts[nImpact].fADDON_VAR = pSkill->m_sAPPLY.sImpacts[nImpact].fADDON_VAR[wlevel];
		}
	}

	for( int nSpec=0; nSpec<SKILL::MAX_SPEC; ++nSpec )
	{
		switch ( pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC )
		{
		case EMSPECA_REFDAMAGE:
		case EMSPECA_NONBLOW:
		case EMSPECA_PIERCE:
		case EMSPECA_TARRANGE:
		case EMSPECA_MOVEVELO:
		case EMSPECA_CRUSHING_BLOW:
		case EMSPECA_PSY_DAMAGE_REDUCE:
		case EMSPECA_MAGIC_DAMAGE_REDUCE:
		case EMSPECA_PSY_DAMAGE_REFLECTION:
		case EMSPECA_MAGIC_DAMAGE_REFLECTION:
		case EMSPECA_DEFENSE_SKILL_ACTIVE:
		case EMSPECA_IMMUNE:
		case EMSPECA_PROHIBIT_POTION:		/*prohibit potion skill logic, Juver, 2017/06/06 */
		case EMSPECA_PROHIBIT_SKILL:		/*prohibit skill logic, Juver, 2017/06/06 */
		case EMSPECA_CHANGE_ATTACK_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_CHANGE_APPLY_RANGE:	/*skill range spec logic, Juver, 2017/06/06 */
		case EMSPECA_STUN:					/*skill stun logic, Juver, 2017/06/06 */
		case EMSPECA_CONTINUOUS_DAMAGE:		/*continuous damage skill logic, Juver, 2017/06/10 */
		case EMSPECA_CURSE:					/*curse skill logic, Juver, 2017/06/10 */
		case EMSPECA_IGNORE_DAMAGE:			/*ignore damage, Juver, 2017/12/12 */
		case EMSPECA_STIGMA:
		case EMSPECA_ENHANCEMENT:
			bHOLD = TRUE;
			sSKILLEF.sSpecs[nSpec].emSPEC = pSkill->m_sAPPLY.sSpecs[nSpec].emSPEC;
			sSKILLEF.sSpecs[nSpec].fSPECVAR1 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].fVAR1;
			sSKILLEF.sSpecs[nSpec].fSPECVAR2 = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].fVAR2;
			sSKILLEF.sSpecs[nSpec].dwSPECFLAG = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].dwFLAG;
			sSKILLEF.sSpecs[nSpec].dwNativeID = pSkill->m_sAPPLY.sSpecs[nSpec].sSPEC[wlevel].dwNativeID;
			break;
		};
	}
	

	//	Note : 지속형 스킬일 경우 최적 슬롯을 찾아서 스킬 효과를 넣어줌.
	//
	if ( bHOLD )
	{
		sSKILLEF.sNATIVEID	= skill_id;
		sSKILLEF.wLEVEL		= wlevel;
		sSKILLEF.fAGE		= sSKILL_DATA.fLIFE;

		dwSELECT = SELECT_SKILLSLOT ( skill_id );
		m_sSKILLFACT[dwSELECT] = sSKILLEF;
	}

	return TRUE;
}
