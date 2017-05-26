#pragma once
class CServiceQueryELementUI : public CListContainerElementUI
{
public:
	CServiceQueryELementUI(void);
	virtual ~CServiceQueryELementUI(void);

	bool InitItem();
	SIZE EstimateSize(SIZE szAvailable);

	bool OnButtonEvent(void* paramters);
	bool OnTextEvent(void* paramters);

	void SetInfo( LPCWSTR strSeatNo, LPCWSTR strName, LPCWSTR strTime, LPCWSTR strContent);

	void SetPos(RECT rc);

	void SetProgressValue( int v );

	CListHeaderUI *m_pHeader;

private:
	CTextUI		*m_pTxtInfo;
	CButtonUI	*m_pBn;
	CHorizontalLayoutUI *m_pHL;
	CProgressUI *m_pPr;

};

