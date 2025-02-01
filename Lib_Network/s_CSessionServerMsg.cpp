///////////////////////////////////////////////////////////////////////////////
// s_CSessionServerMsg.cpp
//
// * History
// 2002.05.30 jgkim Create
// 2003.10.20 jgkim 
//
// Copyright 2002-2006 (c) Mincoms. All rights reserved.                 
// 
// * Note
// 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "s_CSessionServer.h"
#include "s_CIPFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int CSessionServer::MsgProcess(MSG_LIST* pMsg)
{	
    if (pMsg == NULL) return NET_ERROR;

	NET_MSG_GENERIC* nmg = reinterpret_cast<NET_MSG_GENERIC*> (pMsg->Buffer);

	switch (nmg->nType)
	{
	case NET_MSG_HEARTBEAT_SERVER_ANS: // All server->Session:Heartbeat Answer
		m_pClientManager->SetHeartBeat( pMsg->dwClient );
		break;
	// Agent, Filed, Login->Session:Hearbeat Requst
    case NET_MSG_HEARTBEAT_SERVER_REQ:
        MsgServerHeartbeatAnswer( pMsg );
        break;
	// Login, Agent, Field->Session : ¼­¹ö ÀüÃ¼ Á¤º¸¸¦ Session ¼­¹ö¿¡ Àü¼ÛÇÑ´Ù.
	case NET_MSG_SND_FULL_SVR_INFO :
		MsgServerInfo(pMsg);
		break;
	// Server Manager -> Session : ÀüÃ¼ ¼­¹ö Á¤º¸ ¿äÃ»
	case NET_MSG_REQ_ALL_SVR_INFO :
		MsgSndAllSvrInfo(pMsg);
		break;
	// Login->Session : °ÔÀÓ¼­¹öÀÇ »óÅÂÁ¤º¸¸¦ ¿äÃ»ÇÑ´Ù.
	case NET_MSG_REQ_SVR_INFO :
		MsgSndCurSvrInfo(pMsg->dwClient);
		break;
	// Login, Agent, Field->Session : ÇöÀç ¼­¹ö»óÅÂ(°£·«ÇÑ)¸¦ Àü¼ÛÇÑ´Ù.
	case NET_MSG_SND_CUR_STATE :
		MsgServerCurInfo(pMsg);
		break;
	// Agent->Session : Ã¤³ÎÀÇ À¯ÀúÁ¤º¸
	case NET_MSG_SND_CHANNEL_STATE :
		MsgServerChannelInfo(pMsg);
		break;
	// Agent->Session : Ä³¸¯ÅÍ°¡ °ÔÀÓ¿¡ Á¶ÀÎÇßÀ½ 
	case NET_MSG_GAME_JOIN_OK :
		MsgGameJoinOK(pMsg);
		break;
	// Ã¤ÆÃ¸Þ½ÃÁö Ã³¸®
	case NET_MSG_CHAT : 
		MsgChatProcess(pMsg);
		break;
	case NET_MSG_SVR_CMD :
		MsgSvrCmdProcess(pMsg);
		break;
	case NET_MSG_SND_ENCRYPT_KEY :
		MsgEncryptKey(pMsg);
		break;

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	case NET_MSG_UPDATE_TRACING_ALL:
		MsgTracingChar(pMsg,TRUE);
		break;
	case NET_MSG_UPDATE_TRACING_CHARACTER:
		MsgTracingChar(pMsg,FALSE);
		break;
#endif

	case NET_MSG_IPFILTER_BLOCK_ADD_SESSION:
		MsgServerBroadCastIPFilterBlockAdd( pMsg );
		break;

	case NET_MSG_IPFILTER_BLOCK_DEL_SESSION:
		MsgServerBroadCastIPFilterBlockDel( pMsg );
		break;

	case NET_MSG_IPFILTER_KNOWN_ADD_SESSION:
		MsgServerBroadCastIPFilterKnownAdd( pMsg );
		break;

	case NET_MSG_IPFILTER_KNOWN_DEL_SESSION:
		MsgServerBroadCastIPFilterKnownDel( pMsg );
		break;

	case NET_MSG_IPFILTER_LISTSAVE:
		CIPFilter::GetInstance()->ListSave();
		break;

	default:
		break;
	}
	return 0;
}

void CSessionServer::MsgServerHeartbeatAnswer(
	MSG_LIST* pMsg )
{
	DWORD dwClient = pMsg->dwClient;
	NET_HEARTBEAT_SERVER_ANS msg;
	SendClient( dwClient, &msg );
}

