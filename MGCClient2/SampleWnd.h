#pragma once
class CSampleWnd : public CWindowWnd
{
public:
	CSampleWnd(void);
	virtual ~CSampleWnd(void);

	virtual LPCTSTR GetWindowClassName() const
	{
		return L"sample_window_classname";
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

