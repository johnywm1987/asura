//	BasicScrollBarEx Ŭ����
//
//	���� �ۼ��� : ���⿱
//	���� ������ :
//	�α�
//		[2003.12.1]
//			@ ��������

#pragma	once

#include "./BasicTree.h"

class	CBasicTreeEx : public CBasicTree
{
public:
	CBasicTreeEx();
	virtual	~CBasicTreeEx();

public:
	//   void	CreateBaseTree ( char* szBaseTree );

public:
	CBasicTreeNode* InsertNode(CBasicTreeNode* pParent, CString strNodeName);
	CBasicTreeNode* InsertNodePVP(CBasicTreeNode* pParent, CString strNodeName, bool bLineBox = TRUE, bool bBOLD = TRUE);

private:
	UIGUID	m_NodeWndID;
};