/**
 * \date	2012/06/01
 * \author	Jun-Hyuk Choi
 * brief	CRenderDevice Ŭ������ DxResponseMan ���� �������� ����
 *			DxResponseMan ��ġ��� �����Ƽ�..
 */
#pragma once

#include "../ServiceProviderDefine.h"
#include "../DxFontDefine.h"
#include "./D3DObject.h"

class EngineDeviceMan;
class RANPATH;

class CCustomDevice : public CD3DObject
{
protected:
   // EngineDeviceMan* m_pEngineDevice;
	CString m_strPath;
	CWnd* m_pParent;
	CString m_strFontType;
	language::LANGFLAG m_emLangSet;
	CString m_strGDIFont;

public:
    CCustomDevice(
		EMSERVICE_PROVIDER eProvider,
		RANPATH* pSubPath,
		const CString& strPath,
		CWnd* pParent,
		const CString& strFontType,
		language::LANGFLAG emLangSet,
		const CString& strGDIFont );

    virtual ~CCustomDevice();

public:
    virtual HRESULT OnDeviceCreated(
        IDirect3DDevice9* pd3dDevice,
        const D3DSURFACE_DESC* pBackBufferSurfaceDesc );

    virtual VOID OnDeviceDestroyed();

    virtual HRESULT OnDeviceReset(
        IDirect3DDevice9* pd3dDevice,
        const D3DSURFACE_DESC* pBackBufferSurfaceDesc );

    virtual VOID OnDeviceLost();

    virtual VOID OnFrameMove(
        IDirect3DDevice9* pd3dDevice,
        DOUBLE fTime,
        FLOAT fElapsedTime );

    virtual VOID OnFrameRender(
        IDirect3DDevice9* pd3dDevice,
        DOUBLE fTime,
        FLOAT fElapsedTime );
};

extern CCustomDevice* g_pCustomDevice;