#include "stdafx.h"
#include "./GLogicEx.h"
#include "./GLItemMan.h"
#include "./GLGaeaServer.h"
#include "./GLChar.h"
#include "./GLQuest.h"
#include "./GLQuestMan.h"
#include "./GLSchoolFreePK.h"

//#include "CItemSetLists.h"
//#include "CItemSetListOption.h"

#include "../Interface/GameTextControl.h"

//#include "GLSkillLearn/GLSkillLearn.h"

#include "GLGaeaClient.h"

#include "../Lib_ClientUI/Interface/InnerInterface.h"
#include "../Lib_ClientUI/Interface/UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�~�I�t��
bool GLCHARLOGIC::RESET_STATS_TO(const DWORD dwDIS_STAT)
{
	//	Note : ���� ȣ�� ������ stats, skill���̿��� �Ѵ�.
	//RESET_STATS ( dwDIS_STAT );
	/*�u�Ƭ~�I�t�Υ\��N�X , SSODomain 28/05/2021  */
	{
		m_wStatsPoint += m_sStats.GET(EMPOW) + m_sStats.GET(EMDEX) + m_sStats.GET(EMSPI) + m_sStats.GET(EMSTR) + m_sStats.GET(EMSTA);
		m_sStats.RESET();
		INIT_DATA(FALSE, FALSE);
		return true;
	}
	return true;
}

//�~���t��
bool GLCHARLOGIC::RESET_STATS_TO1(const DWORD dwDIS_STAT)
{
	//	Note : ���� ȣ�� ������ stats, skill���̿��� �Ѵ�.
	//RESET_STATS ( dwDIS_STAT );
	//�u�Ƭ~�I�t�Υ\��N�X , SSODomain 28/05/2021  
	{
		m_nBright = 0;
		return true;
	}
	return true;
}