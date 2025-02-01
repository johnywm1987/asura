//	������ ����
//
//	���� �ۼ��� : ���⿱
//	���� ������ : 
//	�α�
//		[2003.12.6]
//			@ �ۼ�
//

#pragma	once

#include "ItemSlot_Renew.h"

class	CItemSlotEx_Renew : public CItemSlot_Renew
{
public:
	CItemSlotEx_Renew ();
	virtual	~CItemSlotEx_Renew ();

public:
	void	CreateSubTradeInventory ( int nMaxColumn );
	void	CreateSubTrade ( int nMaxColumn );

private:
	CUIControl*	CreateFlipImage ( const char* szFlip );

public:
	void	SetFlipItem ( int nIndex, BOOL bFlip );
	BOOL	IsFlipItem ( int nIndex );

private:
	CUIControl*		m_pFlipImage[nLIMIT_COLUMN];
};