int CSessionServer::FindServer(CString strIP, int nPort)
{
	int nMaxClient = m_pClientManager->GetMaxClient();

	for (int i=0; i<nMaxClient; ++i)
	{		
		if ((m_pClientManager->IsOnline((DWORD) i) == true) && 
		    (strcmp(strIP, m_pClientManager->GetClientIP((DWORD)i)) == 0) &&
			(nPort == m_pClientManager->GetSvrServicePort((DWORD)i)))
		{					
			return i;
		}
	}
	return NET_ERROR;
}

int CSessionServer::FindServer(int nSvrType, int nSvrGrp, int nSvrNum, int nFieldNum)
{
	int nMaxClient = m_pClientManager->GetMaxClient();

	for (int i=0; i<nMaxClient; ++i)
	{
		if (nSvrType == NET_SERVER_FIELD)
		{
			if ((m_pClientManager->IsOnline((DWORD) i) == true) && 
				(m_pClientManager->GetSvrGrp((DWORD) i) == nSvrGrp) &&
				(m_pClientManager->GetSvrNum((DWORD) i) == nSvrNum) &&
				(m_pClientManager->GetSvrField((DWORD) i) == nFieldNum))
			{
				CConsoleMessage::GetInstance()->Write(_T("Find Server %d"), i);
				return i;
			}
		}
		else
		{
			if ((m_pClientManager->IsOnline((DWORD) i) == true) && 
				(m_pClientManager->GetSvrGrp((DWORD) i) == nSvrGrp) &&
				(m_pClientManager->GetSvrNum((DWORD) i) == nSvrNum) &&
				(m_pClientManager->GetSvrType((DWORD) i) == nSvrType))
			{
				CConsoleMessage::GetInstance()->Write(_T("Find Server %d"), i);
				return i;
			}
		}
	}
	CConsoleMessage::GetInstance()->Write(_T("Can't Find Server"));
	return NET_ERROR;
}

int CSessionServer::MsgSndServerStateChange(int nClient, int nState)
{
	NET_SVR_CMD_CHANGE_STATE sTemp;
	sTemp.nCmdType = nState;
	return SendClient(static_cast <DWORD> (nClient), static_cast <LPVOID> (&sTemp));
}

void CSessionServer::MsgTracingChar(MSG_LIST* pMsg, bool bAll )
{
	if( pMsg == NULL ) return;

	if( bAll )
	{
		m_vecTracingData.clear();
		NET_UPDATE_TRACINGCHAR_ALL* msg = reinterpret_cast <NET_UPDATE_TRACINGCHAR_ALL*> (pMsg->Buffer);
		BYTE i;
		for( i = 0; i < MAX_TRACING_NUM; i++ )
		{
			if( msg->tracingData[i].dwUserNum == 0 && msg->tracingData[i].strAccount.size() == 0 ) break;
			m_vecTracingData.push_back(msg->tracingData[i]);
		}

		// Àü ¼­¹ö¿¡ ÇöÀç Ãß°¡µÈ Ä³¸¯ÅÍµé¿¡ ´ëÇÑ Á¤º¸¸¦ º¸³½´Ù.
		int nMaxClient = m_pClientManager->GetMaxClient();
		for (int i=0; i<nMaxClient; ++i)
		{
			if( i == pMsg->dwClient ) continue;
			// ¿Â¶óÀÎÀÌ°í °ÔÀÓ¼­¹ö¶ó¸é Ã¤ÆÃ¸Þ½ÃÁö¸¦ º¸³½´Ù.
			if( m_pClientManager->IsOnline((DWORD) i) == true  )
			{
				SendClient((DWORD) i, msg);
			}
		}

	}else{
		NET_UPDATE_TRACINGCHAR* msg = reinterpret_cast <NET_UPDATE_TRACINGCHAR*> (pMsg->Buffer);

		m_vecTracingData[msg->updateNum] = msg->tracingData;

		// Àü ¼­¹ö¿¡ ÇöÀç Ãß°¡µÈ Ä³¸¯ÅÍµé¿¡ ´ëÇÑ Á¤º¸¸¦ º¸³½´Ù.
		int nMaxClient = m_pClientManager->GetMaxClient();
		for (int i=0; i<nMaxClient; ++i)
		{
			// ¿Â¶óÀÎÀÌ°í °ÔÀÓ¼­¹ö¶ó¸é Ã¤ÆÃ¸Þ½ÃÁö¸¦ º¸³½´Ù.
			if( m_pClientManager->IsOnline((DWORD) i) == true &&
				m_pClientManager->GetSvrType(i) != SERVER_AGENT &&
				m_pClientManager->GetSvrType(i) != SERVER_FIELD  )
			{
				SendClient((DWORD) i, msg);
			}
		}
	}

	

}



