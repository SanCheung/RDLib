#pragma once

// COMBO基本属性
#define FLAG_LLE_IMAGE_WIDTH		0
#define FLAG_LLE_IMAGE_HEIGHT		1
#define FLAG_LLE_HEIGHT				2
#define FLAG_LLE_GAP				3
#define FLAG_LLE_COUNT				4
#define FLAG_DB_MIN_WIDTH			6

// COMBO颜色属性
#define FLAG_CLR_SELECT				7
#define FLAG_CLR_HOT				8

// Combo属性
struct ComboProperty 
{
	ComboProperty() : wLLEImageWidth(160),
					  wLLEImageHeight(120),
					  wLLEHeight(136),
					  btLLEGap(8),
					  btLLECount(24),
					  wDBMinWidth(215),
// 					  dwClrShade1(0xff272520),
// 					  dwClrShade2(0xff989389),
					  dwClrSelected1(0x8ce39f67),
					  dwClrSelected2(0x8cbb6e0c),
					  dwClrHot1(0x8cffefc2),
					  dwClrHot2(0x8ccb9a3e)
	{
	}
	// 确定DropBox高度
	UINT16 GetDBHeight(UINT8 btDBPageCount = 4){  return (wLLEHeight + btLLEGap) * btDBPageCount;}

	// LLE属性
	UINT16	wLLEImageWidth;		// 图片高度: 默认160
	UINT16	wLLEImageHeight;	// 图片宽度：默认120
	UINT16	wLLEHeight;			// 书签高度: 默认130（注意：!!!LLE_HEIGHT必使大于120，因为LLE中除了图片之外，上下还要留有一定的距离）
	UINT8	btLLEGap;			// LLE之间的距离: 默认5
	UINT8	btLLECount;			// LLE数目：默认24
	// DropBox属性
	UINT16	wDBMinWidth;		// DropBox最小宽度：默认215
	// LLE颜色属性
// 	DWORD	dwClrShade1;		// 阴影颜色
// 	DWORD	dwClrShade2;
	DWORD   dwClrSelected1;		// 当前选中颜色
	DWORD   dwClrSelected2;
	DWORD   dwClrHot1;			// 鼠标选中颜色
	DWORD	dwClrHot2;
};

class CListLabelElementUIEx;
class CComboUIEx : public CComboUI
{
public:
	CComboUIEx(void);
	~CComboUIEx(void);

public:

	/*
	* FUNCTION:			展开下拉框
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			当前选中项索引
	*/
	int expand();

	/*
	* FUNCTION:			索引为i的LLE加载图片
	* INITIALIZATION:	默认iIndex >= 0且宽字符串存在且非空
	* PARAMETER(S):
	*	@iIndex			需要加载图片的LLE索引值
	*	@strImage		图片绝对路径
	* RETURN:			成功加载图片
	*/
	Image* LoadImage(int iIndex, LPCWSTR strImage);

	/*
	* FUNCTION:			初始化LLE
	* INITIALIZATION:	pItem = NULL 且 iIndex >= 0
	* PARAMETER(S):
	*	@iIndex			LLE索引值（根据iIndex对pItem进行初始化）
	*	@cbProperty		Combo属性
	*	@pItem			LLE指针
	* RETURN:			...
	*/
	void AddItem(int iIndex, CListLabelElementUIEx* pItem);
	
	void PaintText(HDC hDC);
	void DoEvent(TEventUI& event);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	bool SelectItem(int iIndex, bool bTakeFocus = false);

	UINT16 GetImageWidth() const{  return m_cbProperty.wLLEImageWidth;}			// 获取图片宽度
	UINT16 GetImageHeight() const{  return m_cbProperty.wLLEImageHeight;}		// 获取图片高度

	bool SetColorProperty(DWORD dwClr1, DWORD dwClr2, int flag);				// 设置颜色属性
	bool SetCBProperty(LPCVOID data, int flag);									// 设置COMBO基本属性(不包括颜色属性)
	void SetDBSize(UINT8 btPageCount = 4, UINT16 wDBMinWidth = 215);			// 设置DropBox大小
	
protected:
	bool IsExpand();
	void InitComboShadow(BYTE left, BYTE top, BYTE right, BYTE bottom);
	void UpdateItem(int iOldSel);

public:
	int				m_iCurShow;
	Rect			m_rcDB;
	ComboProperty	m_cbProperty;		// Combo属性
	bool			m_bItemSelect;		// 是否发送iteselect消息

protected:
	Region			m_shadowDBRgn;
	GraphicsPath	m_shadowDBPath;

#ifdef ZOOM_LOAD
public:
	bool InitSequence();		// 加载图片序列分配空间，并对其进行初始化
	bool DestroySequence();		// 加载图片序列释放空间

public:
	bool	m_bLoading;			// 正在加载图片
	int*	m_sequence;			// 加载图片序列
#endif
};

/*
* FUNCTION:			...
* INITIALIZATION:	...
* PARAMETER(S):		...
* RETURN:			...
*/
