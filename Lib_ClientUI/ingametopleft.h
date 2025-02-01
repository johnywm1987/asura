#pragma	once

#include "../Lib_Engine/GUInterface/UIGroup.h"

class	CBasicTextBox;

class Cingametopleft : public CUIGroup
{
public:
	Cingametopleft();
	virtual	~Cingametopleft();

public:
	void	CreateSubControl();

public:
	virtual void	Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	CBasicTextBox* m_pLevelText;
	WORD m_wLevelBACK;
};