void CSessionServer::MsgChatProcess(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

	NET_CHAT* nc = (NET_CHAT*) pMsg->Buffer;

	CConsoleMessage::GetInstance()->WriteConsole(_T("Chat message"));

	switch (nc->emType) 
	{
	case CHAT_TYPE_NORMAL: // ÀÏ¹ÝÀû Ã¤ÆÃ ¸Þ½ÃÁö
		break;
	case CHAT_TYPE_PARTY: // ÆÄÆ¼¿ø¿¡°Ô Àü´ÞµÇ´Â ¸Þ½ÃÁö
		break;
	case CHAT_TYPE_PRIVATE: // °³ÀÎÀûÀÎ ¸Þ½ÃÁö, ±Ó¸», ¼Ó»èÀÓ
		break;
	case CHAT_TYPE_GUILD: // ±æµå¿ø¿¡°Ô Àü´ÞµÇ´Â ¸Þ½ÃÁö
		break;
	// °ü¸®ÇÁ·Î±×·¥¿¡¼­ ¿À´Â ÀüÃ¼¸Þ½ÃÁö
	case CHAT_TYPE_CTRL_GLOBAL2: 
		MsgSndChatCtrlGlobal2(pMsg);
		break;
	case CHAT_TYPE_GLOBAL:		
	case CHAT_TYPE_CTRL_GLOBAL:		
		MsgSndChatCtrlGlobal(pMsg);
		break;
	default:
		break;
	}	

	int test;
	if( nc->emType == CHAT_TYPE_GLOBAL  )
	{
		test = 0;
	}
}

// °ü¸®ÇÁ·Î±×·¥¿¡¼­ ¿À´Â ÀüÃ¼ °øÁö Ã¤ÆÃ ¸Þ½ÃÁö¸¦ ÇØ´ç¼­¹ö·Î Àü¼ÛÇÑ´Ù
void CSessionServer::MsgSndChatCtrlGlobal(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

	CConsoleMessage::GetInstance()->Write(_T("Notice Message"));

	NET_CHAT_CTRL* ncc = reinterpret_cast<NET_CHAT_CTRL*> (pMsg->Buffer);    

	// Find server	
	DWORD dwClient = m_pClientManager->FindServer(ncc->szServerIP, ncc->nServicePort);
	if (dwClient == NET_ERROR) // ÇØ´ç ¼­¹ö¸¦ Ã£Áö ¸øÇÔ
	{
        CConsoleMessage::GetInstance()->Write("ERROR:Can't Find Server");
		return;
	}

	// Send Global message
	NET_CHAT_CTRL nc;
	nc.nmg.nType = NET_MSG_CHAT;
	nc.emType    = CHAT_TYPE_GLOBAL;
	::StringCchCopy(nc.szChatMsg, GLOBAL_CHAT_MSG_SIZE+1, ncc->szChatMsg);
	SendClient(dwClient , &nc);

	CConsoleMessage::GetInstance()->Write(nc.szChatMsg);    
}

// °ü¸®ÇÁ·Î±×·¥¿¡¼­ ¿À´Â ÀüÃ¼ °øÁö Ã¤ÆÃ ¸Þ½ÃÁö¸¦ ÇØ´ç¼­¹ö·Î Àü¼ÛÇÑ´Ù
void CSessionServer::MsgSndChatCtrlGlobal2(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

    CConsoleMessage::GetInstance()->Write(_T("Notice Message"));

	NET_CHAT_CTRL2* ncc = reinterpret_cast <NET_CHAT_CTRL2*> (pMsg->Buffer);
	
    // Find server	
	DWORD dwClient = m_pClientManager->FindServer(NET_SERVER_AGENT, ncc->nSvrGrp, 0, 0);	
	if (dwClient == NET_ERROR) // ÇØ´ç ¼­¹ö¸¦ Ã£Áö ¸øÇÔ
	{
		CConsoleMessage::GetInstance()->Write(_T("INFO:Can't Find Server"));
		return;
	}
	
	// Send Global message
	NET_CHAT_CTRL nc;
	nc.nmg.nType = NET_MSG_CHAT;
	nc.emType    = CHAT_TYPE_GLOBAL;
	::StringCchCopy(nc.szChatMsg, GLOBAL_CHAT_MSG_SIZE+1, ncc->szChatMsg);
	SendClient(dwClient , &nc);

	CConsoleMessage::GetInstance()->Write(nc.szChatMsg);    
}

void CSessionServer::MsgSndSvrInfoReset(DWORD dwClient)
{		
	NET_MSG_GENERIC	nmg;	
	nmg.dwSize = sizeof(NET_MSG_GENERIC);
	nmg.nType = NET_MSG_SND_SVR_INFO_RESET;
	SendClient(dwClient, &nmg);
}

