#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#pragma once

//namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//

class CControlUI;
class CPaintManagerUI;
class CEventSource;

typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

//class RDLIB_API CDataObject
//{
//public:
//	void* GetData(LPCTSTR pstrFormat)
//	{
//		return m_DataMap.Find(pstrFormat);
//	}
//
//	bool SetData(LPCTSTR pstrFormat, void* pData)
//	{
//		return m_DataMap.Insert(pstrFormat, pData);
//	}
//
//	bool GetDataPresent(LPCTSTR pstrFormat)
//	{
//		return m_DataMap.Find(pstrFormat) != NULL;
//	}
//private:
//	CStringPtrMap		m_DataMap;
//};


//typedef enum{
//	DropEffectDeny,
//	DropEffectAllow
//}DROPEFFECT;
//
//typedef enum{
//	DragStart, //点击拖动源,并且已调用DoDragDrop
//	DropAllow, //进入到一个目标,目标可以放
//	DropDeny   //目标不允许放
//}DRAGDROPSTEP;
//
//class RDLIB_API CDragEventArgs
//{
//public:
//	
//	CDragEventArgs(){
//		Effect = DropEffectDeny;
//		KeyState = 0;
//		pt.x = pt.y = 0;
//	}
//	//UINT AllowedEffect; //获取拖动事件的发起方（或源）所允许的拖放操作。 
//	CDataObject* Data; //获取 IDataObject，它包含与此事件关联的数据。 
//	DROPEFFECT Effect; //获取或设置拖放操作中目标放置效果。 
//	UINT KeyState; //获取 Shift、Ctrl 和 Alt 键的当前状态以及鼠标按钮的状态。 
//	POINT pt; //获取鼠标指针在屏幕坐标系中的 x 坐标。 
//};

//class RDLIB_API CDropTarget
//{
//public:
//	//virtual void OnDragDrop(CDragEventArgs* e){ e->Effect = DropEffectDeny;}  //drop into a control
//	////virtual void OnDragOver(CDragEventArgs* e){e->Effect = DenyDrop;}
//	//virtual void OnDragEnter(CDragEventArgs* e){e->Effect = DropEffectDeny;} //drag into a control
//	//virtual void OnDragLeave(CDragEventArgs* e){e->Effect = DropEffectDeny;} //drag leave the control that was previous get drag enter
//
//	//virtual DROPEFFECT DoDragDrop(CDataObject* data) = 0; //start the drag_drop operation
//	
//	CDropTarget()
//	{
//		m_AllowDrop = false;
//	}
//	bool m_AllowDrop;
//};


class RDLIB_API CControlUI //: public CDropTarget
{
public:
    CControlUI();
    virtual ~CControlUI();

public:
    virtual CStringW GetName() const;
    virtual void SetName(LPCTSTR pstrName);
	virtual LPCTSTR GetClass() const			{ return _T("ControlUI");}

    virtual LPVOID GetInterface(LPCTSTR pstrName);
    virtual UINT GetControlFlags() const;

    virtual bool Activate();
	virtual CPaintManagerUI* GetManager() const { return m_pManager; }
    virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
	virtual CControlUI* GetParent() const		{ return m_pParent; }

    // 文本相关
    virtual CStringW GetText() const;
    virtual void SetText(LPCTSTR pstrText);

    // 图形相关
    DWORD	GetBkColor() const;
    void	SetBkColor(DWORD dwBackColor);
    DWORD	GetBkColor2() const;
    void	SetBkColor2(DWORD dwBackColor);
    DWORD	GetBkColor3() const;
    void	SetBkColor3(DWORD dwBackColor);

    LPCTSTR GetBkImage();
    void	SetBkImage(LPCTSTR pStrImage);
    DWORD	GetBorderColor() const;
    void	SetBorderColor(DWORD dwBorderColor);
	DWORD	GetFocusBorderColor() const;
	void	SetFocusBorderColor(DWORD dwBorderColor);
   
	bool	IsColorHSL() const;
    void	SetColorHSL(bool bColorHSL);
    
	int		GetBorderSize() const;
    void	SetBorderSize(int nSize);

    SIZE	GetBorderRound() const;
    void	SetBorderRound(SIZE cxyRound);
    bool	DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

    // 位置相关
    virtual const RECT& GetPos() const;
    virtual void SetPos(RECT rc);
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual int GetX() const;
    virtual int GetY() const;
    virtual RECT GetPadding() const;
    virtual void SetPadding(RECT rcPadding); // 设置外边距，由上层窗口绘制
    virtual SIZE GetFixedXY() const;         // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedXY(SIZE szXY);      // 仅float为true时有效
    virtual int GetFixedWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedWidth(int cx);      // 预设的参考值
    virtual int GetFixedHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
    virtual void SetFixedHeight(int cy);     // 预设的参考值
    virtual int GetMinWidth() const;
    virtual void SetMinWidth(int cx);
    virtual int GetMaxWidth() const;
    virtual void SetMaxWidth(int cx);
    virtual int GetMinHeight() const;
    virtual void SetMinHeight(int cy);
    virtual int GetMaxHeight() const;
    virtual void SetMaxHeight(int cy);
    virtual void SetRelativePos(SIZE szMove,SIZE szZoom);
    virtual void SetRelativeParentSize(SIZE sz);
    virtual TRelativePosUI GetRelativePos() const;
    virtual bool IsRelativePos() const;

