#pragma once
#include "CustomListElementUI.h"

class CSQElementUI : public CCustomListElementUI
{
public:
	CSQElementUI(void);
	virtual ~CSQElementUI(void);

	static CSQElementUI* create( CListUI *list );

	void Init();
	void SetProgressValue( int v );

	bool OnButtonEvent( void* paramters );
	bool OnTextEvent( void* paramters );

private:
	CTextUI		*m_pTxtInfo;
	CButtonUI	*m_pBn;
	CProgressUI *m_pPr;
};