/// ¼¼¼Ç¼­¹ö->·Î±×ÀÎ¼­¹ö : °ÔÀÓ¼­¹öÀÇ »óÅÂÁ¤º¸¸¦ Àü¼ÛÇÑ´Ù.
void CSessionServer::MsgSndCurSvrInfo(DWORD dwClient)
{
	// server type [type]
	// [type]
	// 1 : login server
	// 2 : session server
	// 3 : field server
	// 4 : agent server

	// LoginServerÀÇ ClientID¸¦ ¼ÂÆÃÇÑ´Ù.
	m_dwLoginServerID = dwClient;

	NET_CUR_INFO_LOGIN ncil;
	ncil.nmg.nType = NET_MSG_SND_SVR_INFO;

	for (int nGroup=0; nGroup < MAX_SERVER_GROUP; ++nGroup)
	{
		for (int nChannel=0; nChannel < MAX_CHANNEL_NUMBER; ++nChannel)
		{
			if (m_sServerChannel[nGroup][nChannel].nServerMaxClient > 0)
			{
				ncil.gscil               = m_sServerChannel[nGroup][nChannel];
				ncil.gscil.nServerNumber = nChannel; // Channel number

				SendClient(dwClient, &ncil);

				CConsoleMessage::GetInstance()->WriteConsole(
					_T("MsgSndCurSvrInfo %s"),
					m_sServerChannel[nGroup][nChannel].szServerIP );
			}
		}
	}
}

void CSessionServer::MsgSndChatGlobal(char* szChatMsg)
{
    if (szChatMsg == NULL) return;

	NET_CHAT nc;	
	nc.nmg.nType	= NET_MSG_CHAT;
	nc.emType		= CHAT_TYPE_GLOBAL;
	::StringCchCopy(nc.szChatMsg, CHAT_MSG_SIZE+1, szChatMsg);

	int nMaxClient = m_pClientManager->GetMaxClient();

	for (int i=0; i<nMaxClient; ++i)
	{
		// ¿Â¶óÀÎÀÌ°í °ÔÀÓ¼­¹ö¶ó¸é Ã¤ÆÃ¸Þ½ÃÁö¸¦ º¸³½´Ù.
		if ((m_pClientManager->IsOnline((DWORD) i) == true) && 
			(m_pClientManager->IsGameServer((DWORD) i) == true)) 
		{
			SendClient((DWORD) i, &nc);
		}
	}
}

// Session->ServerManager:¿Â¶óÀÎ »óÅÂÀÎ ¸ðµç ¼­¹öÀÇ »óÅÂÁ¤º¸¸¦ Àü¼ÛÇØ ÁØ´Ù
void CSessionServer::MsgSndAllSvrInfo(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

	DWORD dwClient = pMsg->dwClient;
	int nMax = m_pClientManager->GetMaxClient();

	// Start of server info
	NET_MSG_GENERIC nmg;
	nmg.dwSize = sizeof(NET_MSG_GENERIC);
	nmg.nType = NET_MSG_SND_ALL_SVR_INFO_S;
	SendClient(dwClient, &nmg);
	
	// server info
	NET_SERVER_INFO nsi;
	int nSize = sizeof(NET_SERVER_INFO);

	for (DWORD dwSvr=0; dwSvr<(DWORD) nMax; ++dwSvr)
	{
		if (m_pClientManager->IsOnline(dwSvr) == true)
		{	
			memset(&nsi, 0, nSize);
			nsi.nmg.dwSize	= nSize;
			nsi.nmg.nType	= NET_MSG_SND_ALL_SVR_INFO;
			nsi.gsi			= m_pClientManager->GetSvrInfo(dwSvr);
			SendClient(dwClient, &nsi);

			CConsoleMessage::GetInstance()->WriteConsole(
				_T("MsgSndAllSvrInfo %s %d %d"),
				nsi.gsi.szServerIP,
				nsi.gsi.nControlPort,
				nsi.gsi.nServerType );
		}
	}

	// End of server info	
	nmg.dwSize = sizeof(NET_MSG_GENERIC);
	nmg.nType = NET_MSG_SND_ALL_SVR_INFO_E;
	SendClient(dwClient, &nmg);


#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	// Á¢¼ÓÇÑ ¼­¹ö¿¡ ÇöÀç TracingUser Á¤º¸¸¦ Àü¼ÛÇÑ´Ù.
	if( m_pClientManager->IsOnline(dwClient) == true  )
	{
		NET_UPDATE_TRACINGCHAR_ALL msg;	

		size_t i;
		for( i = 0; i < m_vecTracingData.size(); i++ )
		{
			msg.tracingData[i] = m_vecTracingData[i];
		}
		SendClient(dwClient, &msg);
	}
#endif
}

// Agent, Field, Login -> Session
// Heartbeat
/*
void CSessionServer::MsgHeartBeat(MSG_LIST* pMsg)
{    
    if (pMsg == NULL) return;

    m_pClientManager->SetHeartBeat(pMsg->dwClient);
}
*/

