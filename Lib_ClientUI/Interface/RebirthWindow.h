#pragma	once

#include "UIWindowEx.h"

class CInnerInterface;
class GLGaeaClient;
class CBasicLineBoxSmart;
class CBasicTextBox;
class CBasicButton;
class RnButton;
class CRebirthWindowButton;

class CRebirthWindow : public CUIWindowEx
{

private:
    enum
    {
        REBIRTH_WINDOW_BACK = ET_CONTROL_NEXT,
        AUTO_REBIRTH_CHECK,
        HELP_BUTTON,

        CHECK_POINT_BUTTON,
        RECOVERY_EXP_BUTTON,
        CURRENT_POS_BUTTON,
        SKILL_REBIRTH_BUTTON,

        BUY_BUTTON,
    };

    enum
    {
        REBIRTH_NONE,
        REBIRTH_CHECKPOINT,
        REBIRTH_EXP,
        REBIRTH_CURRENT_POSITION,
        REBIRTH_SKILL,
    };

public:
    CRebirthWindow();
    virtual	~CRebirthWindow();

protected:
    GLGaeaClient* m_pGaeaClient;
    CInnerInterface* m_pInterface;

private:
    CBasicLineBoxSmart* m_pNotifyBack;
    CBasicLineBoxSmart* m_pNotifyBack2;
    CBasicLineBoxSmart* m_pRecommandBack;
    CBasicLineBoxSmart* m_pNotifyMain;
    CUIControl* m_pNotifyMainImage;

    CBasicTextBox* m_pRebirthQuestionText;
    CBasicTextBox* m_pRebirthInfoText;

    CUIControl* m_pAutoRebirthCheck;
    CUIControl* m_pAutoRebirthUnCheck;
    CBasicTextBox* m_pAutoRebirthText;

    CRebirthWindowButton* m_pTempLeftButton;
    CRebirthWindowButton* m_pTempRightButton;

    CRebirthWindowButton* m_pCheckPointRebirthButton;
    CRebirthWindowButton* m_pExpRebirthButton;
    CRebirthWindowButton* m_pCurrentPosRebirthButton;
    CRebirthWindowButton* m_pSkillRebirthButton;

    CUIControl* m_pHelpButton;

    CBasicLineBoxSmart* m_pTooltipBack;
    CBasicTextBox* m_pTooltipText;

    CItemImage* m_pRecommendItem;
    CBasicLineBoxSmart* m_pRecommendItemBack;
    CBasicTextBox* m_pRecommendItemText;
    RnButton* m_pBuyButton;

private:
    bool m_bAutoRebirth;
    bool m_bReviveSkill;

    float m_fTIME_LEFT;
    int m_nACTION;
    int m_nDONE_ACTION;

    float m_fAutoRebirthTime;

    LONGLONG m_longDecExp;
    LONGLONG m_longReExp;
    LONGLONG m_longDecMoney;
    LONGLONG m_longDecReExp;
    std::string m_strCheckPoint;
    
private:
    void ShowAllButton();
    void UnShowAllButton();
    void NotifyRebirthLossInformation();

    void GetExpRecovery( LONGLONG& _longDecExp, LONGLONG& _longReExp, LONGLONG& _longDecMoney );

    void PointshopOpenRecommendItem();

    void SetRecommendItemImage();

    void PressCheckAutoRebirth();

    /**
        UI �� ������ ������ ���ϰ� �ϱ� ����
        ĳ���Ϳ� 4���� ��Ȱ ���¸� �߰� �ϰ�
        �̿� ���� ���¿� ���� UI ������(Visible) ��� �Ѵ�.

     */
    void PostSetItemRebirth();

public:
    /**
        ��Ȱ â�� ���� ���� �Ǻ� �Ѵ�.
        ��� Ư���� ���� ���°� �־� �̸� �Ǻ��ϱ� ���� �뵵

     */
    bool CapableVisible();

    void CreateSubControl ();

    void SetItemRebirth( bool bItemRebirth, bool bAutoRebirth, bool bCantUseRebirth );
    void SetReviveSkill( bool bAct );

    void CheckPointRebirth();
    void RecoveryExpRebirth();
    void CurrentPosRebirth();
    void SKillRebirth();

    void SetAutoRebirthTime( UINT iTime );

	void	SetCallerID( UIGUID ControlID )		{ m_CallerID = ControlID; }
	UIGUID	GetCallerID()						{ return m_CallerID; }

private:
	UIGUID	m_CallerID;

public:
    virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
    virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
    virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
};
/*
class MyRebirthWindow : public IRebirthWindow, private CRebirthWindow
{
public:
    MyRebirthWindow( GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice );
    virtual ~MyRebirthWindow() {};

public:
    virtual void CreateUIWindowAndRegisterOwnership();

    virtual bool CapableVisible();
    virtual void SetItemRebirth( bool bItemRebirth, bool bAutoRebirth, bool bCantUseRebirth );
    virtual void SetReviveSkill( bool bAct );
    virtual void SetAutoRebirthTime( UINT iTime );
};*/