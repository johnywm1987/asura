#include "StdAfx.h"
#include "OuterInterface.h"
#include "ModalWindow.h"
#include "GameTextControl.h"
#include "DxGlobalStage.h"
#include "RanParam.h"

#include "SelectServerPage.h"
#include "LoginPage.h"
#include "CharacterCreatePage.h"
#include "CharacterCreateInfo.h"
#include "CharacterCreateRotate.h"
#include "SelectCharacterPage.h"
#include "WaitDialogue.h"
#include "SecPassCheckPage.h"
#include "SecPassSetPage.h"

/*dmk14 ingame web*/
#include "ForgotPassPage.h"
#include "ForgotPassChangePage.h"
#include "SelectCharacterChangePass.h"
#include "SelectCharacterChangePin.h"
#include "SelectCharacterResetPin.h"
#include "SelectCharacterTopUp.h"
#include "SelectCharacterChangeMail.h"
#include "GameTimeConvertUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void COuterInterface::ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg )
{
	switch ( nCallerID )
	{
	case OUTER_MODAL_CLOSEGAME:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
                DxGlobalStage::GetInstance().CloseGame();
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				/*login port, Juver, 2017/11/16 */
				if ( DxGlobalStage::GetInstance().GetNetClient()->ConnectLoginServer(RANPARAM::LoginAddress, RANPARAM::nLoginPort) > NET_ERROR )
				{
 					// 접속 성공
					DxGlobalStage::GetInstance().GetNetClient()->SndReqServerInfo();				
				}
				else
				{
					//접속 실패
					DoModalOuter ( ID2GAMEEXTEXT ( "SERVERSTAGE_3" ), MODAL_INFOMATION, OKCANCEL, OUTER_MODAL_CLOSEGAME );
				}
			}
		}
		break;

	case OUTER_MODAL_WAITSERVERINFO:
		{
			CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
			if ( pNetClient->IsOnline() )		//네트워크 연결이 성공한 경우
			{
				if ( pNetClient->IsGameSvrInfoEnd() )
				{
					HideGroup ( MODAL_WINDOW_OUTER );
					return ;
				}
			}

			if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				DoModalOuter ( ID2GAMEEXTEXT ( "SERVERSTAGE_3" ), MODAL_INFOMATION, OKCANCEL, OUTER_MODAL_CLOSEGAME );				
			}
		}
		break;

	case OUTER_MODAL_RECONNECT:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ToSelectServerPage( GetModalCallWindowID() );
			}
		}
		break;

	case OUTER_MODAL_WAITCONFIRM:
		{
			if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				DxGlobalStage::GetInstance().GetNetClient()->CloseConnect();
				ToSelectServerPage( GetModalCallWindowID() );
			}
		}	
		break;

	case OUTER_MODAL_ALREADYCONNECT:
	case OUTER_MODAL_CREATEFAILED:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				DxGlobalStage::GetInstance().CloseGame();
			}
		}
		break;

	case OUTER_MODAL_NON_PK_CHANNEL:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE ) )			HideGroup ( SELECT_CHARACTER_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_PAGE ) )			HideGroup ( CHARACTER_CREATE_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_INFO ) )			HideGroup ( CHARACTER_CREATE_INFO );
				if ( IsVisibleGroup ( CHARACTER_CREATE_ROTATE ) )		HideGroup ( CHARACTER_CREATE_ROTATE );
				/*dmk14 ingame web*/
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ); 
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_RESETPIN ))		HideGroup ( SELECT_CHARACTER_PAGE_RESETPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_TOPUP ))		HideGroup ( SELECT_CHARACTER_PAGE_TOPUP );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI)) HideGroup (SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI);
				//	Note : 통신용 모듈종료
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
				if ( pNetClient->IsOnline() == true )
					pNetClient->CloseConnect ();

				COuterInterface::GetInstance().ToSelectServerPage ( SELECT_SERVER_PAGE );
			}
		}
		break;

	case OUTER_MODAL_SECONDPASSWORD:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEEXTEXT("WAITSERVER_DEL_CHARACTER"), WAITSERVER_DEL_CHARACTER, 10.0f );
			}
		}
		break;

	case OUTER_MODAL_LOGOUT:
	case OUTER_MODAL_CONNECTCLOSED:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//	Note : 통신용 모듈종료
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
				if ( pNetClient->IsOnline() == true )
				{
					pNetClient->CloseConnect ();
					COuterInterface::GetInstance().SetCancelToLogin();
				}

				//				if ( pNetClient->ConnectLoginServer(RANPARAM::LoginAddress) <= NET_ERROR )
				//					CDebugSet::ToListView ( "CCharacterStage::Logout ConnectLoginServer NET_ERROR" );
				//				else
				//					CDebugSet::ToListView ( "CCharacterStage::Logout ConnectLoginServer NET_OK" );				

				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE ) )			HideGroup ( SELECT_CHARACTER_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_PAGE ) )			HideGroup ( CHARACTER_CREATE_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_INFO ) )			HideGroup ( CHARACTER_CREATE_INFO );
				if ( IsVisibleGroup ( CHARACTER_CREATE_ROTATE ) )		HideGroup ( CHARACTER_CREATE_ROTATE );

				/*dmk14 ingame web*/
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ); 
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_RESETPIN ))		HideGroup ( SELECT_CHARACTER_PAGE_RESETPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_TOPUP ))		HideGroup ( SELECT_CHARACTER_PAGE_TOPUP );
				if ( IsVisibleGroup (SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI))	HideGroup( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI );

				if( IsVisibleGroup( SELECT_CHINA_AREA_PAGE ) )			
				{
					ToChinaAreaPage( SELECT_CHINA_AREA_PAGE );
				}
				else
				{
					ToSelectServerPage ( SELECT_CHARACTER_PAGE );
				}
			}
		}
		break;

	case OUTER_MODAL_CREATESUCCEED:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ToSelectCharacterPage ( CHARACTER_CREATE_PAGE );
			}
		}
		break;

	case OUTER_MODAL_GENERAL_LOGINFAIL:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				if( RANPARAM::VALIDIDPARAM () )
				{
					UIGUID uiGUID = GetModalCallWindowID();

					if( uiGUID == SECPASS_SETPAGE )
						COuterInterface::GetInstance().GetSecPassSetPage()->ResetAll();
					else if( uiGUID == SECPASS_CHECKPAGE )
						COuterInterface::GetInstance().GetSecPassCheckPage()->ResetAll();
					else
						COuterInterface::GetInstance().ToSelectServerPage ( SELECT_SERVER_PAGE );
				}
				else
				{
					//COuterInterface::GetInstance().ToLoginPage ( LOGIN_PAGE );
					COuterInterface::GetInstance().GetLoginPage()->ResetAll();
				}
			}
		}
		break;

	/*dmk14 ingame web*/
	case OUTER_MODAL_GENERAL_FORGOTPASS_FAIL:
		{
			
			if( dwMsg & UIMSG_MODAL_OK )
			{
				COuterInterface::GetInstance().GetForgotPassPage()->ResetAll();
				COuterInterface::GetInstance().ToSelectServerPage ( FORGOTPASS_PAGE );
			}
			
		}
		break;
	case OUTER_MODAL_GENERAL_FORGOTPASS_PROCEED:
		{
			
			if( dwMsg & UIMSG_MODAL_OK )
			{
				CString szUser = "Change Pass: ";
				szUser += COuterInterface::GetInstance().GetForgotPassPage()->GetUser();
				COuterInterface::GetInstance().GetForgotPassPage()->ResetAll();
				COuterInterface::GetInstance().GetForgotPassChangePage()->SetTitleName( szUser );
				COuterInterface::GetInstance().ToForgotPassChangePage ( FORGOTPASS_PAGE );
			}
			
		}
		break;
	case OUTER_MODAL_GENERAL_CHANGEPASS_FAIL:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				COuterInterface::GetInstance().GetForgotPassChangePage()->ResetAll();
				COuterInterface::GetInstance().ToSelectServerPage ( FORGOTPASSCHANGE_PAGE );
			}
		}
		break;

	case OUTER_MODAL_GENERAL_CHANGEPASS_SUCCESS:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				COuterInterface::GetInstance().GetForgotPassChangePage()->ResetAll();
				COuterInterface::GetInstance().ToSelectServerPage ( FORGOTPASSCHANGE_PAGE );
			}
		}
		break;
	case OUTER_MODAL_GENERAL_SELCHAR_CHANGEPASS_FAIL:
	case OUTER_MODAL_GENERAL_SELCHAR_CHANGEPASS_SUCCESS:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ); 
				COuterInterface::GetInstance().GetSelectCharacterChangePass()->ResetAll();
			}
		}
		break;

	case OUTER_MODAL_GENERAL_SELCHAR_CHANGEPIN_FAIL:
	case OUTER_MODAL_GENERAL_SELCHAR_CHANGEPIN_SUCCESS:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN );
				COuterInterface::GetInstance().GetSelectCharacterChangePin()->ResetAll();
			}
		}
		break;

	case OUTER_MODAL_GENERAL_SELCHAR_RESETPIN_FAIL:
	case OUTER_MODAL_GENERAL_SELCHAR_RESETPIN_SUCCESS:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_RESETPIN ))		HideGroup ( SELECT_CHARACTER_PAGE_RESETPIN );
				COuterInterface::GetInstance().GetSelectCharacterResetPin()->ResetAll();
			}
		}
		break;
	
	case OUTER_MODAL_GENERAL_SELCHAR_TOPUP_FAIL:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_TOPUP ))		HideGroup ( SELECT_CHARACTER_PAGE_TOPUP );
				COuterInterface::GetInstance().GetSelectCharacterTopUp()->ResetAll();
			}
		}
		break;
	case OUTER_MODAL_GENERAL_SELCHAR_TOPUP_SUCCESS:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
				if ( pNetClient->IsOnline() == true )
				{
					pNetClient->CloseConnect ();
					COuterInterface::GetInstance().SetCancelToLogin();
				}

				COuterInterface::GetInstance().GetSelectCharacterTopUp()->ResetAll();

				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE ) )						HideGroup ( SELECT_CHARACTER_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_PAGE ) )						HideGroup ( CHARACTER_CREATE_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_INFO ) )						HideGroup ( CHARACTER_CREATE_INFO );
				if ( IsVisibleGroup ( CHARACTER_CREATE_ROTATE ) )					HideGroup ( CHARACTER_CREATE_ROTATE );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ); 
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_RESETPIN ))				HideGroup ( SELECT_CHARACTER_PAGE_RESETPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_TOPUP ))				HideGroup ( SELECT_CHARACTER_PAGE_TOPUP );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI ))	HideGroup( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI );

				if( IsVisibleGroup( SELECT_CHINA_AREA_PAGE ) )			
				{
					ToChinaAreaPage( SELECT_CHINA_AREA_PAGE );
				}
				else
				{
					ToSelectServerPage ( SELECT_CHARACTER_PAGE );
				}
			}
		}
		break;
	case OUTER_MODAL_GENERAL_SELCHAR_CHANGEMAIL_FAIL:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL ))	HideGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL );
				COuterInterface::GetInstance().GetSelectCharacterChangeMail()->ResetAll();
			}
		}
		break;
	case OUTER_MODAL_GENERAL_SELCHAR_CHANGEMAIL_SUCCESS:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
				if ( pNetClient->IsOnline() == true )
				{
					pNetClient->CloseConnect ();
					COuterInterface::GetInstance().SetCancelToLogin();
				}

				COuterInterface::GetInstance().GetSelectCharacterChangeMail()->ResetAll();
				
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE ) )						HideGroup ( SELECT_CHARACTER_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_PAGE ) )						HideGroup ( CHARACTER_CREATE_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_INFO ) )						HideGroup ( CHARACTER_CREATE_INFO );
				if ( IsVisibleGroup ( CHARACTER_CREATE_ROTATE ) )					HideGroup ( CHARACTER_CREATE_ROTATE );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ); 
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_RESETPIN ))				HideGroup ( SELECT_CHARACTER_PAGE_RESETPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_TOPUP ))				HideGroup ( SELECT_CHARACTER_PAGE_TOPUP );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI ))	HideGroup( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI );

				if( IsVisibleGroup( SELECT_CHINA_AREA_PAGE ) )			
				{
					ToChinaAreaPage( SELECT_CHINA_AREA_PAGE );
				}
				else
				{
					ToSelectServerPage ( SELECT_CHARACTER_PAGE );
				}
			}
		}
		break;
	case OUTER_MODAL_GENERAL_GAMETIME_CONVERT_FAIL:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				if ( IsVisibleGroup (SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI))	HideGroup( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI );
				COuterInterface::GetInstance().GetGameTimeConvertUI()->ResetAll();
			}
		}
		break;
	case OUTER_MODAL_GENERAL_GAMETIME_CONVERT_SUCCESS:
		{
			if( dwMsg & UIMSG_MODAL_OK )
			{
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
				if ( pNetClient->IsOnline() == true )
				{
					pNetClient->CloseConnect ();
					COuterInterface::GetInstance().SetCancelToLogin();
				}
				
				COuterInterface::GetInstance().GetGameTimeConvertUI()->ResetAll();
				
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE ) )						HideGroup ( SELECT_CHARACTER_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_PAGE ) )						HideGroup ( CHARACTER_CREATE_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_INFO ) )						HideGroup ( CHARACTER_CREATE_INFO );
				if ( IsVisibleGroup ( CHARACTER_CREATE_ROTATE ) )					HideGroup ( CHARACTER_CREATE_ROTATE );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ); 
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_RESETPIN ))				HideGroup ( SELECT_CHARACTER_PAGE_RESETPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_TOPUP ))				HideGroup ( SELECT_CHARACTER_PAGE_TOPUP );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI ))	HideGroup( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI );

				if( IsVisibleGroup( SELECT_CHINA_AREA_PAGE ) )			
				{
					ToChinaAreaPage( SELECT_CHINA_AREA_PAGE );
				}
				else
				{
					ToSelectServerPage ( SELECT_CHARACTER_PAGE );
				}
			}
		}
		break;
		/*dmk14 iname web end*/

	case OUTER_MODAL_LOGIN_WAIT_DAUM:
		{
			//	NOTE
			//		서버 접속후 복호 키를 받을 때까지 대기한다.
			//		
			CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
			if ( pNetClient->IsKeyReceived () )
			{
				CString strTemp;

				//	컨트롤로 부터 아이디와 암호를 가져와 서버에 전송
				switch ( RANPARAM::emSERVICE_TYPE )
				{
				case EMSERVICE_KOREA:
					pNetClient->DaumSndLogin ( RANPARAM::GETIDPARAM(), m_nServerChannel );
					break;

				//case EMSERVICE_GSP:
				//	pNetClient->GspSndLogin ( RANPARAM::GETIDPARAM(), m_nServerChannel );
				//	break;

				case EMSERVICE_MALAYSIA_CN:
				case EMSERVICE_MALAYSIA_EN:
				case EMSERVICE_PHILIPPINES:
				case EMSERVICE_VIETNAM:
					pNetClient->TerraSndLogin ( RANPARAM::GETIDPARAM(), m_nServerChannel );
					break;

//		Japan 로그인 방식 변경
//				case EMSERVICE_JAPAN:
//					pNetClient->ExciteSndLogin ( RANPARAM::GETIDPARAM(), RANPARAM::GETIDPARAM2(), RANPARAM::GETIDPARAM3(), m_nServerChannel );
//					break;

				default:
					CString strTemp;
					strTemp.Format( "OUTER_MODAL_LOGIN_WAIT_DAUM::default:ServiceType:%d", RANPARAM::emSERVICE_TYPE );
					AfxMessageBox( strTemp.GetString() );
					break;
				};

                START_WAIT_TIME ( 10.0f );
				DoModalOuter ( ID2GAMEEXTEXT ("DAUM_LOGIN_WAIT"), MODAL_INFOMATION, CANCEL, OUTER_MODAL_LOGIN_WAIT_DAUM_AFTER_KEYRECEIVED );
			}
			else			
			{
				//	NOTE
				//		시간이 초과된 경우
				//		아래의 처리없이 바로 서버가 꽉 찼다는 메시지를 뿌린다.
				if ( GET_WAIT_TIME() < 0.0f )
				{
					DoModalOuter ( ID2GAMEEXTEXT ("DAUM_LOGIN_WAIT_SERVERFULL"), MODAL_INFOMATION, YESNO, OUTER_MODAL_LOGIN_WAIT_DAUM_SERVERFULL );
				}
			}
		}
		break;

	case OUTER_MODAL_LOGIN_WAIT_DAUM_AFTER_KEYRECEIVED:
		{
			//	NOTE
			//		시간이 초과된 경우
			//		아래의 처리없이 바로 서버가 꽉 찼다는 메시지를 뿌린다.
			if ( GET_WAIT_TIME() < 0.0f )
			{
				DoModalOuter ( ID2GAMEEXTEXT ("DAUM_LOGIN_WAIT_SERVERFULL"), MODAL_INFOMATION, YESNO, OUTER_MODAL_LOGIN_WAIT_DAUM_SERVERFULL );
			}
		}
		break;

	case OUTER_MODAL_LOGIN_WAIT_DAUM_SERVERFULL:
		{
			//	NOTE
			//		다시 시도해보기
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GetSelectServerPage()->Login ();
			}
		}
		break;

	case OUTER_MODAL_CHARACTERSTAGE_GAME_JOIN_WAIT:
		{
			if( dwMsg & UIMSG_MODAL_CANCEL )
			{
				DxGlobalStage::GetInstance().SetGameJoin( FALSE );
				//	Note : 통신용 모듈종료
				CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
				if ( pNetClient->IsOnline() == true )
				{
					pNetClient->CloseConnect ();
					COuterInterface::GetInstance().SetCancelToLogin();
				}

				//				if ( pNetClient->ConnectLoginServer(RANPARAM::LoginAddress) <= NET_ERROR )
				//					CDebugSet::ToListView ( "CCharacterStage::Logout ConnectLoginServer NET_ERROR" );
				//				else
				//					CDebugSet::ToListView ( "CCharacterStage::Logout ConnectLoginServer NET_OK" );				

				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE ) )			HideGroup ( SELECT_CHARACTER_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_PAGE ) )			HideGroup ( CHARACTER_CREATE_PAGE );
				if ( IsVisibleGroup ( CHARACTER_CREATE_INFO ) )			HideGroup ( CHARACTER_CREATE_INFO );
				if ( IsVisibleGroup ( CHARACTER_CREATE_ROTATE ) )		HideGroup ( CHARACTER_CREATE_ROTATE );
				/*dmk14 ingame web*/
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPASS ); 
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL ))			HideGroup ( SELECT_CHARACTER_PAGE_CHANGEMAIL );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_RESETPIN ))				HideGroup ( SELECT_CHARACTER_PAGE_RESETPIN );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_TOPUP ))				HideGroup ( SELECT_CHARACTER_PAGE_TOPUP );
				if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI ))	HideGroup ( SELECT_CHARACTER_PAGE_GAMETIMECONVERTUI );

				if( IsVisibleGroup( SELECT_CHINA_AREA_PAGE ) )			
				{
					ToChinaAreaPage( SELECT_CHINA_AREA_PAGE );
				}
				else
				{
					ToSelectServerPage ( SELECT_CHARACTER_PAGE );
				}
			}
			else if ( GET_WAIT_TIME() < 0.0f )
			{
				//	NOTE
				//		시간이 초과된 경우
				//		아래의 처리없이 바로 서버가 꽉 찼다는 메시지를 뿌린다.
				DoModalOuter ( ID2GAMEEXTEXT ("CHARACTERSTAGE_DAUM_GAME_JOIN_FAIL"), MODAL_INFOMATION, OK, OUTER_MODAL_CONNECTCLOSED );
			}
		}
		break;

	case OUTER_MODAL_NAME_ERROR:
	case OUTER_MODAL_THAI_CHECK_STRING_ERROR:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GetCharacterCreatePage()->ResetNameEdit ();
			}
		}
		break;
	}
}