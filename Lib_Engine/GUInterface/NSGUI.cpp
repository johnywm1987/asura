#include "stdafx.h"

#include "NSGUI.h"

namespace NSGUI
{
    // �������̽� ���� ĳ������ �������� ����
    BOOL m_bCharMoveBlock(FALSE);
	BOOL m_bCharMoveAllBlock(FALSE);


    BOOL IsCharMoveBlock()
    { 
        return m_bCharMoveBlock;
    }

	void SetCharMoveBlock()	
    { 
        m_bCharMoveBlock = TRUE; 
    }

	void ResetCharMoveBlock()	
    { 
        m_bCharMoveBlock = FALSE;
    }

	BOOL IsCharMoveAllBlock()
	{ 
		return m_bCharMoveAllBlock;
	}

	void SetCharMoveAllBlock()	
	{ 
		m_bCharMoveAllBlock = TRUE; 
	}

	void ResetCharMoveAllBlock()	
	{ 
		m_bCharMoveAllBlock = FALSE;
	}
}