void CSessionServer::SetSvrInfo(DWORD dwClient, G_SERVER_INFO *gsi)
{
	if (gsi == NULL) return;

	int nServerGroup         = gsi->nServerGroup;
	int nServerChannelNumber = gsi->nServerChannelNumber;
	int nPort                = gsi->nServicePort;

	if (gsi->nServerType == SERVER_AGENT)
	{
		for (int nChannel=0; nChannel < nServerChannelNumber; ++nChannel)
		{
			::StringCchCopy(m_sServerChannel[nServerGroup][nChannel].szServerIP, MAX_IP_LENGTH+1, gsi->szServerIP);			
			m_sServerChannel[nServerGroup][nChannel].nServicePort         = nPort;
			m_sServerChannel[nServerGroup][nChannel].nServerGroup         = nServerGroup;
			m_sServerChannel[nServerGroup][nChannel].nServerNumber        = nChannel;
			m_sServerChannel[nServerGroup][nChannel].nServerCurrentClient = gsi->nServerCurrentClient;
			m_sServerChannel[nServerGroup][nChannel].nServerMaxClient     = gsi->nServerChannellMaxClient;
			m_sServerChannel[nServerGroup][nChannel].bPK                  = gsi->bPk;
		}
	}

	m_pClientManager->SetSvrInfo(dwClient, gsi);

	CConsoleMessage::GetInstance()->WriteConsole(
		_T("INFO:ªA°È¾¹°ò¥»«H®§ %s %d %d %d %d"),
		gsi->szServerIP,
		gsi->nServicePort,
		gsi->nServerChannellMaxClient,
		gsi->nServerCurrentClient,
		gsi->nServerType );
}

// ¼­¹ö->¼¼¼Ç¼­¹ö
// ¼­¹öÀÇ Full Á¤º¸
void CSessionServer::MsgServerInfo(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

	NET_SERVER_INFO* nsi = reinterpret_cast <NET_SERVER_INFO*> (pMsg->Buffer);
	SetSvrInfo(pMsg->dwClient, &nsi->gsi);
    
	// ¼­¹ö Á¤º¸¸¦ ·Î±×ÀÎ ¼­¹ö¿¡ Àü¼ÛÇÑ´Ù.
	MsgSndCurSvrInfoToAll();

	// ÇÊµå¼­¹ö¶ó¸é
	if (nsi->gsi.nServerType == NET_SERVER_FIELD)
	{
		for (int i=0; i < m_pClientManager->GetMaxClient(); ++i)
		{
			// ÇÊµå¼­¹ö ´Ù¿î½Ã Àç¿¬°áÀÌ ÀÌ·ç¾î Áö¸é Agent ¿¡°Ô ´Ù½Ã Field ·Î ¿¬°áÇÏ¶ó°í ¾Ë·Á¾ß ÇÑ´Ù.
			if (m_pClientManager->IsOnline(i)   == true &&
				m_pClientManager->GetSvrType(i) == SERVER_AGENT &&
				m_pClientManager->GetSvrGrp(i)  == nsi->gsi.nServerGroup &&
				m_pClientManager->GetSvrNum(i)  == nsi->gsi.nServerNumber)
			{
				// Agent ¿¡ ÀÚ½Å¿¡°Ô ¿¬°áÇÏ¶ó°í ¾Ë¸°´Ù.
				NET_RECONNECT_FILED	nrf;
				SendClient(i, &nrf);
				CConsoleMessage::GetInstance()->Write(_T("INFO:Inform Agent, Reconnect To Field"));
			}
		}
	}

	// Agent¼­¹ö ¶ó¸é...
	if (nsi->gsi.nServerType == NET_SERVER_AGENT)
	{
		DWORD dwClient = pMsg->dwClient;
		if ( m_pClientManager->IsOnline(dwClient) == true )
		{
			MsgSndEncryptKey( dwClient );
		}
	}

#if defined(_RELEASED) || defined(TW_PARAM) || defined(TH_PARAM) || defined(HK_PARAM) // ***Tracing Log print
	// Á¢¼ÓÇÑ ¼­¹ö¿¡ ÇöÀç TracingUser Á¤º¸¸¦ Àü¼ÛÇÑ´Ù.
	DWORD dwClient = pMsg->dwClient;
	if( m_pClientManager->IsOnline(dwClient) == true  )
	{
		NET_UPDATE_TRACINGCHAR_ALL msg;	

		size_t i;
		for( i = 0; i < m_vecTracingData.size(); i++ )
		{
			msg.tracingData[i] = m_vecTracingData[i];
		}
		SendClient(dwClient, &msg);
	}
#endif
}

