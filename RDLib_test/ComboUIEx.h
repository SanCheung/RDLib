#pragma once

// COMBO��������
#define FLAG_LLE_IMAGE_WIDTH		0
#define FLAG_LLE_IMAGE_HEIGHT		1
#define FLAG_LLE_HEIGHT				2
#define FLAG_LLE_GAP				3
#define FLAG_LLE_COUNT				4
#define FLAG_DB_MIN_WIDTH			6

// COMBO��ɫ����
#define FLAG_CLR_SELECT				7
#define FLAG_CLR_HOT				8

// Combo����
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
	// ȷ��DropBox�߶�
	UINT16 GetDBHeight(UINT8 btDBPageCount = 4){  return (wLLEHeight + btLLEGap) * btDBPageCount;}

	// LLE����
	UINT16	wLLEImageWidth;		// ͼƬ�߶�: Ĭ��160
	UINT16	wLLEImageHeight;	// ͼƬ��ȣ�Ĭ��120
	UINT16	wLLEHeight;			// ��ǩ�߶�: Ĭ��130��ע�⣺!!!LLE_HEIGHT��ʹ����120����ΪLLE�г���ͼƬ֮�⣬���»�Ҫ����һ���ľ��룩
	UINT8	btLLEGap;			// LLE֮��ľ���: Ĭ��5
	UINT8	btLLECount;			// LLE��Ŀ��Ĭ��24
	// DropBox����
	UINT16	wDBMinWidth;		// DropBox��С��ȣ�Ĭ��215
	// LLE��ɫ����
// 	DWORD	dwClrShade1;		// ��Ӱ��ɫ
// 	DWORD	dwClrShade2;
	DWORD   dwClrSelected1;		// ��ǰѡ����ɫ
	DWORD   dwClrSelected2;
	DWORD   dwClrHot1;			// ���ѡ����ɫ
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
	* FUNCTION:			չ��������
	* INITIALIZATION:	...
	* PARAMETER(S):		...
	* RETURN:			��ǰѡ��������
	*/
	int expand();

	/*
	* FUNCTION:			����Ϊi��LLE����ͼƬ
	* INITIALIZATION:	Ĭ��iIndex >= 0�ҿ��ַ��������ҷǿ�
	* PARAMETER(S):
	*	@iIndex			��Ҫ����ͼƬ��LLE����ֵ
	*	@strImage		ͼƬ����·��
	* RETURN:			�ɹ�����ͼƬ
	*/
	Image* LoadImage(int iIndex, LPCWSTR strImage);

	/*
	* FUNCTION:			��ʼ��LLE
	* INITIALIZATION:	pItem = NULL �� iIndex >= 0
	* PARAMETER(S):
	*	@iIndex			LLE����ֵ������iIndex��pItem���г�ʼ����
	*	@cbProperty		Combo����
	*	@pItem			LLEָ��
	* RETURN:			...
	*/
	void AddItem(int iIndex, CListLabelElementUIEx* pItem);
	
	void PaintText(HDC hDC);
	void DoEvent(TEventUI& event);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	bool SelectItem(int iIndex, bool bTakeFocus = false);

	UINT16 GetImageWidth() const{  return m_cbProperty.wLLEImageWidth;}			// ��ȡͼƬ���
	UINT16 GetImageHeight() const{  return m_cbProperty.wLLEImageHeight;}		// ��ȡͼƬ�߶�

	bool SetColorProperty(DWORD dwClr1, DWORD dwClr2, int flag);				// ������ɫ����
	bool SetCBProperty(LPCVOID data, int flag);									// ����COMBO��������(��������ɫ����)
	void SetDBSize(UINT8 btPageCount = 4, UINT16 wDBMinWidth = 215);			// ����DropBox��С
	
protected:
	bool IsExpand();
	void InitComboShadow(BYTE left, BYTE top, BYTE right, BYTE bottom);
	void UpdateItem(int iOldSel);

public:
	int				m_iCurShow;
	Rect			m_rcDB;
	ComboProperty	m_cbProperty;		// Combo����
	bool			m_bItemSelect;		// �Ƿ���iteselect��Ϣ

protected:
	Region			m_shadowDBRgn;
	GraphicsPath	m_shadowDBPath;

#ifdef ZOOM_LOAD
public:
	bool InitSequence();		// ����ͼƬ���з���ռ䣬��������г�ʼ��
	bool DestroySequence();		// ����ͼƬ�����ͷſռ�

public:
	bool	m_bLoading;			// ���ڼ���ͼƬ
	int*	m_sequence;			// ����ͼƬ����
#endif
};

/*
* FUNCTION:			...
* INITIALIZATION:	...
* PARAMETER(S):		...
* RETURN:			...
*/
