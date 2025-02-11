#ifndef DXLANDDEF_H_
#define DXLANDDEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common/SerialFile.h"
//class CSerialFile;

enum  EMLANDMAN
{
//	MAXLANDSID		= 32, 
	MAXLANDMID		= 512,	
	MAXLANDSID		= 256, // 인던 개발을 위해 SID를 늘렸다.
	MAXLANDNAME		= 128,
};

struct SLAND_FILEMARK_000
{
	DWORD	dwNAVI_MARK;	//	네비게이션 메시.
	DWORD	dwGATE_MARK;	//	출입구.
	DWORD	dwCOLL_MARK;	//	충돌맵.
};

struct SLAND_FILEMARK_100
{
	DWORD	dwNAVI_MARK;	//	네비게이션 메시.
	DWORD	dwGATE_MARK;	//	출입구.
	DWORD	dwCOLL_MARK;	//	충돌맵.
	DWORD	dwWEATHER_MARK;	//	날씨.
};

struct SLAND_FILEMARK
{
	enum { VERSION = 0x0101, };

	DWORD	dwNAVI_MARK;	//	네비게이션 메시.
	DWORD	dwWEATHER_MARK;	//	날씨.
	DWORD	dwGATE_MARK;	//	출입구.
	DWORD	dwCOLL_MARK;	//	충돌맵.
	
	SLAND_FILEMARK()
		: dwNAVI_MARK(0)
		, dwWEATHER_MARK(0)
		, dwGATE_MARK(0)
		, dwCOLL_MARK(0)
	{
	}

	void Assign( SLAND_FILEMARK_000 &sOldMark );
	void Assign( SLAND_FILEMARK_100 &sOldMark );

	BOOL LoadSet( CSerialFile &SFile );
	BOOL SaveSet( CSerialFile &SFile );
};

#endif // DXLANDDEF_H_