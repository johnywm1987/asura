//	BasicScrollBarEx 클래스
//
//	최초 작성자 : 성기엽
//	이후 수정자 :
//	로그
//		[2003.12.1]
//			@ 최초제작

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