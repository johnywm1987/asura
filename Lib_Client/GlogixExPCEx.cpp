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

//¬~ÂI¨t²Î
bool GLCHARLOGIC::RESET_STATS_TO(const DWORD dwDIS_STAT)
{
	//	Note : ¸®¼Â È£Ãâ ¼ø¼­°¡ stats, skill¼øÀÌ¿©¾ß ÇÑ´Ù.
	//RESET_STATS ( dwDIS_STAT );
	/*Àu¤Æ¬~ÂI¨t²Î¥\¯à¥N½X , SSODomain 28/05/2021  */
	{
		m_wStatsPoint += m_sStats.GET(EMPOW) + m_sStats.GET(EMDEX) + m_sStats.GET(EMSPI) + m_sStats.GET(EMSTR) + m_sStats.GET(EMSTA);
		m_sStats.RESET();
		INIT_DATA(FALSE, FALSE);
		return true;
	}
	return true;
}

//¬~¬õ¨t²Î
bool GLCHARLOGIC::RESET_STATS_TO1(const DWORD dwDIS_STAT)
{
	//	Note : ¸®¼Â È£Ãâ ¼ø¼­°¡ stats, skill¼øÀÌ¿©¾ß ÇÑ´Ù.
	//RESET_STATS ( dwDIS_STAT );
	//Àu¤Æ¬~ÂI¨t²Î¥\¯à¥N½X , SSODomain 28/05/2021  
	{
		m_nBright = 0;
		return true;
	}
	return true;
}