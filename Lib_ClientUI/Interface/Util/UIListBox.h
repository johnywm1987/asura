#pragma once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "../../../EngineLib/GUInterface/UIControlMessage.h"

class CUIGroup;
class EngineDeviceMan;
class CBasicScrollBarEx;
class CBasicLineBoxSmart;

/**
	�⺻ ��Ʈ��(CUIControl) ����� ����Ʈ �ڽ��� ���� �Ѵ�.

    �⺻ ��Ʈ�ѷ� ǥ�ð����� �κ��� ��� ����Ʈ �ڽ�ȭ �Ҽ� �ִ�.
    ������� �̹���(�ؽ�ó) ��� ����Ʈ �ڽ��� ���� �� �ִ�.

    ����Ʈ �ڽ����� ����Ʈ�� ���� �ϰ� �Ǹ� �θ� �޽����� ���� �ȴ�.

 */
class CUIListBox : public CUIGroup
{
public:
		CUIListBox(EngineDeviceMan* pEngineDevice);
virtual ~CUIListBox();

		typedef std::vector<CUIControl*>	ControlContainer;
		typedef ControlContainer::iterator	ControlContainerIterator;

        // ���콺�� ���� ���� ���� �� �θ� ���� �޽���
        static const DWORD  LB_SELCHANGED = 0x01000000;

		/**
			����Ʈ �ڽ��� ���� �Ѵ�.
			���ڿ����� ��Ʈ���� ���¸� �����ϴ� XML�� Control ID �̴�.

			\param ctrl ����Ʈ �ڽ� ��Ʈ�� ���� XML
			\param outline ���� ���� �ؽ�ó ���� XML, 0 �� ��� ����
			\param scroll ��ũ�� ���� XML, 0 �� ��� ����
			\param fLine �� ����
			\param fFront �� ����
			\param fBack  �� ����
			\return ���� ���� ����
		 */
virtual bool				Create(const TCHAR* ctrl, const TCHAR* outline = 0, const TCHAR* scroll = 0, const float fLine = 0.0f, const float fFront = 0.0f, const float fBack = 0.0f);

		/**
			�� ������ ȣ�� �Ǿ� ó���� ������ ó�� �Ѵ�.
			�츮�� ��ũ�ѿ��� �̺�Ʈ�� �߻��ϸ� TranlateUIMessage �� ���޵��� �����Ƿ�
			Update ���� ��ũ�� ������ ���Ǹ鼭 ����� �̿� �°� ���� ������ ó�� �Ѵ�.

		 */
virtual void				Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

		/**
			����Ʈ ������ ���� �Լ�

		 */
virtual void				AddItem(const TCHAR* item, ...);
virtual void				SetItem(int nIndex, const TCHAR* item, ...);
virtual void				DelItem(int nIndex);
virtual void				ClearItem();

		/**
			���� ���� �Լ�
			���� �̱� �Ƿ��Ǹ� ���������� ���� ��Ƽ �Ƿ��ǵ� �����ϸ� ���� ���̴�.

		 */
virtual int					GetCurSel() const { return m_nSelectedLine; }
virtual int                 GetTotalLine() const { return m_nTotalLine; }
virtual void				SetCurSel(int nSelect);

		/**
			�� ��Ʈ�� ������ ���־� ������ ���� ������ true �� ���� �Ѵ�.
			�������̳� �Ƚ� ���߱� ������ Ȱ�� �Ѵ�.

		 */
		void				SetDebugLineBox(bool bDebug) { m_bDebug = bDebug; }
        
		/**
			�ش� �ε����� ��Ʈ���� visible �� ���� �Ѵ�.

		 */
		void 		        SetVisible(int nIndex, bool bVisible);
        bool                GetVisible(int nIndex);

protected:
		/**
			��Ʈ���� �����.
			�ߺ� �ڵ带 ���� �Ѵ�.

			\param pControl ��Ʈ�� XML
			\return ������ ��Ʈ�� ����Ʈ, ���н� NULL
		 */
		CUIControl*			CreateControl(const TCHAR* pControl);

		/**
			��Ʈ���� ��ġ�� ���� �Ѵ�.
			
		 */
		void				SetPosControl(CUIControl* pControl);

		/**
			�ε����� �ش��ϴ� ��Ʈ���� ��´�.

		 */
		CUIControl* 		GetControl(int nIndex);

		/**
			��Ʈ�ѷ� ���� ������ ���� �Ѵ�.
			���� ����ӿ� ���� �Ѵ�.

		 */
		const UIRECT		GetRectLineFromControl(int nLine);

