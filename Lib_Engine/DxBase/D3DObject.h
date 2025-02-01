/**
 * \date	2011/01/24
 * \author	Jun-Hyuk Choi
 * \brief   EngineLib/DxTools/DxObject.h ���Ͽ� ���� Ŭ������ ���� �ϴµ�,
 *          �������̽��μ��� ������ �Ϸ��� ���� �� ������,
 *          ������ Ȱ������ �����Ƿ� �����ϴ� ���� ���� �� ����.
 */
#pragma once

class CD3DObject
{
public:
    CD3DObject() {}
    virtual ~CD3DObject() {}

public:
    virtual HRESULT OnDeviceCreated(
        IDirect3DDevice9* pd3dDevice,
        const D3DSURFACE_DESC* pBackBufferSurfaceDesc ) { return S_OK; }

    virtual VOID OnDeviceDestroyed() {}

    virtual HRESULT OnDeviceReset(
        IDirect3DDevice9* pd3dDevice,
        const D3DSURFACE_DESC* pBackBufferSurfaceDesc ) { return S_OK; }

    virtual VOID OnDeviceLost() {}

    virtual VOID OnFrameMove(
        IDirect3DDevice9* pd3dDevice,
        DOUBLE fTime,
        FLOAT fElapsedTime ) {}

    virtual VOID OnFrameRender(
        IDirect3DDevice9* pd3dDevice,
        DOUBLE fTime,
        FLOAT fElapsedTime ) {}

public:
    virtual VOID SetVisible( BOOL bVisible ) {}
    virtual VOID SetActive( BOOL bActive ) {}

public:
    virtual BOOL IsVisible() { return FALSE; }
    virtual BOOL IsActive() { return FALSE; }

    virtual CRect GetSourceRect() { return CRect( 0, 0, 0, 0 ); }
    virtual CRect GetDestRect() { return CRect( 0, 0, 0, 0 ); }
    virtual CRect GetHitRect() { return CRect( 0, 0, 0, 0 ); }

    virtual HWND GetTargetWnd() { return NULL; }
};