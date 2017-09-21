#pragma once
class CLayeredDialogBase : public CDialogBase
{
public:
	CLayeredDialogBase(void);
	virtual ~CLayeredDialogBase(void);

	virtual void	Init();
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void	layerDraw();
};