		/**
			������ ������ ���Ѵ�.
			������ ���� ��� ��ǥ�ӿ� ���� �Ѵ�.

		 */
		const UIRECT		GetRectLine(int nLine);
		const UIRECT		GetRectLine(int nLine, CUIControl* pControl);

		/**
			���� ��Ʈ�� ���� �Լ���

		 */
virtual bool				CreateCtrl(const TCHAR* ctrl);
virtual bool				CreateScroll(const TCHAR* scroll);
virtual bool				CreateLineBox(const TCHAR* outline);
		
		/**
			���� ���콺 ��ġ�� ���õ� �������� ã�´�.

			\return ���� ���� ����Ǿ����Ƿ� �����ؾ� �ϴ� ����
		 */
virtual bool				UpdateMousePos(int x, int y);

		/**
			������ ����, ���� �� ������ ���濡 ���� ��ũ�ѽ��� ���� �Ѵ�.

		 */
virtual void				UpdateScrollThumb();

		/**
			��ũ�� �Ǿ����� ���� �Ǻ�

		 */
virtual bool				UpdateIsScrolled();

		/**
			��ũ�� �Ǿ��� ��� ó�� ����

		 */
virtual void				UpdateBeScrolled();

		/**
			���� ������ ���� �Ѵ�.
			������ Ŭ���� ���� ���� ������ ����Ǹ� ���� ������ ���� �� �ش�.

		 */
virtual void				UpdateSelected();

		/**
			������ ����(�߰�, ����)�� ���� �������� ������ ������ ó�� �Ѵ�.

		 */
virtual void				UpdateVisibleLine();

		/**
			�������� ù��° ������ �ε����� ���� �Ѵ�.

		 */
virtual void				UpdateVisibleLineFirstIdx();

		/**
			�������� ���ε��� ��ġ�� ���� �ϰ� ��� ��������(visible) ó�� �Ѵ�.

		 */
virtual void				UpdateVisibleLinePosWithVisible();

		/**
			���� ���̴� ������ �Ⱥ��̰� or ���̰� �Ѵ�.

		 */
virtual void				UpdateVisibleLineToVisible();
virtual void				UpdateVisibleLineToUnVisible();

		/**
			��ǥ ���ߴ� �۾��� ���Ǹ� ���� ��ü ����, �ؽ�Ʈ �ڽ����� ������ ǥ�� �Ѵ�.
			�̸� ���� ��ǥ�� ���߰ų� ���� ������ ������ ��� �ϵ��� �Ѵ�.

		 */
		void				CreateDebugLineBox(CUIControl* pControl);
#ifndef RELEASE
		bool				m_bDebug;
#endif

		/**
			��Ʈ���� GlobalPos ��ġ�� ���� �Ѵ�.
			���� ������ GlobalPos �� �����ϰ� �Ǹ� �θ� ��� ��ġ ���Žÿ�
			LocalPos �� ��ġ�� �����ϴµ� �̷���� ���� ���ϴ� GlobalPos �� �ȵǹǷ�
			�θ� ��� LocalPos �� ���� �Է��� �־�� �Ѵ�.

		 */
		void				SetControlGlobalPos(CUIControl* pCtrl, const D3DXVECTOR2& rtGlobalPos);
		void				SetControlGlobalRect(CUIControl* pCtrl, const UIRECT& rtGlobalRect);

		/**
			��Ʈ��

		 */
		ControlContainer	m_kControls;		//@ ��Ʈ�� �����͵�
		TSTRING				m_strControl;		//@ ��Ʈ�� XML
		CBasicLineBoxSmart*	m_pSeleted;			//@ ���� ���� ��Ʈ��
		CBasicScrollBarEx*	m_pScrollBar;		//@ ��ũ�� ��Ʈ��


        /**
            ��Ʈ�� visible ����

         */
        typedef std::map<int, bool>	CtrlVisContainer;
        CtrlVisContainer    m_kVisible;          //@ ��Ʈ�ѵ� visible ����

		/**
			��ũ�� ����

		 */	
		float 				m_fPercent;			//@ ��ũ�� ���� �ۼ�Ƽ��

		/**
			���� ����

		 */	
		int					m_nSelectedLine;	//@ ���� �� ���� �ε���(������ -1)

		/**
			���� ����

		 */
		int					m_nTotalLine;		//@ ��� ���� ��
		int					m_nVisibleLine;		//@ �������� ���� ��
		int					m_nVisibleFirstIdx;	//@ �������� ������ ù��° ���� �ε���

		/**
			���� ����

		 */
		float				m_fLineInterval;	//@ ���� ���� ���� ����
		float				m_fFrontInterval;	//@ ���� ���� �� ����
		float				m_fBackInterval;	//@ ���� ���� �� ����
};
