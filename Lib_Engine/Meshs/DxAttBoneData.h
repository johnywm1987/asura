/*!
 * \file DxAttBoneData.h
 *
 * \author Juver
 * \date May 2017
 * ABL system
 * 
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "DxSkinAniMan.h"
#include "DxSkinPieceContainer.h"
#include "basestream.h"

#define FG_BONEFILE_CHECK	(0x0001)
#define FG_MUSTNEWLOAD		(0x0002)

class DxAttBoneData : public DxSkinAniControl
{
public:
	enum 
	{ 
		VERSION_ABF		= 0x0200,
		VERSION_ENCRYPT	= 0x0200,
	};

	typedef std::vector<std::string>	VECANIFILE;
	typedef VECANIFILE::iterator		VECANIFILE_ITER;

private:
	enum		{ FILE_LENGTH = 64 };
	char		m_szFileName[FILE_LENGTH];

	PDXSKINPIECE		m_pPieceArray[PIECE_SIZE];

public:
	std::string			m_strPIECE[PIECE_SIZE];
	VECANIFILE			m_vecANIFILE;

	DWORD				m_dwRef;

	BOOL				m_bWorldObj;
	float				m_fScale;
	D3DXVECTOR3			m_vMax, m_vMin;

	float				m_fHeight;
	float				m_fRadius;

public:
	const char* GetFileName()		{ return m_szFileName; }

protected:
	BOOL LOAD_Ver_0100_0102( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice );
	BOOL LOAD_Ver_0200( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice );

public:
	BOOL LoadFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, bool bREPRESH=false);
	BOOL SaveFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice );

protected:
	 HRESULT CalculateBoundingBox ( LPDIRECT3DDEVICEQ pd3dDevice, D3DXVECTOR3 &vMax, D3DXVECTOR3 &vMin );

public:
	HRESULT SetPiece ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, DWORD dwFlag = 0x0 );
	void ResetPiece ( LPDIRECT3DDEVICEQ pd3dDevice, int i );
	PDXSKINPIECE GetPiece ( int i )		{ return m_pPieceArray[i]; }
	void SetRadiusHeight();
	void RenderAABBBox( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT ClearAll ();

public:
	DxAttBoneData ();
	~DxAttBoneData ();
};

class DxAttBoneDataContainer
{
private:
	typedef std::map<std::string,DxAttBoneData*>			ATTBONEDATAMAP;
	typedef std::map<std::string,DxAttBoneData*>::iterator	ATTBONEDATAMAP_ITER;

protected:
	char			m_szPath[MAX_PATH];

	ATTBONEDATAMAP	m_mapAttBoneData;

public:
	void			SetPath ( char* szPath ) { StringCchCopy( m_szPath, MAX_PATH, szPath ); }
	char*			GetPath ()	{ return m_szPath; }

public:
	DxAttBoneData* FindData ( const char* szFile );
	DxAttBoneData* LoadData ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread );

	void ReleaseData ( const char* szFile );
	BOOL DeleteData ( const char* szFile );

	HRESULT CleanUp ();

protected:
	DxAttBoneDataContainer ();

public:
	~DxAttBoneDataContainer ();

public:
	static DxAttBoneDataContainer& GetInstance();
};

