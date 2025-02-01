#pragma once


#include "../Lib_Engine/GUInterface/UIGroup.h"

class	CBasicTextBox;

class CReborntf : public CUIGroup
{
public:
	CReborntf();
	virtual	~CReborntf();

public:
	void	CreateSubControl();

public:
	virtual void	Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	CBasicTextBox* m_pRebornText;
	DWORD m_dwRebornBACK;
};