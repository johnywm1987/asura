/*!
 * \file CharacterInfoViewPageStatsRender.h
 *
 * \author Juver
 * \date November 2017
 *
 * 
 */

#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"
#include "DxSkinChar.h"
#include "GLItem.h"
#include "GLCharData.h"

class	CCharacterWindowCharRender : public CUIGroup
{

private:
	enum
	{
	};

public:
	CCharacterWindowCharRender ();
	virtual	~CCharacterWindowCharRender ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );

public:
	DxSkinChar*			m_pSkinChar;
	D3DXMATRIX			m_matTrans;
	WORD				m_wLastFace;
	WORD				m_wLastHair;
	float				m_fDelay;
	D3DXVECTOR3			m_vRot;

public:
	void	CreateModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void	RenderModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void	UpdateSuit( LPDIRECT3DDEVICEQ pd3dDevice );
	void	ClearRender();

};