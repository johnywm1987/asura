#pragma once

#include "../Util/UIGroupHelper.h"

class CInnerInterface;
class EngineDeviceMan;

class CBasicTextBox;

class CMoveableTextBox : public CUIGroupHelper
{
public:
	enum EMOVEABLE_TEXT_TYPE
	{
		MOVE_LEFT_TYPE,
		MOVE_RIGHT_TYPE
	};

public:
	CMoveableTextBox( CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice );
	virtual ~CMoveableTextBox();

public:
	virtual void CreateSub( CUIControl* pParent, const char* szControlKeyword, WORD wAlignFlag = UI_FLAG_DEFAULT, UIGUID WndID = NO_ID );
	virtual void CreateSubControl( const char* szControlKeyword, CD3DFontPar* pFont, int nAlign );
	virtual void SetVisibleSingle( BOOL bVisible );

protected:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

public:
	// �ؽ�Ʈ ���� ( ���õ� �ؽ�Ʈ�� ���� �ִϸ��̼ǿ� ���ŵȴ� );
	void SetText( const std::string& text );
	void SetTextColor( const D3DCOLOR& textColor );
	void SetTextSpeed( float textSpeed );
	void SetMoveableType( EMOVEABLE_TEXT_TYPE type );

	// �ؽ�Ʈ �ִϸ��̼��� �ʱ�ȭ�ϰ� �ؽ�Ʈ�� �ٷ� �����Ѵ�;
	void SetTextWithResetAnimation( const std::string& text );

	// �ؽ�Ʈ �ִϸ��̼� �ʱ�ȭ;
	// �ٷ� �����ϰ� ���� �� �ִϸ��̼��� �ʱ�ȭ ���ش�;
	void ResetTextAnimation();

	// �ִϸ��̼� ó��;
	void StartAnimation();
	void StopAnimation();

private:
	void ChangeText();				// ����� �ؽ�Ʈ�� ����;
	void UpdateText();				// �ؽ�Ʈ ���� ����;
	void UpdateTextString();		// �ؽ�Ʈ ��Ʈ�� �̵�;

private:
	CInnerInterface* m_pInterface;
	CBasicTextBox* m_pDescText;

	std::string m_originString;				// ���� �ؽ�Ʈ;
	std::string m_strDesc;					// ���� �ؽ�Ʈ;
	std::string m_changeString;				// ����� �ؽ�Ʈ ( ���� �ִϸ��̼ǿ� ���ŵ� );
	D3DCOLOR m_textColor;					// �ؽ�Ʈ ����;

	bool m_bAniDesc;						// �ִϸ��̼�;
	bool m_bOverTextLength;					// �ؽ�Ʈ ���̰� ���� ����;
	unsigned int m_nAniDescCur;				// ���� ��ġ;
	unsigned int m_nAniDescMax;				// �ִ� ��ġ;
	unsigned int m_nStrCur;					// ���� ����;
	unsigned int m_nStrMax;					// �ִ� ����;
	float m_fAniTime;						// �ð�;

	float m_fDESC_ANIMATION_SPEED_DELAY;	// �ؽ�Ʈ ���ǵ�;

	EMOVEABLE_TEXT_TYPE m_moveableType;		// �̵� Ÿ��;
};