void CSessionServer::MsgEncryptKey(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_ENCRYPT_KEY* nmg = reinterpret_cast <NET_ENCRYPT_KEY*> (pMsg->Buffer);

	::StringCchCopy( m_szEncrypt, ENCRYPT_KEY+1, nmg->szEncryptKey );


	for (DWORD dwClient=0; dwClient < (DWORD) m_pClientManager->GetMaxClient(); ++dwClient)
	{
		if (m_pClientManager->IsOnline(dwClient) == true &&
			m_pClientManager->GetSvrType(dwClient) == NET_SERVER_AGENT)
		{
			MsgSndEncryptKey( dwClient );
		}
	}
}

void CSessionServer::MsgSndEncryptKey( DWORD dwClient )
{
	NET_ENCRYPT_KEY nmg;
	::StringCchCopy(nmg.szEncryptKey, ENCRYPT_KEY+1, m_szEncrypt );
	SendClient(dwClient, &nmg);
}

// ¸ðµç ¿¬°á¿¡ °ÔÀÓ¼­¹öÀÇ »óÅÂÁ¤º¸¸¦ Àü¼ÛÇÑ´Ù.
// NET_SERVER_FIELD	3
// NET_SERVER_AGENT	4
void CSessionServer::MsgSndCurSvrInfoToAll(void)
{	
	for (DWORD dwClient=0; dwClient < (DWORD) m_pClientManager->GetMaxClient(); ++dwClient)
	{
		if (m_pClientManager->IsOnline(dwClient) == true &&
			m_pClientManager->GetSvrType(dwClient) != NET_SERVER_FIELD)
		{
			MsgSndSvrInfoReset(dwClient);
			MsgSndCurSvrInfo(dwClient);
		}
	}
}

// °ÔÀÓ¼­¹ö->¼¼¼Ç¼­¹ö 
// ÇöÀç ¼­¹ö »óÅÂ Àü¼ÛÃ³¸®
void CSessionServer::MsgServerCurInfo(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

	NET_SERVER_CUR_INFO* nsci = reinterpret_cast <NET_SERVER_CUR_INFO*> (pMsg->Buffer);
	m_pClientManager->SetSvrCurInfo(pMsg->dwClient, &nsci->gsci);
}

// Agent->Session
// ÇöÀç Ã¤³Î »óÅÂÁ¤º¸
void CSessionServer::MsgServerChannelInfo(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

	NET_SERVER_CHANNEL_INFO* pNsci = reinterpret_cast <NET_SERVER_CHANNEL_INFO*> (pMsg->Buffer);
	SetChannelInfo(pNsci);
}

void CSessionServer::SetChannelInfo(NET_SERVER_CHANNEL_INFO* pMsg)
{
	if (pMsg == NULL) return;

	int nServerGroup        = pMsg->nServerGroup;
	int nChannel            = pMsg->nChannel;	
	int nChannelCurrentUser = pMsg->nChannelCurrentUser;
	int nChannelMaxUser     = pMsg->nChannelMaxUser;
	bool bPk                = pMsg->bPk;
	
	if (nChannel < 0 || nChannel >= MAX_CHANNEL_NUMBER || nServerGroup < 0 || nServerGroup >= MAX_SERVER_GROUP)
	{
		return;
	}
	else
	{
		m_sServerChannel[nServerGroup][nChannel].nServerCurrentClient = nChannelCurrentUser;
		m_sServerChannel[nServerGroup][nChannel].nServerMaxClient     =	nChannelMaxUser;
		m_sServerChannel[nServerGroup][nChannel].bPK                  = bPk;

		if( m_dwLoginServerID != -1 )
		{
			if( (float)nChannelMaxUser * 0.95f < (float)nChannelCurrentUser )
			{
				if( m_bServerChannelFull[nServerGroup][nChannel] == FALSE )
				{
					m_bServerChannelFull[nServerGroup][nChannel] = TRUE;
					// LoginServer¿¡ ¸Þ½ÃÁö¸¦ º¸³¿
					NET_SERVER_CHANNEL_FULL_INFO nmg;
					nmg.nServerGroup = nServerGroup;
					nmg.nChannel	 = nChannel;
					nmg.bChannelFull = TRUE;
					nmg.nChannelCurrentUser = nChannelCurrentUser;

					SendClient(m_dwLoginServerID, &nmg);					
				}
			}else 
			{
				if( m_bServerChannelFull[nServerGroup][nChannel] == TRUE )
				{
					m_bServerChannelFull[nServerGroup][nChannel] = FALSE;
					// LoginServer¿¡ ¸Þ½ÃÁö¸¦ º¸³¿
					NET_SERVER_CHANNEL_FULL_INFO nmg;
					nmg.nServerGroup = nServerGroup;
					nmg.nChannel	 = nChannel;
					nmg.bChannelFull = FALSE;
					nmg.nChannelCurrentUser = nChannelCurrentUser;

					SendClient(m_dwLoginServerID, &nmg);
				}			
			}
		}
	}
}

