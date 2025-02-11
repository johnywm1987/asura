//	기본 정보 표시
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.11.21]
//			@ 작성
//

#pragma	once

#include "../../../EngineLib/GUInterface/UIWindow.h"

class	COuterInterface;
class	CBasicTextBox;

class CUIOuterWindow : public CUIWindow
{
public:
	CUIOuterWindow(COuterInterface* pInterface, EngineDeviceMan* pEngineDevice);
	virtual	~CUIOuterWindow ();

public:
	virtual void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

public:
	void	CreateBaseWidnow ( char* szWindowKeyword, char* szWindowName );

private:
	void	ResizeControl ( char* szWindowKeyword );

protected:
	CUIControl*		CreateControl ( char* szControl );
	CBasicTextBox*	CreateStaticControl (const char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );

protected:
	COuterInterface* m_pInterface;

};