///////////////////////////////////////////////////////////////////////
//	SNpcTalk
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//
//
///////////////////////////////////////////////////////////////////////

#pragma	once

#include <string>
#include "basestream.h"

#include "NpcTalkData.h"
#include "GLDefine.h"

#define MAX_NEEDITEM_COUNT 5
#define MAX_QUEST_START	5
#define MAX_RANDOM_PAGE	5

class CSerialFile;
struct SNpcTalkCondition;
struct GLCHARLOGIC;

struct SNpcTalk
{
public:
	enum { VERSION = 0x0200, };

	enum EM_TALK
	{
		EM_DO_NOTHING = 0,
		EM_PAGE_MOVE,		//	대화 진행.
		EM_BASIC,			//	기본 기능. ( 창고, 상점, 치료, 시작위치, stats리셋, 아이템교환 )
		EM_QUEST_START,		//	퀘스트 시작.
		EM_QUEST_STEP,		//	퀘스트 진행.

		EM_TALK_SIZE
	};

	enum EM_BASIC
	{
		EM_STORAGE = 0,		//	창고.
		EM_MARKET,			//	상점.
		EM_CURE,			//	치료.
		EM_STARTPOINT,		//	시작위치 지정.
		EM_CHAR_RESET,		//	stats, skill 리셋.
		EM_ITEM_TRADE,		//	item 교환.
		EM_BUSSTATION,		//	버스 정류장.

		EM_CLUB_NEW,		//	클럽 생성.
		EM_CLUB_UP,			//	클럽 랭크 업.

		EM_CD_CERTIFY,		//	시디 인증하기.
		EM_COMMISSION,		//	상업 수수료 설정.
		EM_CLUB_STORAGE,	//	클럽 창고.

		EM_ITEM_REBUILD,	//	아이템 개조.	// ITEMREBUILD_MARK
		EM_ODDEVEN,			//	홀짝게임
		EM_RECOVERY_EXP,	//	경험치 회복
		EM_RANDOM_PAGE,		//  임의대화
		EM_ITEMSEARCH_PAGE,	//  아이템 검색
		EM_ATTENDANCE_BOOK,	//	출석부
		EM_ITEM_MIX,

		//create reserve for official types
		EM_RESERVE_00,
		EM_RESERVE_01,
		EM_RESERVE_02,
		EM_RESERVE_03,
		EM_RESERVE_04,
		EM_RESERVE_05,
		EM_RESERVE_06,
		EM_RESERVE_07,
		EM_RESERVE_08,
		EM_RESERVE_09,

		EM_NPC_SHOP,		/*npc shop, Juver, 2017/07/25 */
		EM_ITEM_EXCHANGE,	/*item exchange, Juver, 2017/10/11 */

		EM_BASIC_SIZE
	};

public:
	SNpcTalk ();
	~SNpcTalk ();

public:
	static	std::string	strACTIONTYPE[EM_TALK_SIZE];
	static	std::string	szBASICDESC[EM_BASIC_SIZE];

	DWORD				m_dwNID;			//	대화 ID
	DWORD				m_dwGLOB_ID;		//	전체 목록에서 ID.
	std::string			m_strTalk;			//	기본 대화
	SNpcTalkCondition*	m_pCondition;		//	행동조건.
	int					m_nACTION;			//	액션
	DWORD				m_dwACTION_NO;		//	액션 행위시 선택. ( 다음 대화, 기본 기능 종류 )
	DWORD				m_dwACTION_PARAM1;	//	파라메타 1.	아이템 교환 A, 진행할 퀘스트ID  , 
	DWORD				m_dwACTION_PARAM2;	//	파라메타 2.	아이템 교환 B, 진행할 퀘스트STEP, 
    SNPC_ITEM			m_sNeedItem[MAX_NEEDITEM_COUNT];
	SNPC_ITEM			m_sResultItem[MAX_NEEDITEM_COUNT];
	DWORD				m_dwQuestStartID[MAX_QUEST_START];// 시작할 퀘스트 ID
	DWORD				m_dwRandomPageID[MAX_RANDOM_PAGE];
	DWORD				m_dwRandomTime;
	std::string			m_strBusFile;		// 버스스테이션 파일
	SNATIVEID			m_sHireSummon;

public:	
	BOOL SetCondition ( const SNpcTalkCondition &sCondition );
	void ResetCondition ();

	void SetGlobID ( DWORD dwID );

	BOOL DoTEST ( GLCHARLOGIC *pCHARLOGIC, DWORD dwIndex = UINT_MAX );

	const char* GetTalk ()	{ return m_strTalk.c_str(); }
	SNpcTalkCondition* GetCondition () const;

	void SetTalk ( const std::string &strTalk );

	void Init ();

	

    SNpcTalk& operator= ( const SNpcTalk& rNpcTalk );

protected:
	BOOL LOAD_0001 ( basestream& SFile );
	BOOL LOAD_0002 ( basestream& SFile );
	BOOL LOAD_0003 ( basestream& SFile );
	BOOL LOAD_0004 ( basestream& SFile );
	BOOL LOAD_0005 ( basestream& SFile );
	BOOL LOAD_0006 ( basestream& SFile );
	BOOL LOAD_0007 ( basestream& SFile );
	BOOL LOAD_0008 ( basestream& SFile );
	BOOL LOAD_0009 ( basestream& SFile );
	/*dmk14 ver10 npc loader*/
	BOOL LOAD_0010 ( basestream& SFile );
	BOOL LOAD_0200 ( basestream& SFile );

public:
	BOOL SAVE ( CSerialFile& SFile );
	BOOL LOAD ( basestream& SFile );
};