void CSessionServer::MsgReqAllSvrFullInfo(void)
{
	for (int i=0; i<m_pClientManager->GetMaxClient(); ++i)
	{
		if (m_pClientManager->IsOnline(i) == true)
		{
			MsgReqSvrFullInfo(i);
		}
	}
}

void CSessionServer::MsgReqSvrFullInfo(DWORD dwClient)
{
	NET_MSG_GENERIC		nmg;
	nmg.dwSize = sizeof(NET_MSG_GENERIC);
	nmg.nType = NET_MSG_REQ_FULL_SVR_INFO;
	SendClient(dwClient, &nmg);
}

void CSessionServer::MsgReqAllSvrCurInfo(void)
{	
	for (int i=0; i<m_pClientManager->GetMaxClient(); ++i) 
	{
		if (m_pClientManager->IsOnline(i) == true)
		{
			MsgReqSvrCurInfo(i);
		}
	}
}

void CSessionServer::MsgReqSvrCurInfo(DWORD dwClient)
{
	NET_MSG_GENERIC		nmg;
	nmg.dwSize = sizeof(NET_MSG_GENERIC);
	nmg.nType = NET_MSG_REQ_CUR_STATE;
	SendClient(dwClient, &nmg);
}

// °ÔÀÓ¼­¹ö -> ¼¼¼Ç¼­¹ö : Ä³¸¯ÅÍ°¡ °ÔÀÓ¿¡ Á¶ÀÎÇßÀ½ 
void CSessionServer::MsgGameJoinOK(MSG_LIST* pMsg)
{	
	/*
	if (pMsg == NULL) return;

	DWORD dwClient = pMsg->dwClient;
	NET_GAME_JOIN_OK* ngjo = (NET_GAME_JOIN_OK*) pMsg->Buffer;

	// CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "°ÔÀÓ¼­¹ö->¼¼¼Ç¼­¹ö:(%s)Ä³¸¯ÅÍ°¡ °ÔÀÓ¿¡ Á¶ÀÎÇßÀ½", ngjo->szChaName);
	m_pClientManager->UpdateChaJoinOK(ngjo->szUserID,
									ngjo->nUsrNum,
									ngjo->nChaNum,
									ngjo->dwGaeaID,
									ngjo->szChaName);

	// DB Update	
	CUserUpdateCha* pAction = new CUserUpdateCha(ngjo->nUsrNum, ngjo->szChaName);
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
	*/
}

// Agent->Session : Ä³¸¯ÅÍ »ý¼º, °¹¼ö °¨¼Ò
int CSessionServer::MsgChaDecrease(MSG_LIST* pMsg) 
{
	/*
	if (pMsg == NULL) return NET_ERROR;

	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);
	
	CUserChaNumDecrease* pAction = new CUserChaNumDecrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob(pAction);
	*/
	return NET_OK;
}

// Agent->Session :TEST ¼­¹ö  Ä³¸¯ÅÍ »ý¼º, °¹¼ö °¨¼Ò
int CSessionServer::MsgTestChaDecrease(MSG_LIST* pMsg)
{
	/*
	if (pMsg == NULL) return NET_ERROR;

	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);

	CUserTestChaNumDecrease* pAction = new CUserTestChaNumDecrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
	*/
	return NET_OK;
}

// DAUM:Agent->Session : Ä³¸¯ÅÍ »ý¼º, °¹¼ö °¨¼Ò
int CSessionServer::DaumMsgChaDecrease(MSG_LIST* pMsg) 
{
	/*
	if (pMsg == NULL) return NET_ERROR;

	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);
	
	CUserChaNumDecrease* pAction = new CUserChaNumDecrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob(pAction);
	*/
	return NET_OK;
}

// DAUM:Agent->Session :TEST ¼­¹ö  Ä³¸¯ÅÍ »ý¼º, °¹¼ö °¨¼Ò
int CSessionServer::DaumMsgTestChaDecrease(MSG_LIST* pMsg)
{
	/*
	if (pMsg == NULL) return NET_ERROR;
	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);
	CUserTestChaNumDecrease* pAction = new CUserTestChaNumDecrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
	*/
	return NET_OK;
}

// Agent->Session:Ä³¸¯ÅÍ »èÁ¦, °¹¼ö Áõ°¡
int CSessionServer::MsgChaIncrease(MSG_LIST* pMsg)
{
	/*
	if (pMsg == NULL) return NET_ERROR;
	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);	
	CUserChaNumIncrease* pAction = new CUserChaNumIncrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
	*/
	return NET_OK;
}

