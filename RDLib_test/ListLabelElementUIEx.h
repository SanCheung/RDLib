#pragma once

enum ItemState
{
	ITEM_STATE_NORMAL,
	ITEM_STATE_HOT,
	ITEM_STATE_SEL
};

class CComboUIEx;
class CListLabelElementUIEx :
	public CListLabelElementUI
{
public:
	CListLabelElementUIEx(void);
	~CListLabelElementUIEx(void);

public:
	void DoEvent(TEventUI& event);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void SetInnerRect();
	bool DrawImage(CComboUIEx* pCombo, HDC hDC, RECT& rcDraw, LPCTSTR pStrModify = NULL);

	/*
	* FUNCTION:			ʧ��ͼƬ
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:
	*	true			ʧ��ͼƬ�ɹ�
	*	false			ͼƬ��ʧ��
	*/
	bool FreeImage();

	/*
	* FUNCTION:			ͼƬ����
	* INITIALIZATION:	���ַ��������ҷǿ�
	* PARAMETER(S):
	*	@strImage		ͼƬ����·��
	* RETURN:			GDI+ͼƬ����ָ��
	*/
 	Image* LoadImage(LPCWSTR strImage);

protected:
	/*
	* FUNCTION:			ȷ����ǰ���ƶ����״̬
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	ItemState GetItemState();

private:
	RECT			m_rcInner;
	Image*			m_image;			// GDI+ͼƬ����ָ��
};

/*
* FUNCTION:			...
* INITIALIZATION:	...
* PARAMETER(S):		...
* RETURN:			...
*/