#ifndef __UIDLGBUILDER_H__
#define __UIDLGBUILDER_H__

#pragma once

//namespace DuiLib {

class IDialogBuilderCallback
{
public:
    virtual CControlUI* CreateControl(LPCTSTR pstrClass) = 0;
};


class RDLIB_API CDialogBuilder
{
public:
    CDialogBuilder();
    CControlUI* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
        CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL);

    CControlUI* CreateFromXML(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent);

	// old
	CControlUI* CreateFromXML2(IDialogBuilderCallback* pCallback = NULL, CPaintManagerUI* pManager = NULL,
		CControlUI* pParent = NULL);


    CMarkup* GetMarkup();

    void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
    void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;
private:
    CControlUI* _Parse(CMarkupNode* parent, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);

	void	_ParseFont( CMarkupNode* node, CPaintManagerUI *pManager );
	void	_ParseImage( CMarkupNode* node, CPaintManagerUI *pManager );
	void	_ParseDefault( CMarkupNode* node, CPaintManagerUI *pManager );
	//void	_ParseScript( CMarkupNode* node, CPaintManagerUI *pManager );


    CMarkup m_xml;
    IDialogBuilderCallback* m_pCallback;
    LPCTSTR m_pstrtype;
};

//} // namespace DuiLib

#endif // __UIDLGBUILDER_H__
