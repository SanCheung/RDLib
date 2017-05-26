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
	* FUNCTION:			失载图片
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:
	*	true			失载图片成功
	*	false			图片已失载
	*/
	bool FreeImage();

	/*
	* FUNCTION:			图片加载
	* INITIALIZATION:	宽字符串存在且非空
	* PARAMETER(S):
	*	@strImage		图片绝对路径
	* RETURN:			GDI+图片对象指针
	*/
 	Image* LoadImage(LPCWSTR strImage);

protected:
	/*
	* FUNCTION:			确定当前绘制对象的状态
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			...
	*/
	ItemState GetItemState();

private:
	RECT			m_rcInner;
	Image*			m_image;			// GDI+图片对象指针
};

/*
* FUNCTION:			...
* INITIALIZATION:	...
* PARAMETER(S):		...
* RETURN:			...
*/