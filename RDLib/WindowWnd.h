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
    void ResizeClient(int cx = -1, int cy = -1);
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
	�˳�ģ̬���ڿ���ʹ��EndModal���߻����Close����:

	1.EndModal��Postһ��WM_NULL��Ϣ,��ʹShowModal���˳���Ϣѭ��,
	ע��:EndModal�������WM_CLOSE,��������,ֱ�ӽ���ģ̬.

	2.Close����������WM_CLOSE�������п����Ƿ��˳�ģ̬,������˳�ģ̬,�����DialogImpleBase��ʵ��,���ڲ����õ���EndModal,
	��������˳�ģ̬,��������Ϣ,���벻�ܽ�����Ϣ����·�ɵ�ԭʼ���ڹ���,��Ϊ�⽫���´��ڱ�����,�������ڼ������ڶ���״̬.
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