    // 鼠标提示
    virtual CStringW GetToolTip() const;
    virtual void SetToolTip(LPCTSTR pstrText);

    // 快捷键
    virtual TCHAR GetShortcut() const;
    virtual void SetShortcut(TCHAR ch);

    // 菜单
    virtual bool IsContextMenuUsed() const;
    virtual void SetContextMenuUsed(bool bMenuUsed);

    // 用户属性
    virtual const CStringW& GetUserData(); // 辅助函数，供用户使用
    virtual void SetUserData(LPCTSTR pstrText); // 辅助函数，供用户使用
    virtual UINT_PTR GetTag() const; // 辅助函数，供用户使用
    virtual void SetTag(UINT_PTR pTag); // 辅助函数，供用户使用

    // 一些重要的属性
    virtual bool IsVisible() const;
    virtual void SetVisible(bool bVisible = true);
    virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
    virtual bool IsEnabled() const;
    virtual void SetEnabled(bool bEnable = true);
    virtual bool IsMouseEnabled() const;
    virtual void SetMouseEnabled(bool bEnable = true);
    virtual bool IsKeyboardEnabled() const;
    virtual void SetKeyboardEnabled(bool bEnable = true);
    virtual bool IsFocused() const;
    virtual void SetFocus();
    virtual bool IsFloat() const;
    virtual void SetFloat(bool bFloat = true);
	virtual void SetBorderState(int state);

    virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

    void Invalidate();
    bool IsUpdateNeeded() const;
    void NeedUpdate();
    void NeedParentUpdate();
    DWORD GetAdjustColor(DWORD dwColor);

    virtual void Init();
    virtual void DoInit();

    virtual void Event(TEventUI& event);
    virtual void DoEvent(TEventUI& event);

    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    CControlUI* ApplyAttributeList(LPCTSTR pstrList);

    virtual SIZE EstimateSize(SIZE szAvailable);

    virtual void DoPaint(HDC hDC, const RECT& rcPaint);
    virtual void PaintBkColor(HDC hDC);
    virtual void PaintBkImage(HDC hDC);
    virtual void PaintStatusImage(HDC hDC);
    virtual void PaintText(HDC hDC);
    virtual void PaintBorder(HDC hDC);

    virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

	//////////////////////////////////////////////////////////////////////////extends
	//DROPEFFECT DoDragDrop(CDataObject* data);
	//virtual HCURSOR GetFeedback( DRAGDROPSTEP step );


	// 20160319
	static DWORD	FormatToColor( LPCTSTR p );
	static RECT		FormatToRect( LPCTSTR p );

public:
    CEventSource OnInit;
    CEventSource OnDestroy;
    CEventSource OnSize;
    CEventSource OnEvent;
    CEventSource OnNotify;

protected:
    CPaintManagerUI* m_pManager;
    CControlUI* m_pParent;
    CStringW m_sName;
    bool m_bUpdateNeeded;
    bool m_bMenuUsed;
    RECT m_rcItem;
    RECT m_rcPadding;
    SIZE m_cXY;
    SIZE m_cxyFixed;
    SIZE m_cxyMin;
    SIZE m_cxyMax;
    bool m_bVisible;
    bool m_bInternVisible;
    bool m_bEnabled;
    bool m_bMouseEnabled;
	bool m_bKeyboardEnabled ;
    bool m_bFocused;
    bool m_bFloat;
    bool m_bSetPos; // 防止SetPos循环调用
    TRelativePosUI m_tRelativePos;

    CStringW m_sText;
    CStringW m_sToolTip;
    TCHAR m_chShortcut;
    CStringW m_sUserData;
    UINT_PTR m_pTag;

    DWORD m_dwBackColor;
    DWORD m_dwBackColor2;
    DWORD m_dwBackColor3;
    CStringW m_sBkImage;
    DWORD m_dwBorderColor;
	DWORD m_dwFocusBorderColor;
    bool m_bColorHSL;
    int m_nBorderSize;
	int m_nBorderState; //画控件边框状态,15四个边都画(默认值), 1顶 2右 4底 8左
    SIZE m_cxyBorderRound;
    RECT m_rcPaint;
	////////////////////////////////////////////////////////////////////////// extends
	
};

//} // namespace DuiLib

#endif // __UICONTROL_H__
