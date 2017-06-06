#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

#ifdef _DEBUG
#ifndef TRACE
#define TRACE __Trace
#endif
#define TRACEMSG __TraceMsg
#else
#ifndef TRACE
#define TRACE
#endif
#define TRACEMSG _T("")
#endif

void	RDLIB_API __Trace(LPCTSTR pstrFormat, ...);
LPCTSTR RDLIB_API __TraceMsg(UINT uMsg);

/////////////////////////////////////////////////////////////////////////////////////
//

class STRINGorID
{
public:
	STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
	{ }
	STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
	{ }
	LPCTSTR m_lpstr;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class RDLIB_API CWaitCursor
{
public:
	CWaitCursor()
	{
		m_hOrigCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	}

	~CWaitCursor()
	{
		::SetCursor(m_hOrigCursor);
	}

protected:
	HCURSOR m_hOrigCursor;
};


/*
/////////////////////////////////////////////////////////////////////////////////////
//
class RDLIB_API CStdPtrArray
{
public:
    CStdPtrArray(int iPreallocSize = 0);
	CStdPtrArray(const CStdPtrArray& src);
    ~CStdPtrArray();

    void Empty();
    void Resize(int iSize);
    bool IsEmpty() const;
    int Find(LPVOID iIndex) const;
    bool Add(LPVOID pData);
    bool SetAt(int iIndex, LPVOID pData);
    bool InsertAt(int iIndex, LPVOID pData);
    bool Remove(int iIndex);
    int GetSize() const;
    LPVOID* GetData();

    LPVOID GetAt(int iIndex) const;
    LPVOID operator[] (int nIndex) const;

protected:
    LPVOID* m_ppVoid;
    int		m_nCount;
    int		m_nAllocated;
};
//*/


///////////////////////////////////////////////////////////////////////////////////////
////
//
//class RDLIB_API CStdValArray
//{
//public:
//    CStdValArray(int iElementSize, int iPreallocSize = 0);
//    ~CStdValArray();
//
//    void Empty();
//    bool IsEmpty() const;
//    bool Add(LPCVOID pData);
//    bool Remove(int iIndex);
//    int GetSize() const;
//    LPVOID GetData();
//
//    LPVOID GetAt(int iIndex) const;
//    LPVOID operator[] (int nIndex) const;
//
//protected:
//    LPBYTE m_pVoid;
//    int m_iElementSize;
//    int m_nCount;
//    int m_nAllocated;
//};

class RDLIB_API CWindowWnd
{
public:
    CWindowWnd();

	HWND GetHWND() const { return m_hWnd; }
	operator HWND() const{ return m_hWnd; }

    bool RegisterWindowClass();
    bool RegisterSuperclass();

    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
    HWND Subclass(HWND hWnd);
    void Unsubclass();
    void ShowWindow(bool bShow = true, bool bTakeFocus = false);
    void Close();
    void CenterWindow();
    void SetIcon(UINT nRes);

    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    //void ResizeClient(int cx = -1, int cy = -1);
	bool MoveWindow(int x, int y, int width, int height, bool repaint=true);


protected:
	virtual LPCTSTR GetWindowClassName() const = 0;
	virtual LPCTSTR GetSuperClassName() const;
    virtual UINT	GetClassStyle() const;

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void	OnFinalMessage(HWND hWnd);

    static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    HWND		m_hWnd;
    WNDPROC		m_OldWndProc;
    bool		m_bSubclassed;
};

//////////////////////////////////////////////////////////////////////////

class RDLIB_API CDialogWnd : public CWindowWnd
{
public:
	CDialogWnd();
	UINT ShowModal();				//show as modal window,when return the window has been destroyed

    /*
	退出模态窗口可以使用EndModal或者基类的Close方法:

	1.EndModal将Post一个WM_NULL消息,并使ShowModal的退出消息循环,
	注意:EndModal不会产生WM_CLOSE,就如其名,直接结束模态.

	2.Close函数可以在WM_CLOSE的例程中控制是否退出模态,如果想退出模态,则调用DialogImpleBase的实现,其内部调用的是EndModal,
	如果不想退出模态,则丢弃该消息,必须不能将此消息继续路由到原始窗口过程,因为这将导致窗口被销毁,而父窗口继续处于冻结状态.
	*/
	bool EndModal(UINT ret);		
protected:
	bool		m_ExitModal;
	UINT		m_DialogResult;
};

//////////////////////////////////////////////////////////////////////////

class RDLIB_API CHostableWnd : public CWindowWnd
{
public:
	friend class CControlUI;
	CHostableWnd()			{ m_pHost = NULL; }
	virtual ~CHostableWnd()	{}
	
	CControlUI* GetHostUI()				{ return m_pHost; }
	void SetHostUI(CControlUI* pHost)	{ m_pHost = pHost; }

protected:
	CControlUI*			m_pHost;
};