// Agent->Session:TEST ¼­¹ö Ä³¸¯ÅÍ »èÁ¦, °¹¼ö Áõ°¡
int	CSessionServer::MsgTestChaIncrease      (MSG_LIST* pMsg)
{
	/*
	if (pMsg == NULL) return NET_ERROR;
	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);	
	CUserTestChaNumIncrease* pAction = new CUserTestChaNumIncrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
	*/
	return NET_OK;
}

// DAUM:Agent->Session : Ä³¸¯ÅÍ »èÁ¦, °¹¼ö Áõ°¡
int CSessionServer::DaumMsgChaIncrease      (MSG_LIST* pMsg)
{
	/*
	if (pMsg == NULL) return NET_ERROR;
	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);	
	CUserChaNumIncrease* pAction = new CUserChaNumIncrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
	*/
	return NET_OK;
}

// DAUM:Agent->Session :TEST ¼­¹ö  Ä³¸¯ÅÍ »èÁ¦, °¹¼ö Áõ°¡
int CSessionServer::DaumMsgTestChaIncrease  (MSG_LIST* pMsg)
{
	/*
	if (pMsg == NULL) return NET_ERROR;
	NET_CHARACTER_INC_DEC* pTemp = reinterpret_cast <NET_CHARACTER_INC_DEC*> (pMsg->Buffer);	
	CUserTestChaNumIncrease* pAction = new CUserTestChaNumIncrease(pTemp->nUserNum);
	COdbcManager::GetInstance()->AddJob((CDbAction*) pAction);
	*/
	return NET_OK;
}

// Control program->Session server : command message
// NET_MSG_SVR_PAUSE	¼­¹ö¸¦ Àá½Ã ¸ØÃá´Ù.
// NET_MSG_SVR_RESUME	¸ØÃß¾îÁø ¼­¹ö¸¦ ´Ù½Ã °¡µ¿½ÃÅ²´Ù.	
// NET_MSG_SVR_RESTART	¼­¹ö¸¦ ¿ÏÀüÈ÷ ¸ØÃß°í »õ·Ó°Ô °¡µ¿½ÃÅ²´Ù.
// NET_MSG_SVR_STOP		¼­¹ö¸¦ ¿ÏÀüÈ÷ ¸ØÃá´Ù.
void CSessionServer::MsgSvrCmdProcess(MSG_LIST* pMsg)
{
    if (pMsg == NULL) return;

    NET_SVR_CMD_CHANGE_STATE* pTemp = (NET_SVR_CMD_CHANGE_STATE*) pMsg->Buffer;
	int nClient = FindServer(pTemp->nSvrType, pTemp->nSvrGrp, pTemp->nSvrNum, pTemp->nSvrField);
	CConsoleMessage::GetInstance()->Write("INFO:Server state change command %d", nClient);
	if (nClient != NET_ERROR)
	{
		switch(pTemp->nCmdType)
		{
		case NET_MSG_SVR_PAUSE : // ¼­¹ö Àá½Ã Áß´Ü
			CConsoleMessage::GetInstance()->Write("INFO:Server state change 'PAUSE' command %d", nClient);
            MsgSndServerStateChange(nClient, NET_MSG_SVR_PAUSE);
			break;
		case NET_MSG_SVR_RESUME : // ¼­¹ö ½ÃÀÛ
			CConsoleMessage::GetInstance()->Write("INFO:Server state change 'RESUME' command %d", nClient);
			MsgSndServerStateChange(nClient, NET_MSG_SVR_RESUME);
			break;
		case NET_MSG_SVR_RESTART : // ¼­¹ö Àç½ÃÀÛ
			CConsoleMessage::GetInstance()->Write("INFO:Server state change 'RESTART' command %d", nClient);
			MsgSndServerStateChange(nClient, NET_MSG_SVR_RESTART);
			break;
		case NET_MSG_SVR_STOP : // ¼­¹ö °¡µ¿ Áß´Ü
			CConsoleMessage::GetInstance()->Write("INFO:Server state change 'STOP' command %d", nClient);
			MsgSndServerStateChange(nClient, NET_MSG_SVR_STOP);
			break;
		default:
			break;
		}
	}
}

//    _.--"""""--._
//  .'             '.
// /                 \
//;                   ;
//|                   |
//|                   |
//;                   ;
// \ (`'--,    ,--'`) /
//  \ \  _ )  ( _  / /
//   ) )(')/  \(')( (
//  (_ `""` /\ `""` _)
//   \`"-, /  \ ,-"`/
//    `\ / `""` \ /`
//     |/\/\/\/\/\|
//     |\        /|
//     ; |/\/\/\| ;
//      \`-`--`-`/
//       \      /
//        ',__,'
//         q__p
//         q__p
//         q__p
//         q__p
// jgkim
